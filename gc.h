#ifndef OOP_GC_H
#define OOP_GC_H


#include <stdlib.h>


struct collectible_t;


struct collectible_vt {
    void (*mark_recursively) (struct collectible_t *this);
};


struct collectible_t {
    struct collectible_vt *vptr;
    char marked;
};


void collectible_trivial_set_mark (struct collectible_t *this);


void   gc_init      (size_t n_roots_, size_t n_ptrs_);
size_t gc_state     ();
void   gc_set_state (size_t state);
void  *gc_alloc     (size_t size);
void  *gc_alloc_root(size_t size);
void   gc_add_root  (struct collectible_t *ptr);
void   gc_collect   ();
void   gc_fin       ();


#endif //OOP_GC_H
