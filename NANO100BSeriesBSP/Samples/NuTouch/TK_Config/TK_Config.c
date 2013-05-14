/******************************************************************************
 * @file     TK_Config.c
 * @brief    Touch key sample code used to generate configuration table.
 * @version  1.0.1
 * @date     02, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nano1xx.h"
#include "nano1xx_tk.h"
#include "nano1xx_sys.h"

typedef struct {
	uint16_t level_off;
	uint16_t data_off;
	uint16_t level_on;
	uint16_t data_on;
	int8_t score;
}tk_sense_result;

tk_sense_result result[12][15]; // clock div from 1~12, charge current from 1/15~15/15

uint8_t volatile complete = 0;


typedef struct {
	uint8_t current;
	uint8_t div;
	uint16_t base;
	uint16_t diff;		
}best_cfg;

typedef struct {
	uint8_t current;
	uint16_t div;
	uint16_t val;
}rank;

rank base_rank[1];
rank diff_rank[1];

best_cfg final_result[16];

const int8_t div_score[16] = {-20, -5, 3, 4, 4, 3, 2, 1, 1, 0, 0, 0, -1, -2, -3, -4};
const int8_t cur_score[16] = {-5, -3, 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1};
const int8_t lvl_score[16] = {-20, -20, -5, 5, 5, 5, 6, 6, 6, 6, 5, 5, 5, 5, -1, -5};


void tk_config(void)
{
	uint16_t ch = 0, div, i, mask;
	uint16_t current = 1;

	S_TK_CH_CFG ch_cfg = {1, 0, 0xFFFF, 0x0000};
	uint8_t sen_level, sen_state, c, tmp_div;
	uint16_t sen_data;	
	int8_t tmp_score;
	uint8_t tmp_cur;


	printf("\n\n\n");

	for(i = 0; i < 16; i++) {
		final_result[i].current = 1;
		final_result[i].div = 0;
		final_result[i].base = 0;
		final_result[i].diff = 0;

	}

	while((UART0->FSR & UART_FSR_RX_EMPTY_F) == 0) {
		getchar();
	}

again:	

	printf("\nPlese select test channel mask 0001~ffff:\n");
	mask = 0;
	for(i = 0; i < 4; i++) {
		while(1) {	
			c = getchar();
			if(c >= 0x30 && c <= 0x39) {
				c -= 0x30;
				break;
			}
			if(c >= 0x61 && c <= 0x66) {
				c -= 0x57;
				break;
			}
			if(c >= 0x41 && c <= 0x46) {
				c -= 0x37;
				break;
			}

		}
		mask = (mask << 4) + c;
	}

	if(mask == 0)
		goto again;

	GCR->PA_L_MFP = (GCR->PA_L_MFP & ~(PA0_MFP_MASK | PA1_MFP_MASK)) | PA0_MFP_TK8 | PA1_MFP_TK9;		// TK8, 9
	GCR->PA_H_MFP = (GCR->PA_H_MFP & ~(PA12_MFP_MASK | PA13_MFP_MASK)) | PA12_MFP_TK10 | PA13_MFP_TK11;		// TK10, 11
	GCR->PC_H_MFP = (GCR->PC_H_MFP & ~(PC8_MFP_MASK | PC9_MFP_MASK | PC10_MFP_MASK | PC11_MFP_MASK)) |
					PC8_MFP_TK12 | PC9_MFP_TK13 | PC10_MFP_TK14 | PC11_MFP_TK15;		// 12~15
	GCR->PD_L_MFP = (GCR->PD_L_MFP & ~(PD0_MFP_MASK | PD1_MFP_MASK | PD2_MFP_MASK | PD3_MFP_MASK | PD4_MFP_MASK | PD5_MFP_MASK)) |
					PD0_MFP_TK0 | PD1_MFP_TK1 | PD2_MFP_TK2 | PD3_MFP_TK3 | PD4_MFP_TK4 | PD5_MFP_TK5;    // 0~5
	GCR->PF_L_MFP = (GCR->PF_L_MFP & ~(PF4_MFP_MASK | PF5_MFP_MASK)) | PF4_MFP_TK6 | PF5_MFP_TK7;    // 6, 7

        TK_Init(TK_CTL1_SEN_SEL_16BIT);

	TK_EnableInterrupt(TK_INTEN_SCAN_COMPLETE);


	for(ch = 0; ch < 16; ch++) {

		if(mask & (1 << ch)) {

			base_rank[0].val = 0xFFFF;							
			base_rank[0].div = 0;
			base_rank[0].current = 1;
			diff_rank[0].val = 0;
			diff_rank[0].div = 0;
			diff_rank[0].current = 1;


			while((UART0->FSR & UART_FSR_RX_EMPTY_F) == 0) {
				getchar();
			}
			printf("########## Detect channel %d configuration ##########\n", ch);
			printf("Keep finger away from channel %d then press Enter", ch);
			getchar();

			for(div = 1; div < 12; div++) {
				printf(".");
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
		
			while((UART0->FSR & UART_FSR_RX_EMPTY_F) == 0) {
				getchar();
			}
	
			printf("Done\nPut finger on channel %d then press Enter", ch);
			getchar();
		
			for(div = 1; div < 12; div++) {
				printf(".");
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
			printf("Done\n");
	
			
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

			if(tmp_score == 0) {
				printf("Configuration detect failed...\n");
			} else {
				final_result[ch].base = result[tmp_div][tmp_cur - 1].data_off;
				final_result[ch].diff = result[tmp_div][tmp_cur - 1].data_on - result[tmp_div][tmp_cur - 1].data_off;
				final_result[ch].current = tmp_cur;
				final_result[ch].div = tmp_div;

			}

		}
	}

	TK_DeInit();

	printf("Done\nOutput configuration array, please include this array in your project.\n\n");
	// printf out the table.

	printf("libtk_channel_config_s cfg[16] = {\n");
	for(i = 0; i < 16; i++) {
		printf("\t{0x%04x, 0x%04x, %d, %d},\n", final_result[i].base,
															      final_result[i].diff >> 2,
																  (uint32_t)final_result[i].current,
																  final_result[i].div);
	}
	printf("};\n");


	return;
}


int32_t main(void)
{

	tk_config();

	printf("\n\nAdios!!\n");
	return(0);
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
