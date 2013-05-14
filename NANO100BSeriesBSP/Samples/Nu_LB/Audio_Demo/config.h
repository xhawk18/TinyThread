/******************************************************************************
 * @file     config.h
 * @brief    Nano1xx I2S Driver Sample header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
 
// use audio FIFO threshold 
//#define I2S_USE_FIFO

// use PDMA 
#define I2S_USE_PDMA

// use LIN as source, undefine it if MIC is used
//#define INPUT_IS_LIN

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#define BUFF_LEN    32
#define BUFF_HALF_LEN	(BUFF_LEN/2)

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
