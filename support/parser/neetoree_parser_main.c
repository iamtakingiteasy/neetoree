//
// Created by user on 11/16/15.
//

#include "neetoree_parser.h"

#include <string.h>
#include <neetoree_string.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        return 0;
    }
    FILE *f = fopen(argv[1], "r");
    neetoree_stream_t *stream = neetoree_stream_file_new(f, 8);
    neetoree_string_t *parser = neetoree_parser_peg("neetoree_parser_peg_impl", "#include \"neetoree_parser_actions.inc\"", stream);
    fclose(f);

    if (parser == NULL) {
        printf("\n---\nERROR\n");
        neetoree_stream_free(stream);
        return 1;
    }

    printf("\n---\n%s\n", parser->data);
    neetoree_string_free(parser);
    neetoree_stream_free(stream);

    return 0;
}