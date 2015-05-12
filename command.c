#include "command.h"
#include "pools.h"


static struct pool_head *cmd_head_pool = NULL;
static struct pool_head *cmd_data_pool = NULL;


// 创建内存池
int32_t cmd_pool_create(void)
{
    int32_t ret = OPER_OK;
    
    if (cmd_head_pool == NULL)
    {
        if((cmd_head_pool = pool_create("cmd_head", sizeof(cmd_t), MEM_F_SHARED)) == NULL)
        {
            ret = -POOL_CREATE_ERROR;
        }
        else
        {
            if (cmd_data_pool == NULL)
                if((cmd_data_pool = pool_create("cmd_data", CMD_DATA_LEN_MAX, MEM_F_SHARED)) == NULL)
                {
                    cmd_pool_destroy();        
                    ret = -POOL_CREATE_ERROR;
                }
        }
    }

#ifdef DEBUG_POOL
    cmd_pool_info();
#endif

    return ret;
}


#ifdef DEBUG_POOL
void        cmd_pool_info(void)
{
    struct pool_head *entry = cmd_head_pool;
    printf("pool %s (%d bytes) : %d allocated (%u bytes), %d used\n",entry->name, entry->size, entry->allocated,entry->size * entry->allocated, entry->used);

    entry = cmd_data_pool;
    printf("pool %s (%d bytes) : %d allocated (%u bytes), %d used\n",entry->name, entry->size, entry->allocated,entry->size * entry->allocated, entry->used);
}
#endif


// 释放内存池
int32_t cmd_pool_destroy(void)
{
    int32_t ret = OPER_OK;

#ifdef DEBUG_POOL
    cmd_pool_info();
#endif

    if(cmd_head_pool != NULL)
    {
        if(NULL != pool_destroy(cmd_head_pool))
        {
            ret = -POOL_DESTROY_ERROR;
        }
        else
        {
            if(cmd_data_pool != NULL)
                if(NULL != pool_destroy(cmd_data_pool))
                    ret = -POOL_DESTROY_ERROR;
        }
    }

    return ret;
}


// 申请命令内存块，用以保存命令数据
int32_t cmd_alloc(cmd_t **param)
{
    int32_t ret = OPER_OK;

    if((*param = (cmd_t*)pool_alloc(cmd_head_pool)) == NULL)
    {
        ret = -CMD_POOL_ALLOC_ERROR;
    }
    else
    {
        memset(*param,0,sizeof(cmd_t));
        
        if(((*param)->data = pool_alloc(cmd_data_pool)) == NULL)
        {
            cmd_free(*param);
            ret = -CMD_POOL_ALLOC_ERROR;
        }
    }

#ifdef DEBUG_POOL
    cmd_pool_info();
#endif
    
    return ret;
}


// 释放命令内存块
int32_t cmd_free(cmd_t *param)
{
    if(param->data != NULL)
    {
        pool_free(cmd_data_pool,param->data);
        param->data = NULL;
    }
    
    if(param != NULL)
    {
        pool_free(cmd_head_pool,param);
        param = NULL;
    }
    
#ifdef DEBUG_POOL
    cmd_pool_info();
#endif
    return OPER_OK;
}
