#include <stdbool.h>
#include <stddef.h>

struct state_set;

struct state_set *state_set_new(size_t size);
void state_set_del(struct state_set *set);
bool state_set_has(struct state_set *set, size_t index);
size_t state_set_size(struct state_set *set);
size_t state_set_get(struct state_set *set, size_t index);
void state_set_add(struct state_set *set, size_t index);
