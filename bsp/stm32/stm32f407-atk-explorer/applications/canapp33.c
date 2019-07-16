/*
 * File      : canapp.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       		Notes
 * 2015-05-14     aubrcool@qq.com 	first version
 */

#include <board.h>
#include <rtthread.h>
#include <rtdevice.h>
#ifdef RT_USING_CAN
//#include "drv_lpccan.h"

struct can_app_struct
{
    const char* name;
    struct rt_can_filter_config * filter;
    rt_uint8_t eventopt;
    struct rt_semaphore sem;
	  rt_err_t (*rx_ind)(rt_device_t dev, rt_size_t size);
};

static struct can_app_struct can_data[1];
static rt_err_t can1ind(rt_device_t dev,  rt_size_t size)

{
    rt_sem_release(&can_data[0].sem);
    return RT_EOK;
}
static rt_err_t  can1ind_envent(rt_device_t dev,  void *args, rt_int32_t hdr, rt_size_t size)
{
    rt_event_t pevent = (rt_event_t)args;
    rt_event_send(pevent, 1 << (hdr));
    return RT_EOK;
}
//struct rt_can_filter_item
//{
//    rt_uint32_t id  : 29;
//    rt_uint32_t ide : 1;
//    rt_uint32_t rtr : 1;
//    rt_uint32_t mode : 1;
//    rt_uint32_t mask;
//    rt_int32_t hdr;
//#ifdef RT_CAN_USING_HDR
//    rt_err_t (*ind)(rt_device_t dev, void *args , rt_int32_t hdr, rt_size_t size);
//    void *args;
//#endif /*RT_CAN_USING_HDR*/
//};
//#define RT_CAN_FILTER_ITEM_INIT(id,ide,rtr,mode,mask,ind,args) \
//     {(id), (ide), (rtr), (mode), (mask), -1, (ind), (args)}
//#define RT_CAN_FILTER_STD_INIT(id,ind,args) \
//     RT_CAN_FILTER_ITEM_INIT(id,0,0,0,0xFFFFFFFF,ind,args)
//#define RT_CAN_FILTER_EXT_INIT(id,ind,args) \
//     RT_CAN_FILTER_ITEM_INIT(id,1,0,0,0xFFFFFFFF,ind,args)
//#define RT_CAN_STD_RMT_FILTER_INIT(id,ind,args) \
//     RT_CAN_FILTER_ITEM_INIT(id,0,1,0,0xFFFFFFFF,ind,args)
//#define RT_CAN_EXT_RMT_FILTER_INIT(id,ind,args) \
//     RT_CAN_FILTER_ITEM_INIT(id,1,1,0,0xFFFFFFFF,ind,args)
//#define RT_CAN_STD_RMT_DATA_FILTER_INIT(id,ind,args) \
//     RT_CAN_FILTER_ITEM_INIT(id,0,0,1,0xFFFFFFFF,ind,args)
//#define RT_CAN_EXT_RMT_DATA_FILTER_INIT(id,ind,args) \
//     RT_CAN_FILTER_ITEM_INIT(id,1,0,1,0xFFFFFFFF,ind,args)
struct rt_can_filter_item filter1item[4] =
{
    RT_CAN_FILTER_STD_INIT(    1,      can1ind_envent, &can_data[0].eventopt),
    RT_CAN_FILTER_STD_INIT(    2,      can1ind_envent, &can_data[0].eventopt),
    RT_CAN_STD_RMT_FILTER_INIT(3,      can1ind_envent, &can_data[0].eventopt),
    RT_CAN_STD_RMT_DATA_FILTER_INIT(4, can1ind_envent, &can_data[0].eventopt),
};

struct rt_can_filter_config filter1 =
{
    4, /* count */
    1, /* actived */
    filter1item,
};

static struct can_app_struct can_data[1] =
{
    {
        "can1", /* name */
        &filter1, /* filter */
        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, /* eventopt */
    },
};



static uint32_t count_recv;
static uint32_t count_lost;
static void rt_can_recv_thread_entry(void* parameter)
{
    struct rt_can_msg msg;
    struct can_app_struct* canpara = (struct can_app_struct*) parameter;
    rt_device_t candev;
    rt_tick_t tick_start, tick_end;
    static uint32_t count_prev;

    candev = rt_device_find(canpara->name);
    RT_ASSERT(candev);

    rt_sem_init(&canpara->sem, canpara->name, 0, RT_IPC_FLAG_FIFO);
    rt_device_open(candev, (RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX));
    rt_device_control(candev, RT_CAN_CMD_SET_FILTER, canpara->filter);
    rt_device_set_rx_indicate(candev, can1ind);

    count_recv = 0;
    count_lost = 0;
    count_prev = 0xFFFFFFFF;
    while(1)
    {
        rt_sem_take(&canpara->sem, RT_WAITING_FOREVER);
			
		//	rt_device_write(candev, 0, &msg, sizeof(msg));
        while (rt_device_read(candev, 0, &msg, sizeof(msg)) == sizeof(msg))
        {
            if(count_recv == 0)
                tick_start = rt_tick_get();

            count_recv++;

            //check
            {
                uint32_t count;

                count = msg.data[0];
                count |= (uint32_t)msg.data[1] << 8;
                count |= (uint32_t)msg.data[2] << 16;
                count |= (uint32_t)msg.data[3] << 24;

                //rt_kprintf("recv data => count: %d\n", count);
                //rt_kprintf("count_prev        : %d\n", count_prev);

                if( (count_prev != 0xFFFFFFFF) && (count_prev+1 != count) )
                {
                    count_lost ++;
                }
                count_prev = count;
            }

            if(count_recv > 100*10000)
            {
                tick_end = rt_tick_get();

                rt_kprintf("tick_start: %d\n", tick_start);
                rt_kprintf("tick_end: %d\n", tick_end);

                rt_kprintf("count_recv: %d, lost: %d\n", count_recv, count_lost);
                rt_kprintf("%d frame/S\n", count_recv/((tick_end-tick_start)/RT_TICK_PER_SECOND));

                goto _exit;
            }
        }
    }

_exit:
    rt_device_close(candev);

    return;
}

static uint32_t count_send;
static void rt_can_send_thread_entry(void* parameter)
{
    struct rt_can_msg msg;
    struct can_app_struct* canpara = (struct can_app_struct*) parameter;
    rt_device_t candev;
    rt_tick_t tick_start, tick_end;
    uint32_t count_lost, send_pkg_size;

    candev = rt_device_find(canpara->name);
    RT_ASSERT(candev);

    rt_device_open(candev, (RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX));
    rt_device_control(candev, RT_CAN_CMD_SET_FILTER, canpara->filter);

    count_send = 0;
    count_lost = 0;
    tick_start = rt_tick_get();

    msg.id = 2;
    msg.ide = 1;
    msg.rtr = 0;
    msg.len = 8;

    while(1)
    {
        count_send++;

        msg.data[0] = (uint8_t)count_send;
        msg.data[1] = (uint8_t)(count_send>>8);
        msg.data[2] = (uint8_t)(count_send>>16);
        msg.data[3] = (uint8_t)(count_send>>24);

        send_pkg_size = rt_device_write(candev, 0, &msg, sizeof(msg));
        if(send_pkg_size != sizeof(msg))
        {
            count_lost++;
        }

        if(count_send > 100*10000)
        {
            tick_end = rt_tick_get();

            rt_kprintf("tick_start: %d\n", tick_start);
            rt_kprintf("tick_end: %d\n", tick_end);

            rt_kprintf("count_send: %d, lost: %d\n", count_send, count_lost);
            rt_kprintf("%d frame/S\n", count_send/((tick_end-tick_start)/RT_TICK_PER_SECOND));

            goto _exit;
        }
    }

_exit:
    rt_device_close(candev);

    return;
}

int can_recv_test(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("can_recv",
                           rt_can_recv_thread_entry,
                           &can_data[0],
                           1024,
                           RT_THREAD_PRIORITY_MAX /3 - 1,
                           20);
    if (tid != RT_NULL) rt_thread_startup(tid);

    return 0;
}

int can_send_test(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("can_send",
                           rt_can_send_thread_entry,
                           &can_data[0],
                           1024,
                           RT_THREAD_PRIORITY_MAX /3 - 1,
                           20);
    if (tid != RT_NULL) rt_thread_startup(tid);

    return 0;
}
//INIT_APP_EXPORT(can_recv_test);
#ifdef RT_USING_FINSH
#include <finsh.h>

FINSH_FUNCTION_EXPORT(can_recv_test, start CAN recv test);
FINSH_FUNCTION_EXPORT(can_send_test, start CAN send test);

FINSH_VAR_EXPORT(count_lost, finsh_type_int, CAN lost count);
FINSH_VAR_EXPORT(count_recv, finsh_type_int, CAN recv count);

static int can_recv_count(void)
{
    rt_kprintf("count_recv: %d, lost: %d\n", count_recv, count_lost);
    return 0;
}
FINSH_FUNCTION_EXPORT(can_recv_count, dump can recv count);

#endif /*RT_USING_FINSH*/

#endif /*RT_USING_CAN*/
