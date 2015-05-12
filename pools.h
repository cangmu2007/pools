#ifndef __POOLS_H__
#define __POOLS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Define to prevent recursive inclusion
-------------------------------------*/
#include "types.h"
#include "list.h"


#define MEM_F_SHARED 0x1                /* 标示对应的池允许共用 */


/* 每个池的相关信息 */
struct pool_head {
    void **free_list;    
    struct list_head list;            /* list of all known pools */
    int32_t used;            /* how many chunks are currently in use */
    int32_t allocated;        /* how many chunks have been allocated */
    int32_t limit;            /* hard limit on the number of chunks */
    int32_t minavail;        /* how many chunks are expected to be used */
    int32_t size;            /* chunk size */
    int32_t flags;            /* MEM_F_* */
    int32_t users;            /* number of pools sharing this zone */
    int8_t name[12];        /* name of the pool */
};


/* 池创建 */
/* Try to find an existing shared pool with the same characteristics and
 * returns it, otherwise creates this one. NULL is returned if no memory
 * is available for a new creation.
 */
extern struct pool_head *pool_create(char *name, uint32_t size, uint32_t flags);


/* 池销毁 */
/*
 * This function destroys a pool by freeing it completely, unless it's still
 * in use. This should be called only under extreme circumstances. It always
 * returns NULL if the resulting pool is empty, easing the clearing of the old
 * pointer, otherwise it returns the pool.
 * .
 */
extern void *pool_destroy(struct pool_head *pool);


/* 把池中的空闲的元素都给释放掉 */
/*
 * This function frees whatever can be freed in pool <pool>.
 */
extern void pool_clear(struct pool_head *pool);


/* 把池中非必要的元素给释放掉 */
/*
 * This function frees whatever can be freed in all pools, but respecting
 * the minimum thresholds imposed by owners. It takes care of avoiding
 * recursion because it may be called from a signal handler.
 */
extern void pool_flush_nonessential(void);


/* 动态分配一个 pool 元素大小的内存空间 */
/* Allocate a new entry for pool <pool>, and return it for immediate use.
 * NULL is returned if no memory is available for a new creation.
 */
extern void *pool_refill_alloc(struct pool_head *pool);



/*
 * Returns a pointer to type <type> taken from the
 * pool <pool_type> or dynamically allocated. In the
 * first case, <pool_type> is updated to point to the
 * next element in the list.
 */
#define pool_alloc(pool) \
({ \
        void *__p; \
        if ((__p = (pool)->free_list) == NULL)            \
                __p = pool_refill_alloc(pool); \
        else { \
                (pool)->free_list = *(void **)(pool)->free_list;\
        (pool)->used++;                    \
        } \
        __p; \
})



/*
 * Puts a memory area back to the corresponding pool.
 * Items are chained directly through a pointer that
 * is written in the beginning of the memory area, so
 * there's no need for any carrier cell. This implies
 * that each memory area is at least as big as one
 * pointer. Just like with the libc's free(), nothing
 * is done if <ptr> is NULL.
 */
#define pool_free(pool, ptr) \
({ \
        if ((ptr) != NULL) { \
                *(void **)(ptr) = (void *)(pool)->free_list;    \
                (pool)->free_list = (void *)(ptr);    \
                (pool)->used--;                \
        } \
})



#ifdef __cplusplus
}
#endif

#endif
