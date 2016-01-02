//
// Created by user on 11/7/15.
//

#ifndef NEETOREE_NEETOREE_COMMON_H
#define NEETOREE_NEETOREE_COMMON_H

typedef enum {
    NEETOREE_RESULT_NONE    = 0,
    NEETOREE_RESULT_ERROR   = 1,
    NEETOREE_RESULT_SUCCESS = 2,
    NEETOREE_RESULT_FAILURE = 3
} NeetoreeResult;

typedef enum {
    NEETOREE_STATUS_NONE  = 0,
    NEETOREE_STATUS_ERROR = 1,
    NEETOREE_STATUS_CONT  = 2,
    NEETOREE_STATUS_STOP  = 3
} NeetoreeStatus;

#define NEETOREE_NEW_NAME(name) neetoree_new_## name ##_func
#define NEETOREE_NEW(name, ...) \
    void *NEETOREE_NEW_NAME(name)(__VA_ARGS__)

#define NEETOREE_FREE_NAME(name) neetoree_free_## name ##_func
#define NEETOREE_FREE(name) \
    void NEETOREE_FREE_NAME(name)(void *item)

typedef void (*neetoree_freefunc)(void *item);

#endif //NEETOREE_NEETOREE_COMMON_H
