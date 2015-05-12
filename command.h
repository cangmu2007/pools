#ifndef __COMMAND_H__
#define __COMMAND_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Define to prevent recursive inclusion
-------------------------------------*/
#include "types.h"


/* 接收到的命令的来源等 */
typedef struct _cmd
{
    int32_t fd;
    pthread_t id;
    void* data;
}cmd_t;



// 创建内存池
extern int32_t cmd_pool_create(void);


// 释放内存池
extern int32_t cmd_pool_destroy(void);


// 申请命令内存块，用以保存命令数据
extern int32_t cmd_alloc(cmd_t **param);

// 释放命令内存块
extern int32_t cmd_free(cmd_t *param);

#ifdef DEBUG_POOL
extern void cmd_pool_info(void);
#endif


#ifdef __cplusplus
}
#endif

#endif
