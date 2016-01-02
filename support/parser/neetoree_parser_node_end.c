//
// Created by user on 12/18/15.
//

#include "neetoree_parser_compile.h"

NEETOREE_PARSER_SPEC_NEW(end) {
    return NULL;
}

NEETOREE_PARSER_SPEC_FREE(end) {
}

NEETOREE_PARSER_SPEC_APPLY(end) {
    applyctx->grammar->matchoz = 0;
    return NEETOREE_RESULT_NONE;
}