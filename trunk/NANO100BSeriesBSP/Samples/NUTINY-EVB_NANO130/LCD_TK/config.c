/******************************************************************************
 * @file     config.c
 * @brief    Nano1xx NUTINY-EVB_NANO130 Touch Key Demo code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nano1xx.h"
#include "nano1xx_tk.h"
#include "libtk.h"


libtk_channel_config_s cfg[16] = {
        {0x0000, 0x0000, 1, 0},	  // 0
        {0x0000, 0x0000, 1, 0},	  // 1
        {0x0000, 0x0000, 1, 0},
        {0x0000, 0x0000, 1, 0},	  // 3
        {0x0000, 0x0000, 1, 0},
        {0x0000, 0x0000, 1, 0},	  // 5
        {0x0000, 0x0000, 1, 0},
        {0x0000, 0x0000, 1, 0},	  // 7
        {0x0000, 0x0000, 1, 0},
        {0x0000, 0x0000, 1, 0},
        {0x0000, 0x0000, 1, 0},
        {0xbcec, 0x0a38, 7, 3},
        {0xbc8c, 0x0a5d, 6, 2},
        {0xb6b9, 0x0b67, 7, 3},
        {0xb85d, 0x0aec, 7, 3},
        {0xba53, 0x0aca, 7, 3}
};

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
