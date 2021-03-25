#include <stdbool.h>
#include <stdlib.h>

#include "index_vec.h"
#include "state_set.h"
#include "state_vec.h"
#include "str.h"
#include "subset.h"

#include "re.h"

struct re {
    struct state_vec *states;
    size_t start;
    size_t accept;
};

struct state {
    enum {
        NO_TRANS,
        SINGLE_EPS,
        DOUBLE_EPS,
        EXPECT_CHAR,
        EXPECT_ANY,
        EXPECT_START,
        EXPECT_END,
    } trans_type;
    union {
        size_t dest;
        size_t dests[2];
    };
    char expected_char;
};

static struct state *alloc_state(struct state_vec *states, size_t *index) {
    struct state *state = malloc(sizeof *state);
    state_vec_app(states, state);
    *index = state_vec_len(states) - 1;
    return state;
}

struct frag {
    size_t start;
    size_t accept;
};

static void frag_any(struct state_vec *states, struct frag *frag) {
    struct state *start = alloc_state(states, &frag->start);
    alloc_state(states, &frag->accept);
    start->trans_type = EXPECT_ANY;
    start->dest = frag->accept;
}

static void frag_asterate(struct state_vec *states, struct frag *frag) {
    size_t new_start_ind, new_accept_ind;
    struct state *new_start = alloc_state(states, &new_start_ind);
    alloc_state(states, &new_accept_ind);

    new_start->trans_type = DOUBLE_EPS;
    new_start->dests[0] = frag->start;
    new_start->dests[1] = new_accept_ind;

    struct state *accept = state_vec_get(states, frag->accept);
    accept->trans_type = DOUBLE_EPS;
    accept->dests[0] = frag->start;
    accept->dests[1] = new_accept_ind;

    frag->start = new_start_ind;
    frag->accept = new_accept_ind;
}

static void frag_link(struct state_vec *states, struct frag *frag1,
        struct frag *frag2) {
    struct state *accept = state_vec_get(states, frag1->accept);
    accept->trans_type = SINGLE_EPS;
    accept->dest = frag2->start;
}

static void parse_atom(struct state_vec *states, const struct str *str,
        size_t *pos, struct frag *frag) {
    if (*pos == str_len(str)) {
        struct state *start = alloc_state(states, &frag->start);
        alloc_state(states, &frag->accept);
        start->trans_type = SINGLE_EPS;
        start->dest = frag->accept;
        return;
    }
    switch (str_get(str, *pos)) {
        // case ')':
        case '|':
        {
            struct state *start = alloc_state(states, &frag->start);
            alloc_state(states, &frag->accept);
            start->trans_type = SINGLE_EPS;
            start->dest = frag->accept;
            break;
        }
        case '^': {
            ++*pos;
            struct state *start = alloc_state(states, &frag->start);
            alloc_state(states, &frag->accept);
            start->trans_type = EXPECT_START;
            start->dest = frag->accept;
            break;
        }
        case '$': {
            ++*pos;
            struct state *start = alloc_state(states, &frag->start);
            alloc_state(states, &frag->accept);
            start->trans_type = EXPECT_END;
            start->dest = frag->accept;
            break;
        }
        case '.': {
            ++*pos;
            frag_any(states, frag);
            break;
        }
        // todo: add escape sequences
        // todo: add parenthesized group
        default: {
            struct state *start = alloc_state(states, &frag->start);
            alloc_state(states, &frag->accept);
            start->trans_type = EXPECT_CHAR;
            start->dest = frag->accept;
            start->expected_char = str_get(str, *pos);
            ++*pos;
        }
    }
}

static void parse_asterate(struct state_vec *states, const struct str *str,
        size_t *pos, struct frag *frag) {
    parse_atom(states, str, pos, frag);
    if (*pos < str_len(str) && str_get(str, *pos) == '*') {
        do
            ++*pos;
        while (*pos < str_len(str) && str_get(str, *pos) == '*');
        frag_asterate(states, frag);
    }
}

static void parse_concat(struct state_vec *states, const struct str *str,
        size_t *pos, struct frag *frag) {
    parse_asterate(states, str, pos, frag);
    while (
        *pos < str_len(str)
        && str_get(str, *pos) != '|'
        // && str_get(str, *pos) != ')'
    ) {
        struct frag another;
        parse_asterate(states, str, pos, &another);
        frag_link(states, frag, &another);
        frag->accept = another.accept; 
    }
}

static void parse_union(struct state_vec *states, const struct str *str,
        size_t *pos, struct frag *frag) {
    parse_concat(states, str, pos, frag);
    while (*pos < str_len(str) && str_get(str, *pos) == '|') {
        ++*pos;
        struct frag operand1 = *frag;
        struct frag operand2;
        parse_concat(states, str, pos, &operand2);

        struct state *new_start = alloc_state(states, &frag->start);
        alloc_state(states, &frag->accept);
        new_start->trans_type = DOUBLE_EPS;
        new_start->dests[0] = operand1.start;
        new_start->dests[1] = operand2.start;

        struct state *op1_accept = state_vec_get(states, operand1.accept);
        op1_accept->trans_type = SINGLE_EPS;
        op1_accept->dest = frag->accept;

        struct state *op2_accept = state_vec_get(states, operand2.accept);
        op2_accept->trans_type = SINGLE_EPS;
        op2_accept->dest = frag->accept;
    }
}

struct re *re_compile(struct str *str) {
    struct state_vec *states = state_vec_new();
    size_t pos = 0;
    struct frag frag;
    parse_union(states, str, &pos, &frag);

    if (pos != str_len(str)) {
        state_vec_del(states);
        return NULL;
    }

    struct frag prelude;
    frag_any(states, &prelude);
    frag_asterate(states, &prelude);

    struct frag appendix;
    frag_any(states, &appendix);
    frag_asterate(states, &appendix);

    frag_link(states, &prelude, &frag);
    frag_link(states, &frag, &appendix);
    state_vec_get(states, appendix.accept)->trans_type = NO_TRANS;

    struct re *re = malloc(sizeof *re);
    re->states = states;
    re->start = prelude.start;
    re->accept = appendix.accept;

    return re;
}

void re_del(struct re *re) {
    state_vec_del(re->states);
    free(re);
}

bool re_match(struct re *re, struct str *str) {
    struct state_set *set = state_set_new(state_vec_len(re->states));
    state_set_add(set, re->start);

    for (size_t i = 0; i < state_set_size(set); ++i) {
        const struct state *state =
            state_vec_get(re->states, state_set_get(set, i));
        switch (state->trans_type) {
            case SINGLE_EPS: {
                state_set_add(set, state->dest);
                break;
            }
            case DOUBLE_EPS: {
                state_set_add(set, state->dests[0]);
                state_set_add(set, state->dests[1]);
                break;
            }
            case EXPECT_START: {
                state_set_add(set, state->dest);
                break;
            }
        }
    }

    for (size_t pos = 0; pos < str_len(str); ++pos) {
        const char c = str_get(str, pos);
        struct state_set *new_set = state_set_new(state_vec_len(re->states));

        for (size_t i = 0; i < state_set_size(set); ++i) {
            const struct state *state =
                state_vec_get(re->states, state_set_get(set, i));
            switch (state->trans_type) {
                case EXPECT_CHAR: {
                    if (c == state->expected_char)
                        state_set_add(new_set, state->dest);
                    break;
                }
                case EXPECT_ANY: {
                    state_set_add(new_set, state->dest);
                    break;
                }
            }
        }

        state_set_del(set);
        set = new_set;

        for (size_t i = 0; i < state_set_size(set); ++i) {
            const struct state *state =
                state_vec_get(re->states, state_set_get(set, i));
            switch (state->trans_type) {
                case SINGLE_EPS: {
                    state_set_add(set, state->dest);
                    break;
                }
                case DOUBLE_EPS: {
                    state_set_add(set, state->dests[0]);
                    state_set_add(set, state->dests[1]);
                    break;
                }
            }
        }
    }

    for (size_t i = 0; i < state_set_size(set); ++i) {
        const struct state *state =
            state_vec_get(re->states, state_set_get(set, i));
        switch (state->trans_type) {
            case SINGLE_EPS: {
                state_set_add(set, state->dest);
                break;
            }
            case DOUBLE_EPS: {
                state_set_add(set, state->dests[0]);
                state_set_add(set, state->dests[1]);
                break;
            }
            case EXPECT_END: {
                state_set_add(set, state->dest);
                break;
            }
        }
    }

    bool matched = state_set_has(set, re->accept);
    state_set_del(set);

    return matched;
}
