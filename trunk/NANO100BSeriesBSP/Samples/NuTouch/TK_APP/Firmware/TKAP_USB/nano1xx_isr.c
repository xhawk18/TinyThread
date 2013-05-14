/******************************************************************************
 * @file     nano1xx_isr.c
 * @brief    This file contains interrupt service routines
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


#include <stdint.h>
#include "nano1xx.h"
#include "nano1xx_usb.h"
#include "nano1xx_tk.h"
#include "nano1xx_sys.h"

/**
  * @brief  HIRC_IRQHandler, HIRC trim interrupt handler.
  * @param  None.
  * @retval None.
  */
void HIRC_IRQHandler(void)
{
	__IO uint32_t reg = GCR->IRCTRIMISR;

	if ((reg & GCR_IRCTRIMINT_FAIL_INT) || (reg & GCR_IRCTRIMINT_32KERR_ENNT))
	{
		GCR->IRCTRIMISR = reg;
		GCR->IRCTRIMCTL = (GCR_IRCTRIMCTL_TRIM_12M | GCR_IRCTRIMCTL_LOOP_32CLK | GCR_IRCTRIMCTL_RETRY_512);
	}
}


/**
  * @brief  USBD_IRQHandler, USB interrupt handler.
  * @param  None.
  * @retval None.
  */
int z=0;
void USBD_IRQHandler(void)
{

	uint32_t u32INTSTS = USBD->INTSTS;

	if (u32INTSTS & USB_INTSTS_FLD_STS)
	{
	    /* Handle the USB attached/detached event */
		USB_FloatDetect();
	}
	else if(u32INTSTS & USB_INTSTS_BUS_STS)
	{
	    /* Handle the USB bus event: Reset, Suspend, and Resume */
		USB_BusEvent();
	}
	else if(u32INTSTS & USB_INTSTS_USB_STS)
	{
	    /* Handle the USB Protocol/Clase event */
		//if(z>=34)
		//	printf("USB_UsbEvent(u32INTSTS);\n");	 //shanchun
		//z++;
		USB_UsbEvent(u32INTSTS);
    }
	else if (u32INTSTS & USB_INTSTS_WKEUP_STS)
	{
		/* Clear wakeup event. write one clear */
		USBD->INTSTS |= USB_INTSTS_WKEUP_STS;
	}
}


extern uint16_t data[16];
extern uint16_t f, t;
extern uint8_t volatile complete;
extern uint8_t set;


void TK_IRQHandler(void)
{

	uint8_t fail, th;


	TK_ReadStatus(NULL, &fail, &th);
	if(fail)
		f |= fail << (8 * set);
	if(th)
		t |= th << (8 * set);

	// read result from register directly
	if(set == 0) {
		memcpy(&data[0], (void *)&TK->DATA1, 16);
	} else {
		memcpy(&data[8], (void *)&TK->DATA1, 16);
	}

	complete = 1;

	// Clear interrupt status.
	TK_CLEAR_INTERRUPT;

	return;
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
