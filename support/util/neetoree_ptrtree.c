//
// Created by user on 10/21/15.
//

#include <stdlib.h>
#include "neetoree_ptrtree.h"

neetoree_ptrtree_t *neetoree_ptrtree_new(neetoree_freefunc freefunc) {
    neetoree_ptrtree_t *tree = calloc(1, sizeof(neetoree_ptrtree_t));
    tree->global = calloc(1, sizeof(neetoree_ptrtree_global_t));
    tree->global->freefunc = freefunc;
    return tree;
}

void *neetoree_ptrtree_index(neetoree_ptrtree_t *tree, int childno) {
    neetoree_ptrtree_t *kid = tree->kid;
    while (kid->sibling && childno-- > 0) {
        kid = kid->sibling;
    }
    return kid;
}

static neetoree_ptrtree_t *neetoree_ptrtree_holder_new(neetoree_ptrtree_global_t *global) {
    neetoree_ptrtree_t *tree = calloc(1, sizeof(neetoree_ptrtree_t));
    tree->global = global;
    return tree;
}

neetoree_ptrtree_t *neetoree_ptrtree_add(neetoree_ptrtree_t *tree, void *child) {
    neetoree_ptrtree_t *child_tree = neetoree_ptrtree_holder_new(tree->global);
    child_tree->parent = tree;
    child_tree->ptr = child;

    if (!tree->kid) {
        tree->kid = child_tree;
        return child_tree;
    }

    neetoree_ptrtree_t *cursor = tree->kid->sibling;
    if (!cursor) {
        tree->kid->sibling = child_tree;
        return child_tree;
    }

    while (cursor->sibling) {
        cursor = cursor->sibling;
    }

    cursor->sibling = child_tree;
    return child_tree;
}

void neetoree_ptrtree_free(neetoree_ptrtree_t *tree) {
    neetoree_ptrtree_t *kid = tree->kid;
    while (kid) {
        tree->global->freefunc(kid->ptr);
        neetoree_ptrtree_t *tmpkid = kid;
        kid = kid->sibling;
        neetoree_ptrtree_free(tmpkid);
    }

    if (!tree->parent) {
        free(tree->global);
    }
    free(tree);
}

static NeetoreeStatus neetoree_ptrtree_walk_tracking(neetoree_ptrtree_t *parent, neetoree_ptrtree_t *tree, neetoree_ptrtree_walker walker, void *context) {
    neetoree_ptrtree_t *kid = tree->kid;

    NeetoreeStatus result = walker(tree->ptr, context);

    while (kid && result == NEETOREE_STATUS_CONT) {
        neetoree_ptrtree_t *tmpkid = kid;
        kid = kid->sibling;
        result = neetoree_ptrtree_walk_tracking(tree, tmpkid, walker, context);
    }

    return result;
}

NeetoreeStatus neetoree_ptrtree_walk(neetoree_ptrtree_t *tree, neetoree_ptrtree_walker walker, void *context) {
    if (!tree) {
        return NEETOREE_STATUS_CONT;
    }

    return neetoree_ptrtree_walk_tracking(NULL, tree, walker, context);
}