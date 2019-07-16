#include "app_can.h"

/* can接收事件标志 */
#define CAN_RX_EVENT (1 << 0)
static rt_device_t can_device = RT_NULL;
static struct rt_event can_event;

static rt_err_t can_rx_ind(rt_device_t device, rt_size_t len)
{
    rt_event_send(&can_event, CAN_RX_EVENT);
    return RT_EOK;
}

static rt_err_t can_open()
{
    can_device = rt_device_find("can1");
    if (RT_NULL == can_device)
    {
        rt_kprintf("can1 device not found!\n");
        return -RT_ERROR;
    }

    /* 初始化事件对象 */
    rt_event_init(&can_event, "can_rx_event", RT_IPC_FLAG_FIFO); 
    /* 设置回调函数发送事件 */
    rt_device_set_rx_indicate(can_device, can_rx_ind);
    return rt_device_open(can_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX);
}


void print_can_msg(struct rt_can_msg *data)
{
    rt_kprintf("\n");
    if (RT_CAN_STDID == data->ide)
    {
        rt_kprintf("STDID=%03X ", data->id);
    }
    else if (RT_CAN_EXTID == data->ide)
    {
        rt_kprintf("EXTID=%08X ", data->id);
    }
    rt_kprintf("HDR=%02X ", data->hdr);
    if (RT_CAN_DTR == data->rtr)
    {
        rt_kprintf("TYPE=DATA LEN=%02X ", data->len);
        for(int i = 0; i < data->len; ++i)
        {
            rt_kprintf("%02X ", data->data[i]);
        }
    }
    else if (RT_CAN_RTR == data->rtr)
    {
        rt_kprintf("TYPE=RMOT LEN=%02X ", data->len);
    }
}

rt_err_t can_write(struct rt_can_msg *data, rt_size_t len)
{
    return rt_device_write(can_device, 0, data, len);
}

rt_err_t can_read(struct rt_can_msg *data, rt_size_t len)
{
    rt_err_t ret = 0;
    rt_uint32_t e;
    while (rt_device_read(can_device, 0, data, len) != len)
    {
        ret = rt_event_recv(&can_event, CAN_RX_EVENT, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &e);
        if (RT_EOK != ret) return -RT_ETIMEOUT;
    }
    return RT_EOK;
}
static void can_thread_entry(void *param)
{
    
    struct rt_can_msg msg = {
        .id = 1,
        .ide = RT_CAN_STDID,
        .rtr = RT_CAN_DTR,
        .len = 8,
        .data = {0, 1, 2, 3, 4, 5, 6, 7}
    };
    can_write(&msg, sizeof(struct rt_can_msg));
    
    msg.rtr = RT_CAN_RTR;
    can_write(&msg, sizeof(struct rt_can_msg));
    
    msg.ide = RT_CAN_EXTID;
    msg.rtr = RT_CAN_DTR;
    can_write(&msg, sizeof(struct rt_can_msg));
    
    msg.rtr = RT_CAN_RTR;
    can_write(&msg, sizeof(struct rt_can_msg));
    
    for (;;)    
    {
        rt_pin_write(LED_Pin_Index, !rt_pin_read(LED_Pin_Index));
        can_read(&msg, sizeof(struct rt_can_msg));
        print_can_msg(&msg);
    }
}

#define CAN_RX_THREAD_PRIORITY  15
int can_thread_init(void)
{
    rt_thread_mdelay(1000);
    if (RT_EOK != can_open()) return -RT_ERROR;
    rt_thread_t can_thread = rt_thread_create("can", can_thread_entry, RT_NULL, 1024, CAN_RX_THREAD_PRIORITY , 10);
    if(RT_NULL != can_thread) 
    {
        rt_thread_startup(can_thread);
    }
    return -RT_ERROR;
}
INIT_APP_EXPORT(can_thread_init);