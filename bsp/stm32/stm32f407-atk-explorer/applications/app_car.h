
#ifndef __APP_CAR_H__
#define __APP_CAR_H__


#include <rtthread.h>
 rt_err_t car_stop(rt_int8_t cmd, void *param);
 rt_err_t car_forward(rt_int8_t cmd, void *param);
 rt_err_t car_backward(rt_int8_t cmd, void *param);
 rt_err_t car_turnleft(rt_int8_t cmd, void *param);
 rt_err_t car_turnright(rt_int8_t cmd, void *param);
 rt_err_t car_stop(rt_int8_t cmd, void *param);

#endif

