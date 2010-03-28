/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "NUC1xx.h"
#include "DrvGPIO.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define PORT_OFFSET   0x40

/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static void (*_pfGPABCallback)(uint32_t u32GpaStatus, uint32_t u32GpbStatus);
static void (*_pfGPCDECallback)(uint32_t u32GpcStatus, uint32_t u32GpdStatus, uint32_t u32GpeStatus);
static void (*_pfEINT0Callback)(void);
static void (*_pfEINT1Callback)(void);              		 

/*---------------------------------------------------------------------------------------------------------*/
/* GPIO A and GPIO B  ISR                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void GPAB_IRQHandler(void)
{
	uint32_t u32GpaStatus, u32GpbStatus;
	
    /* Keep the interrupt source */
	u32GpaStatus = GPIOA->ISRC;
	u32GpbStatus = GPIOB->ISRC;

    /* Clear the interrupt */
    GPIOA->ISRC = u32GpaStatus;
    GPIOB->ISRC = u32GpbStatus;

    /* Call the callback function of GPIOAB interrupt */
    if(_pfGPABCallback)
        _pfGPABCallback(u32GpaStatus, u32GpbStatus);    

}

/*---------------------------------------------------------------------------------------------------------*/
/* GPIO C,D,E  ISR                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void GPCDE_IRQHandler(void)
{
	uint32_t u32GpcStatus, u32GpdStatus, u32GpeStatus;
	
    /* Keep the interrupt source */
	u32GpcStatus = GPIOC->ISRC;
	u32GpdStatus = GPIOD->ISRC;
	u32GpeStatus = GPIOE->ISRC;

    /* Clear the interrupt */
    GPIOC->ISRC = u32GpcStatus;
    GPIOD->ISRC = u32GpdStatus;
    GPIOE->ISRC = u32GpeStatus;

    /* Call the callback function of GPIOAB interrupt */
    if(_pfGPCDECallback)
        _pfGPCDECallback(u32GpcStatus, u32GpdStatus, u32GpeStatus);    

}

/*---------------------------------------------------------------------------------------------------------*/
/* External INT0 & INT1  ISR                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void EINT0_IRQHandler(void)
{
    /* EINT0 = GPB14. Clear the interrupt */
  	GPIOB->ISRC  = 1UL << 14;
	if(_pfEINT0Callback)
        _pfEINT0Callback();
}

void EINT1_IRQHandler(void)
{
    /* EINT0 = GPB15. Clear the interrupt */
  	GPIOB->ISRC  = 1UL << 15;
	if(_pfEINT1Callback)
        _pfEINT1Callback();
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SetCallbackFunctions                                                              */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             pfGPABCallback  - [in] The callback function of GPA and GPB interrupts.                   */
/*	             pfGPCDECallback - [in] The callback function of GPC, GPD and GPE interrupts.              */
/*	             pfEINT0Callback - [in] The callback function of EINT0 interrupts.                         */
/*	             pfEINT1Callback - [in] The callback function of EINT1 interrupts.                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               To install GPIO interrrupt callback functions                                             */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SetIntCallback(
    GPIO_GPAB_CALLBACK pfGPABCallback, 
    GPIO_GPCDE_CALLBACK pfGPCDECallback 
)
{
    _pfGPABCallback  = (void (*)(uint32_t, uint32_t))pfGPABCallback;
    _pfGPCDECallback = (void (*)(uint32_t, uint32_t, uint32_t))pfGPCDECallback;    
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_InitFunction                                                                      */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             function   -[in] Specified GPIO port. It could be                                         */
/*          					  FUNC_GPIO, FUNC_PWMT, FUNC_I2C0, FUNC_I2C1, FUNC_ADC,                    */
/*          					  FUNC_EXTINT, FUNC_CPO, FUNC_TMR0, FUNC_TMR1, FUNC_TMR2,                  */
/*          					  FUNC_TMR3, FUNC_UART0, FUNC_UART1, FUNC_COMP0,                           */
/*          					  FUNC_COMP1, FUNC_CAN0, FUNC_CAN1, FUNC_SPI0,FUNC_SPI1,                   */
/*          					  FUNC_SPI2, FUNC_SPI3.                                                    */
/*	             option  	-[in] Select the different pin to output the specified function                */
/*						          Function    |      IO                                                    */
/*						      ------------------------------------------                                   */
/*						          FUNC_GPIO   |      |   All GPIO                                          */
/*						          FUNC_PWMT   |      |   GPA12 ~ GPA15                                     */
/*						          FUNC_I2C0   |      |   GPA8  , GPA9                                      */
/*						          FUNC_I2C1   |      |   GPA10 , GPA11                                     */
/*						          FUNC_ADC    |      |   GPA0  ~ GPA7                                      */
/*						          FUNC_EXTINT |      |   GPB14 , GPB15                                     */
/*						          FUNC_CPO    |      |   GPB12 , GPB13                                     */
/*						          FUNC_TMR0   |      |   GPB8                                              */
/*						          FUNC_TMR1   |      |   GPB9                                              */
/*						          FUNC_TMR2   |      |   GPB10                                             */
/*						          FUNC_TMR3   |      |   GPB11                                             */
/*						          FUNC_UART0  |      |   GPB0  ~ GPB3                                      */
/*						          FUNC_UART1  |      |   GPB4  ~ GPB7                                      */
/*						          FUNC_COMP0  |      |   GPC6  , GPC7                                      */
/*						          FUNC_COMP1  |      |   GPC14 ~ GPC15                                     */
/*						          FUNC_CAN0   |      |   GPD6  , GPD7                                      */
/*						          FUNC_CAN1   |      |   GPD14 , GPD15                                     */            
/*						          FUNC_SPI0   |      |   GPC0  ~ GPC5                                      */
/*						          FUNC_SPI1   |      |   GPC8  ~ GPC13                                     */
/*						          FUNC_SPI2   |      |   GPD0  ~ GPD5                                      */
/*						          FUNC_SPI3   |      |   GPD8  ~ GPD13                                     */
/* Returns:                                                                                                */
/*               E_SUCCESS                                                                                 */
/*               E_DRVGPIO_ARGUMENT  Argument error                                                        */
/* Description:                                                                                            */
/*               To initialize the multi-function pin's of the specified function.                                             */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_InitFunction(DRVGPIO_FUNC function)
{
  
	switch(function)
	{
		/*---------------------------------------------------------------------------------------------------------*/
		/* GPIO                                                                                                    */
		/*---------------------------------------------------------------------------------------------------------*/
		case FUNC_GPIO:
		{
		  	outpw(&SYS->GPAMFP,0);
			outpw(&SYS->GPBMFP,0);
			outpw(&SYS->GPCMFP,0);
			outpw(&SYS->GPDMFP,0);
			outpw(&SYS->GPEMFP,0);
		  	
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* PWM                                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case FUNC_PWMT:
		{
		  	SYS->GPAMFP.PWM0 		=1;
		  	SYS->GPAMFP.PWM1 		=1;
		  	SYS->GPAMFP.PWM2 		=1;
		  	SYS->GPAMFP.PWM3 		=1;
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* I2C                                                                                                    */
		/*---------------------------------------------------------------------------------------------------------*/
		case FUNC_I2C0:
		{
		  	SYS->GPAMFP.I2C0_SDA 	=1;
			SYS->GPAMFP.I2C0_SCL 	=1;
		}break;		

		/*---------------------------------------------------------------------------------------------------------*/
		/* I2C1                                                                                                    */
		/*---------------------------------------------------------------------------------------------------------*/
		case FUNC_I2C1:
		{
		  	SYS->GPAMFP.I2C1_SDA 	=1;
			SYS->GPAMFP.I2C1_SCL 	=1;
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* ADC                                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case FUNC_ADC0:
		{
			SYS->GPAMFP.ADC0 		=1;
		}break;

		case FUNC_ADC1:
		{
			SYS->GPAMFP.ADC1 		=1;
		}break;
		
		case FUNC_ADC2:
		{
			SYS->GPAMFP.ADC2 		=1;
		}break;

		case FUNC_ADC3:
		{
			SYS->GPAMFP.ADC3 		=1;
		}break;

		case FUNC_ADC4:
		{
			SYS->GPAMFP.ADC4 		=1;
		}break;

		case FUNC_ADC5:
		{
			SYS->GPAMFP.ADC5 		=1;
		}break;

		case FUNC_ADC6:
		{
			SYS->GPAMFP.ADC6 		=1;
		}break;

		case FUNC_ADC7:
		{
			SYS->GPAMFP.ADC7 		=1;
		}break;

  	
	  	case FUNC_EXTINT:
		{	
			SYS->GPBMFP.INT0 		=1;
			SYS->GPBMFP.INT1 		=1;
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* Analog Comparator-1 digital output                                                                      */
		/*---------------------------------------------------------------------------------------------------------*/
		case FUNC_CPO:
		{
			SYS->GPBMFP.CPO0 		=1;
			SYS->GPBMFP.CPO1 		=1;
		}break;


		/*---------------------------------------------------------------------------------------------------------*/
		/* TIMER                                                                                                   */
		/*---------------------------------------------------------------------------------------------------------*/
		case FUNC_TMR0:
		{
			SYS->GPBMFP.TM0 		=1;
		}break;
	
		case FUNC_TMR1:
		{
			SYS->GPBMFP.TM1 		=1;
		}break;

		case FUNC_TMR2:
		{
			SYS->GPBMFP.TM2 		=1;	
		}break;

		case FUNC_TMR3:
		{
			SYS->GPBMFP.TM3 		=1;	
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* UART                                                                                                    */
		/*---------------------------------------------------------------------------------------------------------*/
		case FUNC_UART0:
		{
			
			SYS->GPBMFP.UART0_RX 	=1;
			SYS->GPBMFP.UART0_TX 	=1;
			SYS->GPBMFP.UART0_nRTS 	=1;
			SYS->GPBMFP.UART0_nCTS 	=1;
		}break;

		case FUNC_UART1:
		{
			SYS->GPBMFP.UART1_RX 	=1;
			SYS->GPBMFP.UART1_TX 	=1;
			SYS->GPBMFP.UART1_nRTS 	=1;
			SYS->GPBMFP.UART1_nCTS 	=1;
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* Comparator                                                                                              */
		/*---------------------------------------------------------------------------------------------------------*/
		case FUNC_COMP0:	
		{
			SYS->GPCMFP.CPN0 =1;
			SYS->GPCMFP.CPP0 =1;
		}break;

		case FUNC_COMP1:
		{
			SYS->GPCMFP.CPN1 =1;
			SYS->GPCMFP.CPP1 =1;
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* CAN                                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case FUNC_CAN0:	
		{
			SYS->GPDMFP.CAN0_TX =1;
			SYS->GPDMFP.CAN0_RX =1;
		}break;	

		case FUNC_CAN1:	
		{
			SYS->GPDMFP.CAN1_TX =1;
			SYS->GPDMFP.CAN1_RX =1;
		}break;	

		/*---------------------------------------------------------------------------------------------------------*/
		/* SPI                                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case FUNC_SPI0:	
		{
			SYS->GPCMFP.SPI0_SS0 	=1;
			SYS->GPCMFP.SPI0_CLK 	=1;
			SYS->GPCMFP.SPI0_MISO0 	=1;
			SYS->GPCMFP.SPI0_MOSI0 	=1;
			SYS->GPCMFP.SPI0_MISO1 	=1;
			SYS->GPCMFP.SPI0_MOSI1 	=1;
		}break;

		case FUNC_SPI1:
		{
			SYS->GPCMFP.SPI1_SS0 	=1;
			SYS->GPCMFP.SPI1_CLK 	=1;
			SYS->GPCMFP.SPI1_MISO0	=1;
			SYS->GPCMFP.SPI1_MOSI0 	=1;
			SYS->GPCMFP.SPI1_MISO1 	=1;
			SYS->GPCMFP.SPI1_MOSI1 	=1;
		}break;

		case FUNC_SPI2:	
		{
			/* SPI2 */
			SYS->GPDMFP.SPI2_SS0 	=1;
			SYS->GPDMFP.SPI2_CLK 	=1;
			SYS->GPDMFP.SPI2_MISO0 	=1;
			SYS->GPDMFP.SPI2_MOSI0 	=1;
			SYS->GPDMFP.SPI2_MISO1 	=1;
			SYS->GPDMFP.SPI2_MOSI1 	=1;
		}break;				

		case FUNC_SPI3:	
		{
			/* SPI3 */
			SYS->GPDMFP.SPI3_SS0 	=1;
			SYS->GPDMFP.SPI3_CLK 	=1;
			SYS->GPDMFP.SPI3_MISO0 	=1;
			SYS->GPDMFP.SPI3_MOSI0 	=1;
			SYS->GPDMFP.SPI3_MISO1 	=1;
			SYS->GPDMFP.SPI3_MOSI1 	=1;
		}break;
		
		default:
			return E_DRVGPIO_ARGUMENT;
	}

	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* FUNCTION                                                                                                */
/*      DrvGPIO_Close()		 		                                                                       */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*      To close the opened IO and reset its configurations                                                */
/*                                                                                                         */
/* INPUTS                                                                                                  */
/*      port                                                                                               */
/*          Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE                                    */
/*      i32Bit                                                                                             */
/*          Specified bit of the IO port. It could be 0~15                                                 */
/*                                                                                                         */
/* OUTPUTS                                                                                                 */
/*      None                            				                                                   */
/*                                                                                                         */
/* RETURN                                                                                                  */
/*      E_SUCCESS   		                                                                               */
/*      E_DRVGPIO_ARGUMENT  Wrong arguments                                                                */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_Close(DRVGPIO_PORT port,int32_t i32Bit)
{
    if((i32Bit < 0) || (i32Bit > 16))
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, (inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) & (~(3 << (i32Bit*2)))));
	
	GPIO_DBNCECON->ICLK_ON = 0; 

    return E_SUCCESS;
}	

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_Open                                                                              */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE              */
/*	             i32Bit  	-[in] Specified bit of the IO port. It could be 0~15                           */
/*	             mode    	-[in] Set the IO to be IO_INPUT , IO_OUTPUT ,IO_OPENDRAIN or IO_QUASI          */
/* Returns:                                                                                                */
/*               E_SUCCESS           Success                                                               */
/*               E_DRVGPIO_ARGUMENT  Wrong arguments                                                       */
/*               E_DRVGPIO_BUSY      The IO has been used                                                  */
/* Description:                                                                                            */
/*               To configure the specified GPIO to use it.                                                */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvGPIO_Open(DRVGPIO_PORT port,int32_t i32Bit,DRVGPIO_IO mode)
{
    if(port > 4)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    if((i32Bit < 0) || (i32Bit > 16))
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    
    if(mode == IO_INPUT)
    {
        outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, (inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) & (~(3 << (i32Bit * 2)))));
    }
    else if(mode == IO_OUTPUT)
    {
		outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, (inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) & (~(3 << (i32Bit * 2)))));
        outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) | (1 << (i32Bit * 2)));
    }
    else if(mode == IO_OPENDRAIN)
    {
		outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, (inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) & (~(3 << (i32Bit * 2)))));
        outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) | (2 << (i32Bit * 2)));
    }
	else if(mode == IO_QUASI)
    {
		outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, (inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) & (~(3 << (i32Bit * 2)))));
        outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) | (3 << (i32Bit * 2)));
    }
	else
    {
        return E_DRVGPIO_ARGUMENT;
    }
        
	return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SetPortBits                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE              */
/*	             i32Data  	-[in] The data to write to the specified IO port.                              */
/* Returns:                                                                                                */
/*               E_SUCCESS        			Success                                                        */
/*               E_DRVGPIO_ARGUMENT        	Wrong arguments                                                */
/* Description:                                                                                            */
/*               The function is used to read flag to judge GPIO bit is used or not.                       */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvGPIO_SetPortBits(DRVGPIO_PORT port,int32_t i32Data)
{
    if(port > 4)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    if(port == GPA)
		GPIOA->DOUT =	i32Data;
	else if(port == GPB)
		GPIOB->DOUT |=	i32Data;
	else if(port == GPC)
		GPIOC->DOUT |=	i32Data;
	else if(port == GPD)
		GPIOD->DOUT |=	i32Data;
	else if(port == GPE)
		GPIOE->DOUT |=	i32Data;
	else
		return E_DRVGPIO_ARGUMENT;	

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetPortBits                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE              */
/* Returns:                                                                                                */
/*               The IO pin value of the specified IO port                                                 */
/* Description:                                                                                            */
/*               The function is used to Get the data of the specified IO port.                            */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetPortBits(DRVGPIO_PORT port)
{
 
    uint32_t u32PortAddr;
    
    if(port > 4)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    u32PortAddr = (uint32_t)&GPIOA->PIN + port * PORT_OFFSET;
   	
    return inpw(u32PortAddr);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetBit                                                                            */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE              */
/*	             i32Bit  	-[in] Specified bit of the IO port. It could be 0~15                           */
/* Returns:                                                                                                */
/*               The bit value of the specified IO bit                                                     */
/* Description:                                                                                            */
/*               The function is used to Get the value of the specified IO bit .                           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetBit(DRVGPIO_PORT port, int32_t i32Bit)
{
    uint32_t u32PortAddr;
    
    if(port > 4)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    if((i32Bit < 0) || (i32Bit > 16))
    {
        return 0;
    }

    u32PortAddr = (uint32_t)&GPIOA->PIN + port * PORT_OFFSET;
   	   

    return ((inpw(u32PortAddr) >> i32Bit) & 1);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SetBit                                                                            */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE              */
/*	             i32Bit  	-[in] Specified bit of the IO port. It could be 0~15                           */
/* Returns:                                                                                                */
/*               E_SUCCESS           Success                                                               */
/*               E_DRVGPIO_ARGUMENT  Wrong arguments                                                       */
/* Description:                                                                                            */
/*               The function is used to Set the specified IO bit to 1  .                                  */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvGPIO_SetBit(DRVGPIO_PORT port,int32_t i32Bit)
{
  if(port > 4)
    {
        return E_DRVGPIO_ARGUMENT;
    }

    if((i32Bit < 0) || (i32Bit > 16))
    {
        return E_DRVGPIO_ARGUMENT;
    }

    if(port == GPA)
		GPIOA->DOUT |=	1 << i32Bit;
	else if(port == GPB)
		GPIOB->DOUT |=	1 << i32Bit;
	else if(port == GPC)
		GPIOC->DOUT |=	1 << i32Bit;
	else if(port == GPD)
		GPIOD->DOUT |=	1 << i32Bit;
	else if(port == GPE)
		GPIOE->DOUT |=	1 << i32Bit;
	else
		return E_DRVGPIO_ARGUMENT;	

	return E_SUCCESS;

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_ClrBit                                                                            */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE              */
/*	             i32Bit  	-[in] Specified bit of the IO port. It could be 0~15                           */
/* Returns:                                                                                                */
/*               E_SUCCESS           Success                                                               */
/*               E_DRVGPIO_ARGUMENT  Wrong arguments                                                       */
/* Description:                                                                                            */
/*               The function is used to  Clear the specified IO bit to 0                                  */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvGPIO_ClrBit(DRVGPIO_PORT port,int32_t i32Bit)
{
    uint32_t u32PortAddr;
    
    if(port > 4)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    if((i32Bit < 0) || (i32Bit > 16))
    {
        return E_DRVGPIO_ARGUMENT;
    }

	u32PortAddr = (uint32_t)&GPIOA->DOUT  + port * PORT_OFFSET;
   	
	outpw(u32PortAddr, inpw(u32PortAddr) & (~(1 << i32Bit)));
   
    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetDoutBit                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE              */
/*	             i32Bit  	-[in] Specified bit of the IO port. It could be 0~15                           */
/* Returns:                                                                                                */
/*               The bit value of the specified IO bit                                                     */
/* Description:                                                                                            */
/*               The function is used to Get the value of the specified IO bit from GPIO Dout register.    */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvGPIO_GetDoutBit(DRVGPIO_PORT port, int32_t i32Bit)
{
	if(port > 4)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    if((i32Bit < 0) || (i32Bit >= 15))
    {
        return E_DRVGPIO_ARGUMENT;
    }
 
    return ((inpw((uint32_t)&GPIOA->DOUT + port * PORT_OFFSET) >> i32Bit) & 1);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetPortDoutBits                                                                   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE              */
/* Returns:                                                                                                */
/*               The value of the GPIO DOUT register value.                                                */
/* Description:                                                                                            */
/*               The function is used to Get the Dout register value of the specified IO port.    		   */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvGPIO_GetPortDoutBits(DRVGPIO_PORT port)
{
    if(port > 4)
    {
        return E_DRVGPIO_ARGUMENT;
    }

    return inpw((uint32_t)&GPIOA->DOUT + port *PORT_OFFSET);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SetDebounceTime                                                                   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             i32DebounceClk    	-[in] The debounce timing is 2^(i32DebounceClk) * source clock         */
/*	             i8ClockSource    	-[in] The debounce clock source can be DBCLKSRC_HCLK or DBCLKSRC_10K   */
/* Returns:                                                                                                */
/*               E_SUCCESS                                                                                 */
/*               E_DRVGPIO_ARGUMENT  Wrong arguments                                                       */
/* Description:                                                                                            */
/*               The function is used to To set the debounce timing and select source.     		           */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvGPIO_SetDebounceTime(uint32_t u32DebounceClk,DRVGPIO_DBCLKSRC clockSource)
{

    /* Maximum debounce time is 2^(15 + 1) * clk src */
    if(u32DebounceClk > 15)
    {
        u32DebounceClk = 15;
    }
    
	GPIO_DBNCECON->DBCLKSEL = u32DebounceClk ; 

	GPIO_DBNCECON->DBCLKSRC = clockSource ; 

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetDebounceTime                                                                   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               The debounce time setting.                                                                */
/* Description:                                                                                            */
/*               The function is used to To Get the debounce timing setting.            		           */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvGPIO_GetDebounceTime(void)
{
    volatile int32_t i32Reg;

    i32Reg = 	GPIO_DBNCECON->DBCLKSEL;
   
    return i32Reg;
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_EnableDebounce                                                                    */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE              */
/*	             u32Bit  	-[in] Specified bit of the IO port. It could be 0~15                           */
/* Returns:                                                                                                */
/*               E_SUCCESS   		 Success .                                                             */
/*               E_DRVGPIO_ARGUMENT  Wrong arguments .                                                     */
/* Description:                                                                                            */
/*               To enable the debounce function of the specified GPIO bit.            	     	           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_EnableDebounce(DRVGPIO_PORT port,uint32_t u32Bit)
{
    if(u32Bit > 15)
        return E_DRVGPIO_ARGUMENT;

    outpw((uint32_t)&GPIOA->DBEN + port * PORT_OFFSET, inpw((uint32_t)&GPIOA->DBEN + port * PORT_OFFSET) | (1 << u32Bit));

	//GPIO_DBNCECON->ICLK_ON = 1;

    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_DisableDebounce                                                                   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE              */
/*	             i32Bit  	-[in] Specified bit of the IO port. It could be 0~15                           */
/* Returns:                                                                                                */
/*               E_SUCCESS   		 Success .                                                             */
/*               E_DRVGPIO_ARGUMENT  Wrong arguments .                                                     */
/* Description:                                                                                            */
/*               To disable the debounce function of the specified GPIO bit.            	     	       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_DisableDebounce(DRVGPIO_PORT port, uint32_t u32Bit)
{
    if(port > GPE)
        return E_DRVGPIO_ARGUMENT;

    outpw((uint32_t)&GPIOA->DBEN + port * PORT_OFFSET , inpw((uint32_t)&GPIOA->DBEN + port * PORT_OFFSET) &(~(1<<u32Bit)));

    
	return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SetPortMask                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE              */
/*	             mask    	-[in] The data to mask to the specified IO port.                               */
/* Returns:                                                                                                */
/*               E_SUCCESS   		 Success .                                                             */
/*               E_DRVGPIO_ARGUMENT  Wrong arguments .                                                     */
/* Description:                                                                                            */
/*               To set GPIO port mask register.                                        	     	       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetPortMask(DRVGPIO_PORT port, uint32_t u32Mask)
{
    if(port > GPE)
		return E_DRVGPIO_ARGUMENT;	

    outpw((uint32_t)&GPIOA->DMASK + port * PORT_OFFSET, u32Mask);

    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SetBitMask                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE              */
/*	             i32Bit    	-[in] Specified bit of the IO port. It could be 0~15 .                         */
/* Returns:                                                                                                */
/*               E_SUCCESS   		 Success .                                                             */
/*               E_DRVGPIO_ARGUMENT  Wrong arguments .                                                     */
/* Description:                                                                                            */
/*               To set bits of GPIO port mask                                           	     	       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetBitMask(DRVGPIO_PORT port, int32_t i32Bit)
{

	outpw((uint32_t)&GPIOA->DMASK+ port*PORT_OFFSET , inpw((uint32_t)&GPIOA->DMASK+ port*PORT_OFFSET) | (1<<i32Bit));

	return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_ClrBitMask                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE              */
/*	             i32Bit    	-[in] Specified bit of the IO port. It could be 0~15 .                         */
/* Returns:                                                                                                */
/*               E_SUCCESS   		 Success .                                                             */
/* Description:                                                                                            */
/*               To clear bits of GPIO port mask	                                       	     	       */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvGPIO_ClrBitMask(DRVGPIO_PORT port, uint32_t u32Bit)
{

	outpw((uint32_t)&GPIOA->DMASK+ port*PORT_OFFSET,inpw((uint32_t)&GPIOA->DMASK+ port*PORT_OFFSET) &(~(1<<u32Bit)));
	
	return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_ReadPortMask                                                                      */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE              */
/* Returns:                                                                                                */
/*               the value of Specified GPIO port mask register .                                          */
/* Description:                                                                                            */
/*               To get current GPIO port mask setting	                                  	     	       */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvGPIO_ReadPortMask(DRVGPIO_PORT port)
{
	return inpw((uint32_t)&GPIOA->DMASK + port * PORT_OFFSET); 
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_EnableInt                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    		   -[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE       */
/*	             u32Bit    	       -[in] Specified bit of the IO port. It could be 0~15.                   */
/*	             DRVGPIO_INT_TYPE  -[in] Specified bit of the IO port. Specified INT type.                 */
/*                                       It could be IO_RISING, IO_FALLING or IO_BOTH_EDGE                 */
/*	             DRVGPIO_INT_MODE  -[in] Specified INT mode.                                               */
/*                                       It could be MODE_EDGE, or MODE_LEVEL                              */
/* Returns:                                                                                                */
/*               E_SUCCESS           Success                                                               */
/*               E_DRVGPIO_ARGUMENT  Wrong arguments                                                       */
/* Description:                                                                                            */
/*               To set one bit of GPIO port to Interrupt type	                                  	       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_EnableInt(DRVGPIO_PORT port, uint32_t u32Bit, DRVGPIO_INT_TYPE tiggerType, DRVGPIO_INT_MODE mode)
{
    if((port == GPB) && ((u32Bit == 14) || (u32Bit == 14)))
    {
        /* GPB14 is EINT0, GPB15 is EINT1. Both of them can't be assigned to GPAB interrupt */
        return E_DRVGPIO_ARGUMENT;    
    }
    
    /* Configure the interrupt to be rising/falling when edge trigger or high/low level when level trigger */
	if(tiggerType ==IO_RISING)
		outpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET , inpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET) | (1<<(u32Bit+16)));	 

	else if(tiggerType == IO_FALLING)
		outpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET , inpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET) | (1<<u32Bit));	 

	else if(tiggerType == IO_BOTH_EDGE)  
		outpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET , inpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET) | (1<<u32Bit)| (1<<(u32Bit+16)));	 
	else
		return E_DRVGPIO_ARGUMENT;

    /* Configure to be level trigger or edge trigger */
	if(mode ==MODE_EDGE)
		outpw((uint32_t)&GPIOA->IMD + port*PORT_OFFSET,inpw((uint32_t)&GPIOA->IMD + port*PORT_OFFSET)&(~(1<<u32Bit)));

	else if(mode ==MODE_LEVEL)
		outpw((uint32_t)&GPIOA->IMD + port*PORT_OFFSET,inpw((uint32_t)&GPIOA->IMD + port*PORT_OFFSET)|(1<<u32Bit));

	else
		return E_DRVGPIO_ARGUMENT;
    
    /* Enable the relative interrupt of M0 */
    if(port == GPA || port == GPB)
    {
        NVIC_SetPriority (GPAB_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(GPAB_IRQn);
    }

    if((port == GPC) || (port == GPD) || (port == GPE))
    {
        NVIC_SetPriority (GPCDE_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(GPCDE_IRQn);
    }    
    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_DisableInt                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    		   -[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE       */
/*	             i16Bit    	       -[in] Specified bit of the IO port. It could be 0~15 .                  */
/*	             DRVGPIO_INT_TYPE  -[in] Specified bit of the IO port. Specified INT type.                 */
/*                                       It could be IO_RISING, IO_FALLING and IO_BOTH_EDGE                */
/* Returns:                                                                                                */
/*               E_SUCCESS           Success                                                               */
/*               E_DRVGPIO_ARGUMENT  Wrong arguments                                                       */
/* Description:                                                                                            */
/*               To disable the interrupt of specified GPIO bit.                                 	       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_DisableInt(DRVGPIO_PORT port,uint32_t u32Bit)
{
    uint32_t u32Reg;

    if(u32Bit > 31)
        return E_DRVGPIO_ARGUMENT;
    
    /* Disable both rising/falling or both high level/low level interrupts */
    u32Reg = (1 << u32Bit) | (1 << (u32Bit + 16));
    outpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET , inpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET) & (~u32Reg));	 

    return E_SUCCESS;
}


void DrvGPIO_EnableEINT0(DRVGPIO_INT_TYPE tiggerType, DRVGPIO_INT_MODE mode, GPIO_EINT0_CALLBACK pfEINT0Callback)
{
    uint32_t u32Reg;


    u32Reg = inpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET);
	if(tiggerType ==IO_RISING)
    {
        u32Reg = u32Reg | (1 << (14 + 16));    
	}
    else if(tiggerType == IO_FALLING)
	{
        u32Reg = u32Reg | (1 << 14);    
	}
    else if(tiggerType == IO_BOTH_EDGE)
    {  
        u32Reg = u32Reg | (1 << 14) | (1 << (14 + 16));    
	}
    
    outpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET , u32Reg);	 

    _pfEINT0Callback = pfEINT0Callback;
    NVIC_EnableIRQ(EINT0_IRQn);
}

void DrvGPIO_DisableEINT0(void)
{
    uint32_t u32Reg;
    u32Reg = inpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET);
    u32Reg = u32Reg & (~((1UL << 14) | (1UL << (14 + 16))));
    outpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET , u32Reg);	 
    NVIC_DisableIRQ(EINT0_IRQn);
}


void DrvGPIO_EnableEINT1(DRVGPIO_INT_TYPE tiggerType, DRVGPIO_INT_MODE mode, GPIO_EINT1_CALLBACK pfEINT1Callback)
{
    uint32_t u32Reg;

    u32Reg = inpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET);
	if(tiggerType ==IO_RISING)
    {
        u32Reg = u32Reg | (1UL << (15 + 16));    
	}
    else if(tiggerType == IO_FALLING)
	{
        u32Reg = u32Reg | (1UL << 15);    
	}
    else if(tiggerType == IO_BOTH_EDGE)
    {  
        u32Reg = u32Reg | (1UL << 15) | (1UL << (15 + 16));    
	}
    
    outpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET , u32Reg);	 
    _pfEINT1Callback = pfEINT1Callback;
    NVIC_EnableIRQ(EINT1_IRQn);
}

void DrvGPIO_DisableEINT1(void)
{
    uint32_t u32Reg;
    u32Reg = inpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET);
    u32Reg = u32Reg & (~((1UL << 15) | (1UL << (15 + 16))));
    outpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET , u32Reg);	 
    NVIC_DisableIRQ(EINT1_IRQn);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetIntStatus                                                                      */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    		   -[in] Specified GPIO port. It could be GPA, GPB , GPC , GPD , GPE       */
/* Returns:                                                                                                */
/*               A value to the GPIO interrupt status register                                             */
/* Description:                                                                                            */
/*               This function is used to return a value to the GPIO interrupt status register.    	       */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvGPIO_GetIntStatus(DRVGPIO_PORT port)
{
    uint32_t u32Reg;
	
	if(port == GPA)
		u32Reg = GPIOA->ISRC;
	else if(port == GPB)
		u32Reg = GPIOB->ISRC;
	else if(port == GPC)
		u32Reg = GPIOC->ISRC;
	else if(port == GPD)
		u32Reg = GPIOD->ISRC;
	else if(port == GPE)
		u32Reg = GPIOE->ISRC;
	else
		return E_DRVGPIO_ARGUMENT;
	return u32Reg;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetVersion                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               This function is used to return the version number of GPIO driver.    	                   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetVersion (void)
{
   return DRVGPIO_VERSION_NUM;
}


