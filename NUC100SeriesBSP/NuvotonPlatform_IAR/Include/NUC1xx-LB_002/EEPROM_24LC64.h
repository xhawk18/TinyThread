#ifndef __EEPROM_24LC64_H__
#define __EEPROM_24LC64_H__
     
extern void Write_24LC64(uint32_t address,uint8_t data);
extern uint8_t Read_24LC64(uint32_t address);
#endif
