#include "NUC1xx.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvSPI.h"
#include "Driver\DrvGPIO.h"
#include "LCD_Driver.h"

extern  SPI_T * SPI_PORT[4]={SPI0, SPI3, SPI2, SPI3};

extern  char Ascii[];

void SysTimerDelay(uint32_t us)
{
    SysTick->LOAD = us * 22; /* Assume the internal 22MHz RC used */
    SysTick->VAL   =  (0x00);
    SysTick->CTRL = (1 << SysTick_CTRL_CLKSOURCE_Pos) | (1<<SysTick_CTRL_ENABLE_Pos);

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & (1 << 16)) == 0);
}

void Initial_pannel(void)
{
	
	SYSCLK->APBCLK.SPI3_EN  =1;			 //enable spi function 
	SYS->IPRSTC2.SPI3_RST   =1;			 //reset spi function
	SYS->IPRSTC2.SPI3_RST   =0;

	/* set GPIO to SPI mode*/
	SYS->GPDMFP.SPI3_SS0 	=1;
	SYS->GPDMFP.SPI3_CLK 	=1;
	//SYS->GPDMFP.SPI3_MISO0 	=1;
	SYS->GPDMFP.SPI3_MOSI0 	=1;

	SPI_PORT[eDRVSPI_PORT3]->CNTRL.CLKP = 1;							//CLKP HIGH IDLE
	SPI_PORT[eDRVSPI_PORT3]->CNTRL.TX_BIT_LEN = 9;						//TX LEGTH 9
	SPI_PORT[eDRVSPI_PORT3]->CNTRL.TX_NEG = 1;							//SET TX_NEG
	SPI_PORT[eDRVSPI_PORT3]->DIVIDER.DIVIDER=0X03;					    //SET DIV

	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=1;									//ENABLE SLAVE SELECT
	// Set BR
	SPI_PORT[eDRVSPI_PORT3]->TX[0] =0xEB;
	SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY = 1;
    while ( SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY == 1 );
	// Set PM
	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=0;

	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=1;
	//outp32(SPI3_Tx0, 0x81);	
	SPI_PORT[eDRVSPI_PORT3]->TX[0] =0x81;
	SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY = 1;
    while ( SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY == 1 );
	SPI_PORT[eDRVSPI_PORT3]->TX[0] =0xa0;
	SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY = 1;
    while ( SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY == 1 );
	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=0;

	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=1;
	//outp32(SPI3_Tx0, 0xC0);
	SPI_PORT[eDRVSPI_PORT3]->TX[0] =0xc0;	
	SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY = 1;
    while ( SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY == 1 );
	// Set Display Enable
	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=0;

	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=1;
	SPI_PORT[eDRVSPI_PORT3]->TX[0] = 0XAF;
	SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY = 1;
    while ( SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY == 1 );
	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=0;
}

//*******************************
//this function to wrtie data to panel
//*******************************
void WriteData(unsigned char data)
{
	// Write Data
	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=1;	   //chip select
	SPI_PORT[eDRVSPI_PORT3]->TX[0] =0x100 | data;    	//write data
	SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY = 1;
    while ( SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY == 1 ); //check data out?
	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=0;
}

//*******************************
//this function to initial start address
//*******************************
void SetPACA(unsigned char PA, unsigned char CA)
{
	// Set PA

	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=1; 	
	SPI_PORT[eDRVSPI_PORT3]->TX[0] = 0xB0 | PA;	
	SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY = 1;
    while ( SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY == 1 );	 //check data out?
	// Set CA MSB
	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=0;

	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=1;	
	SPI_PORT[eDRVSPI_PORT3]->TX[0] =0x10 |(CA>>4)&0xF;
	SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY = 1;
    while ( SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY == 1 );	  //check data out?
 	// Set CA LSB
	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=0;

	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=1;
	SPI_PORT[eDRVSPI_PORT3]->TX[0] =0x00 | (CA & 0xF);		
	SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY = 1;
    while ( SPI_PORT[eDRVSPI_PORT3]->CNTRL.GO_BUSY == 1 );	  //check data out?
	SPI_PORT[eDRVSPI_PORT3]->SSR.SSR=0;
}

void Disable_Buzzer(void)
{
 	DrvGPIO_Open(E_GPB,11, E_IO_OUTPUT);	
	DrvGPIO_SetBit(E_GPB,11);	
}



void Show_Word(unsigned char x, unsigned char y,unsigned char ascii_word)
{
int i=0,k=0;
	unsigned char temp;	  
	  k=(ascii_word-32)*16;	  
	  
	  for(i=0;i<8;i++)
	  {
	  SetPACA((x*2),(129-(y*8)-i));
	  temp=Ascii[k+i];	 
      WriteData(temp);
	  }

      for(i=0;i<8;i++)
	  {
	  SetPACA((x*2)+1,(129-(y*8)-i));	 
      temp=Ascii[k+i+8];
	  WriteData(temp);
	  }
}




void print_lcd(unsigned char line, char *str)
{
int i=0; 
  do{   
    Show_Word(line,i,*str++); 
  	i++;
  	if(i>15)
	  break;
    }
   while(*str!='\0');
}



void clr_all_pannal(void)
{
 int i=0;
	/*CLEAR ALL PANNAL*/
	SetPACA(0x0, 0x0);

	for (i = 0; i < 132 *8; i++)
	{
		WriteData(0x00);
	}
	WriteData(0x0f);
}

