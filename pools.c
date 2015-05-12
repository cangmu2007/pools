#include "pools.h"
#include "standard.h"


/* 管理所有池的链表头 */
static struct list_head pools = LIST_HEAD_INIT(pools);




/* 池创建 */
/* Try to find an existing shared pool with the same characteristics and
 * returns it, otherwise creates this one. NULL is returned if no memory
 * is available for a new creation.
 */
struct pool_head *pool_create(char *name, uint32_t size, uint32_t flags)
{
    struct pool_head *pool;
    struct pool_head *entry;
    struct list_head *start;
    uint32_t align;

    /* We need to store at least a (void *) in the chunks. Since we know
     * that the malloc() function will never return such a small size,
     * let's round the size up to something slightly bigger, in order to
     * ease merging of entries. Note that the rounding is a power of two.
     */

    align = 16;
    size = (size + align - 1) & -align;

    start = &pools;
    pool = NULL;

    list_for_each_entry(entry, &pools, list) {
        if (entry->size == size) {
            /* either we can share this place and we take it, or
             * we look for a sharable one or for the next position
             * before which we will insert a new one.
             */
            if (flags & entry->flags & MEM_F_SHARED) {
                /* we can share this one */
                pool = entry;
                break;
            }
        }
        else if (entry->size > size) {
            /* insert before this one */
            start = &entry->list;
            break;
        }
    }

    if (!pool) {
        pool = calloc(1, sizeof(*pool));
        if (!pool)
            return NULL;
        if (name)
            strlcpy(pool->name, (int8_t*)name, sizeof(pool->name));
        pool->size = size;
        pool->flags = flags;
        list_add_tail(&pool->list,start);
    }
    pool->users++;
    return pool;
}



/* 池销毁 */
void* pool_destroy(struct pool_head *pool)
{
    if (pool)
    {
        pool_clear(pool);            // 请看池中的空闲的元素
        if (pool->used)
            return pool;
        pool->users--;
        if (!pool->users)
        {
            list_del(&pool->list);    // 从 pools 链表中删除
            free(pool);                // 把 pool 结构体占用的内存给释放了
        }
    }
    return NULL;
}



/* 把池中的空闲的元素都给释放掉 */
/*
 * This function frees whatever can be freed in pool <pool>.
 */
void pool_clear(struct pool_head *pool)
{
    void *temp, *next;
    if (!pool)
        return;

    next = pool->free_list;
    while (next) {
        temp = next;
        next = *(void **)temp;
        pool->allocated--;
        free(temp);
    }
    pool->free_list = next;

    /* here, we should have pool->allocate == pool->used */
}



/* 把池中非必要的元素给释放掉 */
/*
 * This function frees whatever can be freed in all pools, but respecting
 * the minimum thresholds imposed by owners. It takes care of avoiding
 * recursion because it may be called from a signal handler.
 */
void pool_flush_nonessential(void)
{
    static int recurse;
    struct pool_head *entry;

    if (recurse++)
        goto out;

    list_for_each_entry(entry, &pools, list) {
        void *temp, *next;
        //qfprintf(stderr, "Flushing pool %s\n", entry->name);
        next = entry->free_list;
        while (next &&
         entry->allocated > entry->minavail &&
         entry->allocated > entry->used) {
            temp = next;
            next = *(void **)temp;
            entry->allocated--;
            free(temp);
        }
        entry->free_list = next;
    }
 out:
    recurse--;
}


/* 动态分配一个 pool 元素大小的内存空间 */
/* Allocate a new entry for pool <pool>, and return it for immediate use.
 * NULL is returned if no memory is available for a new creation. A call
 * to the garbage collector is performed before returning NULL.
 */
void *pool_refill_alloc(struct pool_head *pool)
{
    void *ret;

    if (pool->limit && (pool->allocated >= pool->limit))
        return NULL;
    ret = calloc(1, pool->size);
    if (!ret) {
        pool_flush_nonessential();
        ret = calloc(1, pool->size);
        if (!ret)
            return NULL;
    }

    pool->allocated++;
    pool->used++;
    return ret;
}


/* 销毁所有的池 */
int32_t dump_pools(void)
{
    int32_t ret = OPER_OK;

    return ret;
}
