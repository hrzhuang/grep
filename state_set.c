#include <stdlib.h>

#include "index_vec.h"
#include "subset.h"

#include "state_set.h"

struct state_set {
    struct subset *set;
    struct index_vec *vec;
};

struct state_set *state_set_new(size_t size) {
    struct state_set *set = malloc(sizeof *set);
    set->set = subset_new(size);
    set->vec = index_vec_new();
    return set;
}

void state_set_del(struct state_set *set) {
    subset_del(set->set);
    index_vec_del(set->vec);
    free(set);
}

bool state_set_has(struct state_set *set, size_t index) {
    return subset_has(set->set, index);
}

size_t state_set_size(struct state_set *set) {
    return index_vec_len(set->vec);
}

size_t state_set_get(struct state_set *set, size_t index) {
    return index_vec_get(set->vec, index);
}

void state_set_add(struct state_set *set, size_t index) {
    if (subset_has(set->set, index))
        return;
    subset_add(set->set, index);
    index_vec_app(set->vec, index);
}
