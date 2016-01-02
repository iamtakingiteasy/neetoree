//
// Created by user on 10/21/15.
//

#ifndef NEETOREE_NEETOREE_PTRTREE_H
#define NEETOREE_NEETOREE_PTRTREE_H

#include "neetoree_common.h"

typedef struct neetoree_ptrtree_global_t {
    neetoree_freefunc  freefunc;
} neetoree_ptrtree_global_t;

typedef struct neetoree_ptrtree_t {
    void *ptr;
    struct neetoree_ptrtree_t        *kid;
    struct neetoree_ptrtree_t        *parent;
    struct neetoree_ptrtree_t        *sibling;
    struct neetoree_ptrtree_global_t *global;
} neetoree_ptrtree_t;

#define NEETOREE_PTRTREE_WALKER_ARGS \
    void *data, \
    void *context

typedef NeetoreeStatus (*neetoree_ptrtree_walker)(NEETOREE_PTRTREE_WALKER_ARGS);

#define NEETOREE_PTRTREE_WALKER_NAME(name) neetoree_ptrtree_## name ##_walker
#define NEETOREE_PTRTREE_WALKER(name) \
    NeetoreeStatus NEETOREE_PTRTREE_WALKER_NAME(name)(NEETOREE_PTRTREE_WALKER_ARGS)

neetoree_ptrtree_t *neetoree_ptrtree_new(neetoree_freefunc freefunc);
void *neetoree_ptrtree_index(neetoree_ptrtree_t *tree, int childno);
neetoree_ptrtree_t *neetoree_ptrtree_add(neetoree_ptrtree_t *tree, void *child);
NeetoreeStatus neetoree_ptrtree_walk(neetoree_ptrtree_t *tree, neetoree_ptrtree_walker walker, void *context);
void neetoree_ptrtree_free(neetoree_ptrtree_t *tree);

#endif //NEETOREE_NEETOREE_PTRTREE_H
