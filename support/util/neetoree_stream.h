//
// Created by user on 10/21/15.
//

#ifndef NEETOREE_NEETOREE_STREAM_H
#define NEETOREE_NEETOREE_STREAM_H

#include <stdio.h>
#include "neetoree_common.h"

struct neetoree_stream_t;

typedef size_t (*neetoree_stream_produce)(struct neetoree_stream_t *stream, char *out, size_t off, size_t siz, void *context);

typedef struct neetoree_stream_global_t {
    size_t                          chunkserial;
    size_t                          refcount;
    size_t                          chunksize;
    neetoree_stream_produce         produce;
    neetoree_freefunc               contextfree;
    void                           *context;
} neetoree_stream_global_t;

typedef struct neetoree_stream_chunk_t {
    size_t                          serial;
    size_t                          length;
    size_t                          refcount;
    struct neetoree_stream_chunk_t *next;
    char                           *data;
} neetoree_stream_chunk_t;

typedef struct neetoree_stream_t {
    size_t                           total;
    size_t                           offset;
    struct neetoree_stream_global_t *global;
    struct neetoree_stream_chunk_t  *init;
    struct neetoree_stream_chunk_t  *current;
    struct neetoree_stream_t        *parent;
} neetoree_stream_t;


neetoree_stream_t *neetoree_stream_file_new(FILE *f, size_t buflen);
neetoree_stream_t *neetoree_stream_string_new(char *data, size_t length, size_t buflen);
neetoree_stream_t *neetoree_stream_new(neetoree_stream_produce producer, size_t chunksize, void *context, neetoree_freefunc contextfree);
neetoree_stream_t *neetoree_stream_fork(neetoree_stream_t *parent);

size_t neetoree_stream_pos(neetoree_stream_t *stream);
size_t neetoree_stream_advance(neetoree_stream_t *stream, size_t adv);
void neetoree_stream_commit(neetoree_stream_t *fork);
void neetoree_stream_free(neetoree_stream_t *stream);
size_t neetoree_stream_read(neetoree_stream_t *stream, char *data, size_t siz);

#endif //NEETOREE_NEETOREE_STREAM_H
