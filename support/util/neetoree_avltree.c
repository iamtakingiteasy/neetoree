//
// Created by user on 11/7/15.
//

#include <stdlib.h>
#include "neetoree_avltree.h"

neetoree_avltree_t *neetoree_avltree_new(neetoree_avltree_hashfunc hashfunc, neetoree_freefunc freefunc) {
    neetoree_avltree_t *avl= calloc(1, sizeof(neetoree_avltree_t));
    avl->hashfunc = hashfunc;
    avl->freefunc = freefunc;
    return avl;
}

static neetoree_avltree_bucket_t *neetoree_avltree_bucket_new(void *item, uint64_t hash) {
    neetoree_avltree_bucket_t *bucket = calloc(1, sizeof(neetoree_avltree_bucket_t));
    bucket->hash   = hash;
    bucket->ptr    = item;
    return bucket;
}

static void neetoree_avltree_bucket_free(neetoree_avltree_bucket_t *bucket) {
    free(bucket);
}

static int height(neetoree_avltree_bucket_t *bucket) {
    if (bucket == NULL) {
        return 0;
    }
    return bucket->height;
}

static int neetoree_avltree_heightmax(neetoree_avltree_bucket_t *bucket) {
    int hleft = height(bucket->left);
    int hright = height(bucket->right);
    return ((hleft > hright) ? hleft : hright) + 1;
}

static int neetoree_avltree_balance(neetoree_avltree_bucket_t *bucket) {
    if (bucket == NULL) {
        return 0;
    }
    return height(bucket->left) - height(bucket->right);
}

static neetoree_avltree_bucket_t *rotateright(neetoree_avltree_bucket_t *bucket) {
    neetoree_avltree_bucket_t *lever = bucket->left;
    neetoree_avltree_bucket_t *tmp = lever->right;

    lever->right = bucket;
    bucket->left = tmp;

    bucket->height = neetoree_avltree_heightmax(bucket);
    lever->height  = neetoree_avltree_heightmax(lever);

    return lever;
}

static neetoree_avltree_bucket_t *rotateleft(neetoree_avltree_bucket_t *bucket) {
    neetoree_avltree_bucket_t *lever = bucket->right;
    neetoree_avltree_bucket_t *tmp = lever->left;

    lever->left = bucket;
    bucket->right = tmp;

    bucket->height = neetoree_avltree_heightmax(bucket);
    lever->height  = neetoree_avltree_heightmax(lever);

    return lever;
}

static neetoree_avltree_bucket_t *neetoree_avltree_insert_walker(neetoree_avltree_bucket_t *bucket, uint64_t hash, void *ptr) {
    if (bucket == NULL) {
        return neetoree_avltree_bucket_new(ptr, hash);
    }
    if (hash < bucket->hash) {
        bucket->left = neetoree_avltree_insert_walker(bucket->left, hash, ptr);
    } else {
        bucket->right = neetoree_avltree_insert_walker(bucket->right, hash, ptr);
    }

    bucket->height = neetoree_avltree_heightmax(bucket);
    int balance = neetoree_avltree_balance(bucket);

    if (balance > 1 && hash < bucket->left->hash) {
        return rotateright(bucket);
    }

    if (balance < -1 && hash > bucket->right->hash) {
        return rotateleft(bucket);
    }

    if (balance > 1 && hash > bucket->left->hash) {
        bucket->left = rotateleft(bucket->left);
        return rotateright(bucket);
    }

    if (balance < -1 && hash < bucket->right->hash) {
        bucket->right = rotateright(bucket->right);
        return rotateleft(bucket);
    }

    return bucket;
}

void neetoree_avltree_insert(neetoree_avltree_t *avl, void *item) {
    uint64_t hash = avl->hashfunc(item);
    if (avl->bucket == NULL) {
        avl->bucket = neetoree_avltree_insert_walker(NULL, hash, item);
        return;
    }
    neetoree_avltree_insert_walker(avl->bucket, hash, item);
}

static neetoree_avltree_bucket_t *neetoree_avltree_minvaluenode(neetoree_avltree_bucket_t *bucket) {
    while (bucket->left) {
        bucket = bucket->left;
    }
    return bucket;
}

static neetoree_avltree_bucket_t *neetoree_avltree_remove_walker(neetoree_avltree_bucket_t *bucket, uint64_t hash, neetoree_freefunc freefunc) {
    if (!bucket) {
        return NULL;
    }

    if (hash < bucket->hash) {
        bucket->left = neetoree_avltree_remove_walker(bucket->left, hash, freefunc);
    } else if (hash > bucket->hash) {
        bucket->right = neetoree_avltree_remove_walker(bucket->right, hash, freefunc);
    } else {
        freefunc(bucket->ptr);
        if (!bucket->left || !bucket->right) {
            neetoree_avltree_bucket_t *tmp = bucket->left ? bucket->left : bucket->right;
            if (!tmp) {
                neetoree_avltree_bucket_free(bucket);
                return NULL;
            } else {
                *bucket = *tmp;
                neetoree_avltree_bucket_free(tmp);
            }
        } else {
            neetoree_avltree_bucket_t *tmp = neetoree_avltree_minvaluenode(bucket->right);
            bucket->ptr = tmp->ptr;
            bucket->hash = tmp->hash;
            bucket->right = neetoree_avltree_remove_walker(bucket->right, tmp->hash, freefunc);
        }
    }

    bucket->height = neetoree_avltree_heightmax(bucket);
    int balance = neetoree_avltree_balance(bucket);
    int leftbalance = neetoree_avltree_balance(bucket->left);
    if (balance > 1 && leftbalance >= 0) {
        return rotateright(bucket);
    }
    if (balance > 1 && leftbalance < 0) {
        bucket->left = rotateleft(bucket->left);
        return rotateright(bucket);
    }

    int rightbalance = neetoree_avltree_balance(bucket->right);

    if (balance < -1 && rightbalance <= 0) {
        return rotateleft(bucket);
    }
    if (balance < -1 && rightbalance > 0) {
        bucket->right = rotateright(bucket->right);
        return rotateleft(bucket);
    }

    return bucket;
}

void neetoree_avltree_remove(neetoree_avltree_t *avl, uint64_t hash) {
    neetoree_avltree_remove_walker(avl->bucket, hash, avl->freefunc);
}

void *neetoree_avltree_lookup(neetoree_avltree_t *avl, uint64_t hash) {
    neetoree_avltree_bucket_t *bucket = avl->bucket;
    while (bucket && bucket->hash != hash) {
        if (hash > bucket->hash) {
            bucket = bucket->right;
        } else {
            bucket = bucket->left;
        }
    }

    if (bucket) {
        return bucket->ptr;
    }

    return NULL;
}

static void neetoree_avltree_free_walker(neetoree_avltree_bucket_t *bucket, neetoree_freefunc freefunc) {
    if (!bucket) {
        return;
    }
    neetoree_avltree_free_walker(bucket->left, freefunc);
    neetoree_avltree_free_walker(bucket->right, freefunc);
    freefunc(bucket->ptr);
    neetoree_avltree_bucket_free(bucket);
}

void neetoree_avltree_free(neetoree_avltree_t *avl) {
    neetoree_avltree_free_walker(avl->bucket, avl->freefunc);
    free(avl);
}
