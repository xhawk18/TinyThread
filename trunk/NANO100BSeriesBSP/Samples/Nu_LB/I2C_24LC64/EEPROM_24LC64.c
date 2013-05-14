/******************************************************************************
 * @file     EEPROM_24LC64.c
 * @brief    24LC64 EEPROM library
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_sys.h"
#include "nano1xx_i2c.h"
#include "EEPROM_24LC64.h"

void Write_24LC64(uint32_t address,uint8_t data )
{
	//uint32_t i;
	SystemCoreClock = SYS_GetHCLKFreq(); 
  
	I2C_Init(I2C0);

	I2C_Open(I2C0, 100000);
	
   	//send i2c start
	I2C_Ctrl(I2C0, 1, 0, 0);	//set start
	I2C_Trig(I2C0);
	while(I2C_GetIntFlag(I2C0) == 0);
	I2C_ClearIntFlag(I2C0);
	
	//send writer command
	I2C0->DATA = 0xA0;					//send writer command
    I2C_Trig(I2C0);
    while(I2C_GetIntFlag(I2C0) == 0);
	I2C_ClearIntFlag(I2C0);
	
	//send address high
	I2C0->DATA = (address>>8)&0xFF;	
	I2C_Ctrl(I2C0, 0, 0, 1); 
	I2C_Trig(I2C0);
	while(I2C_GetIntFlag(I2C0) == 0);
	I2C_ClearIntFlag(I2C0);
	
	//send address low
	I2C0->DATA = address&0xFF;	
	I2C_Ctrl(I2C0, 0, 0, 1); 
	I2C_Trig(I2C0);
	while(I2C_GetIntFlag(I2C0) == 0);
	I2C_ClearIntFlag(I2C0);
		
	//send data
	I2C0->DATA = data;					//write data to 
	I2C_Ctrl(I2C0, 0, 0, 1);
	I2C_Trig(I2C0);
	while(I2C_GetIntFlag(I2C0) == 0);
   	I2C_ClearIntFlag(I2C0);
   	
   	//send i2c stop
	I2C_Ctrl(I2C0, 0, 1, 0); //send stop	
	I2C_Trig(I2C0);
		
	SYS_Delay(100);
	I2C_Close(I2C0);	
}

uint8_t Read_24LC64(uint32_t address)
{
	uint8_t TEMP;
  
	SystemCoreClock = SYS_GetHCLKFreq(); 
	
	I2C_Init(I2C0);

	I2C_Open(I2C0, 100000);
	
	//send i2c start
    I2C_Ctrl(I2C0, 1, 0, 0);	 	//set start
	I2C_Trig(I2C0);
	while(I2C_GetIntFlag(I2C0) == 0);
	I2C_ClearIntFlag(I2C0);
	 
   	//send writer command
	I2C0->DATA = 0XA0;
    I2C_Trig(I2C0);
    while(I2C_GetIntFlag(I2C0) == 0);
	I2C_ClearIntFlag(I2C0);
	
	//send address high
    I2C0->DATA = (address>>8)&0xFF;	
	I2C_Ctrl(I2C0, 0, 0, 1);    
	I2C_Trig(I2C0);
	while(I2C_GetIntFlag(I2C0) == 0);
    I2C_ClearIntFlag(I2C0);
    
	//send address low
	I2C0->DATA = address&0xFF;	
	I2C_Ctrl(I2C0, 0, 0, 1);    
	I2C_Trig(I2C0);
	while(I2C_GetIntFlag(I2C0) == 0);
	I2C_ClearIntFlag(I2C0);
	
	//send start flag
	I2C_Ctrl(I2C0, 1, 0, 0);    
	I2C_Trig(I2C0);
	while(I2C_GetIntFlag(I2C0) == 0);
	I2C_ClearIntFlag(I2C0);
	
	//send read command
	I2C0->DATA = 0xA1;
	I2C_Ctrl(I2C0, 0, 0, 1);    
	I2C_Trig(I2C0);
    while(I2C_GetIntFlag(I2C0) == 0);
	I2C_ClearIntFlag(I2C0);

	//resive data
	I2C0->DATA = 0xFF;	
	I2C_Trig(I2C0);
	while(I2C_GetIntFlag(I2C0) == 0);
	TEMP= I2C0->DATA;
	
	//send i2c stop
 	I2C_Ctrl(I2C0, 0, 1, 0);    
 	I2C_Trig(I2C0);
 	 
 	SYS_Delay(100); 	 	
	I2C_Close(I2C0);

	return TEMP; 
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
