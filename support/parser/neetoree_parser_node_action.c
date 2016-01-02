//
// Created by user on 12/18/15.
//

#include "neetoree_parser_compile.h"

NEETOREE_PARSER_SPEC_NEW(action) {
    return spec;
}

NEETOREE_PARSER_SPEC_FREE(action) {
}

NEETOREE_PARSER_SPEC_APPLY(action) {
    neetoree_parser_action_handler handler = data;
    handler(applyctx->grammar, applyctx->grammar->match, applyctx->grammar->valuestack, applyctx->grammar->context);
    return NEETOREE_RESULT_NONE;
}