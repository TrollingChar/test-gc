#include "gc.h"


size_t max_roots, max_ptrs, n_roots, n_ptrs;
void **roots, **ptrs;


void gc_init (size_t n_roots_, size_t n_ptrs_) {
    max_roots = n_roots_;
    max_ptrs  = n_ptrs_;
    n_roots   =
    n_ptrs    = 0;
    roots     = malloc (sizeof (void *) * max_roots);
    ptrs      = malloc (sizeof (void *) * max_ptrs);
}


inline size_t gc_state () {
    return n_roots;
}


inline void gc_set_state (size_t state) {
    n_roots = state;
}


void *gc_alloc (size_t size) {
    if (n_ptrs >= max_ptrs) exit (1);

    void *ptr = malloc (size);
    ptrs [n_ptrs++] = ptr;
    return ptr;
}


void *gc_alloc_root (size_t size) {
    unsigned int error = (n_ptrs >= max_ptrs) | (n_roots >= max_roots) << 1;
    if (error) exit (error);

    void *ptr = malloc (size);
    ptrs  [n_ptrs++]  =
    roots [n_roots++] = ptr;
    return ptr;
}


void gc_add_root (void *root) {
    if (n_roots >= max_roots) exit (2);

    roots [n_roots++] = root;
}


void gc_collect () {
    // очистить метки
    // рекурсивно обойти все корни
    // освободить неиспользуемую память
}


void gc_fin () {
    free (roots);
    free (ptrs);
}
