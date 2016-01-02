//
// Created by user on 11/7/15.
//

#ifndef NEETOREE_NEETOREE_AVLTREE_H
#define NEETOREE_NEETOREE_AVLTREE_H

#include <stdint.h>
#include "neetoree_common.h"

#define NEETOREE_AVLTREE_HASH_NAME(name) neetoree_avltree_## name ##_hash
#define NEETOREE_AVLTREE_HASH(name) \
    uint64_t NEETOREE_AVLTREE_HASH_NAME(name)(void *item)

typedef uint64_t (*neetoree_avltree_hashfunc)(void *item);

typedef struct neetoree_avltree_bucket_t {
    void     *ptr;
    uint64_t  hash;
    int       height;
    struct neetoree_avltree_bucket_t *left;
    struct neetoree_avltree_bucket_t *right;
} neetoree_avltree_bucket_t;

typedef struct {
    neetoree_avltree_hashfunc  hashfunc;
    neetoree_freefunc          freefunc;
    neetoree_avltree_bucket_t *bucket;
} neetoree_avltree_t;

neetoree_avltree_t *neetoree_avltree_new(neetoree_avltree_hashfunc hashfunc, neetoree_freefunc freefunc);
void  neetoree_avltree_insert(neetoree_avltree_t *avl, void *item);
void  neetoree_avltree_remove(neetoree_avltree_t *avl, uint64_t hash);
void *neetoree_avltree_lookup(neetoree_avltree_t *avl, uint64_t hash);
void  neetoree_avltree_free(neetoree_avltree_t *avl);

#endif //NEETOREE_NEETOREE_AVLTREE_H
