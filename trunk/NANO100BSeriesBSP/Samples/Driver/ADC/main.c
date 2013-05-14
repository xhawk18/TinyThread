/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx ADC Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_adc.h"
#include "nano1xx_uart.h"
#include "nano1xx_gpio.h"
#include "nano1xx_sys.h"
#include "nano1xx_assert.h"


void AdcSingleModeTest(void);
void AdcSingleCycleModeTest(void);
void AdcContScanModeTest(void);
void AdcResultMonitorTest(void);
uint8_t SingleEndInput_ScanOpModeChannelSelect(uint32_t * u32ChannelSelBitwise, uint8_t * pu8ActiveChannel);


volatile uint8_t gu8AdcIntFlag;
volatile uint8_t gu8AdcCmp0IntFlag;
volatile uint8_t gu8AdcCmp1IntFlag;


/* Main function */
int main(void)
{
        uint8_t u8Option;

        /* Unlock the locked registers before we can access them */
        UNLOCKREG();

        printf("\n\n");


		GCR->PA_L_MFP = PA0_MFP_ADC_CH0 | PA1_MFP_ADC_CH1 | PA2_MFP_ADC_CH2 | PA3_MFP_ADC_CH3 |
		                     PA4_MFP_ADC_CH4 | PA5_MFP_ADC_CH5 | PA6_MFP_ADC_CH6 | PA7_MFP_ADC_CH7;
		GCR->PD_L_MFP = (GCR->PD_L_MFP & ~(PD0_MFP_MASK | PD1_MFP_MASK | PD2_MFP_MASK | PD3_MFP_MASK)) |
		                     PD0_MFP_ADC_CH8 | PD1_MFP_ADC_CH9 | PD2_MFP_ADC_CH10 | PD3_MFP_ADC_CH11;

        while(1) {



                printf("\n\n");
                printf("+----------------------------------------------------------------------+\n");
                printf("|                            ADC sample code                           |\n");
                printf("+----------------------------------------------------------------------+\n");
                printf("  [1] ADC single mode test\n");
                printf("  [2] ADC single cycle mode test\n");
                printf("  [3] ADC continuous scan mode test\n");
                printf("  [4] ADC result monitor test\n");
                printf("  [q] Quit\n");
                printf("Please choose a test item.\n");

                u8Option = getchar();

                if(u8Option == '1') {
                        AdcSingleModeTest();
                } else if(u8Option == '2') {
                        AdcSingleCycleModeTest();
                } else if(u8Option == '3') {
                        AdcContScanModeTest();
                } else if(u8Option == '4') {
                        AdcResultMonitorTest();
                } else if( (u8Option == 'q') || (u8Option == 'Q') ) {
                        break;
                }
                printf("\n\n");
        }

        return 0;
}

/**
  * @brief  ADC sigle mode demo code
  * @param  None
  * @return None
  */
void AdcSingleModeTest(void)
{
        uint8_t u8ChannelNum, u8ActiveChannelNum;
        uint32_t u32ChannelSelBitwise;
        int32_t i32ConversionData;
        uint8_t au8ActiveChannel[12];

        printf("\n== Single mode test ===\n");
        /* Set the ADC operation mode as single mode */
		ADC_Init(ADC_CR_SINGLE_END, ADC_CR_ADMD_SINGEL, 0, ADC_CR_REFSEL_AVDD);

        while(1) {
                /* Select the active channel */
                u8ActiveChannelNum = SingleEndInput_ScanOpModeChannelSelect(&u32ChannelSelBitwise, au8ActiveChannel);

                if(u8ActiveChannelNum==0xFF)
                        break ;

                /* Set the ADC channel */
                ADC_SetChannelMask(u32ChannelSelBitwise);

                gu8AdcIntFlag = 0;

                ADC_EnableInt();
                ADC_POWER_ON;
                ADC_START_CONV;

                /* Wait ADC interrupt */
                while(gu8AdcIntFlag==0);

                ADC_DisableInt();
                u8ChannelNum = au8ActiveChannel[0];
                i32ConversionData = ADC_GetConversionData(u8ChannelNum);
                printf("Conversion result: 0x%X (%d)\n\n", i32ConversionData, i32ConversionData);
        }
        ADC_DeInit();
}

/**
  * @brief  ADC sigle cycle mode demo code
  * @param  None
  * @return None
  */
void AdcSingleCycleModeTest(void)
{
        uint8_t u8ActiveChannelNum, u8ChannelNum, u8ChannelCount;
        uint32_t u32ChannelSelBitwise;
        int32_t i32ConversionData;
        uint8_t au8ActiveChannel[12];

        printf("\n== Single cycle scan mode test ===\n");
        /* Set the ADC operation mode as single mode */
	    ADC_Init(ADC_CR_SINGLE_END, ADC_CR_ADMD_S_CYCLE, 0, ADC_CR_REFSEL_AVDD);

        while(1) {

                /* Select the active channel */
                u8ActiveChannelNum = SingleEndInput_ScanOpModeChannelSelect(&u32ChannelSelBitwise, au8ActiveChannel);

                if(u8ActiveChannelNum==0xFF)
                        break ;

                ADC_SetChannelMask(u32ChannelSelBitwise);

                gu8AdcIntFlag = 0;
                ADC_EnableInt();
                ADC_POWER_ON;
                ADC_START_CONV;
                /* Wait conversion done */
                while(ADC_IsConversionDone()==FALSE);

                for(u8ChannelCount=0; u8ChannelCount<u8ActiveChannelNum; u8ChannelCount++) {
                        u8ChannelNum = au8ActiveChannel[u8ChannelCount];
                        i32ConversionData = ADC_GetConversionData(u8ChannelNum);
                        printf("Conversion result of channel %d: 0x%X (%d)\n\n", u8ChannelNum, i32ConversionData, i32ConversionData);
                }

        }
        ADC_DeInit();
}

/**
  * @brief  ADC continuous mode demo code
  * @param  None
  * @return None
  */
void AdcContScanModeTest()
{
        uint8_t u8ActiveChannelNum, u8ChannelNum, u8ChannelCount;
        uint32_t u32ChannelSelBitwise;
        int32_t i32ConversionData;
        uint8_t au8ActiveChannel[12];

        printf("\n== Continuous scan mode test ===\n");
        /* Set the ADC operation mode as single mode */
	    ADC_Init(ADC_CR_SINGLE_END, ADC_CR_ADMD_CONTINUOUS, 0, ADC_CR_REFSEL_AVDD);

        while(1) {

                /* Select the active channel */
                u8ActiveChannelNum = SingleEndInput_ScanOpModeChannelSelect(&u32ChannelSelBitwise, au8ActiveChannel);

                if(u8ActiveChannelNum==0xFF)
                        break ;

                ADC_SetChannelMask(u32ChannelSelBitwise);

                gu8AdcIntFlag = 0;
                ADC_EnableInt();
                ADC_POWER_ON;
                ADC_START_CONV;

                while(ADC_IsConversionDone()==FALSE);

                for(u8ChannelCount=0; u8ChannelCount<u8ActiveChannelNum; u8ChannelCount++) {
                        u8ChannelNum = au8ActiveChannel[u8ChannelCount];
                        i32ConversionData = ADC_GetConversionData(u8ChannelNum);
                        printf("Conversion result of channel %d: 0x%X (%d)\n\n", u8ChannelNum, i32ConversionData, i32ConversionData);
                }

                /* Wait conversion done */
                while(ADC_IsConversionDone()==FALSE);

                for(u8ChannelCount=0; u8ChannelCount<u8ActiveChannelNum; u8ChannelCount++) {
                        u8ChannelNum = au8ActiveChannel[u8ChannelCount];
                        i32ConversionData = ADC_GetConversionData(u8ChannelNum);
                        printf("Conversion result of channel %d: 0x%X (%d)\n\n", u8ChannelNum, i32ConversionData, i32ConversionData);
                }


                ADC_STOP_CONV;

                ADC_CLEAR_ALL_INTERRUPT;

        }
        ADC_DeInit();
}

/**
  * @brief  ADC comparator demo code
  * @param  None
  * @return None
  */
void AdcResultMonitorTest(void)
{
        uint8_t u8CmpChannelNum, u8CmpMatchCount;


        printf("\n== ADC result monitor test ===\n");

 	    ADC_Init(ADC_CR_SINGLE_END, ADC_CR_ADMD_CONTINUOUS, 0, ADC_CR_REFSEL_AVDD);

        /* Enable ADC channel 0 */

        ADC_SetChannelMask(0x1);

        u8CmpChannelNum = 0;
        gu8AdcCmp0IntFlag = 0;
        u8CmpMatchCount = 5;
        /* Enable ADC comparator 0. Compare condition: conversion result < 0x800. */
        ADC_EnableComp0(u8CmpChannelNum, ADC_CMPR_CMPCOND_LT, 0x800, u8CmpMatchCount);
        /* enable ADC comparator 0 interrupt and set the callback function */
        ADC_EnableComp0Int();

        gu8AdcCmp1IntFlag = 0;
        u8CmpMatchCount = 5;
        /* Enable ADC comparator 1. Compare condition: conversion result >= 0x800. */
        ADC_EnableComp1(u8CmpChannelNum, ADC_CMPR_CMPCOND_GE, 0x800,  u8CmpMatchCount);
        /* enable ADC comparator 1 interrupt and set the callback function */
        ADC_EnableComp1Int();

	    ADC_POWER_ON;
        ADC_START_CONV;

        /* Wait ADC compare interrupt */
        while( (gu8AdcCmp0IntFlag==0)&&(gu8AdcCmp1IntFlag==0) );

        ADC_STOP_CONV;
        ADC_EnableComp0Int();
        ADC_EnableComp1Int();
        ADC_DISABLE_COMP0;
        ADC_DISABLE_COMP1;

        if(gu8AdcCmp0IntFlag==1) {
                printf("The conversion result of channel %d is less than 0x800\n", u8CmpChannelNum);
        } else {
                printf("The conversion result of channel %d is greater or equal to 0x800\n", u8CmpChannelNum);
        }

        ADC_DeInit();
}


/**
  * @brief  Select ADC active channel
  * @param  u32ChannelSelBitwise Indicate the selected channel mask
  * @param  pu8ActiveChannel Array pointer save active channel numbers
  * @return Number of active channels or abort
  * @retval 0xFF Abort
  * @retval Others Number of avtive channels
  */
uint8_t SingleEndInput_ScanOpModeChannelSelect(uint32_t * u32ChannelSelBitwise, uint8_t * pu8ActiveChannel)
{
        uint8_t u8ChannelCount, u8ActiveChannelCount, u8Option;

        printf("Select ADC channel:\n");
        u8ActiveChannelCount = 0;
        *u32ChannelSelBitwise = 0;
        for(u8ChannelCount=0; u8ChannelCount<12; u8ChannelCount++) {
                printf("  Enable channel %d ? (y/n/q)", u8ChannelCount);
                while(1) {
                        u8Option = getchar();
                        if( (u8Option=='y') || (u8Option=='Y') ) {
                                printf("\n");
                                *u32ChannelSelBitwise = *u32ChannelSelBitwise | (1<<u8ChannelCount);
                                *(pu8ActiveChannel + u8ActiveChannelCount) = u8ChannelCount;
                                u8ActiveChannelCount++;
                                break;
                        } else if( (u8Option=='n') || (u8Option=='N') ) {
                                printf("\n");
                                break;
                        } else if( (u8Option=='q') || (u8Option=='Q') ) {
                                printf("\n");
                                return 0xFF;
                        }
                }
        }
        return u8ActiveChannelCount;
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

