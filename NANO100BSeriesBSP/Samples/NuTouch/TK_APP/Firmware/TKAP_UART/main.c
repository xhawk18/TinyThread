/******************************************************************************
 * @file     HIDTransfer_Main.c
 * @brief    Nano1xx Touch Key configuration sample code (UART interface)
 * @version  1.0.1
 * @date     04, December, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nano1xx.h"
#include "nano1xx_tk.h"
#include "nano1xx_assert.h"
#include "nano1xx_tk2pc.h"

#define CLS				  do {printf("%c[2J", 0x1B); printf("%c[%d;1H", 0x1B, 0);}while(0)
#define JMP(x)			  do {printf("%c[0J", 0x1B); printf("%c[%d;1H", 0x1B, x);}while(0)

extern void UART_Setting(void);


uint16_t th[16][2] = {{0xFFFF, 0},
        {0xFFFF, 0},
        {0xFFFF, 0},
        {0xFFFF, 0},
        {0xFFFF, 0},
        {0xFFFF, 0},
        {0xFFFF, 0},
        {0xFFFF, 0},
        {0xFFFF, 0},
        {0xFFFF, 0},
        {0xFFFF, 0},
        {0xFFFF, 0},
        {0xFFFF, 0},
        {0xFFFF, 0},
        {0xFFFF, 0},
        {0xFFFF, 0}
};
uint16_t data[16];
uint16_t f, t;
uint8_t volatile complete = 0;
uint8_t set = 0;  // set 0 = ch0~7, set 1 = ch8~15

void Delay(uint8_t time) 
{
	while(time--);
}

extern libtk_channel_config_s cfg[16];

void Get_TK_Data(uint16_t ch)
{
	uint8_t i;
	S_TK_CH_CFG ch_cfg = {8, 2, 0xFFFF, 0x0000};
	if(ch & 0xFF)
	{
		complete=0;
		set = 0;
	    for(i = 0; i < 8; i++) {    
			ch_cfg.u8Level=cfg[i].current;
			ch_cfg.u8Div=cfg[i].div;
			TK_ConfigChannel(i, &ch_cfg);
	    }
		TK_Start(ch & 0xFF);					
		while(complete == 0);
	}

	if(ch & 0xFF00)
	{
		complete = 0;
		set = 1;
		for(i = 8; i < 16; i++) {
			ch_cfg.u8Level=cfg[i].current;
			ch_cfg.u8Div=cfg[i].div;
			TK_ConfigChannel(i, &ch_cfg);
		}
		TK_Start(ch & 0xFF00);					
		while(complete == 0);
	}	
}

extern  void TK_Device2PC(void);

void env_test(void)
{        

		TK_DeInit();
        TK_Init(TK_CTL1_SEN_SEL_16BIT);

        TK_EnableInterrupt(TK_INT_ALL); 		
		CLS;
        while(1) {										
				TK_Device2PC();
		}

}



int32_t main(void)
{
			
//	printf("TK demo code begins\n");	
	GCR->PA_L_MFP = (GCR->PA_L_MFP & ~(PA0_MFP_MASK | PA1_MFP_MASK)) | PA0_MFP_TK8 | PA1_MFP_TK9;		// TK8, 9
	GCR->PA_H_MFP = (GCR->PA_H_MFP & ~(PA12_MFP_MASK | PA13_MFP_MASK)) | PA12_MFP_TK10 | PA13_MFP_TK11;		// TK10, 11
	GCR->PC_H_MFP = (GCR->PC_H_MFP & ~(PC8_MFP_MASK | PC9_MFP_MASK | PC10_MFP_MASK | PC11_MFP_MASK)) |
					PC8_MFP_TK12 | PC9_MFP_TK13 | PC10_MFP_TK14 | PC11_MFP_TK15;		// 12~15
	GCR->PD_L_MFP = (GCR->PD_L_MFP & ~(PD0_MFP_MASK | PD1_MFP_MASK | PD2_MFP_MASK | PD3_MFP_MASK | PD4_MFP_MASK | PD5_MFP_MASK)) |  
					PD0_MFP_TK0 | PD1_MFP_TK1 | PD2_MFP_TK2 | PD3_MFP_TK3 | PD4_MFP_TK4 | PD5_MFP_TK5;    // 0~5
	GCR->PF_L_MFP = (GCR->PF_L_MFP & ~(PF4_MFP_MASK | PF5_MFP_MASK)) | PF4_MFP_TK6 | PF5_MFP_TK7;    // 6, 7
		
        env_test();

    //    printf("Adios!!\n");
        return(0);
}

#ifdef USE_ASSERT
/**
  * @brief  The function prints the source file name and line number where the assert_param() error
  *         occurs, and then stops in an infinite loop. User can add his own codes here if necessary.
  * @param[in] file Source file name
  * @param[in] line Line number
  * @return None
  */
void assert_error(uint8_t * file, uint32_t line)
{
 	GCR->PB_L_MFP = (GCR->PB_L_MFP & ~0x77) | (PB1_MFP_UART0_TX | PB0_MFP_UART0_RX);  /* Select multi-function pin for UART0 */
 	CLK->APBCLK |= CLK_APBCLK_UART0_EN;    /* Enable UART0 clock */
	CLK->CLKDIV0 &= ~CLK_CLKDIV0_UART_MASK;
	CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UART_MASK) | CLK_CLKSEL1_UART_HXT;  /* Select 12 Mhz XTAL */

 
    /* Set UART to 115200,n,8,1,none */
    UART0->BAUD = 0x67;             /* Baud Rate:115200 for 12MHz */
    UART0->TLCTL = 0x03;            /* Word len is 8 bits */          
	
	printf("[%s] line %d : wrong parameters.\r\n", file, line);

	/* Infinite loop */
	while(1) ;

}
#endif

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
