/******************************************************************************
 * @file     ReadSim_CCID.c
 * @brief    Nano1xx SIM card reader and CCIS sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/  
#include <stdio.h>
#include <string.h>
#include "nano1xx.h"
#include "nano1xx_sys.h"
#include "nano1xx_ebi.h"
#include "nano1xx_gpio.h"
#include "nano1xx_smartcard.h"
#include "EBI_LCM_OTM3225A.h"
#include "assert.h"


extern void EBI_LCM_Initial(void);
extern int32_t CCID_MainProcess(void);

extern volatile S_SC_DEV_T sc_device[SMARTCARD_NUM];
volatile uint8_t g_gpioINTChange = 0;


#define CARD		SC1

uint8_t g_contactfield_len=0x00;
uint8_t name1[15], name2[15], name3[15];
uint8_t phone1[21], phone2[21], phone3[21];


void Delay(uint32_t delayCnt)
{
	while(delayCnt--)
	{
		__NOP();
		__NOP();
	}
}

uint8_t INTtoCHAR(int num)
{
	switch(num)
	{
		case 0:
			return '0';
		case 1:
			return '1';
		case 2:
			return '2';
		case 3:
			return '3';
		case 4:
			return '4';
		case 5:
			return '5';
		case 6:
			return '6';
		case 7:
			return '7';
		case 8:
			return '8';
		case 9:
			return '9';
		case 10:
			return 'A';
		case 11:
			return 'B';
		case 12:
			return 'C';
		case 13:
			return 'D';
		case 14:
			return 'E';
		case 15:
			return 'F';
	}
	
	return '-';
}

int32_t CheckCardType(void)
{
	int ret;
	uint32_t recvlength;
	uint8_t recvScard[277];		// indicate SW1, SW2
	
	uint8_t cmd3f00[] = {0xA0, 0xA4, 0x00, 0x00, 0x02, 0x3F, 0x00};
	uint8_t cmd7f31[] = {0xA0, 0xA4, 0x00, 0x00, 0x02, 0x7F, 0x31};
	uint8_t cmd7f10[] = {0xA0, 0xA4, 0x00, 0x00, 0x02, 0x7F, 0x10};
	uint8_t cmd6f3a[] = {0xA0, 0xA4, 0x00, 0x00, 0x02, 0x6F, 0x3A};

	uint8_t cmdgetpacketlength[] = {0xA0, 0xC0, 0x00, 0x00, 0x0F};

	memset(recvScard, 0x00, sizeof(recvScard));

	//if(SMARTCARD_ColdReset((S_SC_DEV_T *)&sc_device[1])<0) {
	SMARTCARD_Close(CARD);
	Delay(0x100);
	if(SMARTCARD_Open(CARD)<0) {
		Show_String32x32(1, 1, "Unknown Card", 0xf800);
		return -1;
	}
	
	ret = SMARTCARD_Write(CARD, cmd3f00, sizeof(cmd3f00), &recvScard[0], &recvlength);
	ret = SMARTCARD_Write(CARD, cmd7f31, sizeof(cmd7f31), &recvScard[0], &recvlength);
	ret = SMARTCARD_Write(CARD, cmd3f00, sizeof(cmd3f00), &recvScard[0], &recvlength);
	ret = SMARTCARD_Write(CARD, cmd7f10, sizeof(cmd7f10), &recvScard[0], &recvlength);
	ret = SMARTCARD_Write(CARD, cmd6f3a, sizeof(cmd6f3a), &recvScard[0], &recvlength);
	
	// read contact field length
	ret = SMARTCARD_Write(CARD, cmdgetpacketlength, sizeof(cmdgetpacketlength), &recvScard[0], &recvlength);
	if ( ret < 0 )
	{
		Show_String32x32(1, 1, "Unknown Card", 0xf800);
		printf("Failed SMARTCARD_Write ErrMsg:%s , code=%d\n", SMARTCARD_ERRMSG(ret), ret);
		return -1;
	} else {
		g_contactfield_len = recvScard[14];
		//printf("g_contactfield_len = 0x%x\n", g_contactfield_len);
		if(g_contactfield_len!=0x1C && g_contactfield_len!=0x1A) {
			Show_String32x32(1, 1, "Unknown Card", 0xf800);
			return -1;
		}
	}
	return 0;
}

void ReadContact(void)
{
	int i, j, MaxName=0, rContactDataLen=0;
	uint8_t recvScard[277];		// indicate SW1, SW2
	uint32_t recvlength;
	
	uint8_t cmd3f00[] = {0xA0, 0xA4, 0x00, 0x00, 0x02, 0x3F, 0x00};
	uint8_t cmd7f31[] = {0xA0, 0xA4, 0x00, 0x00, 0x02, 0x7F, 0x31};
	uint8_t cmd7f10[] = {0xA0, 0xA4, 0x00, 0x00, 0x02, 0x7F, 0x10};
	uint8_t cmd6f3a[] = {0xA0, 0xA4, 0x00, 0x00, 0x02, 0x6F, 0x3A};

	uint8_t *rContactData = NULL;
	uint8_t rContactData1C[] = {0xA0, 0xB2, 0x01, 0x04, 0x1C};
	uint8_t rContactData1A[] = {0xA0, 0xB2, 0x01, 0x04, 0x1A};

	if(g_contactfield_len == 0x1C) 
	{
		rContactData = rContactData1C;
		rContactDataLen = sizeof(rContactData1C);
		MaxName = 14;
	}
	else if(g_contactfield_len == 0x1A) 
	{
		rContactData = rContactData1A;
		rContactDataLen = sizeof(rContactData1A);
		MaxName = 12;
	}
	
	SMARTCARD_Write(CARD, cmd3f00, sizeof(cmd3f00), &recvScard[0], &recvlength);
	SMARTCARD_Write(CARD, cmd7f31, sizeof(cmd7f31), &recvScard[0], &recvlength);
	SMARTCARD_Write(CARD, cmd3f00, sizeof(cmd3f00), &recvScard[0], &recvlength);
	SMARTCARD_Write(CARD, cmd7f10, sizeof(cmd7f10), &recvScard[0], &recvlength);
	SMARTCARD_Write(CARD, cmd3f00, sizeof(cmd3f00), &recvScard[0], &recvlength);
	SMARTCARD_Write(CARD, cmd7f31, sizeof(cmd7f31), &recvScard[0], &recvlength);
	SMARTCARD_Write(CARD, cmd3f00, sizeof(cmd3f00), &recvScard[0], &recvlength);
	SMARTCARD_Write(CARD, cmd7f10, sizeof(cmd7f10), &recvScard[0], &recvlength);
	SMARTCARD_Write(CARD, cmd6f3a, sizeof(cmd6f3a), &recvScard[0], &recvlength);

	// read
	// 1
	SMARTCARD_Write(CARD, rContactData, rContactDataLen, &recvScard[0], &recvlength);
	for(i=0; i<MaxName; i++)
	{
		if(recvScard[i] != 0xFF) {
			name1[i] = recvScard[i];
		} else break;
	}
	name1[i] = '\0';
	for(i=1, j=0; i<recvScard[MaxName]; i++)
	{
		phone1[j++] = INTtoCHAR(recvScard[MaxName+1+i]&0xF);
		if( (i!=recvScard[MaxName]) && ((recvScard[MaxName+1+i]&0xF0)!=0xF0) )
			phone1[j++] = INTtoCHAR((recvScard[MaxName+1+i]&0xF0)>>4);
		else
			break;
	}
	phone1[j] = '\0';

	// 2
	recvlength = sizeof(recvScard);
	rContactData[2] = 0x2;
	SMARTCARD_Write(CARD, rContactData, rContactDataLen, &recvScard[0], &recvlength);
	for(i=0; i<MaxName; i++)
	{
		if(recvScard[i] != 0xFF) {
			name2[i] = recvScard[i];
		} else break;
	}
	name2[i] = '\0';
	for(i=1, j=0; i<recvScard[MaxName]; i++)
	{
		phone2[j++] = INTtoCHAR(recvScard[MaxName+1+i]&0xF);
		if( (i!=recvScard[MaxName]) && ((recvScard[MaxName+1+i]&0xF0)!=0xF0) )
			phone2[j++] = INTtoCHAR((recvScard[MaxName+1+i]&0xF0)>>4);
		else
			break;
	}
	phone2[j] = '\0';

	// 3
	recvlength = sizeof(recvScard);
	rContactData[2] = 0x3;
	SMARTCARD_Write(CARD, rContactData, rContactDataLen, &recvScard[0], &recvlength);
	for(i=0; i<MaxName; i++)
	{
		if(recvScard[i] != 0xFF) {
			name3[i] = recvScard[i];
		} else break;
	}
	name3[i] = '\0';
	for(i=1, j=0; i<recvScard[MaxName]; i++)
	{
		phone3[j++] = INTtoCHAR(recvScard[MaxName+1+i]&0xF);
		if( (i!=recvScard[MaxName]) && ((recvScard[MaxName+1+i]&0xF0)!=0xF0) )
			phone3[j++] = INTtoCHAR((recvScard[MaxName+1+i]&0xF0)>>4);
		else
			break;
	}
	phone3[j] = '\0';

}


void showSimCardInfo(void)
{
	bool bLoop = true;
	int32_t C1_state=0, open1=0, clearflag=0;
	int32_t C1_flag=0;

	if(SCARD_PRESENT == SMARTCARD_CheckCardPresent(CARD)) {
		open1 = 1;
		C1_state = 1;
	}		

	while ( bLoop )
	{
		if(open1 == 1)
		{
			clearflag = 0;
			
			/* check card type */
			if(CheckCardType() == 0)
			{
				/* Read the Contacts */
				ReadContact();
				
				/* Show on the LCD */
				Show_String32x32(1, 1, "Name:", 0xf800);
				Show_String32x32(110, 1, name1, 0xf800);
				Show_String32x32(1, 33, "No:", 0xf800);
				Show_String32x32(60, 33, phone1, 0xf800);
				Show_String32x32(1, 65, "Name:", 0x07e0);
				Show_String32x32(110, 65, name2, 0x07e0);
				Show_String32x32(1, 97, "No:", 0x07e0);
				Show_String32x32(60, 97, phone2, 0x07e0);
				Show_String32x32(1, 129, "Name:", 0x001f);
				Show_String32x32(110, 129, name3, 0x001f);
				Show_String32x32(1, 161, "No:", 0x001f);
				Show_String32x32(60, 161, phone3, 0x001f);
			}
		}

		open1 = 0;		

		while(1) 
		{			
			if((SCARD_ABSENT==SMARTCARD_CheckCardPresent(CARD)) && (C1_state==1))
			{
				if(clearflag==0) { ClearScreen(0x0000); clearflag=1; }
				
				C1_flag = 1;	// insert -> remove
				Show_String32x32(1, 1, "Card Removal", 0xf800);
			}
			else if((SCARD_PRESENT==SMARTCARD_CheckCardPresent(CARD)) && (C1_state==1) && (C1_flag==1))
			{
				if(clearflag==0) { ClearScreen(0x0000); clearflag=1; }
				C1_flag = 2;	// remove -> insert
			}
			else if((SCARD_ABSENT==SMARTCARD_CheckCardPresent(CARD)) && (C1_state==0))
			{
				if(clearflag==0) { ClearScreen(0x0000); clearflag=1; }
				C1_state = 0;
				Show_String32x32(1, 1, "No Card", 0xf800);
			}
			else if((SCARD_PRESENT==SMARTCARD_CheckCardPresent(CARD)) && (C1_state==0)) {
				if(clearflag==0) { ClearScreen(0x0000); clearflag=1; }
				
				C1_flag = 2;	// card present
				C1_state = 1;
			}

			if(g_gpioINTChange==0) {
				ClearScreen(0x0000);
				//printf("exit sim card reader mode \n");
				return ;
			}
			
			if(C1_flag == 2) {
				C1_flag = 0;
				open1 = 1;
				break;
			}
		}		

		/* clear screen */
		ClearScreen(0x0000);

	}
	
}

int main (void)
{
	uint16_t regcontent;
	volatile uint32_t loop, xaxis, yaxis;

	SYS_SetClockDivider_0(CLK_CLKDIV0_USB_MASK, USB_CLK_DIVIDER(1));

	// SP1 button triggers PB[15]
	MFP_EXT_INT1_TO_PB15();
	GPIO_EnableEINT1(GPIOB, 15, GPIO_IER_IF_EN_15, GPIO_IMD_EDGE_15);	

	printf("\n\n");
	printf("+-------------------------------------------------------------+\n");
	printf("|               EBI LCD & Smart Card Sample Code              |\n");
	printf("+-------------------------------------------------------------+\n");
	printf("\n");
	
	/* initailize EBI and pins */
	EBI_LCM_Initial();
	
	// read something from LCD driver
	OTM3225A_IDRead(ID_READ_ADDR, &regcontent);
	printf("ID = 0x%x \n", regcontent);

	/* panel initialization */
	OTM3225_init(RES_LANDSCAPE_320X240);
	ClearScreen(0x0000);

	/* Select SC Clock Source From 12MHz */
	CLK->CLKSEL2 = (CLK->CLKSEL2 & ~CLK_CLKSEL2_SC_MASK) | CLK_CLKSEL2_SC_HXT;

	/* Set SC Clock divider which divide by 3 to 4Mhz be used */
	CLK->CLKDIV1 = (CLK->CLKDIV1 & ~CLK_CLKDIV1_SC1_MASK) | 2;
	MFP_SC1_TO_PORTD();
	
	/* SmartCard 1 Initialization */
	SMARTCARD_Init(CARD, 1);
	SMARTCARD_PowerActiveLevel(CARD, 1);
	SMARTCARD_Open(CARD);


	while(1)
	{
		if(g_gpioINTChange==0) {
			/* Execute CCID process */
			printf("CCID mode\n");
			CCID_MainProcess();
		}
		else {
			/* show Sim Card Contact information */
			printf("Read SIM mode\n");
			showSimCardInfo();
		}		
	}

}

#ifdef USE_ASSERT
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     assert_error                                                                              */
/*                                                                                                         */
/* Parameter:                                                                                              */   
/*               file           -[in] source file name                                                     */
/*               line           -[in] line number                                                          */
/* Returns:                                                                                                */
/*               None.                                                                                     */
/* Description:                                                                                            */
/*               The function prints the source file name and line number where the assert_param() error   */
/*               occurs, and then stops in an infinite loop.                                               */
/*               User can add his own codes here if necessary.                                             */
/*---------------------------------------------------------------------------------------------------------*/

void assert_error(uint8_t * file, uint32_t line)
{
	printf("[%s] line %d : wrong parameters.\r\n", file, line);

	/* Infinite loop */
	while(1) ;

}
#endif


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


	 
