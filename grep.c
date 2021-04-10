#include <stdio.h>

#include "re.h"
#include "str.h"
#include "str_util.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("%s: Missing regular expression.\n", argv[0]);
        return 2;
    }
    if (argc > 2) {
        printf("%s: Too many arguments.\n", argv[0]);
        return 2;
    }
    struct str *re_str = str_from(argv[1]);
    struct re *re = re_compile(re_str);
    str_del(re_str);
    if (!re) {
        printf("%s: Invalid regular expression.\n", argv[0]);
        return 2;
    }
    bool matched = false;
    for (struct str *line = get_line(); line; line = get_line()) {
        if (re_match(re, line)) {
            put_str_ln(line);
            matched = true;
        }
        str_del(line);
    }
    re_del(re);
    return !matched;
}
