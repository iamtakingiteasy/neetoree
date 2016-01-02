//
// Created by user on 12/18/15.
//

#include "neetoree_parser_compile.h"

NEETOREE_PARSER_SPEC_NEW(begin) {
    return NULL;
}

NEETOREE_PARSER_SPEC_FREE(begin) {
}

NEETOREE_PARSER_SPEC_APPLY(begin) {
    neetoree_string_reset(applyctx->grammar->match);
    applyctx->grammar->matchoz = 1;
    return NEETOREE_RESULT_NONE;
}