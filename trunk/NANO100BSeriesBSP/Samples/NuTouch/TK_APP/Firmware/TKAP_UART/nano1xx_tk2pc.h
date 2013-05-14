/******************************************************************************
 * @file     HIDTransfer_Main.c
 * @brief    Nano1xx Touch Key configuration sample code (UART interface)
 * @version  1.0.1
 * @date     04, December, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef NANO1XX_TK2PC_H
#define NANO1XX_TK2PC_H

#define OP_GET_CONFIG 			0x10
#define OP_SET_CHANNEL_CONFIG   0x11
#define OP_START_RECORD 		0x12
#define OP_STOP_RECORD 			0x13
#define OP_LISTEN_STATUS 		0x14
#define OP_AUTO_CHANNEL_CONFIG  0x15

#define MAX_CHANNEL 16
//extern tk_channel channel[TK_CH_NUM];
//extern uint8_t tk_index[TK_CH_NUM];	
//===============================================  *.h

#pragma pack(1)
typedef struct
{
	uint8_t  channel;
	uint16_t data;
	uint8_t  reseved;

}LISTEN_STATUS;

#pragma pack(1)
typedef struct {
	uint8_t channel;
	uint16_t base;
	uint16_t diff;	 //threshold
	uint8_t current;
	uint8_t div; 
	uint8_t reseved;
}TK_CNANNEL_CONFIG;

#pragma pack(1)
typedef struct
{
	uint8_t magic[8];
	uint32_t length;
}TK_COMMAND_SEND;


#if 1
typedef struct {
	uint16_t base;
	uint16_t diff;
	uint8_t current;
	uint8_t div; 
}libtk_channel_config_s;
#endif


//Auto Config============
typedef struct {
	uint16_t base;
	uint16_t diff;		
	uint8_t current;
	uint8_t div;		
}best_cfg;

typedef struct {
	uint8_t current;
	uint16_t div;
	uint16_t val;
}rank;

typedef struct {
	uint16_t level_off;
	uint16_t data_off;
	uint16_t level_on;
	uint16_t data_on;
	int8_t score;
}tk_sense_result;
//=======================

#endif
/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
