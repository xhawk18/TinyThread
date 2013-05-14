/******************************************************************************
 * @file     config.c
 * @brief    Nano1xx TK_LCD Driver Sample code
 * @version  1.0.1
 * @date     08, October, 2012
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

#if 1
// w.o. panel
libtk_channel_config_s cfg[TK_CH_NUM] = {{0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0x965f, 0x0d69, 7, 3},
											 {0x949e, 0x13d3, 7, 3},
											 {0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0xabd3, 0x0cf5, 7, 3},
											 {0xa7c8, 0x0e62, 7, 3},
											 {0xa49d, 0x0c8b, 7, 3},
											 {0xa590, 0x0dd5, 7, 3},
											 {0xa86f, 0x0d02, 7, 3}};
#else
// w. panel
libtk_channel_config_s cfg[TK_CH_NUM] = {{0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0x0000, 0x0000, 1, 0},
											 {0xD130, 0x08A0, 1, 3},
											 {0xCBE0, 0x0950, 1, 3},
											 {0xCBB0, 0x0900, 1, 3},
											 {0xCAF0, 0x0950, 1, 3},
											 {0xCAE0, 0x0940, 1, 3},
											 {0xCB60, 0x09A0, 1, 3},
											 {0xC9E0, 0x0AB0, 1, 3}};


#endif


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

