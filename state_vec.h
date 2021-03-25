#include <stddef.h>

struct state;

struct state_vec;

struct state_vec *state_vec_new(void);
void state_vec_del(struct state_vec *vec);
struct state *state_vec_get(const struct state_vec *vec, size_t index);
size_t state_vec_len(const struct state_vec *vec);
void state_vec_app(struct state_vec *vec, struct state *state);
