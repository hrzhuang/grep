#include <stdbool.h>

struct str;

struct re;

struct re *re_compile(struct str *str);
void re_del(struct re *re);
bool re_match(struct re *re, struct str *str);
