#include <stdio.h>
#include "command.h"

static void cmd_pool_oper(void)
{
    int32_t ret = OPER_OK;

    printf("command pool test!\n");

    if((ret = cmd_pool_create()) != OPER_OK)
    {
        printf("Create command pool fail!\n");
    }
    else
    {
        cmd_t* cmd_buf[5];
        int32_t i = 0;
        int32_t count = 0;
        
        printf("Create command pool success!!!\n");

        memset(cmd_buf,0,sizeof(cmd_buf));

        for(i = 0; i < 5; i++)
        {
            printf("alloc \n");
            if(cmd_alloc(&cmd_buf[i]) != OPER_OK)
            {
                printf("Alloc buffer fail : %d\n",i);
                count = i;
                break;
            }
            cmd_buf[i]->fd = i+1;
            strcpy((char*)cmd_buf[i]->data,"hello");
        }

        printf("Alloc complete success!\n");
//        if(i >= 5)    count = i;
        
        for(i = 0 ; i < 5; i++)
        {
            printf("command %d fd : %d,data : %s\n",(i+1),cmd_buf[i]->fd,(char*)cmd_buf[i]->data);
            cmd_free(cmd_buf[i]);
        }

        if((ret = cmd_pool_destroy()) != OPER_OK)
            printf("command pool destroy fail, still in use\n");
        else
            printf("command pool destroy success!\n");
    }
    
}


int main(void)
{
	cmd_pool_oper();
	return 0;
}