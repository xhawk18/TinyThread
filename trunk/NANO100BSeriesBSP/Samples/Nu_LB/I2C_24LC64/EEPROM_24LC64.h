/******************************************************************************
 * @file     EEPROM_24LC64.h
 * @brief    24LC64 EEPROM library Header File
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __EEPROM_24LC64_H__
#define __EEPROM_24LC64_H__
     
extern void Write_24LC64(uint32_t address,uint8_t data);
extern uint8_t Read_24LC64(uint32_t address);
#endif

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
