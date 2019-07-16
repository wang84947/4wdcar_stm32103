/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-08-24     yangjie      the first version
 */

/*
 * 程序清单：动态内存管理例程
 *
 * 这个程序会创建一个动态的线程，这个线程会动态申请内存并释放
 * 每次申请更大的内存，当申请不到的时候就结束
 */
#include <rtthread.h>

#define THREAD_PRIORITY      25
#define THREAD_STACK_SIZE    1024
#define THREAD_TIMESLICE     5

/* 线程入口 */
void thread1_entry(void *parameter)
{
   unsigned int i ,j;
    char *ptr[] = {RT_NULL}; /* 内存块的指针 */
 char *ptr1 = RT_NULL; /* 内存块的指针 */
//	ptr1 = rt_malloc(0x23a3);
    for (i = 0; ; i++)
    {
        /* 每次分配 (1 << i) 大小字节数的内存空间 */
        ptr[i] = rt_malloc(1024);

        /* 如果分配成功 */
        if (ptr[i] != RT_NULL)
        {
            rt_kprintf("get memory :%d %x %d byte\n", (i),ptr[i],1024);
          
        }
        else
        {
          rt_kprintf("try to get %d byte memory failed!\n", (i));
					for(j=i;j>0;j--)
					{
					  /* 释放内存块 */				
            rt_free(ptr[j]);
            rt_kprintf("free memory :%d %xbyte\n", (j),ptr[j]);
            ptr[j] = RT_NULL;					
					}
					
            return;
        }
    }
}

int dynmem_sample(void)
{
    rt_thread_t tid = RT_NULL;

    /* 创建线程1 */
    tid = rt_thread_create("thread1",
                           thread1_entry, RT_NULL,
                           THREAD_STACK_SIZE,
                           THREAD_PRIORITY,
                           THREAD_TIMESLICE);
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(dynmem_sample, dynmem sample);
