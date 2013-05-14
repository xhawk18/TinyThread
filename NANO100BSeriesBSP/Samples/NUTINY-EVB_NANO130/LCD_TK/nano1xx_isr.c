/******************************************************************************
 * @file     nano1xx_isr.c
 * @brief    This file contains interrupt service routines
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <string.h>
#include "nano1xx.h"
#include "nano1xx_tk.h"
#include "nano1xx_lcd.h"
#include "libtk.h"

/******************************************************************************/
/*       DO _NOT_ EDIT IRQ HANDLERS UNLESS YOU KNOW WHAT YOU ARE DOING        */
/******************************************************************************/

extern uint8_t volatile complete;
extern uint8_t tk_current_index;
extern uint16_t tk_channel_mask;
extern tk_channel channel[TK_CH_NUM];
extern uint8_t tk_index[TK_CH_NUM];
extern uint8_t tk_calibrating;

void TK_IRQHandler(void)
{
    tk_channel *ch;
	int i, j;
	uint8_t st;
	if(tk_current_index == LIBTK_STATE_SET0) {
		TK_ReadStatus(NULL, &st, NULL); 
		for(i = 0; i < 8; i++) {
			if(tk_channel_mask & (1 << i)) {
				ch = &channel[tk_index[i]];
				ch->data = TK_ReadData(i);
        		ch->data <<= 2;  // 14 -> 16 bit
				ch->state = (st & (1 << i)) ? 1: 0;
			}
		}
	} else if(tk_current_index == LIBTK_STATE_SET1) {
		TK_ReadStatus(NULL, &st, NULL); 
		for(i = 8; i < TK_CH_NUM; i++) {
			if(tk_channel_mask & (1 << i)) {
				ch = &channel[tk_index[i]];
				ch->data = TK_ReadData(i);
				
        		ch->data <<= 2;  // 14 -> 16 bit
				ch->state = (st & (1 << i)) ? 1: 0; 
			}
		}
	} else if(tk_current_index == LIBTK_STATE_TIMER) {
		TK_ReadStatus(NULL, &st, NULL); 
		for(i = 0; i < TK_CH_NUM; i++) {
			if(tk_channel_mask & (1 << i)) {
				ch = &channel[tk_index[i]];
				ch->data = TK_ReadData(i);
				ch->data <<= 2;  // 14 -> 16 bit
				ch->state = (st & (1 << i)) ? 1: 0; 
			}
		}
	}

    // trigger SET1 (8~15) if necessary
	if((tk_current_index == LIBTK_STATE_SET0) && (tk_channel_mask & 0xFF00)) {
		tk_current_index = LIBTK_STATE_SET1;
		for(i = 8; i < TK_CH_NUM; i++) {
			if(tk_channel_mask & (i << i)) {
				TK_ConfigChannel(i, &channel[tk_index[i]].cfg);
			}
		}
		TK_Start(tk_channel_mask & 0xFF00);
		return;
	}

	// scan complete
	if(tk_calibrating > 0) {
	    for(i = 0; i < TK_CH_NUM; i++) {
	        if((1 << channel[i].ch) & tk_channel_mask) {
				channel[i].base = (channel[i].base + channel[i].data * 15) >> 4;	
	        }
	    }			
		tk_calibrating--;
	} else {
        for(i = 0; i < LIBTK_MAX_COMPONENT; i++) {
            j = tk_check_state(i);
            if(j == LIB_TK_COMPLETE || j == LIB_TK_NOT_INIT)
                break;           // we're done this round
        }
	}
    if(tk_current_index != LIBTK_STATE_TIMER)
    	tk_current_index = LIBTK_STATE_IDLE;

	complete = 1;

	TK_CLEAR_INTERRUPT;

	return;
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
