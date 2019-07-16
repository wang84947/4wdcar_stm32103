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

#include <string.h>

#include <board.h>
#include <rtthread.h>
#include <rtdevice.h>

#ifdef RT_USING_CAN
#include "drv_lpccan.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif /*ARRAY_SIZE*/

struct can_app_struct
{
    const char* name;
    struct rt_can_filter_config * filter;
    rt_uint8_t eventopt;
    struct rt_semaphore sem;
    rt_err_t (*rx_ind)(rt_device_t dev, rt_size_t size);
};

static struct can_app_struct can1_data[1];

struct rt_can_filter_item filter1item[] =
{
    LPC_CAN_AF_STD_INIT(1),
    LPC_CAN_AF_STD_GRP_INIT(3, 5),
    LPC_CAN_AF_EXT_INIT(2),
    LPC_CAN_AF_EXT_GRP_INIT(4,6),
};

struct rt_can_filter_config filter1 =
{
    ARRAY_SIZE(filter1item), /* count */
    1, /* actived */
    filter1item,
};

static struct can_app_struct can1_data[1] =
{
    {
        "can1", /* name */
        &filter1, /* filter */
        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, /* eventopt */
    },
};

static rt_err_t lpccanind1(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&can1_data[0].sem);
    return RT_EOK;
}

static struct rt_can_filter_item can2_filter_item[] =
{
    LPC_CAN_AF_EXT_INIT(2),
};

static struct rt_can_filter_config can2_filter =
{
    ARRAY_SIZE(can2_filter_item), /* count */
    1, /* actived */
    can2_filter_item,
};

static struct can_app_struct can2_data[1] =
{
    {
        "can2", /* name */
        &can2_filter, /* filter */
        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, /* eventopt */
    },
};

static rt_err_t lpccanind2(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&can2_data[0].sem);
    return RT_EOK;
}

static void rt_can_recv_thread_entry(void* parameter)
{
    struct rt_can_msg msg;
    struct can_app_struct* canpara = (struct can_app_struct*) parameter;
    rt_device_t candev;
    rt_size_t send_pkg_size;

    candev = rt_device_find(canpara->name);
    if(candev == RT_NULL)
    {
        rt_kprintf("%s not found!\n", canpara->name);
    }

    rt_sem_init(&canpara->sem, canpara->name, 0, RT_IPC_FLAG_FIFO);

    // RT_CAN_CMD_SET_BAUD must call before open
    rt_device_control(candev, RT_CAN_CMD_SET_BAUD, (void *)(500*1000));

    rt_device_open(candev, (RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX));

    // RT_CAN_CMD_SET_FILTER must call after open
    rt_device_control(candev, RT_CAN_CMD_SET_FILTER, canpara->filter);

    rt_device_set_rx_indicate(candev, canpara->rx_ind);

    // send first
    //if(strcmp(canpara->name, "lpccan2") == 0)
    {
        msg.id = 2;
        msg.ide = 1;
        msg.rtr = 0;
        msg.len = 8;

        memcpy(&msg.data[0], canpara->name, msg.len);

        rt_kprintf("%s send first!\n", canpara->name);
        send_pkg_size = rt_device_write(candev, 0, &msg, sizeof(msg));
        if(send_pkg_size != sizeof(msg))
        {
            rt_kprintf("%s send first failed!\n", canpara->name);
        }
    } // send first

    while(1)
    {
        rt_sem_take(&canpara->sem, RT_WAITING_FOREVER);
        while (rt_device_read(candev, 0, &msg, sizeof(msg)) == sizeof(msg))
        {
            //dump
            rt_kprintf("can %s, recv msg:\n", canpara->name);
            rt_kprintf("id: 0x%08X ide:%d rtr:%d\n", msg.id, msg.ide, msg.rtr);
            if(msg.len)
            {
                int i;
                rt_kprintf("DLC %d: ", msg.len);
                for(i=0; i<msg.len; i++)
                {
                    rt_kprintf("%02X ", msg.data[i]);
                }
                rt_kprintf("\n");
            }

            rt_thread_delay(rt_tick_get()&0xFF); // random delay
            //rt_thread_delay(rt_tick_from_millisecond(500)); // random delay

            rt_kprintf("\n%s echo!\n", canpara->name);
            send_pkg_size = rt_device_write(candev, 0, &msg, sizeof(msg));
            if(send_pkg_size != sizeof(msg))
            {
                rt_kprintf("%s send failed!\n", canpara->name);
            }

        }
    }

_exit:
    rt_device_close(candev);

    return;
}

int can_recv_test(void)
{
    rt_thread_t tid;

    can1_data[0].rx_ind = lpccanind1;
    tid = rt_thread_create("can1_recv",
                           rt_can_recv_thread_entry,
                           &can1_data[0],
                           1024,
                           RT_THREAD_PRIORITY_MAX /3 - 1,
                           3);
    if (tid != RT_NULL) rt_thread_startup(tid);

    can2_data[0].rx_ind = lpccanind2;
    tid = rt_thread_create("can2_recv",
                           rt_can_recv_thread_entry,
                           &can2_data[0],
                           1024,
                           RT_THREAD_PRIORITY_MAX /3 - 1,
                           3);
    if (tid != RT_NULL) rt_thread_startup(tid);

    return 0;
}


#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(can_recv_test, start CAN recv test);
#endif /*RT_USING_FINSH*/

#endif /*RT_USING_CAN*/
