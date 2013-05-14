/******************************************************************************
 * @file     config.c
 * @brief    This file contains the touch key configuration table.
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
#include "libtk.h"

libtk_channel_config_s cfg[16] = {
        {0xa992, 0x018f, 6, 2},	  // 0					       +
        {0xa906, 0x0161, 7, 3},	  // 1
        {0xa8d0, 0x0120, 7, 3},
        {0xa5ee, 0x013a, 7, 3},	  // 3
        {0xa6f4, 0x0154, 6, 2},
        {0xa686, 0x011a, 6, 2},	  // 5
        {0x9ad4, 0x0149, 7, 3},
        {0x9a3e, 0x01d0, 6, 2},	  // 7
        {0xbeb4, 0x00d1, 7, 3},
        {0xa7bb, 0x0172, 6, 2},
        {0xa8fe, 0x0152, 7, 3},
        {0xaaa0, 0x0135, 7, 3},
        {0x9ffb, 0x01dc, 7, 3},
        {0xa7d5, 0x0188, 7, 3},
        {0xad69, 0x014c, 7, 3},
        {0xb50c, 0x0111, 6, 2},
};

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
