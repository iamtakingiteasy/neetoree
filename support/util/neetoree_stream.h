//
// Created by user on 10/21/15.
//

#ifndef NEETOREE_NEETOREE_STREAM_H
#define NEETOREE_NEETOREE_STREAM_H

#include <stdio.h>
#include "neetoree_common.h"

struct neetoree_stream_t;

#define NEETOREE_STREAM_HANDLER_ARGS \
    char *data, \
    size_t len, \
    void *context
typedef void (*neetoree_stream_handler)(NEETOREE_STREAM_HANDLER_ARGS);
#define NEETOREE_STREAM_HANDLER_NAME(name) neetoree_stream_## name ##_handler
#define NEETOREE_STREAM_HANDLER(name) \
    void NEETOREE_STREAM_HANDLER_NAME(name)(NEETOREE_STREAM_HANDLER_ARGS)

#define NEETOREE_STREAM_HANDLER_CONTEXT_CLONE_ARGS \
    void *parent
typedef void *(*neetoree_stream_handler_context_clone)(NEETOREE_STREAM_HANDLER_CONTEXT_CLONE_ARGS);
#define NEETOREE_STREAM_HANDLER_CONTEXT_CLONE_NAME(name) neetoree_stream_## name ##_handler_context_clone
#define NEETOREE_STREAM_HANDLER_CONTEXT_CLONE(name) \
  void *NEETOREE_STREAM_HANDLER_CONTEXT_CLONE_NAME(name)(NEETOREE_STREAM_HANDLER_CONTEXT_CLONE_ARGS)

#define NEETOREE_STREAM_HANDLER_CONTEXT_MERGE_ARGS \
    void *a, \
    void *b
typedef void (*neetoree_stream_handler_context_merge)(NEETOREE_STREAM_HANDLER_CONTEXT_MERGE_ARGS);
#define NEETOREE_STREAM_HANDLER_CONTEXT_MERGE_NAME(name) neetoree_stream_## name ##_handler_context_merge
#define NEETOREE_STREAM_HANDLER_CONTEXT_MERGE(name) \
   void NEETOREE_STREAM_HANDLER_CONTEXT_MERGE_NAME(name)(NEETOREE_STREAM_HANDLER_CONTEXT_MERGE_ARGS)


typedef size_t (*neetoree_stream_produce)(struct neetoree_stream_t *stream, char *out, size_t off, size_t siz, void *context);

typedef struct neetoree_stream_global_t {
    size_t                                 chunkserial;
    size_t                                 refcount;
    size_t                                 chunksize;
    neetoree_stream_produce                produce;
    neetoree_freefunc                      contextfree;
    void                                  *context;
    neetoree_stream_handler                handler;
    neetoree_stream_handler_context_clone  handlerclone;
    neetoree_stream_handler_context_merge  handlermerge;
    void                                  *handlerinit;
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
    void                            *handlerctx;
    int                              merged;
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
void neetoree_stream_set_handler(
        neetoree_stream_t *stream,
        neetoree_stream_handler handler,
        neetoree_stream_handler_context_clone handlerclone,
        neetoree_stream_handler_context_merge handlermerge
);
void neetoree_stream_commit_handler(neetoree_stream_t *stream, void *init);

#endif //NEETOREE_NEETOREE_STREAM_H
