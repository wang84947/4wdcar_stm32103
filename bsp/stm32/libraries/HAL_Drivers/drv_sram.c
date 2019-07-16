/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-5-11     wh         first version
 */
#include <board.h>
#define LOG_TAG             "drv.sram"
#include <drv_log.h>
#include "stm32f4xx_hal.h"
#ifdef RT_USING_EXTSRAM
//使用NOR/SRAM的 Bank1.sector3,地址位HADDR[27,26]=10 
//对IS61LV25616/IS62WV25616,地址线范围为A0~A17 
//对IS61LV51216/IS62WV51216,地址线范围为A0~A18
#define Bank1_SRAM3_ADDR    ((unsigned int)(0x68000000))

#define EXT_SRAM_SIZE			(960 *1024) 


static SRAM_HandleTypeDef hsram3;
static struct rt_memheap system_heap;

static void MX_FSMC_Sram_Init(void)
{
 FSMC_NORSRAM_TimingTypeDef Timing;
 FSMC_NORSRAM_TimingTypeDef ExtTiming;

  /** Perform the SRAM3 memory initialization sequence
  */
  hsram3.Instance = FSMC_NORSRAM_DEVICE;
  hsram3.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram3.Init */
  hsram3.Init.NSBank = FSMC_NORSRAM_BANK3;
  hsram3.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram3.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram3.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram3.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram3.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram3.Init.WrapMode = FSMC_WRAP_MODE_DISABLE; //是否使能环路突发模式,此处未用到
  hsram3.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;//存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
  hsram3.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;//存储器写使能
  hsram3.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;//等待使能位,此处未用到
  hsram3.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;//读写使用相同的时序
  hsram3.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;//是否使能同步传输模式下的等待信号,此处未用到
  hsram3.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;//异步传输期间的等待信号
  hsram3.Init.PageSize = FSMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 0;
  Timing.AddressHoldTime = 0;
  Timing.DataSetupTime = 0x9;
  Timing.BusTurnAroundDuration = 0;
  Timing.CLKDivision = 0;
  Timing.DataLatency = 0;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */
  ExtTiming.AddressSetupTime = 0;
  ExtTiming.AddressHoldTime = 0;
  ExtTiming.DataSetupTime = 0x9;
  ExtTiming.BusTurnAroundDuration = 0;
  ExtTiming.CLKDivision = 0;
  ExtTiming.DataLatency = 0;
  ExtTiming.AccessMode = FSMC_ACCESS_MODE_A;

  if (HAL_SRAM_Init(&hsram3, &Timing, &ExtTiming) != HAL_OK)
  {
    Error_Handler( );
		LOG_E("SDRAM init failed!");

  }
  else
  {
	LOG_D("sdram init success, mapped at 0x%X, size is %d bytes, data width is %d", SDRAM_BANK_ADDR, SDRAM_SIZE, SDRAM_DATA_WIDTH);
#ifdef RT_USING_MEMHEAP_AS_HEAP
    /* If RT_USING_MEMHEAP_AS_HEAP is enabled, SDRAM is initialized to the heap */
     rt_memheap_init(&system_heap, "sram", (void *)Bank1_SRAM3_ADDR, EXT_SRAM_SIZE);
#endif
  }
	
}	
 

INIT_BOARD_EXPORT(MX_FSMC_Sram_Init);

#endif






