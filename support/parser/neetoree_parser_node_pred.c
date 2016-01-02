//
// Created by user on 12/18/15.
//

#include "neetoree_parser_compile.h"

NEETOREE_PARSER_SPEC_NEW(pred) {
    return spec;
}

NEETOREE_PARSER_SPEC_FREE(pred) {
}

NEETOREE_PARSER_SPEC_APPLY(pred) {
    neetoree_parser_pred_handler pred = data;
    return pred(applyctx->grammar, applyctx->grammar->match, applyctx->grammar->valuestack, applyctx->grammar->context);
}