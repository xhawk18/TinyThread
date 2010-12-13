/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------*/
/* Include related headers                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#include "M051Series.h"
#include "core_cm0.h"
#include "DrvADC.h"
#include "DrvSYS.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static DRVADC_ADC_CALLBACK *g_ptADCCallBack;
static DRVADC_ADCMP0_CALLBACK *g_ptADCMP0CallBack;
static DRVADC_ADCMP1_CALLBACK *g_ptADCMP1CallBack;
static uint32_t g_pu32UserData[3];

/*--------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_Open                                                                                              */
/*                                                                                                                    */
/* Parameters:        	                                                                                              */
/*   InputMode           [in]:  Specify the type of the analog input signal.                                          */
/*                              It might be single-end or differential input.                                         */
/*   OpMode              [in]:  Specify the operation mode.                                                           */
/*                              It might be single, single cycle scan or continuous scan mode.                        */
/*   u8ChannelSelBitwise [in]:  Specify the input channels.                                                           */
/*                              For example, "u8ChannelSelBitwise=0x1(00000001b)" means the ADC channel 0 is enabled; */
/*                              "u8ChannelSelBitwise=0x19(00011001b)" means the ADC channel 0, 3 and 4 are enabled.   */
/*   ClockSrc            [in]:  Specify the clock source of ADC clock.                                                */
/*   u8AdcDivisor        [in]:  Determine the ADC clock frequency.                                                    */
/*                              ADC clock frequency = ADC clock source frequency / ( u8AdcDivisor + 1 )               */
/*                                                                                                                    */
/* Returns:                                                                                                           */
/*   None.                                                                                                            */
/*                                                                                                                    */
/* Description:                                                                                                       */
/*   Enable the ADC function. Configure the corresponding pins of the specified channels as analog input pins.        */
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvADC_Open(E_ADC_INPUT_MODE InputMode, E_ADC_OPERATION_MODE OpMode, uint8_t u8ChannelSelBitwise, E_ADC_CLK_SRC ClockSrc, uint8_t u8AdcDivisor)
{
	/* reset ADC */
	SYS->IPRSTC2.ADC_RST = 1;
	SYS->IPRSTC2.ADC_RST = 0;
	
	/* ADC clock source */
	SYSCLK->CLKSEL1.ADC_S = ClockSrc;
	
	/* Set ADC divisor */
	SYSCLK->CLKDIV.ADC_N = u8AdcDivisor;
	/* ADC engine clock enable */
	SYSCLK->APBCLK.ADC_EN = 1;
	/* ADC enable */
	ADC->ADCR.ADEN = 1;
	
	if( InputMode == ADC_SINGLE_END )
		ADC->ADCR.DIFFEN = 0;     /* single end input */
	else
		ADC->ADCR.DIFFEN = 1;     /* differential input */
	
	if( OpMode == ADC_SINGLE_OP )
		ADC->ADCR.ADMD = 0;     /* single mode */
	else if( OpMode == ADC_BURST_OP )
		ADC->ADCR.ADMD = 1;     /* burst mode */
	else if( OpMode == ADC_SINGLE_CYCLE_OP )
		ADC->ADCR.ADMD = 2;     /* single-cycle scan mode */
	else
		ADC->ADCR.ADMD = 3;     /* continuous scan mode */
	
	ADC->ADCHER.CHEN = u8ChannelSelBitwise;
	if(u8ChannelSelBitwise&1)
	{
		/* configure P1.0 as an analog input pin */
		SYS->P1_MFP.MFP0_AIN0_T2	= 1;
		SYS->P1_MFP.ALT0_AIN0_T2	= 0;
		/* configure P1.0 as input mode */
		PORT1->PMD.PMD0 = 0;
	}
	
	if(u8ChannelSelBitwise&2)
	{
		/* configure P1.1 as an analog input pin */
		SYS->P1_MFP.MFP1_AIN1_T3 	= 1;
		SYS->P1_MFP.ALT1_AIN1_T3	= 0;
		/* configure P1.1 as input mode */
		PORT1->PMD.PMD1 = 0;
	}
	
	if(u8ChannelSelBitwise&4)
	{
		/* configure P1.2 as an analog input pin */
		SYS->P1_MFP.MFP2_AIN2_RXD1 	= 1;
		SYS->P1_MFP.ALT2_AIN2_RXD1 	= 0;
		/* configure P1.2 as input mode */
		PORT1->PMD.PMD2 = 0;
	}
	
	if(u8ChannelSelBitwise&8)
	{
		/* configure P1.3 as an analog input pin */
		SYS->P1_MFP.MFP3_AIN3_TXD1 	= 1;
		SYS->P1_MFP.ALT3_AIN3_TXD1 	= 0;
		/* configure P1.3 as input mode */
		PORT1->PMD.PMD3 = 0;
	}
	
	if(u8ChannelSelBitwise&0x10)
	{
		/* configure P1.4 as an analog input pin */
		SYS->P1_MFP.MFP4_AIN4_SS0	= 1;
		SYS->P1_MFP.ALT4_AIN4_SS0 	= 0;
		/* configure P1.4 as input mode */
		PORT1->PMD.PMD4 = 0;
	}
	
	if(u8ChannelSelBitwise&0x20)
	{
		/* configure P1.5 as an analog input pin */
		SYS->P1_MFP.MFP5_AIN5_MOSI0	= 1;
		SYS->P1_MFP.ALT5_AIN5_MOSI0 = 0;
		/* configure P1.5 as input mode */
		PORT1->PMD.PMD5 = 0;
	}
	
	if(u8ChannelSelBitwise&0x40)
	{
		/* configure P1.6 as an analog input pin */
		SYS->P1_MFP.MFP6_AIN6_MISO0 = 1;
		SYS->P1_MFP.ALT6_AIN6_MISO0 = 0;
		/* configure P1.6 as input mode */
		PORT1->PMD.PMD6 = 0;
	}
	
	if(u8ChannelSelBitwise&0x80)
	{
		/* configure P1.7 as an analog input pin */
		SYS->P1_MFP.MFP7_AIN7_SCLK0 = 1;
		SYS->P1_MFP.ALT7_AIN7_SCLK0 = 0;
		/* configure P1.7 as input mode */
		PORT1->PMD.PMD7 = 0;
	}
	
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_Close                                                                                  */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Disable the ADC function, ADC engine clock and ADC interrupt.                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_Close(void)
{     
	ADC->ADCR.ADEN = 0;                /* ADC disable */
	SYSCLK->APBCLK.ADC_EN = 0;         /* ADC engine clock disable */
	ADC->ADCR.ADIE = 0;
	NVIC_DisableIRQ(ADC_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_SetADCChannel                                                                          */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    u8ChannelSelBitwise [in]:  Specify the analog input channels.                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Select ADC input channels.                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_SetADCChannel(uint8_t u8ChannelSelBitwise, E_ADC_INPUT_MODE InputMode)
{     
	ADC->ADCHER.CHEN = u8ChannelSelBitwise;
	if( (u8ChannelSelBitwise&1) || ((InputMode==ADC_DIFFERENTIAL)&&(u8ChannelSelBitwise&2)) )
	{
		/* configure P1.0 as an analog input pin */
		SYS->P1_MFP.MFP0_AIN0_T2	= 1;
		SYS->P1_MFP.ALT0_AIN0_T2	= 0;
		/* configure P1.0 as input mode */
		PORT1->PMD.PMD0 = 0;
	}
	
	if( (u8ChannelSelBitwise&2) || ((InputMode==ADC_DIFFERENTIAL)&&(u8ChannelSelBitwise&1)) )
	{
		/* configure P1.1 as an analog input pin */
		SYS->P1_MFP.MFP1_AIN1_T3 	= 1;
		SYS->P1_MFP.ALT1_AIN1_T3	= 0;
		/* configure P1.1 as input mode */
		PORT1->PMD.PMD1 = 0;
	}
	
	if( (u8ChannelSelBitwise&4) || ((InputMode==ADC_DIFFERENTIAL)&&(u8ChannelSelBitwise&8)) )
	{
		/* configure P1.2 as an analog input pin */
		SYS->P1_MFP.MFP2_AIN2_RXD1 	= 1;
		SYS->P1_MFP.ALT2_AIN2_RXD1 	= 0;
		/* configure P1.2 as input mode */
		PORT1->PMD.PMD2 = 0;
	}
	
	if( (u8ChannelSelBitwise&8) || ((InputMode==ADC_DIFFERENTIAL)&&(u8ChannelSelBitwise&4)) )
	{
		/* configure P1.3 as an analog input pin */
		SYS->P1_MFP.MFP3_AIN3_TXD1 	= 1;
		SYS->P1_MFP.ALT3_AIN3_TXD1 	= 0;
		/* configure P1.3 as input mode */
		PORT1->PMD.PMD3 = 0;
	}
	
	if( (u8ChannelSelBitwise&0x10) || ((InputMode==ADC_DIFFERENTIAL)&&(u8ChannelSelBitwise&0x20)) )
	{
		/* configure P1.4 as an analog input pin */
		SYS->P1_MFP.MFP4_AIN4_SS0	= 1;
		SYS->P1_MFP.ALT4_AIN4_SS0 	= 0;
		/* configure P1.4 as input mode */
		PORT1->PMD.PMD4 = 0;
	}
	
	if( (u8ChannelSelBitwise&0x20) || ((InputMode==ADC_DIFFERENTIAL)&&(u8ChannelSelBitwise&0x10)) )
	{
		/* configure P1.5 as an analog input pin */
		SYS->P1_MFP.MFP5_AIN5_MOSI0	= 1;
		SYS->P1_MFP.ALT5_AIN5_MOSI0 = 0;
		/* configure P1.5 as input mode */
		PORT1->PMD.PMD5 = 0;
	}
	
	if( (u8ChannelSelBitwise&0x40) || ((InputMode==ADC_DIFFERENTIAL)&&(u8ChannelSelBitwise&0x80)) )
	{
		/* configure P1.6 as an analog input pin */
		SYS->P1_MFP.MFP6_AIN6_MISO0 = 1;
		SYS->P1_MFP.ALT6_AIN6_MISO0 = 0;
		/* configure P1.6 as input mode */
		PORT1->PMD.PMD6 = 0;
	}
	
	if( (u8ChannelSelBitwise&0x80) || ((InputMode==ADC_DIFFERENTIAL)&&(u8ChannelSelBitwise&0x40)) )
	{
		/* configure P1.7 as an analog input pin */
		SYS->P1_MFP.MFP7_AIN7_SCLK0 = 1;
		SYS->P1_MFP.ALT7_AIN7_SCLK0 = 0;
		/* configure P1.7 as input mode */
		PORT1->PMD.PMD7 = 0;
	}
	
}

/*--------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_ConfigADCChannel7                                                                                 */
/*                                                                                                                    */
/* Parameters:        	                                                                                              */
/*   Ch7Src [in]:  Specify the analog input source.                                                                   */
/*                                                                                                                    */
/* Returns:                                                                                                           */
/*   None.                                                                                                            */
/*                                                                                                                    */
/* Description:                                                                                                       */
/*   Select the input signal source of channel 7.                                                                     */
/*   ADC channel 7 is a multi-function channel.                                                                       */
/*   Its input source could come from a external signal or a internal bandgap voltage.                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvADC_ConfigADCChannel7(E_ADC_CH7_SRC Ch7Src)
{
	ADC->ADCHER.PRESEL = Ch7Src;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_SetADCInputMode                                                                        */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    InputMode [in]:  Specify the input mode.                                                             */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set the ADC input mode.                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_SetADCInputMode(E_ADC_INPUT_MODE InputMode)
{
 	if( InputMode == ADC_SINGLE_END )
		ADC->ADCR.DIFFEN = 0;     /* single end input */
	else
		ADC->ADCR.DIFFEN = 1;     /* differential input */
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_SetADCOperationMode                                                                    */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    OpMode [in]:  Specify the operation mode.                                                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set the ADC operation mode.                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_SetADCOperationMode(E_ADC_OPERATION_MODE OpMode)
{
 	if( OpMode == ADC_SINGLE_OP )
		ADC->ADCR.ADMD = 0;         /* single mode */
	else if( OpMode == ADC_BURST_OP )
		ADC->ADCR.ADMD = 1;     /* burst mode */
	else if( OpMode == ADC_SINGLE_CYCLE_OP )
		ADC->ADCR.ADMD = 2;         /* single-cycle scan mode */
	else
		ADC->ADCR.ADMD = 3;         /* continuous scan mode */
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_SetADCClkSrc                                                                           */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    ClockSrc [in]:  Specify the ADC clock source.                                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set the ADC clock source.                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_SetADCClkSrc(E_ADC_CLK_SRC ClockSrc)
{
 	SYSCLK->CLKSEL1.ADC_S = ClockSrc; /* ADC clock source */
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_SetADCDivisor                                                                          */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    u8AdcDivisor [in]:  Specify the divisor value.                                                       */
/*                        ADC clock frequency = ADC clock source frequency / ( u8AdcDivisor + 1 )          */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set the divisor value of ADC clock.                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_SetADCDivisor(uint8_t u8AdcDivisor)
{
 	SYSCLK->CLKDIV.ADC_N = u8AdcDivisor;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_EnableADCInt                                                                           */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    Callback    [in]:  The callback function of ADC interrupt.                                           */
/*    u32UserData [in]:  This user's data will be passed to the callback function.                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Enable the ADC interrupt and setup callback function.                                                */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_EnableADCInt(DRVADC_ADC_CALLBACK Callback, uint32_t u32UserData)
{
	g_ptADCCallBack = Callback;
	g_pu32UserData[0] = u32UserData;
	_DRVADC_CLEAR_ADC_INT_FLAG();     /* clear the A/D interrupt flag for safe */
	ADC->ADCR.ADIE = 1;
	NVIC_EnableIRQ(ADC_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_DisableADCInt                                                                          */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Disable the ADC interrupt.                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_DisableADCInt(void)
{
	ADC->ADCR.ADIE = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_EnableADCCmp0Int                                                                        */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    Callback    [in]:  The callback function of ADC result monitor 0 (compare 0) interrupt.              */
/*    u32UserData [in]:  This user's data will be passed to the callback function.                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Enable the ADC compare 0 interrupt and setup callback function.                                      */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_EnableADCCmp0Int(DRVADC_ADCMP0_CALLBACK Callback, uint32_t u32UserData)
{
	g_ptADCMP0CallBack = Callback;
	g_pu32UserData[1] = u32UserData;
	_DRVADC_CLEAR_CMP0_INT_FLAG();        /* clear the ADC comparator 0 interrupt flag for safe */
	ADC->ADCMPR[0].CMPIE = 1;
	NVIC_EnableIRQ(ADC_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_DisableADCCmp0Int                                                                       */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Disable the ADC compare 0 interrupt.                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_DisableADCCmp0Int(void)
{
	ADC->ADCMPR[0].CMPIE = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_EnableADCCmp1Int                                                                        */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    Callback    [in]:  The callback function of ADC result monitor 1 (compare 1) interrupt.              */
/*    u32UserData [in]:  This user's data will be passed to the callback function.                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Enable the ADC compare 1 interrupt and setup callback function.                                      */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_EnableADCCmp1Int(DRVADC_ADCMP1_CALLBACK Callback, uint32_t u32UserData)
{
	g_ptADCMP1CallBack = Callback;
	g_pu32UserData[2] = u32UserData;
	_DRVADC_CLEAR_CMP1_INT_FLAG();        /* clear the ADC comparator 1 interrupt flag for safe */
	ADC->ADCMPR[1].CMPIE = 1;
	NVIC_EnableIRQ(ADC_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_DisableADCCmp1Int                                                                       */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Disable the ADC compare 1 interrupt.                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_DisableADCCmp1Int(void)
{
	ADC->ADCMPR[1].CMPIE = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: ADC_IRQHandler                                                                                */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    ADC Interrupt Service Routine.                                                                       */
/*    According to the interrupt flags to call the corresponding callback function.                        */
/*---------------------------------------------------------------------------------------------------------*/
void ADC_IRQHandler(void)
{
	if(ADC->ADSR.ADF==1)
	{
		if(g_ptADCCallBack)
			g_ptADCCallBack(g_pu32UserData[0]);
		
		/* clear the A/D conversion flag */
		/* "ADC->ADSR.ADF = 1;" is not recommended. It may clear CMPF0 and CMPF1. */
		outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x1);
	}
	
	if(ADC->ADSR.CMPF0==1)
	{
		if(g_ptADCMP0CallBack)
			g_ptADCMP0CallBack(g_pu32UserData[1]);
		
		/* clear the A/D compare flag 0 */
		/* "ADC->ADSR.CMPF0 = 1;" is not recommended. It may clear ADF and CMPF1. */
		outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x2);
	}
	
	if(ADC->ADSR.CMPF1==1)
	{
		if(g_ptADCMP1CallBack)
			g_ptADCMP1CallBack(g_pu32UserData[2]);
		
		/* clear the A/D compare flag 1 */
		/* "ADC->ADSR.CMPF1 = 1;" is not recommended. It may clear ADF and CMPF0. */
		outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x4);
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_GetConversionRate                                                                      */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    Conversion rate.                                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Return the A/D conversion rate (sample/second.)                                                      */
/*    The conversion rate depends on the clock source of ADC clock.                                        */
/*    It needs about 27 ADC clocks to complete an A/D conversion.                                          */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvADC_GetConversionRate(void)
{
	if( SYSCLK->CLKSEL1.ADC_S == 0 )           /* external 12MHz crystal clock */
	{
		return (__XTAL/(SYSCLK->CLKDIV.ADC_N + 1)/27);
	}
	else if( SYSCLK->CLKSEL1.ADC_S == 1 )      /* PLL clock */
	{
		return ( DrvSYS_GetPLLClockFreq()/(SYSCLK->CLKDIV.ADC_N + 1)/27);
	}
	else                                       /* internal 22MHz RC oscillator clock */
	{
		return (__IRC22M/(SYSCLK->CLKDIV.ADC_N + 1)/27);
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_EnableExtTrigger                                                                       */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    TriggerCondition [in]:  Specify the trigger condition.                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Allow the external trigger pin ADCST (P3.2) to be the trigger source of ADC.                         */
/*    The trigger condition could be low-level/high-level/falling-edge/positive-edge.                      */
/*    TRGE bit and ADST bit should be clear to 0 before changing TRGS bit.                                 */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_EnableExtTrigger(E_ADC_EXT_TRI_COND TriggerCondition)
{
	PORT3->PMD.PMD2 = 0;                    /* Configure P3.2 as an input pin */
	ADC->ADCR.ADST = 0;                   /* Stop A/D conversion */
	ADC->ADCR.TRGEN = 0;                  /* Disable the trigger source of ADCST pin */
	ADC->ADCR.TRGS = 0;                   /* Select the ADCST pin as the hardware trigger source */
	ADC->ADCR.TRGCOND = TriggerCondition; /* Set the trigger condition */
	ADC->ADCR.TRGEN = 1;                  /* Enable the trigger source of ADCST pin */
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_DisableExtTrigger                                                                      */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Prohibit the external ADC trigger.                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_DisableExtTrigger(void)
{
	ADC->ADCR.TRGEN = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_StartConvert                                                                           */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set the ADST bit of ADCR to start the A/D conversion action.                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_StartConvert(void)
{
	/* clear the A/D conversion interrupt flag (ADF) */
	/* "ADC->ADSR.ADF = 1;" is not recommended. It may clear CMPF0 and CMPF1 */
	outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x1);
	ADC->ADCR.ADST = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_StopConvert                                                                            */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Clear the ADST bit of ADCR to stop the A/D conversion action.                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_StopConvert(void)
{
	ADC->ADCR.ADST = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_IsConversionDone                                                                       */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    TRUE:  the conversion action is finished.                                                            */
/*    FALSE: the conversion action is in progress.                                                         */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Check whether the conversion action is finished or not.                                              */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvADC_IsConversionDone(void)
{
	if(ADC->ADSR.ADF==0)
		return FALSE;
	else
		return TRUE;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_GetConversionData                                                                      */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    u8ChannelNum [in]:  Specify the ADC channel.                                                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    Conversion result.                                                                                   */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Get the conversion result of the specified ADC channel.                                              */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvADC_GetConversionData(uint8_t u8ChannelNum)
{
	return (ADC->ADDR[u8ChannelNum].RSLT);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_IsDataValid                                                                            */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*   u8ChannelNum [in]:  Specify the ADC channel.                                                          */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    TRUE: valid.                                                                                         */
/*    FALSE: invalid.                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Check whether the conversion data is valid or not.                                                   */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvADC_IsDataValid(uint8_t u8ChannelNum)
{
	volatile uint8_t u8ChannelSelBitwise[8]={ 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80 };
	
	if(u8ChannelNum>7)
		return FALSE;
	if( ADC->ADSR.VALID & u8ChannelSelBitwise[u8ChannelNum] )
		return TRUE;
	else
		return FALSE;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_IsDataOverrun                                                                          */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*   u8ChannelNum [in]:  Specify the ADC channel.                                                          */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    TRUE: overrun.                                                                                       */
/*    FALSE: non-overrun.                                                                                  */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Check whether the conversion data is overrun or not.                                                 */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvADC_IsDataOverrun(uint8_t u8ChannelNum)
{
	volatile uint8_t u8ChannelSelBitwise[8]={ 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80 };
	
	if(u8ChannelNum>7)
		return FALSE;
	if( ADC->ADSR.OVERRUN & u8ChannelSelBitwise[u8ChannelNum] )
		return TRUE;
	else
		return FALSE;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_EnableADCCmp0                                                                            */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    u8CmpChannelNum [in]:  Specify the channel number that want to compare.                                */
/*    CmpCondition    [in]:  Specify the compare condition.                                                  */
/*                           LESS_THAN for the condition of "less than the compare data";                    */
/*                           GREATER_OR_EQUAL for the condition of "greater or equal to the compare data."   */
/*    u16CmpData      [in]:  Specify the compare data.                                                       */
/*    u8CmpMatchCount [in]:  Specify the compare match count.                                                */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    E_DRVADC_ARGUMENT: one of the input arguments is out of the range.                                     */
/*    E_SUCCESS: the compare function is enabled.                                                            */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Enable the ADC result monitor 0 and configure the necessary settings.                                  */
/*-----------------------------------------------------------------------------------------------------------*/
int32_t DrvADC_EnableADCCmp0(uint8_t u8CmpChannelNum, E_ADC_CMP_CONDITION CmpCondition, uint16_t u16CmpData, uint8_t u8CmpMatchCount)
{
	if(u8CmpChannelNum>7)
		return E_DRVADC_ARGUMENT;
	
	if(u16CmpData>0x0FFF)
		return E_DRVADC_ARGUMENT;
	
	if(u8CmpMatchCount>0x0F)
		return E_DRVADC_ARGUMENT;
	
	ADC->ADCMPR[0].CMPCH = u8CmpChannelNum;
	ADC->ADCMPR[0].CMPCOND = CmpCondition;
	ADC->ADCMPR[0].CMPD = u16CmpData;
	ADC->ADCMPR[0].CMPMATCNT = u8CmpMatchCount;
	ADC->ADCMPR[0].CMPEN = 1;
	
	return E_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_DisableADCCmp0                                                                           */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Disable the ADC result monitor 0.                                                                      */
/*-----------------------------------------------------------------------------------------------------------*/
void DrvADC_DisableADCCmp0(void)
{
	ADC->ADCMPR[0].CMPEN = 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_EnableADCCmp1                                                                            */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    u8CmpChannelNum [in]:  Specify the channel number that want to compare.                                */
/*    CmpCondition    [in]:  Specify the compare condition.                                                  */
/*                           LESS_THAN for the condition of "less than the compare data";                    */
/*                           GREATER_OR_EQUAL for the condition of "greater or equal to the compare data."   */
/*    u16CmpData      [in]:  Specify the compare data.                                                       */
/*    u8CmpMatchCount [in]:  Specify the compare match count.                                                */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    E_DRVADC_ARGUMENT: one of the input arguments is out of the range.                                     */
/*    E_SUCCESS: the compare function is enabled.                                                            */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Enable the ADC result monitor 1 and configure the necessary settings.                                  */
/*-----------------------------------------------------------------------------------------------------------*/
int32_t DrvADC_EnableADCCmp1(uint8_t u8CmpChannelNum, E_ADC_CMP_CONDITION CmpCondition, uint16_t u16CmpData, uint8_t u8CmpMatchCount)
{
	if(u8CmpChannelNum>7)
		return E_DRVADC_ARGUMENT;
	
	if(u16CmpData>0x0FFF)
		return E_DRVADC_ARGUMENT;
	
	if(u8CmpMatchCount>0x0F)
		return E_DRVADC_ARGUMENT;
	
	ADC->ADCMPR[1].CMPCH = u8CmpChannelNum;
	ADC->ADCMPR[1].CMPCOND = CmpCondition;
	ADC->ADCMPR[1].CMPD = u16CmpData;
	ADC->ADCMPR[1].CMPMATCNT = u8CmpMatchCount;
	ADC->ADCMPR[1].CMPEN = 1;
	
	return E_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_DisableADCCmp1                                                                           */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Disable the ADC result monitor 1.                                                                      */
/*-----------------------------------------------------------------------------------------------------------*/
void DrvADC_DisableADCCmp1(void)
{
	ADC->ADCMPR[1].CMPEN = 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_EnableSelfCalibration                                                                    */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Enable the self calibration function.                                                                  */
/*-----------------------------------------------------------------------------------------------------------*/
void DrvADC_EnableSelfCalibration(void)
{
	ADC->ADCALR.CALEN = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_IsCalibrationDone                                                                      */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    TRUE:  the self calibration action is finished.                                                      */
/*    FALSE: the self calibration action is in progress.                                                   */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Check whether the self calibration action is finished or not.                                        */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvADC_IsCalibrationDone(void)
{
	if(ADC->ADCALR.CALDONE == 1)
		return TRUE;
	else
		return FALSE;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_DisableSelfCalibration                                                                   */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Disable the self calibration function.                                                                 */
/*-----------------------------------------------------------------------------------------------------------*/
void DrvADC_DisableSelfCalibration(void)
{
	ADC->ADCALR.CALEN = 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_GetVersion                                                                               */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    Version number.                                                                                        */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Get the version number of M05xx ADC driver.                                                            */
/*-----------------------------------------------------------------------------------------------------------*/
uint32_t DrvADC_GetVersion (void)
{
	return DRVADC_VERSION_NUM;
}

