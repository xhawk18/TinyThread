#ifndef __LCD_Driver_H__
#define __LCD_Driver_H__
     
extern void SysTimerDelay(uint32_t us);
extern void Initial_pannel(void);
extern void Disable_Buzzer(void);

extern void Show_Word(unsigned char x, unsigned char y,unsigned char ascii_word);

extern void print_lcd(unsigned char line, char *str);


extern void clr_all_pannal(void);
#endif
