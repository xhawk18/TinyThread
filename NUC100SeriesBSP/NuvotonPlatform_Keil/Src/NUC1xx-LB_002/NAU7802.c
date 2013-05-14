/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "Driver\DrvGPIO.h"
#include "Driver\DrvI2C.h"
#include "Driver\DrvSYS.h"
#include "NAU7802.h"


#define I2Cinterface	I2C_PORT0
#define I2CinterfaceIO	E_FUNC_I2C0		

uint8_t	Register_Addr;
uint8_t* pu8Data;
uint8_t MaxDataLen;
uint8_t DataLen;
volatile uint8_t EndFlag = 0;	

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C0 (Master) Callback Function								            	                                   */
/*---------------------------------------------------------------------------------------------------------*/
void I2C0_Callback_NAU7802(uint32_t status)
{
	switch(status)
	{
		case 0x08: 	   /* START has been transmitted and prepare SLA+W */
			DrvI2C_WriteData(I2Cinterface, 0x54);
			DrvI2C_Ctrl(I2Cinterface, 0, 0, 1, 0);
			DataLen=0;
			break;

		case 0x18: 	   	/* SLA+W has been transmitted and ACK has been received */
			DrvI2C_WriteData(I2Cinterface, Register_Addr);
			DrvI2C_Ctrl(I2Cinterface, 0, 0, 1, 0);
			break;

		case 0x20: 	   /* SLA+W has been transmitted and NACK has been received */
			DrvI2C_Ctrl(I2Cinterface, 1, 1, 1, 0);
			break;

		case 0x28: 	   	/* DATA has been transmitted and ACK has been received */
			if(EndFlag&0x80)
			{
				if (DataLen< MaxDataLen)
				{
					DrvI2C_WriteData(I2Cinterface, pu8Data[DataLen++]);
					DrvI2C_Ctrl(I2Cinterface, 0, 0, 1, 0);
				}
				else
				{
					DrvI2C_Ctrl(I2Cinterface, 0, 1, 1, 0);
					EndFlag = 0;
				}
			}
			else
			{
				DrvI2C_Ctrl(I2Cinterface, 1, 0, 1, 0);	
			}
			break;

		case 0x10: 	   	/* Repeat START has been transmitted and prepare SLA+R */
			DrvI2C_WriteData(I2Cinterface, 0x55);
			DrvI2C_Ctrl(I2Cinterface, 0, 0, 1, 0);
			break;

		case 0x40: 	   	/* SLA+R has been transmitted and ACK has been received */
			if(MaxDataLen>1)
				DrvI2C_Ctrl(I2Cinterface, 0, 0, 1, 1);
			else
		   		DrvI2C_Ctrl(I2Cinterface, 0, 0, 1, 0);
			break;

		case 0x50: 	   	/* DATA has been received and ACK has been returned */
			pu8Data[DataLen++] = DrvI2C_ReadData(I2Cinterface);
	 		if((DataLen+1)<MaxDataLen)
			{
				DrvI2C_Ctrl(I2Cinterface, 0, 0, 1, 1);
			}else
			{
				DrvI2C_Ctrl(I2Cinterface, 0, 0, 1, 0);
			}
			break;

		case 0x58: 	   	/* DATA has been received and NACK has been returned */
			pu8Data[DataLen] = DrvI2C_ReadData(I2Cinterface);
			DrvI2C_Ctrl(I2Cinterface, 0, 1, 1, 0);
			EndFlag = 0;
			break;

//		default:
				//printf("Status 0x%x is NOT processed\n", status);		
	}	
}
/*---------------------------------------------------------------------------------------------------------*/
/*  I2C0 (Master) Callback Function								            	                                   */
/*---------------------------------------------------------------------------------------------------------*/
void InitNAU7802(void)
{
	uint8_t Temp;
    /* Set I2C I/O */
    DrvGPIO_InitFunction(I2CinterfaceIO);
	/* Open I2C0 and I2C1, and set clock = 100Kbps */
	DrvI2C_Open(I2Cinterface, 400000);
	/* Get I2C0 clock */
	//printf("I2C0 clock %d Hz\n", DrvI2C_GetClockFreq(I2Cinterface));
	/* Enable I2C0 interrupt and set corresponding NVIC bit */
	DrvI2C_EnableInt(I2Cinterface);
	/* Install callback function for I2C0 interrupt */
	DrvI2C_InstallCallback(I2Cinterface, I2CFUNC, I2C0_Callback_NAU7802);

	/* Send Reset command into NAU7802 */
	wNAU7802_B(0x00,0x01);
	DrvSYS_Delay(2000);
	/* Enable PUD to power on the chip digital logic */
	wNAU7802_B(0x00,0x02);
	/* Wait power up ready */
	do
	{
		Temp=rNAU7802_B(0x00);		
	}while((Temp&(1<<3))==0);
	/* Enable PUA and AVDDS to support analog power, and enable CS to start ADC conversion */	 		  
	wNAU7802_B(0x00,0x96);
	/* Set the LDO output to 3.3V; PGA to 128x */
	wNAU7802_B(0x01,0x27);
	/* Turn off the chopper function */
	wNAU7802_B(0x15,0x30);
	/* Enable the Cfilter for high PGA gain (> 4) */
	wNAU7802_B(0x1C,0x80);
	/* Disable the Cfilter for low PGA gain  */
//	wNAU7802_B(0x1C,0x00);
	DrvSYS_Delay(2000);
	/* External Calibration: Force the current conversion data to be around zero*/
	wNAU7802_B(0x02,0x06);// Optional
	DrvSYS_Delay(200000);
}

void wNAU7802_B(uint8_t addr, uint8_t data)
{
	Register_Addr=addr;
	pu8Data=&data;
	MaxDataLen=1;
	EndFlag=0x81;
	DrvI2C_Ctrl(I2Cinterface, 1, 0, 0, 0);
	while (EndFlag);
}

uint8_t rNAU7802_B(uint8_t addr)
{
	uint8_t data=0;
	Register_Addr=addr;
	pu8Data=&data;
	MaxDataLen=1;
	EndFlag=0x01;
	DrvI2C_Ctrl(I2Cinterface, 1, 0, 0, 0);
	while (EndFlag);
	return data;
}

void wNAU7802(uint8_t addr, uint8_t* data, uint8_t lenght)
{
	Register_Addr=addr;
	pu8Data=data;
	MaxDataLen=lenght;
	EndFlag=0x81;
	DrvI2C_Ctrl(I2Cinterface, 1, 0, 0, 0);
	while (EndFlag);
}

void rNAU7802(uint8_t addr, uint8_t* data, uint8_t lenght)
{
	Register_Addr=addr;
	pu8Data=data;
	MaxDataLen=lenght;
	EndFlag=0x01;
	DrvI2C_Ctrl(I2Cinterface, 1, 0, 0, 0);
	while (EndFlag);
}

uint32_t GetADCResult_NAU7802(void)
{
	uint8_t u8ADCresult[3];
	uint32_t	u32ADCresult;

	do
	{
		u8ADCresult[0]=rNAU7802_B(0x00);
	}while((u8ADCresult[0]&(1<<5))==0);

	rNAU7802(0x12,u8ADCresult,3);
	u32ADCresult=(u8ADCresult[0]<<16)|(u8ADCresult[1]<<8)|u8ADCresult[2];
	if (u8ADCresult[0]>127)
	{
		u32ADCresult|=0xFF000000;
	}	
	return u32ADCresult;
	
}
