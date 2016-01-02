//
// Created by user on 11/22/15.
//

#include <neetoree_parser.h>

static NeetoreeResult neetoree_parser_peg_impl(void *context, void *init, neetoree_stream_t *stream);

#include "neetoree_parser_actions.inc"

static NeetoreeResult neetoree_parser_peg_impl(void *context, void *init, neetoree_stream_t *stream) {
    neetoree_parser_node_t grammar_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE,       NONE, "Spacing",    NULL),
            NEETOREE_PARSER_NODE(REF, ONEORMORE, NONE, "Definition", NULL),
            NEETOREE_PARSER_NODE(REF, ONE,       NONE, "EndOfFile",  NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t definition_rule_nodes[] = {
            NEETOREE_PARSER_NODE_ACTION(rule_new),
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Identifier", NULL),
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "LeftArrow",  NULL),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Expression", NULL),
            NEETOREE_PARSER_NODE_ACTION(terminate_choice),
            NEETOREE_PARSER_NODE_ACTION(rule_commit),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t expression_variant_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Slash",    NULL),
            NEETOREE_PARSER_NODE_ACTION(set_choice),
            NEETOREE_PARSER_NODE_ACTION(terminate_choice),
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Sequence", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t expression_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF,      ONE,        NONE, "Sequence", NULL),
            NEETOREE_PARSER_NODE(SEQUENCE, ZEROORMORE, NONE, NULL,       expression_variant_rule_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t sequence_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ZEROORMORE, NONE, "Prefix", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t prefix_action_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "And",    NULL),
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Action", NULL),
            NEETOREE_PARSER_NODE_ACTION(type_action),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t prefix_suffix_pred_and_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "And", NULL),
            NEETOREE_PARSER_NODE_ACTION(pred_and),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t prefix_suffix_pred_not_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Not", NULL),
            NEETOREE_PARSER_NODE_ACTION(pred_not),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t prefix_suffix_pred_rule_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, prefix_suffix_pred_and_rule_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, prefix_suffix_pred_not_rule_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t prefix_suffix_rule_nodes[] = {
            NEETOREE_PARSER_NODE(CHOICE, ZEROORONE, NONE, NULL,     prefix_suffix_pred_rule_nodes),
            NEETOREE_PARSER_NODE(REF,    ONE,       NONE, "Suffix", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t prefix_inner_rule_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, prefix_action_rule_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, prefix_suffix_rule_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t prefix_rule_nodes[] = {
            NEETOREE_PARSER_NODE_ACTION(node_new),
            NEETOREE_PARSER_NODE(CHOICE, ONE, NONE, NULL, prefix_inner_rule_nodes),
            NEETOREE_PARSER_NODE_ACTION(node_commit),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t suffix_type_query_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Query", NULL),
            NEETOREE_PARSER_NODE_ACTION(cardinality_query),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t suffix_type_star_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Star", NULL),
            NEETOREE_PARSER_NODE_ACTION(cardinality_star),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t suffix_type_plus_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Plus", NULL),
            NEETOREE_PARSER_NODE_ACTION(cardinality_plus),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t suffix_type_rule_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, suffix_type_query_rule_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, suffix_type_star_rule_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, suffix_type_plus_rule_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t suffix_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF,    ONE,      NONE, "Primary", NULL),
            NEETOREE_PARSER_NODE(CHOICE, ZEROORONE, NONE, NULL,     suffix_type_rule_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t primary_ref_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE,     "Identifier", NULL),
            NEETOREE_PARSER_NODE(REF, ONE, NEGATIVE, "LeftArrow",  NULL),
            NEETOREE_PARSER_NODE_ACTION(type_ref),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t primary_group_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Open",       NULL),
            NEETOREE_PARSER_NODE_ACTION(group_start),
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Expression", NULL),
            NEETOREE_PARSER_NODE_ACTION(terminate_choice),
            NEETOREE_PARSER_NODE_ACTION(group_end),
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Close",      NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t primary_literal_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Literal", NULL),
            NEETOREE_PARSER_NODE_ACTION(type_literal),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t primary_class_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Class", NULL),
            NEETOREE_PARSER_NODE_ACTION(type_class),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t primary_dot_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Dot", NULL),
            NEETOREE_PARSER_NODE_ACTION(type_dot),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t primary_action_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Action", NULL),
            NEETOREE_PARSER_NODE_ACTION(type_action),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t primary_begin_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Begin", NULL),
            NEETOREE_PARSER_NODE_ACTION(type_begin),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t primary_end_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "End", NULL),
            NEETOREE_PARSER_NODE_ACTION(type_end),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t primary_rule_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, primary_ref_rule_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, primary_group_rule_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, primary_literal_rule_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, primary_class_rule_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, primary_dot_rule_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, primary_action_rule_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, primary_begin_rule_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, primary_end_rule_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t identifier_rule_nodes[] = {
            NEETOREE_PARSER_NODE_BEGIN,
            NEETOREE_PARSER_NODE(REF,   ONE,        NONE, "IdentStart", NULL),
            NEETOREE_PARSER_NODE(REF,   ZEROORMORE, NONE, "IdentCont",  NULL),
            NEETOREE_PARSER_NODE_END,
            NEETOREE_PARSER_NODE(REF,   ONE,        NONE, "Spacing",    NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t identcont_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF,   ONE, NONE, "IdentStart", NULL),
            NEETOREE_PARSER_NODE(RANGE, ONE, NONE, "0-9",        NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t identstart_rule_nodes[] = {
            NEETOREE_PARSER_NODE(RANGE, ONE, NONE, "a-zA-Z_", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t literal_one_char_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,  ONE,        NEGATIVE, "'",    NULL),
            NEETOREE_PARSER_NODE(REF,      ONE,        NONE,     "Char", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t literal_two_char_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,  ONE,        NEGATIVE, "\"",   NULL),
            NEETOREE_PARSER_NODE(REF,      ONE,        NONE,     "Char", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t literal_one_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,  ONE,        NONE, "'",       NULL),
            NEETOREE_PARSER_NODE_BEGIN,
            NEETOREE_PARSER_NODE(SEQUENCE, ZEROORMORE, NONE, NULL,      literal_one_char_rule_nodes),
            NEETOREE_PARSER_NODE_END,
            NEETOREE_PARSER_NODE(LITERAL,  ONE,        NONE, "'",       NULL),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE(REF,      ONE,        NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t literal_two_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,  ONE,        NONE, "\"",      NULL),
            NEETOREE_PARSER_NODE_BEGIN,
            NEETOREE_PARSER_NODE(SEQUENCE, ZEROORMORE, NONE, NULL,      literal_two_char_rule_nodes),
            NEETOREE_PARSER_NODE_END,
            NEETOREE_PARSER_NODE(LITERAL,  ONE,        NONE, "\"",      NULL),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE(REF,      ONE,        NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t literal_rule_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, literal_one_rule_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, literal_two_rule_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t class_char_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NEGATIVE, "]",     NULL),
            NEETOREE_PARSER_NODE(REF,     ONE, NONE,     "Range", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t class_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,  ONE,        NONE, "[",       NULL),
            NEETOREE_PARSER_NODE_BEGIN,
            NEETOREE_PARSER_NODE(SEQUENCE, ZEROORMORE, NONE, NULL,      class_char_rule_nodes),
            NEETOREE_PARSER_NODE_END,
            NEETOREE_PARSER_NODE(LITERAL,  ONE,        NONE, "]",       NULL),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE(REF,      ONE,        NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t range_two_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF,      ONE, NONE, "Char", NULL),
            NEETOREE_PARSER_NODE(LITERAL,  ONE, NONE, "-",    NULL),
            NEETOREE_PARSER_NODE(REF,      ONE, NONE, "Char", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t range_rule_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL,   range_two_rule_nodes),
            NEETOREE_PARSER_NODE(REF,      ONE, NONE, "Char", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t char_esc_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "\\",              NULL),
            NEETOREE_PARSER_NODE(RANGE,   ONE, NONE, "abefnrtv'\"\\[\\]\\\\", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t char_hyph_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "\\",  NULL),
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "-",  NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t char_any_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NEGATIVE, "\\",  NULL),
            NEETOREE_PARSER_NODE(DOT,     ONE, NONE,     NULL,  NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t char_rule_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, char_esc_rule_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, char_hyph_rule_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE, ONE, NONE, NULL, char_any_rule_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t action_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE,        NONE, "{",       NULL),
            NEETOREE_PARSER_NODE_BEGIN,
            NEETOREE_PARSER_NODE(RANGE,   ZEROORMORE, NONE, "^}",      NULL),
            NEETOREE_PARSER_NODE_END,
            NEETOREE_PARSER_NODE(LITERAL, ONE,        NONE, "}",       NULL),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE(REF,     ONE,        NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t leftarrow_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "<-",      NULL),
            NEETOREE_PARSER_NODE(REF,     ONE, NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t slash_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "/",       NULL),
            NEETOREE_PARSER_NODE(REF,     ONE, NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t and_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "&",       NULL),
            NEETOREE_PARSER_NODE(REF,     ONE, NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t not_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "!",       NULL),
            NEETOREE_PARSER_NODE(REF,     ONE, NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t query_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "?",       NULL),
            NEETOREE_PARSER_NODE(REF,     ONE, NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t star_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "*",       NULL),
            NEETOREE_PARSER_NODE(REF,     ONE, NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t plus_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "+",       NULL),
            NEETOREE_PARSER_NODE(REF,     ONE, NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t open_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "(",       NULL),
            NEETOREE_PARSER_NODE(REF,     ONE, NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t close_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, ")",       NULL),
            NEETOREE_PARSER_NODE(REF,     ONE, NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t dot_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, ".",       NULL),
            NEETOREE_PARSER_NODE(REF,     ONE, NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t begin_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "<",       NULL),
            NEETOREE_PARSER_NODE(REF,     ONE, NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t end_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, ">",       NULL),
            NEETOREE_PARSER_NODE(REF,     ONE, NONE, "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t spacing_elem_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Space",   NULL),
            NEETOREE_PARSER_NODE(REF, ONE, NONE, "Comment", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t spacing_rule_nodes[] = {
            NEETOREE_PARSER_NODE(CHOICE, ZEROORMORE, NONE, NULL, spacing_elem_rule_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t comment_char_rule_nodes[] = {
            NEETOREE_PARSER_NODE(REF, ONE, NEGATIVE, "EndOfLine", NULL),
            NEETOREE_PARSER_NODE(DOT, ONE, NONE,     NULL,        NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t comment_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,  ONE,        NONE, "#",         NULL),
            NEETOREE_PARSER_NODE(SEQUENCE, ZEROORMORE, NONE, NULL,        comment_char_rule_nodes),
            NEETOREE_PARSER_NODE(REF,      ONE,        NONE, "EndOfLine", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t space_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, " ",         NULL),
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "\t",        NULL),
            NEETOREE_PARSER_NODE(REF,     ONE, NONE, "EndOfLine", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t endofline_rule_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "\r\n", NULL),
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "\n",   NULL),
            NEETOREE_PARSER_NODE(LITERAL, ONE, NONE, "\r",   NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t endoffile_rule_nodes[] = {
            NEETOREE_PARSER_NODE(DOT, ONE, NEGATIVE, NULL, NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_rule_t rules[] = {
            NEETOREE_PARSER_RULE("Grammar",    SEQUENCE, grammar_rule_nodes),
            NEETOREE_PARSER_RULE("Definition", SEQUENCE, definition_rule_nodes),
            NEETOREE_PARSER_RULE("Expression", SEQUENCE, expression_rule_nodes),
            NEETOREE_PARSER_RULE("Sequence",   SEQUENCE, sequence_rule_nodes),
            NEETOREE_PARSER_RULE("Prefix",     SEQUENCE, prefix_rule_nodes),
            NEETOREE_PARSER_RULE("Suffix",     SEQUENCE, suffix_rule_nodes),
            NEETOREE_PARSER_RULE("Primary",    CHOICE,   primary_rule_nodes),
            NEETOREE_PARSER_RULE("Identifier", SEQUENCE, identifier_rule_nodes),
            NEETOREE_PARSER_RULE("IdentCont",  CHOICE,   identcont_rule_nodes),
            NEETOREE_PARSER_RULE("IdentStart", SEQUENCE, identstart_rule_nodes),
            NEETOREE_PARSER_RULE("Literal",    CHOICE,   literal_rule_nodes),
            NEETOREE_PARSER_RULE("Class",      SEQUENCE, class_rule_nodes),
            NEETOREE_PARSER_RULE("Range",      CHOICE,   range_rule_nodes),
            NEETOREE_PARSER_RULE("Char",       CHOICE,   char_rule_nodes),
            NEETOREE_PARSER_RULE("Action",     SEQUENCE, action_rule_nodes),
            NEETOREE_PARSER_RULE("LeftArrow",  SEQUENCE, leftarrow_rule_nodes),
            NEETOREE_PARSER_RULE("Slash",      SEQUENCE, slash_rule_nodes),
            NEETOREE_PARSER_RULE("And",        SEQUENCE, and_rule_nodes),
            NEETOREE_PARSER_RULE("Not",        SEQUENCE, not_rule_nodes),
            NEETOREE_PARSER_RULE("Query",      SEQUENCE, query_rule_nodes),
            NEETOREE_PARSER_RULE("Star",       SEQUENCE, star_rule_nodes),
            NEETOREE_PARSER_RULE("Plus",       SEQUENCE, plus_rule_nodes),
            NEETOREE_PARSER_RULE("Open",       SEQUENCE, open_rule_nodes),
            NEETOREE_PARSER_RULE("Close",      SEQUENCE, close_rule_nodes),
            NEETOREE_PARSER_RULE("Dot",        SEQUENCE, dot_rule_nodes),
            NEETOREE_PARSER_RULE("Begin",      SEQUENCE, begin_rule_nodes),
            NEETOREE_PARSER_RULE("End",        SEQUENCE, end_rule_nodes),
            NEETOREE_PARSER_RULE("Spacing",    SEQUENCE, spacing_rule_nodes),
            NEETOREE_PARSER_RULE("Comment",    SEQUENCE, comment_rule_nodes),
            NEETOREE_PARSER_RULE("Space",      CHOICE,   space_rule_nodes),
            NEETOREE_PARSER_RULE("EndOfLine",  CHOICE,   endofline_rule_nodes),
            NEETOREE_PARSER_RULE("EndOfFile",  SEQUENCE, endoffile_rule_nodes),
            NEETOREE_PARSER_RULE_TERM
    };

    neetoree_parser_grammar_t *compiled = neetoree_parser_compile(rules);
    if (compiled == NULL) {
        return NEETOREE_RESULT_FAILURE;
    }

    neetoree_parser_valuestack_push(compiled->valuestack, init, NULL);
    NeetoreeResult result = neetoree_parser_run(compiled, stream, context);
    neetoree_parser_valuestack_pop(compiled->valuestack);
    if (result != NEETOREE_RESULT_SUCCESS) {
        neetoree_parser_free(compiled);
        return NEETOREE_RESULT_FAILURE;
    }
    neetoree_parser_free(compiled);
    return NEETOREE_RESULT_SUCCESS;


    /*
    neetoree_parser_grammar_t *compiled = neetoree_parser_compile(rules);
    if (compiled == NULL) {
        return NULL;
    }

    neetoree_parser_builder_context_t *ctx = neetoree_parser_builder_new();
    neetoree_parser_valuestack_push(compiled->valuestack, ctx->rules, NEETOREE_FREE_NAME(builder_node));
    NeetoreeResult result = neetoree_parser_run(compiled, stream, ctx);
    neetoree_parser_valuestack_pop(compiled->valuestack);


    if (result != NEETOREE_RESULT_SUCCESS) {
        neetoree_parser_builder_free(ctx);
        neetoree_parser_free(compiled);
        return NULL;
    }

    neetoree_string_t *code = neetoree_parser_builder_render(name, include, ctx);
    neetoree_parser_builder_free(ctx);
    neetoree_parser_free(compiled);
    return code;
    */
}