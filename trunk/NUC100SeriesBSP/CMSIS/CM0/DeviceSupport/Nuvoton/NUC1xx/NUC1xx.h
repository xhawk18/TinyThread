/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#ifndef __NUC1xx_H__
#define __NUC1xx_H__

              
/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
*/
 
typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                             */
  HardFault_IRQn              = -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                     */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                       */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                       */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                   */

/************************ NUC1xx Interrupt Numbers ************************************************/
  BOD_IRQn                  = 0,
  WDT_IRQn                  = 1,
  EINT0_IRQn                = 2,
  EINT1_IRQn                = 3,
  GPAB_IRQn                 = 4,
  GPCDE_IRQn                = 5,
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
  SPI2_IRQn                 = 16,
  SPI3_IRQn                 = 17,
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


#include "core_cm0.h"                   /* Cortex-M0 processor and core peripherals               */
#include "system_NUC1xx.h"              /* NUC1xx System                                          */
#include "System\SysInfra.h"    

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/

/*--------------------- General Purpose Input and Ouptut ---------------------*/
typedef struct
{
    __IO uint32_t PMD0:2;
    __IO uint32_t PMD1:2;
    __IO uint32_t PMD2:2;
    __IO uint32_t PMD3:2;
    __IO uint32_t PMD4:2;
    __IO uint32_t PMD5:2;
    __IO uint32_t PMD6:2;
    __IO uint32_t PMD7:2;
    __IO uint32_t PMD8:2;
    __IO uint32_t PMD9:2;
    __IO uint32_t PMD10:2;
    __IO uint32_t PMD11:2;
    __IO uint32_t PMD12:2;
    __IO uint32_t PMD13:2;
    __IO uint32_t PMD14:2;
    __IO uint32_t PMD15:2;
} GPIO_PMD_T;

typedef __IO uint32_t GPIO_OFFD_T;

typedef __IO uint32_t GPIO_DOUT_T;

typedef __IO uint32_t GPIO_DMASK_T;

typedef __IO uint32_t GPIO_PIN_T;

typedef __IO uint32_t GPIO_DBEN_T;

typedef __IO uint32_t GPIO_IMD_T;

typedef __IO uint32_t GPIO_IEN_T;

typedef __IO uint32_t GPIO_ISRC_T;

typedef struct
{
    union {
        __IO uint32_t u32PMD;
        struct {
            __IO uint32_t PMD0:2;
            __IO uint32_t PMD1:2;
            __IO uint32_t PMD2:2;
            __IO uint32_t PMD3:2;
            __IO uint32_t PMD4:2;
            __IO uint32_t PMD5:2;
            __IO uint32_t PMD6:2;
            __IO uint32_t PMD7:2;
            __IO uint32_t PMD8:2;
            __IO uint32_t PMD9:2;
            __IO uint32_t PMD10:2;
            __IO uint32_t PMD11:2;
            __IO uint32_t PMD12:2;
            __IO uint32_t PMD13:2;
            __IO uint32_t PMD14:2;
            __IO uint32_t PMD15:2;
        } PMD;
    };

    union {
        __IO uint32_t u32OFFD;
        __IO uint32_t OFFD;
    };

    union {
        __IO uint32_t u32DOUT;
        __IO uint32_t DOUT;
    };

    union {
        __IO uint32_t u32DMASK;
        __IO uint32_t DMASK;
    };

    union {
        __IO uint32_t u32PIN;
        __IO uint32_t PIN;
    };

    union {
        __IO uint32_t u32DBEN;
        __IO uint32_t DBEN;
    };

    union {
        __IO uint32_t u32IMD;
        __IO uint32_t IMD;
    };

    union {
        __IO uint32_t u32IEN;
        __IO uint32_t IEN;
    };

    union {
        __IO uint32_t u32ISRC;
        __IO uint32_t ISRC;
    };
} GPIO_T;

typedef struct
{
    union {
        __IO uint32_t u32DBNCECON;
        struct {
            __IO uint32_t   DBCLKSEL:4;
            __IO uint32_t   DBCLKSRC:1;
            __IO uint32_t   ICLK_ON:1;
            __I  uint32_t   RESERVE:26;    
        } DBNCECON;
    };
} GPIO_DBNCECON_T;

/* GPIO PMD Bit Field Definitions */
#define GPIO_PMD_PMD15_Pos          30
#define GPIO_PMD_PMD15_Msk          (0x3ul << GPIO_PMD_PMD15_Pos)

#define GPIO_PMD_PMD14_Pos          28
#define GPIO_PMD_PMD14_Msk          (0x3ul << GPIO_PMD_PMD14_Pos)

#define GPIO_PMD_PMD13_Pos          26
#define GPIO_PMD_PMD13_Msk          (0x3ul << GPIO_PMD_PMD13_Pos)

#define GPIO_PMD_PMD12_Pos          24
#define GPIO_PMD_PMD12_Msk          (0x3ul << GPIO_PMD_PMD12_Pos)

#define GPIO_PMD_PMD11_Pos          22
#define GPIO_PMD_PMD11_Msk          (0x3ul << GPIO_PMD_PMD11_Pos)

#define GPIO_PMD_PMD10_Pos          20
#define GPIO_PMD_PMD10_Msk          (0x3ul << GPIO_PMD_PMD10_Pos)

#define GPIO_PMD_PMD9_Pos           18
#define GPIO_PMD_PMD9_Msk           (0x3ul << GPIO_PMD_PMD9_Pos)

#define GPIO_PMD_PMD8_Pos           16
#define GPIO_PMD_PMD8_Msk           (0x3ul << GPIO_PMD_PMD8_Pos)

#define GPIO_PMD_PMD7_Pos           14
#define GPIO_PMD_PMD7_Msk           (0x3ul << GPIO_PMD_PMD7_Pos)

#define GPIO_PMD_PMD6_Pos           12
#define GPIO_PMD_PMD6_Msk           (0x3ul << GPIO_PMD_PMD6_Pos)

#define GPIO_PMD_PMD5_Pos           10
#define GPIO_PMD_PMD5_Msk           (0x3ul << GPIO_PMD_PMD5_Pos)

#define GPIO_PMD_PMD4_Pos           8
#define GPIO_PMD_PMD4_Msk           (0x3ul << GPIO_PMD_PMD4_Pos)

#define GPIO_PMD_PMD3_Pos           6
#define GPIO_PMD_PMD3_Msk           (0x3ul << GPIO_PMD_PMD3_Pos)

#define GPIO_PMD_PMD2_Pos           4
#define GPIO_PMD_PMD2_Msk           (0x3ul << GPIO_PMD_PMD2_Pos)

#define GPIO_PMD_PMD1_Pos           2
#define GPIO_PMD_PMD1_Msk           (0x3ul << GPIO_PMD_PMD1_Pos)

#define GPIO_PMD_PMD0_Pos           0
#define GPIO_PMD_PMD0_Msk           (0x3ul << GPIO_PMD_PMD0_Pos)

/* GPIO OFFD Bit Field Definitions */
#define GPIO_OFFD_Pos               16
#define GPIO_OFFD_Msk               (0xFFFFul << GPIO_OFFD_Pos)

/* GPIO DOUT Bit Field Definitions */
#define GPIO_DOUT_Pos               0
#define GPIO_DOUT_Msk               (0xFFFFul << GPIO_DOUT_Pos)

/* GPIO DMASK Bit Field Definitions */
#define GPIO_DMASK_Pos              0
#define GPIO_DMASK_Msk              (0xFFFFul << GPIO_DMASK_Pos)

/* GPIO PIN Bit Field Definitions */
#define GPIO_PIN_Pos                0
#define GPIO_PIN_Msk                (0xFFFFul << GPIO_PIN_Pos)

/* GPIO DBEN Bit Field Definitions */
#define GPIO_DBEN_Pos               0
#define GPIO_DBEN_Msk               (0xFFFFul << GPIO_DBEN_Pos)

/* GPIO IMD Bit Field Definitions */
#define GPIO_IMD_Pos                0
#define GPIO_IMD_Msk                (0xFFFFul << GPIO_IMD_Pos)

/* GPIO IEN Bit Field Definitions */
#define GPIO_IEN_IR_EN_Pos          16
#define GPIO_IEN_IR_EN_Msk          (0xFFFFul << GPIO_IEN_IR_EN_Pos)

#define GPIO_IEN_IF_EN_Pos          0
#define GPIO_IEN_IF_EN_Msk          (0xFFFFul << GPIO_IEN_IF_EN_Pos)

/* GPIO ISRC Bit Field Definitions */
#define GPIO_ISRC_Pos               0
#define GPIO_ISRC_Msk               (0xFFFFul << GPIO_ISRC_Pos)

/* GPIO DBNCECON Bit Field Definitions */
#define GPIO_DBNCECON_ICLK_ON_Pos   5
#define GPIO_DBNCECON_ICLK_ON_Msk   (1ul << GPIO_DBNCECON_ICLK_ON_Pos)

#define GPIO_DBNCECON_DBCLKSRC_Pos  4
#define GPIO_DBNCECON_DBCLKSRC_Msk  (1ul << GPIO_DBNCECON_DBCLKSRC_Pos)

#define GPIO_DBNCECON_DBCLKSEL_Pos  0
#define GPIO_DBNCECON_DBCLKSEL_Msk  (0xFul << GPIO_DBNCECON_DBCLKSEL_Pos)

/* GPIO Port[x] Pin I/O Bit Output/Input Control Bit Field Definitions */
#define GPIO_GPIOx_DOUT_Pos         0
#define GPIO_GPIOx_DOUT_Msk         (1ul << GPIO_GPIOx_DOUT_Pos)

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
    __IO uint32_t  WAKE_EN:1;
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
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  LIN_RX_BREAK_IF:1;

    __IO uint32_t  RDA_INT:1;
    __IO uint32_t  THRE_INT:1;
    __IO uint32_t  RLS_INT:1;
    __IO uint32_t  MODEM_INT:1;
    __IO uint32_t  TOUT_INT:1;
    __IO uint32_t  BUF_ERR_INT:1;
    __I  uint32_t  RESERVE1:1;
    __IO uint32_t  LIN_RX_BREAK_INT:1;

    __I  uint32_t  RESERVE2:2;
    __IO uint32_t  HW_RLS_IF:1;
    __IO uint32_t  HW_MODEM_IF:1;
    __IO uint32_t  HW_TOUT_IF:1;
    __IO uint32_t  HW_BUF_ERR_IF:1;
    __IO uint32_t  RESERVE3:1;
    __IO uint32_t  HW_LIN_RX_BREAK_IF:1;

    __I  uint32_t  RESERVE4:2;
    __IO uint32_t  HW_RLS_INT:1;
    __IO uint32_t  HW_MODEM_INT:1;
    __IO uint32_t  HW_TOUT_INT:1;
    __IO uint32_t  HW_BUF_ERR_INT:1;
    __IO uint32_t  RESERVE5:1;
    __IO uint32_t  HW_LIN_RX_BREAK_INT:1;
} UART_ISR_T;

typedef struct
{
    __IO uint32_t  TOIC:7;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  DLY:8;
    __I  uint32_t  RESERVE1:16;

} UART_TOR_T;

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
    __IO uint32_t  LIN_BKFL:4;
    __I  uint32_t  RESERVE0:2;
    __IO uint32_t  LIN_RX_EN:1;
    __IO uint32_t  LIN_TX_EN:1;            
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
    
    union {
        __IO uint32_t u32DATA;
        __IO uint32_t DATA;
    };
    union {
        __IO uint32_t u32IER;
        struct {
            __IO uint32_t  RDA_IEN:1;
            __IO uint32_t  THRE_IEN:1;
            __IO uint32_t  RLS_IEN:1;
            __IO uint32_t  MODEM_IEN:1;
            __IO uint32_t  RTO_IEN:1;          
            __IO uint32_t  BUF_ERR_IEN:1;        
            __IO uint32_t  WAKE_EN:1;
            __I  uint32_t  RESERVE0:1; 
            __IO uint32_t  LIN_RX_BRK_IEN:1;
            __I  uint32_t  RESERVE1:2;
            __IO uint32_t  TIME_OUT_EN:1;
            __IO uint32_t  AUTO_RTS_EN:1;
            __IO uint32_t  AUTO_CTS_EN:1;
            __IO uint32_t  DMA_TX_EN:1;
            __IO uint32_t  DMA_RX_EN:1;
            __I  uint32_t  RESERVE2:16;
        } IER;
    };

    union {
        __IO uint32_t u32FCR;
        struct {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  RFR:1;
            __IO uint32_t  TFR:1;
            __IO uint32_t  RESERVE1:1;
            __IO uint32_t  RFITL:4;          
            __IO uint32_t  RX_DIS:1; 
            __I  uint32_t  RESERVE2 :7;
            __IO uint32_t  RTS_TRI_LEV:4;
            __I  uint32_t  RESERVE3 :4;
        } FCR;
    };

    union {
        __IO uint32_t u32LCR;
        struct {
            __IO uint32_t  WLS:2;
            __IO uint32_t  NSB:1;
            __IO uint32_t  PBE:1;
            __IO uint32_t  EPE:1;
            __IO uint32_t  SPE:1;          
            __IO uint32_t  BCB:1; 
            __I  uint32_t  RESERVE :25;
        } LCR;
    };

    union {
        __IO uint32_t u32MCR;
        struct {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  RTS:1;
            __I  uint32_t  RESERVE1:7;
            __IO uint32_t  LEV_RTS:1;
            __I  uint32_t  RESERVE2:3;          
            __IO uint32_t  RTS_ST:1; 
            __I  uint32_t  RESERVE3:18;
        } MCR;
    };


    union {
        __IO uint32_t u32MSR;
        struct {
            __IO uint32_t  DCTSF:1;
            __I  uint32_t  RESERVE0:3;
            __IO uint32_t  CTS_ST:1;
            __I  uint32_t  RESERVE1:3;
            __IO uint32_t  LEV_CTS:1;          
            __I  uint32_t  RESERVE2:23;
        } MSR;
    };


    union {
        __IO uint32_t u32FSR;
        struct {
            __IO uint32_t  RX_OVER_IF:1;
            __I  uint32_t  RESERVE0:2;
            __IO uint32_t  RS485_ADD_DETF:1;
            __IO uint32_t  PEF:1;
            __IO uint32_t  FEF:1;
            __IO uint32_t  BIF:1;
            __I  uint32_t  RESERVE1:1;
            __IO uint32_t  RX_POINTER:6;
            __IO uint32_t  RX_EMPTY:1;
            __IO uint32_t  RX_FULL:1;
            __IO uint32_t  TX_POINTER:6;
            __IO uint32_t  TX_EMPTY:1;
            __IO uint32_t  TX_FULL:1;
            __IO uint32_t  TX_OVER_IF:1;
            __I  uint32_t  RESERVE2:3;
            __IO uint32_t  TE_FLAG:1;
            __I  uint32_t  RESERVE3:3;
        } FSR;
    };

    union {
        __IO uint32_t u32ISR;
        struct {
            __IO uint32_t  RDA_IF:1;
            __IO uint32_t  THRE_IF:1;
            __IO uint32_t  RLS_IF:1;
            __IO uint32_t  MODEM_IF:1;
            __IO uint32_t  TOUT_IF:1;
            __IO uint32_t  BUF_ERR_IF:1;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  LIN_RX_BREAK_IF:1;
            __IO uint32_t  RDA_INT:1;
            __IO uint32_t  THRE_INT:1;
            __IO uint32_t  RLS_INT:1;
            __IO uint32_t  MODEM_INT:1;
            __IO uint32_t  TOUT_INT:1;
            __IO uint32_t  BUF_ERR_INT:1;
            __I  uint32_t  RESERVE1:1;
            __IO uint32_t  LIN_RX_BREAK_INT:1;
            __I  uint32_t  RESERVE2:2;
            __IO uint32_t  HW_RLS_IF:1;
            __IO uint32_t  HW_MODEM_IF:1;
            __IO uint32_t  HW_TOUT_IF:1;
            __IO uint32_t  HW_BUF_ERR_IF:1;
            __I  uint32_t  RESERVE3:1;
            __IO uint32_t  HW_LIN_RX_BREAK_IF:1;
            __I  uint32_t  RESERVE4:2;
            __IO uint32_t  HW_RLS_INT:1;
            __IO uint32_t  HW_MODEM_INT:1;
            __IO uint32_t  HW_TOUT_INT:1;
            __IO uint32_t  HW_BUF_ERR_INT:1;
            __I  uint32_t  RESERVE5:1;
            __IO uint32_t  HW_LIN_RX_BREAK_INT:1;

        } ISR;
    };

    union {
        __IO uint32_t u32TOR;
        struct {
            __IO uint32_t  TOIC:8;
            __IO uint32_t  DLY:8;
            __I  uint32_t  RESERVE1:16;
        } TOR;
    };

    union {
        __IO uint32_t u32BAUD;
        struct {
            __IO uint32_t  BRD:16;
            __I  uint32_t  RESERVE0:8;
            __IO uint32_t  DIVIDER_X:4;
            __IO uint32_t  DIV_X_ONE:1;
            __IO uint32_t  DIV_X_EN:1;
            __I  uint32_t  RESERVE1:2;
        } BAUD;
    };

    union {
        __IO uint32_t u32IRCR;
        struct {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  TX_SELECT:1;
            __I  uint32_t  RESERVE1:3;
            __IO uint32_t  INV_TX:1;
            __IO uint32_t  INV_RX:1;
            __I  uint32_t  RESERVE2:25;
        } IRCR;
    };

    union {
        __IO uint32_t u32ALTCON;
        struct {
            __IO uint32_t  LIN_BKFL:4;
            __I  uint32_t  RESERVE0:2;
            __IO uint32_t  LIN_RX_EN:1;
            __IO uint32_t  LIN_TX_EN:1;
            __IO uint32_t  RS485_NMM:1;
            __IO uint32_t  RS485_AAD:1;
            __IO uint32_t  RS485_AUD:1;
            __I  uint32_t  RESERVE1:4;
            __IO uint32_t  RS485_ADD_EN :1;
            __I  uint32_t  RESERVE2 :8;
            __IO uint32_t  ADDR_MATCH :8;
        } ALTCON;
    };


    union {
        __IO uint32_t u32FUNSEL;
        struct {
            __IO uint32_t  FUN_SEL:2;
            __I  uint32_t  RESERVE0:30;
        } FUNSEL;
    };
} UART_T;




/* UART THR Bit Field Definitions */
#define UART_THR_THR_Pos         0
#define UART_THR_THR_Msk        (0xFul << UART_THR_THR_Pos)

/* UART RBR Bit Field Definitions */
#define UART_RBR_RBR_Pos         0
#define UART_RBR_RBR_Msk        (0xFul << UART_RBR_RBR_Pos)

/* UART IER Bit Field Definitions */
#define UART_IER_DMA_RX_EN_Pos      15
#define UART_IER_DMA_RX_EN_Msk      (1ul << UART_IER_DMA_RX_EN_Pos)

#define UART_IER_DMA_TX_EN_Pos      14
#define UART_IER_DMA_TX_EN_Msk      (1ul << UART_IER_DMA_TX_EN_Pos)

#define UART_IER_AUTO_CTS_EN_Pos    13
#define UART_IER_AUTO_CTS_EN_Msk    (1ul << UART_IER_AUTO_CTS_EN_Pos)

#define UART_IER_AUTO_RTS_EN_Pos    12
#define UART_IER_AUTO_RTS_EN_Msk    (1ul << UART_IER_AUTO_RTS_EN_Pos)

#define UART_IER_TIME_OUT_EN_Pos    11
#define UART_IER_TIME_OUT_EN_Msk    (1ul << UART_IER_TIME_OUT_EN_Pos)

#define UART_IER_LIN_RX_BRK_IEN_Pos 8
#define UART_IER_LIN_RX_BRK_IEN_Msk (1ul << UART_IER_LIN_RX_BRK_IEN_Pos)

#define UART_IER_WAKE_EN_Pos        6
#define UART_IER_WAKE_EN_Msk        (1ul << UART_IER_WAKE_EN_Pos)

#define UART_IER_BUF_ERR_IEN_Pos    5
#define UART_IER_BUF_ERR_IEN_Msk    (1ul << UART_IER_BUF_ERR_IEN_Pos)

#define UART_IER_RTO_IEN_Pos        4
#define UART_IER_RTO_IEN_Msk        (1ul << UART_IER_RTO_IEN_Pos)

#define UART_IER_MODEM_IEN_Pos      3
#define UART_IER_MODEM_IEN_Msk      (1ul << UART_IER_MODEM_IEN_Pos)

#define UART_IER_RLS_IEN_Pos        2
#define UART_IER_RLS_IEN_Msk        (1ul << UART_IER_RLS_IENN_Pos)

#define UART_IER_THRE_IEN_Pos       1
#define UART_IER_THRE_IEN_Msk       (1ul << UART_IER_THRE_IEN_Pos)

#define UART_IER_RDA_IEN_Pos        0
#define UART_IER_RDA_IEN_Msk        (1ul << UART_IER_RDA_IEN_Pos)

/* UART FCR Bit Field Definitions */
#define UART_FCR_RTS_TRI_LEV_Pos    16
#define UART_FCR_RTS_TRI_LEV_Msk    (0xFul << UART_FCR_RTS_TRI_LEV_Pos)

#define UART_FCR_RX_DIS_Pos         8
#define UART_FCR_RX_DIS_Msk         (1ul << UART_FCR_RX_DIS_Pos)

#define UART_FCR_RFITL_Pos          4
#define UART_FCR_RFITL_Msk          (0xFul << UART_FCR_RFITL_Pos)

#define UART_FCR_TFR_Pos            2
#define UART_FCR_TFR_Msk            (1ul << UART_FCR_TFR_Pos)

#define UART_FCR_RFR_Pos            1
#define UART_FCR_RFR_Msk            (1ul << UART_FCR_RFR_Pos)

/* UART LCR Bit Field Definitions */
#define UART_LCR_BCB_Pos            6
#define UART_LCR_BCB_Msk            (1ul << UART_LCR_BCB_Pos)

#define UART_LCR_SPE_Pos            5
#define UART_LCR_SPE_Msk            (1ul << UART_LCR_SPE_Pos)

#define UART_LCR_EPE_Pos            4
#define UART_LCR_EPE_Msk            (1ul << UART_LCR_EPE_Pos)

#define UART_LCR_PBE_Pos            3
#define UART_LCR_PBE_Msk            (1ul << UART_LCR_PBE_Pos)

#define UART_LCR_NSB_Pos            2
#define UART_LCR_NSB_Msk            (1ul << UART_LCR_NSB_Pos)

#define UART_LCR_WLS_Pos            0
#define UART_LCR_WLS_Msk            (0x3ul << UART_LCR_WLS_Pos)

/* UART MCR Bit Field Definitions */
#define UART_MCR_RTS_ST_Pos         13
#define UART_MCR_RTS_ST_Msk         (1ul << UART_MCR_RTS_ST_Pos)

#define UART_MCR_LEV_RTS_Pos        9
#define UART_MCR_LEV_RTS_Msk        (1ul << UART_MCR_LEV_RTS_Pos)

#define UART_MCR_RTS_Pos            1
#define UART_MCR_RTS_Msk            (1ul << UART_MCR_RTS_Pos)


/* UART MSR Bit Field Definitions */
#define UART_MSR_LEV_CTS_Pos        8
#define UART_MSR_LEV_CTS_Msk        (1ul << UART_MSR_LEV_CTS_Pos)

#define UART_MSR_CTS_ST_Pos         4
#define UART_MSR_CTS_ST_Msk         (1ul << UART_MSR_CTS_ST_Pos)

#define UART_MSR_DCTSF_Pos          0
#define UART_MSR_DCTSF_Msk          (1ul << UART_MSR_DCTSF_Pos)


/* UART FSR Bit Field Definitions */
#define UART_FSR_TE_FLAG_Pos        28
#define UART_FSR_TE_FLAG_Msk        (1ul << UART_FSR_TE_FLAG_Pos)

#define UART_FSR_TX_OVER_IF_Pos     24
#define UART_FSR_TX_OVER_IF_Msk     (1ul << UART_FSR_TX_OVER_IF_Pos)

#define UART_FSR_TX_FULL_Pos        23
#define UART_FSR_TX_FULL_Msk        (1ul << UART_FSR_TX_FULL_Pos)

#define UART_FSR_TX_EMPTY_Pos       22
#define UART_FSR_TX_EMPTY_Msk       (1ul << UART_FSR_TX_EMPTY_Pos)

#define UART_FSR_TX_POINTER_Pos     16
#define UART_FSR_TX_POINTER_Msk     (0x3Ful << UART_FSR_TX_POINTER_Pos)

#define UART_FSR_RX_FULL_Pos        15
#define UART_FSR_RX_FULL_Msk        (1ul << UART_FSR_RX_FULL_Pos)

#define UART_FSR_RX_EMPTY_Pos       14
#define UART_FSR_RX_EMPTY_Msk       (1ul << UART_FSR_RX_EMPTY_Pos)

#define UART_FSR_RX_POINTER_Pos     8
#define UART_FSR_RX_POINTER_Msk     (0x3Ful << UART_FSR_RX_POINTER_Pos)

#define UART_FSR_BIF_Pos            6
#define UART_FSR_BIF_Msk            (1ul << UART_FSR_BIF_Pos)

#define UART_FSR_FEF_Pos            5
#define UART_FSR_FEF_Msk            (1ul << UART_FSR_FEF_Pos)

#define UART_FSR_PEF_Pos            4
#define UART_FSR_PEF_Msk            (1ul << UART_FSR_PEF_Pos)

#define UART_FSR_RS485_ADD_DETF_Pos 3
#define UART_FSR_RS485_ADD_DETF_Msk (1ul << UART_FSR_RS485_ADD_DETF_Pos)

#define UART_FSR_RX_OVER_IF_Pos     0
#define UART_FSR_RX_OVER_IF_Msk     (1ul << UART_FSR_RX_OVER_IF_Pos)

/* UART ISR Bit Field Definitions */
#define UART_ISR_HW_LIN_RX_BREAK_INT_Pos   31
#define UART_ISR_HW_LIN_RX_BREAK_INT_Msk   (1ul << UART_ISR_HW_LIN_RX_BREAK_INT_Pos)

#define UART_ISR_HW_BUF_ERR_INT_Pos        29
#define UART_ISR_HW_BUF_ERR_INT_Msk        (1ul << UART_ISR_HW_BUF_ERR_INT_Pos)

#define UART_ISR_HW_TOUT_INT_Pos           28
#define UART_ISR_HW_TOUT_INT_Msk           (1ul << UART_ISR_HW_TOUT_INT_Pos)

#define UART_ISR_HW_MODEM_INT_Pos          27
#define UART_ISR_HW_MODEM_INT_Msk          (1ul << UART_ISR_HW_MODEM_INT_Pos)

#define UART_ISR_HW_RLS_INT_Pos            26
#define UART_ISR_HW_RLS_INT_Msk            (1ul << UART_ISR_HW_RLS_INT_Pos)

#define UART_ISR_HW_LIN_RX_BREAK_IF_Pos    23
#define UART_ISR_HW_LIN_RX_BREAK_IF_Msk    (1ul << UART_ISR_HW_LIN_RX_BREAK_IF_Pos)

#define UART_ISR_HW_BUF_ERR_IF_Pos         21
#define UART_ISR_HW_BUF_ERR_IF_Msk         (1ul << UART_ISR_HW_BUF_ERR_IF_Pos)

#define UART_ISR_HW_TOUT_IF_Pos            20
#define UART_ISR_HW_TOUT_IF_Msk            (1ul << UART_ISR_HW_TOUT_IFF_Pos)

#define UART_ISR_HW_MODEM_IF_Pos           19
#define UART_ISR_HW_MODEM_IF_Msk           (1ul << UART_ISR_HW_MODEM_IF_Pos)

#define UART_ISR_HW_RLS_IF_Pos             18
#define UART_ISR_HW_RLS_IF_Msk             (1ul << UART_ISR_HW_RLS_IF_Pos)

#define UART_ISR_LIN_RX_BREAK_INT_Pos      15
#define UART_ISR_LIN_RX_BREAK_INT_Msk      (1ul << UART_ISR_LIN_RX_BREAK_INT_Pos)

#define UART_ISR_BUF_ERR_INT_Pos           13
#define UART_ISR_BUF_ERR_INT_Msk           (1ul << UART_ISR_BUF_ERR_INT_Pos)

#define UART_ISR_TOUT_INT_Pos              12
#define UART_ISR_TOUT_INT_Msk              (1ul << UART_ISR_TOUT_INT_Pos)

#define UART_ISR_MODEM_INT_Pos             11    
#define UART_ISR_MODEM_INT_Msk             (1ul << UART_ISR_MODEM_INT_Pos)

#define UART_ISR_RLS_INT_Pos               10    
#define UART_ISR_RLS_INT_Msk               (1ul << UART_ISR_RLS_INT_Pos)

#define UART_ISR_THRE_INT_Pos              9    
#define UART_ISR_THRE_INT_Msk              (1ul << UART_ISR_THRE_INT_Pos)

#define UART_ISR_RDA_INT_Pos               8    
#define UART_ISR_RDA_INT_Msk               (1ul << UART_ISR_RDA_INT_Pos)

#define UART_ISR_LIN_RX_BREAK_IF_Pos       7    
#define UART_ISR_LIN_RX_BREAK_IF_Msk       (1ul << UART_ISR_LIN_RX_BREAK_IF_Pos)

#define UART_ISR_BUF_ERR_IF_Pos            5    
#define UART_ISR_BUF_ERR_IF_Msk            (1ul << UART_ISR_BUF_ERR_IF_Pos)

#define UART_ISR_TOUT_IF_Pos               4    
#define UART_ISR_TOUT_IF_Msk               (1ul << UART_ISR_TOUT_IF_Pos)

#define UART_ISR_MODEM_IF_Pos              3    
#define UART_ISR_MODEM_IF_Msk              (1ul << UART_ISR_MODEM_IF_Pos)

#define UART_ISR_RLS_IF_Pos                2    
#define UART_ISR_RLS_IF_Msk                (1ul << UART_ISR_RLS_IF_Pos)

#define UART_ISR_THRE_IF_Pos               1    
#define UART_ISR_THRE_IF_Msk               (1ul << UART_ISR_THRE_IF_Pos)

#define UART_ISR_RDA_IF_Pos                0    
#define UART_ISR_RDA_IF_Msk                (1ul << UART_ISR_RDA_IF_Pos)


/* UART TOR Bit Field Definitions */
#define UART_TOR_DLY_Pos          8
#define UART_TOR_DLY_Msk          (0xFFul << UART_TOR_DLY_Pos)

#define UART_TOR_TOIC_Pos         0
#define UART_TOR_TOIC_Msk         (0xFFul << UART_TOR_TOIC_Pos)

/* UART BAUD Bit Field Definitions */
#define UART_BAUD_DIV_X_EN_Pos    29
#define UART_BAUD_DIV_X_EN_Msk    (1ul << UART_BAUD_DIV_X_EN_Pos)

#define UART_BAUD_DIV_X_ONE_Pos   28
#define UART_BAUD_DIV_X_ONE_Msk   (1ul << UART_BAUD_DIV_X_ONE_Pos)

#define UART_BAUD_DIVIDER_X_Pos   24
#define UART_BAUD_DIVIDER_X_Msk   (0xFul << UART_BAUD_DIVIDER_X_Pos)

#define UART_BAUD_BRD_Pos         0
#define UART_BAUD_BRD_Msk         (0xFFul << UART_BAUD_BRD_Pos)

/* UART IRCR Bit Field Definitions */
#define UART_IRCR_INV_RX_Pos      6
#define UART_IRCR_INV_RX_Msk     (1ul << UART_IRCR_INV_RX_Pos)

#define UART_IRCR_INV_TX_Pos      5
#define UART_IRCR_INV_TX_Msk     (1ul << UART_IRCR_INV_TX_Pos)

#define UART_IRCR_TX_SELECT_Pos   1
#define UART_IRCR_TX_SELECT_Msk   (1ul << UART_IRCR_TX_SELECT_Pos)

/* UART ALT_CSR Bit Field Definitions */
#define UART_ALT_CSR_ADDR_MATCH_Pos      24
#define UART_ALT_CSR_ADDR_MATCH_Msk     (0xFFul << UART_ALT_CSR_ADDR_MATCH_Pos)

#define UART_ALT_CSR_RS485_ADD_EN_Pos   15
#define UART_ALT_CSR_RS485_ADD_EN_Msk   (1ul << UART_ALT_CSR_RS485_ADD_EN_Pos)

#define UART_ALT_CSR_RS485_AUD_Pos      10
#define UART_ALT_CSR_RS485_AUD_Msk      (1ul << UART_ALT_CSR_RS485_AUD_Pos)

#define UART_ALT_CSR_RS485_AAD_Pos      9
#define UART_ALT_CSR_RS485_AAD_Msk      (1ul << UART_ALT_CSR_RS485_AAD_Pos)

#define UART_ALT_CSR_RS485_NMM_Pos      8
#define UART_ALT_CSR_RS485_NMM_Msk      (1ul << UART_ALT_CSR_RS485_NMM_Pos)

#define UART_ALT_CSR_LIN_TX_EN_Pos      7
#define UART_ALT_CSR_LIN_TX_EN_Msk      (1ul << UART_ALT_CSR_LIN_TX_EN_Pos)

#define UART_ALT_CSR_LIN_RX_EN_Pos      6
#define UART_ALT_CSR_LIN_RX_EN_Msk      (1ul << UART_ALT_CSR_LIN_RX_EN_Pos)

#define UART_ALT_CSR_LIN_BKFL_Pos       0
#define UART_ALT_CSR_LIN_BKFL_Msk       (0xFul << UART_ALT_CSR_LIN_BKFL_Pos)

/* UART FUN_SEL Bit Field Definitions */
#define UART_FUN_SEL_FUN_SEL_Pos        0
#define UART_FUN_SEL_FUN_SEL_Msk       (0x3ul << UART_FUN_SEL_FUN_SEL_Pos)


/*----------------------------- Timer Controller -----------------------------*/
typedef struct
{
    __IO uint32_t  PRESCALE:8;
    __I  uint32_t  RESERVE0:8;
    __IO uint32_t  TDR_EN:1;
    __I  uint32_t  RESERVE1:7;
    __IO uint32_t  CTB:1;          
    __IO uint32_t  CACT:1;        
    __IO uint32_t  CRST:1;
    __IO uint32_t  MODE:2;
    __IO uint32_t  IE:1;
    __IO uint32_t  CEN:1;
    __IO uint32_t  DBGACK_TMR:1;
} TIMER_TCSR_T;

typedef __IO uint32_t TIMER_TCMPR_T;

typedef struct
{
    __IO uint32_t  TIF:1;
    __I  uint32_t  RESERVE:31;
} TIMER_TISR_T;

typedef __IO uint32_t TIMER_TDR_T;

typedef __IO uint32_t TIMER_TCAP_T;

typedef struct
{
    __IO uint32_t  TX_PHASE:1;
    __IO uint32_t  TEX_EDGE:2;
    __IO uint32_t  TEXEN:1;
    __IO uint32_t  RSTCAPSEL:1;
    __IO uint32_t  TEXIEN:1;
    __IO uint32_t  TEXDB:1;
    __IO uint32_t  TCDB:1;
    __I  uint32_t  RESERVE:24;
} TIMER_TEXCON_T;

typedef struct
{
    __IO uint32_t  TEXIF:1;
    __I  uint32_t  RESERVE:31;
} TIMER_TEXISR;

typedef struct
{
    union {
        __IO uint32_t u32TCSR;
        struct {
            __IO uint32_t  PRESCALE:8;
            __I  uint32_t  RESERVE0:8;
            __IO uint32_t  TDR_EN:1;
            __I  uint32_t  RESERVE1:7;
            __IO uint32_t  CTB:1;          
            __IO uint32_t  CACT:1;        
            __IO uint32_t  CRST:1;
            __IO uint32_t  MODE:2;
            __IO uint32_t  IE:1;
            __IO uint32_t  CEN:1;
            __IO uint32_t  DBGACK_TMR:1;
        } TCSR;
    };

    union {
        __IO uint32_t u32TCMPR;
        __IO uint32_t TCMPR;
    };

    union {
        __IO uint32_t u32TISR;
        struct {
            __IO uint32_t  TIF:1;
            __I  uint32_t  RESERVE:31;
        } TISR;
    };

    union {
        __IO uint32_t u32TDR;
        __IO uint32_t TDR;
    };

    union {
        __IO uint32_t u32TCAP;
        __IO uint32_t TCAP;
    };

    union {
        __IO uint32_t u32TEXCON;
        struct {
            __IO uint32_t  TX_PHASE:1;
            __IO uint32_t  TEX_EDGE:2;
            __IO uint32_t  TEXEN:1;
            __IO uint32_t  RSTCAPSEL:1;
            __IO uint32_t  TEXIEN:1;
            __IO uint32_t  TEXDB:1;
            __IO uint32_t  TCDB:1;
            __I  uint32_t  RESERVE:24;
        } TEXCON;
    };

    union {
        __IO uint32_t u32TEXISR;
        struct {
            __IO uint32_t  TEXIF:1;
            __I  uint32_t  RESERVE:31;
        } TEXISR;
    };
} TIMER_T;

/* Timer TCSR Bit Field Definitions */
#define TIMER_TCSR_DBGACK_TMR_Pos   31
#define TIMER_TCSR_DBGACK_TMR_Msk   (1ul << TIMER_TCSR_DBGACK_TMR_Pos)

#define TIMER_TCSR_CEN_Pos          30
#define TIMER_TCSR_CEN_Msk          (1ul << TIMER_TCSR_CEN_Pos)

#define TIMER_TCSR_IE_Pos           29
#define TIMER_TCSR_IE_Msk           (1ul << TIMER_TCSR_IE_Pos)

#define TIMER_TCSR_MODE_Pos         27
#define TIMER_TCSR_MODE_Msk         (0x3ul << TIMER_TCSR_MODE_Pos)

#define TIMER_TCSR_CRST_Pos         26
#define TIMER_TCSR_CRST_Msk         (1ul << TIMER_TCSR_CRST_Pos)

#define TIMER_TCSR_CACT_Pos         25
#define TIMER_TCSR_CACT_Msk         (1ul << TIMER_TCSR_CACT_Pos)

#define TIMER_TCSR_CTB_Pos          24
#define TIMER_TCSR_CTB_Msk          (1ul << TIMER_TCSR_CTB_Pos)

#define TIMER_TCSR_TDR_EN_Pos       16
#define TIMER_TCSR_TDR_EN_Msk       (1ul << TIMER_TCSR_TDR_EN_Pos)

#define TIMER_TCSR_PRESCALE_Pos     0
#define TIMER_TCSR_PRESCALE_Msk     (0xFFul << TIMER_TCSR_PRESCALE_Pos)

/* Timer TCMPR Bit Field Definitions */
#define TIMER_TCMP_Pos              0
#define TIMER_TCMP_Msk              (0xFFFFFFul << TIMER_TCMP_Pos)

/* Timer TISR Bit Field Definitions */
#define TIMER_TISR_TIF_Pos          0
#define TIMER_TISR_TIF_Msk          (1ul << TIMER_TISR_TIF_Pos)

/* Timer TDR Bit Field Definitions */
#define TIMER_TDR_Pos               0
#define TIMER_TDR_Msk               (0xFFFFFFul << TIMER_TDR_Pos)

/* Timer TCAP Bit Field Definitions */
#define TIMER_TCAP_Pos              0
#define TIMER_TCAP_Msk              (0xFFFFFFul << TIMER_TCAP_Pos)

/* Timer TEXCON Bit Field Definitions */
#define TIMER_TEXCON_TCDB_Pos       7
#define TIMER_TEXCON_TCDB_Msk       (1ul << TIMER_TEXCON_TCDB_Pos)

#define TIMER_TEXCON_TEXDB_Pos      6
#define TIMER_TEXCON_TEXDB_Msk      (1ul << TIMER_TEXCON_TEXDB_Pos)

#define TIMER_TEXCON_TEXIEN_Pos     5
#define TIMER_TEXCON_TEXIEN_Msk     (1ul << TIMER_TEXCON_TEXIEN_Pos)

#define TIMER_TEXCON_RSTCAPSEL_Pos  4
#define TIMER_TEXCON_RSTCAPSEL_Msk  (1ul << TIMER_TEXCON_RSTCAPSEL_Pos)

#define TIMER_TEXCON_TEXEN_Pos      3
#define TIMER_TEXCON_TEXEN_Msk      (1ul << TIMER_TEXCON_TEXEN_Pos)

#define TIMER_TEXCON_TEX_EDGE_Pos   1
#define TIMER_TEXCON_TEX_EDGE_Msk   (0x3ul << TIMER_TEXCON_TEX_EDGE_Pos)

#define TIMER_TEXCON_TX_PHASE_Pos   0
#define TIMER_TEXCON_TX_PHASE_Msk   (1ul << TIMER_TEXCON_TX_PHASE_Pos)

/* Timer TEXISR Bit Field Definitions */
#define TIMER_TEXISR_TEXIF_Pos      0
#define TIMER_TEXISR_TEXIF_Msk      (1ul << TIMER_TEXISR_TEXIF_Pos)


/*----------------------------- WDT Controller -----------------------------*/
typedef struct
{
    __IO uint32_t  WTR:1;
    __IO uint32_t  WTRE:1;
    __IO uint32_t  WTRF:1;
    __IO uint32_t  WTIF:1;
    __IO uint32_t  WTWKE:1;
    __IO uint32_t  WTWKF:1;
    __IO uint32_t  WTIE:1;
    __IO uint32_t  WTE:1;
    __IO uint32_t  WTIS:3;
    __I  uint32_t  RESERVE1:20;
    __IO uint32_t  DBGACK_WDT:1;
} WDT_WTCR_T;

typedef struct
{
    union {
        __IO uint32_t u32WTCR;
        struct {
            __IO uint32_t  WTR:1;
            __IO uint32_t  WTRE:1;
            __IO uint32_t  WTRF:1;
            __IO uint32_t  WTIF:1;
            __IO uint32_t  WTWKE:1;
            __IO uint32_t  WTWKF:1;
            __IO uint32_t  WTIE:1;
            __IO uint32_t  WTE:1;
            __IO uint32_t  WTIS:3;
            __I  uint32_t  RESERVE1:20;
            __IO uint32_t  DBGACK_WDT:1;
        } WTCR;
    };
} WDT_T;

/* WDT WTCR Bit Field Definitions */
#define WDT_WTCR_DBGACK_WDT_Pos 31
#define WDT_WTCR_DBGACK_WDT_Msk (1ul << WDT_WTCR_DBGACK_WDT_Pos)

#define WDT_WTCR_WTIS_Pos       8  
#define WDT_WTCR_WTIS_Msk       (0x3ul << WDT_WTCR_WTIS_Pos)

#define WDT_WTCR_WTE_Pos        7  
#define WDT_WTCR_WTE_Msk        (1ul << WDT_WTCR_WTE_Pos)

#define WDT_WTCR_WTIE_Pos       6  
#define WDT_WTCR_WTIE_Msk       (1ul << WDT_WTCR_WTIE_Pos)

#define WDT_WTCR_WTWKF_Pos      5  
#define WDT_WTCR_WTWKF_Msk      (1ul << WDT_WTCR_WTWKF_Pos)

#define WDT_WTCR_WTWKE_Pos      4  
#define WDT_WTCR_WTWKE_Msk      (1ul << WDT_WTCR_WTWKE_Pos)

#define WDT_WTCR_WTIF_Pos       3  
#define WDT_WTCR_WTIF_Msk       (1ul << WDT_WTCR_WTIF_Pos)

#define WDT_WTCR_WTRF_Pos       2  
#define WDT_WTCR_WTRF_Msk       (1ul << WDT_WTCR_WTRF_Pos)

#define WDT_WTCR_WTRE_Pos       1  
#define WDT_WTCR_WTRE_Msk       (1ul << WDT_WTCR_WTRE_Pos)

#define WDT_WTCR_WTR_Pos        0  
#define WDT_WTCR_WTR_Msk        (1ul << WDT_WTCR_WTR_Pos)


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
    __IO uint32_t  REORDER:2;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  TWOB:1;
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
    __IO uint32_t  SSR:2;
    __IO uint32_t  SS_LVL:1;
    __IO uint32_t  AUTOSS:1;
    __IO uint32_t  SS_LTRIG:1;
    __I  uint32_t  LTRIG_FLAG:1;
    __I  uint32_t  RESERVE:26;
} SPI_SSR_T;


typedef __I  uint32_t   SPI_RX_T;
typedef __O  uint32_t   SPI_TX_T;
typedef __IO uint32_t   SPI_VARCLK_T;

typedef struct
{
    __IO uint32_t  TX_DMA_GO:1;
    __IO uint32_t  RX_DMA_GO:1;
    __I  uint32_t  RESERVE:30;
} SPI_DMA_T;

typedef struct
{
    __IO uint32_t  DIV_ONE:1;
    __I  uint32_t  RESERVE0:7;
    __IO uint32_t  NOSLVSEL:1;
    __IO uint32_t  SLV_ABORT:1;
    __IO uint32_t  SSTA_INTEN:1;
    __IO uint32_t  SLV_START_INTSTS:1;
    __I  uint32_t  RESERVE1:20;
} SPI_CNTRL2_T;

typedef struct
{
    union {
        __IO uint32_t u32CNTRL;
        struct {
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
            __IO uint32_t  REORDER:2;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  TWOB:1;
            __IO uint32_t  VARCLK_EN:1;
            __I  uint32_t  RESERVE1:8;
        } CNTRL;
    };

    union {
        __IO uint32_t u32DIVIDER;
        struct {
            __IO uint32_t  DIVIDER:16;
            __IO uint32_t  DIVIDER2:16;
        } DIVIDER;
    };

    union {
        __IO uint32_t u32SSR;
        struct {
            __IO uint32_t  SSR:2;
            __IO uint32_t  SS_LVL:1;
            __IO uint32_t  AUTOSS:1;
            __IO uint32_t  SS_LTRIG:1;
            __I  uint32_t  LTRIG_FLAG:1;
            __I  uint32_t  RESERVE:26;
        } SSR;
    };

    __I uint32_t RESERVE0;

    union {
        __I uint32_t u32RX[2];
        __I uint32_t RX[2];
    };

    __I uint32_t RESERVE1;
    __I uint32_t RESERVE2;

    union {
        __O uint32_t u32TX[2];
        __O uint32_t TX[2];
    };

    __I uint32_t RESERVE3;
    __I uint32_t RESERVE4;
    __I uint32_t RESERVE5;

    union {
        __IO uint32_t u32VARCLK;
        __IO uint32_t VARCLK;
    };

    union {
        __IO uint32_t u32DMA;
        struct {
            __IO uint32_t  TX_DMA_GO:1;
            __IO uint32_t  RX_DMA_GO:1;
            __I  uint32_t  RESERVE:30;
        } DMA;
    };

    union {
        __IO uint32_t u32CNTRL2;
        struct {
            __IO uint32_t  DIV_ONE:1;
            __I  uint32_t  RESERVE0:7;
            __IO uint32_t  NOSLVSEL:1;
            __IO uint32_t  SLV_ABORT:1;
            __IO uint32_t  SSTA_INTEN:1;
            __IO uint32_t  SLV_START_INTSTS:1;
            __I  uint32_t  RESERVE1:20;
        } CNTRL2;
    };

} SPI_T;


/* SPI_CNTRL Bit Field Definitions */
#define SPI_CNTRL_VARCLK_EN_Pos    23
#define SPI_CNTRL_VARCLK_EN_Msk    (1ul << SPI_CNTRL_VARCLK_EN_Pos)

#define SPI_CNTRL_TWOB_Pos         22
#define SPI_CNTRL_TWOB_Msk         (1ul << SPI_CNTRL_TWOB_Pos)

#define SPI_CNTRL_REORDER_Pos      19
#define SPI_CNTRL_REORDER_Msk      (3ul << SPI_CNTRL_REORDER_Pos)

#define SPI_CNTRL_SLAVE_Pos        18
#define SPI_CNTRL_SLAVE_Msk        (1ul << SPI_CNTRL_SLAVE_Pos)

#define SPI_CNTRL_IE_Pos           17
#define SPI_CNTRL_IE_Msk           (1ul << SPI_CNTRL_IE_Pos)

#define SPI_CNTRL_IF_Pos           16
#define SPI_CNTRL_IF_Msk           (1ul << SPI_CNTRL_IF_Pos)

#define SPI_CNTRL_SP_CYCLE_Pos     12
#define SPI_CNTRL_SP_CYCLE_Msk     (0xFul << SPI_CNTRL_SP_CYCLE_Pos)

#define SPI_CNTRL_CLKP_Pos         11
#define SPI_CNTRL_CLKP_Msk         (1ul << SPI_CNTRL_CLKP_Pos)

#define SPI_CNTRL_LSB_Pos          10
#define SPI_CNTRL_LSB_Msk          (1ul << SPI_CNTRL_LSB_Pos)

#define SPI_CNTRL_TX_NUM_Pos       8
#define SPI_CNTRL_TX_NUM_Msk       (3ul << SPI_CNTRL_TX_NUM_Pos)

#define SPI_CNTRL_TX_BIT_LEN_Pos   3
#define SPI_CNTRL_TX_BIT_LEN_Msk   (0x1Ful << SPI_CNTRL_TX_BIT_LEN_Pos)

#define SPI_CNTRL_TX_NEG_Pos       2
#define SPI_CNTRL_TX_NEG_Msk       (1ul << SPI_CNTRL_TX_NEG_Pos)

#define SPI_CNTRL_RX_NEG_Pos       1
#define SPI_CNTRL_RX_NEG_Msk       (1ul << SPI_CNTRL_RX_NEG_Pos)

#define SPI_CNTRL_GO_BUSY_Pos      0
#define SPI_CNTRL_GO_BUSY_Msk      (1ul << SPI_CNTRL_GO_BUSY_Pos)


/* SPI_DIVIDER Bit Field Definitions */
#define SPI_DIVIDER_DIVIDER2_Pos   16
#define SPI_DIVIDER_DIVIDER2_Msk   (0xFFFFul << SPI_DIVIDER_DIVIDER2_Pos)

#define SPI_DIVIDER_DIVIDER_Pos    0
#define SPI_DIVIDER_DIVIDER_Msk    (0xFFFFul << SPI_DIVIDER_DIVIDER_Pos)

/* SPI_SSR Bit Field Definitions */
#define SPI_SSR_LTRIG_FLAG_Pos     5
#define SPI_SSR_LTRIG_FLAG_Msk     (1ul << SPI_SSR_LTRIG_FLAG_Pos)

#define SPI_SSR_SS_LTRIG_Pos       4
#define SPI_SSR_SS_LTRIG_Msk       (1ul << SPI_SSR_SS_LTRIG_Pos)

#define SPI_SSR_AUTOSS_Pos         3
#define SPI_SSR_AUTOSS_Msk         (1ul << SPI_SSR_AUTOSS_Pos)

#define SPI_SSR_SS_LVL_Pos         2
#define SPI_SSR_SS_LVL_Msk         (1ul << SPI_SSR_SS_LVL_Pos)

#define SPI_SSR_SSR_Pos            0
#define SPI_SSR_SSR_Msk            (3ul << SPI_SSR_SSR_Pos)

/* SPI_DMA Bit Field Definitions */
#define SPI_DMA_RX_DMA_GO_Pos   1
#define SPI_DMA_RX_DMA_GO_Msk   (1ul << SPI_DMA_RX_DMA_GO_Pos)

#define SPI_DMA_TX_DMA_GO_Pos   0
#define SPI_DMA_TX_DMA_GO_Msk   (1ul << SPI_DMA_TX_DMA_GO_Pos)

/* SPI_CNTRL2 Bit Field Definitions */
#define SPI_CNTRL2_SLV_START_INTSTS_Pos   11
#define SPI_CNTRL2_SLV_START_INTSTS_Msk   (1ul << SPI_CNTRL2_SLV_START_INTSTS_Pos)

#define SPI_CNTRL2_SSTA_INTEN_Pos   10
#define SPI_CNTRL2_SSTA_INTEN_Msk   (1ul << SPI_CNTRL2_SSTA_INTEN_Pos)

#define SPI_CNTRL2_SLV_ABORT_Pos    9
#define SPI_CNTRL2_SLV_ABORT_Msk    (1ul << SPI_CNTRL2_SLV_ABORT_Pos)

#define SPI_CNTRL2_NOSLVSEL_Pos     8
#define SPI_CNTRL2_NOSLVSEL_Msk     (1ul << SPI_CNTRL2_NOSLVSEL_Pos)

#define SPI_CNTRL2_DIV_ONE_Pos      0
#define SPI_CNTRL2_DIV_ONE_Msk      (1ul << SPI_CNTRL2_DIV_ONE_Pos)


/*------------------------------ I2C Controller ------------------------------*/
typedef struct
{
    __I  uint32_t  RESERVE0:2;
    __IO uint32_t  AA:1;
    __IO uint32_t  SI:1;
    __IO uint32_t  STO:1;
    __IO uint32_t  STA:1;
    __IO uint32_t  ENS1:1;
    __IO uint32_t  EI:1;
    __I  uint32_t  RESERVE1:24;
} I2C_I2CON_T;

typedef struct
{
    __IO uint32_t  GC:1;
    __IO uint32_t  I2CADDR:7;
    __I  uint32_t  RESERVE:24;
} I2C_I2CADDR_T;

typedef __IO uint32_t I2C_I2CDAT_T;

typedef __I  uint32_t I2C_I2CSTATUS_T;

typedef __IO uint32_t I2C_I2CLK_T;

typedef struct
{
    __IO uint32_t  TIF:1;
    __IO uint32_t  DIV4:1;
    __IO uint32_t  ENTI:1;
    __I  uint32_t  RESERVE:29;
} I2C_I2CTOC_T;

typedef struct
{
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  I2CADM:7;
    __I  uint32_t  RESERVE1:24;
} I2C_I2CADM_T;

typedef struct
{
    union 
    {
        __IO uint32_t u32I2CON;
        
        struct 
        {
            __I  uint32_t  RESERVE0:2;
            __IO uint32_t  AA:1;
            __IO uint32_t  SI:1;
            __IO uint32_t  STO:1;
            __IO uint32_t  STA:1;
            __IO uint32_t  ENS1:1;
            __IO uint32_t  EI:1;
            __I  uint32_t  RESERVE1:24;
        } I2CON;
    };

    union 
    {
        __IO uint32_t u32I2CADDR0;
        
        struct 
        {
            __IO uint32_t  GC:1;
            __IO uint32_t  I2CADDR:7;
            __I  uint32_t  RESERVE:24;
        } I2CADDR0;
    };

    union 
    {
        __IO uint32_t u32I2CDAT;
        __IO uint32_t I2CDAT;
    };

    union 
    {
        __I uint32_t u32I2CSTATUS;
        __I uint32_t I2CSTATUS;
    };
    
    union 
    {
        __IO uint32_t u32I2CLK;
        __IO uint32_t I2CLK;
    };

    union 
    {
        __IO uint32_t u32I2CTOC;
        
        struct 
        {
            __IO uint32_t  TIF:1;
            __IO uint32_t  DIV4:1;
            __IO uint32_t  ENTI:1;
            __I  uint32_t  RESERVE:29;
        } I2CTOC;
    };

    union 
    {
        __IO uint32_t u32I2CADDR1;
        
        struct 
        {
            __IO uint32_t  GC:1;
            __IO uint32_t  I2CADDR:7;
            __I  uint32_t  RESERVE:24;
        } I2CADDR1;
    };
    
    union 
    {
        __IO uint32_t u32I2CADDR2;
        
        struct 
        {
            __IO uint32_t  GC:1;
            __IO uint32_t  I2CADDR:7;
            __I  uint32_t  RESERVE:24;
        } I2CADDR2;
    };    
    
    union 
    {
        __IO uint32_t u32I2CADDR3;
        
        struct 
        {
            __IO uint32_t  GC:1;
            __IO uint32_t  I2CADDR:7;
            __I  uint32_t  RESERVE:24;
        } I2CADDR3;
    }; 
    
    union 
    {
        __IO uint32_t u32I2CADM0;
        
        struct 
        {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  I2CADM:7;
            __I  uint32_t  RESERVE1:24;
        } I2CADM0;
    };     
    
    union 
    {
        __IO uint32_t u32I2CADM1;
        
        struct 
        {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  I2CADM:7;
            __I  uint32_t  RESERVE1:24;
        } I2CADM1;
    };

    union 
    {
        __IO uint32_t u32I2CADM2;
        
        struct 
        {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  I2CADM:7;
            __I  uint32_t  RESERVE1:24;
        } I2CADM2;
    };
       
    union 
    {
        __IO uint32_t u32I2CADM3;
        
        struct 
        {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  I2CADM:7;
            __I  uint32_t  RESERVE1:24;
        } I2CADM3;
    };       
} I2C_T;

/* I2C I2CON Bit Field Definitions */
#define I2C_I2CON_EI_Pos                        7
#define I2C_I2CON_EI_Msk                        (1ul << I2C_I2CON_EI_Pos)

#define I2C_I2CON_ENS1_Pos                      6
#define I2C_I2CON_ENS1_Msk                      (1ul << I2C_I2CON_ENS1_Pos)

#define I2C_I2CON_STA_Pos                       5
#define I2C_I2CON_STA_Msk                       (1ul << I2C_I2CON_STA_Pos)

#define I2C_I2CON_STO_Pos                       4
#define I2C_I2CON_STO_Msk                       (1ul << I2C_I2CON_STO_Pos)

#define I2C_I2CON_SI_Pos                        3
#define I2C_I2CON_SI_Msk                        (1ul << I2C_I2CON_SI_Pos)

#define I2C_I2CON_AA_Pos                        2
#define I2C_I2CON_AA_Msk                        (1ul << I2C_I2CON_AA_Pos)

/* I2C I2CADDR Bit Field Definitions */
#define I2C_I2CADDR_I2CADDR_Pos                 1
#define I2C_I2CADDR_I2CADDR_Msk                 (0x7Ful << I2C_I2CADDR_I2CADDR_Pos)

#define I2C_I2CADDR_GC_Pos                      0
#define I2C_I2CADDR_GC_Msk                      (1ul << I2C_I2CADDR_GC_Pos)

/* I2C I2CDAT Bit Field Definitions */
#define I2C_I2CDAT_I2CDAT_Pos                   0
#define I2C_I2CDAT_I2CDAT_Msk                   (0xFFul << I2C_I2CDAT_I2CDAT_Pos)

/* I2C I2CSTATUS Bit Field Definitions */
#define I2C_I2CSTATUS_I2CSTATUS_Pos             0
#define I2C_I2CSTATUS_I2CSTATUS_Msk             (0xFFul << I2C_I2CSTATUS_I2CSTATUS_Pos)

/* I2C I2CLK Bit Field Definitions */
#define I2C_I2CLK_I2CLK_Pos                     0
#define I2C_I2CLK_I2CLK_Msk                     (0xFFul << I2C_I2CLK_I2CLK_Pos)

/* I2C I2CTOC Bit Field Definitions */
#define I2C_I2CTOC_ENTI_Pos                     2
#define I2C_I2CTOC_ENTI_Msk                     (1ul << I2C_I2CTOC_ENTI_Pos)

#define I2C_I2CTOC_DIV4_Pos                     1
#define I2C_I2CTOC_DIV4_Msk                     (1ul << I2C_I2CTOC_DIV4_Pos)

#define I2C_I2CTOC_TIF_Pos                      0
#define I2C_I2CTOC_TIF_Msk                      (1ul << I2C_I2CTOC_TIF_Pos)

/* I2C I2CADM Bit Field Definitions */
#define I2C_I2CADM_I2CADM_Pos                   1
#define I2C_I2CADM_I2CADM_Msk                   (0x7Ful << I2C_I2CADM_I2CADM_Pos)


/*----------------------------- RTC Controller -------------------------------*/
typedef __IO uint32_t RTC_INIR_T;

typedef struct
{
    __IO uint32_t  AER:16;
    __I  uint32_t  ENF:1;
    __I  uint32_t  RESERVE1:15;
} RTC_AER_T;

typedef struct
{
    __IO uint32_t  FRACTION:6;
    __I  uint32_t  RESERVE0:2;
    __IO uint32_t  INTEGER:4;
    __I  uint32_t  RESERVE1:20;
} RTC_FCR_T;

typedef struct
{
    __IO uint32_t  SEC1:4;
    __IO uint32_t  SEC10:3;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  MIN1:4;
    __IO uint32_t  MIN10:3;
    __I  uint32_t  RESERVE1:1;
    __IO uint32_t  HR1:4;
    __IO uint32_t  HR10:2;
    __I  uint32_t  RESERVE2:10;
} RTC_TLR_T;

typedef struct
{
    __IO uint32_t  DAY1:4;
    __IO uint32_t  DAY10:2;
    __I  uint32_t  RESERVE0:2;
    __IO uint32_t  MON1:4;
    __IO uint32_t  MON10:1;
    __I  uint32_t  RESERVE1:3;
    __IO uint32_t  YEAR1:4;
    __IO uint32_t  YEAR10:4;
    __I  uint32_t  RESERVE2:8;
} RTC_CLR_T;

typedef struct
{
    __IO uint32_t  HR24_HR12:1;
    __I  uint32_t  RESERVE:31;
} RTC_TSSR_T;

typedef struct
{
    __IO uint32_t  DWR:3;
    __I  uint32_t  RESERVE:29;
} RTC_DWR_T;

typedef RTC_TLR_T   RTC_TAR_T;

typedef RTC_CLR_T   RTC_CAR_T;

typedef struct
{
    __IO uint32_t  LIR:1;
    __I  uint32_t  RESERVE:31;
} RTC_LIR_T;

typedef struct
{
    __IO uint32_t  AIER:1;
    __IO uint32_t  TIER:1;
    __I  uint32_t  RESERVE:30;
} RTC_RIER_T;

typedef struct
{
    __IO uint32_t  AIF:1;
    __IO uint32_t  TIF:1;
    __I  uint32_t  RESERVE:30;
} RTC_RIIR_T;

typedef struct
{
    __IO uint32_t  TTR:3;
    __IO uint32_t  TWKE:1;
    __I  uint32_t  RESERVE:28;
} RTC_TTR_T;

typedef struct
{
    union {
        __IO uint32_t u32INIR;
        __IO uint32_t INIR;
    };

    union {
        __IO uint32_t u32AER;
        struct {
            __IO uint32_t  AER:16;
            __I  uint32_t  ENF:1;
            __I  uint32_t  RESERVE1:15;
        } AER;
    };

    union {
        __IO uint32_t u32FCR;
        struct {
            __IO uint32_t  FRACTION:6;
            __I  uint32_t  RESERVE0:2;
            __IO uint32_t  INTEGER:4;
            __I  uint32_t  RESERVE1:20;
        } FCR;
    };

    union {
        __IO uint32_t u32TLR;
        struct {
            __IO uint32_t  SEC1:4;
            __IO uint32_t  SEC10:3;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  MIN1:4;
            __IO uint32_t  MIN10:3;
            __I  uint32_t  RESERVE1:1;
            __IO uint32_t  HR1:4;
            __IO uint32_t  HR10:2;
            __I  uint32_t  RESERVE2:10;
        } TLR;
    };

    union {
        __IO uint32_t u32CLR;
        struct {
            __IO uint32_t  DAY1:4;
            __IO uint32_t  DAY10:2;
            __I  uint32_t  RESERVE0:2;
            __IO uint32_t  MON1:4;
            __IO uint32_t  MON10:1;
            __I  uint32_t  RESERVE1:3;
            __IO uint32_t  YEAR1:4;
            __IO uint32_t  YEAR10:4;
            __I  uint32_t  RESERVE2:8;
        } CLR;
    };

    union {
        __IO uint32_t u32TSSR;
        struct {
            __IO uint32_t  HR24_HR12:1;
            __I  uint32_t  RESERVE:31;
        } TSSR;
    };

    union {
        __IO uint32_t u32DWR;
        struct {
            __IO uint32_t  DWR:3;
            __I  uint32_t  RESERVE:29;
        } DWR;
    };

    union {
        __IO uint32_t u32TAR;
        struct {
            __IO uint32_t  SEC1:4;
            __IO uint32_t  SEC10:3;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  MIN1:4;
            __IO uint32_t  MIN10:3;
            __I  uint32_t  RESERVE1:1;
            __IO uint32_t  HR1:4;
            __IO uint32_t  HR10:2;
            __I  uint32_t  RESERVE2:10;
        } TAR;
    };

    union {
        __IO uint32_t u32CAR;
        struct {
            __IO uint32_t  DAY1:4;
            __IO uint32_t  DAY10:2;
            __I  uint32_t  RESERVE0:2;
            __IO uint32_t  MON1:4;
            __IO uint32_t  MON10:1;
            __I  uint32_t  RESERVE1:3;
            __IO uint32_t  YEAR1:4;
            __IO uint32_t  YEAR10:4;
            __I  uint32_t  RESERVE2:8;
        } CAR;
    };

    union {
        __IO uint32_t u32LIR;
        struct {
            __IO uint32_t  LIR:1;
            __I  uint32_t  RESERVE:31;
        } LIR;
    };

    union {
        __IO uint32_t u32RIER;
        struct {
            __IO uint32_t  AIER:1;
            __IO uint32_t  TIER:1;
            __I  uint32_t  RESERVE:30;
        } RIER;
    };

    union {
        __IO uint32_t u32RIIR;
        struct {
            __IO uint32_t  AIF:1;
            __IO uint32_t  TIF:1;
            __I  uint32_t  RESERVE:30;
        } RIIR;
    };

    union {
        __IO uint32_t u32TTR;
        struct {
            __IO uint32_t  TTR:3;
            __IO uint32_t  TWKE:1;
            __I  uint32_t  RESERVE:28;
        } TTR;
    };
} RTC_T;

/* RTC INIR Bit Field Definitions */
#define RTC_INIR_INIR_Pos       0  
#define RTC_INIR_INIR_Msk       (0xFFFFFFFFul << RTC_INIR_INIR_Pos)

#define RTC_INIR_ACTIVE_Pos     0  
#define RTC_INIR_ACTIVE_Msk     (1ul << RTC_INIR_ACTIVE_Pos)

/* RTC AER Bit Field Definitions */
#define RTC_AER_ENF_Pos         16  
#define RTC_AER_ENF_Msk         (1ul << RTC_AER_ENF_Pos)

#define RTC_AER_AER_Pos         0  
#define RTC_AER_AER_Msk         (0xFFFFul << RTC_AER_AER_Pos)

/* RTC FCR Bit Field Definitions */
#define RTC_FCR_INTEGER_Pos     8  
#define RTC_FCR_INTEGER_Msk     (0xFul << RTC_FCR_INTEGER_Pos)

#define RTC_FCR_FRACTION_Pos    0  
#define RTC_FCR_FRACTION_Msk    (0x3Ful << RTC_FCR_FRACTION_Pos)

/* RTC TLR Bit Field Definitions */
#define RTC_TLR_10HR_Pos        20  
#define RTC_TLR_10HR_Msk        (0x3ul << RTC_TLR_10HR_Pos)

#define RTC_TLR_1HR_Pos         16  
#define RTC_TLR_1HR_Msk         (0xFul << RTC_TLR_1HR_Pos)

#define RTC_TLR_10MIN_Pos       12  
#define RTC_TLR_10MIN_Msk       (0x7ul << RTC_TLR_10MIN_Pos)

#define RTC_TLR_1MIN_Pos        8  
#define RTC_TLR_1MIN_Msk        (0xFul << RTC_TLR_1MIN_Pos)

#define RTC_TLR_10SEC_Pos       4  
#define RTC_TLR_10SEC_Msk       (0x7ul << RTC_TLR_10SEC_Pos)

#define RTC_TLR_1SEC_Pos        0  
#define RTC_TLR_1SEC_Msk        (0xFul << RTC_TLR_1SEC_Pos)

/* RTC CLR Bit Field Definitions */
#define RTC_CLR_10YEAR_Pos      20  
#define RTC_CLR_10YEAR_Msk      (0xFul << RTC_CLR_10YEAR_Pos)

#define RTC_CLR_1YEAR_Pos       16  
#define RTC_CLR_1YEAR_Msk       (0xFul << RTC_CLR_1YEAR_Pos)

#define RTC_CLR_10MON_Pos       12  
#define RTC_CLR_10MON_Msk       (1ul << RTC_CLR_10MON_Pos)

#define RTC_CLR_1MON_Pos        8  
#define RTC_CLR_1MON_Msk        (0xFul << RTC_CLR_1MON_Pos)

#define RTC_CLR_10DAY_Pos       4  
#define RTC_CLR_10DAY_Msk       (0x3ul << RTC_CLR_10DAY_Pos)

#define RTC_CLR_1DAY_Pos        0  
#define RTC_CLR_1DAY_Msk        (0xFul << RTC_CLR_1DAY_Pos)

/* RTC TSSR Bit Field Definitions */
#define RTC_TSSR_24H_12H_Pos    0  
#define RTC_TSSR_24H_12H_Msk    (1ul << RTC_TSSR_24H_12H_Pos)

/* RTC DWR Bit Field Definitions */
#define RTC_DWR_DWR_Pos         0  
#define RTC_DWR_DWR_Msk         (0x7ul << RTC_DWR_DWR_Pos)

/* RTC TAR Bit Field Definitions */
#define RTC_TAR_10HR_Pos        20  
#define RTC_TAR_10HR_Msk        (0x3ul << RTC_TAR_10HR_Pos)

#define RTC_TAR_1HR_Pos         16  
#define RTC_TAR_1HR_Msk         (0xFul << RTC_TAR_1HR_Pos)

#define RTC_TAR_10MIN_Pos       12  
#define RTC_TAR_10MIN_Msk       (0x7ul << RTC_TAR_10MIN_Pos)

#define RTC_TAR_1MIN_Pos        8  
#define RTC_TAR_1MIN_Msk        (0xFul << RTC_TAR_1MIN_Pos)

#define RTC_TAR_10SEC_Pos       4  
#define RTC_TAR_10SEC_Msk       (0x7ul << RTC_TAR_10SEC_Pos)

#define RTC_TAR_1SEC_Pos        0  
#define RTC_TAR_1SEC_Msk        (0xFul << RTC_TAR_1SEC_Pos)

/* RTC CAR Bit Field Definitions */
#define RTC_CAR_10YEAR_Pos      20  
#define RTC_CAR_10YEAR_Msk      (0xFul << RTC_CAR_10YEAR_Pos)

#define RTC_CAR_1YEAR_Pos       16  
#define RTC_CAR_1YEAR_Msk       (0xFul << RTC_CAR_1YEAR_Pos)

#define RTC_CAR_10MON_Pos       12  
#define RTC_CAR_10MON_Msk       (1ul << RTC_CAR_10MON_Pos)

#define RTC_CAR_1MON_Pos        8  
#define RTC_CAR_1MON_Msk        (0xFul << RTC_CAR_1MON_Pos)

#define RTC_CAR_10DAY_Pos       4  
#define RTC_CAR_10DAY_Msk       (0x3ul << RTC_CAR_10DAY_Pos)

#define RTC_CAR_1DAY_Pos        0  
#define RTC_CAR_1DAY_Msk        (0xFul << RTC_CAR_1DAY_Pos)

/* RTC LIR Bit Field Definitions */
#define RTC_LIR_LIR_Pos         0  
#define RTC_LIR_LIR_Msk         (1ul << RTC_LIR_LIR_Pos)

/* RTC RIER Bit Field Definitions */
#define RTC_RIER_TIER_Pos       1  
#define RTC_RIER_TIER_Msk       (1ul << RTC_RIER_TIER_Pos)

#define RTC_RIER_AIER_Pos       0  
#define RTC_RIER_AIER_Msk       (1ul << RTC_RIER_AIER_Pos)

/* RTC RIIR Bit Field Definitions */
#define RTC_RIIR_TIF_Pos        1  
#define RTC_RIIR_TIF_Msk        (1ul << RTC_RIIR_TIF_Pos)

#define RTC_RIIR_AIF_Pos        0  
#define RTC_RIIR_AIF_Msk        (1ul << RTC_RIIR_AIF_Pos)

/* RTC TTR Bit Field Definitions */
#define RTC_TTR_TWKE_Pos        3  
#define RTC_TTR_TWKE_Msk        (1ul << RTC_TTR_TWKE_Pos)

#define RTC_TTR_TTR_Pos         0  
#define RTC_TTR_TTR_Msk         (0x7ul << RTC_TTR_TTR_Pos)


/*----------------------------- ADC Controller -------------------------------*/
typedef struct
{
    __I  uint32_t  RSLT:16;
    __I  uint32_t  OVERRUN:1;
    __I  uint32_t  VALID:1;
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
    __IO uint32_t  PTEN:1;
    __IO uint32_t  DIFFEN:1;
    __IO uint32_t  ADST:1;
    __I  uint32_t  RESERVE0:19;
    __IO uint32_t  DMOF:1;
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
    __I  uint32_t  BUSY:1;
    __I  uint32_t  CHANNEL:3;
    __I  uint32_t  RESERVE0:1;
    __I  uint32_t  VALID:8;
    __I  uint32_t  OVERRUN:8;
    __I  uint32_t  RESERVE1:8;
} ADC_ADSR_T;

typedef struct
{
    __IO uint32_t  CALEN:1;
    __I  uint32_t  CALDONE:1;
    __I  uint32_t  RESERVE:30;
} ADC_ADCALR_T;

typedef struct
{
    __IO uint32_t  AD_PDMA:12;
    __I  uint32_t  RESERVE:20;
} ADC_ADPDMA_T;

typedef struct
{
    union {
        __I uint32_t u32ADDR[8];
        struct {
            __I  uint32_t  RSLT:16;
            __I  uint32_t  OVERRUN:1;
            __I  uint32_t  VALID:1;
            __I  uint32_t  RESERVE1:14;
        } ADDR[8];
    };
    
    union {
        __IO uint32_t u32ADCR;
        struct {
            __IO uint32_t  ADEN:1;
            __IO uint32_t  ADIE:1;
            __IO uint32_t  ADMD:2;
            __IO uint32_t  TRGS:2;
            __IO uint32_t  TRGCOND:2;
            __IO uint32_t  TRGEN:1;
            __IO uint32_t  PTEN:1;
            __IO uint32_t  DIFFEN:1;
            __IO uint32_t  ADST:1;
            __I  uint32_t  RESERVE0:19;
            __IO uint32_t  DMOF:1;
        } ADCR;
    };
    
    union {
        __IO uint32_t u32ADCHER;
        struct {
            __IO uint32_t  CHEN:8;
            __IO uint32_t  PRESEL:2;
            __I  uint32_t  RESERVE:22;
        } ADCHER;
    };
    
    union {
        __IO uint32_t u32ADCMPR[2];
        struct {
            __IO uint32_t  CMPEN:1;
            __IO uint32_t  CMPIE:1;
            __IO uint32_t  CMPCOND:1;
            __IO uint32_t  CMPCH:3;
            __I  uint32_t  RESERVE0:2;
            __IO uint32_t  CMPMATCNT:4;
            __I  uint32_t  RESERVE1:4;
            __IO uint32_t  CMPD:12;
            __I  uint32_t  RESERVE2:4;
        } ADCMPR[2];
    };
    
    union {
        __IO uint32_t u32ADSR;
        struct {
            __IO uint32_t  ADF:1;
            __IO uint32_t  CMPF0:1;
            __IO uint32_t  CMPF1:1;
            __I  uint32_t  BUSY:1;
            __I  uint32_t  CHANNEL:3;
            __I  uint32_t  RESERVE0:1;
            __I  uint32_t  VALID:8;
            __I  uint32_t  OVERRUN:8;
            __I  uint32_t  RESERVE1:8;
        } ADSR;
    };
    
    union {
        __IO uint32_t u32ADCALR;
        struct {
            __IO uint32_t  CALEN:1;
            __I  uint32_t  CALDONE:1;
            __I  uint32_t  RESERVE:30;
        } ADCALR;
    };
    
    __I uint32_t RESERVE0;
    __I uint32_t RESERVE1;
    
    union {
        __IO uint32_t u32ADPDMA;
        struct {
            __IO uint32_t  AD_PDMA:12;
            __I  uint32_t  RESERVE:20;
        } ADPDMA;
    };
} ADC_T;

/* ADDR Bit Field Definitions */
#define ADC_ADDR_VALID_Pos      17
#define ADC_ADDR_VALID_Msk      (1ul << ADC_ADDR_VALID_Pos)

#define ADC_ADDR_OVERRUN_Pos    16
#define ADC_ADDR_OVERRUN_Msk    (1ul << ADC_ADDR_OVERRUN_Pos)

#define ADC_ADDR_RSLT_Pos       0
#define ADC_ADDR_RSLT_Msk       (0xFFFFul << ADC_ADDR_RSLT_Pos)

/* ADCR Bit Field Definitions */
#define ADC_ADCR_DMOF_Pos       31
#define ADC_ADCR_DMOF_Msk       (1ul << ADC_ADCR_DMOF_Pos)

#define ADC_ADCR_ADST_Pos       11
#define ADC_ADCR_ADST_Msk       (1ul << ADC_ADCR_ADST_Pos)

#define ADC_ADCR_DIFFEN_Pos     10
#define ADC_ADCR_DIFFEN_Msk     (1ul << ADC_ADCR_DIFFEN_Pos)

#define ADC_ADCR_PTEN_Pos       9
#define ADC_ADCR_PTEN_Msk       (1ul << ADC_ADCR_PTEN_Pos)

#define ADC_ADCR_TRGEN_Pos      8
#define ADC_ADCR_TRGEN_Msk      (1ul << ADC_ADCR_TRGEN_Pos)

#define ADC_ADCR_TRGCOND_Pos    6
#define ADC_ADCR_TRGCOND_Msk    (3ul << ADC_ADCR_TRGCOND_Pos)

#define ADC_ADCR_TRGS_Pos       4
#define ADC_ADCR_TRGS_Msk       (3ul << ADC_ADCR_TRGS_Pos)

#define ADC_ADCR_ADMD_Pos       2
#define ADC_ADCR_ADMD_Msk       (3ul << ADC_ADCR_ADMD_Pos)

#define ADC_ADCR_ADIE_Pos       1
#define ADC_ADCR_ADIE_Msk       (1ul << ADC_ADCR_ADIE_Pos)

#define ADC_ADCR_ADEN_Pos       0
#define ADC_ADCR_ADEN_Msk       (1ul << ADC_ADCR_ADEN_Pos)

/* ADCHER Bit Field Definitions */
#define ADC_ADCHER_PRESEL_Pos   8
#define ADC_ADCHER_PRESEL_Msk   (3ul << ADC_ADCHER_PRESEL_Pos)

#define ADC_ADCHER_CHEN_Pos     0
#define ADC_ADCHER_CHEN_Msk     (0xFFul << ADC_ADCHER_CHEN_Pos)

/* ADCMPR Bit Field Definitions */
#define ADC_ADCMPR_CMPD_Pos        16
#define ADC_ADCMPR_CMPD_Msk        (0xFFFul << ADC_ADCMPR_CMPD_Pos)

#define ADC_ADCMPR_CMPMATCNT_Pos   8
#define ADC_ADCMPR_CMPMATCNT_Msk   (0xFul << ADC_ADCMPR_CMPMATCNT_Pos)

#define ADC_ADCMPR_CMPCH_Pos       3
#define ADC_ADCMPR_CMPCH_Msk       (7ul << ADC_ADCMPR_CMPCH_Pos)

#define ADC_ADCMPR_CMPCOND_Pos     2
#define ADC_ADCMPR_CMPCOND_Msk     (1ul << ADC_ADCMPR_CMPCOND_Pos)

#define ADC_ADCMPR_CMPIE_Pos       1
#define ADC_ADCMPR_CMPIE_Msk       (1ul << ADC_ADCMPR_CMPIE_Pos)

#define ADC_ADCMPR_CMPEN_Pos       0
#define ADC_ADCMPR_CMPEN_Msk       (1ul << ADC_ADCMPR_CMPEN_Pos)

/* ADSR Bit Field Definitions */
#define ADC_ADSR_OVERRUN_Pos       16
#define ADC_ADSR_OVERRUN_Msk       (0xFFul << ADC_ADSR_OVERRUN_Pos)

#define ADC_ADSR_VALID_Pos         8
#define ADC_ADSR_VALID_Msk         (0xFFul << ADC_ADSR_VALID_Pos)

#define ADC_ADSR_CHANNEL_Pos       4
#define ADC_ADSR_CHANNEL_Msk       (7ul << ADC_ADSR_CHANNEL_Pos)

#define ADC_ADSR_BUSY_Pos          3
#define ADC_ADSR_BUSY_Msk          (1ul << ADC_ADSR_BUSY_Pos)

#define ADC_ADSR_CMPF1_Pos         2
#define ADC_ADSR_CMPF1_Msk         (1ul << ADC_ADSR_CMPF1_Pos)

#define ADC_ADSR_CMPF0_Pos         1
#define ADC_ADSR_CMPF0_Msk         (1ul << ADC_ADSR_CMPF0_Pos)

#define ADC_ADSR_ADF_Pos           0
#define ADC_ADSR_ADF_Msk           (1ul << ADC_ADSR_ADF_Pos)

/* ADCALR Bit Field Definitions */
#define ADC_ADCALR_CALDONE_Pos     1
#define ADC_ADCALR_CALDONE_Msk     (1ul << ADC_ADCALR_CALDONE_Pos)

#define ADC_ADCALR_CALEN_Pos       0
#define ADC_ADCALR_CALEN_Msk       (1ul << ADC_ADCALR_CALEN_Pos)

/* ADPDMA Bit Field Definitions */
#define ADC_ADPDMA_AD_PDMA_Pos     0
#define ADC_ADPDMA_AD_PDMA_Msk     (1ul << ADC_ADPDMA_AD_PDMA_Pos)


/*---------------------- Analog Comparator Controller -------------------------*/
typedef struct
{
    __IO uint32_t  CMPEN:1;
    __IO uint32_t  CMPIE:1;
    __IO uint32_t  CMP_HYSEN:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  CMPCN:1;
    __I  uint32_t  RESERVE1:27;
} ACMP_CMPCR_T;

typedef struct
{
    __IO uint32_t  CMPF0:1;
    __IO uint32_t  CMPF1:1;
    __I  uint32_t  CO0:1;
    __I  uint32_t  CO1:1;
    __I  uint32_t  RESERVE:28;
} ACMP_CMPSR_T;

typedef struct
{
    union {
        __IO uint32_t u32CMPCR[2];
        struct {
            __IO uint32_t  CMPEN:1;
            __IO uint32_t  CMPIE:1;
            __IO uint32_t  CMP_HYSEN:1;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  CMPCN:1;
            __I  uint32_t  RESERVE1:27;
        } CMPCR[2];
    };
    
    union {
        __IO uint32_t u32CMPSR;
        struct {
            __IO uint32_t  CMPF0:1;
            __IO uint32_t  CMPF1:1;
            __I  uint32_t  CO0:1;
            __I  uint32_t  CO1:1;
            __I  uint32_t  RESERVE:28;
        } CMPSR;
    };
} ACMP_T;


/* CMPCR Bit Field Definitions */
#define ACMP_CMPCR_CMPCN_Pos       4
#define ACMP_CMPCR_CMPCN_Msk       (1ul << ACMP_CMPCR_CMPCN_Pos)

#define ACMP_CMPCR_CMP_HYSEN_Pos   2
#define ACMP_CMPCR_CMP_HYSEN_Msk   (1ul << ACMP_CMPCR_CMP_HYSEN_Pos)

#define ACMP_CMPCR_CMPIE_Pos       1
#define ACMP_CMPCR_CMPIE_Msk       (1ul << ACMP_CMPCR_CMPIE_Pos)

#define ACMP_CMPCR_CMPEN_Pos       0
#define ACMP_CMPCR_CMPEN_Msk       (1ul << ACMP_CMPCR_CMPEN_Pos)

/* CMPSR Bit Field Definitions */
#define ACMP_CMPSR_CO1_Pos     3
#define ACMP_CMPSR_CO1_Msk     (1ul << ACMP_CMPSR_CO1_Pos)

#define ACMP_CMPSR_CO0_Pos     2
#define ACMP_CMPSR_CO0_Msk     (1ul << ACMP_CMPSR_CO0_Pos)

#define ACMP_CMPSR_CMPF1_Pos   1
#define ACMP_CMPSR_CMPF1_Msk   (1ul << ACMP_CMPSR_CMPF1_Pos)

#define ACMP_CMPSR_CMPF0_Pos   0
#define ACMP_CMPSR_CMPF0_Msk   (1ul << ACMP_CMPSR_CMPF0_Pos)


/*---------------------------- Clock Controller ------------------------------*/
typedef struct
{
    __IO uint32_t  XTL12M_EN:1;
    __IO uint32_t  XTL32K_EN:1;
    __IO uint32_t  OSC22M_EN:1;
    __IO uint32_t  OSC10K_EN:1;
    __IO uint32_t  PD_WU_DLY:1;
    __IO uint32_t  PD_WU_INT_EN:1;
    __IO uint32_t  PD_WU_STS:1;
    __IO uint32_t  PWR_DOWN_EN:1;
    __IO uint32_t  PD_WAIT_CPU:1;
    __I  uint32_t  RESERVE:23;
} SYSCLK_PWRCON_T;

typedef struct
{
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  PDMA_EN:1;
    __IO uint32_t  ISP_EN:1;
    __IO uint32_t  EBI_EN:1;
    __I  uint32_t  RESERVE1:28;
} SYSCLK_AHBCLK_T;

typedef struct
{
    __IO uint32_t  WDT_EN:1;
    __IO uint32_t  RTC_EN:1;
    __IO uint32_t  TMR0_EN:1;
    __IO uint32_t  TMR1_EN:1;
    __IO uint32_t  TMR2_EN:1;
    __IO uint32_t  TMR3_EN:1;
    __IO uint32_t  FDIV_EN:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  I2C0_EN:1;
    __IO uint32_t  I2C1_EN:1;
    __I  uint32_t  RESERVE1:2;
    __IO uint32_t  SPI0_EN:1;
    __IO uint32_t  SPI1_EN:1;
    __IO uint32_t  SPI2_EN:1;
    __IO uint32_t  SPI3_EN:1;
    __IO uint32_t  UART0_EN:1;
    __IO uint32_t  UART1_EN:1;
    __IO uint32_t  UART2_EN:1;
    __I  uint32_t  RESERVE2:1;
    __IO uint32_t  PWM01_EN:1;
    __IO uint32_t  PWM23_EN:1;
    __IO uint32_t  PWM45_EN:1;
    __IO uint32_t  PWM67_EN:1;
    __IO uint32_t  CAN0_EN:1;
    __I  uint32_t  RESERVE3:2;
    __IO uint32_t  USBD_EN:1;
    __IO uint32_t  ADC_EN:1;
    __IO uint32_t  I2S_EN:1;
    __IO uint32_t  ACMP_EN:1;
    __IO uint32_t  PS2_EN:1;
} SYSCLK_APBCLK_T;

typedef struct
{
    __I  uint32_t  XTL12M_STB:1;
    __I  uint32_t  XTL32K_STB:1;
    __I  uint32_t  PLL_STB:1;
    __I  uint32_t  OSC10K_STB:1;
    __I  uint32_t  OSC22M_STB:1;
    __I  uint32_t  RESERVE0:2;
    __IO uint32_t  CLK_SW_FAIL:1;
    __I  uint32_t  RESERVE1:24;
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
    __IO uint32_t  I2S_S:2;
    __IO uint32_t  FRQDIV_S:2;
    __IO uint32_t  PWM45_S:2;
    __IO uint32_t  PWM67_S:2;
    __I  uint32_t  RESERVE:24;
} SYSCLK_CLKSEL2_T;

typedef struct
{
    __IO uint32_t  HCLK_N:4;
    __IO uint32_t  USB_N:4;
    __IO uint32_t  UART_N:4;
    __I  uint32_t  RESERVE0:4;
    __IO uint32_t  ADC_N:8;
    __I  uint32_t  RESERVE1:8;
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
    union 
    {
        __IO uint32_t u32PWRCON;
        
        struct 
        {
            __IO uint32_t  XTL12M_EN:1;
            __IO uint32_t  XTL32K_EN:1;
            __IO uint32_t  OSC22M_EN:1;
            __IO uint32_t  OSC10K_EN:1;
            __IO uint32_t  PD_WU_DLY:1;
            __IO uint32_t  PD_WU_INT_EN:1;
            __IO uint32_t  PD_WU_STS:1;
            __IO uint32_t  PWR_DOWN_EN:1;
            __IO uint32_t  PD_WAIT_CPU:1;
            __I  uint32_t  RESERVE:23;
        } PWRCON;
    };    
    
    union 
    {
        __IO uint32_t u32AHBCLK;
        
        struct 
        {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  PDMA_EN:1;
            __IO uint32_t  ISP_EN:1;
            __IO uint32_t  EBI_EN:1;
            __I  uint32_t  RESERVE1:28;
        } AHBCLK;
    };    

    union 
    {
        __IO uint32_t u32APBCLK;
        
        struct 
        {
            __IO uint32_t  WDT_EN:1;
            __IO uint32_t  RTC_EN:1;
            __IO uint32_t  TMR0_EN:1;
            __IO uint32_t  TMR1_EN:1;
            __IO uint32_t  TMR2_EN:1;
            __IO uint32_t  TMR3_EN:1;
            __IO uint32_t  FDIV_EN:1;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  I2C0_EN:1;
            __IO uint32_t  I2C1_EN:1;
            __I  uint32_t  RESERVE1:2;
            __IO uint32_t  SPI0_EN:1;
            __IO uint32_t  SPI1_EN:1;
            __IO uint32_t  SPI2_EN:1;
            __IO uint32_t  SPI3_EN:1;
            __IO uint32_t  UART0_EN:1;
            __IO uint32_t  UART1_EN:1;
            __IO uint32_t  UART2_EN:1;
            __I  uint32_t  RESERVE2:1;
            __IO uint32_t  PWM01_EN:1;
            __IO uint32_t  PWM23_EN:1;
            __IO uint32_t  PWM45_EN:1;
            __IO uint32_t  PWM67_EN:1;
            __IO uint32_t  CAN0_EN:1;
            __I  uint32_t  RESERVE3:2;
            __IO uint32_t  USBD_EN:1;
            __IO uint32_t  ADC_EN:1;
            __IO uint32_t  I2S_EN:1;
            __IO uint32_t  ACMP_EN:1;
            __IO uint32_t  PS2_EN:1;
        } APBCLK;
    };          

    union 
    {
        __IO uint32_t u32CLKSTATUS;
        
        struct 
        {
            __I  uint32_t  XTL12M_STB:1;
            __I  uint32_t  XTL32K_STB:1;
            __I  uint32_t  PLL_STB:1;
            __I  uint32_t  OSC10K_STB:1;
            __I  uint32_t  OSC22M_STB:1;
            __I  uint32_t  RESERVE0:2;
            __IO uint32_t  CLK_SW_FAIL:1;
            __I  uint32_t  RESERVE1:24;
        } CLKSTATUS;
    };  

    union 
    {
        __IO uint32_t u32CLKSEL0;
        
        struct 
        {
            __IO uint32_t  HCLK_S:3;
            __IO uint32_t  STCLK_S:3;
            __I  uint32_t  RESERVE:26;
        } CLKSEL0;
    };  

    union 
    {
        __IO uint32_t u32CLKSEL1;
        
        struct 
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
        } CLKSEL1;
    };  

    union 
    {
        __IO uint32_t u32CLKDIV;
        
        struct 
        {
            __IO uint32_t  HCLK_N:4;
            __IO uint32_t  USB_N:4;
            __IO uint32_t  UART_N:4;
            __I  uint32_t  RESERVE0:4;
            __IO uint32_t  ADC_N:8;
            __I  uint32_t  RESERVE1:8;
        } CLKDIV;
    };  

    union 
    {
        __IO uint32_t u32CLKSEL2;
        
        struct 
        {
            __IO uint32_t  I2S_S:2;
            __IO uint32_t  FRQDIV_S:2;
            __IO uint32_t  PWM45_S:2;
            __IO uint32_t  PWM67_S:2;
            __I  uint32_t  RESERVE:24;
        } CLKSEL2;
    };  
    
    union 
    {
        __IO uint32_t u32PLLCON;
        
        struct 
        {
            __IO uint32_t  FB_DV:9;
            __IO uint32_t  IN_DV:5;
            __IO uint32_t  OUT_DV:2;
            __IO uint32_t  PD:1;
            __IO uint32_t  BP:1;
            __IO uint32_t  OE:1;
            __IO uint32_t  PLL_SRC:1;
            __I  uint32_t  RESERVE:12;
        } PLLCON;
    };  
    
    union 
    {
        __IO uint32_t u32FRQDIV;
        
        struct 
        {
            __IO uint32_t  FSEL:4;
            __IO uint32_t  FDIV_EN:1;
            __I  uint32_t  RESERVE:27;
        } FRQDIV;
    };      
} SYSCLK_T;

/* SYSCLK PWRCON Bit Field Definitions */
#define SYSCLK_PWRCON_PD_WAIT_CPU_Pos           8
#define SYSCLK_PWRCON_PD_WAIT_CPU_Msk           (1ul << SYSCLK_PWRCON_PD_WAIT_CPU_Pos)

#define SYSCLK_PWRCON_PWR_DOWN_EN_Pos           7
#define SYSCLK_PWRCON_PWR_DOWN_EN_Msk           (1ul << SYSCLK_PWRCON_PWR_DOWN_EN_Pos)

#define SYSCLK_PWRCON_PD_WU_STS_Pos             6
#define SYSCLK_PWRCON_PD_WU_STS_Msk             (1ul << SYSCLK_PWRCON_PD_WU_STS_Pos)

#define SYSCLK_PWRCON_PD_WU_INT_EN_Pos          5
#define SYSCLK_PWRCON_PD_WU_INT_EN_Msk          (1ul << SYSCLK_PWRCON_PD_WU_INT_EN_Pos)

#define SYSCLK_PWRCON_PD_WU_DLY_Pos             4
#define SYSCLK_PWRCON_PD_WU_DLY_Msk             (1ul << SYSCLK_PWRCON_PD_WU_DLY_Pos)

#define SYSCLK_PWRCON_OSC10K_EN_Pos             3
#define SYSCLK_PWRCON_OSC10K_EN_Msk             (1ul << SYSCLK_PWRCON_OSC10K_EN_Pos)

#define SYSCLK_PWRCON_OSC22M_EN_Pos             2
#define SYSCLK_PWRCON_OSC22M_EN_Msk             (1ul << SYSCLK_PWRCON_OSC22M_EN_Pos)

#define SYSCLK_PWRCON_XTL32K_EN_Pos             1
#define SYSCLK_PWRCON_XTL32K_EN_Msk             (1ul << SYSCLK_PWRCON_XTL32K_EN_Pos)

#define SYSCLK_PWRCON_XTL12M_EN_Pos             0
#define SYSCLK_PWRCON_XTL12M_EN_Msk             (1ul << SYSCLK_PWRCON_XTL12M_EN_Pos)

/* SYSCLK AHBCLK Bit Field Definitions */
#define SYSCLK_AHBCLK_EBI_EN_Pos                3
#define SYSCLK_AHBCLK_EBI_EN_Msk                (1ul << SYSCLK_AHBCLK_EBI_EN_Pos)

#define SYSCLK_AHBCLK_ISP_EN_Pos                2
#define SYSCLK_AHBCLK_ISP_EN_Msk                (1ul << SYSCLK_AHBCLK_ISP_EN_Pos)

#define SYSCLK_AHBCLK_PDMA_EN_Pos               1
#define SYSCLK_AHBCLK_PDMA_EN_Msk               (1ul << SYSCLK_AHBCLK_PDMA_EN_Pos)


/* SYSCLK APBCLK Bit Field Definitions */
#define SYSCLK_APBCLK_PS2_EN_Pos                31
#define SYSCLK_APBCLK_PS2_EN_Msk                (1ul << SYSCLK_APBCLK_PS2_EN_Pos)

#define SYSCLK_APBCLK_ACMP_EN_Pos               30
#define SYSCLK_APBCLK_ACMP_EN_Msk               (1ul << SYSCLK_APBCLK_ACMP_EN_Pos)

#define SYSCLK_APBCLK_I2S_EN_Pos                29
#define SYSCLK_APBCLK_I2S_EN_Msk                (1ul << SYSCLK_APBCLK_I2S_EN_Pos)

#define SYSCLK_APBCLK_ADC_EN_Pos                28
#define SYSCLK_APBCLK_ADC_EN_Msk                (1ul << SYSCLK_APBCLK_ADC_EN_Pos)

#define SYSCLK_APBCLK_USBD_EN_Pos               27
#define SYSCLK_APBCLK_USBD_EN_Msk               (1ul << SYSCLK_APBCLK_USBD_EN_Pos)

#define SYSCLK_APBCLK_CAN0_EN_Pos               24
#define SYSCLK_APBCLK_CAN0_EN_Msk               (1ul << SYSCLK_APBCLK_CAN0_EN_Pos)

#define SYSCLK_APBCLK_PWM67_EN_Pos              23
#define SYSCLK_APBCLK_PWM67_EN_Msk              (1ul << SYSCLK_APBCLK_PWM67_EN_Pos)

#define SYSCLK_APBCLK_PWM45_EN_Pos              22
#define SYSCLK_APBCLK_PWM45_EN_Msk              (1ul << SYSCLK_APBCLK_PWM45_EN_Pos)

#define SYSCLK_APBCLK_PWM23_EN_Pos              21
#define SYSCLK_APBCLK_PWM23_EN_Msk              (1ul << SYSCLK_APBCLK_PWM23_EN_Pos)

#define SYSCLK_APBCLK_PWM01_EN_Pos              20
#define SYSCLK_APBCLK_PWM01_EN_Msk              (1ul << SYSCLK_APBCLK_PWM01_EN_Pos)

#define SYSCLK_APBCLK_UART2_EN_Pos              18
#define SYSCLK_APBCLK_UART2_EN_Msk              (1ul << SYSCLK_APBCLK_UART2_EN_Pos)

#define SYSCLK_APBCLK_UART1_EN_Pos              17
#define SYSCLK_APBCLK_UART1_EN_Msk              (1ul << SYSCLK_APBCLK_UART1_EN_Pos)

#define SYSCLK_APBCLK_UART0_EN_Pos              16
#define SYSCLK_APBCLK_UART0_EN_Msk              (1ul << SYSCLK_APBCLK_UART0_EN_Pos)

#define SYSCLK_APBCLK_SPI3_EN_Pos               15
#define SYSCLK_APBCLK_SPI3_EN_Msk               (1ul << SYSCLK_APBCLK_SPI3_EN_Pos)

#define SYSCLK_APBCLK_SPI2_EN_Pos               14
#define SYSCLK_APBCLK_SPI2_EN_Msk               (1ul << SYSCLK_APBCLK_SPI2_EN_Pos)

#define SYSCLK_APBCLK_SPI1_EN_Pos               13
#define SYSCLK_APBCLK_SPI1_EN_Msk               (1ul << SYSCLK_APBCLK_SPI1_EN_Pos)

#define SYSCLK_APBCLK_SPI0_EN_Pos               12
#define SYSCLK_APBCLK_SPI0_EN_Msk               (1ul << SYSCLK_APBCLK_SPI0_EN_Pos)

#define SYSCLK_APBCLK_I2C1_EN_Pos               9
#define SYSCLK_APBCLK_I2C1_EN_Msk               (1ul << SYSCLK_APBCLK_I2C1_EN_Pos)

#define SYSCLK_APBCLK_I2C0_EN_Pos               8
#define SYSCLK_APBCLK_I2C0_EN_Msk               (1ul << SYSCLK_APBCLK_I2C0_EN_Pos)

#define SYSCLK_APBCLK_FDIV_EN_Pos               6
#define SYSCLK_APBCLK_FDIV_EN_Msk               (1ul << SYSCLK_APBCLK_FDIV_EN_Pos)

#define SYSCLK_APBCLK_TMR3_EN_Pos               5
#define SYSCLK_APBCLK_TMR3_EN_Msk               (1ul << SYSCLK_APBCLK_TMR3_EN_Pos)

#define SYSCLK_APBCLK_TMR2_EN_Pos               4
#define SYSCLK_APBCLK_TMR2_EN_Msk               (1ul << SYSCLK_APBCLK_TMR2_EN_Pos)

#define SYSCLK_APBCLK_TMR1_EN_Pos               3
#define SYSCLK_APBCLK_TMR1_EN_Msk               (1ul << SYSCLK_APBCLK_TMR1_EN_Pos)
                                                
#define SYSCLK_APBCLK_TMR0_EN_Pos               2
#define SYSCLK_APBCLK_TMR0_EN_Msk               (1ul << SYSCLK_APBCLK_TMR0_EN_Pos)

#define SYSCLK_APBCLK_RTC_EN_Pos                1
#define SYSCLK_APBCLK_RTC_EN_Msk                (1ul << SYSCLK_APBCLK_RTC_EN_Pos)

#define SYSCLK_APBCLK_WDT_EN_Pos                0
#define SYSCLK_APBCLK_WDT_EN_Msk                (1ul << SYSCLK_APBCLK_WDT_EN_Pos)

/* SYSCLK CLKSTATUS Bit Field Definitions */
#define SYSCLK_CLKSTATUS_CLK_SW_FAIL_Pos        7
#define SYSCLK_CLKSTATUS_CLK_SW_FAIL_Msk        (1ul << SYSCLK_CLKSTATUS_CLK_SW_FAIL_Pos)

#define SYSCLK_CLKSTATUS_OSC22M_STB_Pos         4
#define SYSCLK_CLKSTATUS_OSC22M_STB_Msk         (1ul << SYSCLK_CLKSTATUS_OSC22M_STB_Pos)

#define SYSCLK_CLKSTATUS_OSC10K_STB_Pos         3
#define SYSCLK_CLKSTATUS_OSC10K_STB_Msk         (1ul << SYSCLK_CLKSTATUS_OSC10K_STB_Pos)

#define SYSCLK_CLKSTATUS_PLL_STB_Pos            2
#define SYSCLK_CLKSTATUS_PLL_STB_Msk            (1ul << SYSCLK_CLKSTATUS_PLL_STB_Pos)

#define SYSCLK_CLKSTATUS_XTL32K_STB_Pos         1
#define SYSCLK_CLKSTATUS_XTL32K_STB_Msk         (1ul << SYSCLK_CLKSTATUS_XTL32K_STB_Pos)

#define SYSCLK_CLKSTATUS_XTL12M_STB_Pos         0
#define SYSCLK_CLKSTATUS_XTL12M_STB_Msk         (1ul << SYSCLK_CLKSTATUS_XTL12M_STB_Pos)

/* SYSCLK CLKSEL0 Bit Field Definitions */
#define SYSCLK_CLKSEL0_STCLK_S_Pos              3
#define SYSCLK_CLKSEL0_STCLK_S_Msk              (7ul << SYSCLK_CLKSEL0_STCLK_S_Pos)

#define SYSCLK_CLKSEL0_HCLK_S_Pos               0
#define SYSCLK_CLKSEL0_HCLK_S_Msk               (7ul << SYSCLK_CLKSEL0_HCLK_S_Pos)

/* SYSCLK CLKSEL1 Bit Field Definitions */
#define SYSCLK_CLKSEL1_PWM23_S_Pos              30
#define SYSCLK_CLKSEL1_PWM23_S_Msk              (3ul << SYSCLK_CLKSEL1_PWM23_S_Pos)

#define SYSCLK_CLKSEL1_PWM01_S_Pos              28
#define SYSCLK_CLKSEL1_PWM01_S_Msk              (3ul << SYSCLK_CLKSEL1_PWM01_S_Pos)

#define SYSCLK_CLKSEL1_UART_S_Pos               24
#define SYSCLK_CLKSEL1_UART_S_Msk               (3ul << SYSCLK_CLKSEL1_UART_S_Pos)

#define SYSCLK_CLKSEL1_TMR3_S_Pos               20
#define SYSCLK_CLKSEL1_TMR3_S_Msk               (7ul << SYSCLK_CLKSEL1_TMR3_S_Pos)

#define SYSCLK_CLKSEL1_TMR2_S_Pos               16
#define SYSCLK_CLKSEL1_TMR2_S_Msk               (7ul << SYSCLK_CLKSEL1_TMR2_S_Pos)

#define SYSCLK_CLKSEL1_TMR1_S_Pos               12
#define SYSCLK_CLKSEL1_TMR1_S_Msk               (7ul << SYSCLK_CLKSEL1_TMR1_S_Pos)

#define SYSCLK_CLKSEL1_TMR0_S_Pos               8
#define SYSCLK_CLKSEL1_TMR0_S_Msk               (7ul << SYSCLK_CLKSEL1_TMR0_S_Pos)

#define SYSCLK_CLKSEL1_ADC_S_Pos                2
#define SYSCLK_CLKSEL1_ADC_S_Msk                (3ul << SYSCLK_CLKSEL1_ADC_S_Pos)

#define SYSCLK_CLKSEL1_WDT_S_Pos                0
#define SYSCLK_CLKSEL1_WDT_S_Msk                (3ul << SYSCLK_CLKSEL1_WDT_S_Pos)

/* SYSCLK CLKSEL2 Bit Field Definitions */
#define SYSCLK_CLKSEL2_PWM67_S_Pos              6
#define SYSCLK_CLKSEL2_PWM67_S_Msk              (3ul << SYSCLK_CLKSEL2_PWM67_S_Pos)

#define SYSCLK_CLKSEL2_PWM45_S_Pos              4
#define SYSCLK_CLKSEL2_PWM45_S_Msk              (3ul << SYSCLK_CLKSEL2_PWM45_S_Pos)

#define SYSCLK_CLKSEL2_FRQDIV_S_Pos             2
#define SYSCLK_CLKSEL2_FRQDIV_S_Msk             (3ul << SYSCLK_CLKSEL2_FRQDIV_S_Pos)

#define SYSCLK_CLKSEL2_I2S_S_Pos                0
#define SYSCLK_CLKSEL2_I2S_S_Msk                (3ul << SYSCLK_CLKSEL2_I2S_S_Pos)

/* SYSCLK CLKDIV Bit Field Definitions */
#define SYSCLK_CLKDIV_ADC_N_Pos                 16
#define SYSCLK_CLKDIV_ADC_N_Msk                 (0xFFul << SYSCLK_CLKDIV_ADC_N_Pos)

#define SYSCLK_CLKDIV_UART_N_Pos                8
#define SYSCLK_CLKDIV_UART_N_Msk                (0xFul << SYSCLK_CLKDIV_UART_N_Pos)

#define SYSCLK_CLKDIV_USB_N_Pos                 4
#define SYSCLK_CLKDIV_USB_N_Msk                 (0xFul << SYSCLK_CLKDIV_USB_N_Pos)

#define SYSCLK_CLKDIV_HCLK_N_Pos                0
#define SYSCLK_CLKDIV_HCLK_N_Msk                (0xFul << SYSCLK_CLKDIV_HCLK_N_Pos)

/* SYSCLK PLLCON Bit Field Definitions */
#define SYSCLK_PLLCON_PLL_SRC_Pos               19
#define SYSCLK_PLLCON_PLL_SRC_Msk               (1ul << SYSCLK_PLLCON_PLL_SRC_Pos)

#define SYSCLK_PLLCON_OE_Pos                    18
#define SYSCLK_PLLCON_OE_Msk                    (1ul << SYSCLK_PLLCON_OE_Pos)

#define SYSCLK_PLLCON_BP_Pos                    17
#define SYSCLK_PLLCON_BP_Msk                    (1ul << SYSCLK_PLLCON_BP_Pos)

#define SYSCLK_PLLCON_PD_Pos                    16
#define SYSCLK_PLLCON_PD_Msk                    (1ul << SYSCLK_PLLCON_PD_Pos)

#define SYSCLK_PLLCON_OUT_DV_Pos                14
#define SYSCLK_PLLCON_OUT_DV_Msk                (3ul << SYSCLK_PLLCON_OUT_DV_Pos)

#define SYSCLK_PLLCON_IN_DV_Pos                 9
#define SYSCLK_PLLCON_IN_DV_Msk                 (0x1Ful << SYSCLK_PLLCON_IN_DV_Pos)

#define SYSCLK_PLLCON_FB_DV_Pos                 0
#define SYSCLK_PLLCON_FB_DV_Msk                 (0x1FFul << SYSCLK_PLLCON_FB_DV_Pos)

/* SYSCLK FRQDIV Bit Field Definitions */
#define SYSCLK_FRQDIV_FDIV_EN_Pos               4
#define SYSCLK_FRQDIV_FDIV_EN_Msk               (1ul << SYSCLK_FRQDIV_FDIV_EN_Pos)

#define SYSCLK_FRQDIV_FSEL_Pos                  0
#define SYSCLK_FRQDIV_FSEL_Msk                  (0xFul << SYSCLK_FRQDIV_FSEL_Pos)

/*---------------------------- Global Controller -----------------------------*/
typedef __I uint32_t GCR_PDID_T;  

typedef struct
{
    __IO uint32_t  RSTS_POR:1;
    __IO uint32_t  RSTS_RESET:1;
    __IO uint32_t  RSTS_WDT:1;
    __IO uint32_t  RSTS_LVR:1;
    __IO uint32_t  RSTS_BOD:1;
    __IO uint32_t  RSTS_SYS:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  RSTS_CPU:1;    
    __I  uint32_t  RESERVE1:24;
} GCR_RSTSRC_T;


typedef struct
{
    __IO uint32_t  CHIP_RST:1;
    __IO uint32_t  CPU_RST:1;
    __IO uint32_t  PDMA_RST:1;
    __IO uint32_t  EBI_RST:1;
    __I  uint32_t  RESERVE:28;
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
    __IO uint32_t  I2C0_RST:1;
    __IO uint32_t  I2C1_RST:1;
    __I  uint32_t  RESERVE2:2;
    __IO uint32_t  SPI0_RST:1;
    __IO uint32_t  SPI1_RST:1;
    __IO uint32_t  SPI2_RST:1;
    __IO uint32_t  SPI3_RST:1;
    __IO uint32_t  UART0_RST:1;
    __IO uint32_t  UART1_RST:1;
    __IO uint32_t  UART2_RST:1;
    __I  uint32_t  RESERVE3:1;
    __IO uint32_t  PWM03_RST:1;
    __IO uint32_t  PWM47_RST:1;
    __IO uint32_t  ACMP_RST:1;
    __IO uint32_t  PS2_RST:1;
    __IO uint32_t  CAN0_RST:1;
    __I  uint32_t  RESERVE4:2;
    __IO uint32_t  USBD_RST:1;
    __IO uint32_t  ADC_RST:1;
    __IO uint32_t  I2S_RST:1;
    __I  uint32_t  RESERVE5:2;
} GCR_IPRSTC2_T;

typedef struct
{
    __IO uint32_t  HPE:1;
    __I  uint32_t  RESERVE:31;
} GCR_CPR_T;


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

typedef struct
{
    __IO uint32_t  VTEMP_EN:1;
    __I  uint32_t  RESERVE:31;
} GCR_TEMPCR_T;

typedef __IO uint32_t GCR_PORCR_T;  

typedef struct
{
    __IO uint32_t ADC0:1;
    __IO uint32_t ADC1_AD12:1;
    __IO uint32_t ADC2_AD11:1;
    __IO uint32_t ADC3_AD10:1;
    __IO uint32_t ADC4_AD9:1;
    __IO uint32_t ADC5_AD8:1;
    __IO uint32_t ADC6_AD7:1;
    __IO uint32_t ADC7_SS21_AD6:1;
    __IO uint32_t I2C0_SDA:1;
    __IO uint32_t I2C0_SCL:1;
    __IO uint32_t I2C1_SDA_nWR:1;
    __IO uint32_t I2C1_SCL_nRD:1;
    __IO uint32_t PWM0_AD13:1;
    __IO uint32_t PWM1_AD14:1;
    __IO uint32_t PWM2_AD15:1;
    __IO uint32_t PWM3_I2SMCLK:1;
    __IO uint32_t SCHMITT:16;    
} GCR_GPAMFP_T;

typedef struct
{
    __IO uint32_t UART0_RX:1;
    __IO uint32_t UART0_TX:1;
    __IO uint32_t UART0_nRTS_nWRL:1;
    __IO uint32_t UART0_nCTS_nWRH:1;
    __IO uint32_t UART1_RX:1;
    __IO uint32_t UART1_TX:1;
    __IO uint32_t UART1_nRTS_ALE:1;
    __IO uint32_t UART1_nCTS_nCS:1;
    __IO uint32_t TM0:1;
    __IO uint32_t TM1_SS11:1;                              
    __IO uint32_t TM2_SS01:1;
    __IO uint32_t TM3_PWM4:1;                     
    __IO uint32_t CPO0_CLKO_AD0:1;
    __IO uint32_t CPO1_AD1:1;
    __IO uint32_t INT0_SS31:1;
    __IO uint32_t INT1:1;
    __IO uint32_t SCHMITT:16;    
} GCR_GPBMFP_T;

typedef struct
{
    __IO uint32_t SPI0_SS0_I2SLRCLK:1;
    __IO uint32_t SPI0_CLK_I2SBCLK:1;
    __IO uint32_t SPI0_MISO0_I2SDI:1;
    __IO uint32_t SPI0_MOSI0_I2SDO:1;
    __IO uint32_t SPI0_MISO1:1;
    __IO uint32_t SPI0_MOSI1:1;
    __IO uint32_t CPP0_AD4:1;
    __IO uint32_t CPN0_AD5:1;
    __IO uint32_t SPI1_SS0_MCLK:1;
    __IO uint32_t SPI1_CLK:1;
    __IO uint32_t SPI1_MISO0:1;
    __IO uint32_t SPI1_MOSI0:1;
    __IO uint32_t SPI1_MISO1:1;
    __IO uint32_t SPI1_MOSI1:1;
    __IO uint32_t CPP1_AD2:1;
    __IO uint32_t CPN1_AD3:1;
    __IO uint32_t SCHMITT:16;    
} GCR_GPCMFP_T;

typedef struct
{
    __IO uint32_t SPI2_SS0:1;
    __IO uint32_t SPI2_CLK_SPI0_SS1:1;
    __IO uint32_t SPI2_MISO0_SPI0_MISO1:1;
    __IO uint32_t SPI2_MOSI0_SPI0_MOSI1:1;
    __IO uint32_t SPI2_MISO1:1;
    __IO uint32_t SPI2_MOSI1:1;
    __IO uint32_t CAN0_RX:1;
    __IO uint32_t CAN0_TX:1;
    __IO uint32_t SPI3_SS0:1;
    __IO uint32_t SPI3_CLK:1;
    __IO uint32_t SPI3_MISO0:1;
    __IO uint32_t SPI3_MOSI0:1;
    __IO uint32_t SPI3_MISO1:1;
    __IO uint32_t SPI3_MOSI1:1;
    __IO uint32_t UART2_RX:1;
    __IO uint32_t UART2_TX:1;
    __IO uint32_t SCHMITT:16;    
} GCR_GPDMFP_T;


typedef struct
{
    __IO uint32_t  PWM6:1;
    __IO uint32_t  PWM7:1;
    __I  uint32_t  RESERVE1:3;
    __IO uint32_t  PWM5:1;
    __I  uint32_t  RESERVE2:10;
    __IO uint32_t  SCHMITT:16;
} GCR_GPEMFP_T;

typedef struct
{
    __IO uint32_t  PB10_S01:1;      /* GPB10 */
    __IO uint32_t  PB9_S11:1;       /* GPB9  */
    __IO uint32_t  PA7_S21:1;       /* GPA7  */
    __IO uint32_t  PB14_S31:1;      /* GPB14 */
    __IO uint32_t  PB11_PWM4:1;     /* GPB11 */
    __IO uint32_t  PC0_I2SLRCLK:1;  /* GPC0  */
    __IO uint32_t  PC1_I2SBCLK:1;   /* GPC1  */
    __IO uint32_t  PC2_I2SDI:1;     /* GPC2  */
    __IO uint32_t  PC3_I2SDO:1;     /* GPC3  */
    __IO uint32_t  PA15_I2SMCLK:1;  /* GPA15 */
    __IO uint32_t  PB12_CLKO:1;     /* GPB12 */
    __IO uint32_t  EBI_EN:1;            
    __IO uint32_t  EBI_MCLK_EN:1;   /* GPC8  */
    __IO uint32_t  EBI_WRL_EN:1;    /* GPB2  */
    __IO uint32_t  EBI_WRH_EN:1;    /* GPB3  */
    __I  uint32_t  RESERVE0:1;  
    __IO uint32_t  EBI_HB_EN:8;     
    __IO uint32_t  PB15_T0EX:1;
    __IO uint32_t  PE5_T1EX:1;
    __IO uint32_t  PB2_T2EX:1;
    __IO uint32_t  PB3_T3EX:1;
    __I  uint32_t  RESERVE1:4;
} GCR_ALTMFP_T;

typedef __IO uint32_t GCR_REGWRPROT_T;

typedef struct
{
    union 
    {
        __I uint32_t u32PDID;
        __I uint32_t PDID;
    };

    union 
    {
        __IO uint32_t u32RSTSRC;
        
        struct 
        {
            __IO uint32_t  RSTS_POR:1;
            __IO uint32_t  RSTS_RESET:1;
            __IO uint32_t  RSTS_WDT:1;
            __IO uint32_t  RSTS_LVR:1;
            __IO uint32_t  RSTS_BOD:1;
            __IO uint32_t  RSTS_SYS:1;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  RSTS_CPU:1;    
            __I  uint32_t  RESERVE1:24;
        } RSTSRC;
    };

    union 
    {
        __IO uint32_t u32IPRSTC1;
        
        struct 
        {
            __IO uint32_t  CHIP_RST:1;
            __IO uint32_t  CPU_RST:1;
            __IO uint32_t  PDMA_RST:1;
            __IO uint32_t  EBI_RST:1;
            __I  uint32_t  RESERVE:28;
        } IPRSTC1;
    };    
    
    union 
    {
        __IO uint32_t u32IPRSTC2;
        
        struct 
        {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  GPIO_RST:1;
            __IO uint32_t  TMR0_RST:1;
            __IO uint32_t  TMR1_RST:1;
            __IO uint32_t  TMR2_RST:1;
            __IO uint32_t  TMR3_RST:1;
            __I  uint32_t  RESERVE1:2;
            __IO uint32_t  I2C0_RST:1;
            __IO uint32_t  I2C1_RST:1;
            __I  uint32_t  RESERVE2:2;
            __IO uint32_t  SPI0_RST:1;
            __IO uint32_t  SPI1_RST:1;
            __IO uint32_t  SPI2_RST:1;
            __IO uint32_t  SPI3_RST:1;
            __IO uint32_t  UART0_RST:1;
            __IO uint32_t  UART1_RST:1;
            __IO uint32_t  UART2_RST:1;
            __I  uint32_t  RESERVE3:1;
            __IO uint32_t  PWM03_RST:1;
            __IO uint32_t  PWM47_RST:1;
            __IO uint32_t  ACMP_RST:1;
            __IO uint32_t  PS2_RST:1;
            __IO uint32_t  CAN0_RST:1;
            __I  uint32_t  RESERVE4:2;
            __IO uint32_t  USBD_RST:1;
            __IO uint32_t  ADC_RST:1;
            __IO uint32_t  I2S_RST:1;
            __I  uint32_t  RESERVE5:2;
        } IPRSTC2;
    };    

    union 
    {
        __IO uint32_t u32CPR;
        
        struct 
        {
            __IO uint32_t  HPE:1;
            __I  uint32_t  RESERVE:31;
        } CPR;
    };    

    uint32_t RESERVE0;
    
    union 
    {
        __IO uint32_t u32BODCR;
        
        struct 
        {
            __IO uint32_t  BOD_EN:1;
            __IO uint32_t  BOD_VL:2;
            __IO uint32_t  BOD_RSTEN:1;
            __IO uint32_t  BOD_INTF:1;
            __IO uint32_t  BOD_LPM:1;
            __IO uint32_t  BOD_OUT:1;
            __IO uint32_t  LVR_EN:1;
            __I  uint32_t  RESERVE1:24;
        } BODCR;
    };

    union 
    {
        __IO uint32_t u32TEMPCR;
        
        struct 
        {
            __IO uint32_t  VTEMP_EN:1;
            __I  uint32_t  RESERVE:31;
        } TEMPCR;
    };   
    
    uint32_t RESERVE1;
    
    union 
    {
        __IO uint32_t u32PORCR;
        __IO uint32_t PORCR;
    };   

    uint32_t RESERVE2[2];

    union 
    {
        __IO uint32_t u32GPAMFP;
        
        struct 
        {
            __IO uint32_t ADC0:1;
            __IO uint32_t ADC1_AD12:1;
            __IO uint32_t ADC2_AD11:1;
            __IO uint32_t ADC3_AD10:1;
            __IO uint32_t ADC4_AD9:1;
            __IO uint32_t ADC5_AD8:1;
            __IO uint32_t ADC6_AD7:1;
            __IO uint32_t ADC7_SS21_AD6:1;
            __IO uint32_t I2C0_SDA:1;
            __IO uint32_t I2C0_SCL:1;
            __IO uint32_t I2C1_SDA_nWR:1;
            __IO uint32_t I2C1_SCL_nRD:1;
            __IO uint32_t PWM0_AD13:1;
            __IO uint32_t PWM1_AD14:1;
            __IO uint32_t PWM2_AD15:1;
            __IO uint32_t PWM3_I2SMCLK:1;
            __IO uint32_t SCHMITT:16;   
        } GPAMFP;
    };   
    
    union 
    {
        __IO uint32_t u32GPBMFP;
        
        struct 
        {
            __IO uint32_t UART0_RX:1;
            __IO uint32_t UART0_TX:1;
            __IO uint32_t UART0_nRTS_nWRL:1;
            __IO uint32_t UART0_nCTS_nWRH:1;
            __IO uint32_t UART1_RX:1;
            __IO uint32_t UART1_TX:1;
            __IO uint32_t UART1_nRTS_ALE:1;
            __IO uint32_t UART1_nCTS_nCS:1;
            __IO uint32_t TM0:1;
            __IO uint32_t TM1_SS11:1;                              
            __IO uint32_t TM2_SS01:1;
            __IO uint32_t TM3_PWM4:1;                     
            __IO uint32_t CPO0_CLKO_AD0:1;
            __IO uint32_t CPO1_AD1:1;
            __IO uint32_t INT0_SS31:1;
            __IO uint32_t INT1:1;
            __IO uint32_t SCHMITT:16;    
        } GPBMFP;
    };   
    
    union 
    {
        __IO uint32_t u32GPCMFP;
        
        struct 
        {
            __IO uint32_t SPI0_SS0_I2SLRCLK:1;
            __IO uint32_t SPI0_CLK_I2SBCLK:1;
            __IO uint32_t SPI0_MISO0_I2SDI:1;
            __IO uint32_t SPI0_MOSI0_I2SDO:1;
            __IO uint32_t SPI0_MISO1:1;
            __IO uint32_t SPI0_MOSI1:1;
            __IO uint32_t CPP0_AD4:1;
            __IO uint32_t CPN0_AD5:1;
            __IO uint32_t SPI1_SS0_MCLK:1;
            __IO uint32_t SPI1_CLK:1;
            __IO uint32_t SPI1_MISO0:1;
            __IO uint32_t SPI1_MOSI0:1;
            __IO uint32_t SPI1_MISO1:1;
            __IO uint32_t SPI1_MOSI1:1;
            __IO uint32_t CPP1_AD2:1;
            __IO uint32_t CPN1_AD3:1;
            __IO uint32_t SCHMITT:16; 
        } GPCMFP;
    };   
    
    union 
    {
        __IO uint32_t u32GPDMFP;
        
        struct 
        {
            __IO uint32_t SPI2_SS0:1;
            __IO uint32_t SPI2_CLK_SPI0_SS1:1;
            __IO uint32_t SPI2_MISO0_SPI0_MISO1:1;
            __IO uint32_t SPI2_MOSI0_SPI0_MOSI1:1;
            __IO uint32_t SPI2_MISO1:1;
            __IO uint32_t SPI2_MOSI1:1;
            __IO uint32_t CAN0_RX:1;
            __IO uint32_t CAN0_TX:1;
            __IO uint32_t SPI3_SS0:1;
            __IO uint32_t SPI3_CLK:1;
            __IO uint32_t SPI3_MISO0:1;
            __IO uint32_t SPI3_MOSI0:1;
            __IO uint32_t SPI3_MISO1:1;
            __IO uint32_t SPI3_MOSI1:1;
            __IO uint32_t UART2_RX:1;
            __IO uint32_t UART2_TX:1;
            __IO uint32_t SCHMITT:16;
        } GPDMFP;
    };   
    
    union 
    {
        __IO uint32_t u32GPEMFP;
        
        struct 
        {
            __IO uint32_t  PWM6:1;
            __IO uint32_t  PWM7:1;
            __I  uint32_t  RESERVE1:3;
            __IO uint32_t  PWM5:1;
            __I  uint32_t  RESERVE2:10;
            __IO uint32_t  SCHMITT:16;
        } GPEMFP;
    };                   
    
    uint32_t RESERVE3[3];
     
    union 
    {
        __IO uint32_t u32ALTMFP;
        
        struct 
        {
            __IO uint32_t  PB10_S01:1;      /* GPB10 */
            __IO uint32_t  PB9_S11:1;       /* GPB9  */
            __IO uint32_t  PA7_S21:1;       /* GPA7  */
            __IO uint32_t  PB14_S31:1;      /* GPB14 */
            __IO uint32_t  PB11_PWM4:1;     /* GPB11 */
            __IO uint32_t  PC0_I2SLRCLK:1;  /* GPC0  */
            __IO uint32_t  PC1_I2SBCLK:1;   /* GPC1  */
            __IO uint32_t  PC2_I2SDI:1;     /* GPC2  */
            __IO uint32_t  PC3_I2SDO:1;     /* GPC3  */
            __IO uint32_t  PA15_I2SMCLK:1;  /* GPA15 */
            __IO uint32_t  PB12_CLKO:1;     /* GPB12 */
            __IO uint32_t  EBI_EN:1;            
            __IO uint32_t  EBI_MCLK_EN:1;   /* GPC8  */
            __IO uint32_t  EBI_WRL_EN:1;    /* GPB2  */
            __IO uint32_t  EBI_WRH_EN:1;    /* GPB3  */
            __I  uint32_t  RESERVE0:1;  
            __IO uint32_t  EBI_HB_EN:8;     
            __IO uint32_t  PB15_T0EX:1;
            __IO uint32_t  PE5_T1EX:1;
            __IO uint32_t  PB2_T2EX:1;
            __IO uint32_t  PB3_T3EX:1;
            __I  uint32_t  RESERVE1:4;  
        } ALTMFP;
    };   
    
    uint32_t RESERVE4[43];
    
    union 
    {
        __IO uint32_t u32REGWRPROT;
        __IO uint32_t REGWRPROT;
    };  
} GCR_T;

/* GCR RSTSRC Bit Field Definitions */
#define GCR_RSTSRC_RSTS_CPU_Pos                 7
#define GCR_RSTSRC_RSTS_CPU_Msk                 (1ul << GCR_RSTSRC_RSTS_CPU_Pos)

#define GCR_RSTSRC_RSTS_SYS_Pos                 5
#define GCR_RSTSRC_RSTS_SYS_Msk                 (1ul << GCR_RSTSRC_RSTS_SYS_Pos)

#define GCR_RSTSRC_RSTS_BOD_Pos                 4
#define GCR_RSTSRC_RSTS_BOD_Msk                 (1ul << GCR_RSTSRC_RSTS_BOD_Pos)

#define GCR_RSTSRC_RSTS_LVR_Pos                 3
#define GCR_RSTSRC_RSTS_LVR_Msk                 (1ul << GCR_RSTSRC_RSTS_LVR_Pos)

#define GCR_RSTSRC_RSTS_WDT_Pos                 2
#define GCR_RSTSRC_RSTS_WDT_Msk                 (1ul << GCR_RSTSRC_RSTS_WDT_Pos)

#define GCR_RSTSRC_RSTS_RESET_Pos               1
#define GCR_RSTSRC_RSTS_RESET_Msk               (1ul << GCR_RSTSRC_RSTS_RESET_Pos)

#define GCR_RSTSRC_RSTS_POR_Pos                 0
#define GCR_RSTSRC_RSTS_POR_Msk                 (1ul << GCR_RSTSRC_RSTS_POR_Pos)

/* GCR IPRSTC1 Bit Field Definitions */
#define GCR_IPRSTC1_EBI_RST_Pos                 3
#define GCR_IPRSTC1_EBI_RST_Msk                 (1ul << GCR_IPRSTC1_EBI_RST_Pos)

#define GCR_IPRSTC1_PDMA_RST_Pos                2
#define GCR_IPRSTC1_PDMA_RST_Msk                (1ul << GCR_IPRSTC1_PDMA_RST_Pos)

#define GCR_IPRSTC1_CPU_RST_Pos                 1
#define GCR_IPRSTC1_CPU_RST_Msk                 (1ul << GCR_IPRSTC1_CPU_RST_Pos)

#define GCR_IPRSTC1_CHIP_RST_Pos                0
#define GCR_IPRSTC1_CHIP_RST_Msk                (1ul << GCR_IPRSTC1_CHIP_RST_Pos)

/* GCR IPRSTC2 Bit Field Definitions */
#define GCR_IPRSTC2_I2S_RST_Pos                 29
#define GCR_IPRSTC2_I2S_RST_Msk                 (1ul << GCR_IPRSTC2_I2S_RST_Pos)

#define GCR_IPRSTC2_ADC_RST_Pos                 28
#define GCR_IPRSTC2_ADC_RST_Msk                 (1ul << GCR_IPRSTC2_ADC_RST_Pos)

#define GCR_IPRSTC2_USBD_RST_Pos                27
#define GCR_IPRSTC2_USBD_RST_Msk                (1ul << GCR_IPRSTC2_USBD_RST_Pos)

#define GCR_IPRSTC2_CAN0_RST_Pos                24
#define GCR_IPRSTC2_CAN0_RST_Msk                (1ul << GCR_IPRSTC2_CAN0_RST_Pos)

#define GCR_IPRSTC2_PS2_RST_Pos                 23
#define GCR_IPRSTC2_PS2_RST_Msk                 (1ul << GCR_IPRSTC2_PS2_RST_Pos)

#define GCR_IPRSTC2_ACMP_RST_Pos                22
#define GCR_IPRSTC2_ACMP_RST_Msk                (1ul << GCR_IPRSTC2_ACMP_RST_Pos)

#define GCR_IPRSTC2_PWM47_RST_Pos               21
#define GCR_IPRSTC2_PWM47_RST_Msk               (1ul << GCR_IPRSTC2_PWM47_RST_Pos)

#define GCR_IPRSTC2_PWM03_RST_Pos               20
#define GCR_IPRSTC2_PWM03_RST_Msk               (1ul << GCR_IPRSTC2_PWM03_RST_Pos)

#define GCR_IPRSTC2_UART2_RST_Pos               18
#define GCR_IPRSTC2_UART2_RST_Msk               (1ul << GCR_IPRSTC2_UART2_RST_Pos)

#define GCR_IPRSTC2_UART1_RST_Pos               17
#define GCR_IPRSTC2_UART1_RST_Msk               (1ul << GCR_IPRSTC2_UART1_RST_Pos)

#define GCR_IPRSTC2_UART0_RST_Pos               16
#define GCR_IPRSTC2_UART0_RST_Msk               (1ul << GCR_IPRSTC2_UART0_RST_Pos)

#define GCR_IPRSTC2_SPI3_RST_Pos                15
#define GCR_IPRSTC2_SPI3_RST_Msk                (1ul << GCR_IPRSTC2_SPI3_RST_Pos)

#define GCR_IPRSTC2_SPI2_RST_Pos                14
#define GCR_IPRSTC2_SPI2_RST_Msk                (1ul << GCR_IPRSTC2_SPI2_RST_Pos)

#define GCR_IPRSTC2_SPI1_RST_Pos                13
#define GCR_IPRSTC2_SPI1_RST_Msk                (1ul << GCR_IPRSTC2_SPI1_RST_Pos)

#define GCR_IPRSTC2_SPI0_RST_Pos                12
#define GCR_IPRSTC2_SPI0_RST_Msk                (1ul << GCR_IPRSTC2_SPI0_RST_Pos)

#define GCR_IPRSTC2_I2C1_RST_Pos                9
#define GCR_IPRSTC2_I2C1_RST_Msk                (1ul << GCR_IPRSTC2_I2C1_RST_Pos)

#define GCR_IPRSTC2_I2C0_RST_Pos                8
#define GCR_IPRSTC2_I2C0_RST_Msk                (1ul << GCR_IPRSTC2_I2C0_RST_Pos)

#define GCR_IPRSTC2_TMR3_RST_Pos                5
#define GCR_IPRSTC2_TMR3_RST_Msk                (1ul << GCR_IPRSTC2_TMR3_RST_Pos)

#define GCR_IPRSTC2_TMR2_RST_Pos                4
#define GCR_IPRSTC2_TMR2_RST_Msk                (1ul << GCR_IPRSTC2_TMR2_RST_Pos)

#define GCR_IPRSTC2_TMR1_RST_Pos                3
#define GCR_IPRSTC2_TMR1_RST_Msk                (1ul << GCR_IPRSTC2_TMR1_RST_Pos)

#define GCR_IPRSTC2_TMR0_RST_Pos                2
#define GCR_IPRSTC2_TMR0_RST_Msk                (1ul << GCR_IPRSTC2_TMR0_RST_Pos)

#define GCR_IPRSTC2_GPIO_RST_Pos                1
#define GCR_IPRSTC2_GPIO_RST_Msk                (1ul << GCR_IPRSTC2_GPIO_RST_Pos)

/* GCR CPR Bit Field Definitions */
#define GCR_CPR_HPE_Pos                         0
#define GCR_CPR_HPE_Msk                         (1ul << GCR_CPR_HPE_Pos)

/* GCR BODCR Bit Field Definitions */
#define GCR_BODCR_LVR_EN_Pos                    7
#define GCR_BODCR_LVR_EN_Msk                    (1ul << GCR_BODCR_LVR_EN_Pos)

#define GCR_BODCR_BOD_OUT_Pos                   6
#define GCR_BODCR_BOD_OUT_Msk                   (1ul << GCR_BODCR_BOD_OUT_Pos)

#define GCR_BODCR_BOD_LPM_Pos                   5
#define GCR_BODCR_BOD_LPM_Msk                   (1ul << GCR_BODCR_BOD_LPM_Pos)

#define GCR_BODCR_BOD_INTF_Pos                  4
#define GCR_BODCR_BOD_INTF_Msk                  (1ul << GCR_BODCR_BOD_INTF_Pos)

#define GCR_BODCR_BOD_RSTEN_Pos                 3
#define GCR_BODCR_BOD_RSTEN_Msk                 (1ul << GCR_BODCR_BOD_RSTEN_Pos)

#define GCR_BODCR_BOD_VL_Pos                    1
#define GCR_BODCR_BOD_VL_Msk                    (3ul << GCR_BODCR_BOD_VL_Pos)

#define GCR_BODCR_BOD_EN_Pos                    0
#define GCR_BODCR_BOD_EN_Msk                    (1ul << GCR_BODCR_BOD_EN_Pos)

/* GCR TEMPCR Bit Field Definitions */
#define GCR_TEMPCR_VTEMP_EN_Pos                 0
#define GCR_TEMPCR_VTEMP_EN_Msk                 (1ul << GCR_TEMPCR_VTEMP_EN_Pos)

/* GCR PORCR Bit Field Definitions */
#define GCR_PORCR_POR_DIS_CODE_Pos              0
#define GCR_PORCR_POR_DIS_CODE_Msk              (0xFFFFul << GCR_PORCR_POR_DIS_CODE_Pos)

/* GCR GPAMFP Bit Field Definitions */
#define GCR_GPAMFP_GPA_TYPE_Pos                 16
#define GCR_GPAMFP_GPA_TYPE_Msk                 (0xFFFFul << GCR_GPAMFP_GPA_TYPE_Pos)

#define GCR_GPAMFP_GPA_MFP_Pos                  0
#define GCR_GPAMFP_GPA_MFP_Msk                  (0xFFFFul << GCR_GPAMFP_GPA_MFP_Pos)


/* GCR GPBMFP Bit Field Definitions */
#define GCR_GPBMFP_GPB_TYPE_Pos                 16
#define GCR_GPBMFP_GPB_TYPE_Msk                 (0xFFFFul << GCR_GPBMFP_GPB_TYPE_Pos)

#define GCR_GPBMFP_GPB_MFP_Pos                  0
#define GCR_GPBMFP_GPB_MFP_Msk                  (0xFFFFul << GCR_GPBMFP_GPB_MFP_Pos)

/* GCR GPCMFP Bit Field Definitions */
#define GCR_GPCMFP_GPC_TYPE_Pos                 16
#define GCR_GPCMFP_GPC_TYPE_Msk                 (0xFFFFul << GCR_GPCMFP_GPC_TYPE_Pos)

#define GCR_GPCMFP_GPC_MFP_Pos                  0
#define GCR_GPCMFP_GPC_MFP_Msk                  (0xFFFFul << GCR_GPCMFP_GPC_MFP_Pos)

/* GCR GPDMFP Bit Field Definitions */
#define GCR_GPDMFP_GPD_TYPE_Pos                 16
#define GCR_GPDMFP_GPD_TYPE_Msk                 (0xFFFFul << GCR_GPDMFP_GPD_TYPE_Pos)

#define GCR_GPDMFP_GPD_MFP_Pos                  0
#define GCR_GPDMFP_GPD_MFP_Msk                  (0xFFFFul << GCR_GPDMFP_GPD_MFP_Pos)

/* GCR GPEMFP Bit Field Definitions */
#define GCR_GPEMFP_GPE_TYPE_Pos                 16
#define GCR_GPEMFP_GPE_TYPE_Msk                 (0xFFFFul << GCR_GPEMFP_GPE_TYPE_Pos)

#define GCR_GPEMFP_GPE_MFP5_Pos                 5
#define GCR_GPEMFP_GPE_MFP5_Msk                 (1ul << GCR_GPEMFP_GPE_MFP5_Pos)

#define GCR_GPEMFP_GPE_MFP1_Pos                 1
#define GCR_GPEMFP_GPE_MFP1_Msk                 (1ul << GCR_GPEMFP_GPE_MFP1_Pos)

#define GCR_GPEMFP_GPE_MFP0_Pos                 0
#define GCR_GPEMFP_GPE_MFP0_Msk                 (1ul << GCR_GPEMFP_GPE_MFP0_Pos)

/* GCR ALTMFP Bit Field Definitions */
#define GCR_ALTMFP_PB3_T3EX_Pos                 27
#define GCR_ALTMFP_PB3_T3EX_Msk                 (1ul << GCR_ALTMFP_PB3_T3EX_Pos)

#define GCR_ALTMFP_PB2_T2EX_Pos                 26
#define GCR_ALTMFP_PB2_T2EX_Msk                 (1ul << GCR_ALTMFP_PB2_T2EX_Pos)

#define GCR_ALTMFP_PE5_T1EX_Pos                 25
#define GCR_ALTMFP_PE5_T1EX_Msk                 (1ul << GCR_ALTMFP_PE5_T1EX_Pos)

#define GCR_ALTMFP_PB15_T0EX_Pos                24
#define GCR_ALTMFP_PB15_T0EX_Msk                (1ul << GCR_ALTMFP_PB15_T0EX_Pos)

#define GCR_ALTMFP_EBI_HB_EN_Pos                16
#define GCR_ALTMFP_EBI_HB_EN_Msk                (0xFFul << GCR_ALTMFP_EBI_HB_EN_Pos)

#define GCR_ALTMFP_EBI_nWRH_EN_Pos              14
#define GCR_ALTMFP_EBI_nWRH_EN_Msk              (1ul << GCR_ALTMFP_EBI_nWRH_EN_Pos)

#define GCR_ALTMFP_EBI_nWRL_EN_Pos              13
#define GCR_ALTMFP_EBI_nWRL_EN_Msk              (1ul << GCR_ALTMFPEBI_nWRL_EN_Pos)

#define GCR_ALTMFP_EBI_MCLK_EN_Pos              12
#define GCR_ALTMFP_EBI_MCLK_EN_Msk              (1ul << GCR_ALTMFP_EBI_MCLK_EN_Pos)

#define GCR_ALTMFP_EBI_EN_Pos                   11
#define GCR_ALTMFP_EBI_EN_Msk                   (1ul << GCR_ALTMFP_EBI_EN_Pos)

#define GCR_ALTMFP_PB12_CLKO_Pos                10
#define GCR_ALTMFP_PB12_CLKO_Msk                (1ul << GCR_ALTMFP_PB12_CLKO_Pos)

#define GCR_ALTMFP_PA15_I2SMCLK_Pos             9
#define GCR_ALTMFP_PA15_I2SMCLK_Msk             (1ul << GCR_ALTMFP_PA15_I2SMCLK_Pos)

#define GCR_ALTMFP_PC3_I2SDO_Pos                8
#define GCR_ALTMFP_PC3_I2SDO_Msk                (1ul << GCR_ALTMFP_PC3_I2SDO_Pos)

#define GCR_ALTMFP_PC2_I2SDI_Pos                7
#define GCR_ALTMFP_PC2_I2SDI_Msk                (1ul << GCR_ALTMFP_PC2_I2SDI_Pos)

#define GCR_ALTMFP_PC1_I2SBCLK_Pos              6
#define GCR_ALTMFP_PC1_I2SBCLK_Msk              (1ul << GCR_ALTMFP_PC1_I2SBCLK_Pos)

#define GCR_ALTMFP_PC0_I2SLRCLK_Pos             5
#define GCR_ALTMFP_PC0_I2SLRCLK_Msk             (1ul << GCR_ALTMFP_PC0_I2SLRCLK_Pos)

#define GCR_ALTMFP_PB11_PWM4_Pos                4
#define GCR_ALTMFP_PB11_PWM4_Msk                (1ul << GCR_ALTMFP_PB11_PWM4_Pos)

#define GCR_ALTMFP_PB14_S31_Pos                 3
#define GCR_ALTMFP_PB14_S31_Msk                 (1ul << GCR_ALTMFP_PB14_S31_Pos)

#define GCR_ALTMFP_PA7_S21_Pos                  2
#define GCR_ALTMFP_PA7_S21_Msk                  (1ul << GCR_ALTMFP_PA7_S21_Pos)

#define GCR_ALTMFP_PB9_S11_Pos                  1
#define GCR_ALTMFP_PB9_S11_Msk                  (1ul << GCR_ALTMFP_PB9_S11_Pos)

#define GCR_ALTMFP_PB10_S01_Pos                 0
#define GCR_ALTMFP_PB10_S01_Msk                 (1ul << GCR_ALTMFP_PB10_S01_Pos)

/* GCR REGWRPROT Bit Field Definitions */
#define GCR_REGWRPROT_REGWRPROT_Pos             0
#define GCR_REGWRPROT_REGWRPROT_Msk             (0xFFul << GCR_REGWRPROT_REGWRPROT_Pos)


typedef struct
{
    __IO uint32_t  INTSRC:4;
    __I  uint32_t  RESERVE:28;
} GCR_IRQSRC_T;

typedef struct
{
    __IO uint32_t  NMISEL:5;
    __I  uint32_t  RESERVE0:3;
    __IO uint32_t  NMI_EN:1;
    __I  uint32_t  RESERVE1:23;
} GCR_NMISEL_T;


typedef __IO uint32_t GCR_MCUIRQ_T;

typedef struct
{
    union 
    {
        __I uint32_t u32IRQSRC[32];
        __I uint32_t IRQSRC[32];
    };

    union 
    {
        __IO uint32_t u32NMISEL;
        
        struct 
        {
            __IO uint32_t  NMISEL:5;
            __I  uint32_t  RESERVE0:3;
            __IO uint32_t  NMI_EN:1;
            __I  uint32_t  RESERVE1:23;
        } NMISEL;
    };

    union 
    {
        __IO uint32_t u32MCUIRQ;
        __IO uint32_t MCUIRQ;
    };
} GCR_INT_T;

/* GCR IRQSRC Bit Field Definitions */
#define GCR_IRQSRC_INT_SRC_Pos                  0
#define GCR_IRQSRC_INT_SRC_Msk                  (0xFul << GCR_IRQSRC_INT_SRC_Pos)

/* GCR NMISEL Bit Field Definitions */
#define GCR_NMISEL_NMI_EN_Pos                   8
#define GCR_NMISEL_NMI_EN_Msk                   (1ul << GCR_NMISEL_NMI_EN_Pos)

#define GCR_NMISEL_NMISEL_Pos                   0
#define GCR_NMISEL_NMISEL_Msk                   (0x1Ful << GCR_NMISEL_NMISEL_Pos)

/*-------------------------- FLASH Memory Controller -------------------------*/
typedef struct
{
    __IO uint32_t  ISPEN:1;
    __IO uint32_t  BS:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  APUEN:1;
    __IO uint32_t  CFGUEN:1;
    __IO uint32_t  LDUEN:1;
    __IO uint32_t  ISPFF:1;
    __I  uint32_t  RESERVE1:1;
    __IO uint32_t  PT:3;
    __I  uint32_t  RESERVE2:1;
    __IO uint32_t  ET:3;
    __I  uint32_t  RESERVE3:17;

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
    __IO uint32_t  LFOM:1;
    __I  uint32_t  RESERVE:27;
} FMC_FATCON_T;

typedef struct
{
    union 
    {
        __IO uint32_t u32ISPCON;
        
        struct 
        {
            __IO uint32_t  ISPEN:1;
            __IO uint32_t  BS:1;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  APUEN:1;
            __IO uint32_t  CFGUEN:1;
            __IO uint32_t  LDUEN:1;
            __IO uint32_t  ISPFF:1;
            __I  uint32_t  RESERVE1:1;
            __IO uint32_t  PT:3;
            __I  uint32_t  RESERVE2:1;
            __IO uint32_t  ET:3;
            __I  uint32_t  RESERVE3:17;
        } ISPCON;
    };

    union 
    {
        __IO uint32_t u32ISPADR;
        __IO uint32_t ISPADR;
    };

    union 
    {
        __IO uint32_t u32ISPDAT;
        __IO uint32_t ISPDAT;
    };
    
    union 
    {
        __IO uint32_t u32ISPCMD;
        
        struct 
        {
            __IO uint32_t  FCTRL:4;
            __IO uint32_t  FCEN:1;
            __IO uint32_t  FOEN:1;
            __I  uint32_t  RESERVE:26;
        } ISPCMD;
    };

    union 
    {
        __IO uint32_t u32ISPTRG;
        
        struct 
        {
            __IO uint32_t  ISPGO:1;
            __I  uint32_t  RESERVE:31;
        } ISPTRG;
    };
    
    union 
    {
        __I uint32_t u32DFBADR;
        __I uint32_t DFBADR;
    };

    union 
    {
        __IO uint32_t u32FATCON;
        
        struct 
        {
            __IO uint32_t  FPSEN:1;
            __IO uint32_t  FATS:3;
            __IO uint32_t  LFOM:1;
            __I  uint32_t  RESERVE:27;
        } FATCON;
    };
} FMC_T;

/* FMC ISPCON Bit Field Definitions */
#define FMC_ISPCON_ET_Pos                       12
#define FMC_ISPCON_ET_Msk                       (7ul << FMC_ISPCON_ET_Pos)

#define FMC_ISPCON_PT_Pos                       8
#define FMC_ISPCON_PT_Msk                       (7ul << FMC_ISPCON_PT_Pos)

#define FMC_ISPCON_ISPFF_Pos                    6
#define FMC_ISPCON_ISPFF_Msk                    (1ul << FMC_ISPCON_ISPFF_Pos)

#define FMC_ISPCON_LDUEN_Pos                    5
#define FMC_ISPCON_LDUEN_Msk                    (1ul << FMC_ISPCON_LDUEN_Pos)

#define FMC_ISPCON_CFGUEN_Pos                   4
#define FMC_ISPCON_CFGUEN_Msk                   (1ul << FMC_ISPCON_CFGUEN_Pos)

#define FMC_ISPCON_APUEN_Pos                    3
#define FMC_ISPCON_APUEN_Msk                    (1ul << FMC_ISPCON_APUEN_Pos)

#define FMC_ISPCON_BS_Pos                       1
#define FMC_ISPCON_BS_Msk                       (1ul << FMC_ISPCON_BS_Pos)

#define FMC_ISPCON_ISPEN_Pos                    0
#define FMC_ISPCON_ISPEN_Msk                    (1ul << FMC_ISPCON_ISPEN_Pos)

/* FMC ISPADR Bit Field Definitions */
#define FMC_ISPADR_ISPADR_Pos                   0
#define FMC_ISPADR_ISPADR_Msk                   (0xFFFFFFFFul << FMC_ISPADR_ISPADR_Pos)

/* FMC ISPADR Bit Field Definitions */
#define FMC_ISPDAT_ISPDAT_Pos                   0
#define FMC_ISPDAT_ISPDAT_Msk                   (0xFFFFFFFFul << FMC_ISPDAT_ISPDAT_Pos)

/* FMC ISPCMD Bit Field Definitions */
#define FMC_ISPCMD_FOEN_Pos                     5
#define FMC_ISPCMD_FOEN_Msk                     (1ul << FMC_ISPCMD_FOEN_Pos)

#define FMC_ISPCMD_FCEN_Pos                     4
#define FMC_ISPCMD_FCEN_Msk                     (1ul << FMC_ISPCMD_FCEN_Pos)

#define FMC_ISPCMD_FCTRL_Pos                    0
#define FMC_ISPCMD_FCTRL_Msk                    (0xFul << FMC_ISPCMD_FCTRL_Pos)

/* FMC ISPTRG Bit Field Definitions */
#define FMC_ISPTRG_ISPGO_Pos                    0
#define FMC_ISPTRG_ISPGO_Msk                    (1ul << FMC_ISPTRG_ISPGO_Pos)

/* FMC DFBADR Bit Field Definitions */
#define FMC_DFBADR_DFBA_Pos                     0
#define FMC_DFBADR_DFBA_Msk                     (0xFFFFFFFFul << FMC_DFBADR_DFBA_Pos)

/* FMC FATCON Bit Field Definitions */
#define FMC_FATCON_LFOM_Pos                     4
#define FMC_FATCON_LFOM_Msk                     (1ul << FMC_FATCON_LFOM_Pos)

#define FMC_FATCON_FATS_Pos                     1
#define FMC_FATCON_FATS_Msk                     (7ul << FMC_FATCON_FATS_Pos)

#define FMC_FATCON_FPSEN_Pos                    0
#define FMC_FATCON_FPSEN_Msk                    (1ul << FMC_FATCON_FPSEN_Pos)

/*------------------------ PS2 Device Interface Controller -------------------*/
typedef struct
{
    __IO uint32_t  PS2EN:1;
    __IO uint32_t  TXINTEN:1;
    __IO uint32_t  RXINTEN:1;
    __IO uint32_t  TXFIFO_DEPTH:4;
    __IO uint32_t  ACK:1;
    __IO uint32_t  CLRFIFO:1;
    __IO uint32_t  OVERRIDE:1;
    __IO uint32_t  FPS2CLK:1;
    __IO uint32_t  FPS2DAT:1;
    __I  uint32_t  RESERVE:20;
} PS2_CON_T;

typedef __IO uint32_t PS2_DATA_T;

typedef struct
{
    __IO uint32_t  PS2CLK:1;
    __IO uint32_t  PS2DATA:1;
    __IO uint32_t  FRAMERR:1;
    __IO uint32_t  RXPARTY:1;
    __IO uint32_t  RXBUSY:1;
    __IO uint32_t  TXBUSY:1;
    __IO uint32_t  RXOVF:1;
    __IO uint32_t  TXEMPTY:1;
    __IO uint32_t  BYTEIDX:4;
    __I  uint32_t  RESERVE:20;
} PS2_STATUS_T;

typedef struct
{
    __IO uint32_t  RXINT:1;
    __IO uint32_t  TXINT:1;
    __I  uint32_t  RESERVE:30;
} PS2_INTID_T;

typedef struct
{
    union
    {
        __IO uint32_t u32PS2CON;

        struct
        {
            __IO uint32_t  PS2EN:1;
            __IO uint32_t  TXINTEN:1;
            __IO uint32_t  RXINTEN:1;
            __IO uint32_t  TXFIFO_DEPTH:4;
            __IO uint32_t  ACK:1;
            __IO uint32_t  CLRFIFO:1;
            __IO uint32_t  OVERRIDE:1;
            __IO uint32_t  FPS2CLK:1;
            __IO uint32_t  FPS2DAT:1;
            __I  uint32_t  RESERVE:20;      
        } PS2CON;
    };

    union
    {
        __IO uint32_t u32TXDATA[4];
        __IO uint32_t TXDATA[4];        
    };

    union
    {
        __I uint32_t u32RXDATA;
        __I uint32_t RXDATA;
    };

    union
    {
        __IO uint32_t u32STATUS;

        struct
        {
            __IO uint32_t  PS2CLK:1;
            __IO uint32_t  PS2DATA:1;
            __IO uint32_t  FRAMERR:1;
            __IO uint32_t  RXPARTY:1;
            __IO uint32_t  RXBUSY:1;
            __IO uint32_t  TXBUSY:1;
            __IO uint32_t  RXOVF:1;
            __IO uint32_t  TXEMPTY:1;
            __IO uint32_t  BYTEIDX:4;
            __I  uint32_t  RESERVE:20;            
        } STATUS;
    };

    union
    {
        __IO uint32_t u32INTID;

        struct
        {
            __IO uint32_t  RXINT:1;
            __IO uint32_t  TXINT:1;
            __I  uint32_t  RESERVE:30;
        } INTID;          
    };
} PS2_T;

/* PS/2 PS2CON Bit Field Definitions */
#define PS2_PS2CON_PS2EN_Pos                       0
#define PS2_PS2CON_PS2EN_Msk                       (1ul << PS2_PS2CON_PS2EN_Pos)  

#define PS2_PS2CON_TXINTEN_Pos                     1
#define PS2_PS2CON_TXINTEN_Msk                     (1ul << PS2_PS2CON_TXINTEN_Pos) 

#define PS2_PS2CON_RXINTEN_Pos                     2
#define PS2_PS2CON_RXINTEN_Msk                     (1ul << PS2_PS2CON_RXINTEN_Pos)  

#define PS2_PS2CON_TXFIFO_DEPTH_Pos                3
#define PS2_PS2CON_TXFIFO_DEPTH_Msk                (0xFul << PS2_PS2CON_TXFIFO_DEPTH_Pos)

#define PS2_PS2CON_ACK_Pos                         7
#define PS2_PS2CON_ACK_Msk                         (1ul << PS2_PS2CON_ACK_Pos) 

#define PS2_PS2CON_CLRFIFO_Pos                     8
#define PS2_PS2CON_CLRFIFO_Msk                     (1ul << PS2_PS2CON_CLRFIFO_Pos)  

#define PS2_PS2CON_OVERRIDE_Pos                    9
#define PS2_PS2CON_OVERRIDE_Msk                    (1ul << PS2_PS2CON_OVERRIDE_Pos)  

#define PS2_PS2CON_FPS2CLK_Pos                     10
#define PS2_PS2CON_FPS2CLK_Msk                     (1ul << PS2_PS2CON_FPS2CLK_Pos) 

#define PS2_PS2CON_FPS2DAT_Pos                     11
#define PS2_PS2CON_FPS2DAT_Msk                     (1ul << PS2_PS2CON_FPS2DAT_Pos) 

/* PS/2 PS2RXDATA Bit Field Definitions */
#define PS2_PS2RXDATA_RXDATA_Pos                   0
#define PS2_PS2RXDATA_RXDATA_Msk                   (0xFFul << PS2_PS2RXDATA_RXDATA_Pos)

/* PS/2 PS2STATUS Bit Field Definitions */
#define PS2_PS2STATUS_PS2CLK_Pos                   0
#define PS2_PS2STATUS_PS2CLK_Msk                   (1ul << PS2_PS2STATUS_PS2CLK_Pos) 

#define PS2_PS2STATUS_PS2DATA_Pos                  1
#define PS2_PS2STATUS_PS2DATA_Msk                  (1ul << PS2_PS2STATUS_PS2DATA_Pos) 

#define PS2_PS2STATUS_FRAMERR_Pos                  2
#define PS2_PS2STATUS_FRAMERR_Msk                  (1ul << PS2_PS2STATUS_FRAMERR_Pos) 

#define PS2_PS2STATUS_RXPARITY_Pos                 3
#define PS2_PS2STATUS_RXPARITY_Msk                 (1ul << PS2_PS2STATUS_RXPARITY_Pos) 

#define PS2_PS2STATUS_RXPARITY_Pos                 3
#define PS2_PS2STATUS_RXPARITY_Msk                 (1ul << PS2_PS2STATUS_RXPARITY_Pos)

#define PS2_PS2STATUS_RXBUSY_Pos                   4
#define PS2_PS2STATUS_RXBUSY_Msk                   (1ul << PS2_PS2STATUS_RXBUSY_Pos)

#define PS2_PS2STATUS_TXBUSY_Pos                   5
#define PS2_PS2STATUS_TXBUSY_Msk                   (1ul << PS2_PS2STATUS_TXBUSY_Pos)

#define PS2_PS2STATUS_RXOVF_Pos                    6
#define PS2_PS2STATUS_RXOVF_Msk                    (1ul << PS2_PS2STATUS_RXOVF_Pos)

#define PS2_PS2STATUS_TXEMPTY_Pos                  7
#define PS2_PS2STATUS_TXEMPTY_Msk                  (1ul << PS2_PS2STATUS_TXEMPTY_Pos)

#define PS2_PS2STATUS_BYTEIDX_Pos                  8
#define PS2_PS2STATUS_BYTEIDX_Msk                  (0xFul << PS2_PS2STATUS_BYTEIDX_Pos)

/* PS/2 PS2INTID Bit Field Definitions */
#define PS2_PS2INTID_RXINT_Pos                     0
#define PS2_PS2INTID_RXINT_Msk                     (1ul << PS2_PS2INTID_RXINT_Pos) 
                     
#define PS2_PS2INTID_TXINT_Pos                     1
#define PS2_PS2INTID_TXINT_Msk                     (1ul << PS2_PS2INTID_TXINT_Pos) 

/*---------------------------- CAN Bus Controller ----------------------------*/

/*--------------- Controller Area Network Interface Register -----------------*/

/* CAN_MsgObj_T :Message interface structure */
typedef struct
{
    union 
    {
        __IO uint32_t u32CREQ;

        struct {
            __IO uint32_t  MSG_NUM:6;
            __I  uint32_t  RESERVE0:9;
            __IO uint32_t  BUSY:1;	  
            __I  uint32_t  RESERVE1:16;
        } CREQ;
    };
 
 
    union {
        __IO uint32_t  	u32CMASK;
        		
        struct {
            __IO uint32_t  DAT_B:1;
            __IO uint32_t  DAT_A:1;
            __IO uint32_t  TXRQST_NEWDAT:1;
            __IO uint32_t  CLRINTPND:1;
            __IO uint32_t  CONTROL:1;
            __IO uint32_t  ARB:1;
            __IO uint32_t  MASK:1;
            __IO uint32_t  WRRD:1;
            __I  uint32_t  RESERVE0:24;
        } CMASK;
    };
 
    
    union {
        __IO uint32_t  	u32MASK1;
        		
        struct {
            __IO uint32_t  MASK15_0:16;
            __I  uint32_t  RESERVE0:16;
        } MASK1;
    };


    union {
        __IO uint32_t  	u32MASK2;
        		
        struct {
            __IO uint32_t  MASK28_16:13;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  MDIR:1;
            __IO uint32_t  MXTD:1;
            __I  uint32_t  RESERVE1:16;
        } MASK2;
    };
    
    union {
        __IO uint32_t  	u32ARB1;

        struct {
            __IO uint32_t  ID15_0:16;
            __I  uint32_t  RESERVE0:16;
        } ARB1;
    };    
 

    union {
        __IO uint32_t  	u32ARB2;	

        struct {
            __IO uint32_t  ID28_16:13;
            __IO uint32_t  DIR:1;
            __IO uint32_t  XTD:1;
            __IO uint32_t  MSGVAL:1;
            __I  uint32_t  RESERVE0:16;
        } ARB2;
    };    

    
    union {
        __IO uint32_t  	u32MCON;			

        struct {
            __IO uint32_t  DLC:4;
            __I  uint32_t  RESERVE0:3;
            __IO uint32_t  EOB:1;
            __IO uint32_t  TXRQST:1;
            __IO uint32_t  RMTEN:1;
            __IO uint32_t  RXIE:1;
            __IO uint32_t  TXIE:1;
            __IO uint32_t  UMASK:1;
            __IO uint32_t  INTPND:1;
            __IO uint32_t  MSGLST:1;
            __IO uint32_t  NEWDAT:1;
            __I  uint32_t  RESERVE1:16;
         } MCON;
    };    

    union {
        __IO uint32_t	u32DAT_A1;			

        struct {
            __IO uint32_t  DATA0:8;
            __IO uint32_t  DATA1:8;
            __I  uint32_t  RESERVE0:16;
        } DAT_A1;
    };     
    
    union {
        __IO uint32_t 	u32DAT_A2;		

        struct {
            __IO uint32_t  DATA2:8;
            __IO uint32_t  DATA3:8;
            __I  uint32_t  RESERVE0:16;
        } DAT_A2;
    };     
    
    union {
        __IO uint32_t 	u32DAT_B1;

        struct {
            __IO uint32_t  DATA4:8;
            __IO uint32_t  DATA5:8;
            __I  uint32_t  RESERVE0:16;
        } DAT_B1;
    };     

    union {
        __IO uint32_t	u32DAT_B2;		

        struct {
            __IO uint32_t  DATA6:8;
            __IO uint32_t  DATA7:8;
            __I  uint32_t  RESERVE0:16;
        } DAT_B2;
    };     


  __I uint32_t RESERVE0[13];        
                                    
} CAN_MsgObj_T;


typedef struct
{
    union {
        __IO uint32_t   u32CON;		 	
        
        struct {
            __IO uint32_t  INIT:1;
            __IO uint32_t  IE:1;
            __IO uint32_t  SIE:1;
            __IO uint32_t  EIE:1;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  DAR:1;	  
            __IO uint32_t  CCE:1;
            __IO uint32_t  TEST:1;
            __I  uint32_t  RESERVE1:24;
        } CON;
    };

    union {
        __IO uint32_t   u32STATUS;	

        struct {
            __IO uint32_t  LEC:3;
            __IO uint32_t  TXOK:1;
            __IO uint32_t  RXOK:1;
            __IO uint32_t  EPASS:1;
            __IO uint32_t  EWARN:1;	  
            __IO uint32_t  BOFF:1;
            __I  uint32_t  RESERVE0:24;
        } STATUS;
    };

    union {
        __IO uint32_t   u32ERR;     		
        
        struct {
            __IO uint32_t  TEC:8;
            __IO uint32_t  REC:7;
            __IO uint32_t  RP:1;
            __I  uint32_t  RESERVE0:16;
        } ERR;
    };  	

    union {
	    __IO uint32_t   u32BTIME;
        
        struct {
            __IO uint32_t  BRP:6;
            __IO uint32_t  SJW:2;
            __IO uint32_t  TSEG1:4;
            __IO uint32_t  TSEG2:3;
            __I  uint32_t  RESERVE0:17;
        } BTIME;
    };
	
    union {
        __IO uint32_t   u32IIDR;
        
        struct {
            __IO uint32_t  INTID:16;
            __I  uint32_t  RESERVE0:16;
        } IIDR;
    };

   union {
        __IO uint32_t   u32TEST;
                 
        struct {
            __I  uint32_t  RESERVE0:2;
            __IO uint32_t  BASIC:1;
            __IO uint32_t  SILENT:1;
            __IO uint32_t  LBACK:1;
            __IO uint32_t  TX:2;
            __IO uint32_t  RX:1;	  
            __I  uint32_t  RESERVE1:24;
        } TEST;
   };
    
   union {
        __IO uint32_t   u32BRPE;
        
        struct {
            __IO uint32_t  BPRE:4;
            __I  uint32_t  RESERVE0:28;
        } BRPE;
   };
    __I uint32_t   	RESERVE0[1];     
	
    CAN_MsgObj_T    sMsgObj[2];             
                              
	__I uint32_t   	RESERVE1[8];

   union {
        __IO uint32_t   u32TXREQ1;
        
        struct {
            __IO uint32_t  TXRQST16_1:16;
            __I  uint32_t  RESERVE0:16;
        } TXREQ1;
   };


   union {
        __IO uint32_t   u32TXREQ2;
        
        struct {
            __IO uint32_t  TXRQST32_17:16;
            __I  uint32_t  RESERVE0:16;
        } TXREQ2;
   };

    __I uint32_t   	RESERVE2[6];        

   union {
        __IO uint32_t   u32NDAT1;

        struct {
            __IO uint32_t  NEWDAT16_1:16;
            __I  uint32_t  RESERVE0:16;
        } NDAT1;
   };

   union {
        __IO uint32_t   u32NDAT2;

        struct {
            __IO uint32_t  NEWDAT32_17:16;
            __I  uint32_t  RESERVE0:16;
        } NDAT2;
   };

    __I uint32_t   	RESERVE3[6];

   union {
        __IO uint32_t   u32IPND1;

        struct {
            __IO uint32_t  IPND16_1:16;
            __I  uint32_t  RESERVE0:16;
        } IPND1;
   };

   union {
        __IO uint32_t   u32IPND2;

        struct {
            __IO uint32_t  IPND32_17:16;
            __I  uint32_t  RESERVE0:16;
        } IPND2;
   };

    __I uint32_t   	RESERVE4[6];

   union {
        __IO uint32_t   u32MVLD1;

        struct {
            __IO uint32_t  MSGVAL16_1:16;
            __I  uint32_t  RESERVE0:16;
        } MVLD1;
   };

   union {
        __IO uint32_t   u32MVLD2;

        struct {
            __IO uint32_t  MSGVAL32_17:16;
            __I  uint32_t  RESERVE0:16;
        } MVLD2;
   };
    
    union {
        __IO uint32_t   u32WU_EN;            
        struct {
            __IO uint32_t  WAKUP_EN:1;
            __I  uint32_t  RESERVE0:31;
        } WU_EN;
    }; 

    union {
        __IO uint32_t   u32WU_STATUS;
        struct {
            __IO uint32_t  WAKUP_STS:1;
            __I  uint32_t  RESERVE0:31;
        } WU_STATUS;
    }; 

    
} CAN_T;


/* CAN CON Bit Field Definitions */
#define CAN_CON_TEST_Pos           7
#define CAN_CON_TEST_Msk           (1ul << CAN_CON_TEST_Pos)

#define CAN_CON_CCE_Pos            6
#define CAN_CON_CCE_Msk            (1ul << CAN_CON_CCE_Pos)

#define CAN_CON_DAR_Pos            5
#define CAN_CON_DAR_Msk            (1ul << CAN_CON_DAR_Pos)

#define CAN_CON_EIE_Pos            3
#define CAN_CON_EIE_Msk            (1ul << CAN_CON_EIE_Pos)

#define CAN_CON_SIE_Pos            2
#define CAN_CON_SIE_Msk            (1ul << CAN_CON_SIE_Pos)

#define CAN_CON_IE_Pos             1
#define CAN_CON_IE_Msk             (1ul << CAN_CON_IE_Pos)

#define CAN_CON_INIT_Pos           0
#define CAN_CON_INIT_Msk           (1ul << CAN_CON_INIT_Pos)

/* CAN STATUS Bit Field Definitions */
#define CAN_STATUS_BOFF_Pos        7
#define CAN_STATUS_BOFF_Msk        (1ul << CAN_STATUS_BOFF_Pos)

#define CAN_STATUS_EWARN_Pos       6
#define CAN_STATUS_EWARN_Msk       (1ul << CAN_STATUS_EWARN_Pos)

#define CAN_STATUS_EPASS_Pos       5
#define CAN_STATUS_EPASS_Msk       (1ul << CAN_STATUS_EPASS_Pos)

#define CAN_STATUS_RXOK_Pos        4
#define CAN_STATUS_RXOK_Msk        (1ul << CAN_STATUS_RXOK_Pos)

#define CAN_STATUS_TXOK_Pos        3
#define CAN_STATUS_TXOK_Msk        (1ul << CAN_STATUS_RXOK_Pos)

#define CAN_STATUS_LEC_Pos         0
#define CAN_STATUS_LEC_Msk         (0x3ul << CAN_STATUS_LEC_Pos)

/* CAN ERR Bit Field Definitions */
#define CAN_ERR_RP_Pos             15
#define CAN_ERR_RP_Msk             (1ul << CAN_ERR_RP_Pos)

#define CAN_ERR_REC_Pos            8
#define CAN_ERR_REC_Msk            (0x7Ful << CAN_ERR_REC_Pos)

#define CAN_ERR_TEC_Pos            0
#define CAN_ERR_TEC_Msk            (0xFFul << CAN_ERR_TEC_Pos)

/* CAN BTIME Bit Field Definitions */
#define CAN_BTIME_TSEG2_Pos        12
#define CAN_BTIME_TSEG2_Msk        (0x7ul << CAN_BTIME_TSEG2_Pos)

#define CAN_BTIME_TSEG1_Pos        8
#define CAN_BTIME_TSEG1_Msk        (0xFul << CAN_BTIME_TSEG1_Pos)

#define CAN_BTIME_SJW_Pos          6
#define CAN_BTIME_SJW_Msk          (0x3ul << CAN_BTIME_SJW_Pos)

#define CAN_BTIME_BRP_Pos          0
#define CAN_BTIME_BRP_Msk          (0x3Ful << CAN_BTIME_BRP_Pos)

/* CAN IIDR Bit Field Definitions */
#define CAN_IIDR_INTID_Pos         0
#define CAN_IIDR_INTID_Msk         (0xFFFFul << CAN_IIDR_INTID_Pos)

/* CAN TEST Bit Field Definitions */
#define CAN_TEST_RX_Pos            7
#define CAN_TEST_RX_Msk            (1ul << CAN_TEST_RX_Pos)

#define CAN_TEST_TX_Pos            5
#define CAN_TEST_TX_Msk            (0x3ul << CAN_TEST_TX_Pos)

#define CAN_TEST_LBACK_Pos         4
#define CAN_TEST_LBACK_Msk         (1ul << CAN_TEST_LBACK_Pos)
             
#define CAN_TEST_SILENT_Pos        3
#define CAN_TEST_SILENT_Msk        (1ul << CAN_TEST_SILENT_Pos)

#define CAN_TEST_BASIC_Pos         2
#define CAN_TEST_BASIC_Msk         (1ul << CAN_TEST_BASIC_Pos)

#define CAN_TEST_BASIC_Pos         2
#define CAN_TEST_BASIC_Msk         (1ul << CAN_TEST_BASIC_Pos)

/* CAN BPRE Bit Field Definitions */
#define CAN_BPRE_BPRE_Pos          0
#define CAN_BPRE_BPRE_Msk          (0xFul << CAN_BPRE_BPRE_Pos)

/* CAN IFn_CREQ Bit Field Definitions */
#define CAN_IFnCREQ_BUSY_Pos       15
#define CAN_IFnCREQ_BUSY_Msk       (1ul << CAN_IFnCREQ_BUSY_Pos)

#define CAN_IFnCREQ_MSGNUM_Pos     0
#define CAN_IFnCREQ_MSGNUM_Msk     (0x3Ful << CAN_IFnCREQ_MSGNUM_Pos)

/* CAN IFn_CMASK Bit Field Definitions */
#define CAN_IFnCMASK_WRRD_Pos      7
#define CAN_IFnCMASK_WRRD_Msk      (1ul << CAN_IFnCMASK_WRRD_Pos)

#define CAN_IFnCMASK_MASK_Pos      6
#define CAN_IFnCMASK_MASK_Msk      (1ul << CAN_IFnCMASK_MASK_Pos)

#define CAN_IFnCMASK_ARB_Pos       5
#define CAN_IFnCMASK_ARB_Msk       (1ul << CAN_IFnCMASK_ARB_Pos)

#define CAN_IFnCMASK_CONTROL_Pos   4
#define CAN_IFnCMASK_CONTROL_Msk   (1ul << CAN_IFnCMASK_CONTROL_Pos)

#define CAN_IFnCMASK_CLRINTPND_Pos 3
#define CAN_IFnCMASK_CLRINTPND_Msk (1ul << CAN_IFnCMASK_CLRINTPND_Pos)
  
#define CAN_IFnCMASK_TXRQSTNEWDAT_Pos 2
#define CAN_IFnCMASK_TXRQSTNEWDAT_Msk (1ul << CAN_IFnCMASK_TXRQSTNEWDAT_Pos)

#define CAN_IFnCMASK_DATAA_Pos     1
#define CAN_IFnCMASK_DATAA_Msk     (1ul << CAN_IFnCMASK_DATAA_Pos)

#define CAN_IFnCMASK_DATAB_Pos     0
#define CAN_IFnCMASK_DATAB_Msk     (1ul << CAN_IFnCMASK_DATAB_Pos)

/* CAN IFn_MASK1 Bit Field Definitions */
#define CAN_IFnMASK1_MSK_Pos       0
#define CAN_IFnMASK1_MSK_Msk       (0xFFul << CAN_IFnMASK1_MSK_Pos)

/* CAN IFn_MASK2 Bit Field Definitions */
#define CAN_IFnMASK2_MXTD_Pos      15
#define CAN_IFnMASK2_MXTD_Msk      (1ul << CAN_IFnMASK2_MXTD_Pos)

#define CAN_IFnMASK2_MDIR_Pos      14
#define CAN_IFnMASK2_MDIR_Msk      (1ul << CAN_IFnMASK2_MDIR_Pos)

#define CAN_IFnMASK2_MSK_Pos       0
#define CAN_IFnMASK2_MSK_Msk       (0x1FFul << CAN_IFnMASK2_MSK_Pos)

/* CAN IFn_ARB1 Bit Field Definitions */
#define CAN_IFnARB1_ID_Pos         0
#define CAN_IFnARB1_ID_Msk         (0xFFFFul << CAN_IFnARB1_ID_Pos)

/* CAN IFn_ARB2 Bit Field Definitions */
#define CAN_IFnARB2_MSGVAL_Pos     15
#define CAN_IFnARB2_MSGVAL_Msk     (1ul << CAN_IFnARB2_MSGVAL_Pos)

#define CAN_IFnARB2_XTD_Pos        14
#define CAN_IFnARB2_XTD_Msk        (1ul << CAN_IFnARB2_XTD_Pos)

#define CAN_IFnARB2_DIR_Pos        13
#define CAN_IFnARB2_DIR_Msk        (1ul << CAN_IFnARB2_DIR_Pos)

#define CAN_IFnARB2_ID_Pos         0
#define CAN_IFnARB2_ID_Msk         (0x1FFFul << CAN_IFnARB2_ID_Pos)

/* CAN IFn_MCON Bit Field Definitions */
#define CAN_IFnMCON_NEWDAT_Pos     15
#define CAN_IFnMCON_NEWDAT_Msk     (1ul << CAN_IFnMCON_NEWDAT_Pos)

#define CAN_IFnMCON_MSGLST_Pos     14
#define CAN_IFnMCON_MSGLST_Msk     (1ul << CAN_IFnMCON_MSGLST_Pos)

#define CAN_IFnMCON_INTPND_Pos     13
#define CAN_IFnMCON_INTPND_Msk     (1ul << CAN_IFnMCON_INTPND_Pos)

#define CAN_IFnMCON_UMASK_Pos      12
#define CAN_IFnMCON_UMASK_Msk      (1ul << CAN_IFnMCON_UMASK_Pos)

#define CAN_IFnMCON_TXIE_Pos       11
#define CAN_IFnMCON_TXIE_Msk       (1ul << CAN_IFnMCON_TXIE_Pos)

#define CAN_IFnMCON_RXIE_Pos       10
#define CAN_IFnMCON_RXIE_Msk       (1ul << CAN_IFnMCON_RXIE_Pos)

#define CAN_IFnMCON_RMTEN_Pos      9
#define CAN_IFnMCON_RMTEN_Msk      (1ul << CAN_IFnMCON_RMTEN_Pos)

#define CAN_IFnMCON_TXRQST_Pos     8
#define CAN_IFnMCON_TXRQST_Msk     (1ul << CAN_IFnMCON_TXRQST_Pos)

#define CAN_IFnMCON_EOB_Pos        7
#define CAN_IFnMCON_EOB_Msk        (1ul << CAN_IFnMCON_EOB_Pos)

#define CAN_IFnMCON_DLC_Pos        0
#define CAN_IFnMCON_DLC_Msk        (0xFul << CAN_IFnMCON_DLC_Pos)

/* CAN IFn_DATA_A1 Bit Field Definitions */
#define CAN_IFnDATAA1_DATA1_Pos    8
#define CAN_IFnDATAA1_DATA1_Msk    (0xFul << CAN_IFnDATAA1_DATA1_Pos)

#define CAN_IFnDATAA1_DATA0_Pos    0
#define CAN_IFnDATAA1_DATA0_Msk    (0xFul << CAN_IFnDATAA1_DATA0_Pos)

/* CAN IFn_DATA_A2 Bit Field Definitions */
#define CAN_IFnDATAA2_DATA3_Pos    8
#define CAN_IFnDATAA2_DATA3_Msk    (0xFul << CAN_IFnDATAA2_DATA3_Pos)

#define CAN_IFnDATAA2_DATA2_Pos    0
#define CAN_IFnDATAA2_DATA2_Msk    (0xFul << CAN_IFnDATAA2_DATA2_Pos)

/* CAN IFn_DATA_B1 Bit Field Definitions */
#define CAN_IFnDATAB1_DATA5_Pos    8
#define CAN_IFnDATAB1_DATA5_Msk    (0xFul << CAN_IFnDATAB1_DATA5_Pos)

#define CAN_IFnDATAB1_DATA4_Pos    0
#define CAN_IFnDATAB1_DATA4_Msk    (0xFul << CAN_IFnDATAB1_DATA4_Pos)

/* CAN IFn_DATA_B2 Bit Field Definitions */
#define CAN_IFnDATAB2_DATA7_Pos    8
#define CAN_IFnDATAB2_DATA7_Msk    (0xFul << CAN_IFnDATAB2_DATA7_Pos)

#define CAN_IFnDATAB2_DATA6_Pos    8
#define CAN_IFnDATAB2_DATA6_Msk    (0xFul << CAN_IFnDATAB2_DATA6_Pos)

/* CAN IFn_TXRQST1 Bit Field Definitions */
#define CAN_IFnTXRQST1_TXRQST_Pos  0
#define CAN_IFnTXRQST1_TXRQST_Msk  (0xFFFFul << CAN_IFnTXRQST1_TXRQST_Pos)

/* CAN IFn_TXRQST2 Bit Field Definitions */
#define CAN_IFnTXRQST2_TXRQST_Pos  0
#define CAN_IFnTXRQST2_TXRQST_Msk  (0xFFFFul << CAN_IFnTXRQST2_TXRQST_Pos)

/* CAN IFn_NDAT1 Bit Field Definitions */
#define CAN_IFnNDAT1_NEWDATA_Pos   0
#define CAN_IFnNDAT1_NEWDATA_Msk   (0xFFFFul << CAN_IFnNDAT1_NEWDATA_Pos)

/* CAN IFn_NDAT2 Bit Field Definitions */
#define CAN_IFnNDAT2_NEWDATA_Pos   0
#define CAN_IFnNDAT2_NEWDATA_Msk   (0xFFFFul << CAN_IFnNDAT2_NEWDATA_Pos)

/* CAN IFn_IPND1 Bit Field Definitions */
#define CAN_IFnIPND1_INTPND_Pos   0
#define CAN_IFnIPND1_INTPND_Msk   (0xFFFFul << CAN_IFnIPND1_INTPND_Pos)

/* CAN IFn_IPND2 Bit Field Definitions */
#define CAN_IFnIPND2_INTPND_Pos   0
#define CAN_IFnIPND2_INTPND_Msk   (0xFFFFul << CAN_IFnIPND2_INTPND_Pos)

/* CAN IFn_MVLD1 Bit Field Definitions */
#define CAN_IFnMVLD1_MSGVAL_Pos   0
#define CAN_IFnMVLD1_MSGVAL_Msk   (0xFFFFul << CAN_IFnMVLD1_MSGVAL_Pos)

/* CAN IFn_MVLD2 Bit Field Definitions */
#define CAN_IFnMVLD2_MSGVAL_Pos   0
#define CAN_IFnMVLD2_MSGVAL_Msk   (0xFFFFul << CAN_IFnMVLD2_MSGVAL_Pos)

/* CAN WUEN Bit Field Definitions */
#define CAN_WUEN_WAKUP_EN_Pos     0
#define CAN_WUEN_WAKUP_EN_Msk    (1ul << CAN_WUEN_WAKUP_EN_Pos)

/* CAN WUSTATUS Bit Field Definitions */
#define CAN_WUSTATUS_WAKUP_STS_Pos     0
#define CAN_WUSTATUS_WAKUP_STS_Msk    (1ul << CAN_WUSTATUS_WAKUP_STS_Pos)
                                  
/*--------------------------- USB Device Controller --------------------------*/
typedef struct
{
    __IO uint32_t  BUS_IE:1;
    __IO uint32_t  USB_IE:1;
    __IO uint32_t  FLDET_IE:1;
    __IO uint32_t  WAKEUP_IE:1;
    __I  uint32_t  RESERVE0:4;
    __IO uint32_t  WAKEUP_EN:1;
    __I  uint32_t  RESERVE1:6;
    __IO uint32_t  INNAK_EN:1;
    __I  uint32_t  RESERVE2:16;
} USBD_INTEN_T;

typedef struct
{
    __IO uint32_t  BUS_STS:1;
    __IO uint32_t  USB_STS:1;
    __IO uint32_t  FLDET_STS:1;
    __IO uint32_t  WAKEUP_STS:1;
    __I  uint32_t  RESERVE0:12;
    __IO uint32_t  EPEVT:6;
    __I  uint32_t  RESERVE1:9;
    __IO uint32_t  SETUP:1;
} USBD_INTSTS_T;

typedef struct
{
    __IO uint32_t  FADDR:7;
    __I  uint32_t  RESERVE:25;
} USBD_FADDR_T;

typedef struct
{
    __I  uint32_t  RESERVE0:7;
    __I  uint32_t  OVERRUN:1;
    __I  uint32_t  EPSTS0:3;
    __I  uint32_t  EPSTS1:3;
    __I  uint32_t  EPSTS2:3;
    __I  uint32_t  EPSTS3:3;
    __I  uint32_t  EPSTS4:3;
    __I  uint32_t  EPSTS5:3;
    __I  uint32_t  RESERVE1:6;
} USBD_EPSTS_T;



typedef struct
{
    __I  uint32_t  USBRST:1;
    __I  uint32_t  SUSPEND:1;
    __I  uint32_t  RESUME:1;
    __I  uint32_t  TIMEOUT:1;
    __IO uint32_t  PHY_EN:1;
    __IO uint32_t  RWAKEUP:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  USB_EN:1;
    __IO uint32_t  DPPU_EN:1;
    __IO uint32_t  PWRDN:1;
    __I  uint32_t  RESERVE1:22;
} USBD_ATTR_T;



typedef struct
{
    __IO uint32_t  FLDET:1;
    __I  uint32_t  RESERVE:31;
} USBD_FLDET_T;

typedef struct
{
    __I  uint32_t  RESERVE0:3;
    __IO uint32_t  STBUFSEG:6;
    __I  uint32_t  RESERVE:23;
} USBD_STBUFSEG_T;

typedef struct
{
    __IO uint32_t  MXPLD:9;
    __I  uint32_t  RESERVE:23;
} USBD_MXPLD_T;

typedef struct
{
    __IO uint32_t  EP_NUM:4;
    __IO uint32_t  ISOCH:1;
    __IO uint32_t  STATE:2;
    __IO uint32_t  DSQ_SYNC:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  CSTALL:1;
    __I  uint32_t  RESERVE1:22;
} USBD_CFG_T;

typedef struct
{
    __IO uint32_t  CLRRDY:1;
    __IO uint32_t  SSTALL:1;
    __I  uint32_t  RESERVE:30;
} USBD_CFGP_T;

typedef struct
{
    __IO uint32_t  DRVSE0:1;
    __I  uint32_t  RESERVE:31;
} USBD_DRVSE0_T;

typedef struct
{
    union {
        __IO uint32_t u32BUFSEG;
        struct {
            __I  uint32_t  RESERVE0:3;
            __IO uint32_t  BUFSEG:6;
            __I  uint32_t  RESERVE:23;
        } BUFSEG;
    };
    
    union {
        __IO uint32_t u32MXPLD;
        struct {
            __IO uint32_t  MXPLD:9;
            __I  uint32_t  RESERVE:23;
        } MXPLD;
    };
    
    union {
        __IO uint32_t u32CFG;
        struct {
            __IO uint32_t  EP_NUM:4;
            __IO uint32_t  ISOCH:1;
            __IO uint32_t  STATE:2;
            __IO uint32_t  DSQ_SYNC:1;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  CSTALL:1;
            __I  uint32_t  RESERVE1:22;
        } CFG;
    };
    
    union {
        __IO uint32_t u32CFGP;
        struct {
            __IO uint32_t  CLRRDY:1;
            __IO uint32_t  SSTALL:1;
            __I  uint32_t  RESERVE:30;
        } CFGP;
    };
    
} USBD_EP_T;


typedef struct
{
    union {
        __IO uint32_t u32INTEN;
        struct {
            __IO uint32_t  BUS_IE:1;
            __IO uint32_t  USB_IE:1;
            __IO uint32_t  FLDET_IE:1;
            __IO uint32_t  WAKEUP_IE:1;
            __I  uint32_t  RESERVE0:4;
            __IO uint32_t  WAKEUP_EN:1;
            __I  uint32_t  RESERVE1:6;
            __IO uint32_t  INNAK_EN:1;
            __I  uint32_t  RESERVE2:16;
        } INTEN;
    };
    
    union {
        __IO uint32_t u32INTSTS;
        struct {
            __IO uint32_t  BUS_STS:1;
            __IO uint32_t  USB_STS:1;
            __IO uint32_t  FLDET_STS:1;
            __IO uint32_t  WAKEUP_STS:1;
            __I  uint32_t  RESERVE0:12;
            __IO uint32_t  EPEVT:6;
            __I  uint32_t  RESERVE1:9;
            __IO uint32_t  SETUP:1;
        } INTSTS;
    };
    
    union {
        __IO uint32_t u32FADDR;
        struct {
            __IO uint32_t  FADDR:7;
            __I  uint32_t  RESERVE:25;
        } FADDR;
    };
    
    union {
        __IO uint32_t u32EPSTS;
        struct {
            __I  uint32_t  RESERVE0:7;
            __I  uint32_t  OVERRUN:1;
            __I  uint32_t  EPSTS0:3;
            __I  uint32_t  EPSTS1:3;
            __I  uint32_t  EPSTS2:3;
            __I  uint32_t  EPSTS3:3;
            __I  uint32_t  EPSTS4:3;
            __I  uint32_t  EPSTS5:3;
            __I  uint32_t  RESERVE1:6;
        } EPSTS;
    };
    
    union {
        __IO uint32_t u32ATTR;
        struct {
            __I  uint32_t  USBRST:1;
            __I  uint32_t  SUSPEND:1;
            __I  uint32_t  RESUME:1;
            __I  uint32_t  TIMEOUT:1;
            __IO uint32_t  PHY_EN:1;
            __IO uint32_t  RWAKEUP:1;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  USB_EN:1;
            __IO uint32_t  DPPU_EN:1;
            __IO uint32_t  PWRDN:1;
            __I  uint32_t  RESERVE1:22;
        } ATTR;
    };
    
    union {
        __IO uint32_t u32FLDET;
        struct {
            __I  uint32_t  FLDET:1;
            __I  uint32_t  RESERVE:31;
        } FLDET;
    };
    
    union {
        __IO uint32_t u32STBUFSEG;
        struct {
            __I  uint32_t  RESERVE0:3;
            __IO uint32_t  STBUFSEG:6;
            __I  uint32_t  RESERVE:23;
        } STBUFSEG;
    };
    
    __I uint32_t RESERVE0;
    
    USBD_EP_T EP[6];
    
    __I uint32_t RESERVE1[4];
    
    union {
        __IO uint32_t u32DRVSE0;
        struct {
            __IO uint32_t  DRVSE0:1;
            __I  uint32_t  RESERVE:31;
        } DRVSE0;
    };
    
     __I uint32_t RESERVE2[4];
    
} USBD_T;

/* USBD INTEN Bit Field Definitions */
#define USBD_INTEN_INNAK_EN_Pos    15
#define USBD_INTEN_INNAK_EN_Msk    (1ul << USBD_INTEN_INNAK_EN_Pos)

#define USBD_INTEN_WAKEUP_EN_Pos   8
#define USBD_INTEN_WAKEUP_EN_Msk   (1ul << USBD_INTEN_WAKEUP_EN_Pos)

#define USBD_INTEN_WAKEUP_IE_Pos   3
#define USBD_INTEN_WAKEUP_IE_Msk   (1ul << USBD_INTEN_WAKEUP_IE_Pos)

#define USBD_INTEN_FLDET_IE_Pos    2
#define USBD_INTEN_FLDET_IE_Msk    (1ul << USBD_INTEN_FLDET_IE_Pos)

#define USBD_INTEN_USB_IE_Pos      1
#define USBD_INTEN_USB_IE_Msk      (1ul << USBD_INTEN_USB_IE_Pos)

#define USBD_INTEN_BUS_IE_Pos      0
#define USBD_INTEN_BUS_IE_Msk      (1ul << USBD_INTEN_BUS_IE_Pos)

/* USBD INTSTS Bit Field Definitions */
#define USBD_INTSTS_SETUP_Pos        31
#define USBD_INTSTS_SETUP_Msk        (1ul << USBD_INTSTS_SETUP_Pos)

#define USBD_INTSTS_EPEVT_Pos        16
#define USBD_INTSTS_EPEVT_Msk        (0x3Ful << USBD_INTSTS_EPEVT_Pos)

#define USBD_INTSTS_WAKEUP_STS_Pos   3
#define USBD_INTSTS_WAKEUP_STS_Msk   (1ul << USBD_INTSTS_WAKEUP_STS_Pos)

#define USBD_INTSTS_FLDET_STS_Pos    2
#define USBD_INTSTS_FLDET_STS_Msk    (1ul << USBD_INTSTS_FLDET_STS_Pos)

#define USBD_INTSTS_USB_STS_Pos      1
#define USBD_INTSTS_USB_STS_Msk      (1ul << USBD_INTSTS_USB_STS_Pos)

#define USBD_INTSTS_BUS_STS_Pos      0
#define USBD_INTSTS_BUS_STS_Msk      (1ul << USBD_INTSTS_BUS_STS_Pos)

/* USBD FADDR Bit Field Definitions */
#define USBD_FADDR_FADDR_Pos     0
#define USBD_FADDR_FADDR_Msk     (0x7Ful << USBD_FADDR_FADDR_Pos)

/* USBD EPSTS Bit Field Definitions */
#define USBD_EPSTS_EPSTS5_Pos    23
#define USBD_EPSTS_EPSTS5_Msk    (7ul << USBD_EPSTS_EPSTS5_Pos)

#define USBD_EPSTS_EPSTS4_Pos    20
#define USBD_EPSTS_EPSTS4_Msk    (7ul << USBD_EPSTS_EPSTS4_Pos)

#define USBD_EPSTS_EPSTS3_Pos    17
#define USBD_EPSTS_EPSTS3_Msk    (7ul << USBD_EPSTS_EPSTS3_Pos)

#define USBD_EPSTS_EPSTS2_Pos    14
#define USBD_EPSTS_EPSTS2_Msk    (7ul << USBD_EPSTS_EPSTS2_Pos)

#define USBD_EPSTS_EPSTS1_Pos    11
#define USBD_EPSTS_EPSTS1_Msk    (7ul << USBD_EPSTS_EPSTS1_Pos)

#define USBD_EPSTS_EPSTS0_Pos    8
#define USBD_EPSTS_EPSTS0_Msk    (7ul << USBD_EPSTS_EPSTS0_Pos)

#define USBD_EPSTS_OVERRUN_Pos   7
#define USBD_EPSTS_OVERRUN_Msk   (1ul << USBD_EPSTS_OVERRUN_Pos)

/* USBD ATTR Bit Field Definitions */
#define USBD_ATTR_PWRDN_Pos      9
#define USBD_ATTR_PWRDN_Msk      (1ul << USBD_ATTR_PWRDN_Pos)

#define USBD_ATTR_DPPU_EN_Pos    8
#define USBD_ATTR_DPPU_EN_Msk    (1ul << USBD_ATTR_DPPU_EN_Pos)

#define USBD_ATTR_USB_EN_Pos     7
#define USBD_ATTR_USB_EN_Msk     (1ul << USBD_ATTR_USB_EN_Pos)

#define USBD_ATTR_RWAKEUP_Pos    5
#define USBD_ATTR_RWAKEUP_Msk    (1ul << USBD_ATTR_RWAKEUP_Pos)

#define USBD_ATTR_PHY_EN_Pos     4
#define USBD_ATTR_PHY_EN_Msk     (1ul << USBD_ATTR_PHY_EN_Pos)

#define USBD_ATTR_TIMEOUT_Pos    3
#define USBD_ATTR_TIMEOUT_Msk    (1ul << USBD_ATTR_TIMEOUT_Pos)

#define USBD_ATTR_RESUME_Pos     2
#define USBD_ATTR_RESUME_Msk     (1ul << USBD_ATTR_RESUME_Pos)

#define USBD_ATTR_SUSPEND_Pos    1
#define USBD_ATTR_SUSPEND_Msk    (1ul << USBD_ATTR_SUSPEND_Pos)

#define USBD_ATTR_USBRST_Pos     0
#define USBD_ATTR_USBRST_Msk     (1ul << USBD_ATTR_USBRST_Pos)

/* USBD FLDET Bit Field Definitions */
#define USBD_FLDET_FLDET_Pos     0
#define USBD_FLDET_FLDET_Msk     (1ul << USBD_FLDET_FLDET_Pos)

/* USBD STBUFSEG Bit Field Definitions */
#define USBD_STBUFSEG_STBUFSEG_Pos   3
#define USBD_STBUFSEG_STBUFSEG_Msk   (0x3Ful << USBD_STBUFSEG_STBUFSEG_Pos)

/* USBD BUFSEG Bit Field Definitions */
#define USBD_BUFSEG_BUFSEG_Pos   3
#define USBD_BUFSEG_BUFSEG_Msk   (0x3Ful << USBD_BUFSEG_BUFSEG_Pos)

/* USBD MXPLD Bit Field Definitions */
#define USBD_MXPLD_MXPLD_Pos    0
#define USBD_MXPLD_MXPLD_Msk    (0x1FFul << USBD_MXPLD_MXPLD_Pos)

/* USBD CFG Bit Field Definitions */
#define USBD_CFG_CSTALL_Pos     9
#define USBD_CFG_CSTALL_Msk     (1ul << USBD_CFG_CSTALL_Pos)

#define USBD_CFG_DSQ_SYNC_Pos   7
#define USBD_CFG_DSQ_SYNC_Msk   (1ul << USBD_CFG_DSQ_SYNC_Pos)

#define USBD_CFG_STATE_Pos      5
#define USBD_CFG_STATE_Msk      (3ul << USBD_CFG_STATE_Pos)

#define USBD_CFG_ISOCH_Pos      4
#define USBD_CFG_ISOCH_Msk      (1ul << USBD_CFG_ISOCH_Pos)

#define USBD_CFG_EP_NUM_Pos     0
#define USBD_CFG_EP_NUM_Msk     (0xFul << USBD_CFG_EP_NUM_Pos)

/* USBD CFGP Bit Field Definitions */
#define USBD_CFGP_SSTALL_Pos    1
#define USBD_CFGP_SSTALL_Msk    (1ul << USBD_CFGP_SSTALL_Pos)

#define USBD_CFGP_CLRRDY_Pos    0
#define USBD_CFGP_CLRRDY_Msk    (1ul << USBD_CFGP_CLRRDY_Pos)

/* USBD DRVSE0 Bit Field Definitions */
#define USBD_DRVSE0_DRVSE0_Pos   0
#define USBD_DRVSE0_DRVSE0_Msk   (1ul << USBD_DRVSE0_DRVSE0_Pos)


/*------------------------------ PDMA Controller -----------------------------*/
typedef struct
{
    __IO uint32_t  PDMACEN:1;
    __IO uint32_t  SW_RST:1;
    __IO uint32_t  MODE_SEL:2;
    __IO uint32_t  SAD_SEL:2;
    __IO uint32_t  DAD_SEL:2;
    __I  uint32_t  RESERVE0:4;
    __IO uint32_t  WAR_BCR_SEL:4;
    __I  uint32_t  RESERVE1:3;
    __IO uint32_t  APB_TWS:2;
    __I  uint32_t  RESERVE2:2;
    __IO uint32_t  TRIG_EN:1;
    __I  uint32_t  RESERVE3:8;
} PDMA_CSR_T;

typedef __IO uint32_t PDMA_SAR_T;
typedef __IO uint32_t PDMA_DAR_T;

typedef __IO uint32_t PDMA_BCR_T;

typedef __I uint32_t PDMA_CSAR_T;
typedef __I uint32_t PDMA_CDAR_T;

typedef struct
{
    __I uint32_t  CBCR:16;
    __I  uint32_t  RESERVE:16;
} PDMA_CBCR_T;

typedef struct
{
    __IO uint32_t  TABORT_IE:1;
    __IO uint32_t  BLKD_IE:1;
    __I  uint32_t  RESERVE:30;
} PDMA_IER_T;

typedef struct
{
    __IO uint32_t  TABORT_IF:1;
    __IO uint32_t  BLKD_IF:1;
    __I  uint32_t  RESERVE:30;
} PDMA_ISR_T;

typedef __I uint32_t PDMA_SBUF_T;

typedef struct
{
    __I  uint32_t  RESERVE0:8;
    __IO uint32_t  CLK0_EN:1;
    __IO uint32_t  CLK1_EN:1;
    __IO uint32_t  CLK2_EN:1;
    __IO uint32_t  CLK3_EN:1;
    __IO uint32_t  CLK4_EN:1;
    __IO uint32_t  CLK5_EN:1;
    __IO uint32_t  CLK6_EN:1;
    __IO uint32_t  CLK7_EN:1;
    __IO uint32_t  CLK8_EN:1;
    __I  uint32_t  RESERVE1:15;
} PDMA_GCRCSR_T;


typedef struct
{
    __IO uint32_t  SPI0_RXSEL:4;
    __IO uint32_t  SPI0_TXSEL:4;
    __IO uint32_t  SPI1_RXSEL:4;
    __IO uint32_t  SPI1_TXSEL:4;
    __IO uint32_t  SPI2_RXSEL:4;
    __IO uint32_t  SPI2_TXSEL:4;
    __IO uint32_t  SPI3_RXSEL:4;
    __IO uint32_t  SPI3_TXSEL:4;
} PDMA_PDSSR0_T;


typedef struct
{
    __IO uint32_t  UART0_RXSEL:4;
    __IO uint32_t  UART0_TXSEL:4;
    __IO uint32_t  UART1_RXSEL:4;
    __IO uint32_t  UART1_TXSEL:4;
    __IO uint32_t  RESERVE0:8;
    __IO uint32_t  ADC_RXSEL:4;
    __IO uint32_t  RESERVE1:4;
} PDMA_PDSSR1_T;
                                                        

typedef struct
{
    __I uint32_t  INTR0:1;
    __I uint32_t  INTR1:1;
    __I uint32_t  INTR2:1;
    __I uint32_t  INTR3:1;
    __I uint32_t  INTR4:1;
    __I uint32_t  INTR5:1;
    __I uint32_t  INTR6:1;
    __I uint32_t  INTR7:1;
    __I uint32_t  INTR8:1;
    __I uint32_t  RESERVE:22;
    __I uint32_t  INTR:1;
} PDMA_GCRISR_T;


typedef struct
{
    __IO uint32_t  I2S_RXSEL:4;
    __IO uint32_t  I2S_TXSEL:4;
    __I  uint32_t  RESERVE:24;
} PDMA_PDSSR2_T;

typedef struct
{
    union 
    {
        __IO uint32_t u32CSR;
        
        struct 
        {
            __IO uint32_t  PDMACEN:1;
            __IO uint32_t  SW_RST:1;
            __IO uint32_t  MODE_SEL:2;
            __IO uint32_t  SAD_SEL:2;
            __IO uint32_t  DAD_SEL:2;
            __I  uint32_t  RESERVE0:11;
            __IO uint32_t  APB_TWS:2;
            __I  uint32_t  RESERVE1:2;
            __IO uint32_t  TRIG_EN:1;
            __I  uint32_t  RESERVE2:8;
        } CSR;
    };

    union 
    {
        __IO uint32_t u32SAR;
        __IO uint32_t SAR;
    };

    union 
    {
        __IO uint32_t u32DAR;
        __IO uint32_t DAR;
    };

    union 
    {
        __IO uint32_t u32BCR;
        
        struct 
        {
            __IO uint32_t  BCR:16;
            __I  uint32_t  RESERVE0:16;
        } BCR;
    };

    union 
    {
        __I uint32_t u32POINT;
        
        struct 
        {
            __I  uint32_t  POINT:4;
            __I  uint32_t  RESERVE0:28;
        } POINT;
    };

    union 
    {
        __I uint32_t u32CSAR;
        __I uint32_t CSAR;
    };

    union 
    {
        __I uint32_t u32CDAR;
        __I uint32_t CDAR;
    };

    union 
    {
        __I uint32_t u32CBCR;
        
        struct 
        {
            __I uint32_t  CBCR:16;
            __I  uint32_t  RESERVE0:16;
        } CBCR;
    };

    union 
    {
        __IO uint32_t u32IER;
        
        struct 
        {
            __IO uint32_t  TABORT_IE:1;
            __IO uint32_t  BLKD_IE:1;
            __I  uint32_t  RESERVE:30;
        } IER;
    };

    union 
    {
        __IO uint32_t u32ISR;
        
        struct 
        {
            __IO uint32_t  TABORT_IF:1;
            __IO uint32_t  BLKD_IF:1;
            __I  uint32_t  RESERVE:30;
        } ISR;
    };

    __I uint32_t RESERVE[22];

    union 
    {
        __I uint32_t u32SBUF;
        __I uint32_t SBUF;
    };
} PDMA_T;

typedef struct
{
    union 
    {
        __IO uint32_t u32GCRCSR;
        
        struct 
        {
            __I  uint32_t  RESERVE0:8;
            __IO uint32_t  CLK0_EN:1;
            __IO uint32_t  CLK1_EN:1;
            __IO uint32_t  CLK2_EN:1;
            __IO uint32_t  CLK3_EN:1;
            __IO uint32_t  CLK4_EN:1;
            __IO uint32_t  CLK5_EN:1;
            __IO uint32_t  CLK6_EN:1;
            __IO uint32_t  CLK7_EN:1;
            __IO uint32_t  CLK8_EN:1;
            __I  uint32_t  RESERVE1:15;
        } GCRCSR;
    };

    union 
    {
        __IO uint32_t u32PDSSR0;
        
        struct 
        {
            __IO uint32_t  SPI0_RXSEL:4;
            __IO uint32_t  SPI0_TXSEL:4;
            __IO uint32_t  SPI1_RXSEL:4;
            __IO uint32_t  SPI1_TXSEL:4;
            __IO uint32_t  SPI2_RXSEL:4;
            __IO uint32_t  SPI2_TXSEL:4;
            __IO uint32_t  SPI3_RXSEL:4;
            __IO uint32_t  SPI3_TXSEL:4;
        } PDSSR0;
    };

    union 
    {
        __IO uint32_t u32PDSSR1;
        
        struct 
        {
            __IO uint32_t  UART0_RXSEL:4;
            __IO uint32_t  UART0_TXSEL:4;
            __IO uint32_t  UART1_RXSEL:4;
            __IO uint32_t  UART1_TXSEL:4;
            __IO uint32_t  RESERVE0:8;
            __IO uint32_t  ADC_RXSEL:4;
            __IO uint32_t  RESERVE1:4;
        } PDSSR1;
    };

    union 
    {
        __IO uint32_t u32GCRISR;
        
        struct 
        {
            __I uint32_t  INTR0:1;
            __I uint32_t  INTR1:1;
            __I uint32_t  INTR2:1;
            __I uint32_t  INTR3:1;
            __I uint32_t  INTR4:1;
            __I uint32_t  INTR5:1;
            __I uint32_t  INTR6:1;
            __I uint32_t  INTR7:1;
            __I uint32_t  INTR8:1;
            __I uint32_t  RESERVE:22;
            __I uint32_t  INTR:1;
        } GCRISR;
    };

    union 
    {
        __IO uint32_t u32PDSSR2;
        
        struct 
        {
            __IO uint32_t  I2S_RXSEL:4;
            __IO uint32_t  I2S_TXSEL:4;
            __I  uint32_t  RESERVE:24;
        } PDSSR2;
    };
} PDMA_GCR_T;

/* PDMA CSR Bit Field Definitions */
#define PDMA_CSR_PDMACEN_Pos                        0
#define PDMA_CSR_PDMACEN_Msk                        (1ul << PDMA_CSR_PDMACEN_Pos)

#define PDMA_CSR_SW_RST_Pos                         1
#define PDMA_CSR_SW_RST_Msk                         (1ul << PDMA_CSR_SW_RST_Pos)

#define PDMA_CSR_MODE_SEL_Pos                       2
#define PDMA_CSR_MODE_SEL_Msk                       (3ul << PDMA_CSR_MODE_SEL_Pos)

#define PDMA_CSR_SAD_SEL_Pos                        4
#define PDMA_CSR_SAD_SEL_Msk                        (3ul << PDMA_CSR_SAD_SEL_Pos)

#define PDMA_CSR_DAD_SEL_Pos                        6
#define PDMA_CSR_DAD_SEL_Msk                        (3ul << PDMA_CSR_DAD_SEL_Pos)

#define PDMA_CSR_APB_TWS_Pos                        19
#define PDMA_CSR_APB_TWS_Msk                        (3ul << PDMA_CSR_APB_TWS_Pos)

#define PDMA_CSR_TRIG_EN_Pos                        23
#define PDMA_CSR_TRIG_EN_Msk                        (1ul << PDMA_CSR_TRIG_EN_Pos)

/* PDMA BCR Bit Field Definitions */
#define PDMA_BCR_BCR_Pos                            0
#define PDMA_BCR_BCR_Msk                            (0xFFFFul << PDMA_BCR_BCR_Pos)

/* PDMA POINT Bit Field Definitions */
#define PDMA_POINT_POINT_Pos                        0
#define PDMA_POINT_POINT_Msk                        (0xFul << PDMA_POINT_POINT_Pos)

/* PDMA CBCR Bit Field Definitions */
#define PDMA_CBCR_CBCR_Pos                          0
#define PDMA_CBCR_CBCR_Msk                          (0xFFFFul << PDMA_CBCR_CBCR_Pos)


/* PDMA IER Bit Field Definitions */
#define PDMA_IER_TABORT_IE_Pos                      0
#define PDMA_IER_TABORT_IE_Msk                      (1ul << PDMA_IER_TABORT_IE_Pos)

#define PDMA_IER_BLKD_IE_Pos                        1
#define PDMA_IER_BLKD_IE_Msk                        (1ul << PDMA_IER_BLKD_IE_Pos)

/* PDMA ISR Bit Field Definitions */
#define PDMA_ISR_TABORT_IF_Pos                      0
#define PDMA_ISR_TABORT_IF_Msk                      (1ul << PDMA_ISR_TABORT_IF_Pos)

#define PDMA_ISR_BLKD_IF_Pos                        1
#define PDMA_ISR_BLKD_IF_Msk                        (1ul << PDMA_ISR_BLKD_IF_Pos)

/* PDMA GCRCSR Bit Field Definitions */
#define PDMA_GCRCSR_CLK0_EN_Pos                     8
#define PDMA_GCRCSR_CLK0_EN_Msk                     (1ul << PDMA_GCRCSR_CLK0_EN_Pos)

#define PDMA_GCRCSR_CLK1_EN_Pos                     9
#define PDMA_GCRCSR_CLK1_EN_Msk                     (1ul << PDMA_GCRCSR_CLK1_EN_Pos)

#define PDMA_GCRCSR_CLK2_EN_Pos                     10
#define PDMA_GCRCSR_CLK2_EN_Msk                     (1ul << PDMA_GCRCSR_CLK2_EN_Pos)

#define PDMA_GCRCSR_CLK3_EN_Pos                     11
#define PDMA_GCRCSR_CLK3_EN_Msk                     (1ul << PDMA_GCRCSR_CLK3_EN_Pos)

#define PDMA_GCRCSR_CLK4_EN_Pos                     12
#define PDMA_GCRCSR_CLK4_EN_Msk                     (1ul << PDMA_GCRCSR_CLK4_EN_Pos)

#define PDMA_GCRCSR_CLK5_EN_Pos                     13
#define PDMA_GCRCSR_CLK5_EN_Msk                     (1ul << PDMA_GCRCSR_CLK5_EN_Pos)

#define PDMA_GCRCSR_CLK6_EN_Pos                     14
#define PDMA_GCRCSR_CLK6_EN_Msk                     (1ul << PDMA_GCRCSR_CLK6_EN_Pos)

#define PDMA_GCRCSR_CLK7_EN_Pos                     15
#define PDMA_GCRCSR_CLK7_EN_Msk                     (1ul << PDMA_GCRCSR_CLK7_EN_Pos)

#define PDMA_GCRCSR_CLK8_EN_Pos                     16
#define PDMA_GCRCSR_CLK8_EN_Msk                     (1ul << PDMA_GCRCSR_CLK8_EN_Pos)

/* PDMA PDSSR0 Bit Field Definitions */
#define PDMA_PDSSR0_SPI0_RXSEL_Pos                  0
#define PDMA_PDSSR0_SPI0_RXSEL_Msk                  (0xFul << PDMA_PDSSR0_SPI0_RXSEL_Pos)

#define PDMA_PDSSR0_SPI0_TXSEL_Pos                  4
#define PDMA_PDSSR0_SPI0_TXSEL_Msk                  (0xFul << PDMA_PDSSR0_SPI0_TXSEL_Pos)

#define PDMA_PDSSR0_SPI1_RXSEL_Pos                  8
#define PDMA_PDSSR0_SPI1_RXSEL_Msk                  (0xFul << PDMA_PDSSR0_SPI1_RXSEL_Pos)

#define PDMA_PDSSR0_SPI1_TXSEL_Pos                  12
#define PDMA_PDSSR0_SPI1_TXSEL_Msk                  (0xFul << PDMA_PDSSR0_SPI1_TXSEL_Pos)

#define PDMA_PDSSR0_SPI2_RXSEL_Pos                  16
#define PDMA_PDSSR0_SPI2_RXSEL_Msk                  (0xFul << PDMA_PDSSR0_SPI2_RXSEL_Pos)

#define PDMA_PDSSR0_SPI2_TXSEL_Pos                  20
#define PDMA_PDSSR0_SPI2_TXSEL_Msk                  (0xFul << PDMA_PDSSR0_SPI2_TXSEL_Pos)

#define PDMA_PDSSR0_SPI3_RXSEL_Pos                  24
#define PDMA_PDSSR0_SPI3_RXSEL_Msk                  (0xFul << PDMA_PDSSR0_SPI3_RXSEL_Pos)

#define PDMA_PDSSR0_SPI3_TXSEL_Pos                  28
#define PDMA_PDSSR0_SPI3_TXSEL_Msk                  (0xFul << PDMA_PDSSR0_SPI3_TXSEL_Pos)

/* PDMA PDSSR1 Bit Field Definitions */
#define PDMA_PDSSR1_UART0_RXSEL_Pos                 0
#define PDMA_PDSSR1_UART0_RXSEL_Msk                 (0xFul << PDMA_PDSSR1_UART0_RXSEL_Pos)

#define PDMA_PDSSR1_UART0_TXSEL_Pos                 4
#define PDMA_PDSSR1_UART0_TXSEL_Msk                 (0xFul << PDMA_PDSSR1_UART0_TXSEL_Pos)

#define PDMA_PDSSR1_UART1_RXSEL_Pos                 8
#define PDMA_PDSSR1_UART1_RXSEL_Msk                 (0xFul << PDMA_PDSSR1_UART1_RXSEL_Pos)

#define PDMA_PDSSR1_UART1_TXSEL_Pos                 12
#define PDMA_PDSSR1_UART1_TXSEL_Msk                 (0xFul << PDMA_PDSSR1_UART1_TXSEL_Pos)

#define PDMA_PDSSR1_ADC_RXSEL_Pos                   24
#define PDMA_PDSSR1_ADC_RXSEL_Msk                   (0xFul << PDMA_PDSSR1_ADC_RXSEL_Pos)

/* PDMA GCRISR Bit Field Definitions */
#define PDMA_GCRISR_INTR0_Pos                       0
#define PDMA_GCRISR_INTR0_Msk                       (1ul << PDMA_GCRISR_INTR0_Pos)

#define PDMA_GCRISR_INTR1_Pos                       1
#define PDMA_GCRISR_INTR1_Msk                       (1ul << PDMA_GCRISR_INTR1_Pos)

#define PDMA_GCRISR_INTR2_Pos                       2
#define PDMA_GCRISR_INTR2_Msk                       (1ul << PDMA_GCRISR_INTR2_Pos)

#define PDMA_GCRISR_INTR3_Pos                       3
#define PDMA_GCRISR_INTR3_Msk                       (1ul << PDMA_GCRISR_INTR3_Pos)

#define PDMA_GCRISR_INTR4_Pos                       4
#define PDMA_GCRISR_INTR4_Msk                       (1ul << PDMA_GCRISR_INTR4_Pos)

#define PDMA_GCRISR_INTR5_Pos                       5
#define PDMA_GCRISR_INTR5_Msk                       (1ul << PDMA_GCRISR_INTR5_Pos)

#define PDMA_GCRISR_INTR6_Pos                       6
#define PDMA_GCRISR_INTR6_Msk                       (1ul << PDMA_GCRISR_INTR6_Pos)

#define PDMA_GCRISR_INTR7_Pos                       7
#define PDMA_GCRISR_INTR7_Msk                       (1ul << PDMA_GCRISR_INTR7_Pos)

#define PDMA_GCRISR_INTR8_Pos                       8
#define PDMA_GCRISR_INTR8_Msk                       (1ul << PDMA_GCRISR_INTR8_Pos)

#define PDMA_GCRISR_INTR_Pos                        31
#define PDMA_GCRISR_INTR_Msk                        (1ul << PDMA_GCRISR_INTR_Pos)

/* PDMA PDSSR2 Bit Field Definitions */
#define PDMA_PDSSR2_I2S_RXSEL_Pos                   0
#define PDMA_PDSSR2_I2S_RXSEL_Msk                   (0xFul << PDMA_PDSSR2_I2S_RXSEL_Pos)

#define PDMA_PDSSR2_I2S_TXSEL_Pos                   4
#define PDMA_PDSSR2_I2S_TXSEL_Msk                   (0xFul << PDMA_PDSSR2_I2S_TXSEL_Pos)

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

typedef __I  uint32_t PWM_PDR_T;

typedef struct
{
    __IO uint32_t  BCn:1;
    __I  uint32_t  RESERVE:31;
} PWM_PBCR_T;

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
    union 
    {
        __IO uint32_t u32PPR;
        
        struct 
        {
            __IO uint32_t  CP01:8;
            __IO uint32_t  CP23:8;
            __IO uint32_t  DZI01:8;
            __IO uint32_t  DZI23:8;
        } PPR;
    };
    
    union 
    {
        __IO uint32_t u32CSR;
        
        struct 
        {
            __IO uint32_t  CSR0:3;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  CSR1:3;
            __I  uint32_t  RESERVE1:1;
            __IO uint32_t  CSR2:3;
            __I  uint32_t  RESERVE2:1;
            __IO uint32_t  CSR3:3;
            __I  uint32_t  RESERVE3:17;
        } CSR;
    };

    union 
    {
        __IO uint32_t u32PCR;
        
        struct 
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
        } PCR;
    };

    union 
    {
        __IO uint32_t u32CNR0;
        __IO uint32_t CNR0;
    };

    union 
    {
        __IO uint32_t u32CMR0;
        __IO uint32_t CMR0;
    };

    union 
    {
        __I  uint32_t u32PDR0;
        __I  uint32_t PDR0;
    };
    
    union 
    {
        __IO uint32_t u32CNR1;
        __IO uint32_t CNR1;
    };

    union 
    {
        __IO uint32_t u32CMR1;
        __IO uint32_t CMR1;
    };

    union 
    {
        __I  uint32_t u32PDR1;
        __I  uint32_t PDR1;
    };

    union 
    {
        __IO uint32_t u32CNR2;
        __IO uint32_t CNR2;
    };

    union 
    {
        __IO uint32_t u32CMR2;
        __IO uint32_t CMR2;
    };

    union 
    {
        __I  uint32_t u32PDR2;
        __I  uint32_t PDR2;
    };

    union 
    {
        __IO uint32_t u32CNR3;
        __IO uint32_t CNR3;
    };

    union 
    {
        __IO uint32_t u32CMR3;
        __IO uint32_t CMR3;
    };

    union 
    {
        __I  uint32_t u32PDR3;
        __I  uint32_t PDR3;
    };

    union 
    {
        __IO uint32_t u32PBCR;
        
        struct 
        {
            __IO uint32_t  BCn:1;
            __I  uint32_t  RESERVE:31;
        } PBCR;
    };

    union 
    {
        __IO uint32_t u32PIER;
        
        struct 
        {
            __IO uint32_t  PWMIE0:1;
            __IO uint32_t  PWMIE1:1;
            __IO uint32_t  PWMIE2:1;
            __IO uint32_t  PWMIE3:1;
            __I  uint32_t  RESERVE:28;
        } PIER;
    };
    
    union 
    {
        __IO uint32_t u32PIIR;
        
        struct 
        {
            __IO uint32_t  PWMIF0:1;
            __IO uint32_t  PWMIF1:1;
            __IO uint32_t  PWMIF2:1;
            __IO uint32_t  PWMIF3:1;
            __I  uint32_t  RESERVE:28;
        } PIIR;
    };    
    
    uint32_t    RESERVE1[2];

    union 
    {
        __IO uint32_t u32CCR0;
        
        struct 
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
        } CCR0;
    };
    
    union 
    {
        __IO uint32_t u32CCR2;
        
        struct 
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
        } CCR2;
    };    

    union 
    {
        __IO uint32_t u32CRLR0;
        __IO uint32_t CRLR0;
    };

    union 
    {
        __IO uint32_t u32CFLR0;
        __IO uint32_t CFLR0;
    };

    union 
    {
        __IO uint32_t u32CRLR1;
        __IO uint32_t CRLR1;
    };

    union 
    {
        __IO uint32_t u32CFLR1;
        __IO uint32_t CFLR1;
    };

    union 
    {
        __IO uint32_t u32CRLR2;
        __IO uint32_t CRLR2;
    };

    union 
    {
        __IO uint32_t u32CFLR2;
        __IO uint32_t CFLR2;
    };
    
    union 
    {
        __IO uint32_t u32CRLR3;
        __IO uint32_t CRLR3;
    };

    union 
    {
        __IO uint32_t u32CFLR3;
        __IO uint32_t CFLR3;
    };    

    union 
    {
        __IO uint32_t u32CAPENR;
        __IO uint32_t CAPENR;
    };

    union 
    {
        __IO uint32_t u32POE;
        
        struct 
        {
            __IO uint32_t  PWM0:1;
            __IO uint32_t  PWM1:1;
            __IO uint32_t  PWM2:1;
            __IO uint32_t  PWM3:1;
            __I  uint32_t  RESERVE:28;
        } POE;
    }; 
} PWM_T;

/* PWM PPR Bit Field Definitions */
#define PWM_PPR_DZI23_Pos                       24
#define PWM_PPR_DZI23_Msk                       (0xFFul << PWM_PPR_DZI23_Pos)

#define PWM_PPR_DZI01_Pos                       16
#define PWM_PPR_DZI01_Msk                       (0xFFul << PWM_PPR_DZI01_Pos)

#define PWM_PPR_CP23_Pos                        8
#define PWM_PPR_CP23_Msk                        (0xFFul << PWM_PPR_CP23_Pos)

#define PWM_PPR_CP01_Pos                        0
#define PWM_PPR_CP01_Msk                        (0xFFul << PWM_PPR_CP01_Pos)

/* PWM CSR Bit Field Definitions */
#define PWM_CSR_CSR3_Pos                        12
#define PWM_CSR_CSR3_Msk                        (7ul << PWM_CSR_CSR3_Pos)

#define PWM_CSR_CSR2_Pos                        8
#define PWM_CSR_CSR2_Msk                        (7ul << PWM_CSR_CSR2_Pos)

#define PWM_CSR_CSR1_Pos                        4
#define PWM_CSR_CSR1_Msk                        (7ul << PWM_CSR_CSR1_Pos)

#define PWM_CSR_CSR0_Pos                        0
#define PWM_CSR_CSR0_Msk                        (7ul << PWM_CSR_CSR0_Pos)

/* PWM PCR Bit Field Definitions */
#define PWM_PCR_CH3MOD_Pos                      27
#define PWM_PCR_CH3MOD_Msk                      (1ul << PWM_PCR_CH3MOD_Pos)

#define PWM_PCR_CH3INV_Pos                      26
#define PWM_PCR_CH3INV_Msk                      (1ul << PWM_PCR_CH3INV_Pos)

#define PWM_PCR_CH3EN_Pos                       24
#define PWM_PCR_CH3EN_Msk                       (1ul << PWM_PCR_CH3EN_Pos)

#define PWM_PCR_CH2MOD_Pos                      19
#define PWM_PCR_CH2MOD_Msk                      (1ul << PWM_PCR_CH2MOD_Pos)

#define PWM_PCR_CH2INV_Pos                      18
#define PWM_PCR_CH2INV_Msk                      (1ul << PWM_PCR_CH2INV_Pos)

#define PWM_PCR_CH2EN_Pos                       16
#define PWM_PCR_CH2EN_Msk                       (1ul << PWM_PCR_CH2EN_Pos)

#define PWM_PCR_CH1MOD_Pos                      11
#define PWM_PCR_CH1MOD_Msk                      (1ul << PWM_PCR_CH1MOD_Pos)

#define PWM_PCR_CH1INV_Pos                      10
#define PWM_PCR_CH1INV_Msk                      (1ul << PWM_PCR_CH1INV_Pos)

#define PWM_PCR_CH1EN_Pos                       8
#define PWM_PCR_CH1EN_Msk                       (1ul << PWM_PCR_CH1EN_Pos)

#define PWM_PCR_DZEN23_Pos                      5
#define PWM_PCR_DZEN23_Msk                      (1ul << PWM_PCR_DZEN23_Pos)

#define PWM_PCR_DZEN01_Pos                      4
#define PWM_PCR_DZEN01_Msk                      (1ul << PWM_PCR_DZEN01_Pos)

#define PWM_PCR_CH0MOD_Pos                      3
#define PWM_PCR_CH0MOD_Msk                      (1ul << PWM_PCR_CH0MOD_Pos)

#define PWM_PCR_CH0INV_Pos                      2
#define PWM_PCR_CH0INV_Msk                      (1ul << PWM_PCR_CH0INV_Pos)

#define PWM_PCR_CH0EN_Pos                       0
#define PWM_PCR_CH0EN_Msk                       (1ul << PWM_PCR_CH0EN_Pos)

/* PWM CNR Bit Field Definitions */
#define PWM_CNR_CNR_Pos                         0
#define PWM_CNR_CNR_Msk                         (0xFFFFul << PWM_CNR_CNR_Pos)

/* PWM CMR Bit Field Definitions */
#define PWM_CMR_CMR_Pos                         0
#define PWM_CMR_CMR_Msk                         (0xFFFFul << PWM_CMR_CMR_Pos)

/* PWM PDR Bit Field Definitions */
#define PWM_PDR_PDR_Pos                         0
#define PWM_PDR_PDR_Msk                         (0xFFFFul << PWM_PDR_PDR_Pos)

/* PWM PBCR Bit Field Definitions */
#define PWM_PBCR_BCn_Pos                        0
#define PWM_PBCR_BCn_Msk                        (1ul << PWM_PBCR_BCn_Pos)

/* PWM PIER Bit Field Definitions */
#define PWM_PIER_PWMIE3_Pos                     3
#define PWM_PIER_PWMIE3_Msk                     (1ul << PWM_PIER_PWMIE3_Pos)

#define PWM_PIER_PWMIE2_Pos                     2
#define PWM_PIER_PWMIE2_Msk                     (1ul << PWM_PIER_PWMIE2_Pos)

#define PWM_PIER_PWMIE1_Pos                     1
#define PWM_PIER_PWMIE1_Msk                     (1ul << PWM_PIER_PWMIE1_Pos)

#define PWM_PIER_PWMIE0_Pos                     0
#define PWM_PIER_PWMIE0_Msk                     (1ul << PWM_PIER_PWMIE0_Pos)

/* PWM PIIR Bit Field Definitions */
#define PWM_PIIR_PWMIF3_Pos                     3
#define PWM_PIIR_PWMIF3_Msk                     (1ul << PWM_PIIR_PWMIF3_Pos)

#define PWM_PIIR_PWMIF2_Pos                     2
#define PWM_PIIR_PWMIF2_Msk                     (1ul << PWM_PIIR_PWMIF2_Pos)

#define PWM_PIIR_PWMIF1_Pos                     1
#define PWM_PIIR_PWMIF1_Msk                     (1ul << PWM_PIIR_PWMIF1_Pos)

#define PWM_PIIR_PWMIF0_Pos                     0
#define PWM_PIIR_PWMIF0_Msk                     (1ul << PWM_PIIR_PWMIF0_Pos)

/* PWM CCR0 Bit Field Definitions */
#define PWM_CCR0_CFLRI1_Pos                     23
#define PWM_CCR0_CFLRI1_Msk                     (1ul << PWM_CCR0_CFLRI1_Pos)

#define PWM_CCR0_CRLRI1_Pos                     22
#define PWM_CCR0_CRLRI1_Msk                     (1ul << PWM_CCR0_CRLRI1_Pos)

#define PWM_CCR0_CAPIF1_Pos                     20
#define PWM_CCR0_CAPIF1_Msk                     (1ul << PWM_CCR0_CAPIF1_Pos)

#define PWM_CCR0_CAPCH1EN_Pos                   19
#define PWM_CCR0_CAPCH1EN_Msk                   (1ul << PWM_CCR0_CAPCH1EN_Pos)

#define PWM_CCR0_CFL_IE1_Pos                    18
#define PWM_CCR0_CFL_IE1_Msk                    (1ul << PWM_CCR0_CFL_IE1_Pos)

#define PWM_CCR0_CRL_IE1_Pos                    17
#define PWM_CCR0_CRL_IE1_Msk                    (1ul << PWM_CCR0_CRL_IE1_Pos)

#define PWM_CCR0_INV1_Pos                       16
#define PWM_CCR0_INV1_Msk                       (1ul << PWM_CCR0_INV1_Pos)

#define PWM_CCR0_CFLRI0_Pos                     7
#define PWM_CCR0_CFLRI0_Msk                     (1ul << PWM_CCR0_CFLRI0_Pos)

#define PWM_CCR0_CRLRI0_Pos                     6
#define PWM_CCR0_CRLRI0_Msk                     (1ul << PWM_CCR0_CRLRI0_Pos)

#define PWM_CCR0_CAPIF0_Pos                     4
#define PWM_CCR0_CAPIF0_Msk                     (1ul << PWM_CCR0_CAPIF0_Pos)

#define PWM_CCR0_CAPCH0EN_Pos                   3
#define PWM_CCR0_CAPCH0EN_Msk                   (1ul << PWM_CCR0_CAPCH0EN_Pos)

#define PWM_CCR0_CFL_IE0_Pos                    2
#define PWM_CCR0_CFL_IE0_Msk                    (1ul << PWM_CCR0_CFL_IE0_Pos)

#define PWM_CCR0_CRL_IE0_Pos                    1
#define PWM_CCR0_CRL_IE0_Msk                    (1ul << PWM_CCR0_CRL_IE0_Pos)

#define PWM_CCR0_INV0_Pos                       0
#define PWM_CCR0_INV0_Msk                       (1ul << PWM_CCR0_INV0_Pos)

/* PWM CCR2 Bit Field Definitions */
#define PWM_CCR2_CFLRI3_Pos                     23
#define PWM_CCR2_CFLRI3_Msk                     (1ul << PWM_CCR2_CFLRI3_Pos)

#define PWM_CCR2_CRLRI3_Pos                     22
#define PWM_CCR2_CRLRI3_Msk                     (1ul << PWM_CCR2_CRLRI3_Pos)

#define PWM_CCR2_CAPIF3_Pos                     20
#define PWM_CCR2_CAPIF3_Msk                     (1ul << PWM_CCR2_CAPIF3_Pos)

#define PWM_CCR2_CAPCH3EN_Pos                   19
#define PWM_CCR2_CAPCH3EN_Msk                   (1ul << PWM_CCR2_CAPCH3EN_Pos)

#define PWM_CCR2_CFL_IE3_Pos                    18
#define PWM_CCR2_CFL_IE3_Msk                    (1ul << PWM_CCR2_CFL_IE3_Pos)

#define PWM_CCR2_CRL_IE3_Pos                    17
#define PWM_CCR2_CRL_IE3_Msk                    (1ul << PWM_CCR2_CRL_IE3_Pos)

#define PWM_CCR2_INV3_Pos                       16
#define PWM_CCR2_INV3_Msk                       (1ul << PWM_CCR2_INV3_Pos)

#define PWM_CCR2_CFLRI2_Pos                     7
#define PWM_CCR2_CFLRI2_Msk                     (1ul << PWM_CCR2_CFLRI2_Pos)

#define PWM_CCR2_CRLRI2_Pos                     6
#define PWM_CCR2_CRLRI2_Msk                     (1ul << PWM_CCR2_CRLRI2_Pos)

#define PWM_CCR2_CAPIF2_Pos                     4
#define PWM_CCR2_CAPIF2_Msk                     (1ul << PWM_CCR2_CAPIF2_Pos)

#define PWM_CCR2_CAPCH2EN_Pos                   3
#define PWM_CCR2_CAPCH2EN_Msk                   (1ul << PWM_CCR2_CAPCH2EN_Pos)

#define PWM_CCR2_CFL_IE2_Pos                    2
#define PWM_CCR2_CFL_IE2_Msk                    (1ul << PWM_CCR2_CFL_IE2_Pos)

#define PWM_CCR2_CRL_IE2_Pos                    1
#define PWM_CCR2_CRL_IE2_Msk                    (1ul << PWM_CCR2_CRL_IE2_Pos)

#define PWM_CCR2_INV2_Pos                       0
#define PWM_CCR2_INV2_Msk                       (1ul << PWM_CCR2_INV2_Pos)

/* PWM CRLR Bit Field Definitions */
#define PWM_CRLR_CRLR_Pos                       0
#define PWM_CRLR_CRLR_Msk                       (0xFFFFul << PWM_CRLR_CRLR_Pos)

/* PWM CFLR Bit Field Definitions */
#define PWM_CFLR_CFLR_Pos                       0
#define PWM_CFLR_CFLR_Msk                       (0xFFFFul << PWM_CFLR_CFLR_Pos)

/* PWM CAPENR Bit Field Definitions */
#define PWM_CAPENR_CAPENR_Pos                   0
#define PWM_CAPENR_CAPENR_Msk                   (0xFul << PWM_CAPENR_CAPENR_Pos)

/* PWM POE Bit Field Definitions */
#define PWM_POE_PWM3_Pos                        3
#define PWM_POE_PWM3_Msk                        (1ul << PWM_POE_PWM3_Pos)

#define PWM_POE_PWM2_Pos                        2
#define PWM_POE_PWM2_Msk                        (1ul << PWM_POE_PWM2_Pos)

#define PWM_POE_PWM1_Pos                        1
#define PWM_POE_PWM1_Msk                        (1ul << PWM_POE_PWM1_Pos)

#define PWM_POE_PWM0_Pos                        0
#define PWM_POE_PWM0_Msk                        (1ul << PWM_POE_PWM0_Pos)

/*----------------------------- I2S Controller -------------------------------*/

typedef struct
{
    __IO uint32_t  I2SEN:1;
    __IO uint32_t  TXEN:1;
    __IO uint32_t  RXEN:1;
    __IO uint32_t  MUTE:1;
    __IO uint32_t  WORDWIDTH:2;
    __IO uint32_t  MONO:1;
    __IO uint32_t  FORMAT:1;
    __IO uint32_t  SLAVE:1;
    __IO uint32_t  TXTH:3;
    __IO uint32_t  RXTH:3;
    __IO uint32_t  MCLKEN:1;
    __IO uint32_t  RCHZCEN:1;
    __IO uint32_t  LCHZCEN:1;
    __IO uint32_t  CLR_TXFIFO:1;
    __IO uint32_t  CLR_RXFIFO:1;
    __IO uint32_t  TXDMA:1;
    __IO uint32_t  RXDMA:1; 
    __I  uint32_t  RESERVE:10;
} I2S_I2SCON_T;

typedef struct
{
    __IO uint32_t  MCLK_DIV:3;
    __I  uint32_t  RESERVE0:5;
    __IO uint32_t  BCLK_DIV:8;
    __I  uint32_t  RESERVE1:16;
} I2S_I2SCLKDIV_T;

typedef struct
{
    __IO uint32_t  RXUDFIE:1;
    __IO uint32_t  RXOVFIE:1;
    __IO uint32_t  RXTHIE:1;
    __I  uint32_t  RESERVE0:5;
    __IO uint32_t  TXUDFIE:1;
    __IO uint32_t  TXOVFIE:1;
    __IO uint32_t  TXTHIE:1;
    __IO uint32_t  RZCIE:1;
    __IO uint32_t  LZCIE:1;
    __I  uint32_t  RESERVE1:19;
} I2S_I2SIE_T;

typedef struct
{
    __I  uint32_t  I2SINT:1;
    __I  uint32_t  I2SRXINT:1;
    __I  uint32_t  I2STXINT:1;
    __I  uint32_t  RIGHT:1;
    __I  uint32_t  RESERVE0:4;
    __IO uint32_t  RXUDF:1;
    __IO uint32_t  RXOVF:1;
    __I  uint32_t  RXTHF:1;
    __I  uint32_t  RXFULL:1;
    __I  uint32_t  RXEMPTY:1;
    __I  uint32_t  RESERVE1:3;
    __IO uint32_t  TXUDF:1;
    __IO uint32_t  TXOVF:1;
    __I  uint32_t  TXTHF:1;
    __I  uint32_t  TXFULL:1;
    __I  uint32_t  TXEMPTY:1;
    __I  uint32_t  TXBUSY:1;
    __I  uint32_t  RZCF:1;
    __I  uint32_t  LZCF:1;  
    __I  uint32_t  RX_LEVEL:4;
    __I  uint32_t  TX_LEVEL:4;
} I2S_I2SSTATUS_T;

typedef __O uint32_t I2S_I2STXFIFO_T;

typedef __I uint32_t I2S_I2SRXFIFO_T;

typedef struct
{
    union 
    {
        __IO uint32_t u32I2SCON;
        
        struct 
        {
            __IO uint32_t  I2SEN:1;
            __IO uint32_t  TXEN:1;
            __IO uint32_t  RXEN:1;
            __IO uint32_t  MUTE:1;
            __IO uint32_t  WORDWIDTH:2;
            __IO uint32_t  MONO:1;
            __IO uint32_t  FORMAT:1;
            __IO uint32_t  SLAVE:1;
            __IO uint32_t  TXTH:3;
            __IO uint32_t  RXTH:3;
            __IO uint32_t  MCLKEN:1;
            __IO uint32_t  RCHZCEN:1;
            __IO uint32_t  LCHZCEN:1;
            __IO uint32_t  CLR_TXFIFO:1;
            __IO uint32_t  CLR_RXFIFO:1;
            __IO uint32_t  TXDMA:1;
            __IO uint32_t  RXDMA:1; 
            __I  uint32_t  RESERVE:10;
        } I2SCON;
    };

    union 
    {
        __IO uint32_t u32I2SCLKDIV;
        
        struct 
        {
            __IO uint32_t  MCLK_DIV:3;
            __I  uint32_t  RESERVE0:5;
            __IO uint32_t  BCLK_DIV:8;
            __I  uint32_t  RESERVE1:16;
        } I2SCLKDIV;
    };

    union 
    {
        __IO uint32_t u32I2SIE;
        
        struct 
        {
            __IO uint32_t  RXUDFIE:1;
            __IO uint32_t  RXOVFIE:1;
            __IO uint32_t  RXTHIE:1;
            __I  uint32_t  RESERVE0:5;
            __IO uint32_t  TXUDFIE:1;
            __IO uint32_t  TXOVFIE:1;
            __IO uint32_t  TXTHIE:1;
            __IO uint32_t  RZCIE:1;
            __IO uint32_t  LZCIE:1;
            __I  uint32_t  RESERVE1:19;
        } I2SIE;
    };

    union 
    {
        __I uint32_t u32I2SSTATUS;
        struct 
        {
            __I  uint32_t  I2SINT:1;
            __I  uint32_t  I2SRXINT:1;
            __I  uint32_t  I2STXINT:1;
            __I  uint32_t  RIGHT:1;
            __I  uint32_t  RESERVE0:4;
            __IO uint32_t  RXUDF:1;
            __IO uint32_t  RXOVF:1;
            __I  uint32_t  RXTHF:1;
            __I  uint32_t  RXFULL:1;
            __I  uint32_t  RXEMPTY:1;
            __I  uint32_t  RESERVE1:3;
            __IO uint32_t  TXUDF:1;
            __IO uint32_t  TXOVF:1;
            __I  uint32_t  TXTHF:1;
            __I  uint32_t  TXFULL:1;
            __I  uint32_t  TXEMPTY:1;
            __I  uint32_t  TXBUSY:1;
            __I  uint32_t  RZCF:1;
            __I  uint32_t  LZCF:1;  
            __I  uint32_t  RX_LEVEL:4;
            __I  uint32_t  TX_LEVEL:4;
        } I2SSTATUS;
    };
    
    union 
    {
        __O uint32_t u32I2STXFIFO;
        __O uint32_t I2STXFIFO;
    };

    union 
    {
        __I uint32_t u32I2SRXFIFO;
        __I uint32_t I2SRXFIFO;
    };
} I2S_T;


/* I2S I2SCON Bit Field Definitions */
#define I2S_I2SCON_RXDMA_Pos                    21
#define I2S_I2SCON_RXDMA_Msk                    (1ul << I2S_I2SCON_RXDMA_Pos)

#define I2S_I2SCON_TXDMA_Pos                    20
#define I2S_I2SCON_TXDMA_Msk                    (1ul << I2S_I2SCON_TXDMA_Pos)

#define I2S_I2SCON_CLR_RXFIFO_Pos               19
#define I2S_I2SCON_CLR_RXFIFO_Msk               (1ul << I2S_I2SCON_CLR_RXFIFO_Pos)

#define I2S_I2SCON_CLR_TXFIFO_Pos               18
#define I2S_I2SCON_CLR_TXFIFO_Msk               (1ul << I2S_I2SCON_CLR_TXFIFO_Pos)

#define I2S_I2SCON_LCHZCEN_Pos                  17
#define I2S_I2SCON_LCHZCEN_Msk                  (1ul << I2S_I2SCON_LCHZCEN_Pos)

#define I2S_I2SCON_RCHZCEN_Pos                  16
#define I2S_I2SCON_RCHZCEN_Msk                  (1ul << I2S_I2SCON_RCHZCEN_Pos)

#define I2S_I2SCON_MCLKEN_Pos                   15
#define I2S_I2SCON_MCLKEN_Msk                   (1ul << I2S_I2SCON_MCLKEN_Pos)

#define I2S_I2SCON_RXTH_Pos                     12
#define I2S_I2SCON_RXTH_Msk                     (7ul << I2S_I2SCON_RXTH_Pos)

#define I2S_I2SCON_TXTH_Pos                     9
#define I2S_I2SCON_TXTH_Msk                     (7ul << I2S_I2SCON_TXTH_Pos)

#define I2S_I2SCON_SLAVE_Pos                    8
#define I2S_I2SCON_SLAVE_Msk                    (1ul << I2S_I2SCON_SLAVE_Pos)

#define I2S_I2SCON_FORMAT_Pos                   7
#define I2S_I2SCON_FORMAT_Msk                   (1ul << I2S_I2SCON_FORMAT_Pos)

#define I2S_I2SCON_MONO_Pos                     6
#define I2S_I2SCON_MONO_Msk                     (1ul << I2S_I2SCON_MONO_Pos)

#define I2S_I2SCON_WORDWIDTH_Pos                4
#define I2S_I2SCON_WORDWIDTH_Msk                (3ul << I2S_I2SCON_WORDWIDTH_Pos)

#define I2S_I2SCON_MUTE_Pos                     3
#define I2S_I2SCON_MUTE_Msk                     (1ul << I2S_I2SCON_MUTE_Pos)

#define I2S_I2SCON_RXEN_Pos                     2
#define I2S_I2SCON_RXEN_Msk                     (1ul << I2S_I2SCON_RXEN_Pos)

#define I2S_I2SCON_TXEN_Pos                     1
#define I2S_I2SCON_TXEN_Msk                     (1ul << I2S_I2SCON_TXEN_Pos)

#define I2S_I2SCON_I2SEN_Pos                    0
#define I2S_I2SCON_I2SEN_Msk                    (1ul << I2S_I2SCON_I2SEN_Pos)

/* I2S I2SCLKDIV Bit Field Definitions */
#define I2S_I2SCLKDIV_BCLK_DIV_Pos              8
#define I2S_I2SCLKDIV_BCLK_DIV_Msk              (0xFFul << I2S_I2SCLKDIV_BCLK_DIV_Pos)

#define I2S_I2SCLKDIV_MCLK_DIV_Pos              0
#define I2S_I2SCLKDIV_MCLK_DIV_Msk              (7ul << I2S_I2SCLKDIV_MCLK_DIV_Pos)

/* I2S I2SIE Bit Field Definitions */
#define I2S_I2SIE_LZCIE_Pos                     12
#define I2S_I2SIE_LZCIE_Msk                     (1ul << I2S_I2SIE_LZCIE_Pos)

#define I2S_I2SIE_RZCIE_Pos                     11
#define I2S_I2SIE_RZCIE_Msk                     (1ul << I2S_I2SIE_RZCIE_Pos)

#define I2S_I2SIE_TXTHIE_Pos                    10
#define I2S_I2SIE_TXTHIE_Msk                    (1ul << I2S_I2SIE_TXTHIE_Pos)
                                        
#define I2S_I2SIE_TXOVFIE_Pos                   9
#define I2S_I2SIE_TXOVFIE_Msk                   (1ul << I2S_I2SIE_TXOVFIE_Pos)

#define I2S_I2SIE_TXUDFIE_Pos                   8
#define I2S_I2SIE_TXUDFIE_Msk                   (1ul << I2S_I2SIE_TXUDFIE_Pos)

#define I2S_I2SIE_RXTHIE_Pos                    2
#define I2S_I2SIE_RXTHIE_Msk                    (1ul << I2S_I2SIE_RXTHIE_Pos)
                                        
#define I2S_I2SIE_RXOVFIE_Pos                   1
#define I2S_I2SIE_RXOVFIE_Msk                   (1ul << I2S_I2SIE_RXOVFIE_Pos)

#define I2S_I2SIE_RXUDFIE_Pos                   0
#define I2S_I2SIE_RXUDFIE_Msk                   (1ul << I2S_I2SIE_RXUDFIE_Pos)


/* I2S I2SSTATUS Bit Field Definitions */
#define I2S_I2SSTATUS_TX_LEVEL_Pos              28
#define I2S_I2SSTATUS_TX_LEVEL_Msk              (0xFul << I2S_I2SSTATUS_TX_LEVEL_Pos)

#define I2S_I2SSTATUS_RX_LEVEL_Pos              24
#define I2S_I2SSTATUS_RX_LEVEL_Msk              (0xFul << I2S_I2SSTATUS_RX_LEVEL_Pos)

#define I2S_I2SSTATUS_LZCF_Pos                  23
#define I2S_I2SSTATUS_LZCF_Msk                  (1ul << I2S_I2SSTATUS_LZCF_Pos)

#define I2S_I2SSTATUS_RZCF_Pos                  22
#define I2S_I2SSTATUS_RZCF_Msk                  (1ul << I2S_I2SSTATUS_RZCF_Pos)

#define I2S_I2SSTATUS_TXBUSY_Pos                21
#define I2S_I2SSTATUS_TXBUSY_Msk                (1ul << I2S_I2SSTATUS_TXBUSY_Pos)

#define I2S_I2SSTATUS_TXEMPTY_Pos               20
#define I2S_I2SSTATUS_TXEMPTY_Msk               (1ul << I2S_I2SSTATUS_TXEMPTY_Pos)

#define I2S_I2SSTATUS_TXFULL_Pos                19
#define I2S_I2SSTATUS_TXFULL_Msk                (1ul << I2S_I2SSTATUS_TXFULL_Pos)

#define I2S_I2SSTATUS_TXTHF_Pos                 18
#define I2S_I2SSTATUS_TXTHF_Msk                 (1ul << I2S_I2SSTATUS_TXTHF_Pos)

#define I2S_I2SSTATUS_TXOVF_Pos                 17
#define I2S_I2SSTATUS_TXOVF_Msk                 (1ul << I2S_I2SSTATUS_TXOVF_Pos)

#define I2S_I2SSTATUS_TXUDF_Pos                 16
#define I2S_I2SSTATUS_TXUDF_Msk                 (1ul << I2S_I2SSTATUS_TXUDF_Pos)

#define I2S_I2SSTATUS_RXEMPTY_Pos               12
#define I2S_I2SSTATUS_RXEMPTY_Msk               (1ul << I2S_I2SSTATUS_RXEMPTY_Pos)

#define I2S_I2SSTATUS_RXFULL_Pos                11
#define I2S_I2SSTATUS_RXFULL_Msk                (1ul << I2S_I2SSTATUS_RXFULL_Pos)

#define I2S_I2SSTATUS_RXTHF_Pos                 10
#define I2S_I2SSTATUS_RXTHF_Msk                 (1ul << I2S_I2SSTATUS_RXTHF_Pos)

#define I2S_I2SSTATUS_RXOVF_Pos                 9
#define I2S_I2SSTATUS_RXOVF_Msk                 (1ul << I2S_I2SSTATUS_RXOVF_Pos)

#define I2S_I2SSTATUS_RXUDF_Pos                 8
#define I2S_I2SSTATUS_RXUDF_Msk                 (1ul << I2S_I2SSTATUS_RXUDF_Pos)

#define I2S_I2SSTATUS_RIGHT_Pos                 3
#define I2S_I2SSTATUS_RIGHT_Msk                 (1ul << I2S_I2SSTATUS_RIGHT_Pos)

#define I2S_I2SSTATUS_I2STXINT_Pos              2
#define I2S_I2SSTATUS_I2STXINT_Msk              (1ul << I2S_I2SSTATUS_I2STXINT_Pos)

#define I2S_I2SSTATUS_I2SRXINT_Pos              1
#define I2S_I2SSTATUS_I2SRXINT_Msk              (1ul << I2S_I2SSTATUS_I2SRXINT_Pos)

#define I2S_I2SSTATUS_I2SINT_Pos                0
#define I2S_I2SSTATUS_I2SINT_Msk                (1ul << I2S_I2SSTATUS_I2SINT_Pos)


/*----------------------------- EBI Controller -------------------------------*/
typedef struct
{
    __IO uint32_t  ExtEN:1;
    __IO uint32_t  ExtBW16:1;
    __I  uint32_t  RESERVE0:6;
    __IO uint32_t  MCLKDIV:3;
    __I  uint32_t  RESERVE1:5;
    __IO uint32_t  ExttALE:3;
    __I  uint32_t  RESERVE2:13;
} EBI_EBICON_T;

typedef struct
{
    __I  uint32_t  RESERVE0:3;
    __IO uint32_t  ExttACC:5;
    __IO uint32_t  ExttAHD:3;
    __I  uint32_t  RESERVE1:1;
    __IO uint32_t  ExtIW2X:4;
    __I  uint32_t  RESERVE2:8;
    __IO uint32_t  ExtIR2R:4;
    __I  uint32_t  RESERVE3:4;
} EBI_EXTIME_T;

typedef struct
{
    union {
        __IO uint32_t u32EBICON;
        struct {
            __IO uint32_t  ExtEN:1;
            __IO uint32_t  ExtBW16:1;
            __I  uint32_t  RESERVE0:6;
            __IO uint32_t  MCLKDIV:3;
            __I  uint32_t  RESERVE1:5;
            __IO uint32_t  ExttALE:3;
            __I  uint32_t  RESERVE2:13;
        } EBICON;
    };

    union {
        __IO uint32_t u32EXTIME;
        struct {
            __I  uint32_t  RESERVE0:3;
            __IO uint32_t  ExttACC:5;
            __IO uint32_t  ExttAHD:3;
            __I  uint32_t  RESERVE1:1;
            __IO uint32_t  ExtIW2X:4;
            __I  uint32_t  RESERVE2:8;
            __IO uint32_t  ExtIR2R:4;
            __I  uint32_t  RESERVE3:4;
        } EXTIME;
    };
} EBI_T;

/* EBI EBICON Bit Field Definitions */
#define EBI_EBICON_ExttALE_Pos      16
#define EBI_EBICON_ExttALE_Msk      (0x7ul << EBI_EBICON_ExttALE_Pos)

#define EBI_EBICON_MCLKDIV_Pos      8
#define EBI_EBICON_MCLKDIV_Msk      (0x7ul << EBI_EBICON_MCLKDIV_Pos)

#define EBI_EBICON_ExtBW16_Pos      1
#define EBI_EBICON_ExtBW16_Msk      (1ul << EBI_EBICON_ExtBW16_Pos)

#define EBI_EBICON_ExtEN_Pos        0
#define EBI_EBICON_ExtEN_Msk        (1ul << EBI_EBICON_ExtEN_Pos)

/* EBI EXTIME Bit Field Definitions */
#define EBI_EXTIME_ExtIR2R_Pos      24
#define EBI_EXTIME_ExtIR2R_Msk      (0xFul << EBI_EXTIME_ExtIR2R_Pos)

#define EBI_EXTIME_ExtIW2X_Pos      12
#define EBI_EXTIME_ExtIW2X_Msk      (0xFul << EBI_EXTIME_ExtIW2X_Pos)

#define EBI_EXTIME_ExttAHD_Pos      8
#define EBI_EXTIME_ExttAHD_Msk      (0x7ul << EBI_EXTIME_ExttAHD_Pos)

#define EBI_EXTIME_ExttACC_Pos      3
#define EBI_EXTIME_ExttACC_Msk      (0x1Ful << EBI_EXTIME_ExttACC_Pos)


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
#define GPIO_BASE           (AHB_BASE        + 0x4000)

#define GPIOA_BASE          (GPIO_BASE               )
#define GPIOB_BASE          (GPIO_BASE       + 0x0040)
#define GPIOC_BASE          (GPIO_BASE       + 0x0080)
#define GPIOD_BASE          (GPIO_BASE       + 0x00C0)
#define GPIOE_BASE          (GPIO_BASE       + 0x0100)
#define GPIO_DBNCECON_BASE  (GPIO_BASE       + 0x0180)

#define UART0_BASE           (APB1_BASE      + 0x50000)
#define UART1_BASE           (APB2_BASE      + 0x50000)
#define UART2_BASE           (APB2_BASE      + 0x54000)


#define TIMER0_BASE          (APB1_BASE      + 0x10000)  
#define TIMER1_BASE          (APB1_BASE      + 0x10020)  
#define TIMER2_BASE          (APB2_BASE      + 0x10000)  
#define TIMER3_BASE          (APB2_BASE      + 0x10020)  

#define WDT_BASE             (APB1_BASE      + 0x4000)  

#define SPI0_BASE            (APB1_BASE      + 0x30000)
#define SPI1_BASE            (APB1_BASE      + 0x34000)
#define SPI2_BASE            (APB2_BASE      + 0x30000)
#define SPI3_BASE            (APB2_BASE      + 0x34000)

#define I2C0_BASE            (APB1_BASE      + 0x20000)
#define I2C1_BASE            (APB2_BASE      + 0x20000)

#define RTC_BASE             (APB1_BASE      + 0x08000)

#define ADC_BASE             (APB1_BASE      + 0xE0000)

#define ACMP_BASE            (APB1_BASE      + 0xD0000)   

#define SYSCLK_BASE          (AHB_BASE       + 0x00200)

#define GCR_BASE             (AHB_BASE       + 0x00000)

#define INT_BASE             (AHB_BASE       + 0x00300)

#define FMC_BASE             (AHB_BASE       + 0x0C000)

#define PS2_BASE             (APB2_BASE      + 0x00000)

#define CAN0_BASE            (APB2_BASE      + 0x80000)
#define CAN1_BASE            (APB2_BASE      + 0x84000)
#define C_CAN_BASE           (APB2_BASE      + 0x88000)

#define USBD_BASE            (APB1_BASE      + 0x60000)

#define PDMA0_BASE           (AHB_BASE       + 0x08000)
#define PDMA1_BASE           (AHB_BASE       + 0x08100)
#define PDMA2_BASE           (AHB_BASE       + 0x08200)
#define PDMA3_BASE           (AHB_BASE       + 0x08300)
#define PDMA4_BASE           (AHB_BASE       + 0x08400)
#define PDMA5_BASE           (AHB_BASE       + 0x08500)
#define PDMA6_BASE           (AHB_BASE       + 0x08600)
#define PDMA7_BASE           (AHB_BASE       + 0x08700)
#define PDMA8_BASE           (AHB_BASE       + 0x08800)
#define PDMA9_BASE           (AHB_BASE       + 0x08900)
#define PDMA10_BASE          (AHB_BASE       + 0x08A00)
#define PDMA11_BASE          (AHB_BASE       + 0x08B00)
#define PDMA_GCR_BASE        (AHB_BASE       + 0x08F00)

#define PWMA_BASE            (APB1_BASE      + 0x40000)
#define PWMB_BASE            (APB2_BASE      + 0x40000)

#define I2S_BASE             (APB2_BASE      + 0xA0000)

#define EBI_BASE             (AHB_BASE       + 0x10000)

/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
#define GPIOA               ((GPIO_T *) GPIOA_BASE)
#define GPIOB               ((GPIO_T *) GPIOB_BASE)
#define GPIOC               ((GPIO_T *) GPIOC_BASE)
#define GPIOD               ((GPIO_T *) GPIOD_BASE)
#define GPIOE               ((GPIO_T *) GPIOE_BASE)
#define GPIO_DBNCECON       ((GPIO_DBNCECON_T *) GPIO_DBNCECON_BASE)
#define GPIO_BIT_ADDR_BASE  (GPIO_BASE + 0x200)

#define UART0               ((UART_T *) UART0_BASE)
#define UART1               ((UART_T *) UART1_BASE)
#define UART2               ((UART_T *) UART2_BASE)


#define TIMER0              ((TIMER_T *) TIMER0_BASE)
#define TIMER1              ((TIMER_T *) TIMER1_BASE)
#define TIMER2              ((TIMER_T *) TIMER2_BASE)
#define TIMER3              ((TIMER_T *) TIMER3_BASE)

#define WDT                 ((WDT_T *) WDT_BASE)

#define SPI0                ((SPI_T *) SPI0_BASE)
#define SPI1                ((SPI_T *) SPI1_BASE)
#define SPI2                ((SPI_T *) SPI2_BASE)
#define SPI3                ((SPI_T *) SPI3_BASE)

#define I2C0                ((I2C_T *) I2C0_BASE)
#define I2C1                ((I2C_T *) I2C1_BASE)

#define I2S                 ((I2S_T *) I2S_BASE)

#define RTC                 ((RTC_T *) RTC_BASE)

#define ADC                 ((ADC_T *) ADC_BASE)

#define ACMP                ((ACMP_T *) ACMP_BASE)

#define SYSCLK              ((SYSCLK_T *) SYSCLK_BASE)

#define SYS                 ((GCR_T *) GCR_BASE)

#define SYSINT              ((GCR_INT_T *) INT_BASE)

#define FMC                 ((FMC_T *) FMC_BASE)

#define PS2                 ((PS2_T *) PS2_BASE)

#define CAN                 ((CAN_T *) CAN0_BASE)
#define USBD                ((USBD_T *) USBD_BASE)

#define PDMA0               ((PDMA_T *) PDMA0_BASE)
#define PDMA1               ((PDMA_T *) PDMA1_BASE)
#define PDMA2               ((PDMA_T *) PDMA2_BASE)
#define PDMA3               ((PDMA_T *) PDMA3_BASE)
#define PDMA4               ((PDMA_T *) PDMA4_BASE)
#define PDMA5               ((PDMA_T *) PDMA5_BASE)
#define PDMA6               ((PDMA_T *) PDMA6_BASE)
#define PDMA7               ((PDMA_T *) PDMA7_BASE)
#define PDMA8               ((PDMA_T *) PDMA8_BASE)
#define PDMA9               ((PDMA_T *) PDMA9_BASE)
#define PDMA_GCR            ((PDMA_GCR_T *) PDMA_GCR_BASE)

#define PWMA                ((PWM_T *) PWMA_BASE)
#define PWMB                ((PWM_T *) PWMB_BASE)

#define EBI                 ((EBI_T *) EBI_BASE)

#define UNLOCKREG(x)        *((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x59;*((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x16;*((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x88
#define LOCKREG(x)          *((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x00;     

#define REGCOPY(dest, src)  *((uint32_t *)&(dest)) = *((uint32_t *)&(src))
#define CLEAR(dest)         *((uint32_t *)&(dest)) = 0

//=============================================================================
typedef volatile unsigned char  vu8;
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))

#define outpw(port,value)   *((volatile unsigned int *)(port))=value
#define inpw(port)          (*((volatile unsigned int *)(port)))
#define outpb(port,value)   *((volatile unsigned char *)(port))=value
#define inpb(port)          (*((volatile unsigned char *)(port)))
#define outps(port,value)   *((volatile unsigned short *)(port))=value
#define inps(port)          (*((volatile unsigned short *)(port)))

#define outp32(port,value)  *((volatile unsigned int *)(port))=value
#define inp32(port)         (*((volatile unsigned int *)(port)))
#define outp8(port,value)   *((volatile unsigned char *)(port))=value
#define inp8(port)          (*((volatile unsigned char *)(port)))
#define outp16(port,value)  *((volatile unsigned short *)(port))=value
#define inp16(port)         (*((volatile unsigned short *)(port)))


#define E_SUCCESS   0
#ifndef NULL
#define NULL        0
#endif

#define TRUE        1
#define FALSE       0

#define ENABLE     1
#define DISABLE    0

/* Define one bit mask */
#define BIT0    0x00000001
#define BIT1    0x00000002
#define BIT2    0x00000004
#define BIT3    0x00000008
#define BIT4    0x00000010
#define BIT5    0x00000020
#define BIT6    0x00000040
#define BIT7    0x00000080
#define BIT8    0x00000100
#define BIT9    0x00000200
#define BIT10   0x00000400
#define BIT11   0x00000800
#define BIT12   0x00001000
#define BIT13   0x00002000
#define BIT14   0x00004000
#define BIT15   0x00008000
#define BIT16   0x00010000
#define BIT17   0x00020000
#define BIT18   0x00040000
#define BIT19   0x00080000
#define BIT20   0x00100000
#define BIT21   0x00200000
#define BIT22   0x00400000
#define BIT23   0x00800000
#define BIT24   0x01000000
#define BIT25   0x02000000
#define BIT26   0x04000000
#define BIT27   0x08000000
#define BIT28   0x10000000
#define BIT29   0x20000000
#define BIT30   0x40000000
#define BIT31   0x80000000

#endif
                                                                                                 
