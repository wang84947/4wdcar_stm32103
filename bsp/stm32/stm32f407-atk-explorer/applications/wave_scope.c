#include "wave_scope.h"
void wave_uart_send(void *buf,unsigned int size)
{
	rt_device_t console=rt_console_get_device();
	//rt_device_write(console,-1,buf,size);
}

void wust_sendware(void *buf, unsigned int size)
{
    #define CMD_WARE   3
    char  cmdf[2] = {CMD_WARE, ~CMD_WARE};
    char  cmdr[2] = {~CMD_WARE, CMD_WARE};
		
		
		wave_uart_send(cmdf,2);
		wave_uart_send(buf, size);
		wave_uart_send(cmdr, 2);
	
}	


