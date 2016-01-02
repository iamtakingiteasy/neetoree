//
// Created by user on 12/9/15.
//

#include <string.h>
#include <neetoree_string.h>
#include "neetoree_parser_compile.h"

typedef enum {
    NEETOREE_PARSER_RANGE_STATE_NORMAL,
    NEETOREE_PARSER_RANGE_STATE_ESCAPE
} NeetoreeParserRangeState;

NEETOREE_PARSER_SPEC_NEW(literal) {
    NeetoreeParserRangeState state = NEETOREE_PARSER_RANGE_STATE_NORMAL;
    char *match = spec;
    neetoree_string_t *str = neetoree_string_new();
    for (size_t i = 0; i < strlen(match); i++) {
        char ch = match[i];
        switch (state) {
            case NEETOREE_PARSER_RANGE_STATE_NORMAL:
                if (ch == '\\') {
                    state = NEETOREE_PARSER_RANGE_STATE_ESCAPE;
                } else {
                    neetoree_string_append_char(str, ch);
                }
                break;
            case NEETOREE_PARSER_RANGE_STATE_ESCAPE:
                switch (ch) {
                    case 'a':
                        ch = '\a';
                        break;
                    case 'b':
                        ch = '\b';
                        break;
                    case 'e':
                        ch = '\e';
                        break;
                    case 'f':
                        ch = '\f';
                        break;
                    case 'n':
                        ch = '\n';
                        break;
                    case 'r':
                        ch = '\r';
                        break;
                    case 't':
                        ch = '\t';
                        break;
                    case 'v':
                        ch = '\v';
                        break;
                    default:
                        break;
                }
                neetoree_string_append_char(str, ch);
                state = NEETOREE_PARSER_RANGE_STATE_NORMAL;
                break;
        }
    }
    return str;
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