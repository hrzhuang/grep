#include <stdio.h>

#include "str.h"

#include "str_util.h"

struct str *str_from(const char *s) {
    struct str *str = str_new();
    while (*s != '\0') {
        str_app(str, *s);
        ++s;
    }
    return str;
}

struct str *str_from_arr(const char *s, size_t len) {
    struct str *str = str_new();
    for (size_t i = 0; i < len; ++i)
        str_app(str, s[i]);
    return str;
}

void put_str_ln(const struct str *str) {
    for (size_t i = 0; i < str_len(str); ++i)
        putchar(str_get(str, i));
    putchar('\n');
}

struct str *get_line(void) {
    struct str *str = str_new();
    for (int c = getchar(); c >= 0; c = getchar()) {
        if (c == '\n')
            return str;
        str_app(str, (char) c);
    }
    if (str_len(str) > 0)
        return str;
    str_del(str);
    return NULL;
}
