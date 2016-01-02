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

void neetoree_ptrlist_add(neetoree_ptrlist_t *list, void *item) {
    list->length += 1;
    if (list->length > list->allocd) {
        list->allocd <<= 1;
        list->ptrs = realloc(list->ptrs, sizeof(void*) * list->allocd);
    }
    list->ptrs[list->length-1] = item;
}

void neetoree_ptrlist_remove(neetoree_ptrlist_t *list, size_t i) {
    if (i < list->length - 1) {
        list->ptrs[i] = list->ptrs[list->length - 1];
    }
    list->length -= 1;
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
