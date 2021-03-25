#include <stdlib.h>

#include "str.h"

struct str {
    char *arr;
    size_t cap;
    size_t len;
};

struct str *str_new(void) {
    size_t cap = 1;
    char *arr = malloc(cap * sizeof *arr);
    struct str *str = malloc(sizeof *str);
    str->arr = arr;
    str->cap = cap;
    str->len = 0;
    return str;
}

void str_del(struct str *str) {
    free(str->arr);
    free(str);
}

void str_app(struct str *str, char c) {
    if (str->len == str->cap) {
        size_t new_cap = 2 * str->cap;
        char *new_arr = malloc(new_cap * sizeof *new_arr);
        for (size_t i = 0; i < str->len; ++i)
            new_arr[i] = str->arr[i];
        free(str->arr);
        str->arr = new_arr;
        str->cap = new_cap;
    }
    str->arr[str->len++] = c;
}

size_t str_len(const struct str *str) {
    return str->len;
}

char str_get(const struct str *str, size_t index) {
    return str->arr[index];
}
