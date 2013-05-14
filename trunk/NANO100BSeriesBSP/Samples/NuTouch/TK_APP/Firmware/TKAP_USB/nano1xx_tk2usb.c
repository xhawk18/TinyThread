/******************************************************************************
 * @file     HIDTransfer_Main.c
 * @brief    Nano1xx Touch Key configuration sample code (USB interface)
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
#include "nano1xx_tk2usb.h"
#include "nano1xx_tk.h"
#include "nano1xx_sys.h"

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

//=======================Auto_config=======
#if 1
rank base_rank[1];
rank diff_rank[1];

uint8_t volatile finger=0;


tk_sense_result result[12][15]; // clock div from 1~12, charge current from 1/15~15/15

best_cfg final_result;
const int8_t div_score[16] = {-20, -5, 3, 4, 4, 3, 2, 1, 1, 0, 0, 0, -1, -2, -3, -4};
const int8_t cur_score[16] = {-5, -3, 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1};
const int8_t lvl_score[16] = {-20, -20, -5, 5, 5, 5, 6, 6, 6, 6, 5, 5, 5, 5, -1, -5};
#endif



uint16_t data[16];
uint16_t f, t;
uint8_t volatile complete = 0;
uint8_t set = 0;  // set 0 = ch0~7, set 1 = ch8~15
uint8_t AutoType=0;	 //0 => up key, 1=> down key, 2=> tarin key
uint8_t AutoCh=0;
uint8_t AutoProgress=0;
void AutoChannelConfigUp(void)
{
        uint16_t div, i;
        uint16_t current = 1;
        S_TK_CH_CFG ch_cfg = {1, 0, 0xFFFF, 0x0000};
        uint16_t sen_data;
        uint8_t sen_level, sen_state;
        AutoProgress=10;
        for(div = 1; div < 12; div++) {
                for(current = 1; current <= 15; current++) {
                        ch_cfg.u8Level = current;
                        ch_cfg.u8Div = div;
                        TK_ConfigChannel(AutoCh, &ch_cfg);
                        result[div][current - 1].level_off = 0;
                        result[div][current - 1].data_off = 0;

                        for(i = 0; i < 2; i++) {
                                complete = 0;
                                TK_Start(1 << AutoCh);
                                while(complete != 1);
                                TK_ReadStatus(&sen_level, &sen_state, NULL);
                                sen_data = TK_ReadData(AutoCh);
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
        AutoProgress=30;
}

void AutoChannelConfigDown()
{
        uint16_t div, i;
        uint16_t current = 1;
        S_TK_CH_CFG ch_cfg = {1, 0, 0xFFFF, 0x0000};
        uint16_t sen_data;
        uint8_t sen_level, sen_state;
        AutoProgress=40;
        for(div = 1; div < 12; div++) {
                for(current = 1; current <= 15; current++) {
                        ch_cfg.u8Level = current;
                        ch_cfg.u8Div = div;
                        TK_ConfigChannel(AutoCh, &ch_cfg);

                        result[div][current - 1].level_on = 0;
                        result[div][current - 1].data_on = 0;
                        if(result[div][current - 1].level_off == 0)
                                continue;
                        for(i = 0; i < 2; i++) {
                                complete = 0;
                                TK_Start(1 << AutoCh);
                                while(complete != 1);
                                TK_ReadStatus(&sen_level, &sen_state, NULL);
                                sen_data = TK_ReadData(AutoCh);
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
        AutoProgress=60;
}

void AutoChannelConfigTrain()
{
        uint16_t div;
        uint16_t current = 1;
        uint8_t tmp_div;

        int8_t tmp_score;
        uint8_t tmp_cur;


        final_result.current = 1;
        final_result.div = 0;
        final_result.base = 0;
        final_result.diff = 0;

        base_rank[0].val = 0xFFFF;
        base_rank[0].div = 0;
        base_rank[0].current = 1;
        diff_rank[0].val = 0;
        diff_rank[0].div = 0;
        diff_rank[0].current = 1;


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

        AutoProgress=70;
        //SendChars((uint8_t *)&progress,sizeof(progress));

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

        AutoProgress=80;
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

        AutoProgress=90;
        //SendChars((uint8_t *)&progress,sizeof(progress));

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

        AutoProgress=100;
        //SendChars((uint8_t *)&progress,sizeof(progress));

        if(tmp_score == 0) {
                final_result.base=0;
                final_result.diff=0;
                final_result.current=0;
                final_result.div=0;
        } else {
                final_result.base = result[tmp_div][tmp_cur - 1].data_off;
                final_result.diff = result[tmp_div][tmp_cur - 1].data_on - result[tmp_div][tmp_cur - 1].data_off;
                final_result.current = tmp_cur;
                final_result.div = tmp_div;
        }
}

void TK_GetConfig(uint8_t *SendBuf,uint32_t *SendLen,uint8_t set)
{
        uint8_t i;
        uint32_t count=0;
        TK_CNANNEL_CONFIG tk_channel_config;

        for(i=0+set*8; i<8+set*8; i++) {
                tk_channel_config.channel=i;
                tk_channel_config.base=cfg[i].base;
                tk_channel_config.diff=cfg[i].diff;
                tk_channel_config.current=cfg[i].current;
                tk_channel_config.div=cfg[i].div;

                memcpy(SendBuf+count,(uint8_t *)(&tk_channel_config),sizeof(TK_CNANNEL_CONFIG));
                count+=sizeof(TK_CNANNEL_CONFIG);
        }

        *SendLen=count;
}

void TK_SetConfig(uint8_t *ReceiveBuf,uint32_t ReceiveLen)
{
        TK_CNANNEL_CONFIG *tkchconfig;
        tkchconfig=(TK_CNANNEL_CONFIG *)(ReceiveBuf);

        cfg[tkchconfig->channel].base=tkchconfig->base;
        cfg[tkchconfig->channel].diff=tkchconfig->diff;
        cfg[tkchconfig->channel].current=tkchconfig->current;
        cfg[tkchconfig->channel].div=tkchconfig->div;
}

void Get_TK_Data(uint16_t ch)
{
	uint8_t i;
	S_TK_CH_CFG ch_cfg = {8, 2, 0xFFFF, 0x0000};
	if(ch & 0xFF)
	{
		complete=0;
		set = 0;
	    for(i = 0; i < 8; i++) {    
			ch_cfg.u8Level=cfg[i].current;
			ch_cfg.u8Div=cfg[i].div;
			TK_ConfigChannel(i, &ch_cfg);
	    }
		TK_Start(ch & 0xFF);					
		while(complete == 0);
	}

	if(ch & 0xFF00)
	{
		complete = 0;
		set = 1;
		for(i = 8; i < 16; i++) {
			ch_cfg.u8Level=cfg[i].current;
			ch_cfg.u8Div=cfg[i].div;
			TK_ConfigChannel(i, &ch_cfg);
		}
		TK_Start(ch & 0xFF00);					
		while(complete == 0);
	}	
}

void TK_ListenStatus(uint32_t CNum,uint8_t *SendBuf,uint32_t *SendLen)
{
        volatile LISTEN_STATUS *plisten;
        int count=0;
        int i;
        plisten=(LISTEN_STATUS *)SendBuf;
		Get_TK_Data((uint16_t)CNum);
        for(i=0; i<MAX_CHANNEL; i++) {
                if(CNum & 0x1) {
                        printf("i=%d\n",i);
                        plisten->channel=i;
                        plisten->data=i;
                        plisten->data=data[i];
                        count+=sizeof(LISTEN_STATUS);
                        plisten++;
                }
                CNum=CNum>>1;
        }
        *SendLen=count;
}

#define CLS		do {printf("%c[2J", 0x1B); printf("%c[%d;1H", 0x1B, 0);}while(0)
void env_test(void)
{
  
#if 1
        GCR->PA_L_MFP = (GCR->PA_L_MFP & ~(PA0_MFP_MASK | PA1_MFP_MASK)) | PA0_MFP_TK8 | PA1_MFP_TK9;		// TK8, 9
        GCR->PA_H_MFP = (GCR->PA_H_MFP & ~(PA12_MFP_MASK | PA13_MFP_MASK)) | PA12_MFP_TK10 | PA13_MFP_TK11;		// TK10, 11
        GCR->PC_H_MFP = (GCR->PC_H_MFP & ~(PC8_MFP_MASK | PC9_MFP_MASK | PC10_MFP_MASK | PC11_MFP_MASK)) |
                        PC8_MFP_TK12 | PC9_MFP_TK13 | PC10_MFP_TK14 | PC11_MFP_TK15;		// 12~15
        GCR->PD_L_MFP = (GCR->PD_L_MFP & ~(PD0_MFP_MASK | PD1_MFP_MASK | PD2_MFP_MASK | PD3_MFP_MASK | PD4_MFP_MASK | PD5_MFP_MASK)) |
                        PD0_MFP_TK0 | PD1_MFP_TK1 | PD2_MFP_TK2 | PD3_MFP_TK3 | PD4_MFP_TK4 | PD5_MFP_TK5;    // 0~5
        GCR->PF_L_MFP = (GCR->PF_L_MFP & ~(PF4_MFP_MASK | PF5_MFP_MASK)) | PF4_MFP_TK6 | PF5_MFP_TK7;    // 6, 7

        TK_Init(TK_CTL1_SEN_SEL_16BIT);
#endif
        TK_EnableInterrupt(TK_INT_ALL);
        
        CLS;
        while(1) {

                switch(AutoType) { /*1 => up key, 2=> down key, 3=> tarin key 4=> return busy or idle( 0xff => busy, 0x0=>idle) */
                case 1:
                        AutoType=0xff;
                        printf("AutoChannelConfigUp AutoCh=%d\n",AutoCh);
                        AutoChannelConfigUp();
                        AutoType=0;
                        break;

                case 2:
                        AutoType=0xff;
                        printf("AutoChannelConfigDown AutoCh=%d\n",AutoCh);
                        AutoChannelConfigDown();
                        printf("AutoChannelConfigTrain AutoCh=%d\n",AutoCh);
                        AutoChannelConfigTrain();
                        AutoType=0;
                        break;

                default:
                        break;
                }


        }
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
