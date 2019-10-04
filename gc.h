#ifndef OOP_GC_H
#define OOP_GC_H


#include <stdlib.h>


void   gc_init      ();
size_t gc_state     ();
void   gc_set_state (size_t state);
void  *gc_alloc     (size_t size);
void  *gc_alloc_root(size_t size);
void   gc_add_root  (void *ptr);
void   gc_collect   ();
void   gc_fin       ();


#endif //OOP_GC_H
