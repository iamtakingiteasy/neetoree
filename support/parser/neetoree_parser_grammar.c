#include <neetoree_parser.h>

static NeetoreeResult neetoree_parser_peg_impl(void *context, void *init, neetoree_stream_t *stream);

#include "neetoree_parser_actions.inc"

static NeetoreeResult neetoree_parser_peg_impl(void *context, void *init, neetoree_stream_t *stream) {

    neetoree_parser_node_t rule_Grammar_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing",    NULL),
            NEETOREE_PARSER_NODE(REF,       ONEORMORE,  NONE,     "Definition", NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "EndOfFile",  NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Definition_nodes[] = {
            NEETOREE_PARSER_NODE_ACTION(rule_new),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Identifier",  NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "LEFTARROW",   NULL),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Expression",  NULL),
            NEETOREE_PARSER_NODE_ACTION(rule_commit),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Expression_1_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "SLASH",       NULL),
            NEETOREE_PARSER_NODE_ACTION(node_new),
            NEETOREE_PARSER_NODE_ACTION(group),
            NEETOREE_PARSER_NODE_ACTION(set_choice),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Sequence",    NULL),
            NEETOREE_PARSER_NODE_ACTION(node_commit),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Expression_nodes[] = {
            NEETOREE_PARSER_NODE_ACTION(node_new),
            NEETOREE_PARSER_NODE_ACTION(group),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Sequence",    NULL),
            NEETOREE_PARSER_NODE_ACTION(node_commit),
            NEETOREE_PARSER_NODE(SEQUENCE,  ZEROORMORE, NONE,     NULL,          rule_Expression_1_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Sequence_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ZEROORMORE, NONE,     "Prefix", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Prefix_1_1_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "AND",         NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Action",      NULL),
            NEETOREE_PARSER_NODE_ACTION(type_action),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Prefix_1_2_1_1_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "AND",      NULL),
            NEETOREE_PARSER_NODE_ACTION(pred_and),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Prefix_1_2_1_2_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "NOT",      NULL),
            NEETOREE_PARSER_NODE_ACTION(pred_not),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Prefix_1_2_1_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Prefix_1_2_1_1_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Prefix_1_2_1_2_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Prefix_1_2_nodes[] = {
            NEETOREE_PARSER_NODE(CHOICE,    ZEROORONE,  NONE,     NULL,     rule_Prefix_1_2_1_nodes),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Suffix", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Prefix_1_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Prefix_1_1_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Prefix_1_2_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Prefix_nodes[] = {
            NEETOREE_PARSER_NODE_ACTION(node_new),
            NEETOREE_PARSER_NODE(CHOICE,    ONE,        NONE,     NULL,          rule_Prefix_1_nodes),
            NEETOREE_PARSER_NODE_ACTION(node_commit),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Suffix_1_1_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "QUERY",             NULL),
            NEETOREE_PARSER_NODE_ACTION(cardinality_query),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Suffix_1_2_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "STAR",             NULL),
            NEETOREE_PARSER_NODE_ACTION(cardinality_star),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Suffix_1_3_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "PLUS",             NULL),
            NEETOREE_PARSER_NODE_ACTION(cardinality_plus),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Suffix_1_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Suffix_1_1_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Suffix_1_2_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Suffix_1_3_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Suffix_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Primary", NULL),
            NEETOREE_PARSER_NODE(CHOICE,    ZEROORONE,  NONE,     NULL,      rule_Suffix_1_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_1_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Identifier", NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NEGATIVE, "LEFTARROW",  NULL),
            NEETOREE_PARSER_NODE_ACTION(type_ref),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_2_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "OPEN",       NULL),
            NEETOREE_PARSER_NODE_ACTION(group),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Expression", NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "CLOSE",      NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_3_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Literal",      NULL),
            NEETOREE_PARSER_NODE_ACTION(type_literal),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_4_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Class",      NULL),
            NEETOREE_PARSER_NODE_ACTION(type_class),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_5_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "DOT",      NULL),
            NEETOREE_PARSER_NODE_ACTION(type_dot),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_6_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Action",      NULL),
            NEETOREE_PARSER_NODE_ACTION(type_action),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_7_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "BEGIN",      NULL),
            NEETOREE_PARSER_NODE_ACTION(type_begin),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_8_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "END",      NULL),
            NEETOREE_PARSER_NODE_ACTION(type_end),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Primary_1_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Primary_2_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Primary_3_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Primary_4_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Primary_5_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Primary_6_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Primary_7_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Primary_8_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Identifier_nodes[] = {
            NEETOREE_PARSER_NODE_BEGIN,
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "IdentStart", NULL),
            NEETOREE_PARSER_NODE(REF,       ZEROORMORE, NONE,     "IdentCont",  NULL),
            NEETOREE_PARSER_NODE_END,
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing",    NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_IdentStart_nodes[] = {
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NONE,     "a-zA-Z_", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_IdentCont_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "IdentStart", NULL),
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NONE,     "0-9",        NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Literal_1_1_nodes[] = {
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NEGATIVE, "'",    NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Char", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Literal_1_nodes[] = {
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NONE,     "'",        NULL),
            NEETOREE_PARSER_NODE_BEGIN,
            NEETOREE_PARSER_NODE(SEQUENCE,  ZEROORMORE, NONE,     NULL,       rule_Literal_1_1_nodes),
            NEETOREE_PARSER_NODE_END,
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NONE,     "'",        NULL),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing",  NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Literal_2_1_nodes[] = {
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NEGATIVE, "\"",   NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Char", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Literal_2_nodes[] = {
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NONE,     "\"",       NULL),
            NEETOREE_PARSER_NODE_BEGIN,
            NEETOREE_PARSER_NODE(SEQUENCE,  ZEROORMORE, NONE,     NULL,       rule_Literal_2_1_nodes),
            NEETOREE_PARSER_NODE_END,
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NONE,     "\"",       NULL),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing",  NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Literal_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Literal_1_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Literal_2_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Class_1_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NEGATIVE, "]",     NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Range", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Class_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "[",        NULL),
            NEETOREE_PARSER_NODE_BEGIN,
            NEETOREE_PARSER_NODE(SEQUENCE,  ZEROORMORE, NONE,     NULL,       rule_Class_1_nodes),
            NEETOREE_PARSER_NODE_END,
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "]",        NULL),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing",  NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Range_1_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Char", NULL),
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "-",    NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Char", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Range_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL,   rule_Range_1_nodes),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Char", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Char_1_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "\\",                  NULL),
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NONE,     "abefnrtv'\"\\[\\]\\\\", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Char_2_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "\\", NULL),
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "-",    NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Char_3_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NEGATIVE, "\\", NULL),
            NEETOREE_PARSER_NODE(DOT,       ONE,        NONE,     NULL,   NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Char_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Char_1_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Char_2_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Char_3_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_LEFTARROW_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "<-",      NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_SLASH_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "/",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_AND_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "&",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_NOT_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "!",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_QUERY_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "?",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_STAR_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "*",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_PLUS_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "+",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_OPEN_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "(",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_CLOSE_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     ")",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_DOT_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     ".",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Spacing_1_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Space",   NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Comment", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Spacing_nodes[] = {
            NEETOREE_PARSER_NODE(CHOICE,    ZEROORMORE, NONE,     NULL,   rule_Spacing_1_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Comment_1_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NEGATIVE, "EndOfLine", NULL),
            NEETOREE_PARSER_NODE(DOT,       ONE,        NONE,     NULL,        NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Comment_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "#",         NULL),
            NEETOREE_PARSER_NODE(SEQUENCE,  ZEROORMORE, NONE,     NULL,        rule_Comment_1_nodes),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "EndOfLine", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Space_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     " ",         NULL),
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "\t",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "EndOfLine", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_EndOfLine_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "\r\n", NULL),
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "\n",    NULL),
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "\r",    NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_EndOfFile_nodes[] = {
            NEETOREE_PARSER_NODE(DOT,       ONE,        NEGATIVE, NULL,   NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Action_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "{",        NULL),
            NEETOREE_PARSER_NODE_BEGIN,
            NEETOREE_PARSER_NODE(RANGE,     ZEROORMORE, NONE,     "^}",       NULL),
            NEETOREE_PARSER_NODE_END,
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "}",        NULL),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing",  NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_BEGIN_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "<",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_END_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     ">",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_rule_t rules[] = {
            NEETOREE_PARSER_RULE("Grammar",    SEQUENCE, rule_Grammar_nodes),
            NEETOREE_PARSER_RULE("Definition", SEQUENCE, rule_Definition_nodes),
            NEETOREE_PARSER_RULE("Expression", SEQUENCE, rule_Expression_nodes),
            NEETOREE_PARSER_RULE("Sequence",   SEQUENCE, rule_Sequence_nodes),
            NEETOREE_PARSER_RULE("Prefix",     SEQUENCE, rule_Prefix_nodes),
            NEETOREE_PARSER_RULE("Suffix",     SEQUENCE, rule_Suffix_nodes),
            NEETOREE_PARSER_RULE("Primary",    CHOICE,   rule_Primary_nodes),
            NEETOREE_PARSER_RULE("Identifier", SEQUENCE, rule_Identifier_nodes),
            NEETOREE_PARSER_RULE("IdentStart", SEQUENCE, rule_IdentStart_nodes),
            NEETOREE_PARSER_RULE("IdentCont",  CHOICE,   rule_IdentCont_nodes),
            NEETOREE_PARSER_RULE("Literal",    CHOICE,   rule_Literal_nodes),
            NEETOREE_PARSER_RULE("Class",      SEQUENCE, rule_Class_nodes),
            NEETOREE_PARSER_RULE("Range",      CHOICE,   rule_Range_nodes),
            NEETOREE_PARSER_RULE("Char",       CHOICE,   rule_Char_nodes),
            NEETOREE_PARSER_RULE("LEFTARROW",  SEQUENCE, rule_LEFTARROW_nodes),
            NEETOREE_PARSER_RULE("SLASH",      SEQUENCE, rule_SLASH_nodes),
            NEETOREE_PARSER_RULE("AND",        SEQUENCE, rule_AND_nodes),
            NEETOREE_PARSER_RULE("NOT",        SEQUENCE, rule_NOT_nodes),
            NEETOREE_PARSER_RULE("QUERY",      SEQUENCE, rule_QUERY_nodes),
            NEETOREE_PARSER_RULE("STAR",       SEQUENCE, rule_STAR_nodes),
            NEETOREE_PARSER_RULE("PLUS",       SEQUENCE, rule_PLUS_nodes),
            NEETOREE_PARSER_RULE("OPEN",       SEQUENCE, rule_OPEN_nodes),
            NEETOREE_PARSER_RULE("CLOSE",      SEQUENCE, rule_CLOSE_nodes),
            NEETOREE_PARSER_RULE("DOT",        SEQUENCE, rule_DOT_nodes),
            NEETOREE_PARSER_RULE("Spacing",    SEQUENCE, rule_Spacing_nodes),
            NEETOREE_PARSER_RULE("Comment",    SEQUENCE, rule_Comment_nodes),
            NEETOREE_PARSER_RULE("Space",      CHOICE,   rule_Space_nodes),
            NEETOREE_PARSER_RULE("EndOfLine",  CHOICE,   rule_EndOfLine_nodes),
            NEETOREE_PARSER_RULE("EndOfFile",  SEQUENCE, rule_EndOfFile_nodes),
            NEETOREE_PARSER_RULE("Action",     SEQUENCE, rule_Action_nodes),
            NEETOREE_PARSER_RULE("BEGIN",      SEQUENCE, rule_BEGIN_nodes),
            NEETOREE_PARSER_RULE("END",        SEQUENCE, rule_END_nodes),
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
}

