//
// Created by user on 11/16/15.
//

#include "neetoree_parser.h"

#include <string.h>
#include <neetoree_string.h>

// sample invocation: ARGV[0] ../assets/peg.grammar neetoree_parser_peg_impl neetoree_parser_actions.inc
int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Error, not enough arguments\n");
        return 2;
    }

    FILE *f = fopen(argv[1], "r");
    neetoree_stream_t *stream = neetoree_stream_file_new(f, 256);
    neetoree_string_t *parser = neetoree_parser_peg(argv[2], argv[3], stream);
    fclose(f);

    if (parser == NULL) {
        neetoree_stream_free(stream);
        return 1;
    }

    printf("%s\n", parser->data);
    neetoree_string_free(parser);
    neetoree_stream_free(stream);

    return 0;
}