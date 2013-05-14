/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx PWM Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx_pwm.h"
#include "nano1xx_pdma.h"
#include "nano1xx_assert.h"


int32_t	volatile g_bCapInt = 0;
uint8_t volatile g_u8PWMCount = 1;
uint16_t g_u16Frequency;
uint32_t g_u32Pulse = 0;
uint8_t u8CapTimer = 3;


/*--------------------------------------------------------------------------------------*/
/* Capture function to calculate the input waveform information                         */
/*--------------------------------------------------------------------------------------*/
void CalPeriodTime(uint8_t u8Capture)
{
        uint16_t u32Count[2];
        uint32_t u32i;
        uint16_t u16HighPeroid, u16LowPeroid, u16TotalPeroid;


        // expect to put falling capture in u32Count[0], so exit this loop after rising interrupt
        g_bCapInt = 0;
        while(g_bCapInt != 2);

        u32i = 0;

        while(g_bCapInt != 1);
        /* Get the Falling Counter Data */
        u32Count[u32i++] = PWM_GetFallingCounter(u8Capture);

        while(g_bCapInt != 2);

        /* Get the Rising Counter Data */
        u32Count[u32i++] = PWM_GetRisingCounter(u8Capture);


        u16HighPeroid = 0xFFFF - u32Count[0];

        u16LowPeroid = 0xFFFF - u32Count[1];

        u16TotalPeroid = u16HighPeroid + u16LowPeroid;

        printf("Test Result:\nHigh Period = %d, Low  Period = %d, Total Period = %d.\n\n",
               u16HighPeroid, u16LowPeroid, u16TotalPeroid);


}

uint16_t pdma_data[16];
int32_t	volatile pdma_done = 0;

int32_t main (void)
{
        S_DRVPWM_TIME_DATA_T sPt;
        STR_PDMA_T sPDMA;
        uint8_t u8Item, i;
        uint8_t u8Timer;
        int32_t i32Loop = 1;
        int32_t i32TestLoop = 1;

        /* Enable PWM clock */
        PWM_Init(0);
        PWM_Init(1);
        PWM_Init(4);
        PWM_Init(3);	//for capture
        PWM_Init(6);	// for capture

        /* Set PWM pins */
        GCR->PA_H_MFP = (GCR->PA_H_MFP & ~(PA12_MFP_MASK | PA13_MFP_MASK | PA15_MFP_MASK)) |
						PA12_MFP_PWM0_CH0 | PA13_MFP_PWM0_CH1 | PA15_MFP_PWM0_CH3;

		GCR->PB_H_MFP = (GCR->PB_H_MFP & ~PB11_MFP_MASK) | PB11_MFP_PWM1_CH0;
	    GCR->PE_L_MFP = (GCR->PE_L_MFP & ~PE0_MFP_MASK) | PE0_MFP_PWM1_CH2;
		printf("\n\n");
        printf("+------------------------------------------------------------------------+\n");
        printf("|                          PWM Driver Sample Code                        |\n");
        printf("|                                                                        |\n");
        printf("+------------------------------------------------------------------------+\n");
        printf("  This sample code can use PWM0 to drive Buzzer,\n");
        printf("  use PWM3 to capture the signal from PWM1,\n");
        printf("  and use PWM4 to capture the signal from PWM6 with PDMA\n");
        printf("  I/O configuration:\n");
        printf("    GPA12 <--> Buzzer\n");
        printf("    GPA13 <--> GPA15\n");
        printf("    GPB11 <--> GPE0\n");

        while (i32Loop) {
                printf("\n  Please Select Test Item\n");
                printf("    0 : PWM Timer Waveform Test\n");
                printf("    1 : PWM Caputre Test\n");
                printf("    2 : PWM Caputre Test w. PDMA\n");
                printf("    3 : Exit\n\n");

                u8Item = getchar();

                switch (u8Item) {
                case '0': {
                        uint8_t u8ItemOK;

                        printf("\nPWM Waveform Test. PWM0 output to Buzzer\n");

                        i32TestLoop = 1;

                        printf("Select Test Item\n");
                        printf(" 1: Do (261Hz)\n");
                        printf(" 2: Re (293Hz)\n");
                        printf(" 3: Mi (329Hz)\n");
                        printf(" 4: Fa (349Hz)\n");
                        printf(" 5: Sol(392Hz) \n");
                        printf(" 6: La (440Hz)\n");
                        printf(" 7: Si (493Hz)\n");
                        printf(" 0: Exit\n");

                        while (i32TestLoop) {
                                u8ItemOK = 1;
                                u8Item = getchar();

                                switch (u8Item) {
                                case '1':
                                        g_u16Frequency = 261;
                                        break;
                                case '2':
                                        g_u16Frequency = 293;
                                        break;
                                case '3':
                                        g_u16Frequency = 329;
                                        break;
                                case '4':
                                        g_u16Frequency = 349;
                                        break;
                                case '5':
                                        g_u16Frequency = 392;
                                        break;
                                case '6':
                                        g_u16Frequency = 440;
                                        break;
                                case '7':
                                        g_u16Frequency = 493;
                                        break;
                                case '0':
                                        i32TestLoop = 0;
                                        break;
                                default:
                                        u8ItemOK = 0;
                                        break;
                                }

                                if (i32TestLoop && u8ItemOK) {
                                        g_u32Pulse = 0;
                                        g_u8PWMCount = 1;

                                        /* PWM Timer property */
                                        /*
                                         * clock = 12MHz / 2 = 6Mhz.
                                         * CN = clock / freq - 1
                                         */
                                        sPt.u8Mode = PWM_CTL_CHMOD_AUTO_RELOAD;
                                        sPt.u8PreScale = 1;		// actually 1 + 1
                                        sPt.u8Div = PWM_CLKSEL_CLKSEL_DIV1;
                                        sPt.u32Cn = 6000000 / g_u16Frequency - 1;
					sPt.u16Cm = sPt.u32Cn / 2;   	// 50% duty ratio
                                        sPt.u8Inv = 0;			// Disable inverter
                                        u8Timer = 0;


                                        /* Set PWM Timer0 Configuration */
                                        PWM_SetClk(u8Timer, &sPt);

                                        /* Enable Output for PWM Timer0 */
                                        PWM_ConfigOutput(u8Timer, 1);

                                        /* Enable Interrupt Sources of PWM Timer0 and install call back function */
                                        PWM_EnableInt(u8Timer);

                                        /* Enable the PWM Timer 0 */
                                        PWM_Enable(u8Timer);

                                        while (g_u8PWMCount);

                                        /*--------------------------------------------------------------------------------------*/
                                        /* Stop PWM Timer0 (Recommended procedure method 2)		                            	*/
                                        /* Set PWM Timer counter as 0, When interrupt request happen, disable PWM Timer			*/
                                        /* Set PWM Timer counter as 0 in Call back function										*/
                                        /*--------------------------------------------------------------------------------------*/

                                        /* Disable the PWM Timer 0 */
                                        PWM_Disable(u8Timer);
                                }
                        }
                        break;
                }
                case '1': {
                        printf("PWM Capture Test\n");
                        printf("Use PWM3 to capture the PWM1 waveform\n");

                        /*--------------------------------------------------------------------------------------*/
                        /* Set the PWM1 as output function.    				                            */
                        /*--------------------------------------------------------------------------------------*/

                        /* PWM property for output waveform */
                        /*
                         * clock = 12MHz / 2 = 6Mhz.
                         * CN = clock / freq - 1
			 */
			sPt.u8Mode = PWM_CTL_CHMOD_AUTO_RELOAD;
                        sPt.u8PreScale = 1;		// actually 1 + 1
                        sPt.u8Div = PWM_CLKSEL_CLKSEL_DIV1;
                        sPt.u32Cn = 6000000 / 250 - 1;		// 250 Hz
			sPt.u16Cm = (sPt.u32Cn * 3) / 10;   	// 30% duty ratio
                        sPt.u8Inv = 0;			// Disable inverter
                        u8Timer	= 1;

                        /* Set PWM Timer1 Configuration */
                        PWM_SetClk(u8Timer, &sPt);

                        /* Enable Output for PWM Timer1 */
                        PWM_ConfigOutput(u8Timer, 1);

                        /* Enable the PWM Timer1 */
                        PWM_Enable(u8Timer);

                        /*--------------------------------------------------------------------------------------*/
                        /* Set the PWM3 for capture function 				                            */
                        /*--------------------------------------------------------------------------------------*/

                        /* PWM property for Capture */
			sPt.u8Mode = PWM_CTL_CHMOD_AUTO_RELOAD;
                        sPt.u8PreScale = 1;		// actually 1 + 1
                        sPt.u8Div = PWM_CLKSEL_CLKSEL_DIV1;
                        sPt.u32Cn = 0xFFFF;		// Max CN in non-cascade mode
			sPt.u16Cm = 0;   	// don't care in capture function

                        /* Set PWM Timer 3 for Capture */
                        PWM_SetClk(u8CapTimer, &sPt);

                        /* Enable Interrupt Sources of PWM Capture3 */
                        PWM_EnableCaptureInt(u8CapTimer, PWM_CAPINTEN_CRLIE | PWM_CAPINTEN_CFLIE);
                        PWM_SetCaptureAttribute(u8CapTimer, 0, 0, PWM_CAPCTL_CAPRELOAD_MASK);

                        /* Enable Input function for PWM 3 */
                        PWM_ConfigCaptureIntput(u8CapTimer, 1);

                        /* Enable the PWM Capture3 */
                        PWM_CaptureEnable(u8CapTimer);

                        /* Capture the Input Waveform Data */
                        CalPeriodTime(u8CapTimer);

                        /* Set PWM Timer 1 counter as 0 */
                        PWM_SetCounter(u8Timer, 0, 0);
                        /* Disable Output function for PWM Timer 1 */
                        PWM_ConfigOutput(u8Timer, 0);
                        /* Disable PWM Timer 1 */
                        PWM_Disable(u8Timer);

                        /* Set PWM Capture 3 counter as 0 */
                        PWM_SetCounter(u8CapTimer, 0, 0);
                        /* Clear the PWM Capture 3 Interrupt */
                        PWM_ClearCaptureInt(u8CapTimer);
                        /* Disable PWM Capture 3 */
                        PWM_CaptureDisable(u8CapTimer);
                        /* Disable Input function for PWM Capture 3 */
                        PWM_ConfigCaptureIntput(u8CapTimer, 0);

                        break;
                }
                case '2': {
                        printf("PWM Capture Test w. PDMA\n");
                        printf("Use PWM4 to capture the PWM6 waveform\n");

                        /* Configure output channel */
                        /*
                         * clock = 12MHz / 2 = 6Mhz.
                         * CN = clock / freq - 1
			 */
			sPt.u8Mode = PWM_CTL_CHMOD_AUTO_RELOAD;
                        sPt.u8PreScale = 1;		// actually 1 + 1
                        sPt.u8Div = PWM_CLKSEL_CLKSEL_DIV1;
                        sPt.u32Cn = 6000000 / 1000 - 1;		// 1000 Hz
			sPt.u16Cm = (sPt.u32Cn * 3) / 10;   	// 30% duty ratio
                        sPt.u8Inv = 0;			// Disable inverter

                        /* todo: Select PWM67 engine clock */

                        /* Set PWM ch 6 timer configuration */
                        PWM_SetClk(6, &sPt);

                        /* Enable ch 6 output */
                        PWM_ConfigOutput(6, 1);

                        /* Enable PWM ch 6 */
                        PWM_Enable(6);

                        /* Configure capture channel */
                        /* PDMA Init */
                        PDMA_Init();
                        /* Use PDMA ch1 here. */
			PDMA_DISABLE_CH1();
			PDMA_EnableChannelClock(PDMAGCR_CSR_CLK1);
			PDMA_PWM1_CH0_TO_CH1();
                        sPDMA.SrcAddr 		= (uint32_t)&(PWM1->CH0PDMA);
                        sPDMA.DestAddr	 	= (uint32_t)&pdma_data[0];
                        sPDMA.TransWidth 	= PDMA_CSR_APB_TWS_16BIT;		   // Change to 32 bit if cascade mode is enabled
                        sPDMA.Mode 		= PDMA_CSR_IP2MEM;
                        sPDMA.SrcCtrl	 	= PDMA_CSR_SAD_FIX;
                        sPDMA.DestCtrl	 	= PDMA_CSR_DAD_INC;
                        sPDMA.ByteCnt        = sizeof(pdma_data);
                        PDMA_Open(PDMA1, &sPDMA);
                        PDMA_EnableInt(PDMA1, PDMA_IER_TD);
			PDMA_CHEnableTransfer(PDMA1);

                        /* PWM timer property for capture channel */
			sPt.u8Mode = PWM_CTL_CHMOD_AUTO_RELOAD;
                        sPt.u8PreScale = 1;		// actually 1 + 1
                        sPt.u8Div = PWM_CLKSEL_CLKSEL_DIV1;
                        sPt.u32Cn = 0xFFFF;		// Max CN in non-cascade mode
			sPt.u16Cm = 0;   	// don't care in capture function
                        sPt.u8Inv = 0;		// don't care in capture function


                        /*todo:  Select PWM engine clock */

                        /* Set PWM ch 4 timer configuration */
                        PWM_SetClk(4, &sPt);

                        /* Reload on both edge */
                        PWM_SetCaptureAttribute(4, 0, 0, PWM_CAPCTL_CAPRELOADREN | PWM_CAPCTL_CAPRELOADFEN);

                        /* Enable PWM ch 4 input function */
                        PWM_ConfigCaptureIntput(4, 1);

                        /* Configure PDMA function*/
                        PWM_EnablePdma(4, PWM_CAPCTL_CHRFORDER_R, PWM_CAPCTL_PDMACAPMOD_BOTH);

                        /* Enable PWM ch 4 */
                        PWM_CaptureEnable(4);

                        /* Wait for PDMA capture complete */
                        while(pdma_done == 0);

                        printf("Done\n");

                        for(i = 0; i < 16; i++)
                                printf("%d\n", 0xFFFF - pdma_data[i]);

                        /* STOP */
                        PWM_SetCounter(6, 0, 0);
                        PWM_Disable(6);
                        PWM_SetCounter(4, 0, 0);
                        PWM_CaptureDisable(4);
			PDMA_DeInit();
                        break;

                }
                case '3': {
                        i32Loop = 0;
                        break;
                }
                }
        }

        printf("PWM sample is complete.\n");

        PWM_DeInit(0);
        PWM_DeInit(1);
        PWM_DeInit(3);
        PWM_DeInit(4);
        PWM_DeInit(6);

        return 0;
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

