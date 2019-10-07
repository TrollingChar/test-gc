#include <slcurses.h>
#include "gc.h"


size_t max_roots, max_ptrs, n_roots, n_ptrs;
struct collectible_t **roots, **ptrs;


void gc_init (size_t n_roots_, size_t n_ptrs_) {
    max_roots = n_roots_;
    max_ptrs  = n_ptrs_;
    n_roots   =
    n_ptrs    = 0;
    roots     = malloc (sizeof (void *) *max_roots);
    ptrs      = malloc (sizeof (void *) *max_ptrs);
}


inline size_t gc_state () {
    return n_roots;
}


inline void gc_set_state (size_t state) {
    n_roots = state;
}


void *gc_alloc (size_t size) {
    if (n_ptrs >= max_ptrs) exit (1);

    struct collectible_t *ptr = malloc (size);
    ptrs [n_ptrs++] = ptr;
    return ptr;
}


void *gc_alloc_root (size_t size) {
    int error = (n_ptrs >= max_ptrs) | (n_roots >= max_roots) << 1;
    if (error) exit (error);

    struct collectible_t *ptr = malloc (size);
    ptrs  [n_ptrs++]  =
    roots [n_roots++] = ptr;
    return ptr;
}


void gc_add_root (struct collectible_t *root) {
    if (n_roots >= max_roots) exit (2);

    roots [n_roots++] = root;
}


void gc_fin () {
    for (size_t i = 0; i < n_ptrs;  ++i) free (ptrs  [i]);
    for (size_t i = 0; i < n_roots; ++i) free (roots [i]);
    free (roots);
    free (ptrs);
}


void gc_collect () {
    // очистить метки
    for (size_t i = 0; i < n_ptrs;  ++i) ptrs  [i]->marked = 0;
    for (size_t i = 0; i < n_roots; ++i) roots [i]->marked = 0;

    // рекурсивно обойти все корни
    for (size_t i = 0; i < n_roots; ++i) roots [i]->vptr->mark_recursively (roots [i]);

    // освободить неиспользуемую память
    size_t c, d; // current, destination
    for (c = 0; c < n_ptrs; ++c) {
        // доходим до 1 неиспользуемого элемента без ненужных присвоений
        if (!ptrs [c]->marked) {
            free (ptrs [c]);
            for (d = c++; c < n_ptrs; ++c) {
                if (ptrs [c]->marked) ptrs [d++] = ptrs [c];
                else                  free (ptrs [c]);
            }
            n_ptrs = d;
            break;
        }
    }
}


void collectible_trivial_set_mark (struct collectible_t *this) {
    this->marked = 1;
}
