/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#ifndef __M051Series_H__
#define __M051Series_H__


/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */
 
typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                             */
  HardFault_IRQn		      = -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                     */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                       */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                       */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                   */

/******  ARMIKMCU Swift specific Interrupt Numbers ************************************************/
  BOD_IRQn                  = 0,
  WDT_IRQn                  = 1,
  EINT0_IRQn                = 2,
  EINT1_IRQn                = 3,
  GPIO_P0P1_IRQn            = 4,
  GPIO_P2P3P4_IRQn          = 5,
  PWMA_IRQn                 = 6,
  PWMB_IRQn                 = 7,
  TMR0_IRQn                 = 8,
  TMR1_IRQn                 = 9,
  TMR2_IRQn                 = 10,
  TMR3_IRQn                 = 11,
  UART0_IRQn                = 12,
  UART1_IRQn                = 13,
  SPI0_IRQn                 = 14,
  SPI1_IRQn                 = 15,
  I2C0_IRQn                 = 18,
  I2C1_IRQn                 = 19,
  CAN0_IRQn                 = 20,
  CAN1_IRQn                 = 21,
  SD_IRQn                   = 22,
  USBD_IRQn                 = 23,
  PS2_IRQn                  = 24,
  ACMP_IRQn                 = 25,
  PDMA_IRQn                 = 26,
  I2S_IRQn                  = 27,
  PWRWU_IRQn                = 28,
  ADC_IRQn                  = 29,
  DAC_IRQn                  = 30,
  RTC_IRQn                  = 31
         
                                        /*!< maximum of 32 Interrupts are possible                */
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __MPU_PRESENT           0       /*!< armikcmu does not provide a MPU present or not       */
#define __NVIC_PRIO_BITS        2       /*!< armikcmu Supports 2 Bits for the Priority Levels     */
#define __Vendor_SysTickConfig  0       /*!< Set to 1 if different SysTick Config is used         */

#pragma diag_suppress=Pa082

#include "core_cm0.h"                   /* Cortex-M0 processor and core peripherals               */
#include "system_M051Series.h"          /* M051Series System                                      */
#include "System\SysInfra.h"    


/**
 * Initialize the system clock
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system
 *         Initialize the PLL and update the SystemFrequency variable
 */
extern void SystemInit (void);				  


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/

/*--------------------- General Purpose Input and Ouptut ---------------------*/
typedef struct
{
    __IO uint32_t 	PMD0:2;
    __IO uint32_t 	PMD1:2;
    __IO uint32_t 	PMD2:2;
    __IO uint32_t 	PMD3:2;
    __IO uint32_t 	PMD4:2;
    __IO uint32_t 	PMD5:2;
    __IO uint32_t 	PMD6:2;
    __IO uint32_t 	PMD7:2;
    __I  uint32_t  	RESERVE:16;    
} GPIO_PMD_T;

typedef __IO uint32_t GPIO_SCH_T;

typedef __IO uint32_t GPIO_DOUT_T;

typedef __IO uint32_t GPIO_DMASK_T;

typedef __IO uint32_t GPIO_PIN_T;

typedef __IO uint32_t GPIO_DBEN_T;

typedef __IO uint32_t GPIO_IMD_T;

typedef __IO uint32_t GPIO_IEN_T;

typedef __IO uint32_t GPIO_ISRC_T;

typedef struct
{
    __IO uint32_t  DBCLKSEL:4;
    __IO uint32_t  DBCLKSRC:1;
    __IO uint32_t  ICLK_ON:1;
    __I  uint32_t  RESERVE:26;    
} GPIO_DBNCECON_T;

typedef struct
{
    GPIO_PMD_T      PMD;
    GPIO_SCH_T      SCH;
    GPIO_DOUT_T     DOUT;
    GPIO_DMASK_T    DMASK;
    GPIO_PIN_T      PIN;
    GPIO_DBEN_T     DBEN;
    GPIO_IMD_T      IMD;
    GPIO_IEN_T      IEN;
    GPIO_ISRC_T     ISRC;
} GPIO_T;

/*------------------------- UART Interface Controller ------------------------*/

typedef __IO uint32_t UART_DATA_T;


typedef struct
{
    __IO uint32_t  RDA_IEN:1;
    __IO uint32_t  THRE_IEN:1;
    __IO uint32_t  RLS_IEN:1;
    __IO uint32_t  MODEM_IEN:1;
    __IO uint32_t  RTO_IEN:1;	  
    __IO uint32_t  BUF_ERR_IEN:1;
    __IO uint32_t  WAKE_IEN:1;
    __I  uint32_t  RESERVE0:1;
	__IO uint32_t  LIN_RX_BRK_IEN:1;
    __I  uint32_t  RESERVE1:2;
    __IO uint32_t  TIME_OUT_EN:1;      /* Time-out counter enable */
    __IO uint32_t  AUTO_RTS_EN:1;
    __IO uint32_t  AUTO_CTS_EN:1;
    __IO uint32_t  DMA_TX_EN:1;
    __IO uint32_t  DMA_RX_EN:1;
    __I  uint32_t  RESERVE2:16;    
    
} UART_IER_T;

typedef struct
{
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  RFR:1;
    __IO uint32_t  TFR:1;
    __I  uint32_t  RESERVE1:1;
    __IO uint32_t  RFITL:4;             /* Rx FIFO Interrupt Trigger Level */
	__IO uint32_t  RX_DIS:1;
    __I  uint32_t  RESERVE2:7;
    __IO uint32_t  RTS_TRI_LEV:4;
    __I  uint32_t  RESERVE3:12;
} UART_FCR_T;

typedef struct
{
    __IO uint32_t  WLS:2;               /* Word length select */
    __IO uint32_t  NSB:1;               /* Number of STOP bit */
    __IO uint32_t  PBE:1;               /* Parity bit enable  */
    __IO uint32_t  EPE:1;               /* Even parity enable */
    __IO uint32_t  SPE:1;               /* Stick parity enable*/
    __IO uint32_t  BCB:1;               /* Break control bit  */
    __I  uint32_t  RESERVE:25;
} UART_LCR_T;

typedef struct
{
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  RTS:1;
    __I  uint32_t  RESERVE1:2;
    __IO uint32_t  LBME:1;
    __I  uint32_t  RESERVE2:4;
    __IO uint32_t  LEV_RTS:1;
    __I  uint32_t  RESERVE3:3;
    __I  uint32_t  RTS_ST:1;               /* RTS status */
    __I  uint32_t  RESERVE4:18;
} UART_MCR_T;

typedef struct
{
    __IO uint32_t  DCTSF:1;
    __I  uint32_t  RESERVE0:3;
    __I  uint32_t  CTS_ST:1;               /* CTS status */
    __I  uint32_t  RESERVE1:3;
    __IO uint32_t  LEV_CTS:1;
    __I  uint32_t  RESERVE2:23;
} UART_MSR_T;

typedef struct
{
    __IO uint32_t  RX_OVER_IF:1;
    __I  uint32_t  RESERVE0:2;
	__IO uint32_t  RS485_ADD_DETF:1;
    __IO uint32_t  PEF:1;
    __IO uint32_t  FEF:1;
    __IO uint32_t  BIF:1;
    __I  uint32_t  RESERVE1:1;
    __I  uint32_t  RX_POINTER:6;
    __I  uint32_t  RX_EMPTY:1;
    __I  uint32_t  RX_FULL:1;
    __I  uint32_t  TX_POINTER:6;
    __I  uint32_t  TX_EMPTY:1;
    __I  uint32_t  TX_FULL:1;
    __IO uint32_t  TX_OVER_IF:1;
    __I  uint32_t  RESERVE2:3;
    __I  uint32_t  TE_FLAG:1;                /* Transmitter empty flag */
    __I  uint32_t  RESERVE3:3;
} UART_FSR_T;

typedef struct
{
    __IO uint32_t  RDA_IF:1;
    __IO uint32_t  THRE_IF:1;
    __IO uint32_t  RLS_IF:1;
    __IO uint32_t  MODEM_IF:1;
    __IO uint32_t  TOUT_IF:1;
    __IO uint32_t  BUF_ERR_IF:1;
    __I  uint32_t  RESERVE0:2;

    __IO uint32_t  RDA_INT:1;
    __IO uint32_t  THRE_INT:1;
    __IO uint32_t  RLS_INT:1;
    __IO uint32_t  MODEM_INT:1;
    __IO uint32_t  TOUT_INT:1;
    __IO uint32_t  BUF_ERR_INT:1;
    __I  uint32_t  RESERVE1:18;
} UART_ISR_T;



typedef struct
{
   __IO uint32_t  TOIC:7;
   __I  uint32_t  RESERVE0:1;
   __IO uint32_t  DLY:8;
   __I  uint32_t  RESERVE1:16;

}UART_TOR_T;


typedef struct
{
    __IO uint32_t  BRD:16;
    __I  uint32_t  RESERVE0:8;
    __IO uint32_t  DIVIDER_X:4;
    __IO uint32_t  DIV_X_ONE:1;            
    __IO uint32_t  DIV_X_EN:1;            
    __I  uint32_t  RESERVE1:2;
} UART_BAUD_T;

typedef struct
{
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  TX_SELECT:1;
	__I  uint32_t  RESERVE1:3;         
    __IO uint32_t  INV_TX:1;            
    __IO uint32_t  INV_RX:1;
    __I  uint32_t  RESERVE2:25;
} UART_IRCR_T;


typedef struct
{
	__I  uint32_t  RESERVE0:8;
    __IO uint32_t  RS485_NMM:1;
	__IO uint32_t  RS485_AAD:1;
	__IO uint32_t  RS485_AUD:1;
	__I  uint32_t  RESERVE1:4;
	__IO uint32_t  RS485_ADD_EN:1;
	__I  uint32_t  RESERVE2:8;
	__IO uint32_t  ADDR_MATCH:8;
} UART_ALTCON_T;

typedef struct
{
    __IO uint32_t  FUN_SEL:2;
	__I  uint32_t  RESERVE0:30;

} UART_FUNSEL_T;

typedef struct
{
    UART_DATA_T     DATA;
    UART_IER_T      IER;
    UART_FCR_T      FCR;
    UART_LCR_T      LCR;
    UART_MCR_T      MCR;
    UART_MSR_T      MSR;
    UART_FSR_T      FSR;
    UART_ISR_T      ISR;
    UART_TOR_T      TOR;
    UART_BAUD_T     BAUD;
    UART_IRCR_T     IRCR;   
    UART_ALTCON_T   ALTCON;    
	UART_FUNSEL_T   FUNSEL;    
} UART_T;

/*----------------------------- Timer Controller -----------------------------*/
typedef struct
{
    __IO uint32_t  PRESCALE:8;
    __I  uint32_t  RESERVE0:8;
    __IO uint32_t  TDR_EN:1;
    __I  uint32_t  RESERVE1:8;
    __IO uint32_t  CACT:1;
    __IO uint32_t  CRST:1;
    __IO uint32_t  MODE:2;
    __IO uint32_t  IE:1;
    __IO uint32_t  CEN:1;
    __IO uint32_t  nDBGACK_EN:1;
} TIMER_TCSR_T;

typedef __IO uint32_t TIMER_TCMPR_T;

typedef __IO uint32_t TIMER_TDR_T;

typedef struct
{
    __IO uint32_t  TIF:1;
    __I  uint32_t  RESERVE:31;
} TIMER_TISR_T;

typedef struct
{
    TIMER_TCSR_T     TCSR;
    TIMER_TCMPR_T    TCMPR;
    TIMER_TISR_T     TISR;    
    TIMER_TDR_T      TDR;
 } TIMER_T;

/*----------------------------- WDT Controller -----------------------------*/
typedef struct
{
    __IO uint32_t  WTR:1;
	__IO uint32_t  WTRE:1;
	__IO uint32_t  WTRF:1;
	__IO uint32_t  WTIF:1;
	__IO uint32_t  WTWKE:1;
	__I  uint32_t  WTWKF:1;
	__IO uint32_t  WTIE:1;
	__IO uint32_t  WTE:1;
	__IO uint32_t  WTIS:3;
    __I  uint32_t  RESERVE1:21;
} WDT_WTCR_T;

typedef struct
{
    WDT_WTCR_T     WTCR;
    
 } WDT_T;

/*------------------------- SPI Interface Controller -------------------------*/
typedef struct
{
    __IO uint32_t  GO_BUSY:1;
    __IO uint32_t  RX_NEG:1;
    __IO uint32_t  TX_NEG:1;
    __IO uint32_t  TX_BIT_LEN:5;
    __IO uint32_t  TX_NUM:2;
    __IO uint32_t  LSB:1;
    __IO uint32_t  CLKP:1;
    __IO uint32_t  SP_CYCLE:4;
    __IO uint32_t  IF:1;
    __IO uint32_t  IE:1;
    __IO uint32_t  SLAVE:1;
    __IO uint32_t  BYTE_REORDER:2;
    __I  uint32_t  RESERVE0:2;
    __IO uint32_t  VARCLK_EN:1;
    __I  uint32_t  RESERVE1:8;
} SPI_CNTRL_T;

typedef struct
{
    __IO uint32_t  DIVIDER:16;
    __IO uint32_t  DIVIDER2:16;
} SPI_DIVIDER_T;

typedef struct
{
    __IO uint32_t  SSR:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  SS_LVL:1;
    __IO uint32_t  AUTOSS:1;
    __IO uint32_t  SS_LTRIG:1;
    __IO uint32_t  LTRIG_FLAG:1;
    __I  uint32_t  RESERVE:26;
} SPI_SSR_T;

typedef __I  uint32_t   SPI_RX_T;
typedef __O  uint32_t   SPI_TX_T;

typedef __IO uint32_t SPI_VARCLK_T;

typedef struct
{
    SPI_CNTRL_T     CNTRL;
    SPI_DIVIDER_T   DIVIDER;
    SPI_SSR_T       SSR;
    uint32_t        RESERVE0;
    SPI_RX_T        RX[2];
    uint32_t        RESERVE1;
    uint32_t        RESERVE2;
    SPI_TX_T        TX[2];
    uint32_t        RESERVE3;
    uint32_t        RESERVE4;
    uint32_t        RESERVE5;
    SPI_VARCLK_T    VARCLK;
    uint32_t        RESERVE6;
} SPI_T;

/*------------------------------ I2C Controller ------------------------------*/
typedef struct
{
    __I  uint32_t  RESERVE0:2;
    __IO uint32_t  AA:1;
    __IO uint32_t  SI:1;
    __IO uint32_t  STO:1;
    __IO uint32_t  STA:1;
    __IO uint32_t  ENSI:1;
   	__IO uint32_t  EI:1;
    __I  uint32_t  RESERVE1:24;
} I2C_CON_T;

typedef struct
{
    __IO uint32_t  GC:1;
    __IO uint32_t  ADDR:7;
    __I  uint32_t  RESERVE:24;
} I2C_ADDR_T;

typedef __IO uint32_t I2C_DATA_T;

typedef __I  uint32_t I2C_STATUS_T;

typedef __IO uint32_t I2C_CLK_T;

typedef struct
{
    __IO uint32_t  TIF:1;
    __IO uint32_t  DIV4:1;
    __IO uint32_t  ENTI:1;
    __I  uint32_t  RESERVE:29;
} I2C_TOC_T;

typedef struct
{
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  ADM:7;
    __I  uint32_t  RESERVE1:24;
} I2C_ADRM_T;

typedef struct
{
    I2C_CON_T       CON;
    I2C_ADDR_T      ADDR0;
    I2C_DATA_T      DATA;
    I2C_STATUS_T    STATUS;
    I2C_CLK_T       CLK;
    I2C_TOC_T       TOC;
	I2C_ADDR_T	    ADDR1;
	I2C_ADDR_T	    ADDR2;
	I2C_ADDR_T	    ADDR3;
	I2C_ADRM_T		ADRM0;
	I2C_ADRM_T		ADRM1;
	I2C_ADRM_T		ADRM2;
	I2C_ADRM_T	    ADRM3;        
} I2C_T;

/*----------------------------- ADC Controller -------------------------------*/
typedef struct
{
    __IO uint32_t  RSLT:16;
    __IO uint32_t  OVERRUN:1;
    __IO uint32_t  VALID:1;
    __I  uint32_t  RESERVE1:14;
} ADC_ADDR_T;

typedef struct
{
    __IO uint32_t  ADEN:1;
    __IO uint32_t  ADIE:1;
    __IO uint32_t  ADMD:2;
    __IO uint32_t  TRGS:2;
    __IO uint32_t  TRGCOND:2;
    __IO uint32_t  TRGEN:1;
    __IO uint32_t  RESERVE0:1;
    __IO uint32_t  DIFFEN:1;
    __IO uint32_t  ADST:1;
    __I  uint32_t  RESERVE1:20;
} ADC_ADCR_T;

typedef struct
{
    __IO uint32_t  CHEN:8;
    __IO uint32_t  PRESEL:2;
    __I  uint32_t  RESERVE:22;
} ADC_ADCHER_T;

typedef struct
{
    __IO uint32_t  CMPEN:1;
    __IO uint32_t  CMPIE:1;
    __IO uint32_t  CMPCOND:1;
    __IO uint32_t  CMPCH:3;
    __I  uint32_t  RESERVE0:2;
    __IO uint32_t  CMPMATCNT:4;
    __I  uint32_t  RESERVE1:4;
    __IO uint32_t  CMPD:12;
    __I  uint32_t  RESERVE2:4;
} ADC_ADCMPR_T;

typedef struct
{
    __IO uint32_t  ADF:1;
    __IO uint32_t  CMPF0:1;
    __IO uint32_t  CMPF1:1;
    __IO uint32_t  BUSY:1;
    __IO uint32_t  CHANNEL:3;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  VALID:8;
    __IO uint32_t  OVERRUN:8;
    __I  uint32_t  FIFO_SIZE:3;
    __I  uint32_t  RESERVE1:5;
} ADC_ADSR_T;

typedef struct
{
    __IO uint32_t  CALEN:1;
    __IO uint32_t  CALDONE:1;
    __I  uint32_t  RESERVE:30;
} ADC_ADCALR_T;

typedef struct
{
    ADC_ADDR_T      ADDR[8];
    ADC_ADCR_T      ADCR;
    ADC_ADCHER_T    ADCHER;
    ADC_ADCMPR_T    ADCMPR[2];
    ADC_ADSR_T      ADSR;
    ADC_ADCALR_T    ADCALR;
} ADC_T;


/*---------------------------- Clock Controller ------------------------------*/
typedef struct
{
    __IO uint32_t  XTL12M_EN:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  OSC22M_EN:1;
    __IO uint32_t  OSC10K_EN:1;
    __IO uint32_t  PD_WU_DLY:1;
    __IO uint32_t  PD_WU_INT_EN:1;
    __IO uint32_t  PD_WU_STS:1;
    __IO uint32_t  PWR_DOWN:1;
    __IO uint32_t  PD_WAIT_CPU:1;
    __I  uint32_t  RESERVE1:23;
} SYSCLK_PWRCON_T;

typedef struct
{
    //__IO uint32_t  CPU_EN:1;
	__I  uint32_t  RESERVE0:2;
    __IO uint32_t  ISP_EN:1;
	__IO uint32_t  EBI_EN:1;
    __I  uint32_t  RESERVE1:28;
} SYSCLK_AHBCLK_T;

typedef struct
{
    __IO uint32_t  WDT_EN:1;
	__I  uint32_t  RESERVE0:1;
    __IO uint32_t  TMR0_EN:1;
    __IO uint32_t  TMR1_EN:1;
    __IO uint32_t  TMR2_EN:1;
    __IO uint32_t  TMR3_EN:1;
    __IO uint32_t  FDIV_EN:1;
	__I  uint32_t  RESERVE1:1;
    __IO uint32_t  I2C_EN:1;
    __I  uint32_t  RESERVE2:3;
    __IO uint32_t  SPI0_EN:1;
    __IO uint32_t  SPI1_EN:1;
    __I  uint32_t  RESERVE3:2;
    __IO uint32_t  UART0_EN:1;
    __IO uint32_t  UART1_EN:1;
    __I  uint32_t  RESERVE4:2;
    __IO uint32_t  PWM01_EN:1;
    __IO uint32_t  PWM23_EN:1;
	__IO uint32_t  PWM45_EN:1;
    __IO uint32_t  PWM67_EN:1;
    __I  uint32_t  RESERVE5:4;
    __IO uint32_t  ADC_EN:1;
    __I  uint32_t  RESERVE6:3;
} SYSCLK_APBCLK_T;

typedef struct
{
    __I  uint32_t  XTL12M_STB:1;
	__I  uint32_t  RESERVE0:1;
    __I  uint32_t  PLL_STB:1;
	__I  uint32_t  OSC10K_STB:1;
	__I  uint32_t  OSC22M_STB:1;
    __I  uint32_t  RESERVE1:2;
	__IO uint32_t  CLK_SW_FAIL:1;
    __I  uint32_t  RESERVE2:24;
} SYSCLK_CLKSTATUS_T;


typedef struct
{
    __IO uint32_t  HCLK_S:3;
    __IO uint32_t  STCLK_S:3;
    __I  uint32_t  RESERVE:26;
} SYSCLK_CLKSEL0_T;

typedef struct
{
    __IO uint32_t  WDT_S:2;
    __IO uint32_t  ADC_S:2;
    __I  uint32_t  RESERVE1:4;
    __IO uint32_t  TMR0_S:3;
    __I  uint32_t  RESERVE2:1;
    __IO uint32_t  TMR1_S:3;
    __I  uint32_t  RESERVE3:1;
    __IO uint32_t  TMR2_S:3;
    __I  uint32_t  RESERVE4:1;
    __IO uint32_t  TMR3_S:3;
    __I  uint32_t  RESERVE5:1;
    __IO uint32_t  UART_S:2;
    __I  uint32_t  RESERVE6:2;
	__IO uint32_t  PWM01_S:2;
    __IO uint32_t  PWM23_S:2;
} SYSCLK_CLKSEL1_T;

typedef struct
{
    __I  uint32_t  RESERVE0:2;
	__IO uint32_t  FRQDIV_S:2;
	__IO uint32_t  PWM45_S:2;
    __IO uint32_t  PWM67_S:2;
    __I  uint32_t  RESERVE1:24;
} SYSCLK_CLKSEL2_T;

typedef struct
{
    __IO uint32_t  HCLK_N:4;
    __I  uint32_t  RESERVE0:4;
	__IO uint32_t  UART_N:4;
    __I  uint32_t  RESERVE1:4;
	__IO uint32_t  ADC_N:8;
    __I  uint32_t  RESERVE2:8;
} SYSCLK_CLKDIV_T;

typedef struct
{
    __IO uint32_t  FB_DV:9;
    __IO uint32_t  IN_DV:5;
    __IO uint32_t  OUT_DV:2;
    __IO uint32_t  PD:1;
    __IO uint32_t  BP:1;
    __IO uint32_t  OE:1;
    __IO uint32_t  PLL_SRC:1;
    __I  uint32_t  RESERVE:12;
} SYSCLK_PLLCON_T;

typedef struct
{    
    __IO uint32_t  FSEL:4;
	__IO uint32_t  FDIV_EN:1;
    __I  uint32_t  RESERVE:27;
} SYSCLK_FRQDIV_T;

typedef struct
{
    SYSCLK_PWRCON_T    PWRCON;
    SYSCLK_AHBCLK_T    AHBCLK;
    SYSCLK_APBCLK_T    APBCLK;
    SYSCLK_CLKSTATUS_T CLKSTATUS;
    SYSCLK_CLKSEL0_T   CLKSEL0;
    SYSCLK_CLKSEL1_T   CLKSEL1;
    SYSCLK_CLKDIV_T    CLKDIV;
    SYSCLK_CLKSEL2_T   CLKSEL2;
	SYSCLK_PLLCON_T    PLLCON;
	SYSCLK_FRQDIV_T    FRQDIV;
} SYSCLK_T;

/*---------------------------- Global Controller -----------------------------*/
typedef __I uint32_t GCR_PDID_T;  

typedef struct
{
	__IO uint32_t  RSTS_POR:1;
	__IO uint32_t  RSTS_RESET:1;
	__IO uint32_t  RSTS_WDT:1;
	__IO uint32_t  RSTS_LVR:1;
	__IO uint32_t  RSTS_BOD:1;
	__IO uint32_t  RSTS_MCU:1;
	__I  uint32_t  RESERVE0:1;
	__IO uint32_t  RSTS_CPU:1;    
	__I  uint32_t  RESERVE1:24;
} GCR_RSTSRC_T;

typedef struct
{
    __IO uint32_t  CHIP_RST:1;
    __IO uint32_t  CPU_RST:1;
    __I  uint32_t  RESERVE0:1;
	__IO uint32_t  EBI_RST:1;
    __I  uint32_t  RESERVE1:28;
} GCR_IPRSTC1_T;

typedef struct
{
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  GPIO_RST:1;
    __IO uint32_t  TMR0_RST:1;
    __IO uint32_t  TMR1_RST:1;
    __IO uint32_t  TMR2_RST:1;
    __IO uint32_t  TMR3_RST:1;
    __I  uint32_t  RESERVE1:2;
    __IO uint32_t  I2C_RST:1;
    __I  uint32_t  RESERVE2:3;
    __IO uint32_t  SPI0_RST:1;
    __IO uint32_t  SPI1_RST:1;
    __I  uint32_t  RESERVE3:2;
	__IO uint32_t  UART0_RST:1;
    __IO uint32_t  UART1_RST:1;
    __I  uint32_t  RESERVE4:2;
    __IO uint32_t  PWM03_RST:1;
    __IO uint32_t  PWM47_RST:1;
    __I  uint32_t  RESERVE5:6;
    __IO uint32_t  ADC_RST:1;
    __I  uint32_t  RESERVE6:3;
} GCR_IPRSTC2_T;

typedef struct
{
    __IO uint32_t  BOD_EN:1;
    __IO uint32_t  BOD_VL:2;
    __IO uint32_t  BOD_RSTEN:1;
    __IO uint32_t  BOD_INTF:1;
    __IO uint32_t  BOD_LPM:1;
    __IO uint32_t  BOD_OUT:1;
	__IO uint32_t  LVR_EN:1;
    __I  uint32_t  RESERVE1:24;
} GCR_BODCR_T;

typedef __IO uint32_t GCR_PORCR_T;

typedef struct
{
	__IO uint32_t 	MFP0_AD0_CTS1:1;
	__IO uint32_t 	MFP1_AD1_RTS1:1;
	__IO uint32_t 	MFP2_AD2_CTS0:1;
	__IO uint32_t 	MFP3_AD3_RTS0:1;
	__IO uint32_t 	MFP4_AD4_SS1:1;
	__IO uint32_t 	MFP5_AD5_MOSI1:1;
	__IO uint32_t 	MFP6_AD6_MISO1:1;
	__IO uint32_t 	MFP7_AD7_SCLK1:1;
	__IO uint32_t 	ALT0_AD0_CTS1:1;
	__IO uint32_t 	ALT1_AD1_RTS1:1;
	__IO uint32_t 	ALT2_AD2_CTS0:1;
	__IO uint32_t 	ALT3_AD3_RTS0:1;
	__IO uint32_t 	ALT4_AD4_SS1:1;
	__IO uint32_t 	ALT5_AD5_MOSI1:1;
	__IO uint32_t 	ALT6_AD6_MISO1:1;
	__IO uint32_t 	ALT7_AD7_SCLK1:1;
	__IO uint32_t 	SCHMITT:8;
	__I  uint32_t 	RESERVE:8;    
} GCR_P0_MFP_T;

typedef struct
{
	__IO uint32_t 	MFP0_AIN0_T2:1;
	__IO uint32_t 	MFP1_AIN1_T3:1;
	__IO uint32_t 	MFP2_AIN2_RXD1:1;
	__IO uint32_t 	MFP3_AIN3_TXD1:1;
	__IO uint32_t 	MFP4_AIN4_SS0:1;
	__IO uint32_t 	MFP5_AIN5_MOSI0:1;
	__IO uint32_t 	MFP6_AIN6_MISO0:1;
	__IO uint32_t 	MFP7_AIN7_SCLK0:1;
	__IO uint32_t 	ALT0_AIN0_T2:1;
	__IO uint32_t 	ALT1_AIN1_T3:1;
	__IO uint32_t 	ALT2_AIN2_RXD1:1;
	__IO uint32_t 	ALT3_AIN3_TXD1:1;
	__IO uint32_t 	ALT4_AIN4_SS0:1;
	__IO uint32_t 	ALT5_AIN5_MOSI0:1;
	__IO uint32_t 	ALT6_AIN6_MISO0:1;
	__IO uint32_t 	ALT7_AIN7_SCLK0:1;
	__IO uint32_t 	SCHMITT:8;
	__I  uint32_t 	RESERVE:8;    
} GCR_P1_MFP_T;

typedef struct
{
	__IO uint32_t 	MFP0_AD8_PWM0:1;
	__IO uint32_t 	MFP1_AD9_PWM1:1;
	__IO uint32_t 	MFP2_AD10_PWM2:1;
	__IO uint32_t 	MFP3_AD11_PWM3:1;
	__IO uint32_t 	MFP4_AD12_PWM4:1;
	__IO uint32_t 	MFP5_AD13_PWM5:1;
	__IO uint32_t 	MFP6_AD14_PWM6:1;
	__IO uint32_t 	MFP7_AD15_PWM7:1;
	__IO uint32_t 	ALT0_AD8_PWM0:1;
	__IO uint32_t 	ALT1_AD9_PWM1:1;
	__IO uint32_t 	ALT2_AD10_PWM2:1;
	__IO uint32_t 	ALT3_AD11_PWM3:1;
	__IO uint32_t 	ALT4_AD12_PWM4:1;
	__IO uint32_t 	ALT5_AD13_PWM5:1;
	__IO uint32_t 	ALT6_AD14_PWM6:1;
	__IO uint32_t 	ALT7_AD15_PWM7:1;
	__IO uint32_t 	SCHMITT:8;
	__I  uint32_t 	RESERVE:8;    
} GCR_P2_MFP_T;

typedef struct
{
	__IO uint32_t 	MFP0_RXD0:1;
	__IO uint32_t 	MFP1_TXD0:1;
	__IO uint32_t 	MFP2_INT0:1;
	__IO uint32_t 	MFP3_INT1_MCLK:1;
	__IO uint32_t 	MFP4_T0_SDA:1;
	__IO uint32_t 	MFP5_T1_SCL:1;
	__IO uint32_t 	MFP6_WR_CKO:1;
	__IO uint32_t 	MFP7_RD:1;
	__IO uint32_t 	ALT0_RXD0:1;
	__IO uint32_t 	ALT1_TXD0:1;
	__IO uint32_t 	ALT2_INT0:1;
	__IO uint32_t 	ALT3_INT1_MCLK:1;
	__IO uint32_t 	ALT4_T0_SDA:1;
	__IO uint32_t 	ALT5_T1_SCL:1;
	__IO uint32_t 	ALT6_WR_CKO:1;
	__IO uint32_t 	ALT7_RD:1;
	__IO uint32_t 	SCHMITT:8;
	__I  uint32_t 	RESERVE:8;    
} GCR_P3_MFP_T;

typedef struct
{
	__IO uint32_t 	MFP0_PWM0:1;
	__IO uint32_t 	MFP1_PWM1:1;
	__IO uint32_t 	MFP2_PWM2:1;
	__IO uint32_t 	MFP3_PWM3:1;
	__IO uint32_t 	MFP4_CS:1;
	__IO uint32_t 	MFP5_ALE:1;
	__IO uint32_t 	MFP6_ICE_CLK:1;
	__IO uint32_t 	MFP7_ICE_DAT:1;
	__IO uint32_t 	ALT0_PWM0:1;
	__IO uint32_t 	ALT1_PWM1:1;
	__IO uint32_t 	ALT2_PWM2:1;
	__IO uint32_t 	ALT3_PWM3:1;
	__IO uint32_t 	ALT4_CS:1;
	__IO uint32_t 	ALT5_ALE:1;
	__IO uint32_t 	ALT6_ICE_CLK:1;
	__IO uint32_t 	ALT7_ICE_DAT:1;
	__IO uint32_t 	SCHMITT:8;
	__I  uint32_t 	RESERVE:8;    
} GCR_P4_MFP_T;

typedef __IO uint32_t GCR_REGLOCK_T;
typedef __IO uint32_t GCR_RCADJ_T;

typedef struct
{
    __IO uint32_t  INTSRC:3;
    __I  uint32_t  RESERVE:29;
} GCR_INTSRC_T;

typedef struct
{
    __IO uint32_t  NMISEL:5;
    __I  uint32_t  RESERVE0:2;
    __IO uint32_t  INT_TEST:1;
    __I  uint32_t  RESERVE1:24;
} GCR_NMISEL_T;

typedef __IO uint32_t GCR_MCUIRQ_T;

typedef struct
{
	GCR_PDID_T      PDID;   
	GCR_RSTSRC_T    RSTSRC;
	GCR_IPRSTC1_T   IPRSTC1;
	GCR_IPRSTC2_T   IPRSTC2;
	uint32_t        RESERVE0[2];
	GCR_BODCR_T     BODCR;
	uint32_t        RESERVE1[2];
	GCR_PORCR_T		PORCR;
	uint32_t        RESERVE2[2];
	GCR_P0_MFP_T    P0_MFP;
	GCR_P1_MFP_T    P1_MFP;
	GCR_P2_MFP_T    P2_MFP;
	GCR_P3_MFP_T    P3_MFP;
	GCR_P4_MFP_T    P4_MFP;
	uint32_t        RESERVE3[47];
	GCR_REGLOCK_T   REGLOCK;
	uint32_t        RESERVE4[3];
	GCR_RCADJ_T		RCADJ;
} GCR_T;

typedef struct
{
    GCR_INTSRC_T    INTSRC;
    GCR_NMISEL_T    NMISEL;
    GCR_MCUIRQ_T    MCUIRQ;   
} GCR_INT_T;

/*-------------------------- FLASH Memory Controller -------------------------*/
typedef struct
{
    __IO uint32_t  ISPEN:1;
    __IO uint32_t  BS:1;
	__I  uint32_t  RESERVE0:2;
    __IO uint32_t  CFGUEN:1;
	__IO uint32_t  LDUEN:1;
    __IO uint32_t  ISPFF:1;
    __IO uint32_t  SWRST:1;
    __IO uint32_t  PT:3;
	__I  uint32_t  RESERVE1:1;
	__IO uint32_t  ET:3;
    __I  uint32_t  RESERVE2:17;

} FMC_ISPCON_T;

typedef __IO uint32_t FMC_ISPADR_T;
typedef __IO uint32_t FMC_ISPDAT_T;

typedef struct
{
    __IO uint32_t  FCTRL:4;
    __IO uint32_t  FCEN:1;
    __IO uint32_t  FOEN:1;
    __I  uint32_t  RESERVE:26;
} FMC_ISPCMD_T;

typedef struct
{
    __IO uint32_t  ISPGO:1;
    __I  uint32_t  RESERVE:31;
} FMC_ISPTRG_T;

typedef __I uint32_t FMC_DFBADR_T;

typedef struct
{
    __IO uint32_t  FPSEN:1;
    __IO uint32_t  FATS:3;
    __IO uint32_t  L_SPEED:1;
	__I  uint32_t  RESERVE:27;
} FMC_FATCON_T;

typedef struct
{
    FMC_ISPCON_T ISPCON;
    FMC_ISPADR_T ISPADR;
    FMC_ISPDAT_T ISPDAT;
    FMC_ISPCMD_T ISPCMD;
    FMC_ISPTRG_T ISPTRG;
    FMC_DFBADR_T DFBADR;
    FMC_FATCON_T FATCON;
} FMC_T;

/*----------------------------- PWM Controller -------------------------------*/
typedef struct
{
    __IO uint32_t  CP01:8;
    __IO uint32_t  CP23:8;
    __IO uint32_t  DZI01:8;
    __IO uint32_t  DZI23:8;
} PWM_PPR_T;

typedef struct
{
    __IO uint32_t  CSR0:3;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  CSR1:3;
    __I  uint32_t  RESERVE1:1;
    __IO uint32_t  CSR2:3;
    __I  uint32_t  RESERVE2:1;
    __IO uint32_t  CSR3:3;
    __I  uint32_t  RESERVE3:17;
} PWM_CSR_T;

typedef struct
{
    __IO uint32_t  CH0EN:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  CH0INV:1;
    __IO uint32_t  CH0MOD:1;
    __IO uint32_t  DZEN01:1;
    __IO uint32_t  DZEN23:1;
    __I  uint32_t  RESERVE1:2;
    __IO uint32_t  CH1EN:1;
    __I  uint32_t  RESERVE2:1;
    __IO uint32_t  CH1INV:1;
    __IO uint32_t  CH1MOD:1;
    __I  uint32_t  RESERVE3:4;
    __IO uint32_t  CH2EN:1;
    __I  uint32_t  RESERVE4:1;
    __IO uint32_t  CH2INV:1;
    __IO uint32_t  CH2MOD:1;
    __I  uint32_t  RESERVE5:4;
    __IO uint32_t  CH3EN:1;
    __I  uint32_t  RESERVE6:1;
    __IO uint32_t  CH3INV:1;
    __IO uint32_t  CH3MOD:1;
    __I  uint32_t  RESERVE7:4;   
} PWM_PCR_T;

typedef __IO uint32_t PWM_CNR_T;

typedef __IO uint32_t PWM_CMR_T;

typedef __IO uint32_t PWM_PDR_T;

typedef struct
{
    __IO uint32_t  PWMIE0:1;
    __IO uint32_t  PWMIE1:1;
    __IO uint32_t  PWMIE2:1;
    __IO uint32_t  PWMIE3:1;
	__I  uint32_t  RESERVE:28;
} PWM_PIER_T;

typedef struct
{
    __IO uint32_t  PWMIF0:1;
    __IO uint32_t  PWMIF1:1;
    __IO uint32_t  PWMIF2:1;
    __IO uint32_t  PWMIF3:1;
	__I  uint32_t  RESERVE:28;
} PWM_PIIR_T;

typedef struct
{
    __IO uint32_t  INV0:1;
    __IO uint32_t  CRL_IE0:1;
    __IO uint32_t  CFL_IE0:1;
    __IO uint32_t  CAPCH0EN:1;
    __IO uint32_t  CAPIF0:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  CRLRI0:1;
    __IO uint32_t  CFLRI0:1;
    __I  uint32_t  RESERVE1:8;
    __IO uint32_t  INV1:1;
    __IO uint32_t  CRL_IE1:1;
    __IO uint32_t  CFL_IE1:1;
    __IO uint32_t  CAPCH1EN:1;
    __IO uint32_t  CAPIF1:1;
    __I  uint32_t  RESERVE2:1;
    __IO uint32_t  CRLRI1:1;
    __IO uint32_t  CFLRI1:1;
    __I  uint32_t  RESERVE3:8;
} PWM_CCR0_T;

typedef struct
{
    __IO uint32_t  INV2:1;
    __IO uint32_t  CRL_IE2:1;
    __IO uint32_t  CFL_IE2:1;
    __IO uint32_t  CAPCH2EN:1;
    __IO uint32_t  CAPIF2:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  CRLRI2:1;
    __IO uint32_t  CFLRI2:1;
    __I  uint32_t  RESERVE1:8;
    __IO uint32_t  INV3:1;
    __IO uint32_t  CRL_IE3:1;
    __IO uint32_t  CFL_IE3:1;
    __IO uint32_t  CAPCH3EN:1;
    __IO uint32_t  CAPIF3:1;
    __I  uint32_t  RESERVE2:1;
    __IO uint32_t  CRLRI3:1;
    __IO uint32_t  CFLRI3:1;
    __I  uint32_t  RESERVE3:8;
} PWM_CCR2_T;

typedef __IO uint32_t PWM_CRLR_T;

typedef __IO uint32_t PWM_CFLR_T;

typedef __IO uint32_t PWM_CAPENR_T;

typedef struct
{
    __IO uint32_t  PWM0:1;
    __IO uint32_t  PWM1:1;
    __IO uint32_t  PWM2:1;
    __IO uint32_t  PWM3:1;
	__I  uint32_t  RESERVE:28;
} PWM_POE_T;

typedef struct
{
    PWM_PPR_T       PPR;
    PWM_CSR_T       CSR;
    PWM_PCR_T       PCR;
    PWM_CNR_T       CNR0;
    PWM_CMR_T       CMR0;
    PWM_PDR_T       PDR0;
    PWM_CNR_T       CNR1;
    PWM_CMR_T       CMR1;
    PWM_PDR_T       PDR1;
    PWM_CNR_T       CNR2;
    PWM_CMR_T       CMR2;
    PWM_PDR_T       PDR2;
    PWM_CNR_T       CNR3;
    PWM_CMR_T       CMR3;
    PWM_PDR_T       PDR3;
    __I uint32_t    RESERVE0;
    PWM_PIER_T      PIER;
    PWM_PIIR_T      PIIR;
    __I uint32_t    RESERVE1[2];
    PWM_CCR0_T      CCR0;
    PWM_CCR2_T      CCR2;
    PWM_CRLR_T      CRLR0;
    PWM_CFLR_T      CFLR0;
    PWM_CRLR_T      CRLR1;
    PWM_CFLR_T      CFLR1;
    PWM_CRLR_T      CRLR2;
    PWM_CFLR_T      CFLR2;
    PWM_CRLR_T      CRLR3;
    PWM_CFLR_T      CFLR3;
    PWM_CAPENR_T    CAPENR;
    PWM_POE_T       POE;    
        
} PWM_T;

/*----------------------------- EBI Controller -------------------------------*/
typedef struct
{
    __IO uint32_t  ExtEN:1;
    __IO uint32_t  ExtBW16:1;
    __I  uint32_t  RESERVE0:6;
    __IO uint32_t  MCLKDIV:3;
    __I  uint32_t  RESERVE1:5;
    __IO uint32_t  ExttALE:3;
    __I  uint32_t  RESERVE2:5;
    __I  uint32_t  RESERVE3:8;
} EBI_CON_T;

typedef struct
{
    __I  uint32_t  RESERVE0:3;
    __IO uint32_t  ExttACC:5;
    __IO uint32_t  ExttAHD:3;
    __I  uint32_t  RESERVE1:1;
	__IO uint32_t  ExtIW2X:4;
	__IO uint32_t  ExtIR2W:4;
    __I  uint32_t  RESERVE2:4;
	__IO uint32_t  ExtIR2R:4;
    __I  uint32_t  RESERVE3:4;
} EBI_TIME_T;

typedef struct
{
    EBI_CON_T       CON;
    EBI_TIME_T      TIME;
} EBI_T;




/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* Peripheral and SRAM base address */
#define FLASH_BASE          ((     uint32_t)0x00000000)
#define SRAM_BASE           ((     uint32_t)0x20000000)
#define AHB_BASE            ((     uint32_t)0x50000000)
#define APB1_BASE           ((     uint32_t)0x40000000)
#define APB2_BASE           ((     uint32_t)0x40100000)

/* Peripheral memory map */
#define GPIO_BASE           (AHB_BASE       + 0x4000)
#define PORT0_BASE          (GPIO_BASE             	)
#define PORT1_BASE         	(GPIO_BASE     	+ 0x0040)
#define PORT2_BASE         	(GPIO_BASE     	+ 0x0080)
#define PORT3_BASE         	(GPIO_BASE     	+ 0x00C0)
#define PORT4_BASE         	(GPIO_BASE     	+ 0x0100)
#define PORT_DBNCECON_BASE  (GPIO_BASE     	+ 0x0180)
#define PORT_BIT_DOUT_BASE	(GPIO_BASE 		+ 0x200)

#define UART0_BASE           (APB1_BASE      + 0x50000)
#define UART1_BASE           (APB2_BASE      + 0x50000)

#define TIMER0_BASE          (APB1_BASE      + 0x10000)  
#define TIMER1_BASE          (APB1_BASE      + 0x10020)  
#define TIMER2_BASE          (APB2_BASE      + 0x10000)  
#define TIMER3_BASE          (APB2_BASE      + 0x10020)  

#define WDT_BASE       	     (APB1_BASE      + 0x4000)  

#define SPI0_BASE            (APB1_BASE      + 0x30000)
#define SPI1_BASE            (APB1_BASE      + 0x34000)

#define I2C_BASE             (APB1_BASE      + 0x20000)

#define RTC_BASE             (APB1_BASE      + 0x08000)

#define ADC_BASE             (APB1_BASE      + 0xE0000) 

#define SYSCLK_BASE          (AHB_BASE       + 0x00200)

#define GCR_BASE             (AHB_BASE       + 0x00000)

#define INT_BASE             (AHB_BASE       + 0x00300)

#define FMC_BASE             (AHB_BASE       + 0x0C000)

#define PWMA_BASE            (APB1_BASE      + 0x40000)
#define PWMB_BASE            (APB2_BASE      + 0x40000)

#define EBI_BASE             (AHB_BASE       + 0x10000)

/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
#define PORT0              	((GPIO_T *) PORT0_BASE)
#define PORT1              	((GPIO_T *) PORT1_BASE)
#define PORT2              	((GPIO_T *) PORT2_BASE)
#define PORT3              	((GPIO_T *) PORT3_BASE)
#define PORT4              	((GPIO_T *) PORT4_BASE)
#define PORT_DBNCECON       ((GPIO_DBNCECON_T *) PORT_DBNCECON_BASE)
#define PORT_BIT_DOUT		(PORT_BIT_DOUT_BASE)

#define UART0               ((UART_T *) UART0_BASE)
#define UART1               ((UART_T *) UART1_BASE)

#define TIMER0              ((TIMER_T *) TIMER0_BASE)
#define TIMER1              ((TIMER_T *) TIMER1_BASE)
#define TIMER2              ((TIMER_T *) TIMER2_BASE)
#define TIMER3              ((TIMER_T *) TIMER3_BASE)

#define WDT                 ((WDT_T *) WDT_BASE)

#define SPI0                ((SPI_T *) SPI0_BASE)
#define SPI1                ((SPI_T *) SPI1_BASE)

#define I2C                 ((I2C_T *) I2C_BASE)

#define ADC                 ((ADC_T *) ADC_BASE)

#define SYSCLK              ((SYSCLK_T *) SYSCLK_BASE)

#define SYS                 ((GCR_T *) GCR_BASE)

#define SYSINT              ((GCR_INT_T *) INT_BASE)

#define FMC                 ((FMC_T *) FMC_BASE)

#define PWMA                ((PWM_T *) PWMA_BASE)
#define PWMB                ((PWM_T *) PWMB_BASE)

#define EBI                 ((EBI_T *) EBI_BASE)

#define UNLOCKREG(x)        *((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x59;*((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x16;*((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x88
#define LOCKREG(x)          *((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x00;     

#define REGCOPY(dest, src)  *((uint32_t *)&(dest)) = *((uint32_t *)&(src))
#define CLEAR(dest) 		*((uint32_t *)&(dest)) = 0

//=============================================================================
typedef volatile unsigned char  vu8;
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))

#define outpw(port,value)	*((volatile unsigned int *)(port))=value
#define inpw(port)			(*((volatile unsigned int *)(port)))
#define outpb(port,value)	*((volatile unsigned char *)(port))=value
#define inpb(port)			(*((volatile unsigned char *)(port)))
#define outps(port,value)	*((volatile unsigned short *)(port))=value
#define inps(port)			(*((volatile unsigned short *)(port)))

#define outp32(port,value)	*((volatile unsigned int *)(port))=value
#define inp32(port)			(*((volatile unsigned int *)(port)))
#define outp8(port,value)	*((volatile unsigned char *)(port))=value
#define inp8(port)			(*((volatile unsigned char *)(port)))
#define outp16(port,value)	*((volatile unsigned short *)(port))=value
#define inp16(port)			(*((volatile unsigned short *)(port)))

#define E_SUCCESS 	0
#ifndef NULL
#define NULL        0
#endif

#define TRUE	   	1
#define FALSE   	0

#define ENABLE      1
#define DISABLE     0

// Define one bit mask
#define BIT0	0x00000001
#define BIT1	0x00000002
#define BIT2	0x00000004
#define BIT3	0x00000008
#define BIT4	0x00000010
#define BIT5	0x00000020
#define BIT6	0x00000040
#define BIT7	0x00000080
#define BIT8	0x00000100
#define BIT9	0x00000200
#define BIT10	0x00000400
#define BIT11	0x00000800
#define BIT12	0x00001000
#define BIT13	0x00002000
#define BIT14	0x00004000
#define BIT15	0x00008000
#define BIT16	0x00010000
#define BIT17	0x00020000
#define BIT18	0x00040000
#define BIT19	0x00080000
#define BIT20	0x00100000
#define BIT21	0x00200000
#define BIT22	0x00400000
#define BIT23	0x00800000
#define BIT24	0x01000000
#define BIT25	0x02000000
#define BIT26	0x04000000
#define BIT27	0x08000000
#define BIT28	0x10000000
#define BIT29	0x20000000
#define BIT30	0x40000000
#define BIT31	0x80000000

#endif
                                                                                                 

