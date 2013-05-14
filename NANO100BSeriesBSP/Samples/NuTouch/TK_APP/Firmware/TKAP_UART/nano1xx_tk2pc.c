/******************************************************************************
 * @file     HIDTransfer_Main.c
 * @brief    Nano1xx Touch Key configuration sample code (UART interface)
 * @version  1.0.1
 * @date     04, December, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nano1xx.h"
#include "nano1xx_tk.h"
#include "nano1xx_gpio.h"
#include "nano1xx_timer.h"
#include "nano1xx_tk2pc.h"

	

#if 1

libtk_channel_config_s cfg[16] = {
        {0xa696, 0x0127, 7, 3},	  // 0
        {0xaad1, 0x00e6, 7, 3},	  // 1
        {0xaa51, 0x00d2, 7, 3},
        {0xa764, 0x00f7, 7, 3},	  // 3
        {0xa5a1, 0x00ce, 7, 3},
        {0xa231, 0x00f6, 7, 3},	  // 5
        {0x9d3e, 0x00e8, 7, 3},
        {0x9cdc, 0x011a, 6, 3},	  // 7
        {0xc18b, 0x0069, 7, 3},
        {0xa69f, 0x00cb, 7, 3},
        {0xaa0e, 0x00e8, 7, 3},
        {0xaa27, 0x00cd, 7, 3},
        {0xa1de, 0x00d3, 7, 3},
        {0xa6f3, 0x00ee, 7, 3},
        {0xab66, 0x00cf, 7, 3},
        {0xb3f6, 0x00d7, 5, 2},
};
#else
//extern libtk_channel_config_s cfg[16];
#endif

#if 1  //=======================Auto_config

rank base_rank[1];
rank diff_rank[1];

uint8_t volatile finger=0;
extern uint8_t volatile complete;

tk_sense_result result[12][15]; // clock div from 1~12, charge current from 1/15~15/15

best_cfg final_result;
const int8_t div_score[16] = {-20, -5, 3, 4, 4, 3, 2, 1, 1, 0, 0, 0, -1, -2, -3, -4};
const int8_t cur_score[16] = {-5, -3, 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1};
const int8_t lvl_score[16] = {-20, -20, -5, 5, 5, 5, 6, 6, 6, 6, 5, 5, 5, 5, -1, -5};
#endif
//=========================================
extern int kbhit(void);
extern char GetChar(void);

uint8_t MagicId[4]={'N','U','V','O'};
uint8_t MagicIdR[4]={'N','U','C','X'};


//Used by OP_LISTEN_STATUS
uint16_t  RecordChannel=0;
uint8_t   RecordCount=0;
LISTEN_STATUS Record;
//------------------------





//------------
extern uint16_t data[16];
extern void Get_TK_Data(uint16_t ch);
//------------




//UART2PC========================================
#define DEBUG_PORT 0
void SendChar_ToUART2(int ch)
{
	if(DEBUG_PORT == 1)
	{
	    while(UART1->FSR & UART_FSR_TX_FULL_F);
		UART1->THR = ch;
	}
	else if(DEBUG_PORT == 0)
	{
		while(UART0->FSR & UART_FSR_TX_FULL_F);
		UART0->THR = ch;
	}
	else
		;
}

void SendChars(uint8_t *buf,uint32_t len)
{
	int i=0;
  		for(i=0;i<len;i++)
			SendChar_ToUART2(buf[i]);
}

void AutoChannelConfig(uint8_t ch,best_cfg* final_result)
{
	uint16_t div, i;
	uint16_t current = 1;
	S_TK_CH_CFG ch_cfg = {1, 0, 0xFFFF, 0x0000};
	uint8_t sen_level, sen_state,tmp_div;
	uint16_t sen_data;	
	int8_t tmp_score;
	uint8_t tmp_cur;
	uint8_t progress=0;

	final_result->current = 1;
	final_result->div = 0;
	final_result->base = 0;
	final_result->diff = 0;

	base_rank[0].val = 0xFFFF;							
	base_rank[0].div = 0;
	base_rank[0].current = 1;
	diff_rank[0].val = 0;
	diff_rank[0].div = 0;
	diff_rank[0].current = 1;
	
			progress=20;
			SendChars((uint8_t *)&progress,sizeof(progress));

			for(div = 1; div < 12; div++) {			
				for(current = 1; current <= 15; current++) {
					ch_cfg.u8Level = current;
					ch_cfg.u8Div = div;
					TK_ConfigChannel(ch, &ch_cfg);		
					result[div][current - 1].level_off = 0;	
					result[div][current - 1].data_off = 0;					
		
					for(i = 0; i < 2; i++) {
						complete = 0;
						TK_Start(1 << ch);
						while(complete != 1);
						TK_ReadStatus(&sen_level, &sen_state, NULL);
						sen_data = TK_ReadData(ch);
						if(sen_state != 0 || sen_data == 0 || sen_data == 0xFFFF) {
							result[div][current - 1].level_off = 0;	
							result[div][current - 1].data_off = 0;
							break;
						} else {
							result[div][current - 1].level_off += sen_level;
							result[div][current - 1].data_off += (sen_data >> 1);						
						}									
					}
					result[div][current - 1].level_off >>= 1;
				}
			}
			
			
			progress=40;
			SendChars((uint8_t *)&progress,sizeof(progress));
	
		 	while(1)
			{	
				if(kbhit())												
					if(GetChar()==progress)															
						break;				
			} 	

			for(div = 1; div < 12; div++) {				
				for(current = 1; current <= 15; current++) {
					ch_cfg.u8Level = current;
					ch_cfg.u8Div = div;
					TK_ConfigChannel(ch, &ch_cfg);
		
					result[div][current - 1].level_on = 0;	
					result[div][current - 1].data_on = 0;					
					if(result[div][current - 1].level_off == 0)
						continue;
					for(i = 0; i < 2; i++) {
						complete = 0;
						TK_Start(1 << ch);
						while(complete != 1);
						TK_ReadStatus(&sen_level, &sen_state, NULL);
						sen_data = TK_ReadData(ch);
						if(sen_state != 0 || sen_data == 0 || sen_data == 0xFFFF) {
							result[div][current - 1].level_on = 0;	
							result[div][current - 1].data_on = 0;
							break;
						} else {
							result[div][current - 1].level_on += sen_level;
							result[div][current - 1].data_on += (sen_data >> 1);
						
						}									
					}
					result[div][current - 1].level_on >>= 1;
				}
			}


			// calculate sense level, timer divider, and change current score
			for(div = 1; div < 12; div++) {
				for(current = 1; current <= 15; current++) {

					result[div][current - 1].score = 0;

					if((result[div][current - 1].level_off != 0) &&
					   (result[div][current - 1].level_on != 0) &&	
					   (result[div][current - 1].data_on > result[div][current - 1].data_off)) {
						
						result[div][current - 1].score += (div_score[div] + cur_score[current] + lvl_score[result[div][current - 1].level_off]);
		
					}
				}
			}	
			
			// find out entry with highest diff
			for(div = 1; div < 12; div++) {
				for(current = 1; current <= 15; current++) {
					if(result[div][current - 1].score != 0) {
						if(((result[div][current - 1].data_on - result[div][current - 1].data_off) > diff_rank[0].val) &&
						   (result[div][current - 1].data_on > result[div][current - 1].data_off) &&
						   ((result[div][current - 1].data_on - result[div][current - 1].data_off) > 0x100)) {
							diff_rank[0].val = (result[div][current - 1].data_on - result[div][current - 1].data_off);
							diff_rank[0].current = current;
							diff_rank[0].div = div;
							break;
						}	
					}
				}
			}				

		   progress=60;
			SendChars((uint8_t *)&progress,sizeof(progress));

			// give score base on the differences
			for(div = 1; div < 12; div++) {
				for(current = 1; current <= 15; current++) {
					if(result[div][current - 1].score > 0) {											
						if(result[div][current - 1].data_on < result[div][current - 1].data_off)
							result[div][current - 1].score -=	100;
						else if((result[div][current - 1].data_on - result[div][current - 1].data_off) < 0x50)
							result[div][current - 1].score -=	100;	
						else
							result[div][current - 1].score += (8 - (diff_rank[0].val - (result[div][current - 1].data_on - result[div][current - 1].data_off)) / 0x800);
					}
				}
			}

			
			// find out lowest base
			for(div = 1; div < 12; div++) {
				for(current = 1; current <= 15; current++) {
					if(result[div][current - 1].score > 0) {
						if((result[div][current - 1].data_off < base_rank[0].val) && (result[div][current - 1].data_off < 0xF000)) {
							base_rank[0].val = result[div][current - 1].data_off;
							base_rank[0].current = current;
							base_rank[0].div = div;
							break;
						}
	
					}
				}
			}

			progress=80;
			SendChars((uint8_t *)&progress,sizeof(progress));

			// give score base on the differences
			for(div = 1; div < 12; div++) {
				for(current = 1; current <= 15; current++) {
					if(result[div][current - 1].score > 0) {											
							result[div][current - 1].score += (4 - (diff_rank[0].val - result[div][current - 1].data_off) / 0x1000);
					}
				}
			}

			// find the entry with highest score
			tmp_score = 0;
			tmp_cur = 1;  // eliminate compilation warning
			tmp_div = 0;						 // eliminate compilation warning
			for(div = 1; div < 12; div++) {
				for(current = 1; current <= 15; current++) {
					if(result[div][current - 1].score > tmp_score) {
						tmp_score = result[div][current - 1].score;
						tmp_div = div;
						tmp_cur = current;
							
					}
				}
			}

		   	progress=100;
			SendChars((uint8_t *)&progress,sizeof(progress));

			if(tmp_score == 0) {
				final_result->base=0;
				final_result->diff=0;
				final_result->current=0;
				final_result->div=0;
			} else {
				final_result->base = result[tmp_div][tmp_cur - 1].data_off;
				final_result->diff = result[tmp_div][tmp_cur - 1].data_on - result[tmp_div][tmp_cur - 1].data_off;
				final_result->current = tmp_cur;
				final_result->div = tmp_div;
			}
}

#define DELAY_TIME_OUT 0x20000
uint16_t reclen=0;
uint8_t ReceiveBuf[256];
uint8_t SendBuf[256];
uint8_t SendLen;
 void TK_Device2PC(void)
 {
    int count=0,i=0;
	uint16_t len=0;
	uint8_t test;		
	TK_CNANNEL_CONFIG tk_channel_config;
	TK_CNANNEL_CONFIG *tkchconfig;
	len=sizeof(TK_CNANNEL_CONFIG);	
	
	
	//get magic id
	i=0;
	count=0;
 	while(1)
	{	
		if(kbhit())	
		{		
			test=GetChar();
			if(test!=MagicId[i++])
			{										
				return;
			}
			if(i==sizeof(MagicId)) break;
		}
		else
		{			
			count++;			
		}
		if(count > DELAY_TIME_OUT) return;
	} 	
	
	i=0;
	count=0;
	while(i<2)
	{
	 	if(kbhit())	
		{
			((uint8_t *)&reclen)[i]=GetChar();
			i++;
		}


	}

	i=0;
	count=0;
	while(i<reclen)
	{
		 if(kbhit())	
		{
			ReceiveBuf[i]=GetChar();
			i++;
		}

	}

	SendChars(MagicIdR,sizeof(MagicIdR));

	count=0;
	switch(ReceiveBuf[0])
	{
		case OP_AUTO_CHANNEL_CONFIG:
		/*Send : MagicidR | len(16) | "START" | progess (5 time) |len<NEXT> (16) */
			len=5+sizeof(len);
				
			memcpy(SendBuf+count,&len,sizeof(len));
			count+=sizeof(len);
			memcpy(SendBuf+count,(uint8_t *)"START",5);
			count+=5;
			len=sizeof(TK_CNANNEL_CONFIG)+5 /*Used by Progress */;
			memcpy(SendBuf+count,&len,2);
			count+=sizeof(len);
			SendChars((uint8_t *)SendBuf,count);
												
			AutoChannelConfig(ReceiveBuf[1],&final_result);
			
			tk_channel_config.channel=ReceiveBuf[1];					
			tk_channel_config.base=final_result.base;
			tk_channel_config.diff=final_result.diff;
			tk_channel_config.current=final_result.current;
			tk_channel_config.div=final_result.div;	
							
			SendChars((uint8_t *)&tk_channel_config,sizeof(TK_CNANNEL_CONFIG));

		break;
		case OP_GET_CONFIG:				
			len=TK_CH_NUM*sizeof(TK_CNANNEL_CONFIG);			
			memcpy( SendBuf+count,(uint8_t *)&len,sizeof(len));		
		   	count+=sizeof(len);
			
			for(i=0;i<TK_CH_NUM;i++)			
			{			
				tk_channel_config.channel=i;					
				tk_channel_config.base=cfg[i].base;
				tk_channel_config.diff=cfg[i].diff;
				tk_channel_config.current=cfg[i].current;
				tk_channel_config.div=cfg[i].div;		
	
				memcpy(SendBuf+count,(uint8_t *)(&tk_channel_config),sizeof(TK_CNANNEL_CONFIG));
				count+=sizeof(TK_CNANNEL_CONFIG);
			}		
			 SendChars(SendBuf,count);

		break;


		case OP_SET_CHANNEL_CONFIG:
		 			
			tkchconfig=(TK_CNANNEL_CONFIG *)(ReceiveBuf+1);
											
			cfg[tkchconfig->channel].base=tkchconfig->base;
			cfg[tkchconfig->channel].diff=tkchconfig->diff;
			cfg[tkchconfig->channel].current=tkchconfig->current;
			cfg[tkchconfig->channel].div=tkchconfig->div;
			len=3;	
			SendBuf[0]='A';
			SendBuf[1]='C';
			SendBuf[2]='K';	
			SendChars((uint8_t *)&len,sizeof(len));					
			SendChars(SendBuf,3);
						
		break;

		case OP_LISTEN_STATUS:
		/*Recive :   magic_id(64) |  len(16) | OP_LISTEN_STATUS(8) | Channel(16)                          */ 		
		/*Send   :   magic_id(32) |  len(16) | struct LISTEN_STATUS(32) | ... | struct LISTEN_STATUS(32)  */
		 		  RecordCount=0;
				  memcpy((uint8_t *)&RecordChannel,(uint8_t *)(&ReceiveBuf[1]),2);
				  Get_TK_Data(RecordChannel);
				  for(i=0;i<MAX_CHANNEL;i++)
				  {
				  	if(RecordChannel & 0x0001)					
						RecordCount++;				
					RecordChannel=RecordChannel>>1;
				  }
				  len=sizeof(LISTEN_STATUS)*RecordCount;
				  SendChars((uint8_t *)&len,sizeof(len));

				  memcpy((uint8_t *)&RecordChannel,(uint8_t *)(&ReceiveBuf[1]),2);
				  for(i=0;i<MAX_CHANNEL;i++)
				  {
				  	if(RecordChannel & 0x0001)
					{
						Record.channel=i;
						Record.data=data[i];
					    SendChars((uint8_t *)&Record,sizeof(Record));
					}
					RecordChannel=RecordChannel>>1;
				  }				
		break;
		
		default:
		break;
	}
	
	return ;

 }
/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
