/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "NUC1xx.h"
#include "DrvSYS.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static uint32_t s_u32ExtClockKHz = 12000;

static BOD_CALLBACK BOD_callbackFn;
static PWRWU_CALLBACK PWRWU_callbackFn;

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_GetVersion                                                                             */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None	                                                         						   */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               The DrvSYS version number                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to get the DrvSYS version number                                    */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvSYS_GetVersion(void)
{
	return DRVSYS_VERSION_NUM;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_ReadProductID				                                               	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None    				   										   						   */
/*                     		                           	   												   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               Product ID																		   		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Read Product ID                             	                       					   */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvSYS_ReadProductID(void)
{
	return SYS->PDID;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_UnlockKeyAddr				                                               	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None    				   										   						   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               0 		Success																		   	   */
/*              <0		Failed					                                                           */
/* Description:                                                                                            */
/*               Unlock protected registers                             	                   			   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvSYS_UnlockKeyAddr(void)
{
	uint32_t lockbit;
	
	SYS->REGLOCK = 0x59;
	SYS->REGLOCK = 0x16;
	SYS->REGLOCK = 0x88;

	lockbit = SYS->REGLOCK & 0x01;
	return (lockbit)? 0:E_DRVSYS_ERR_UNLOCK_FAIL;
} 
 
/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_LockKeyAddr				                                               	   		   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None    				   										   						   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               0 		Success																		   	   */
/*              -1		Failed					                                                           */
/* Description:                                                                                            */
/*               Lock protected registers                             	                   			   	   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvSYS_LockKeyAddr(void)
{
	uint32_t lockbit;
	
	SYS->REGLOCK = 0x00;

	lockbit = SYS->REGLOCK & 0x01;
	return (lockbit)? E_DRVSYS_ERR_LOCK_FAIL:0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_GetRstSrc				                                               	   		   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None    				   										   						   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               Reset source																		   	   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Get reset source from last operation                             	           		   	   */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvSYS_GetRstSrc(void)
{
	return *((__IO uint32_t *) &SYS->RSTSRC);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_ClearRstSrc				                                               	   		   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32Src    	- [in]    The corresponding bit of reset source			   					   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               0																		   	   			   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Clear reset source by write 0                             	           		   	   		   */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvSYS_ClearRstSrc(uint32_t u32Src)
{
	*((__IO uint32_t *) &SYS->RSTSRC) = u32Src;
	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_ResetIP				                                               	   		   	   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               eIpRst  - [in]  	E_SYS_GPIO_RST  / E_SYS_TMR0_RST / E_SYS_TMR1_RST / E_SYS_TMR2_RST	   */
/*	 					    		E_SYS_TMR3_RST  / E_SYS_I2C0_RST / E_SYS_I2C1_RST / E_SYS_SPI0_RST	   */
/*									E_SYS_SPI1_RST  / E_SYS_SPI2_RST / E_SYS_SPI3_RST / E_SYS_UART0_RST	   */
/*									E_SYS_UART1_RST	/ E_SYS_PWM_RST	 / E_SYS_ACMP_RST / E_SYS_PS2_RST	   */
/*									E_SYS_CAN0_RST  / E_SYS_CAN1_RST / E_SYS_USBD_RST / E_SYS_ADC_RST	   */
/*									E_SYS_PDMA_RST		   										   		   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None																		   	   		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Reset coresponding IP                            	           		   	   		   		   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_ResetIP(E_SYS_IP_RST eIpRst)
{	
	uint32_t u32Reg;

	if (eIpRst == E_SYS_PDMA_RST)
	{
		SYS->IPRSTC1.PDMA_RST = 1;
		SYS->IPRSTC1.PDMA_RST = 0;
	}
	else
	{
		u32Reg = *((__IO uint32_t *) &SYS->IPRSTC2);
		*((__IO uint32_t *) &SYS->IPRSTC2) = u32Reg | (1<<eIpRst);
		*((__IO uint32_t *) &SYS->IPRSTC2) = u32Reg & ~(1<<eIpRst);
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_ResetCPU				                                               	   		   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None			   						   												   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None																		   	   		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Reset CPU                           	           		   	   		   					   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_ResetCPU(void)
{
	SYS->IPRSTC1.CPU_RST = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_ResetChip				                                               	   		   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None			   						   												   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None																		   	   		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Reset whole chip                           	           		   	   		   			   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_ResetChip(void)
{
	SYS->IPRSTC1.CHIP_RST = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_EnableBOD				                                               	   		   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               i32Enable		- [in] 	  1 : Enable / 0 : Disable 						   				   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None																		   	   		   */
/*              					                                                           			   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*               Enable BOD function                           	           		   	   		   			   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_EnableBOD(int32_t i32Enable)
{
	SYS->BODCR.BOD_EN = i32Enable;
}
	 
/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_SelectBODVolt				                                               	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Volt		- [in] 	  3:4.5V, 2:3.8V, 1:2.6V, 0:2.2V  									   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None																		   	   		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Select BOD threshold voltage                           	           		   	   		   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_SelectBODVolt(uint8_t u8Volt)
{
	SYS->BODCR.BOD_VL = u8Volt;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_EnableBODRst				                                               	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               i32Enable		- [in] 	  1: Enable BOD reset function / 0:Enable BOD interrupt function   */
/*               bodcallbackFn	- [in] 	  BOD Call back function  										   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None																		   	   		   */
/*              					                                                           			   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*               Enable BOD reset function or interrupt function, and install call back function if		   */
/*				 interrupt function is enabled.                           	           		   	   		   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_EnableBODRst(int32_t i32Enable, BOD_CALLBACK bodcallbackFn)
{
	SYS->BODCR.BOD_RSTEN = i32Enable;
	
	if (i32Enable)
	{		
		NVIC_DisableIRQ(BOD_IRQn);
	}
	else
	{ 
		BOD_callbackFn = bodcallbackFn;
		NVIC_EnableIRQ(BOD_IRQn);
	}	
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_EnableBODLowPowerMode				                                               	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               i32Enable	- [in] 	  1 : Enable / 0 : Disable 						   					   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None																		   	   		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Enable BOD low power mode                           	           		   	   			   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_EnableBODLowPowerMode(int32_t i32Enable)
{
	SYS->BODCR.BOD_LPM = i32Enable;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_GetBODState				                                               	   			   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None			  						   									   			   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               BOD output status  1: the detected voltage is lower than BOD threshold voltage			   */
/*									0: the detected voltage is higher than BOD threshold voltage		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Get BOD state                           	           		   	   			   			   */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvSYS_GetBODState(void)
{
	return SYS->BODCR.BOD_OUT;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_EnableLowVoltRst				                                               	   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               i32Enable	 - [in] 	  1 : Enable / 0 : Disable   						   			   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None 										 											   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Enable low voltage reset                           	           		   	   			   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_EnableLowVoltRst(int32_t i32Enable)
{
	SYS->BODCR.LVR_EN = i32Enable;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_EnableTempatureSensor				                                               	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               i32Enable		- [in] 	  1 : Enable / 0 : Disable   								   	   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None 										 											   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Enable Tempature Sensor                           	           		   	   			       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_EnableTempatureSensor(int32_t i32Enable)
{
	SYS->BODCR.VTEMP_EN = i32Enable;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_SetPORDisCode				                                               	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32Code	- [in]		POD DIS CODE  					   						    	   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None 										 											   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Set POD DIS CODE for power on reset enable control            		   	   			       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_SetPORDisCode(uint32_t u32Code)
{
	SYS->PORCR = u32Code;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_SetRCAdjValue				                                               	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32Adj		- [in]	RC adjustment value  						   					       */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None 										 											   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Set RC adjustment value            		   	   			       						   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_SetRCAdjValue(uint32_t u32Adj)
{
	SYS->RCADJ = u32Adj;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     <BOD_IRQHandler>                                                                          */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Side effects:                                                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               ISR to handle BOD interrupt event           		                                       */
/*---------------------------------------------------------------------------------------------------------*/
void BOD_IRQHandler(void)
{
	BOD_callbackFn();
	DrvSYS_EnableBOD(0);
	DrvSYS_EnableBOD(1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_SetIPClock				                                        	       	  	   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               eIpClk    	- [in]  E_SYS_WD_CLK    / E_SYS_RTC_CLK   / E_SYS_TMR0_CLK  / E_SYS_TMR1_CLK   */
/*	 					    		E_SYS_TMR2_CLK  / E_SYS_TMR3_CLK  / E_SYS_I2C0_CLK  / E_SYS_I2C1_CLK   */
/*									E_SYS_SPI0_CLK  / E_SYS_SPI1_CLK  / E_SYS_SPI2_CLK  / E_SYS_SPI3_CLK   */
/*									E_SYS_UART0_CLK	/ E_SYS_UART1_CLK / E_SYS_PWM10_CLK / E_SYS_PWM32_CLK  */
/*									E_SYS_CAN0_CLK  / E_SYS_CAN1_CLK  / E_SYS_USBD_CLK  / E_SYS_ADC_CLK	   */
/*									E_SYS_ACMP_CLK	/ E_SYS_PS2_CLK   / E_SYS_PDMA_CLK  / E_SYS_ISP_CLK    */
/* 				 i32Enable	- [in] 	1 : Enable / 0 : Disable                    						   */
/* Returns:                                                                                                */
/*               None																		   	   		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Enable/Disable coresponding IP                            	  		   	   		   		   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_SetIPClock(E_SYS_IP_CLK eIpClk, int32_t i32Enable)
{	
	uint32_t u32Reg;

	if (eIpClk == E_SYS_PDMA_CLK)
	{
		SYSCLK->AHBCLK.PDMA_EN = i32Enable;
	}
	else if (eIpClk == E_SYS_ISP_CLK)
	{
		SYSCLK->AHBCLK.ISP_EN = i32Enable;
	}
	else
	{
		u32Reg = *((__IO uint32_t *) &SYSCLK->APBCLK);
		
		if (i32Enable)
		{
			*((__IO uint32_t *) &SYSCLK->APBCLK) = u32Reg | (1<<eIpClk);
		}
		else
		{
			*((__IO uint32_t *) &SYSCLK->APBCLK) = u32Reg & ~(1<<eIpClk);
		}
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_SetHCLKSource				                                               	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*           u8ClkSrcSel       - [in] 	0: External 12M clock                      	   					   */
/*          							1: External 32K clock                      	   					   */
/*            							2: PLL clock                      	   							   */
/*            							3: Internal 10K clock                      	   					   */
/*            							4~7: Internal 22M clock                      	   				   */
/*              											                                        	   */
/* Returns:                                                                                                */
/*                 0:  Success																			   */
/*               < 0:  Wrong value											                    		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               This function is used to select HCLK clock source                                   	   */		
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvSYS_SetHCLKSource(uint8_t u8ClkSrcSel)
{
	if (u8ClkSrcSel > 7)
	{
		return E_DRVSYS_ERR_ARGUMENT;
	}
	else
	{
		SYSCLK->CLKSEL0.HCLK_S = u8ClkSrcSel;
	}
	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_SetSysTickSource				                                              		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*           u8ClkSrcSel       - [in] 	1: External 32K clock                      	   					   */
/*          							2: External 12M clock / 2                      	   				   */
/*            							3: HCLK / 2                      	   							   */
/*            							4~7: Internal 22M clock / 2                     	   			   */
/*              											                                        	   */
/* Returns:                                                                                                */
/*                 0:  Success																			   */
/*               < 0:  Wrong value											                    		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               This function is used to select sysyem tick clock source                              	   */		
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvSYS_SetSysTickSource(uint8_t u8ClkSrcSel)
{
	if ((u8ClkSrcSel > 7) || (u8ClkSrcSel == 0))
	{
		return E_DRVSYS_ERR_ARGUMENT;
	}
	else
	{
		SYSCLK->CLKSEL0.STCLK_S = u8ClkSrcSel;
	}
	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_SetIPClockSource	     		                                               	   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               eIpClkSrc	 - [in]     E_SYS_WDG_CLKSRC  / E_SYS_ADC_CLKSRC  / E_SYS_TMR0_CLKSRC	 	   */
/*               	     		 		E_SYS_TMR1_CLKSRC / E_SYS_TMR2_CLKSRC / E_SYS_TMR3_CLKSRC		   */
/*               	     		 		E_SYS_UART_CLKSRC / E_SYS_CAN_CLKSRC  / E_SYS_PWM10_CLKSRC		   */
/*               	     		 		E_SYS_PWM32_CLKSRC 						   						   */
/*               u8ClkSrcSel - [in]    Corresponding clock source                     	 				   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*                 0:  Success																			   */
/*               < 0:  Wrong value											                    		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               This function is used to select IP clock source	       		   	   		   			   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvSYS_SetIPClockSource(E_SYS_IP_CLKSRC eIpClkSrc, uint8_t u8ClkSrcSel)
{
	
	switch(eIpClkSrc)
	{
		case E_SYS_PWM32_CLKSRC:  
			SYSCLK->CLKSEL1.PWM32_S = u8ClkSrcSel;
			break;
		case E_SYS_PWM10_CLKSRC:
			SYSCLK->CLKSEL1.PWM10_S = u8ClkSrcSel;
			break;
		case E_SYS_CAN_CLKSRC:
		    SYSCLK->CLKSEL1.CAN_S = u8ClkSrcSel;
			break;
		case E_SYS_UART_CLKSRC:
		    SYSCLK->CLKSEL1.UART_S = u8ClkSrcSel;
			break;
		case E_SYS_TMR3_CLKSRC:
		    SYSCLK->CLKSEL1.TMR3_S = u8ClkSrcSel;
			break;				
		case E_SYS_TMR2_CLKSRC:
		    SYSCLK->CLKSEL1.TMR2_S = u8ClkSrcSel;
			break;				
		case E_SYS_TMR1_CLKSRC:
		    SYSCLK->CLKSEL1.TMR1_S = u8ClkSrcSel;
			break;				
		case E_SYS_TMR0_CLKSRC:
		    SYSCLK->CLKSEL1.TMR0_S = u8ClkSrcSel;
			break;				
		case E_SYS_ADC_CLKSRC:
		    SYSCLK->CLKSEL1.ADC_S = u8ClkSrcSel;
			break;				
		case E_SYS_WDG_CLKSRC:
		    SYSCLK->CLKSEL1.WDG_S = u8ClkSrcSel;
			break;				
		default:
			return E_DRVSYS_ERR_IPSRC;
	}
	
	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_SetClockDivider 		                                               	   		   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*           eIpDiv   - [in] E_SYS_ADC_DIV / E_SYS_CAN_DIV / E_SYS_UART_DIV / E_SYS_USB_DIV /E_SYS_HCLK_DIV*/
/*           i32value - [in] 0 ~ 255 for ADC, 0 ~ 15 for other IP or HCLK  								   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*           0:  Success																			   	   */
/*           < 0:  Wrong value											                    		       */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               This function is used to set IP divider number from the corresponding clock source	   	   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvSYS_SetClockDivider(E_SYS_IP_DIV eIpDiv , int32_t i32value)
{
	if (eIpDiv == E_SYS_ADC_DIV)
	{
	 	SYSCLK->CLKDIV.ADC_N = i32value;
	}
	else if(eIpDiv == E_SYS_CAN_DIV)
	{
	 	SYSCLK->CLKDIV.CAN_N = i32value;
	}
	else if(eIpDiv == E_SYS_UART_DIV)
	{
	 	SYSCLK->CLKDIV.UART_N = i32value;
	}
	else if(eIpDiv == E_SYS_USB_DIV)
	{
	 	SYSCLK->CLKDIV.USB_N = i32value;
	}
	else if(eIpDiv == E_SYS_HCLK_DIV)
	{
	 	SYSCLK->CLKDIV.HCLK_N = i32value;
	}
	else
		return E_DRVSYS_ERR_IPDIV;

	return E_SUCCESS;
	
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_SetOscCtrl	     		                                               	   	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               eOscCtrl	 - [in]    E_SYS_XTL12M  / E_SYS_XTL32K  / E_SYS_OSC22M / E_SYS_OSC10K	 	   */
/*               i32Enable   - [in]    1: Enable / 0: Disable                     	 					   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*                 0:  Success																			   */
/*               < 0:  Wrong value											                    		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               This function is used to enable/disable Oscillator	       		   	   		   			   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvSYS_SetOscCtrl(E_SYS_OSC_CTRL eOscCtrl, int32_t i32Enable)
{	
	switch(eOscCtrl)
	{
		case E_SYS_XTL12M:  
			SYSCLK->PWRCON.XTL12M_EN = i32Enable;
			break;
		case E_SYS_XTL32K:
			SYSCLK->PWRCON.XTL32K_EN = i32Enable;
			break;
		case E_SYS_OSC22M:
		    SYSCLK->PWRCON.OSC22M_EN = i32Enable;
			break;
		case E_SYS_OSC10K:
		    SYSCLK->PWRCON.OSC10K_EN = i32Enable;
			break;				
		default:
			return E_DRVSYS_ERR_ARGUMENT;
	}
	
	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     <PWRWU_IRQHandler>                                                                          */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               ISR to handle Power Down Wake up interrupt event           		                                       */
/*---------------------------------------------------------------------------------------------------------*/
void PWRWU_IRQHandler(void)
{
	SYSCLK->PWRCON.PD_WU_STS = 0;
	if (PWRWU_callbackFn != NULL)
		PWRWU_callbackFn();	
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_EnablePWRWUInt				                                               	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               i32Enable	    - [in]	1: Enable Power dowm Wake up interrup 							   */
/*									  	0: Disable Power dowm Wake up interrup	   						   */
/*               pdwucallbackFn	- [in]	Power Down Wake Up Call back function  						   	   */
/*               i32enWUDelay	- [in]	1: Enable 64 clock cycle delay 							  		   */
/*									  	0: Disable 64 clock cycle delay	   						   		   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None																		   	   		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Enable or disable PD_WU interrupt function, and install call back function, and 		   */
/*				 enable 64 clock cycle delay to wait oscillator clock stable.          		   	   		   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_EnablePWRWUInt(int32_t i32Enable, PWRWU_CALLBACK pdwucallbackFn, int32_t i32enWUDelay)
{
	SYSCLK->PWRCON.WINT_EN = i32Enable;
	
	if (i32Enable)
	{		
		SYSCLK->PWRCON.WU_DLY = i32enWUDelay;
		PWRWU_callbackFn = pdwucallbackFn;
		NVIC_EnableIRQ(PWRWU_IRQn);
	}
	else
	{ 
		SYSCLK->PWRCON.WU_DLY = 0;
		PWRWU_callbackFn = NULL;
		NVIC_DisableIRQ(PWRWU_IRQn);
	}	
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_EnablePowerDown				                                               	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               i32Enable	- [in]	 1: Enable / 0: Disable 						   					   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None																		   	   		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Enable or active power down function                      		   	   		   			   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_EnablePowerDown(int32_t i32Enable)
{
	SYSCLK->PWRCON.PWR_DOWN = i32Enable;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_SetPowerDownWaitCPU				                                               	   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               i32Enable	- [in]  1: Enter power down mode when PWR_DOWN and CPU runs WFE/WFI instruction*/
/*               			 		0: Enter power down mode when PWR_DOWN is set 						   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None																		   	   		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               This function is used to control power down entry condition         		   			   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_SetPowerDownWaitCPU(int32_t i32Enable)
{
	SYSCLK->PWRCON.PD_WAIT_CPU = i32Enable;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_SetPllSrc				                                               	   		   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*           u8ClkSrcSel       - [in] 	E_DRVSYS_EXT_12M: External 12M clock               				   */
/*          							E_DRVSYS_INT_22M: Internal 22M clock               				   */
/*              											                                        	   */
/* Returns:                                                                                                */
/*               None																					   */
/*               							                                                               */
/* Description:                                                                                            */
/*               This function is used to select PLL clock source                                   	   */		
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_SetPllSrc(E_DRVSYS_SRC_CLK ePllSrc)
{
	SYSCLK->PLLCON.PLL_SRC = ePllSrc;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_SetPLLPowerDown 		                                               	   		   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               i32Enable			  1: Power down mode / 0: Normal mode 								   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None																		   	   		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Enable PLL power down function                           	           		   	   		   			   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSYS_SetPLLPowerDown(int32_t i32Enable)
{
	SYSCLK->PLLCON.PD = i32Enable;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_GetEXTClock                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      The external clock frequency.                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*              Get external clock frequency. The clock UNIT is in kHz.                                    */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvSYS_GetEXTClock(void)
{	
	return s_u32ExtClockKHz;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_GetPllContent                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      u32ExtClockKHz - [in], The external clock frequency (kHz).                                         */
/*      u32PllClockKHz - [in], The target PLL clock frequency (kHz).                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      The PLL register setting for the target PLL clock frequency.                                       */
/*                                                                                                         */
/* Description:                                                                                            */
/*      This function is used to calculate the nearest PLL frequency to fit the target PLL frequency       */
/*      which is defined by u32PllClockKHz and return the relative PLL control register setting.           */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvSYS_GetPllContent(uint32_t u32ExtClockKHz, uint32_t u32PllClockKHz)
{
	uint32_t u32NR, u32NF;
	uint32_t u32Register=0;

    u32PllClockKHz = u32PllClockKHz << 2;
    
    u32NF = u32PllClockKHz / 1000;
    u32NR = u32ExtClockKHz / 1000;
   
    if (SYSCLK->PLLCON.PLL_SRC == 0)
	{
		u32NR >>= 2;
		u32NF >>= 2;	
	}
	else
	{
		u32NR >>= 1;
		u32NF >>= 1;	
	}
		
	u32Register = (0x03<<14)| ((u32NR - 2)<<9) | (u32NF - 2);

    /* Return the nearest value */
	return u32Register;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_GetPLLClock                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      The PLL clock.                                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*              Get PLL clock frquency. The clock UNIT is in kHz.                                          */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvSYS_GetPLLClock(void)
{
	uint32_t u32Freq =0, u32PLLSrc;
	uint32_t au32Map[4] = {1, 2, 2, 4};

	if (SYSCLK->PLLCON.PLL_SRC == 1)
	{
		u32PLLSrc = 22000;	
	}
	else
	{
		u32PLLSrc = s_u32ExtClockKHz;
	}

	if (SYSCLK->PLLCON.PD ==1)	
		u32Freq = 0;	
	else		
		u32Freq = u32PLLSrc * (SYSCLK->PLLCON.FB_DV+2) / (SYSCLK->PLLCON.IN_DV+2) / au32Map[SYSCLK->PLLCON.OUT_DV];

	return u32Freq;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_GetHCLK                                                                            	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      HCLK clock frequency in kHz.                                                                       */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Get HCLK clock and the UNIT is in kHz.                                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvSYS_GetHCLK(void)
{
	
	uint32_t u32Freqout = 0, u32AHBDivider;

	if (SYSCLK->CLKSEL0.HCLK_S == 0)		/* external 12MHz crystal clock */
	{
		u32Freqout = s_u32ExtClockKHz;	
	}
	else if(SYSCLK->CLKSEL0.HCLK_S == 1)    /* external 32KHz crystal clock */ 
	{
		u32Freqout = 32;
	}
	else if(SYSCLK->CLKSEL0.HCLK_S == 2)	/* PLL clock */
	{
		u32Freqout = DrvSYS_GetPLLClock();
	}
	else if(SYSCLK->CLKSEL0.HCLK_S == 3)	/* internal 10KHz oscillator clock */
	{
	 	u32Freqout = 10;
	}
	else									/* internal 22MHz oscillator clock */
	{
	 	u32Freqout = 22000;
	
	}
	u32AHBDivider = SYSCLK->CLKDIV.HCLK_N + 1 ;
	
	return (u32Freqout/u32AHBDivider);	
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSYS_Open                                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      u32ExtClockKHz - [in], The external clock frequency in kHz.                                        */
/*      u32PllClockKHz - [in], The target PLL clock frequency in kHz.                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      E_SUCCESS                                                                                          */
/*                                                                                                         */
/* Description:                                                                                            */
/*      This could configure the PLL according to the external clock and target PLL clock.                 */
/*      NOTE: Due to hardware limitation, the actual PLL clock may be different to target PLL clock.       */
/*            The DrvSYS_GetPLLClock() could be used to get actual PLL clock.                              */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvSYS_Open(uint32_t u32ExtClockKHz, uint32_t u32PllClockKHz)
{
	uint32_t u32PllCr;	
	volatile uint32_t delayCnt;
    uint32_t isOsc22mEn;
	
	s_u32ExtClockKHz = u32ExtClockKHz;	
	u32PllCr = DrvSYS_GetPllContent(u32ExtClockKHz, u32PllClockKHz);

    /* Backup internal 22MHz clock status */
    isOsc22mEn = SYSCLK->PWRCON.OSC22M_EN;

    /* Force to internal 22MHz clock */
    SYSCLK->PWRCON.OSC22M_EN = 1;
	DrvSYS_SetHCLKSource(4);
	
	SYSCLK->PLLCON.FB_DV  = (u32PllCr & 0x01FF);
	SYSCLK->PLLCON.IN_DV  = (u32PllCr >> 9) & 0x1F;
	SYSCLK->PLLCON.OUT_DV = (u32PllCr >> 14) & 0x03;
	DrvSYS_SetPLLPowerDown(0);
	
	/*Delay for PLL stable*/
	for (delayCnt=0;delayCnt<10000;delayCnt++);

	/* Change HCLK clock source to be PLL. */
	DrvSYS_SetHCLKSource(2);

    /* Restore internal 22MHz clock source status */ 
    SYSCLK->PWRCON.OSC22M_EN = isOsc22mEn;
	
	return E_SUCCESS;

}




