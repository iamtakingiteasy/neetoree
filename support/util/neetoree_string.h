//
// Created by user on 10/21/15.
//

#ifndef NEETOREE_NEETOREE_STRING_H
#define NEETOREE_NEETOREE_STRING_H

#include "neetoree_common.h"
#include <stddef.h>

typedef struct {
    char               *data;
    size_t              len;
    size_t              all;
} neetoree_string_t;

#define neetoree_string_free NEETOREE_FREE_NAME(string)
NEETOREE_FREE(string);
neetoree_string_t *neetoree_string_new();
neetoree_string_t *neetoree_string_dup_new(char *value);
void neetoree_string_append_char(neetoree_string_t *str, char ch);
void neetoree_string_append_chars(neetoree_string_t *str, char *in, size_t len);
void neetoree_string_append_str(neetoree_string_t *str, neetoree_string_t *app);
void neetoree_string_append_c_str(neetoree_string_t *str, char *in);
void neetoree_string_reserve(neetoree_string_t *str, size_t len);
void neetoree_string_advance(neetoree_string_t *str, size_t len);
void neetoree_string_reset(neetoree_string_t *str);

#endif //NEETOREE_NEETOREE_STRING_H
