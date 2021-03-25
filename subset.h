#include <stdbool.h>
#include <stddef.h>

struct subset;

struct subset *subset_new(size_t size);
void subset_del(struct subset *set);
bool subset_has(struct subset *set, size_t index);
void subset_add(struct subset *set, size_t index);
