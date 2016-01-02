//
// Created by user on 11/22/15.
//

#ifndef NEETOREE_NEETOREE_PARSER_VALUESTACK_H
#define NEETOREE_NEETOREE_PARSER_VALUESTACK_H

#include <stddef.h>
#include <neetoree_ptrstack.h>

typedef neetoree_ptrstack_t neetoree_parser_valuestack_t;

typedef struct {
    neetoree_freefunc  freefunc;
    void              *data;
} neetoree_parser_value_t;

neetoree_parser_valuestack_t *neetoree_parser_valuestack_new();
void neetoree_parser_valuestack_free(neetoree_parser_valuestack_t *valuestack);
void *neetoree_parser_valuestack_pop(neetoree_parser_valuestack_t *valuestack);
void  neetoree_parser_valuestack_push(neetoree_parser_valuestack_t *valuestack, void *data, neetoree_freefunc freefunc);
void *neetoree_parser_valuestack_peek(neetoree_parser_valuestack_t *valuestack, size_t n);
void  neetoree_parser_valuestack_drop(neetoree_parser_valuestack_t *valuestack, size_t limit);

#endif //NEETOREE_NEETOREE_PARSER_VALUESTACK_H
