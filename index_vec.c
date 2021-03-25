#include <stdlib.h>

#include "index_vec.h"

struct index_vec {
    size_t *arr;
    size_t cap;
    size_t len;
};

struct index_vec *index_vec_new(void) {
    size_t cap = 1;
    size_t *arr = malloc(cap * sizeof *arr);
    struct index_vec *vec = malloc(sizeof *vec);
    vec->arr = arr;
    vec->cap = cap;
    vec->len = 0;
    return vec;
}

void index_vec_del(struct index_vec *vec) {
    free(vec->arr);
    free(vec);
}

void index_vec_app(struct index_vec *vec, size_t index) {
    if (vec->len == vec->cap) {
        size_t new_cap = 2 * vec->cap;
        size_t *new_arr = malloc(new_cap * sizeof *new_arr);
        for (size_t i = 0; i < vec->len; ++i)
            new_arr[i] = vec->arr[i];
        free(vec->arr);
        vec->arr = new_arr;
        vec->cap = new_cap;
    }
    vec->arr[vec->len++] = index;
}

size_t index_vec_len(const struct index_vec *vec) {
    return vec->len;
}

size_t index_vec_get(const struct index_vec *vec, size_t index) {
    return vec->arr[index];
}
