#include <stdlib.h>

#include "subset.h"

struct subset {
    char *bits;
};

struct subset *subset_new(size_t size) {
    size_t bytes = size % 8 ? size / 8 + 1 : size / 8;
    char *bits = malloc(bytes * sizeof *bits);
    for (size_t i = 0; i < bytes; ++i)
        bits[i] = 0;
    struct subset *set = malloc(sizeof *set);
    set->bits = bits;
    return set;
}

void subset_del(struct subset *set) {
    free(set->bits);
    free(set);
}

bool subset_has(struct subset *set, size_t index) {
    return set->bits[index / 8] & 1 << index % 8;
}

void subset_add(struct subset *set, size_t index) {
    set->bits[index / 8] |= 1 << index % 8;
}
