/*
 * object   : [vptr, data]
 * gcobject : [gcdata, object ptr]
 * class    : [data, methods]
 *
 *
 * ._ memory _______________________.
 * [size & mark] [vptr] [object]
 *
 *
 * gc methods:
 *  push_root       - add gc root
 *  pop_roots       - remove gc roots
 *  alloc           - allocate memory for an object
 *  alloc_root      - allocate and keep alive
 *  collect         - free unreachable memory
 * for exceptions:
 *  store_roots     - stores stack depth, in case of exception
 *  restore_roots   - restores stack depth so everything in try block will be root no longer
 *
 * so gc will have:
 *  a stack of gc roots
 *  gc controlled objects
 *
 * methods, again:
 *  push_root       - push a ptr to stack of roots
 *  pop_roots       - decrease root stack depth (do we need this?)
 *  alloc           - allocate an object and put it to collection of regulars
 *  alloc_root      - allocate an object and put in both collections
 *  collect         - unmark objects, then mark from roots recursively
 * for exceptions:
 *  store_roots     - better to do via a local variable
 *  restore_roots   - set root stack depth to a specific number
 *
 *
 * so this
 *  i := new Int;
 *  something..i;
 * will become
 *  int depth = gc_store_roots ();
 *  int *i = gc_alloc_root(4);
 *  something (i);
 *  gc_restore_roots(depth);
 */


/*
#include <stdio.h>
#include <stdlib.h>


struct vtable_t {
    void (* spread_mark) (void * obj);
};


struct gc_obj_t {
    char              mark;
    struct vtable_t * vptr;
};


struct gc_t {
    size_t            n_roots;
    struct gc_obj_t * roots [1024];
    size_t            n_ptrs;
    struct gc_obj_t * ptrs  [1024];
};


struct gc_t gc;


void gc_init () {
    gc.n_ptrs = gc.n_roots = 0;
}


void gc_push_root (struct gc_obj_t * ptr) {
    if (gc.n_roots >= 1024) exit (1);
    gc.roots [gc.n_roots++] = ptr;
}


void * gc_alloc (size_t size) {
    if (gc.n_ptrs >= 1024) exit (1);
    struct gc_obj_t * ptr = malloc (size);
    gc.ptrs [gc.n_ptrs++] = ptr;
    return ptr;
}


void * gc_alloc_root (size_t size) {
    if (gc.n_ptrs >= 1024 || gc.n_roots >= 1024) exit (1);
    struct gc_obj_t * ptr = malloc (size + 1);
    gc.ptrs  [gc.n_ptrs ++] = ptr;
    gc.roots [gc.n_roots++] = ptr;
    return ptr;
}


void spread_mark (struct gc_obj_t * obj) {
    if (obj->mark) return;
    obj->mark = 1;
    obj->vptr->spread_mark (obj);
}


void gc_collect () {
    // unmark everything
    for (size_t i = 0; i < gc.n_ptrs;  ++i) gc.ptrs  [i]->mark = 0;
    for (size_t i = 0; i < gc.n_roots; ++i) gc.roots [i]->mark = 0;

    // mark recursively
    for (size_t i = 0; i < gc.n_roots; ++i) {
        struct gc_obj_t * root = gc.roots [i];
        if (root) spread_mark (root);
    }

    // delete unmarked
    size_t c, d; // current, destination
    for (c = 0; c < gc.n_ptrs; ++c) {
        if (!gc.ptrs [c]->mark) {
            free (gc.ptrs [c]);
            for (d = c++; c < gc.n_ptrs; ++c) {
                if (gc.ptrs [c]->mark) gc.ptrs [d++] = gc.ptrs [c];
                else                   free (gc.ptrs [c]);
            }
            gc.n_ptrs = d;
            break;
        }
    }
}


struct linked_list_vtable_t;


struct linked_list_vtable_t linked_list_vtable;


struct linked_list_t {
    char mark;
    struct linked_list_vtable_t * vptr;
    struct linked_list_t * next;
    int value;
};


struct linked_list_vtable_t {
    void (* spread_mark) (struct linked_list_t * obj);
};


void linked_list_spread_mark (struct linked_list_t * obj) {
    if (obj->next) spread_mark ((struct gc_obj_t *) obj->next);
}


void linked_list_vtable_init () {
    linked_list_vtable.spread_mark = &linked_list_spread_mark;
}


void linked_list_init (struct linked_list_t * obj, int value) {
    obj->vptr  = &linked_list_vtable;
    obj->value = value;
    obj->next  = 0;
}


void linked_list_print (struct linked_list_t * obj, size_t depth) {
    if (!depth) return;

    printf ("%i", obj->value);
    if (obj->next) linked_list_print (obj->next, --depth);
}


int main () {
    gc_init ();
    linked_list_vtable_init ();

    size_t scope_1 = gc.n_roots;
        struct linked_list_t * l1;
        l1 = gc_alloc_root (sizeof (struct linked_list_t));  linked_list_init (l1, 1);

        size_t scope_2 = gc.n_roots;
            struct linked_list_t * l2, * l3;

            l2 = gc_alloc_root (sizeof (struct linked_list_t));  linked_list_init (l2, 2);
            l3 = gc_alloc_root (sizeof (struct linked_list_t));  linked_list_init (l3, 3);

            l1->next = l2;
            l2->next = l3;
            l3->next = l1;

            gc_collect ();

            linked_list_print (l1, 20);
        gc.n_roots = scope_2;
        gc_collect ();

        linked_list_print (l1, 20);

    gc.n_roots = scope_1;
    gc_collect ();

//    linked_list_print (l1, 20);  // valgrind warning - invalid read

    return 0;
}
*/


#include "classes.h"


int main () {
    gc_init (60000, 60000);
    gc_fin ();
    return 0;
}