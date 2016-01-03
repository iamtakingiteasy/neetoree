//
// Created by user on 10/21/15.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "neetoree_stream.h"

typedef struct {
    char *data;
    size_t len;
} neetoree_stream_string_data_t;

static neetoree_stream_chunk_t *neetoree_stream_chunk_ref(neetoree_stream_chunk_t *chunk) {
    chunk->refcount++;
    return chunk->next;
}

static neetoree_stream_chunk_t *neetoree_stream_chunk_unref(neetoree_stream_chunk_t *chunk) {
    neetoree_stream_chunk_t *next = chunk->next;
    if (chunk->refcount == 0) {
        free(chunk->data);
        chunk->data = NULL;
        free(chunk);
    } else {
        chunk->refcount--;
    }
    return next;
}

static neetoree_stream_chunk_t *neetoree_stream_chunk_new(neetoree_stream_global_t *global) {
    neetoree_stream_chunk_t *chunk = calloc(1, sizeof(neetoree_stream_chunk_t));
    chunk->data                    = calloc(1, global->chunksize);
    chunk->serial                  = global->chunkserial++;
    chunk->refcount                = global->refcount;
    return chunk;
}

neetoree_stream_t *neetoree_stream_new(neetoree_stream_produce producer, size_t chunksize, void *context, neetoree_freefunc  contextfree) {
    neetoree_stream_t *stream   = calloc(1, sizeof(neetoree_stream_t));
    stream->global              = calloc(1, sizeof(neetoree_stream_global_t));
    stream->global->chunksize   = chunksize;
    stream->global->produce     = producer;
    stream->global->context     = context;
    stream->global->contextfree = contextfree;

    neetoree_stream_chunk_t *initchunk = neetoree_stream_chunk_new(stream->global);

    stream->init          = stream->current      = initchunk;
    return stream;
}

neetoree_stream_t *neetoree_stream_fork(neetoree_stream_t *parent) {
    neetoree_stream_t *fork = calloc(1, sizeof(neetoree_stream_t));
    fork->parent  = parent;
    fork->global  = parent->global;
    fork->init    = parent->current;
    fork->current = parent->current;
    fork->offset  = parent->offset;
    fork->total   = parent->total;
    fork->merged  = 0;

    if (parent->handlerctx) {
        fork->handlerctx = parent->global->handlerclone(parent->handlerctx);
    }

    parent->global->refcount++;

    neetoree_stream_chunk_t *chunk = parent->current;

    while (chunk) {
        chunk = neetoree_stream_chunk_ref(chunk);
    }

    return fork;
}

void neetoree_stream_commit(neetoree_stream_t *fork) {
    if (fork->parent->current->serial <= fork->current->serial) {
        if (fork->parent->current != fork->current) {
            neetoree_stream_chunk_t *chunk = fork->parent->init;
            while (chunk != fork->current) {
                chunk = neetoree_stream_chunk_unref(chunk);
            }
            fork->parent->init = chunk;
        }

        fork->parent->total   = fork->total;
        fork->parent->init    = fork->current;
        fork->parent->current = fork->current;
        fork->parent->offset  = fork->offset;
        fork->merged = 1;
    }
}

void neetoree_stream_free(neetoree_stream_t *stream) {
    neetoree_stream_chunk_t *chunk = stream->init;
    while (chunk) {
        chunk = neetoree_stream_chunk_unref(chunk);
    }

    if (stream->handlerctx) {
        if (stream->merged || stream->parent->handlerctx == stream->global->handlerinit) {
            stream->global->handlermerge(stream->parent->handlerctx, stream->handlerctx);
        } else {
            stream->global->handlermerge(NULL, stream->handlerctx);
        }
    }

    if (!stream->parent) {
        if (stream->global->contextfree) {
            stream->global->contextfree(stream->global->context);
        }
        free(stream->global);
    } else {
        stream->global->refcount--;
    }
    free(stream);
}

static size_t neetoree_stream_ensure_read(neetoree_stream_t *stream, size_t requested) {
    neetoree_stream_chunk_t *chunk = stream->current;
    size_t available = chunk->length - stream->offset;
    if (chunk->next) {
        chunk = chunk->next;
        while (available < requested) {
            available += chunk->length;
            if (!chunk->next) {
                break;
            }
            chunk = chunk->next;
        }

        if (available >= requested) {
            return requested;
        }
    }

    while (chunk && available < requested) {
        char *out = chunk->data + chunk->length;
        size_t need = stream->global->chunksize - chunk->length;

        if (need == 0) {
            chunk->next = neetoree_stream_chunk_new(stream->global);
            chunk = chunk->next;
            out = chunk->data;
            need = stream->global->chunksize;
        }

        if (need > requested - available) {
            need = requested - available;
        }

        size_t read = stream->global->produce(stream, out, stream->total + available, need, stream->global->context);

        if (read == -1) {
            return (size_t) -1;
        }
        //stream->total += read;
        chunk->length += read;
        available += read;
    }

    if (available > requested) {
        return requested;
    }

    return available;
}

size_t neetoree_stream_read(neetoree_stream_t *stream, char *data, size_t siz) {
    size_t read = 0;
    size_t available = neetoree_stream_ensure_read(stream, siz);
    if (available == -1) {
        return 0;
    }

    neetoree_stream_chunk_t *chunk = stream->current;

    while (read < available) {
        size_t need = chunk->length - stream->offset;
        if (need == 0) {
            chunk = chunk->next;
            if (chunk) {
                stream->current = chunk;
                stream->offset  = 0;
                need            = chunk->length;
            } else {
                break;
            }
        }

        if (need > available - read) {
            need = available - read;
        }
        if (data) {
            memmove(data + read, chunk->data + stream->offset, need);
        }
        read += need;
        stream->offset += need;
    }
    stream->total += read;
    if (stream->handlerctx) {
        stream->global->handler(data, read, stream->handlerctx);
    }
    return read;
}

static size_t neetoree_file_stream_produce(neetoree_stream_t *stream, char *out, size_t off, size_t siz, void *context) {
    FILE *f = context;
    if (feof(f)) {
        return (size_t) -1;
    }
    return fread(out, 1, siz, f);
}

neetoree_stream_t *neetoree_stream_file_new(FILE *f, size_t buflen) {
    return neetoree_stream_new(neetoree_file_stream_produce, buflen, f, NULL);
}

static size_t neetoree_stream_string_producer(neetoree_stream_t *stream, char *out, size_t off, size_t siz, void *context) {
    neetoree_stream_string_data_t *strdata = context;
    size_t read = siz;
    if (read + off > strdata->len) {
        read = strdata->len - off;
    }
    if (read == 0) {
        return (size_t)-1;
    }
    memmove(out, strdata->data + off, read);
    return read;
}

neetoree_stream_t *neetoree_stream_string_new(char *data, size_t length, size_t buflen) {
    neetoree_stream_string_data_t *strdata = calloc(1, sizeof(neetoree_stream_string_data_t));
    strdata->data = data;
    strdata->len  = length;
    return neetoree_stream_new(neetoree_stream_string_producer, buflen, strdata, free);
}

size_t neetoree_stream_pos(neetoree_stream_t *stream) {
    return stream->current->serial * stream->global->chunksize + stream->offset;
}

size_t neetoree_stream_advance(neetoree_stream_t *stream, size_t adv) {
    return neetoree_stream_read(stream, NULL, adv);
}

void neetoree_stream_set_handler(
        neetoree_stream_t *stream,
        neetoree_stream_handler handler,
        neetoree_stream_handler_context_clone handlerclone,
        neetoree_stream_handler_context_merge handlermerge
) {
    stream->global->handler = handler;
    stream->global->handlerclone = handlerclone;
    stream->global->handlermerge = handlermerge;
    stream->handlerctx = handlerclone(NULL);
    stream->global->handlerinit = stream->handlerctx;
}

void neetoree_stream_commit_handler(neetoree_stream_t *stream, void *init) {
    stream->global->handlermerge(init, stream->handlerctx);
    stream->handlerctx = NULL;
}