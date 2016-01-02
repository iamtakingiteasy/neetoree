//
// Created by user on 12/9/15.
//

#include <string.h>
#include "neetoree_parser_compile.h"

NEETOREE_PARSER_SPEC_NEW(literal) {
    return neetoree_string_dup_new(spec);
}

NEETOREE_PARSER_SPEC_FREE(literal) {
    neetoree_string_free(data);
}

NEETOREE_PARSER_SPEC_APPLY(literal) {
    neetoree_string_t *string = data;
    neetoree_string_reserve(applyctx->grammar->buffer, string->len);
    applyctx->grammar->buffer->len = neetoree_stream_read(applyctx->stream, applyctx->grammar->buffer->data, string->len);
    if (applyctx->grammar->buffer->len != string->len) {
        return NEETOREE_RESULT_FAILURE;
    }
    if (memcmp(applyctx->grammar->buffer->data, string->data, string->len) == 0) {
        return NEETOREE_RESULT_SUCCESS;
    }
    return NEETOREE_RESULT_FAILURE;
}