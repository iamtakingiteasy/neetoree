//
// Created by user on 10/21/15.
//

#ifndef NEETOREE_NEETOREE_PTRLIST_H
#define NEETOREE_NEETOREE_PTRLIST_H

#include <stddef.h>
#include "neetoree_common.h"

typedef struct {
    void             **ptrs;
    size_t             length;
    size_t             allocd;
    neetoree_freefunc  freefunc;
} neetoree_ptrlist_t;

#define NEETOREE_PTRLIST_WALKER_ARGS \
    size_t idx, \
    void *item, \
    void *context

typedef NeetoreeStatus (*neetoree_ptrlist_walker)(NEETOREE_PTRLIST_WALKER_ARGS);

#define NEETOREE_PTRLIST_WALKER_NAME(name) neetoree_ptrlist_## name ##_walker

#define NEETOREE_PTRLIST_WALKER(name) \
    NeetoreeStatus NEETOREE_PTRLIST_WALKER_NAME(name)(NEETOREE_PTRLIST_WALKER_ARGS)

#define neetoree_ptrlist_access(list, idx) ((list)->ptrs[(idx)])
#define neetoree_ptrlist_free NEETOREE_FREE_NAME(ptrlist)
NEETOREE_FREE(ptrlist);
neetoree_ptrlist_t *neetoree_ptrlist_new(neetoree_freefunc freefunc);
void neetoree_ptrlist_add(neetoree_ptrlist_t *list, void *item);
void neetoree_ptrlist_remove(neetoree_ptrlist_t *list, size_t i);
NeetoreeStatus neetoree_ptrlist_walk(neetoree_ptrlist_t *list, neetoree_ptrlist_walker walker, void *context);

#endif //NEETOREE_NEETOREE_PTRLIST_H
