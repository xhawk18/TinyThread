/******************************************************************************
 * @file     nano1xx_isr.c
 * @brief    This file contains interrupt service routines
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdint.h>
#include "nano1xx.h"
#include "nano1xx_dac.h"

const uint16_t sine[] = {2047, 2251, 2453, 2651, 2844, 3028, 3202, 3365, 3515, 3650, 3769, 3871, 3954,
                        4019, 4064, 4088, 4095, 4076, 4040, 3984, 3908, 3813, 3701, 3573, 3429, 3272,
                        3102, 2921, 2732, 2536, 2335, 2132, 1927, 1724, 1523, 1328, 1141,  962,  794,
                         639,  497,  371,  262,  171,   99,   45,   12,    0,    7,   35,   84,  151,
                         238,  343,  465,  602,  754,  919, 1095, 1281, 1475, 1674, 1876};

static uint32_t index0 = 0;
static uint32_t index1 = 0;
const uint32_t array_size = sizeof(sine)/sizeof(uint16_t);

void DAC_IRQHandler(void)
{

	if(DAC->STS0 & DAC_STS_IFG) {
		DAC->DATA0 = sine[index0++];
		if(index0 == array_size)
			index0 = 0;

		DAC_CLEAR_CH0_INTERRUPT;
	}

	if(DAC->STS1 & DAC_STS_IFG) {
		DAC->DATA1 = sine[index1++];
		if(index1 == array_size)
			index1 = 0;
		DAC_CLEAR_CH1_INTERRUPT;
	}

	return;
}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

