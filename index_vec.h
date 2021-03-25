#include <stddef.h>

struct index_vec;

struct index_vec *index_vec_new(void);
void index_vec_del(struct index_vec *vec);
size_t index_vec_get(const struct index_vec *vec, size_t index);
size_t index_vec_len(const struct index_vec *vec);
void index_vec_app(struct index_vec *vec, size_t index);
