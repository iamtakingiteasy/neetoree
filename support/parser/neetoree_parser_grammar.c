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
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "LeftArrow",   NULL),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Expression",  NULL),
            NEETOREE_PARSER_NODE_ACTION(rule_commit),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Expression_1_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Slash",       NULL),
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
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "And",         NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Action",      NULL),
            NEETOREE_PARSER_NODE_ACTION(type_action),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Prefix_1_2_1_1_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "And",      NULL),
            NEETOREE_PARSER_NODE_ACTION(pred_and),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Prefix_1_2_1_2_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Not",      NULL),
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
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Query",             NULL),
            NEETOREE_PARSER_NODE_ACTION(cardinality_query),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Suffix_1_2_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Star",             NULL),
            NEETOREE_PARSER_NODE_ACTION(cardinality_star),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Suffix_1_3_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Plus",             NULL),
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
            NEETOREE_PARSER_NODE(REF,       ONE,        NEGATIVE, "LeftArrow",  NULL),
            NEETOREE_PARSER_NODE_ACTION(type_ref),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_2_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Open",       NULL),
            NEETOREE_PARSER_NODE_ACTION(group),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Expression", NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Close",      NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_3_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Dot",      NULL),
            NEETOREE_PARSER_NODE_ACTION(type_dot),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_4_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Action",      NULL),
            NEETOREE_PARSER_NODE_ACTION(type_action),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_5_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Begin",      NULL),
            NEETOREE_PARSER_NODE_ACTION(type_begin),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_6_nodes[] = {
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "End",      NULL),
            NEETOREE_PARSER_NODE_ACTION(type_end),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Primary_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL,      rule_Primary_1_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL,      rule_Primary_2_nodes),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Literal", NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Class",   NULL),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL,      rule_Primary_3_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL,      rule_Primary_4_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL,      rule_Primary_5_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL,      rule_Primary_6_nodes),
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
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NONE,     "'",            NULL),
            NEETOREE_PARSER_NODE_BEGIN,
            NEETOREE_PARSER_NODE(SEQUENCE,  ZEROORMORE, NONE,     NULL,           rule_Literal_1_1_nodes),
            NEETOREE_PARSER_NODE_END,
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NONE,     "'",            NULL),
            NEETOREE_PARSER_NODE_ACTION(type_literal),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing",      NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Literal_2_1_nodes[] = {
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NEGATIVE, "\"",   NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Char", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Literal_2_nodes[] = {
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NONE,     "\"",           NULL),
            NEETOREE_PARSER_NODE_BEGIN,
            NEETOREE_PARSER_NODE(SEQUENCE,  ZEROORMORE, NONE,     NULL,           rule_Literal_2_1_nodes),
            NEETOREE_PARSER_NODE_END,
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NONE,     "\"",           NULL),
            NEETOREE_PARSER_NODE_ACTION(type_literal),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing",      NULL),
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
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "[",          NULL),
            NEETOREE_PARSER_NODE_BEGIN,
            NEETOREE_PARSER_NODE(SEQUENCE,  ZEROORMORE, NONE,     NULL,         rule_Class_1_nodes),
            NEETOREE_PARSER_NODE_END,
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "]",          NULL),
            NEETOREE_PARSER_NODE_ACTION(type_class),
            NEETOREE_PARSER_NODE_ACTION(set_spec),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing",    NULL),
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
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "\\",                NULL),
            NEETOREE_PARSER_NODE(RANGE,     ONE,        NONE,     "abefnrtv'\"\\[\\]\\\\", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Char_2_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "\\", NULL),
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "-",  NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Char_3_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NEGATIVE, "\\", NULL),
            NEETOREE_PARSER_NODE(DOT,       ONE,        NONE,     NULL, NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Char_nodes[] = {
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Char_1_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Char_2_nodes),
            NEETOREE_PARSER_NODE(SEQUENCE,  ONE,        NONE,     NULL, rule_Char_3_nodes),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_LeftArrow_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "<-",      NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Slash_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "/",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_And_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "&",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Not_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "!",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Query_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "?",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Star_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "*",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Plus_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "+",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Open_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "(",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Close_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     ")",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_Dot_nodes[] = {
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
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "\t",        NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "EndOfLine", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_EndOfLine_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "\r\n", NULL),
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "\n",   NULL),
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "\r",   NULL),
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

    neetoree_parser_node_t rule_Begin_nodes[] = {
            NEETOREE_PARSER_NODE(LITERAL,   ONE,        NONE,     "<",       NULL),
            NEETOREE_PARSER_NODE(REF,       ONE,        NONE,     "Spacing", NULL),
            NEETOREE_PARSER_NODE_TERM
    };

    neetoree_parser_node_t rule_End_nodes[] = {
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
            NEETOREE_PARSER_RULE("LeftArrow",  SEQUENCE, rule_LeftArrow_nodes),
            NEETOREE_PARSER_RULE("Slash",      SEQUENCE, rule_Slash_nodes),
            NEETOREE_PARSER_RULE("And",        SEQUENCE, rule_And_nodes),
            NEETOREE_PARSER_RULE("Not",        SEQUENCE, rule_Not_nodes),
            NEETOREE_PARSER_RULE("Query",      SEQUENCE, rule_Query_nodes),
            NEETOREE_PARSER_RULE("Star",       SEQUENCE, rule_Star_nodes),
            NEETOREE_PARSER_RULE("Plus",       SEQUENCE, rule_Plus_nodes),
            NEETOREE_PARSER_RULE("Open",       SEQUENCE, rule_Open_nodes),
            NEETOREE_PARSER_RULE("Close",      SEQUENCE, rule_Close_nodes),
            NEETOREE_PARSER_RULE("Dot",        SEQUENCE, rule_Dot_nodes),
            NEETOREE_PARSER_RULE("Spacing",    SEQUENCE, rule_Spacing_nodes),
            NEETOREE_PARSER_RULE("Comment",    SEQUENCE, rule_Comment_nodes),
            NEETOREE_PARSER_RULE("Space",      CHOICE,   rule_Space_nodes),
            NEETOREE_PARSER_RULE("EndOfLine",  CHOICE,   rule_EndOfLine_nodes),
            NEETOREE_PARSER_RULE("EndOfFile",  SEQUENCE, rule_EndOfFile_nodes),
            NEETOREE_PARSER_RULE("Action",     SEQUENCE, rule_Action_nodes),
            NEETOREE_PARSER_RULE("Begin",      SEQUENCE, rule_Begin_nodes),
            NEETOREE_PARSER_RULE("End",        SEQUENCE, rule_End_nodes),
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

