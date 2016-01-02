//
// Created by user on 10/21/15.
//

#include <stdlib.h>
#include "neetoree_ptrstack.h"

neetoree_ptrstack_t *neetoree_ptrstack_new(neetoree_freefunc freefunc) {
    neetoree_ptrstack_t *stack = calloc(1, sizeof(neetoree_ptrstack_t));
    stack->global = calloc(1, sizeof(neetoree_ptrstack_global_t));
    stack->global->freefunc = freefunc;
    stack->global->depth = 0;
    return stack;
}

static neetoree_ptrstack_t *neetoree_ptrstack_holder_new(neetoree_ptrstack_global_t *global) {
    neetoree_ptrstack_t *stack = calloc(1, sizeof(neetoree_ptrstack_t));
    stack->global = global;
    return stack;
}

void neetoree_ptrstack_push(neetoree_ptrstack_t *stack, void *ptr) {
    neetoree_ptrstack_t *newel = stack;
    if (!newel->ptr) {
        newel->ptr = ptr;
        stack->global->head = stack;
        stack->global->depth++;
    } else {
        newel = neetoree_ptrstack_holder_new(stack->global);
        newel->ptr = ptr;
        newel->next = stack->global->head;
        stack->global->depth++;
        stack->global->head = newel;
    }
}

void *neetoree_ptrstack_pop(neetoree_ptrstack_t *stack) {
    void *ptr;
    if (!stack->global->head) {
        ptr = stack->ptr;
        stack->ptr = NULL;
        stack->global->head = NULL;
        stack->global->depth--;
    } else {
        ptr = stack->global->head->ptr;
        neetoree_ptrstack_t *next = stack->global->head->next;
        if (stack->global->head != stack) {
            free(stack->global->head);
        }
        stack->global->head = next;
        stack->global->depth--;
    }
    return ptr;
}


void *neetoree_ptrstack_peek(neetoree_ptrstack_t *stack, size_t depth) {
    neetoree_ptrstack_t *newl = stack->global->head;
    for (size_t i = 0; newl && (newl->next || depth < 2); i++) {
        if (i == depth) {
            return newl->ptr;
        }
        newl = newl->next;
    }
    return NULL;
}

void neetoree_ptrstack_free(neetoree_ptrstack_t *stack) {
    neetoree_ptrstack_t *next = stack->global->head;
    while (next) {
        stack->global->freefunc(next->ptr);
        void *tmpnext = next;
        next = next->next;
        if (tmpnext != stack) {
            free(tmpnext);
        }
    }
    free(stack->global);
    free(stack);
}

NeetoreeStatus neetoree_ptrstack_walk(neetoree_ptrstack_t *stack, neetoree_ptrstack_walker walker, void *context) {
    neetoree_ptrstack_t *next = stack->global->head;
    while (next) {
        NeetoreeStatus result = walker(next->ptr, context);
        if (result != NEETOREE_STATUS_CONT) {
            return result;
        }
        next = next->next;
    }
    return NEETOREE_STATUS_CONT;
}