#include <stddef.h>

struct str;

struct str *str_new(void);
void str_del(struct str *str);
char str_get(const struct str *str, size_t index);
size_t str_len(const struct str *str);
void str_app(struct str *str, char c);
