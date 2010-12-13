/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "DrvI2C.h"

#define I2C_STA              0x20
#define I2C_STO              0x10
#define I2C_SI               0x08
#define I2C_AA               0x04

static I2C_CALLBACK_T I2CHandler = {0};
		  

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     <I2C_IRQHandler>                                                                          */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               ISR to handle I2C interrupt event           		                                       */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_IRQHandler(void)
{
    uint32_t status;

  	status  = I2C->STATUS;

    if (I2C->TOC.TIF)
	{
		I2C->TOC.TIF = 1;	/* Clear TIF */
       	
		if (I2CHandler.TimeoutCallBackFn)
		{
			I2CHandler.TimeoutCallBackFn(status); 
		}
	}
	else
	{
		switch (status)
    	{	
     		case 0x38:		/* Arbitration loss */
     		{
        		if (I2CHandler.ArbitLossCallBackFn)
					I2CHandler.ArbitLossCallBackFn(status); 
     	    	break;
     		}
 			case 0x00: 		/* Bus error */
			{
        		if (I2CHandler.BusErrCallBackFn)
					I2CHandler.BusErrCallBackFn(status); 
     	    	break;
			}
			default:
			{
        		if (I2CHandler.I2CCallBackFn)
					I2CHandler.I2CCallBackFn(status); 			
			}		
		}		
	}
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_Ctrl				                                                   			   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               start 		- [in]		1:Enable / 0:Disable	       		               				   */
/*               stop 		- [in]		1:Enable / 0:Disable	       		                               */
/*               intFlag 	- [in]		Wrtie '1' to clear this flag	       		                       */
/*               ack 		- [in]		1:Enable / 0:Disable	       		                               */
/* Returns:                                                                                                */
/*               None											                    		       		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Set I2C control bit										           		   			   */		
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2C_Ctrl(uint8_t start, uint8_t stop, uint8_t intFlag, uint8_t ack)
{	
	uint32_t Reg = 0;
		
	if (start)
		Reg |= I2C_STA;
	if (stop)
	    Reg |= I2C_STO;
	if (intFlag)
		Reg |= I2C_SI;
	if (ack)
		Reg |= I2C_AA;

	*((__IO uint32_t *)&I2C->CON) = (*((__IO uint32_t *)&I2C->CON) & ~0x3C) | Reg;	    	
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_WriteData				                                                   			   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8data 	- [in]		Byte Data	       		                           				   */
/* Returns:                                                                                                */
/*               None											                    		       		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Write Data into I2C Data register										           		   */		
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2C_WriteData(uint8_t u8data)
{
	I2C->DATA = u8data;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_ReadData				                                                   			   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None	       		                               										   */
/* Returns:                                                                                                */
/*               Data from I2C Data register											       		   	   */
/*                                                                                      			       */
/* Description:                                                                                            */
/*               Read Data from I2C Data register										               	   */		
/*---------------------------------------------------------------------------------------------------------*/
uint8_t DrvI2C_ReadData(void)
{
	return I2C->DATA;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_GetIntFlag				                                                   			   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None											       		                               */
/* Returns:                                                                                                */
/*               0 : Flag is not set											                    	   */
/*               1 : Flag is set							                                               */
/* Description:                                                                                            */
/*               Get I2C interrupt flag										               				   */		
/*---------------------------------------------------------------------------------------------------------*/
uint8_t DrvI2C_GetIntFlag(void)
{
	return I2C->CON.SI;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_ClearIntFlag				                                                   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None											       		                               */
/* Returns:                                                                                                */
/*               None											                    		       		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Clear I2C interrupt flag										       					   */		
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2C_ClearIntFlag(void)
{
	I2C->CON.SI = 1;	    	
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_ClearTimeoutFlag				                                                   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None											       		                               */
/* Returns:                                                                                                */
/*               None											                    		       		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Clear I2C time out flag										          				   */		
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2C_ClearTimeoutFlag(void)
{
	I2C->TOC.TIF = 1;  	
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_GetStatus				                                                   			   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None											       		                               */
/* Returns:                                                                                                */
/*               status											                    		       		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Get I2C status. There are 26 status codes. Please refer to data sheet in detail.   	   */		
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvI2C_GetStatus(void)
{
	return I2C->STATUS;	
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_Open				                                                   				   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32BusClock   - [in]		I2C Bus Clock (Hz)       		                               */
/* Returns:                                                                                                */
/*               0 : Success											                    		       */
/*               							                                                               */
/* Description:                                                                                            */
/*               Enable I2C function and set clock divider. I2C clock = I2C source clock / (4x(divider+1)) */		
/*               The maximum of I2C clock is 1MHz.														   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_Open(uint32_t u32BusClock)
{
    uint32_t divider;    
 
 	divider = (uint32_t) (((SystemCoreClock * 10)/(u32BusClock * 4) + 5) / 10 - 1);	/* Compute proper divider for I2C clock */
	
	SYSCLK->APBCLK.I2C_EN = 1;
	SYS->IPRSTC2.I2C_RST = 1;
	SYS->IPRSTC2.I2C_RST = 0;	
	I2C->CON.ENSI = 1;
	I2C->CLK = divider;

	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_Close				                                                   				   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None											       		                               */
/* Returns:                                                                                                */
/*               0 : Success											                    		       */
/*               							                                                               */
/* Description:                                                                                            */
/*               Disable I2C function and clock source										               */		
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_Close(void)
{
	I2C->CON.ENSI = 0;
	SYS->IPRSTC2.I2C_RST = 1;
	SYS->IPRSTC2.I2C_RST = 0;
	SYSCLK->APBCLK.I2C_EN = 0;
	
	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_SetClockFreq				                                               			   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32BusClock  - [in]		I2C Bus Clock (Hz)       		                               */
/* Returns:                                                                                                */
/*               0 : Success 											                    		       */
/*               							                                                               */
/* Description:                                                                                            */
/*               Set clock divider. I2C clock = I2C source clock / (4x(divider+1))		   				   */		
/*               The maximum of I2C clock is 1MHz.														   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_SetClockFreq(uint32_t u32BusClock)
{
    uint32_t divider;     
 
	divider = (uint32_t) (((SystemCoreClock * 10)/(u32BusClock * 4) + 5) / 10 - 1);

	I2C->CLK = divider;

	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_GetClockFreq				                                              			   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None       		            													       */
/* Returns:                                                                                                */
/*               I2C Bus Clock 											                    		       */
/*               							                                                               */
/* Description:                                                                                            */
/*               Get I2C Bus Clock										           						   */		
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvI2C_GetClockFreq(void)
{
    uint32_t divider;
	 
	divider = I2C->CLK;
	return ( SystemCoreClock / ((divider+1)<<2) );
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_EnableInt				                                                   			   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None											       		                               */
/* Returns:                                                                                                */
/*               0 : Success  											                    		       */
/*               							                                                               */
/* Description:                                                                                            */
/*               Enable I2C interrupt and NVIC corresponding to I2C		        						   */		
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_EnableInt(void)
{
	I2C->CON.EI = 1;

	NVIC_EnableIRQ(I2C0_IRQn);

	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_DisableInt				                                                   			   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None											       		                               */
/* Returns:                                                                                                */
/*               0 : Success  											                    		       */
/*               							                                                               */
/* Description:                                                                                            */
/*               Disable I2C interrupt and NVIC corresponding to I2C		        					   */		
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_DisableInt(void)
{
	I2C->CON.EI = 0;

	NVIC_DisableIRQ(I2C0_IRQn);

	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_SetTimeoutCounter				                                       	            	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               i32enable 	   - [in]		1:Enable / 0:Disable	       		                           */
/*               u8div4 	   - [in]		1:Enable / 0:Disable	       		                           */
/* Returns:                                                                                                */
/*               0 : Success  											                    		       */
/*               							                                                               */
/* Description:                                                                                            */
/*               Enable/Disable I2C 14-bit timeout counter and set div4 bit of timeout counter			   */		
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_SetTimeoutCounter(int32_t i32enable, uint8_t u8div4)
{
	I2C->TOC.DIV4 = u8div4;
	I2C->TOC.ENTI = i32enable;
	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_SetAddress				                                                   	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               slaveNo 	   - [in]		There are 4 slave addrss. The slaveNo is 0 ~ 3.        		   */
/*               slave_addr    - [in]		7-bit data	       		                           			   */
/*               GC_Flag 	   - [in]		1:Enable / 0:Disable	       		                           */
/* Returns:                                                                                                */
/*               0 : Success 											                    		       */
/*               							                                                               */
/* Description:                                                                                            */
/*               Set 4 7-bit slave addresses and enable General Call function		        			   */		
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_SetAddress(uint8_t slaveNo, uint8_t slave_addr, uint8_t GC_Flag)
{
	switch (slaveNo)
	{
		case 0:
			I2C->ADDR0.ADDR  = slave_addr;
			I2C->ADDR0.GC    = GC_Flag;
			break;
		case 1:
			I2C->ADDR1.ADDR = slave_addr;
			I2C->ADDR1.GC   =	GC_Flag;
			break;
		case 2:
			I2C->ADDR2.ADDR = slave_addr;
			I2C->ADDR2.GC   =	GC_Flag;
			break;
		case 3:
			I2C->ADDR3.ADDR = slave_addr;
			I2C->ADDR3.GC   =	GC_Flag;
			break;
		default:
			return E_DRVI2C_ERR_ARGUMENT;
	}	

	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_SetAddressMask				                                               	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               slaveNo 	   - [in]		There are 4 slave addrss. The slaveNo is 0 ~ 3.        		   */
/*               slaveAddrMask - [in]		7-bit data	       		                           			   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               0 : Success											                    		       */
/*               							                                                               */
/* Description:                                                                                            */
/*               Set 4 7-bit slave address mask. The corresponding address bit is "Don't Care".			   */		
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_SetAddressMask(uint8_t slaveNo, uint8_t slaveAddrMask)
{
	slaveAddrMask <<= 1;

	switch (slaveNo)
	{
		case 0:
			I2C->ADRM0.ADM = slaveAddrMask;
			break;
		case 1:
			I2C->ADRM1.ADM = slaveAddrMask;
			break;
		case 2:
			I2C->ADRM2.ADM = slaveAddrMask;
			break;
		case 3:
			I2C->ADRM3.ADM = slaveAddrMask;
			break;
		default:
			return E_DRVI2C_ERR_ARGUMENT;
	}	
	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_InstallCallback				                                               	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               Type 	   	   - [in]		I2CFUNC / ARBITLOSS / BUSERROR / TIMEOUT      				   */
/*               callbackfn    - [in]		Call back function	       		                           	   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               0		 Success																		   */
/*               <0      Failed 											                    		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Install I2C call back function for I2C normal function, Arbitration loss, Bus error	   */
/*                                                  and Counter timeout .			   					   */		
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_InstallCallback(E_I2C_CALLBACK_TYPE Type, I2C_CALLBACK callbackfn)
{
	switch(Type)
    {
       	case I2CFUNC:
       	{
           	I2CHandler.I2CCallBackFn = callbackfn;  
       		break;
       	}
       	case ARBITLOSS:
       	{
           	I2CHandler.ArbitLossCallBackFn = callbackfn;  
       		break;
       	}
        case BUSERROR:
        {
          	I2CHandler.BusErrCallBackFn = callbackfn;  
       		break;
       	}        
       	case TIMEOUT:
       	{
           	I2CHandler.TimeoutCallBackFn = callbackfn;  
       		break;
       	}
		default:
			return E_DRVI2C_ERR_ARGUMENT;                             	
	}
	
	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_UninstallCallBack				                                               	   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               Type 	   	   - [in]		I2CFUNC / ARBITLOSS / BUSERROR / TIMEOUT      				   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               0		 Success																		   */
/*               <0      Failed 											                    		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Uninstall I2C call back function for I2C normal function, Arbitration loss, Bus error	   */
/*                                                  and Counter timeout .			   					   */		
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_UninstallCallBack(E_I2C_CALLBACK_TYPE Type)
{
	switch(Type)
    {
       	case I2CFUNC:
       	{
           	I2CHandler.I2CCallBackFn = 0;  
       		break;
       	}
       	case ARBITLOSS:
       	{
           	I2CHandler.ArbitLossCallBackFn = 0;  
       		break;
       	}
       	case BUSERROR:
       	{
           	I2CHandler.BusErrCallBackFn = 0;  
       		break;
       	}        
       	case TIMEOUT:
       	{
          	I2CHandler.TimeoutCallBackFn = 0;  
       		break;
       	}
		default:
			return E_DRVI2C_ERR_ARGUMENT;                             	
	}	                  	

	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_GetVersion                                                                             */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None	                                                         						   */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               The DrvI2C version number                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to get the DrvI2C version number                                    */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvI2C_GetVersion(void)
{
	return DRVI2C_VERSION_NUM;
}

