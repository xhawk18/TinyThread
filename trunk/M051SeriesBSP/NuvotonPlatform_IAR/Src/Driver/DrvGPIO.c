/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>

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
static void (*_pfP0P1Callback)(uint32_t u32P0Status, uint32_t u32P1Status);
static void (*_pfP2P3P4Callback)(uint32_t u32P2Status, uint32_t u32P3Status, uint32_t u32P4Status);
static void (*_pfEINT0Callback)(void);
static void (*_pfEINT1Callback)(void);

/*---------------------------------------------------------------------------------------------------------*/
/* Port 0, 1 ISR                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void GPIOP0P1_IRQHandler(void)
{
	uint32_t u32P0Status, u32P1Status;
	
    /* Keep the interrupt source */
	u32P0Status = PORT0->ISRC;
	u32P1Status = PORT1->ISRC;

    /* Clear the interrupt */
    PORT0->ISRC = u32P0Status;
    PORT1->ISRC = u32P1Status;

    /* Call the callback function of P0/P1 interrupt */
    if (_pfP0P1Callback)
        _pfP0P1Callback(u32P0Status, u32P1Status);    
}

/*---------------------------------------------------------------------------------------------------------*/
/* Port 2, 3, 4 ISR                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void GPIOP2P3P4_IRQHandler(void)
{
	uint32_t u32P2Status, u32P3Status, u32P4Status;
	
    /* Keep the interrupt source */
	u32P2Status = PORT2->ISRC;
	u32P3Status = PORT3->ISRC;
	u32P4Status = PORT4->ISRC;

    /* Clear the interrupt */
    PORT2->ISRC = u32P2Status;
    PORT3->ISRC = u32P3Status;
    PORT4->ISRC = u32P4Status;

    /* Call the callback function of P2/P3/P4 interrupt */
    if (_pfP2P3P4Callback)
        _pfP2P3P4Callback(u32P2Status, u32P3Status, u32P4Status);    
}

/*---------------------------------------------------------------------------------------------------------*/
/* External INT0(Port32) & INT1(Port33) ISR                                                                */
/*---------------------------------------------------------------------------------------------------------*/
void EINT0_IRQHandler(void)
{
	/* For P32, clear the INT flag */
    PORT3->ISRC = PORT3->ISRC;
	
	if (_pfEINT0Callback)
        _pfEINT0Callback();
}

void EINT1_IRQHandler(void)
{	
	/* For P33, clear the INT flag */
    PORT3->ISRC = PORT3->ISRC;
	
	if (_pfEINT1Callback)
        _pfEINT1Callback();
}






/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_Open                                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            pin - [in]                                                                                 */
/*                  Specify pin of the GPIO port. It could be E_PIN0, E_PIN2 ... ~  E_PIN7.                */
/*	            IOMode - [in]                                                                              */
/*                  E_DRVGPIO_IO, set the specified GPIO pin to be E_IO_INPUT, E_IO_OUTPUT,                */
/*                  E_IO_OPENDRAIN or E_IO_QUASI mode.                                                     */
/* Returns:                                                                                                */
/*              None                                                                                       */
/* Description:                                                                                            */
/*              Set the specified GPIO pin to the specified GPIO operation mode.	                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_Open(E_DRVGPIO_PORT port, E_DRVGPIO_PIN pin, E_DRVGPIO_IO IOMode)
{
    volatile uint32_t u32Reg = (uint32_t)((uint32_t)&PORT0->PMD + (port*PORT_OFFSET));

    if ((IOMode == E_IO_INPUT) ||  (IOMode == E_IO_OUTPUT) || (IOMode == E_IO_OPENDRAIN))
    {
        outpw(u32Reg, inpw(u32Reg) & ~(0x3<<(pin*2)));

        if (IOMode == E_IO_OUTPUT)
        {
            outpw(u32Reg, inpw(u32Reg) | (0x1<<(pin*2)));
        }else
        if (IOMode == E_IO_OPENDRAIN)
        {
            outpw(u32Reg, inpw(u32Reg) | (0x2<<(pin*2)));
        }
    }else
	if (IOMode == E_IO_QUASI)
    {
        outpw(u32Reg, inpw(u32Reg) | (0x3<<(pin*2)));
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_Close                                                                              */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            pin - [in]                                                                                 */
/*                  Specify pin of the GPIO port. It could be E_PIN0, E_PIN2 ... ~  E_PIN7.                */
/* Returns:                                                                                                */
/*              None                                                                                       */
/* Description:                                                                                            */
/*              Close the specified GPIO pin function and set the pin to quasi-bidirectional mode.         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_Close(E_DRVGPIO_PORT port, E_DRVGPIO_PIN pin)
{  
    volatile uint32_t u32Reg = (uint32_t)((uint32_t)&PORT0->PMD + (port*PORT_OFFSET));

    outpw(u32Reg, inpw(u32Reg) | (0x3<<(pin*2)));
	
	PORT_DBNCECON->ICLK_ON = 0; 
}	

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_SetBit                                                                             */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            pin - [in]                                                                                 */
/*                  Specify pin of the GPIO port. It could be E_PIN0, E_PIN2 ... ~  E_PIN7.                */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              Set the specified GPIO pin to 1.                                                      	   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetBit(E_DRVGPIO_PORT port, E_DRVGPIO_PIN pin)
{
	outpw((PORT_BIT_DOUT + (port*0x20) + (pin*4)), 1);

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetBit                                                                             */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            pin - [in]                                                                                 */
/*                  Specify pin of the GPIO port. It could be E_PIN0, E_PIN2 ... ~  E_PIN7.                */
/* Returns:                                                                                                */
/*              The specified input pin value					0 / 1                                      */
/* Description:                                                                                            */
/*              Get the pin value from the specified input GPIO pin.            			               */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetBit(E_DRVGPIO_PORT port, E_DRVGPIO_PIN pin)
{
	return inpw((PORT_BIT_DOUT + (port*0x20) + (pin*4)));
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_ClrBit                                                                             */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            pin - [in]                                                                                 */
/*                  Specify pin of the GPIO port. It could be E_PIN0, E_PIN2 ... ~  E_PIN7.                */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              Set the specified GPIO pin to 0.               			                                   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_ClrBit(E_DRVGPIO_PORT port, E_DRVGPIO_PIN pin)
{
	outpw((PORT_BIT_DOUT + (port*0x20) + (pin*4)), 0);

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_SetPortBits                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            i32PortValue - [in]                                                                        */
/*                  The data output value. It could be 0~0xFF                            				   */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              Set the output port value to the specified GPIO port.             	                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetPortBits(E_DRVGPIO_PORT port, int32_t i32PortValue)
{
	outpw((uint32_t)&PORT0->DOUT + (port*PORT_OFFSET), i32PortValue);

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetPortBits                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/* Returns:                                                                                                */
/*              The specified input port value            		0 ~ 0xFF                                   */
/* Description:                                                                                            */
/*             Get the input port value from the specified GPIO port.                                      */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetPortBits(E_DRVGPIO_PORT port)
{
    volatile uint32_t u32Reg = (uint32_t)((uint32_t)&PORT0->PIN + (port*PORT_OFFSET));

    return inpw(u32Reg);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetDoutBit                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            pin - [in]                                                                                 */
/*                  Specify pin of the GPIO port. It could be E_PIN0, E_PIN2 ... ~  E_PIN7.                */
/* Returns:                                                                                                */
/*              The bit value of the specified register			0 / 1                                      */
/* Description:                                                                                            */
/*              Get the bit value from the specified Data Output Value Register.                           */
/*              If the bit value is 1, it's meaning the pin is output data to high.                   	   */
/*              Otherwise, it's output data to low.           	                  						   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetDoutBit(E_DRVGPIO_PORT port, E_DRVGPIO_PIN pin)
{    
    return ((inpw((uint32_t)&PORT0->DOUT + (port*PORT_OFFSET)) >> pin) & 0x1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetPortDoutBits                                                                    */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/* Returns:                                                                                                */
/*              The portt value of the specified register		0 ~ 0xFF                                   */
/* Description:                                                                                            */
/*              Get the port value from the specified Data Output Value Register.                          */
/*              If the corresponding bit of the return port value is 1, it's meaning the the corresponding */
/*              bit is output data to high. Otherwise, it's output data to low.                            */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetPortDoutBits(E_DRVGPIO_PORT port)
{
    return inpw((uint32_t)&PORT0->DOUT + (port*PORT_OFFSET));
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_SetBitMask                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            pin - [in]                                                                                 */
/*                  Specify pin of the GPIO port. It could be E_PIN0, E_PIN2 ... ~  E_PIN7.                */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              This function is used to protect the write data function of the corresponding GPIO pin.    */
/*              When set the bit mask, the write signal is masked and write data to the protect bit is     */
/*              ignored.															                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetBitMask(E_DRVGPIO_PORT port, E_DRVGPIO_PIN pin)
{
    volatile uint32_t u32Reg = (uint32_t)((uint32_t)&PORT0->DMASK + (port*PORT_OFFSET));

    outpw(u32Reg, inpw(u32Reg) | (1<<pin));

	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetBitMask                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            pin - [in]                                                                                 */
/*                  Specify pin of the GPIO port. It could be E_PIN0, E_PIN2 ... ~  E_PIN7.                */
/* Returns:                                                                                                */
/*              The bit value of the specified register			0 / 1           		                   */
/* Description:                                                                                            */
/*              Get the bit value from the specified Data Output Write Mask Register.       			   */
/*              If the bit value is 1, it's meaning the corresponding bit is protected.       			   */
/*              And write data to the bit is ignored.                            						   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetBitMask(E_DRVGPIO_PORT port, E_DRVGPIO_PIN pin)
{
    volatile uint32_t u32Reg;

    u32Reg = (uint32_t)&PORT0->DMASK + (port*PORT_OFFSET);    

    return ((inpw(u32Reg)>>pin) & 0x1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_ClrBitMask                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            pin - [in]                                                                                 */
/*                  Specify pin of the GPIO port. It could be E_PIN0, E_PIN2 ... ~  E_PIN7.                */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              This function is used to remove the write protect function of the the corresponding GPIO   */
/*              pin. After remove the bit mask, write data to the corresponding bit is workable.           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_ClrBitMask(E_DRVGPIO_PORT port, E_DRVGPIO_PIN pin)
{
    volatile uint32_t u32Reg = (uint32_t)((uint32_t)&PORT0->DMASK + (port*PORT_OFFSET));

    outpw(u32Reg, inpw(u32Reg) & ~(1<<pin));

	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_SetPortMask                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            i32PortValue - [in]                                                                        */
/*                  Specify pins of the GPIO port. It could be 0~0xFF.              					   */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              This function is used to protect the write data function of the corresponding GPIO pins.   */
/*              When set the bits are masked, write data to the protect bits are ignored.                  */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetPortMask(E_DRVGPIO_PORT port, int32_t i32PortValue)
{
    volatile uint32_t u32Reg = (uint32_t)((uint32_t)&PORT0->DMASK + (port*PORT_OFFSET));

    outpw(u32Reg, inpw(u32Reg) | (i32PortValue));

	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetPortMask                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/* Returns:                                                                                                */
/*              The portt value of the specified register		0 ~ 0xFF        	                       */
/* Description:                                                                                            */
/*             	Get the port value from the specified Data Output Write Mask Register.                     */
/*              If the corresponding bit of the return port value is 1, it's meaning the bits are 		   */
/*				protected. And write data to the bits are ignored.          		                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetPortMask(E_DRVGPIO_PORT port)
{
    return inpw((uint32_t)&PORT0->DMASK + (port*PORT_OFFSET));
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_ClrPortMask                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            i32PortValue - [in]                                                                        */
/*                  Specify pins of the GPIO port. It could be 0~0xFF.                                     */
/* Returns:                                                                                                */
/*              E_SUCCESS, operation successful                                                            */
/* Description:                                                                                            */
/*              This function is used to remove the write protect function of the the corresponding GPIO   */
/*              pins. After remove those bits mask, write data to the corresponding bits are workable.     */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_ClrPortMask(E_DRVGPIO_PORT port, int32_t i32PortValue)
{
	outpw((uint32_t)&PORT0->DMASK + (port*PORT_OFFSET), DrvGPIO_GetPortMask(port) & ~(i32PortValue));
	
	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_EnableDebounce                                                                     */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            pin - [in]                                                                                 */
/*                  Specify pin of the GPIO port. It could be E_PIN0, E_PIN2 ... ~  E_PIN7.                */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              Enable the debounce function of the specified GPIO input pin.                              */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_EnableDebounce(E_DRVGPIO_PORT port, E_DRVGPIO_PIN pin)
{
    volatile uint32_t u32Reg = (uint32_t)&PORT0->DBEN + (port*PORT_OFFSET);

    outpw(u32Reg, inpw(u32Reg) | (1 << pin));

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_DisableDebounce                                                                    */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            pin - [in]                                                                                 */
/*                  Specify pin of the GPIO port. It could be E_PIN0, E_PIN2 ... ~  E_PIN7.                */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              Disable the debounce function of the specified GPIO input pin.                             */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_DisableDebounce(E_DRVGPIO_PORT port, E_DRVGPIO_PIN pin)
{
    volatile uint32_t u32Reg = (uint32_t)&PORT0->DBEN + (port*PORT_OFFSET);

    outpw(u32Reg , inpw(u32Reg) & (~(1<<pin)));

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
    
	PORT_DBNCECON->DBCLKSEL = u32CycleSelection ; 

	PORT_DBNCECON->DBCLKSRC = ClockSource ; 

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
    return PORT_DBNCECON->DBCLKSEL;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_EnableInt                                                                          */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            pin - [in]                                                                                 */
/*                  Specify pin of the GPIO port. It could be E_PIN0, E_PIN2 ... ~  E_PIN7.                */
/*                  But the Port3-2 and Port3-3 are only used for external interrupt 0/1.                  */ 
/*	            Type - [in]                                                                                */
/*                  E_DRVGPIO_INT_TYPE, specify the interrupt trigger type.                                */
/*                  It could be E_IO_RISING, E_IO_FALLING or E_IO_BOTH_EDGE and                            */
/*                  it's meaning the interrupt function enable by rising egde/high level,                  */ 
/*                  falling edge/low level or both riging edge and falling egde.                           */
/*                  If the interrupt mode is E_MODE_LEVEL and interrupt type is                            */
/*                  E_BOTH_EDGE�Athen calling this API is ignored.                                     	   */
/*	            Mode - [in]                                                                                */
/*                  E_DRVGPIO_INT_MODE, specify the interrupt mode.                                        */
/*                  It could be E_MODE_EDGE or E_MODE_LEVEL to control the interrupt is by                 */
/*                	edge trigger or by level trigger.                                                      */
/*                  If the interrupt mode is E_MODE_LEVEL and interrupt type is                            */
/*                  E_BOTH_EDGE�Athen calling this API is ignored.                                         */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*              Enable the interrupt function of the specified GPIO pin.                                   */
/*              Except for Port3-2 and Port3-2 pins are for external interrupt used.  				       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_EnableInt(E_DRVGPIO_PORT port, E_DRVGPIO_PIN pin, E_DRVGPIO_INT_TYPE Type, E_DRVGPIO_INT_MODE Mode)
{
    volatile uint32_t u32Reg = (uint32_t)((uint32_t)&PORT0->IEN + (port*PORT_OFFSET));

    if ((port == E_PORT3) && ((pin == (E_DRVGPIO_PIN)E_EINT0_PIN) || (pin == (E_DRVGPIO_PIN)E_EINT1_PIN)))
    {
        /* P32 is EINT0, P33 is EINT1. Both of them can't be assigned to gereral P3 interrupt */
        return E_DRVGPIO_ARGUMENT;    
    }
    
    /* Configure the interrupt to be rising/falling when edge trigger or high/low level when level trigger */
	if (Type == E_IO_RISING)
		outpw(u32Reg, inpw(u32Reg) | (1<<(pin+16)));	 
	else if (Type == E_IO_FALLING)
		outpw(u32Reg , inpw(u32Reg) | (1<<pin));	 
	else if (Type == E_IO_BOTH_EDGE)  
		outpw(u32Reg , inpw(u32Reg) | (1<<pin)| (1<<(pin+16)));	 
	else
		return E_DRVGPIO_ARGUMENT;

    /* Configure to be level trigger or edge trigger */
    u32Reg = (uint32_t)((uint32_t)&PORT0->IMD + (port*PORT_OFFSET));
	if (Mode == E_MODE_EDGE)
		outpw(u32Reg, inpw(u32Reg) & (~(1<<pin)));
	else if (Mode == E_MODE_LEVEL)
		outpw(u32Reg, inpw(u32Reg) | (1<<pin));
	else
		return E_DRVGPIO_ARGUMENT;
    
    /* Configure the relative interrupt of M0 */
    if ((port == E_PORT0) || (port == E_PORT1))
    {
        NVIC_SetPriority(GPIO_P0P1_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(GPIO_P0P1_IRQn);
    }

    if ((port == E_PORT2) || (port == E_PORT3) || (port == E_PORT4))
    {
        NVIC_SetPriority(GPIO_P2P3P4_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(GPIO_P2P3P4_IRQn);
    } 
	
    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_DisableInt                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/*	            pin - [in]                                                                                 */
/*                  Specify pin of the GPIO port. It could be E_PIN0, E_PIN2 ... ~  E_PIN7.                */
/*                  But the Port3-2 and Port3-3 are only used for external interrupt 0/1.                  */ 
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              Disable the interrupt function of the specified GPIO pin.                                  */
/*              Except for Port3-2 and Port3-2 pins are for external interrupt used.  				       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_DisableInt(E_DRVGPIO_PORT port, E_DRVGPIO_PIN pin)
{
    volatile uint32_t u32Reg = (uint32_t)((uint32_t)&PORT0->IEN + (port*PORT_OFFSET));
   
    /* Disable both rising/falling or both high level/low level interrupts */
    outpw(u32Reg , inpw(u32Reg) & (~((1<<pin) | (1<<(pin+16)))));	
     
    if ((port == E_PORT0) || (port == E_PORT1))
        NVIC_DisableIRQ(GPIO_P0P1_IRQn);

    if ((port == E_PORT2) || (port == E_PORT3) || (port == E_PORT4))
        NVIC_DisableIRQ(GPIO_P2P3P4_IRQn);

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_SetIntCallback                  		                                           */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            pfP0P1Callback - [in], the function pointer of Port0/Port1 callback function.              */
/*	            pfP2P3P4Callback - [in], the function pointer of Port2/Port3/Port4 callback function.      */
/* Returns:                                                                                                */
/*              None                                                                                       */
/* Description:                                                                                            */
/*              Install the interrupt callback function for GPIO Port0/Port1 and Port2/Port3/Port4.        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SetIntCallback(P0P1_CALLBACK pfP0P1Callback, P2P3P4_CALLBACK pfP2P3P4Callback)
{
    _pfP0P1Callback  = (void (*)(uint32_t, uint32_t))pfP0P1Callback;
    _pfP2P3P4Callback = (void (*)(uint32_t, uint32_t, uint32_t))pfP2P3P4Callback;    
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_EnableEINT                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            pin - [in]                                                                                 */
/*                  Specify the external INT pin of GPIO Port3.                                            */
/*                  It could be E_EINT0_PIN (Port3-2) or E_EINT1_PIN (Port3-3).                            */
/*	            Type - [in]                                                                                */
/*                  E_DRVGPIO_INT_TYPE, specify the interrupt trigger type.                                */
/*                  It could be E_IO_RISING, E_IO_FALLING or E_IO_BOTH_EDGE and                            */
/*                  it's meaning the interrupt function enable by rising egde/high level,                  */ 
/*                  falling edge/low level or both riging edge and falling egde.                           */
/*                  If the interrupt mode is E_MODE_LEVEL and interrupt type is                            */
/*                  E_BOTH_EDGE�Athen calling this API is ignored.                                     	   */
/*	            Mode - [in]                                                                                */
/*                  E_DRVGPIO_INT_MODE, specify the interrupt mode.                                        */
/*                  It could be E_MODE_EDGE or E_MODE_LEVEL to control the interrupt is by                 */
/*                	edge trigger or by level trigger.                                                      */
/*                  If the interrupt mode is E_MODE_LEVEL and interrupt type is                            */
/*                  E_BOTH_EDGE�Athen calling this API is ignored.                                         */
/*	            pfEINTCallback - [in]                                                                      */
/*                  It's the function pointer of the specified external INT callback function.             */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              Enable the interrupt function for external GPIO interrupt from P32/P33 pin.                */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_EnableEINT(E_DRVGPIO_EXT_INT_PIN pin, E_DRVGPIO_INT_TYPE Type, E_DRVGPIO_INT_MODE Mode, EINT_CALLBACK pfEINTCallback)
{
    volatile uint32_t u32Reg = (uint32_t)((uint32_t)&PORT0->IEN + (E_PORT3*PORT_OFFSET));
	
	if (Type == E_IO_RISING)
        outpw(u32Reg , inpw(u32Reg) | (1<<(pin+16)));	 
    else if (Type == E_IO_FALLING)
        outpw(u32Reg , inpw(u32Reg) | (1<<pin));	 
    else if (Type == E_IO_BOTH_EDGE)
        outpw(u32Reg , inpw(u32Reg) | ((1<<pin) | (1<<(pin+16))));	 

    u32Reg = (uint32_t)((uint32_t)&PORT0->IMD + (E_PORT3*PORT_OFFSET));
	if (Mode == E_MODE_EDGE)
		outpw(u32Reg, inpw(u32Reg) & (~(1<<pin)));
	else    /* Mode == MODE_LEVEL */
		outpw(u32Reg, inpw(u32Reg) | (1<<pin));

    if (pin == E_EINT0_PIN)
    {
        _pfEINT0Callback = pfEINTCallback;
        NVIC_EnableIRQ(EINT0_IRQn);
    }else
    {
        _pfEINT1Callback = pfEINTCallback;
        NVIC_EnableIRQ(EINT1_IRQn);
    }

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_DisableEINT                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            pin - [in]                                                                                 */
/*                  Specify the external INT pin of GPIO Port3. It could be E_EINT0_PIN or E_EINT1_PIN.    */
/*                  And E_EINT0_PIN is the pin2, E_EINT1_PIN is the pin3.                                  */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/* Description:                                                                                            */
/*              Disable the interrupt function for specified external GPIO interrupt from P32/P33 pin.     */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_DisableEINT(E_DRVGPIO_EXT_INT_PIN pin)
{
    volatile uint32_t u32Reg = (uint32_t)((uint32_t)&PORT0->IEN + (E_PORT3*PORT_OFFSET));
   
    outpw(u32Reg , inpw(u32Reg) & (~((1<<pin) | (1<<(pin+16)))));	 

    if (pin == E_EINT0_PIN)
        NVIC_DisableIRQ(EINT0_IRQn);
    else
        NVIC_DisableIRQ(EINT1_IRQn);

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvGPIO_GetIntStatus                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            port - [in]                                                                                */
/*                  E_DRVGPIO_PORT, specify GPIO port.                                                     */
/*                  It could be E_PORT0, E_PORT1, E_PORT2, E_PORT3 and E_PORT4.                            */
/* Returns:                                                                                                */
/*              The portt value of the specified register		0 ~ 0xFF                                   */
/* Description:                                                                                            */
/*              Get the port value from the specified Interrupt Trigger Source Indicator Register.	       */
/*              If the corresponding bit of the return port value is 1, it's meaning the interrupt		   */ 
/*				occurred at the corresponding bit. Otherwise, no interrupt occurred at that bit.           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetIntStatus(E_DRVGPIO_PORT port)
{
    GPIO_T *pReg;
                        
	pReg = (GPIO_T *)((uint32_t)PORT0 + (port*PORT_OFFSET));  

	return pReg->ISRC;
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
/*                              E_FUNC_I2C                                                                 */
/*                              E_FUNC_SPI0 /E_ FUNC_SPI1                                                  */
/*                              E_FUNC_ADC0 / E_FUNC_ADC1 / E_FUNC_ADC2 / E_FUNC_ADC3 / 				   */
/*								E_FUNC_ADC4 / E_FUNC_ADC5 / E_FUNC_ADC6 / E_FUNC_ADC7                      */
/*                              E_FUNC_EXTINT0 / E_FUNC_EXTINT1                                            */
/*                              E_FUNC_TMR0 / E_FUNC_TMR1 / E_FUNC_TMR2 / E_FUNC_TMR3                      */
/*                              E_FUNC_UART0 / E_FUNC_UART1                                                */
/*                              E_FUNC_PWM01 / E_FUNC_PWM23 / E_FUNC_PWM45 / E_FUNC_PWM67                  */
/*                              E_FUNC_EBI_8B / E_FUNC_EBI_16B                                             */
/*                              E_FUNC_ICE                                                                 */
/* Returns:                                                                                                */
/*              E_SUCCESS										Operation successful                       */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                         */
/* Description:                                                                                            */
/*              Initialize the specified function and configure the relative pins             			   */
/*              for specified function used.               											       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_InitFunction(E_DRVGPIO_FUNC function)
{
	switch (function)
	{
		/*---------------------------------------------------------------------------------------------------------*/
		/* GPIO                                                                                                    */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_GPIO:
		{
		  	outpw(&SYS->P0_MFP, 0);
			outpw(&SYS->P1_MFP, 0);
			outpw(&SYS->P2_MFP, 0);
			outpw(&SYS->P3_MFP, 0);
			outpw(&SYS->P4_MFP, 0);
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* Clock Driver output                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_CLKO:
		{
            outpw(&SYS->P3_MFP, (inpw(&SYS->P3_MFP) | (0x1<<14)) & ~(0x1<<6));			
		}break;
		
		/*---------------------------------------------------------------------------------------------------------*/
		/* I2C                                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_I2C:
		{
            outpw(&SYS->P3_MFP, (inpw(&SYS->P3_MFP) | (0x3<<12)) & ~(0x3<<4));			
		}break;	
        	
		/*---------------------------------------------------------------------------------------------------------*/
		/* SPI                                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_SPI0:	
		{
            outpw(&SYS->P1_MFP, (inpw(&SYS->P1_MFP) | (0xF<<12)) & ~(0xF<<4));			
		}break;

		case E_FUNC_SPI1:
		{
            outpw(&SYS->P0_MFP, (inpw(&SYS->P0_MFP) | (0xF<<12)) & ~(0xF<<4));			
		}break;
		
		/*---------------------------------------------------------------------------------------------------------*/
		/* ADC                                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_ADC0:
		case E_FUNC_ADC1:
		case E_FUNC_ADC2:
		case E_FUNC_ADC3:
		case E_FUNC_ADC4:
		case E_FUNC_ADC5:
		case E_FUNC_ADC6:
		case E_FUNC_ADC7:
		{
            outpw(&SYS->P1_MFP, (inpw(&SYS->P1_MFP) & ~(0x1<<(function-E_FUNC_ADC0)+8)) | (0x1<<(function-E_FUNC_ADC0)));			
		}break;
		
  		/*---------------------------------------------------------------------------------------------------------*/
		/* External Interrupt                                                                                      */
		/*---------------------------------------------------------------------------------------------------------*/	
	  	case E_FUNC_EXTINT0:
		{	
            outpw(&SYS->P3_MFP, (inpw(&SYS->P3_MFP) & ~(0x1<<10)) | (0x1<<2));			
		}break;

	  	case E_FUNC_EXTINT1:
		{	
            outpw(&SYS->P3_MFP, (inpw(&SYS->P3_MFP) & ~(0x1<<11)) | (0x1<<3));			
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* TIMER                                                                                                   */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_TMR0:
		{
            outpw(&SYS->P3_MFP, (inpw(&SYS->P3_MFP) & ~(0x1<<12)) | (0x1<<4));			
		}break;
	
		case E_FUNC_TMR1:
		{
            outpw(&SYS->P3_MFP, (inpw(&SYS->P3_MFP) & ~(0x1<<13)) | (0x1<<5));			
		}break;

		case E_FUNC_TMR2:
		{
            outpw(&SYS->P1_MFP, (inpw(&SYS->P1_MFP) | (0x1<<8)) & ~(0x1<<0));			
		}break;

		case E_FUNC_TMR3:
		{
            outpw(&SYS->P1_MFP, (inpw(&SYS->P1_MFP) | (0x1<<9)) & ~(0x1<<1));			
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* UART                                                                                                    */
		/*---------------------------------------------------------------------------------------------------------*/	
		case E_FUNC_UART0:
		{
            outpw(&SYS->P3_MFP, (inpw(&SYS->P3_MFP) & ~(0x3<<8)) | (0x3));
            outpw(&SYS->P0_MFP, (inpw(&SYS->P0_MFP) | (0x3<<10)) & ~(0x3<<2));
 		}break;

		case E_FUNC_UART1:
		{
            outpw(&SYS->P1_MFP, (inpw(&SYS->P1_MFP) | (0x3<<10)) & ~(0x3<<2));
            outpw(&SYS->P0_MFP, (inpw(&SYS->P0_MFP) | (0x3<<8)) & ~(0x3<<0));
 		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* PWM                                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_PWM01:
		case E_FUNC_PWM23:
    	case E_FUNC_PWM45:
		case E_FUNC_PWM67:
		{
            outpw(&SYS->P2_MFP, (inpw(&SYS->P2_MFP) | (0x3<<((function-E_FUNC_PWM01)*2)+8)) & ~(0x3<<((function-E_FUNC_PWM01)*2)));

            if (function == E_FUNC_PWM01)
                outpw(&SYS->P4_MFP, (inpw(&SYS->P4_MFP) & ~(0x3<<8)) | (0x3<<0));
            else if (function == E_FUNC_PWM23)
                outpw(&SYS->P4_MFP, (inpw(&SYS->P4_MFP) & ~(0x3<<10)) | (0x3<<2));
		}break;

		/*---------------------------------------------------------------------------------------------------------*/
		/* EBI                                                                                                     */
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_EBI_16B:
        {
            outpw(&SYS->P2_MFP, (inpw(&SYS->P2_MFP) & ~(0xFF<<8)) | (0xFF<<0));			
        }
		case E_FUNC_EBI_8B:
        {
            outpw(&SYS->P0_MFP, (inpw(&SYS->P0_MFP) & ~(0xFF<<8)) | (0xFF<<0));			
			
            outpw(&SYS->P3_MFP, (inpw(&SYS->P3_MFP) | (0x1<<11)) & ~(0x1<<3));			
            outpw(&SYS->P3_MFP, (inpw(&SYS->P3_MFP) & ~(0x3<<14)) | (0x3<<6));			
            outpw(&SYS->P4_MFP, (inpw(&SYS->P4_MFP) & ~(0x3<<12)) | (0x3<<4));			
        }
		break;
		
		/*---------------------------------------------------------------------------------------------------------*/
		/* ICE                                                                                                     */			 
		/*---------------------------------------------------------------------------------------------------------*/
		case E_FUNC_ICE:
		{
            outpw(&SYS->P4_MFP, (inpw(&SYS->P4_MFP) & ~(0x3<<14)) | (0x3<<6));			
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


