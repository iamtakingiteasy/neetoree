//
// Created by user on 12/9/15.
//
#include <stdlib.h>
#include <neetoree_string.h>
#include "neetoree_parser_compile.h"

NEETOREE_PARSER_SPEC_NEW(dot) {
    return NULL;
}

NEETOREE_PARSER_SPEC_FREE(dot) {
}

NEETOREE_PARSER_SPEC_APPLY(dot) {
    neetoree_string_reserve(applyctx->grammar->buffer, 1);

    applyctx->grammar->buffer->len = neetoree_stream_read(applyctx->stream, applyctx->grammar->buffer->data, 1);
    if (applyctx->grammar->buffer->len == 1) {
        return NEETOREE_RESULT_SUCCESS;
    }
    return NEETOREE_RESULT_FAILURE;
}