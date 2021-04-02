#include <stdlib.h>

#include "state.h"
#include "state_vec.h"

struct state_vec {
    struct state **arr;
    size_t cap;
    size_t len;
};

struct state_vec *state_vec_new(void) {
    size_t cap = 1;
    struct state **arr = malloc(cap * sizeof *arr);
    struct state_vec *vec = malloc(sizeof *vec);
    vec->arr = arr;
    vec->cap = cap;
    vec->len = 0;
    return vec;
}

void state_vec_del(struct state_vec *vec) {
    for (size_t i = 0; i < vec->len; ++i) {
        state_del(vec->arr[i]);
    }
    free(vec->arr);
    free(vec);
}

struct state *state_vec_get(const struct state_vec *vec, size_t index) {
    return vec->arr[index];
}

size_t state_vec_len(const struct state_vec *vec) {
    return vec->len;
}

void state_vec_app(struct state_vec *vec, struct state *state) {
    if (vec->len == vec->cap) {
        size_t new_cap = vec->cap * 2;
        struct state **new_arr = malloc(new_cap * sizeof *new_arr);
        for (size_t i = 0; i < vec->len; ++i)
            new_arr[i] = vec->arr[i];
        free(vec->arr);
        vec->arr = new_arr;
        vec->cap = new_cap;
    }
    vec->arr[vec->len++] = state;
}
