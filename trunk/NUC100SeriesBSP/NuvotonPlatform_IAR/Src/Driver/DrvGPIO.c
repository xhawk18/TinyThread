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
static void (*_pfGPABCallback)(uint32_t u32GPAStatus, uint32_t u32GPBStatus);
static void (*_pfGPCDECallback)(uint32_t u32GPCStatus, uint32_t u32GPDStatus, uint32_t u32GPEStatus);
static void (*_pfEINT0Callback)(void);
static void (*_pfEINT1Callback)(void);              		 

/*---------------------------------------------------------------------------------------------------------*/
/* GPIO A and GPIO B  ISR                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void GPAB_IRQHandler(void)
{
	uint32_t u32GPAStatus, u32GPBStatus;
	
    /* Keep the interrupt source */
	u32GPAStatus = GPIOA->ISRC;
	u32GPBStatus = GPIOB->ISRC;

    /* Clear the interrupt */
    GPIOA->ISRC = u32GPAStatus;
    GPIOB->ISRC = u32GPBStatus;

    /* Call the callback function of GPIOAB interrupt */
    if ( _pfGPABCallback )
        _pfGPABCallback(u32GPAStatus, u32GPBStatus);    
}

/*---------------------------------------------------------------------------------------------------------*/
/* GPIO C,D,E  ISR                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void GPCDE_IRQHandler(void)
{
	uint32_t u32GPCStatus, u32GPDStatus, u32GPEStatus;
	
    /* Keep the interrupt source */
	u32GPCStatus = GPIOC->ISRC;
	u32GPDStatus = GPIOD->ISRC;
	u32GPEStatus = GPIOE->ISRC;

    /* Clear the interrupt */
    GPIOC->ISRC = u32GPCStatus;
    GPIOD->ISRC = u32GPDStatus;
    GPIOE->ISRC = u32GPEStatus;

    /* Call the callback function of GPIOAB interrupt */
    if ( _pfGPCDECallback )
        _pfGPCDECallback(u32GPCStatus, u32GPDStatus, u32GPEStatus);    
}

/*---------------------------------------------------------------------------------------------------------*/
/* External INT0 ISR                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void EINT0_IRQHandler(void)
{
    /* EINT0 = GPB14. Clear the interrupt */
  	GPIOB->ISRC  = 1UL << 14;
	if ( _pfEINT0Callback )
        _pfEINT0Callback();
}

/*---------------------------------------------------------------------------------------------------------*/
/* External INT1 ISR                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void EINT1_IRQHandler(void)
{
    /* EINT0 = GPB15. Clear the interrupt */
  	GPIOB->ISRC  = 1UL << 15;
	if ( _pfEINT1Callback )
        _pfEINT1Callback();
}






/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_Open                                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32Bit - [in]                                                                              */
/*                  Specify pin of the GPIO port. It could be 0~15.                                        */
/*	            mode - [in]                                                                                */
/*                  E_DRVGPIO_IO, set the specified GPIO pin to be E_IO_INPUT, E_IO_OUTPUT,                */
/*                  E_IO_OPENDRAIN or E_IO_QUASI mode.                                                     */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*              Set the specified GPIO pin to the specified GPIO operation mode.	                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_Open(E_DRVGPIO_PORT port, int32_t i32Bit, E_DRVGPIO_IO mode)
{
    volatile uint32_t u32Reg;
    
    if ((i32Bit < 0) || (i32Bit > 16))
    {
        return E_DRVGPIO_ARGUMENT;
    }    

    u32Reg = (uint32_t)&GPIOA->PMD + (port*PORT_OFFSET);    
    if ((mode == E_IO_INPUT) || (mode == E_IO_OUTPUT) || (mode == E_IO_OPENDRAIN))
    {
        outpw(u32Reg, inpw(u32Reg) & ~(0x3<<(i32Bit*2)));
        if (mode == E_IO_OUTPUT)
        {
            outpw(u32Reg, inpw(u32Reg) | (0x1<<(i32Bit*2)));
        }else
        if (mode == E_IO_OPENDRAIN) 
        {
            outpw(u32Reg, inpw(u32Reg) | (0x2<<(i32Bit*2)));
        }
    }else
	if (mode == E_IO_QUASI)
    {
        outpw(u32Reg, inpw(u32Reg) | (0x3<<(i32Bit*2)));
    }else
    {
        return E_DRVGPIO_ARGUMENT;
    }
        
	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_Close                                                                              */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32Bit - [in]                                                                              */
/*                  Specify pin of the GPIO port. It could be 0~15.                                        */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*              Close the specified GPIO pin function and set the pin to quasi-bidirectional mode.         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_Close(E_DRVGPIO_PORT port, int32_t i32Bit)
{
    volatile uint32_t u32Reg;

    if ((i32Bit < 0) || (i32Bit > 16))
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    u32Reg = (uint32_t)&GPIOA->PMD + (port*PORT_OFFSET);    
    outpw(u32Reg, inpw(u32Reg) | (0x3<<(i32Bit*2)));
	
	GPIO_DBNCECON->ICLK_ON = 0; 

    return E_SUCCESS;
}	

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_SetBit                                                                             */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32Bit - [in]                                                                              */
/*                  Specify pin of the GPIO port. It could be 0~15.                                        */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*              Set the specified GPIO pin to 1.                                                      	   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetBit(E_DRVGPIO_PORT port, int32_t i32Bit)
{
	GPIO_T * tGPIO;

    if ((i32Bit < 0) || (i32Bit > 16))
    {
        return E_DRVGPIO_ARGUMENT;
    }

	tGPIO = (GPIO_T *)((uint32_t)GPIOA + (port*PORT_OFFSET));  

	tGPIO->DOUT |= (1 << i32Bit);

	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetBit                                                                             */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32Bit - [in]                                                                              */
/*                  Specify pin of the GPIO port. It could be 0~15.                                        */
/* Returns:                                                                                                */
/*              The specified input pin value					0 / 1                                      */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*              Get the pin value from the specified input GPIO pin.            			               */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetBit(E_DRVGPIO_PORT port, int32_t i32Bit)
{
    volatile uint32_t u32Reg;
        
    if ((i32Bit < 0) || (i32Bit > 16))
    {
        return E_DRVGPIO_ARGUMENT;
    }

    u32Reg = (uint32_t)&GPIOA->PIN + (port*PORT_OFFSET);    

    return ((inpw(u32Reg)>>i32Bit) & 0x1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_ClrBit                                                                             */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32Bit - [in]                                                                              */
/*                  Specify pin of the GPIO port. It could be 0~15.                                        */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*              Set the specified GPIO pin to 0.               			                                   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_ClrBit(E_DRVGPIO_PORT port, int32_t i32Bit)
{
	GPIO_T * tGPIO;

    if ((i32Bit < 0) || (i32Bit > 16))
    {
        return E_DRVGPIO_ARGUMENT;
    }

	tGPIO = (GPIO_T *)((uint32_t)GPIOA + (port*PORT_OFFSET));  

	tGPIO->DOUT &= ~(1 << i32Bit);

	return E_SUCCESS;    
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_SetPortBits                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32Data - [in]                                                                             */
/*                  The data output value. It could be 0~0xFFFF                         				   */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*              Set the output port value to the specified GPIO port.             	                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetPortBits(E_DRVGPIO_PORT port,int32_t i32Data)
{
    GPIO_T * tGPIO;

	tGPIO = (GPIO_T *)((uint32_t)GPIOA + (port*PORT_OFFSET));  

	tGPIO->DOUT = i32Data;

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetPortBits                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/* Returns:                                                                                                */
/*              The specified input port value            		0 ~ 0xFFFF                                 */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*             Get the input port value from the specified GPIO port.                                      */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetPortBits(E_DRVGPIO_PORT port)
{
	GPIO_T * tGPIO;

	tGPIO = (GPIO_T *)((uint32_t)GPIOA + (port*PORT_OFFSET));  

    return tGPIO->PIN;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetDoutBit                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32Bit - [in]                                                                              */
/*                  Specify pin of the GPIO port. It could be 0~15.                                        */
/* Returns:                                                                                                */
/*              The bit value of the specified register			0 / 1                                      */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*              Get the bit value from the specified Data Output Value Register.                           */
/*              If the bit value is 1, it's meaning the pin is output data to high.                   	   */
/*              Otherwise, it's output data to low.           	                  						   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetDoutBit(E_DRVGPIO_PORT port, int32_t i32Bit)
{    
    if ((i32Bit < 0) || (i32Bit >= 15))
    {
        return E_DRVGPIO_ARGUMENT;
    }
 
    return ((inpw((uint32_t)&GPIOA->DOUT + (port*PORT_OFFSET))>>i32Bit) & 0x1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetPortDoutBits                                                                    */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/* Returns:                                                                                                */
/*              The portt value of the specified register		0 ~ 0xFFFF                                 */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*              Get the port value from the specified Data Output Value Register.                          */
/*              If the corresponding bit of the return port value is 1, it's meaning the the corresponding */
/*              bit is output data to high. Otherwise, it's output data to low.                            */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetPortDoutBits(E_DRVGPIO_PORT port)
{
    if (port > 4)
    {
        return E_DRVGPIO_ARGUMENT;
    }

    return inpw((uint32_t)&GPIOA->DOUT + (port*PORT_OFFSET));
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_SetBitMask                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32Bit - [in]                                                                              */
/*                  Specify pin of the GPIO port. It could be 0~15.                                        */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              This function is used to protect the write data function of the corresponding GPIO pin.    */
/*              When set the bit mask, the write signal is masked and write data to the protect bit is     */
/*              ignored.															                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetBitMask(E_DRVGPIO_PORT port, int32_t i32Bit)
{
    volatile uint32_t u32Reg;

    u32Reg = (uint32_t)&GPIOA->DMASK + (port*PORT_OFFSET);    

	outpw(u32Reg, inpw(u32Reg) | (1<<i32Bit));

	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetBitMask                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32Bit - [in]                                                                              */
/*                  Specify pin of the GPIO port. It could be 0~15.                                        */
/* Returns:                                                                                                */
/*              The bit value of the specified register			0 / 1           		                   */
/* Description:                                                                                            */
/*              Get the bit value from the specified Data Output Write Mask Register.       			   */
/*              If the bit value is 1, it's meaning the corresponding bit is protected.       			   */
/*              And write data to the bit is ignored.                            						   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetBitMask(E_DRVGPIO_PORT port, int32_t i32Bit)
{
    volatile uint32_t u32Reg;

    u32Reg = (uint32_t)&GPIOA->DMASK + (port*PORT_OFFSET);    

    return ((inpw(u32Reg)>>i32Bit) & 0x1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_ClrBitMask                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32Bit - [in]                                                                              */
/*                  Specify pin of the GPIO port. It could be 0~15.                                        */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              This function is used to remove the write protect function of the the corresponding GPIO   */
/*              pin. After remove the bit mask, write data to the corresponding bit is workable.           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_ClrBitMask(E_DRVGPIO_PORT port, int32_t i32Bit)
{
    volatile uint32_t u32Reg;

    u32Reg = (uint32_t)&GPIOA->DMASK + (port*PORT_OFFSET);    

	outpw(u32Reg, inpw(u32Reg) & ~(1<<i32Bit));
	
	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_SetPortMask                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32MaskData - [in]                                                                         */
/*                  Specify pins of the GPIO port. It could be 0~0xFFFF.            					   */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*              This function is used to protect the write data function of the corresponding GPIO pins.   */
/*              When set the bits are masked, write data to the protect bits are ignored.                  */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetPortMask(E_DRVGPIO_PORT port, int32_t i32MaskData)
{
    outpw((uint32_t)&GPIOA->DMASK + (port*PORT_OFFSET), DrvGPIO_GetPortMask(port) | i32MaskData);

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetPortMask                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/* Returns:                                                                                                */
/*              The portt value of the specified register		0 ~ 0xFFFF        	                       */
/* Description:                                                                                            */
/*             	Get the port value from the specified Data Output Write Mask Register.                     */
/*              If the corresponding bit of the return port value is 1, it's meaning the bits are 		   */
/*				protected. And write data to the bits are ignored.          		                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetPortMask(E_DRVGPIO_PORT port)
{
	return inpw((uint32_t)&GPIOA->DMASK + (port* PORT_OFFSET)); 
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_ClrPortMask                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32MaskData - [in]                                                                         */
/*                  Specify pins of the GPIO port. It could be 0~0xFFFF.                                   */
/* Returns:                                                                                                */
/*              E_SUCCESS, operation successful                                                            */
/* Description:                                                                                            */
/*              This function is used to remove the write protect function of the the corresponding GPIO   */
/*              pins. After remove those bits mask, write data to the corresponding bits are workable.     */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_ClrPortMask(E_DRVGPIO_PORT port, int32_t i32MaskData)
{
	outpw((uint32_t)&GPIOA->DMASK + (port*PORT_OFFSET), DrvGPIO_GetPortMask(port) & ~(i32MaskData));
	
	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_EnableDebounce                                                                     */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32Bit - [in]                                                                              */
/*                  Specify pin of the GPIO port. It could be 0~15.                                        */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              Enable the debounce function of the specified GPIO input pin.                              */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_EnableDebounce(E_DRVGPIO_PORT port, int32_t i32Bit)
{
    volatile uint32_t u32Reg;

    u32Reg = (uint32_t)&GPIOA->DBEN + (port*PORT_OFFSET);    

    outpw(u32Reg, inpw(u32Reg) | (1<<i32Bit));

	GPIO_DBNCECON->ICLK_ON = 1;

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_DisableDebounce                                                                    */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32Bit - [in]                                                                              */
/*                  Specify pin of the GPIO port. It could be 0~15.                                        */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              Disable the debounce function of the specified GPIO input pin.                             */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_DisableDebounce(E_DRVGPIO_PORT port, int32_t i32Bit)
{
    volatile uint32_t u32Reg;

    u32Reg = (uint32_t)&GPIOA->DBEN + (port*PORT_OFFSET);
        
    outpw(u32Reg, inpw(u32Reg) & ~(1<<i32Bit));
    
	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_SetDebounceTime                                                                    */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            u32CycleSelection - [in]                                                                   */
/*                  The number of sampling cycle selection, the range of value is from 0 ~ 15.         	   */
/*                  The target debounce time is (2^(u32CycleSelection))*(ClockSource) second               */
/*	            ClockSource - [in]                                                                         */
/*                 	E_DRVGPIO_DBCLKSRC, it could be DBCLKSRC_HCLK or DBCLKSRC_10K						   */                 
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*              Set the interrupt debounce sampling time based on the debounce counter clock source.       */
/*              If the debounce clock source is from internal 10 KHz and sampling cycle selection is 4.    */
/*              The target debounce time is (2^4)*(1/(10*1000)) s = 16*0.0001 s = 1600 us, and system      */
/*              will sampling interrupt input once per 1600 us.          								   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetDebounceTime(uint32_t u32CycleSelection, E_DRVGPIO_DBCLKSRC ClockSource)
{
    /* Maximum debounce time is 2^(15)*(clk src) */
    if (u32CycleSelection > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
	GPIO_DBNCECON->DBCLKSEL = u32CycleSelection ; 

	GPIO_DBNCECON->DBCLKSRC = ClockSource ; 

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetDebounceSampleCycle                                                             */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            None                                                                                       */
/* Returns:                                                                                                */
/*              Number of the sampling cycle selection        	0 ~ 15                                     */
/* Description:                                                                                            */
/*              This function is used to get the number of debounce sampling cycle selection.              */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetDebounceSampleCycle(void)
{   
    return GPIO_DBNCECON->DBCLKSEL;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_EnableInt                                                                          */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32Bit - [in]                                                                              */
/*                  Specify pin of the GPIO port. It could be 0~15.                                        */
/*                  But the GPB.14 and 15 are only used for external interrupt 0/1.                        */
/*	            TriggerType - [in]                                                                         */
/*                  E_DRVGPIO_INT_TYPE, specify the interrupt trigger type.                                */
/*                  It could be E_IO_RISING, E_IO_FALLING or E_IO_BOTH_EDGE and                            */
/*                  it's meaning the interrupt function enable by rising egde/high level,                  */ 
/*                  falling edge/low level or both riging edge and falling egde.                           */
/*                  If the interrupt mode is E_MODE_LEVEL and interrupt type is                            */
/*                  E_BOTH_EDGE¡Athen calling this API is ignored.                                     	   */
/*	            Mode - [in]                                                                                */
/*                  E_DRVGPIO_INT_MODE, specify the interrupt mode.                                        */
/*                  It could be E_MODE_EDGE or E_MODE_LEVEL to control the interrupt is by                 */
/*                	edge trigger or by level trigger.                                                      */
/*                  If the interrupt mode is E_MODE_LEVEL and interrupt type is                            */
/*                  E_BOTH_EDGE¡Athen calling this API is ignored.                                         */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*              Enable the interrupt function of the specified GPIO pin.                                   */
/*              Excpet for GPB.14 and GPB.15 pins are for external interrupt used.          			   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_EnableInt(E_DRVGPIO_PORT port, int32_t i32Bit, E_DRVGPIO_INT_TYPE TriggerType, E_DRVGPIO_INT_MODE Mode)
{
    volatile uint32_t u32Reg;

    if ((TriggerType == E_IO_BOTH_EDGE) && (Mode == E_MODE_LEVEL))
        return E_DRVGPIO_ARGUMENT;

    if ((port == E_GPB) && ((i32Bit == 14) || (i32Bit == 15)))
    {
        /* GPB14 is EINT0, GPB15 is EINT1. Both of them can't be assigned to GPAB interrupt */
        return E_DRVGPIO_ARGUMENT;    
    }
    
    /* Configure the interrupt to be rising/falling when edge trigger or high/low level when level trigger */
    u32Reg = (uint32_t)&GPIOA->IEN + (port*PORT_OFFSET);
	if (TriggerType == E_IO_RISING)
		outpw(u32Reg, inpw(u32Reg) | (1<<(i32Bit+16)));	 
	else if(TriggerType == E_IO_FALLING)
		outpw(u32Reg, inpw(u32Reg) | (1<<i32Bit));	 
	else if(TriggerType == E_IO_BOTH_EDGE)  
		outpw(u32Reg, inpw(u32Reg) | (1<<i32Bit)| (1<<(i32Bit+16)));	 
	else
		return E_DRVGPIO_ARGUMENT;

    /* Configure to be level trigger or edge trigger */
    u32Reg = (uint32_t)&GPIOA->IMD + (port*PORT_OFFSET);
	if(Mode == E_MODE_EDGE)
		outpw(u32Reg, inpw(u32Reg) & ~(1<<i32Bit));
	else if(Mode ==E_MODE_LEVEL)
		outpw(u32Reg, inpw(u32Reg) | (1<<i32Bit));
	else
		return E_DRVGPIO_ARGUMENT;
    
    /* Enable the relative interrupt of M0 */
    if ((port == E_GPA) || (port == E_GPB))
    {
        NVIC_SetPriority(GPAB_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(GPAB_IRQn);
    }

    if ((port == E_GPC) || (port == E_GPD) || (port == E_GPE))
    {
        NVIC_SetPriority(GPCDE_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(GPCDE_IRQn);
    }    

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_DisableInt                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/*	            i32Bit - [in]                                                                              */
/*                  Specify pin of the GPIO port. It could be 0~15.                                        */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              Disable the interrupt function of the specified GPIO pin.                                  */
/*              Excpet for GPB.14 and GPB.15 pins are for external interrupt used.          			   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_DisableInt(E_DRVGPIO_PORT port, int32_t i32Bit)
{                                             
    volatile uint32_t u32Reg;

    /* Disable both rising/falling or both high level/low level interrupts */
    u32Reg = (uint32_t)&GPIOA->IEN + (port*PORT_OFFSET);

    outpw(u32Reg, inpw(u32Reg) & ~((1<<i32Bit) | (1<<(i32Bit+16))));	 

    /* Disable the relative interrupt of M0 */
    if ((port == E_GPA) || (port == E_GPB))
    {
        NVIC_DisableIRQ(GPAB_IRQn);
    }

    if ((port == E_GPC) || (port == E_GPD) || (port == E_GPE))
    {
        NVIC_DisableIRQ(GPCDE_IRQn);
    }  
      
    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_SetIntCallback                  		                                           */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            pfGPABCallback - [in], the function pointer of GPA/GPB callback function.                  */
/*	            pfGPCDECallback - [in], the function pointer of GPC/GPD/GPE callback function.             */
/* Returns:                                                                                                */
/*              None                                                                                       */
/* Description:                                                                                            */
/*              Install the interrupt callback function for GPA/GPB port and GPC/GPD/GPE port.             */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SetIntCallback(GPIO_GPAB_CALLBACK pfGPABCallback, GPIO_GPCDE_CALLBACK pfGPCDECallback)
{
    _pfGPABCallback  = (void (*)(uint32_t, uint32_t))pfGPABCallback;
    _pfGPCDECallback = (void (*)(uint32_t, uint32_t, uint32_t))pfGPCDECallback;    
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_EnableEINT0                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            TriggerType - [in]                                                                         */
/*                  E_DRVGPIO_INT_TYPE, specify the interrupt trigger type.                                */
/*                  It could be E_IO_RISING, E_IO_FALLING or E_IO_BOTH_EDGE and                            */
/*                  it's meaning the interrupt function enable by rising egde/high level,                  */ 
/*                  falling edge/low level or both riging edge and falling egde.                           */
/*                  If the interrupt mode is E_MODE_LEVEL and interrupt type is                            */
/*                  E_BOTH_EDGE¡Athen calling this API is ignored.                                     	   */
/*	            Mode - [in]                                                                                */
/*                  E_DRVGPIO_INT_MODE, specify the interrupt mode.                                        */
/*                  It could be E_MODE_EDGE or E_MODE_LEVEL to control the interrupt is by                 */
/*                	edge trigger or by level trigger.                                                      */
/*                  If the interrupt mode is E_MODE_LEVEL and interrupt type is                            */
/*                  E_BOTH_EDGE¡Athen calling this API is ignored.                                         */
/*	            pfEINT0Callback - [in]                                                                     */
/*                  It's the function pointer of the external INT0 callback function.                      */
/* Returns:                                                                                                */
/*              None                                                                                       */
/* Description:                                                                                            */
/*              Enable the interrupt function for external GPIO interrupt from /INT0(GPB.14) pin.          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_EnableEINT0(E_DRVGPIO_INT_TYPE TriggerType, E_DRVGPIO_INT_MODE Mode, GPIO_EINT0_CALLBACK pfEINT0Callback)
{
    volatile uint32_t u32Reg;

    if ((TriggerType == E_IO_BOTH_EDGE) && (Mode == E_MODE_LEVEL))
        return ;

    u32Reg = (uint32_t)&GPIOA->IEN + (E_GPB*PORT_OFFSET);
	if (TriggerType == E_IO_RISING)
    {
        outpw(u32Reg, inpw(u32Reg) | (1UL<<(14+16)));	 
	}
    else if (TriggerType == E_IO_FALLING)
	{
        outpw(u32Reg, inpw(u32Reg) | (1UL<<(14)));	 
	}
    else if (TriggerType == E_IO_BOTH_EDGE)
    {  
        outpw(u32Reg, inpw(u32Reg) | (1UL<<(14))|(1UL<<(14+16)));	 
	}    	 

     /* Configure to be level trigger or edge trigger */
    u32Reg = (uint32_t)&GPIOA->IMD + (E_GPB*PORT_OFFSET);
	if (Mode == E_MODE_EDGE)
		outpw(u32Reg, inpw(u32Reg) & ~(1<<14));
	else if(Mode == E_MODE_LEVEL)
		outpw(u32Reg, inpw(u32Reg) | (1<<14));

   _pfEINT0Callback = pfEINT0Callback;

    NVIC_EnableIRQ(EINT0_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_DisableEINT0                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            None                                                                                       */
/* Returns:                                                                                                */
/*              None                                                                                       */
/* Description:                                                                                            */
/*              Disable the interrupt function for external GPIO interrupt from /INT0(GPB.14) pin.         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_DisableEINT0(void)
{
    volatile uint32_t u32Reg;

    u32Reg = (uint32_t)&GPIOA->IEN + (E_GPB*PORT_OFFSET);

    outpw(u32Reg, inpw(u32Reg) & ~((1UL<<(14))|(1UL<<(14+16))));	 

    NVIC_DisableIRQ(EINT0_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_EnableEINT1                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            TriggerType - [in]                                                                         */
/*                  E_DRVGPIO_INT_TYPE, specify the interrupt trigger type.                                */
/*                  It could be E_IO_RISING, E_IO_FALLING or E_IO_BOTH_EDGE and                            */
/*                  it's meaning the interrupt function enable by rising egde/high level,                  */ 
/*                  falling edge/low level or both riging edge and falling egde.                           */
/*                  If the interrupt mode is E_MODE_LEVEL and interrupt type is                            */
/*                  E_BOTH_EDGE¡Athen calling this API is ignored.                                     	   */
/*	            Mode - [in]                                                                                */
/*                  E_DRVGPIO_INT_MODE, specify the interrupt mode.                                        */
/*                  It could be E_MODE_EDGE or E_MODE_LEVEL to control the interrupt is by                 */
/*                	edge trigger or by level trigger.                                                      */
/*                  If the interrupt mode is E_MODE_LEVEL and interrupt type is                            */
/*                  E_BOTH_EDGE¡Athen calling this API is ignored.                                         */
/*	            pfEINT0Callback - [in]                                                                     */
/*                  It's the function pointer of the external INT0 callback function.                      */
/* Returns:                                                                                                */
/*              None                                                                                       */
/* Description:                                                                                            */
/*              Enable the interrupt function for external GPIO interrupt from /INT1(GPB.15) pin.          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_EnableEINT1(E_DRVGPIO_INT_TYPE TriggerType, E_DRVGPIO_INT_MODE Mode, GPIO_EINT1_CALLBACK pfEINT1Callback)
{
    volatile uint32_t u32Reg;

    if ((TriggerType == E_IO_BOTH_EDGE) && (Mode == E_MODE_LEVEL))
        return ;

    u32Reg = (uint32_t)&GPIOA->IEN + (E_GPB*PORT_OFFSET);
	if (TriggerType == E_IO_RISING)
    {
        outpw(u32Reg, inpw(u32Reg) | (1UL<<(15+16)));	 
	}
    else if (TriggerType == E_IO_FALLING)
	{
        outpw(u32Reg, inpw(u32Reg) | (1UL<<(15)));	 
	}
    else if (TriggerType == E_IO_BOTH_EDGE)
    {  
        outpw(u32Reg, inpw(u32Reg) | (1UL<<(15))|(1UL<<(15+16)));	 
	}    	 

     /* Configure to be level trigger or edge trigger */
    u32Reg = (uint32_t)&GPIOA->IMD + (E_GPB*PORT_OFFSET);
	if (Mode == E_MODE_EDGE)
		outpw(u32Reg, inpw(u32Reg) & ~(1<<15));
	else if(Mode == E_MODE_LEVEL)
		outpw(u32Reg, inpw(u32Reg) | (1<<15));

    _pfEINT1Callback = pfEINT1Callback;

    NVIC_EnableIRQ(EINT1_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_DisableEINT1                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            None                                                                                       */
/* Returns:                                                                                                */
/*              None                                                                                       */
/* Description:                                                                                            */
/*              Disable the interrupt function for external GPIO interrupt from /INT1(GPB.15) pin.         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_DisableEINT1(void)
{
    volatile uint32_t u32Reg;

    u32Reg = (uint32_t)&GPIOA->IEN + (E_GPB*PORT_OFFSET);

    outpw(u32Reg, inpw(u32Reg) & ~((1UL<<(15))|(1UL<<(15+16))));	 

    NVIC_DisableIRQ(EINT1_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetIntStatus                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port. It could be E_GPA, E_GPB, E_GPC, E_GPD and E_GPE.   */
/* Returns:                                                                                                */
/*              The portt value of the specified register		0 ~ 0xFFFF                                 */
/* Description:                                                                                            */
/*              Get the port value from the specified Interrupt Trigger Source Indicator Register.	       */
/*              If the corresponding bit of the return port value is 1, it's meaning the interrupt		   */ 
/*				occurred at the corresponding bit. Otherwise, no interrupt occurred at that bit.           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetIntStatus(E_DRVGPIO_PORT port)
{
	GPIO_T * tGPIO;

	tGPIO = (GPIO_T *)((uint32_t)GPIOA + (port*PORT_OFFSET));  

	return tGPIO->ISRC;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_InitFunction                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            function - [in]                                                                            */
/*                  E_DRVGPIO_FUNC, specified the relative GPIO pins as special function pins.             */
/*                  It could be :                                                                          */
/*                              E_FUNC_GPIO                                                                */
/*                              E_FUNC_CLKO                                                                */
/*                              E_FUNC_I2C0 / E_FUNC_I2C1                                                  */
/*                              E_FUNC_I2S                                                                 */
/*                              E_FUNC_CAN0                                                                */
/*                              E_FUNC_ACMP0 / E_FUNC_ACMP1                                                */
/*                              E_FUNC_SPI0 /E_ FUNC_SPI1 / E_FUNC_SPI2 / E_FUNC_SPI3 /                    */
/*                              E_FUNC_ADC0 / E_FUNC_ADC1 / E_FUNC_ADC2 / E_FUNC_ADC3 / 				   */
/*								E_FUNC_ADC4 / E_FUNC_ADC5 / E_FUNC_ADC6 / E_FUNC_ADC7                      */
/*                              E_FUNC_EXTINT0 / E_FUNC_EXTINT1                                            */
/*                              E_FUNC_TMR0 / E_FUNC_TMR1 / E_FUNC_TMR2 / E_FUNC_TMR3                      */
/*                              E_FUNC_UART0 / E_FUNC_UART1 / E_FUNC_UART2                                 */
/*                              E_FUNC_PWM01 / E_FUNC_PWM23 / E_FUNC_PWM45 / E_FUNC_PWM67                  */
/*                              E_FUNC_EBI_8B / E_FUNC_EBI_16B                                             */
/*                              E_FUNC_SPI0_QFN36PIN                                                       */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*              Initialize the specified function and configure the relative pins             			   */
/*              for specified function used.                											   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_InitFunction(E_DRVGPIO_FUNC function)
{  
	switch ( function )
	{
		/*---------------------------------------------------------------------------------------------------------*/
		/* GPIO                                                                                                    */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_GPIO:
		{
		  	outpw(&SYS->GPAMFP, 0);
			outpw(&SYS->GPBMFP, 0);
			outpw(&SYS->GPCMFP, 0);
			outpw(&SYS->GPDMFP, 0);
			outpw(&SYS->GPEMFP, 0);		  	
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* Clock frequence output                                                                                        */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_CLKO:
		{
		  	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0x1<<12));
		  	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) | (0x1<<10));
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* I2C0                                                                                                    */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_I2C0:
		{
		  	outpw(&SYS->GPAMFP, inpw(&SYS->GPAMFP) | (0x3<<8));
		}break;		

		/*---------------------------------------------------------------------------------------------------------*/
		/* I2C1                                                                                                    */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_I2C1:
		{
		  	outpw(&SYS->GPAMFP, inpw(&SYS->GPAMFP) | (0x3<<10));
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* I2S                                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_I2S:	
		{
          	outpw(&SYS->GPCMFP, inpw(&SYS->GPCMFP) | (0xF<<0));
          	outpw(&SYS->GPAMFP, inpw(&SYS->GPAMFP) | (0x1<<15));
          	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) | (0x1F<<5));			
		}break;
		
		/*---------------------------------------------------------------------------------------------------------*/
		/* CAN0                                                                                                    */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_CAN0:	
		{
          	outpw(&SYS->GPDMFP, inpw(&SYS->GPDMFP) | (0x3<<6));
		}break;	

		/*---------------------------------------------------------------------------------------------------------*/
		/* Analog Comparator                                                                                       */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_ACMP0:	
		{
          	outpw(&SYS->GPCMFP, inpw(&SYS->GPCMFP) | (0x3<<6));
		}break;

		case E_FUNC_ACMP1:
		{
          	outpw(&SYS->GPCMFP, inpw(&SYS->GPCMFP) | (0x3<<14));
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* SPI                                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_SPI0:	
		{
          	outpw(&SYS->GPCMFP, inpw(&SYS->GPCMFP) | (0xF<<0));
          	outpw(&SYS->ALTMFP, (inpw(&SYS->ALTMFP) & ~(0xF<<5)) | (0x1<<0));

          	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0x1<<10));
          	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) | (0x1<<0));

		}break;

		case E_FUNC_SPI1:
		{
          	outpw(&SYS->GPCMFP, inpw(&SYS->GPCMFP) | (0xF<<8));

          	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0x1<<9));
          	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) | (0x1<<1));
		}break;

		case E_FUNC_SPI2:	
		{
          	outpw(&SYS->GPDMFP, inpw(&SYS->GPDMFP) | (0xF<<0));
          	
           	outpw(&SYS->GPAMFP, inpw(&SYS->GPAMFP) | (0x1<<9));
            outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) | (0x1<<7));
		}break;				

		case E_FUNC_SPI3:	
		{
          	outpw(&SYS->GPDMFP, inpw(&SYS->GPDMFP) | (0xF<<8));

           	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0x1<<14));
            outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) | (0x1<<3));
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* ADC                                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_ADC7:
		{
          	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) & ~(0x1<<2));
        }
		case E_FUNC_ADC0:
		case E_FUNC_ADC1:
		case E_FUNC_ADC2:
		case E_FUNC_ADC3:
		case E_FUNC_ADC4:
		case E_FUNC_ADC5:
		case E_FUNC_ADC6:
		{
          	outpw(&SYS->GPAMFP, inpw(&SYS->GPAMFP) | (0x1<<(function-E_FUNC_ADC0)));
        }break;

  		/*---------------------------------------------------------------------------------------------------------*/
		/* External Interrupt                                                                                      */
		/*---------------------------------------------------------------------------------------------------------*/		
	  	case E_FUNC_EXTINT0:
		{	
          	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0x1<<14));
          	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) & ~(0x1<<3));
		}break;

	  	case E_FUNC_EXTINT1:
		{	
          	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0x1<<15));
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* TIMER                                                                                                   */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_TMR0:
		{
          	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0x1<<8));
		}break;
	
		case E_FUNC_TMR1:
		{
          	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0x1<<9));
          	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) & ~(0x1<<1));
		}break;

		case E_FUNC_TMR2:
		{
          	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0x1<<10));
          	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) & ~(0x1<<0));
		}break;

		case E_FUNC_TMR3:
		{
          	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0x1<<11));
          	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) & ~(0x1<<4));
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* UART                                                                                                    */
		/*--------------------------------------------------------------------------------------------------------*/
		case E_FUNC_UART0:
		{
          	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0xF<<0));
		}break;

		case E_FUNC_UART1:
		{
          	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0xF<<4));
		}break;

		case E_FUNC_UART2:
		{
          	outpw(&SYS->GPDMFP, inpw(&SYS->GPDMFP) | (0x3<<14));
        }break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* PWM                                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_PWM01:
		{
		  	outpw(&SYS->GPAMFP, inpw(&SYS->GPAMFP) | (0x3<<12));
		}break;

		case E_FUNC_PWM23:
		{
		  	outpw(&SYS->GPAMFP, inpw(&SYS->GPAMFP) | (0x3<<14));
		  	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) & ~(0x1<<9));
		}break;

		case E_FUNC_PWM45:
		{
		  	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0x1<<11));
		  	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) | (0x1<<4));
		  	outpw(&SYS->GPEMFP, inpw(&SYS->GPEMFP) | (0x1<<5));
		}break;

		case E_FUNC_PWM67:
		{
		  	outpw(&SYS->GPEMFP, inpw(&SYS->GPEMFP) | (0x1));
		  	outpw(&SYS->GPEMFP, inpw(&SYS->GPEMFP) | (0x1<<1));
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* EBI                                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_EBI_16B:	
		{
			// Enable nWRH & nWRL for support Byte-Write in 16bit Data Width Device(SARM) 
          	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0x3<<2));
          	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) | (0x3<<13));			
		
			// Enable EBI AD High-byte, bit 15~8
          	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) | (0xFF<<16));			
          	outpw(&SYS->GPAMFP, inpw(&SYS->GPAMFP) | (0x703E));
		}
				
		case E_FUNC_EBI_8B:	
		{
			// Enable EBI_EN and EBI_MCLK_EN
          	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) | (0x3<<11));			
          	outpw(&SYS->GPCMFP, inpw(&SYS->GPCMFP) | (0x1<<8));
			
			// Enable nRD/nWR/ALE/nCS for EBI
          	outpw(&SYS->GPAMFP, inpw(&SYS->GPAMFP) | (0x3<<10));
          	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0x3<<6));
			
			// Enable EBI AD Low-byte, bit 7~0
          	outpw(&SYS->GPAMFP, inpw(&SYS->GPAMFP) | (0x3<<6));
          	outpw(&SYS->GPCMFP, inpw(&SYS->GPCMFP) | (0x3<<6) | (0x3<<14));
          	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) | (0x3<<12));
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* SPI - QFN36PIN                                                                                          */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_SPI0_QFN36PIN:	
		{           		
          	outpw(&SYS->GPCMFP, inpw(&SYS->GPCMFP) | (0xF<<0));
          	outpw(&SYS->ALTMFP, inpw(&SYS->ALTMFP) & ~(0xF<<5));

          	outpw(&SYS->GPDMFP, inpw(&SYS->GPDMFP) | (0x1<<1));
		}break;

		default:
			return E_DRVGPIO_ARGUMENT;
	}

	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetVersion                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            None                                                                                       */
/* Returns:                                                                                                */
/*              The version number of GPIO driver                                                          */
/* Description:                                                                                            */
/*              This function is used to return the version number of GPIO driver.                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetVersion(void)
{
   return DRVGPIO_VERSION_NUM;
}


