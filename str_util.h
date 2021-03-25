#include <stddef.h>

struct str;

#define STR(s) str_from_arr(s, sizeof(s) - 1)

struct str *str_from(const char *s);
struct str *str_from_arr(const char *s, size_t len);
void put_str_ln(const struct str* str);
struct str *get_line(void);
