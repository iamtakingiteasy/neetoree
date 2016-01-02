//
// Created by user on 10/21/15.
//

#include <stdlib.h>
#include "neetoree_ptrlist.h"

neetoree_ptrlist_t *neetoree_ptrlist_new(neetoree_freefunc freefunc) {
    neetoree_ptrlist_t *list = calloc(1, sizeof(neetoree_ptrlist_t));
    list->length = 0;
    list->allocd = 1;
    list->ptrs = calloc(1, sizeof(void*));
    list->freefunc = freefunc;
    return list;
}

void neetoree_ptrlist_insert(neetoree_ptrlist_t *list, size_t idx, void *item) {
    void *ptr = item;
    for (size_t i = idx; i < list->length; i++) {
        void *tmp = neetoree_ptrlist_access(list, i);
        neetoree_ptrlist_set(list, i, ptr);
        ptr = tmp;
    }
    neetoree_ptrlist_add(list, ptr);
}

void neetoree_ptrlist_add(neetoree_ptrlist_t *list, void *item) {
    list->length += 1;
    if (list->length > list->allocd) {
        list->allocd <<= 1;
        list->ptrs = realloc(list->ptrs, sizeof(void*) * list->allocd);
    }
    list->ptrs[list->length-1] = item;
}

void * neetoree_ptrlist_remove_fast(neetoree_ptrlist_t *list, size_t idx) {
    void *res = neetoree_ptrlist_access(list ,idx);
    if (idx < list->length - 1) {
        list->ptrs[idx] = list->ptrs[list->length - 1];
    }
    list->length -= 1;
    return res;
}

void *neetoree_ptrlist_remove(neetoree_ptrlist_t *list, size_t idx) {
    void *res = neetoree_ptrlist_access(list ,idx);
    for (size_t i = idx; i < list->length - 1; i++) {
        void *next = neetoree_ptrlist_access(list, i+1);
        neetoree_ptrlist_set(list, i, next);
    }
    list->length -= 1;
    return res;
}

NEETOREE_PTRLIST_WALKER(free) {
    neetoree_ptrlist_t *list = context;
    if (list->freefunc) {
        list->freefunc(item);
    }

    return NEETOREE_STATUS_CONT;
}

NEETOREE_FREE(ptrlist) {
    neetoree_ptrlist_t *list = item;
    if (!list) {
        return;
    }
    neetoree_ptrlist_walk(list, NEETOREE_PTRLIST_WALKER_NAME(free), list);
    free(list->ptrs);
    list->ptrs = NULL;
    list->length = 0;
    list->allocd = 0;
    free(list);
}

NeetoreeStatus neetoree_ptrlist_walk(neetoree_ptrlist_t *list, neetoree_ptrlist_walker walker, void *context) {
    for (size_t i = 0; i < list->length; i++) {
        NeetoreeStatus result = walker(i, list->ptrs[i], context);
        if (result != NEETOREE_STATUS_CONT) {
            return result;
        }
    }
    return NEETOREE_STATUS_CONT;
}
