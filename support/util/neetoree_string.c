//
// Created by user on 10/21/15.
//

#include <stdlib.h>
#include <string.h>
#include "neetoree_string.h"
#include "neetoree_common.h"

neetoree_string_t *neetoree_string_new() {
    neetoree_string_t *str = calloc(1, sizeof(neetoree_string_t));
    str->all = 32;
    str->data = calloc(1, str->all);

    return str;
}

neetoree_string_t *neetoree_string_dup_new(char *value) {
    neetoree_string_t *str = neetoree_string_new();
    neetoree_string_append_chars(str, value, strlen(value));
    return str;
}

void neetoree_string_append_char(neetoree_string_t *str, char ch) {
    neetoree_string_reserve(str, 1);
    str->data[str->len] = ch;
    str->data[str->len+1] = 0;
    str->len += 1;
}

NEETOREE_FREE(string) {
    neetoree_string_t *str = item;
    if (str) {
        free(str->data);
        free(str);
    }
}

void neetoree_string_reset(neetoree_string_t *str) {
    str->len = 0;
    str->data[0] = 0;
}

void neetoree_string_append_chars(neetoree_string_t *str, char *in, size_t len) {
    neetoree_string_reserve(str, len);
    memmove(str->data + str->len, in, len);
    str->len += len;
    str->data[str->len] = 0;
}

void neetoree_string_reserve(neetoree_string_t *str, size_t len) {
    if ((str->len+len+1) > str->all) {
        while ((str->len + len + 1) > str->all) {
            str->all <<= 1;
        }
        str->data = realloc(str->data, str->all);
        memset(str->data + str->len, 0, len);
    }
}

void neetoree_string_advance(neetoree_string_t *str, size_t len) {
    str->len += len;
}

void neetoree_string_append_str(neetoree_string_t *str, neetoree_string_t *app) {
    neetoree_string_append_chars(str, app->data, app->len);
}

void neetoree_string_append_c_str(neetoree_string_t *str, char *in) {
    neetoree_string_append_chars(str, in, strlen(in));
}