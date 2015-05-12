#ifndef __TYPES_H__
#define __TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Define to prevent recursive inclusion
-------------------------------------*/
#include <stdio.h>     // 标准输入输出定义
#include <stdlib.h>     // 标准函数库定义
#include <string.h>             // memset
#include <stdint.h>

#define    BUFFER_SIZE 256

/* 1，pools池相关 */
#define POOL_CREATE_ERROR -1
#define POOL_DESTROY_ERROR -2
#define CMD_POOL_ALLOC_ERROR -3


/* 2，命令相关*/
#define    CMD_DATA_LEN_MAX        (BUFFER_SIZE)

#define OPER_OK 0


#ifdef __cplusplus
}
#endif

#endif
