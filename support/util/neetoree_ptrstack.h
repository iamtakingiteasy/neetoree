//
// Created by user on 10/21/15.
//

#ifndef NEETOREE_NEETOREE_PTRSTACK_H
#define NEETOREE_NEETOREE_PTRSTACK_H

#include "neetoree_common.h"

typedef struct neetoree_ptrstack_global_t {
    struct neetoree_ptrstack_t *head;
    neetoree_freefunc           freefunc;
    size_t                      depth;
} neetoree_ptrstack_global_t;

typedef struct neetoree_ptrstack_t {
    void                              *ptr;
    struct neetoree_ptrstack_global_t *global;
    struct neetoree_ptrstack_t        *next;
} neetoree_ptrstack_t;

#define NEETOREE_PTRSTACK_WALKER_ARGS \
    void *item, \
    void *context

typedef NeetoreeStatus (*neetoree_ptrstack_walker)(NEETOREE_PTRSTACK_WALKER_ARGS);

#define NEETOREE_PTRSTACK_WALKER_NAME(name) neetoree_ptrstack_## name ##_walker
#define NEETOREE_PTRSTACK_WALKER(name) \
    NeetoreeStatus NEETOREE_PTRSTACK_WALKER_NAME(name)(NEETOREE_PTRSTACK_WALKER_ARGS)

neetoree_ptrstack_t *neetoree_ptrstack_new(neetoree_freefunc freefunc);
void neetoree_ptrstack_push(neetoree_ptrstack_t *stack, void *ptr);
void *neetoree_ptrstack_pop(neetoree_ptrstack_t *stack);
void *neetoree_ptrstack_peek(neetoree_ptrstack_t *stack, size_t depth);
void neetoree_ptrstack_free(neetoree_ptrstack_t *stack);
NeetoreeStatus neetoree_ptrstack_walk(neetoree_ptrstack_t *stack, neetoree_ptrstack_walker walker, void *context);

#endif //NEETOREE_NEETOREE_PTRSTACK_H