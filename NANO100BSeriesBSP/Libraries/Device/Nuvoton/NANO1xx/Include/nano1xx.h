/******************************************************************************
 * @file     nano1xx.h
 * @brief    CMSIS Cortex-M0 Core Peripheral Access Layer Header File for
 *           NANO1xx devices
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __NANO1xx_H    
#define __NANO1xx_H

#ifdef __cplusplus
 extern "C" {
#endif 


/** @addtogroup NANO1xx_Definitions NANO1xx Definitions
  This file defines all structures and symbols for NANO1xx:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - Peripheral definitions
  @{
*/


/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup NANO1xx_CMSIS Device CMSIS Definitions
  Configuration of the Cortex-M0 Processor and Core Peripherals
  @{
*/

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers **************************************************/
  NonMaskableInt_IRQn	= -14,    /*!< 2 Non Maskable Interrupt                                  */
  HardFault_IRQn		= -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                          */
  SVCall_IRQn			= -5,     /*!< 11 Cortex-M0 SV Call Interrupt                            */
  PendSV_IRQn			= -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                            */
  SysTick_IRQn			= -1,     /*!< 15 Cortex-M0 System Tick Interrupt                        */
/******  NANO1xx Specific Interrupt Numbers ******************************************************/
  BOD_IRQn              = 0,      /*!< Brownout low voltage detected interrupt                   */
  WDT_IRQn              = 1,      /*!< Watch Dog Timer interrupt                                 */
  EINT0_IRQn            = 2,      /*!< External signal interrupt from PB.14 pin                  */
  EINT1_IRQn            = 3,      /*!< External signal interrupt from PB.15 pin                  */
  GPABC_IRQn            = 4,      /*!< External signal interrupt from PA[15:0]/PB[13:0]/PC[15:0] */
  GPDEF_IRQn            = 5,      /*!< External interrupt from PD[15:0]/PE[15:0]/PF[15:0]        */
  PWM0_IRQn             = 6,      /*!< PWM 0 interrupt                                           */
  PWM1_IRQn             = 7,      /*!< PWM 1 interrupt                                           */
  TMR0_IRQn             = 8,      /*!< Timer 0 interrupt                                         */
  TMR1_IRQn             = 9,      /*!< Timer 1 interrupt                                         */
  TMR2_IRQn             = 10,     /*!< Timer 2 interrupt                                         */
  TMR3_IRQn             = 11,     /*!< Timer 3 interrupt                                         */
  UART0_IRQn            = 12,     /*!< UART0 interrupt                                           */
  UART1_IRQn            = 13,     /*!< UART1 interrupt                                           */
  SPI0_IRQn             = 14,     /*!< SPI0 interrupt                                            */
  SPI1_IRQn             = 15,     /*!< SPI1 interrupt                                            */
  SPI2_IRQn             = 16,     /*!< SPI2 interrupt                                            */
  HIRC_IRQn             = 17,     /*!< HIRC interrupt                                            */
  I2C0_IRQn             = 18,     /*!< I2C0 interrupt                                            */
  I2C1_IRQn             = 19,     /*!< I2C1 interrupt                                            */
  SC2_IRQn              = 20,     /*!< Smart Card 2 interrupt                                    */
  SC0_IRQn              = 21,     /*!< Smart Card 0 interrupt                                    */
  SC1_IRQn              = 22,     /*!< Smart Card 1 interrupt                                    */
  USBD_IRQn             = 23,     /*!< USB FS Device interrupt                                   */
  TK_IRQn               = 24,     /*!< Touch key interrupt                                       */
  LCD_IRQn              = 25,     /*!< LCD interrupt                                             */
  PDMA_IRQn             = 26,     /*!< PDMA interrupt                                            */
  I2S_IRQn              = 27,     /*!< I2S interrupt                                             */
  PDWU_IRQn             = 28,     /*!< Power Down Wake up interrupt                              */
  ADC_IRQn              = 29,     /*!< ADC interrupt                                             */
  DAC_IRQn              = 30,     /*!< DAC interrupt                                             */
  RTC_IRQn              = 31      /*!< Real time clock interrupt                                 */
} IRQn_Type;

																			
/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M# Processor and Core Peripherals */
#define __CM0_REV				0x0201    /*!< Core Revision r2p1                               */
#define __NVIC_PRIO_BITS		2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig	0         /*!< Set to 1 if different SysTick Config is used     */
#define __MPU_PRESENT			0         /*!< MPU present or not                               */
#define __FPU_PRESENT			0         /*!< FPU present or not                               */

/*@}*/ /* end of group NANO1xx_CMSIS */


#include "core_cm0.h"                       /* Cortex-M0 processor and core peripherals           */
#include "system_nano1xx.h"                 /* NANO1xx System include file                        */
#include <stdint.h>

/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/
/** @addtogroup NANO1xx_Peripherals NANO1xx Peripherals
  NANO1xx Device Specific Peripheral registers structures
  @{
*/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


/*------------- Global Control Register (GCR) -----------------------------*/
/** @addtogroup NANO1xx_GCR NANO1xx Global Control Register (GCR)
  @{
*/
typedef struct
{
  __I  uint32_t  PDID;						/*!< Offset: 0x0000   Part Device Identification Number Register         */
  __IO uint32_t  RST_SRC;				    /*!< Offset: 0x0004   System Reset Source Register                       */                
  __IO uint32_t  IPRST_CTL1;				/*!< Offset: 0x0008   IP Reset Control Register 1                        */
  __IO uint32_t  IPRST_CTL2;				/*!< Offset: 0x000C   IP Reset Control Register 2                        */
       uint32_t  RESERVED0[4];
  __IO uint32_t  TEMCTL;					/*!< Offset: 0x0020   Temperature Sensor Control Register                */
       uint32_t  RESERVED1[3];
  __IO uint32_t  PA_L_MFP;				    /*!< Offset: 0x0030   Port A low byte multiple function control Register */                 
  __IO uint32_t  PA_H_MFP;					/*!< Offset: 0x0034   Port A high byte multiple function control Register */
  __IO uint32_t  PB_L_MFP;					/*!< Offset: 0x0038   Port B low byte multiple function control Register */
  __IO uint32_t  PB_H_MFP;					/*!< Offset: 0x003C   Port B high byte multiple function control Register */
  __IO uint32_t  PC_L_MFP;					/*!< Offset: 0x0040   Port C low byte multiple function control Register */
  __IO uint32_t  PC_H_MFP;					/*!< Offset: 0x0044   Port C high byte multiple function control Register */
  __IO uint32_t  PD_L_MFP;					/*!< Offset: 0x0048   Port D low byte multiple function control Register */
  __IO uint32_t  PD_H_MFP;					/*!< Offset: 0x004C   Port D high byte multiple function control Register */
  __IO uint32_t  PE_L_MFP;					/*!< Offset: 0x0050   Port E low byte multiple function control Register */
  __IO uint32_t  PE_H_MFP;					/*!< Offset: 0x0054   Port E high byte multiple function control Register */
  __IO uint32_t  PF_L_MFP;					/*!< Offset: 0x0058   Port F low byte multiple function control Register  */
       uint32_t  RESERVED3[1];
  __IO uint32_t  PORCTL;					/*!< Offset: 0x0060   Power-On-Reset Controller Register */
  __IO uint32_t  BODCTL;					/*!< Offset: 0x0064   Brown-out Detector Control Register */
  __IO uint32_t  BODSTS;					/*!< Offset: 0x0068   Brown-out Detector Status Register */
  __IO uint32_t  VREFCTL;					/*!< Offset: 0x006C   Voltage reference Control Register */
       uint32_t  RESERVED4[4];
  __IO uint32_t  IRCTRIMCTL;				/*!< Offset: 0x0080   HIRC Trim Control Register */
  __IO uint32_t  IRCTRIMIER;				/*!< Offset: 0x0084   HIRC Trim Interrupt Enable Register */
  __IO uint32_t  IRCTRIMISR;				/*!< Offset: 0x0088   HIRC Trim Interrupt Status Register */
       uint32_t  RESERVED5[29];
  __IO uint32_t  RegLockAddr;				/*!< Offset: 0x0100   Register Lock Key address */
       uint32_t  RESERVED6[3];
  __IO uint32_t  RCADJ;						/*!< Offset: 0x0110   RC Adjustment control register */
} GCR_TypeDef;
/*@}*/ /* end of group NANO1xx_GCR */


/*------------- Clock Control Register (CLK) -----------------------------*/
/** @addtogroup NANO1xx_CLK NANO1xx Clock Control Register (CLK)
  @{
*/
typedef struct
{
  __IO uint32_t  PWRCTL; 					/*!< Offset: 0x0000   System Power Down Control Register                 */
  __IO uint32_t  AHBCLK;					/*!< Offset: 0x0004   AHB Devices Clock Enable Control Register          */
  __IO uint32_t  APBCLK;				    /*!< Offset: 0x0008   APB Devices Clock Enable Control Register          */
  __IO uint32_t  CLKSTATUS;					/*!< Offset: 0x000C   Clock Status Monitor Register                      */
  __IO uint32_t  CLKSEL0;					/*!< Offset: 0x0010   Clock Source Select Control Register 0			 */
  __IO uint32_t  CLKSEL1;					/*!< Offset: 0x0014   Clock Source Select Control Register 1			 */
  __IO uint32_t  CLKSEL2;					/*!< Offset: 0x0018   Clock Source Select Control Register 2			 */
  __IO uint32_t  CLKDIV0;				    /*!< Offset: 0x001C   Clock Divider Number Register	0					 */
  __IO uint32_t  CLKDIV1;					/*!< Offset: 0x0020   Clock Divider Number Register	1					 */
  __IO uint32_t  PLLCTL;					/*!< Offset: 0x0024   PLL Control Register		                         */
  __IO uint32_t  FRQDIV;					/*!< Offset: 0x0028   Frequency Divider Control Register                 */
  __IO uint32_t  TESTCLK;					/*!< Offset: 0x002C   Test Clock Source Select Control Register          */
  __IO uint32_t  WK_INTSTS;					/*!< Offset: 0x0030   Wake-up interrupt status Register                  */
} CLK_TypeDef;
/*@}*/ /* end of group NANO1xx_CLK */


/*------------- General Purpose I/O (GPIO) -----------------------------*/
/** @addtogroup NANO1xx_GPIO NANO1xx General Purpose I/O (GPIO)
  @{
*/
typedef struct
{
  __IO uint32_t  PMD;                        /*!< Offset: 0x0000   GPIO Port Bit Mode Control                         */ 
  __IO uint32_t  OFFD;                       /*!< Offset: 0x0004   GPIO Port Bit Off Digital Enable                   */
  __IO uint32_t  DOUT;                       /*!< Offset: 0x0008   GPIO Port Data Output                              */
  __IO uint32_t  DMASK;                      /*!< Offset: 0x000C   GPIO Port Data Output Write Mask                   */
  __I  uint32_t  PIN;                        /*!< Offset: 0x0010   GPIO Port Pin Value                                */
  __IO uint32_t  DBEN;                       /*!< Offset: 0x0014   GPIO Port De-bounce Enable                         */
  __IO uint32_t  IMD;                        /*!< Offset: 0x0018   GPIO Port Interrupt Mode Select                    */
  __IO uint32_t  IER;                        /*!< Offset: 0x001C   GPIO Port Interrupt Enable                         */
  __IO uint32_t  ISR;                        /*!< Offset: 0x0020   GPIO Port Interrupt Source Flag                    */
  __IO uint32_t  PUEN;                       /*!< Offset: 0x0024   GPIO Port Pull-Up Enable                           */
} GPIO_TypeDef;


/*  
 *  GPIO De-bounce Cycle Control
 */
typedef struct
{
  __IO uint32_t  CON;                        /*!< Offset: 0x0000   GPIO De-bounce Cycle Control Register              */
} GPIODBNCE_TypeDef;


/*  
 *  General Purpose I/O bit mode (GPIO bit mode)
 */
typedef struct
{
  __IO uint32_t  GP_BIT0;                        /*!< Offset: 0x0000   GPIO Port Bit 0 Data Register                  */
  __IO uint32_t  GP_BIT1;                        /*!< Offset: 0x0004   GPIO Port Bit 1 Data Register                  */
  __IO uint32_t  GP_BIT2;                        /*!< Offset: 0x0008   GPIO Port Bit 2 Data Register                  */
  __IO uint32_t  GP_BIT3;                        /*!< Offset: 0x000C   GPIO Port Bit 3 Data Register                  */
  __IO uint32_t  GP_BIT4;                        /*!< Offset: 0x0010   GPIO Port Bit 4 Data Register                  */
  __IO uint32_t  GP_BIT5;                        /*!< Offset: 0x0014   GPIO Port Bit 5 Data Register                  */
  __IO uint32_t  GP_BIT6;                        /*!< Offset: 0x0018   GPIO Port Bit 6 Data Register                  */
  __IO uint32_t  GP_BIT7;                        /*!< Offset: 0x001C   GPIO Port Bit 7 Data Register                  */
  __IO uint32_t  GP_BIT8;                        /*!< Offset: 0x0020   GPIO Port Bit 8 Data Register                  */
  __IO uint32_t  GP_BIT9;                        /*!< Offset: 0x0024   GPIO Port Bit 9 Data Register                  */
  __IO uint32_t  GP_BIT10;                       /*!< Offset: 0x0028   GPIO Port Bit 10 Data Register                 */
  __IO uint32_t  GP_BIT11;                       /*!< Offset: 0x002C   GPIO Port Bit 11 Data Register                 */
  __IO uint32_t  GP_BIT12;                       /*!< Offset: 0x0030   GPIO Port Bit 12 Data Register                 */
  __IO uint32_t  GP_BIT13;                       /*!< Offset: 0x0034   GPIO Port Bit 13 Data Register                 */
  __IO uint32_t  GP_BIT14;                       /*!< Offset: 0x0038   GPIO Port Bit 14 Data Register                 */
  __IO uint32_t  GP_BIT15;                       /*!< Offset: 0x003C   GPIO Port Bit 15 Data Register                 */
} GPIOBIT_TypeDef;
/*@}*/ /* end of group NANO1xx_GPIO */


/*------------- VDMA Control Register (VDMA) -----------------------------*/
/** @addtogroup NANO1xx_VDMA NANO1xx VDMA Control Register (VDMA)
  @{
*/
typedef struct
{
  __IO uint32_t  CSR;                        	 /*!< Offset: 0x0000   VDMA Control Register                          */
  __IO uint32_t  SAR;                        	 /*!< Offset: 0x0004   VDMA Source Address Register                   */
  __IO uint32_t  DAR;                        	 /*!< Offset: 0x0008   VDMA Destination Address Register              */
  __IO uint32_t  BCR;                        	 /*!< Offset: 0x000C   VDMA Transfer Byte Count Register              */
       uint32_t  RESERVED0;                    	 /*!< Offset: 0x0010   Reserved                                       */
  __I  uint32_t  CSAR;                        	 /*!< Offset: 0x0014   VDMA Current Source Address Register           */
  __I  uint32_t  CDAR;                        	 /*!< Offset: 0x0018   VDMA Current Destination Address Register      */
  __I  uint32_t  CBCR;                        	 /*!< Offset: 0x001C   VDMA Current Transfer Byte Count Register      */
  __IO uint32_t  IER;                        	 /*!< Offset: 0x0020   VDMA Interrupt Enable Register                 */
  __IO uint32_t  ISR;                        	 /*!< Offset: 0x0024   VDMA Interrupt Status Register                 */
       uint32_t  RESERVED1;                      /*!< Offset: 0x0028   Reserved                                       */
  __IO uint32_t  SASOCR;                         /*!< Offset: 0x002C   VDMA Source Address Stride Offset Register     */
  __IO uint32_t  DASOCR;                         /*!< Offset: 0x0030   VDMA Destination Address Stride Offset Register*/
       uint32_t  RESERVED2[19];                  /*!< Offset: 0x0034~0x007C   Reserved                                */
  __I  uint32_t  BUF0;                        	 /*!< Offset: 0x0080   VDMA Internal Buffer FIFO 0                    */
  __I  uint32_t  BUF1;                        	 /*!< Offset: 0x0084   VDMA Internal Buffer FIFO 1                    */
} VDMA_TypeDef;
/*@}*/ /* end of group NANO1xx_VDMA */


/*------------- PDMA Control Register (PDMA) -----------------------------*/
/** @addtogroup NANO1xx_PDMA NANO1xx PDMA Control Register (PDMA)
  @{
*/
typedef struct
{
  __IO uint32_t  CSR;                        	 /*!< Offset: 0x0000   PDMA Control Register                          */
  __IO uint32_t  SAR;                        	 /*!< Offset: 0x0004   PDMA Source Address Register                   */
  __IO uint32_t  DAR;                        	 /*!< Offset: 0x0008   PDMA Destination Address Register              */
  __IO uint32_t  BCR;                        	 /*!< Offset: 0x000C   PDMA Transfer Byte Count Register              */
       uint32_t  RESERVED0;                      /*!< Offset: 0x0010   Reserved                                       */
  __I  uint32_t  CSAR;                        	 /*!< Offset: 0x0014   PDMA Current Source Address Register           */
  __I  uint32_t  CDAR;                        	 /*!< Offset: 0x0018   PDMA Current Destination Address Register      */
  __I  uint32_t  CBCR;                        	 /*!< Offset: 0x001C   PDMA Current Transfer Byte Count Register      */
  __IO uint32_t  IER;                        	 /*!< Offset: 0x0020   PDMA Interrupt Enable Register                 */
  __IO uint32_t  ISR;                        	 /*!< Offset: 0x0024   PDMA Interrupt Status Register                 */
  __IO uint32_t  TCR;                        	 /*!< Offset: 0x0028   PDMA Timer Counter Setting Register            */
       uint32_t  RESERVED1[21];                  /*!< Offset: 0x002C~0x7C   Reserved                                  */
  __I  uint32_t  BUF0;                        	 /*!< Offset: 0x0080   PDMA Internal Buffer FIFO                      */
} PDMA_TypeDef;


/*  
 *  PDMA Global Control Registers
 */
typedef struct
{
  __IO uint32_t  CSR;                        	 /*!< Offset: 0x0000   DMA Global Control Register                    */
  __IO uint32_t  PDSSR0;                         /*!< Offset: 0x0004   DMA Service Selection Control Register 0       */
  __IO uint32_t  PDSSR1;                         /*!< Offset: 0x0008   DMA Service Selection Control Register 1       */
  __I  uint32_t  ISR;                        	 /*!< Offset: 0x000C   DMA Global Interrupt Register                  */
} PDMAGCR_TypeDef;


/*  
 *  PDMA CRC Control Registers
 */
typedef struct
{
  __IO uint32_t  CTL;
  __IO uint32_t  DMASAR;
       uint32_t  RESERVED0;
  __IO uint32_t  DMABCR;
       uint32_t  RESERVED1;
  __I  uint32_t  DMACSAR;
       uint32_t  RESERVED2;
  __I  uint32_t  DMACBCR;
  __IO uint32_t  DMAIER;
  __IO uint32_t  DMAISR;
       uint32_t  RESERVED3[22];
  __IO uint32_t  WDATA;
  __IO uint32_t  SEED;
  __I  uint32_t  CHECKSUM;
} PDMACRC_TypeDef;
/*@}*/ /* end of group NANO1xx_PDMA */


/*------------- Flash Memory Controller (FMC) -----------------------------*/
/** @addtogroup NANO1xx_FMC NANO1xx Flash Memory Controller Register (FMC)
  @{
*/
typedef struct
{
  __IO uint32_t  ISPCON;					/*!< Offset: 0x0000   ISP Control Register                               */
  __IO uint32_t  ISPADR;					/*!< Offset: 0x0004   ISP Address Register                               */ 
  __IO uint32_t  ISPDAT;					/*!< Offset: 0x0008   ISP Data Register                                  */ 
  __IO uint32_t  ISPCMD;					/*!< Offset: 0x000C   ISP Command Register                               */
  __IO uint32_t  ISPTRG;					/*!< Offset: 0x0010   ISP Trigger Register                               */
  __I  uint32_t  DFBADR;					/*!< Offset: 0x0014   Data Flash Start Address (CONFIG1)                 */ 
  __I  uint32_t  RESERVED0[10];
  __IO uint32_t  ISPSTA;					/*!< Offset: 0x0040   ISP Status Register                                */
} FMC_TypeDef;
/*@}*/ /* end of group NANO1xx_FMC */


/*------------- External Bus Interface Control Register (EBI) -----------------------------*/
/** @addtogroup NANO1xx_EBI NANO1xx External Bus Interfacel Control Register (EBI)
  @{
*/
typedef struct
{
  __IO uint32_t  EBICON;					/*!< Offset: 0x0000   EBI general control register                       */
  __IO uint32_t  EXTIME;					/*!< Offset: 0x0004   EBI timing control register                        */
} EBI_TypeDef;
/*@}*/ /* end of group NANO1xx_EBI */


/*------------- Watchdog Timer Control Register (WDT) -----------------------------*/
/** @addtogroup NANO1xx_WDT NANO1xx Watchdog Timer Control Register (WDT)
  @{
*/
typedef struct
{
  __IO uint32_t  CTL;						/*!< Offset: 0x0000   Watchdog Timer Control Register   		         */
  __IO uint32_t  IER;						/*!< Offset: 0x0004   Watchdog Timer Interrupt Enable Register 			 */
  __IO uint32_t  ISR;						/*!< Offset: 0x0008   Watchdog Timer Interrupt Status Register 			 */

} WDT_TypeDef;
/*@}*/ /* end of group NANO1xx_WDT */


/*------------- Window Watchdog Timer Control Register (WWDT) -----------------------------*/
/** @addtogroup NANO1xx_WWDT NANO1xx Watchdog Timer Control Register (WWDT)
  @{
*/
typedef struct
{
  __O  uint32_t  RLD;						/*!< Offset: 0x0000   Window Watchdog Timer Reload Counter Register		 */
  __IO uint32_t  CR;						/*!< Offset: 0x0004   Window Watchdog Timer Control Register	    		 */
  __IO uint32_t  IER;						/*!< Offset: 0x0008   Window Watchdog Timer Interrupt Enable Register		 */
  __IO uint32_t  STS;						/*!< Offset: 0x000C   Window Watchdog Timer Status Register			 */
  __I  uint32_t  VAL;						/*!< Offset: 0x0010   Window Watchdog Counter Value Register			 */
} WWDT_TypeDef;
/*@}*/ /* end of group NANO1xx_WWDT */


/*----------------------------- Real Time Clock Controller -------------------------------*/
/** @addtogroup NANO1xx_RTC NANO1xx Real Time Clock (RTC)
  @{
*/
typedef struct
{
  __IO uint32_t  INIR;		/*!< Offset: 0x0000   RTC Initiation Register	  	       	*/
  __IO uint32_t  AER;		/*!< Offset: 0x0004   RTC Access Enable Register  	     	*/
  __IO uint32_t  FCR;		/*!< Offset: 0x0008   RTC Frequency Compensation Register	*/
  __IO uint32_t  TLR;		/*!< Offset: 0x000C   Time Loading Register					*/
  __IO uint32_t  CLR;		/*!< Offset: 0x0010   Calendar Loading Register				*/
  __IO uint32_t  TSSR;		/*!< Offset: 0x0014   Time Scale Selection Register			*/
  __IO uint32_t  DWR;		/*!< Offset: 0x0018   Day of the Week Register				*/
  __IO uint32_t  TAR;		/*!< Offset: 0x001C   Time Alarm Register					*/
  __IO uint32_t  CAR;		/*!< Offset: 0x0020   Calendar Alarm Register				*/
  __I  uint32_t  LIR;		/*!< Offset: 0x0024   Leap year Indicator Register			*/
  __IO uint32_t  RIER;		/*!< Offset: 0x0028   RTC Interrupt Enable Register			*/
  __IO uint32_t  RIIR;		/*!< Offset: 0x002C   RTC Interrupt Indicator Register		*/
  __IO uint32_t  TTR;		/*!< Offset: 0x0030   RTC Time Tick Register				*/
  uint32_t  RESERVED0[2];
  __IO uint32_t  SPRCTL;	/*!< Offset: 0x003C   RTC Spare Functional Control Register	*/
  __IO uint32_t  SPR0;		/*!< Offset: 0x0040   RTC Spare Register 0					*/
  __IO uint32_t  SPR1;		/*!< Offset: 0x0044   RTC Spare Register 1					*/
  __IO uint32_t  SPR2;		/*!< Offset: 0x0048   RTC Spare Register 2					*/
  __IO uint32_t  SPR3;		/*!< Offset: 0x004C   RTC Spare Register 3					*/
  __IO uint32_t  SPR4;		/*!< Offset: 0x0050   RTC Spare Register 4					*/
  __IO uint32_t  SPR5;		/*!< Offset: 0x0054   RTC Spare Register 5					*/
  __IO uint32_t  SPR6;		/*!< Offset: 0x0058   RTC Spare Register 6					*/
  __IO uint32_t  SPR7;		/*!< Offset: 0x005C   RTC Spare Register 7					*/
  __IO uint32_t  SPR8;		/*!< Offset: 0x0060   RTC Spare Register 8					*/
  __IO uint32_t  SPR9;		/*!< Offset: 0x0064   RTC Spare Register 9					*/
  __IO uint32_t  SPR10;		/*!< Offset: 0x0068   RTC Spare Register 10					*/
  __IO uint32_t  SPR11;		/*!< Offset: 0x006C   RTC Spare Register 11					*/
  __IO uint32_t  SPR12;		/*!< Offset: 0x0070   RTC Spare Register 12					*/
  __IO uint32_t  SPR13;		/*!< Offset: 0x0074   RTC Spare Register 13					*/
  __IO uint32_t  SPR14;		/*!< Offset: 0x0078   RTC Spare Register 14					*/
  __IO uint32_t  SPR15;		/*!< Offset: 0x007C   RTC Spare Register 15					*/
  __IO uint32_t  SPR16;		/*!< Offset: 0x0080   RTC Spare Register 16					*/
  __IO uint32_t  SPR17;		/*!< Offset: 0x0084   RTC Spare Register 17					*/
  __IO uint32_t  SPR18;		/*!< Offset: 0x0088   RTC Spare Register 18					*/
  __IO uint32_t  SPR19;		/*!< Offset: 0x008C   RTC Spare Register 19					*/
} RTC_TypeDef;
/*@}*/ /* end of group NANO1xx_RTC */


/*----------------------------- ADC Controller -------------------------------*/
/** @addtogroup NANO1xx_ADC NANO1xx A/D Converter (ADC)
  @{
*/
typedef struct
{
  __I  uint32_t  RESULT0;					/*!< Offset: 0x0000   A/D result Register 0								 */
  __I  uint32_t  RESULT1;					/*!< Offset: 0x0004   A/D result Register 1								 */
  __I  uint32_t  RESULT2;					/*!< Offset: 0x0008   A/D result Register 2								 */
  __I  uint32_t  RESULT3;					/*!< Offset: 0x000C   A/D result Register 3								 */
  __I  uint32_t  RESULT4;					/*!< Offset: 0x0010   A/D result Register 4								 */
  __I  uint32_t  RESULT5;					/*!< Offset: 0x0014   A/D result Register 5								 */
  __I  uint32_t  RESULT6;					/*!< Offset: 0x0018   A/D result Register 6								 */
  __I  uint32_t  RESULT7;					/*!< Offset: 0x001C   A/D result Register 7								 */
  __I  uint32_t  RESULT8;					/*!< Offset: 0x0020   A/D result Register 8								 */
  __I  uint32_t  RESULT9;					/*!< Offset: 0x0024   A/D result Register 9								 */
  __I  uint32_t  RESULT10;					/*!< Offset: 0x0028   A/D result Register 10							 */
  __I  uint32_t  RESULT11;					/*!< Offset: 0x002C   A/D result Register 11							 */
  __I  uint32_t  RESULT12;					/*!< Offset: 0x0030   A/D result Register 12							 */
  __I  uint32_t  RESULT13;					/*!< Offset: 0x0034   A/D result Register 13							 */
  __I  uint32_t  RESULT14;					/*!< Offset: 0x0038   A/D result Register 14							 */
  __I  uint32_t  RESULT15;					/*!< Offset: 0x003C   A/D result Register 15							 */				 
  __I  uint32_t  RESULT16;					/*!< Offset: 0x0040   A/D result Register 16							 */
  __I  uint32_t  RESULT17;					/*!< Offset: 0x0044   A/D result Register 17							 */
  __IO uint32_t  CR;						/*!< Offset: 0x0048   A/D Control Register 								 */
  __IO uint32_t  CHER;						/*!< Offset: 0x004C   A/D Channel Enable Register						 */
  __IO uint32_t  CMPR0;						/*!< Offset: 0x0050   A/D Compare Register 0							 */
  __IO uint32_t  CMPR1;						/*!< Offset: 0x0054   A/D Compare Register 1							 */
  __IO uint32_t  SR;						/*!< Offset: 0x0058   A/D Status Register								 */
  uint32_t  RESERVED0;						/*!< Offset: 0x005C   Reserved                                           */
  __I  uint32_t  PDMA;  					/*!< Offset: 0x0060   A/D PDMA Current Transfer Data Register            */
  __IO uint32_t  PWRCTL;					/*!< Offset: 0x0064   A/D Power Control Register                         */
  __IO uint32_t  CALCTL;					/*!< Offset: 0x0068   A/D Calibration Control Register                   */
  __IO uint32_t  CALWORD;					/*!< Offset: 0x006C   A/D Calibration Load Word Register                 */
  __IO uint32_t  SMPLCNT0;					/*!< Offset: 0x0070   A/D Sample Register 0                              */
  __IO uint32_t  SMPLCNT1;					/*!< Offset: 0x0074   A/D Sample Register 1                              */
} ADC_TypeDef;
/*@}*/ /* end of group NANO1xx_ADC */


/*------------- I2S Control Register (I2S) -----------------------------*/
/** @addtogroup NANO1xx_I2S NANO1xx I2S Control Register (I2S)
  @{
*/
typedef struct
{
  __IO uint32_t  CTRL;					/*!< Offset: 0x0000   I2S Control Register                               */
  __IO uint32_t  CLKDIV;				/*!< Offset: 0x0004   I2S Clock Divider Register                         */
  __IO uint32_t  INTEN;					/*!< Offset: 0x0008   I2S Interrupt Enable Register                      */
  __IO uint32_t  STATUS;				/*!< Offset: 0x000C   I2S Status Register                                */
  __O uint32_t  TXFIFO;					/*!< Offset: 0x0010   I2S Transmit FIFO Register                         */
  __I uint32_t  RXFIFO;					/*!< Offset: 0x0014   I2S Receive FIFO Register                          */
} I2S_TypeDef;
/*@}*/ /* end of group NANO1xx_I2S */


/*------------- USB Device Control Register (USBD) -----------------------------*/
/** @addtogroup NANO1xx_USBD NANO1xx USB Device Control Register (USBD)
  @{
*/
typedef struct
{
  __IO uint32_t  CTRL;                           /*!< Offset: 0x0000   USB Control Register                           */ 
  __I  uint32_t  BUSSTS;                         /*!< Offset: 0x0004   USB Bus Status Register                        */ 
  __IO uint32_t  INTEN;                          /*!< Offset: 0x0008   USB Interrupt Enable Register                  */ 
  __IO uint32_t  INTSTS;                         /*!< Offset: 0x000C   USB Interrupt Event Status Register            */ 
  __IO uint32_t  DADDR;                          /*!< Offset: 0x0010   USB Device's Function Address Register         */ 
  __I  uint32_t  EPSTS;                          /*!< Offset: 0x0014   USB Endpoint Status Register                   */ 
  __IO uint32_t  BUFSEG;                         /*!< Offset: 0x0018   USB Setup Token Buffer Segmentation Register   */ 
  __I  uint32_t  EPSTS2;                         /*!< Offset: 0x001C   USB Endpoint Status Register 2                 */ 
  __IO uint32_t  BUFSEG0;                        /*!< Offset: 0x0020   USB Endpoint 0 Buffer Segmentation Register    */ 
  __IO uint32_t  MXPLD0;                         /*!< Offset: 0x0024   USB Endpoint 0 Maximal Payload Register        */ 
  __IO uint32_t  CFG0;                           /*!< Offset: 0x0028   USB Endpoint 0 Configuration Register          */ 
       uint32_t  RESERVED1;                      /*!< Offset: 0x002C   Reserved                                       */ 
  __IO uint32_t  BUFSEG1;                        /*!< Offset: 0x0030   USB Endpoint 1 Buffer Segmentation Register    */ 
  __IO uint32_t  MXPLD1;                         /*!< Offset: 0x0034   USB Endpoint 1 Maximal Payload Register        */ 
  __IO uint32_t  CFG1;                           /*!< Offset: 0x0038   USB Endpoint 1 Configuration Register          */ 
       uint32_t  RESERVED2;                      /*!< Offset: 0x003C   Reserved                                       */ 
  __IO uint32_t  BUFSEG2;                        /*!< Offset: 0x0040   USB Endpoint 2 Buffer Segmentation Register    */ 
  __IO uint32_t  MXPLD2;                         /*!< Offset: 0x0044   USB Endpoint 2 Maximal Payload Register        */ 
  __IO uint32_t  CFG2;                           /*!< Offset: 0x0048   USB Endpoint 2 Configuration Register          */ 
       uint32_t  RESERVED3;                      /*!< Offset: 0x004C   Reserved                                       */ 
  __IO uint32_t  BUFSEG3;                        /*!< Offset: 0x0050   USB Endpoint 3 Buffer Segmentation Register    */ 
  __IO uint32_t  MXPLD3;                         /*!< Offset: 0x0054   USB Endpoint 3 Maximal Payload Register        */ 
  __IO uint32_t  CFG3;                           /*!< Offset: 0x0058   USB Endpoint 3 Configuration Register          */ 
       uint32_t  RESERVED4;                      /*!< Offset: 0x005C   Reserved                                       */ 
  __IO uint32_t  BUFSEG4;                        /*!< Offset: 0x0060   USB Endpoint 4 Buffer Segmentation Register    */ 
  __IO uint32_t  MXPLD4;                         /*!< Offset: 0x0064   USB Endpoint 4 Maximal Payload Register        */ 
  __IO uint32_t  CFG4;                           /*!< Offset: 0x0068   USB Endpoint 4 Configuration Register          */ 
       uint32_t  RESERVED5;                      /*!< Offset: 0x006C   Reserved                                       */ 
  __IO uint32_t  BUFSEG5;                        /*!< Offset: 0x0070   USB Endpoint 5 Buffer Segmentation Register    */ 
  __IO uint32_t  MXPLD5;                         /*!< Offset: 0x0074   USB Endpoint 5 Maximal Payload Register        */ 
  __IO uint32_t  CFG5;                           /*!< Offset: 0x0078   USB Endpoint 5 Configuration Register          */ 
       uint32_t  RESERVED6;                      /*!< Offset: 0x007C   Reserved                                       */ 
  __IO uint32_t  BUFSEG6;                        /*!< Offset: 0x0080   USB Endpoint 6 Buffer Segmentation Register    */ 
  __IO uint32_t  MXPLD6;                         /*!< Offset: 0x0084   USB Endpoint 6 Maximal Payload Register        */ 
  __IO uint32_t  CFG6;                           /*!< Offset: 0x0088   USB Endpoint 6 Configuration Register          */ 
       uint32_t  RESERVED7;                      /*!< Offset: 0x008C   Reserved                                       */ 
  __IO uint32_t  BUFSEG7;                        /*!< Offset: 0x0090   USB Endpoint 7 Buffer Segmentation Register    */ 
  __IO uint32_t  MXPLD7;                         /*!< Offset: 0x0094   USB Endpoint 7 Maximal Payload Register        */ 
  __IO uint32_t  CFG7;                           /*!< Offset: 0x0098   USB Endpoint 7 Configuration Register          */ 
       uint32_t  RESERVED8[2];                   /*!< Offset: 0x009C~0x00A0   Reserved                                       */ 
  __IO uint32_t  PDMA;                           /*!< Offset: 0x00A4   USB PDMA Control Register                           */ 
} USBD_TypeDef;
/*@}*/ /* end of group NANO1xx_USBD */


/*------------- DAC (DAC) -----------------------------*/
/** @addtogroup NANO1xx_DAC NANO1xx DAC Control Register (DAC)
  @{
*/
typedef struct
{
  __IO uint32_t  CTL0;						/*!< Offset: 0x0000   DAC0 Control Register                              */
  __IO uint32_t  DATA0;						/*!< Offset: 0x0004   DAC0 Data Register                                 */
  __IO uint32_t  STS0;						/*!< Offset: 0x0008   DAC0 Status Register                               */
  uint32_t  RESERVED0;						/*!< Offset: 0x000C   Reserved                                           */
  __IO uint32_t  CTL1;						/*!< Offset: 0x0010   DAC1 Control Register                              */
  __IO uint32_t  DATA1;						/*!< Offset: 0x0014   DAC1 Data Register                                 */
  __IO uint32_t  STS1;						/*!< Offset: 0x0018   DAC1 Status Register                               */
  uint32_t  RESERVED1;						/*!< Offset: 0x001C   Reserved                                           */
  __IO uint32_t  COMCTL;					/*!< Offset: 0x0020   DAC01 Common Control Register                      */
} DAC_TypeDef;
/*@}*/ /* end of group NANO1xx_DAC */


/*------------- LCD (LCD) -----------------------------*/
/** @addtogroup NANO1xx_LCD NANO1xx LCD Control Register (LCD)
  @{
*/
typedef struct
{
  __IO uint32_t  CTL;		/*!< Offset: 0x0000   LCD Control Register		    	   */
  __IO uint32_t  DISPCTL;	/*!< Offset: 0x0004   LCD Display Control Register	      */
  __IO uint32_t  MEM_0;		/*!< Offset: 0x0008   LCD SEG3 ~ SEG0 data				  */
  __IO uint32_t  MEM_1;		/*!< Offset: 0x000C   LCD SEG7 ~ SEG4 data				  */
  __IO uint32_t  MEM_2;		/*!< Offset: 0x0010   LCD SEG11 ~ SEG8 data				  */
  __IO uint32_t  MEM_3;		/*!< Offset: 0x0014   LCD SEG15 ~ SEG12 data			  */
  __IO uint32_t  MEM_4;		/*!< Offset: 0x0018   LCD SEG19 ~ SEG16 data			  */
  __IO uint32_t  MEM_5;		/*!< Offset: 0x001C   LCD SEG23 ~ SEG20 data			  */
  __IO uint32_t  MEM_6;		/*!< Offset: 0x0020   LCD SEG27 ~ SEG24 data			  */
  __IO uint32_t  MEM_7;		/*!< Offset: 0x0024   LCD SEG31 ~ SEG28 data			  */
  __IO uint32_t  MEM_8;		/*!< Offset: 0x0028   LCD SEG35 ~ SEG32 data			  */
  __IO uint32_t  MEM_9;		/*!< Offset: 0x002C   LCD SEG39 ~ SEG36 data			  */
  __IO uint32_t  FCR;		/*!< Offset: 0x0030   LCD frame counter control register  */
  __IO uint32_t  FCSTS;		/*!< Offset: 0x0034   LCD frame counter status			  */
} LCD_TypeDef;
/*@}*/ /* end of group NANO1xx_LCD */


/*  
 * @brief Capacitive Touch-Key Sensing Engine
 */
 /*------------- Touch-Key Sensing Engine -----------------------------*/
/** @addtogroup NANO1xx_TK NANO1xx TK Control Register (TK)
  @{
*/
typedef struct
{
  __IO uint32_t  CTL1;						/*!< Offset: 0x0000  Touch Key Control Register 1				 	 	 */
  __IO uint32_t  CTL2;						/*!< Offset: 0x0004  Touch Key Control Register 2				 	 	 */
  __IO uint32_t  CTL3;						/*!< Offset: 0x0008  Touch Key Control Register 3				 	 	 */
  __IO uint32_t  STAT;						/*!< Offset: 0x000C  Touch Key Status Register  				 	 	 */
  __IO uint32_t  DATA1;						/*!< Offset: 0x0010  Touch Key Data Register 1					 	 	 */
  __IO uint32_t  DATA2;						/*!< Offset: 0x0014  Touch Key Data Register 2					 	 	 */
  __IO uint32_t  DATA3;						/*!< Offset: 0x0018  Touch Key Data Register 3					 	 	 */
  __IO uint32_t  DATA4;						/*!< Offset: 0x001C  Touch Key Data Register 4					 	 	 */
  __IO uint32_t  INTEN;						/*!< Offset: 0x0020  Touch Key Interrupt Enable Register		 	 	 */
  __IO uint32_t  TK8_0_THC;					/*!< Offset: 0x0024  TK8/TK0 Threshold Control Register			 	 	 */
  __IO uint32_t  TK9_1_THC;					/*!< Offset: 0x0028  TK9/TK1 Threshold Control Register			 	 	 */
  __IO uint32_t  TK10_2_THC;				/*!< Offset: 0x002C  TK10/TK2 Threshold Control Register		 	 	 */
  __IO uint32_t  TK11_3_THC;				/*!< Offset: 0x0030  TK11/TK3 Threshold Control Register		 	 	 */
  __IO uint32_t  TK12_4_THC;				/*!< Offset: 0x0034  TK12/TK4 Threshold Control Register		 	 	 */
  __IO uint32_t  TK13_5_THC;				/*!< Offset: 0x0038  TK13/TK5 Threshold Control Register		 	 	 */
  __IO uint32_t  TK14_6_THC;				/*!< Offset: 0x003C  TK14/TK6 Threshold Control Register		 	 	 */
  __IO uint32_t  TK15_7_THC;				/*!< Offset: 0x0040  TK15/TK7 Threshold Control Register		 	 	 */
} TK_TypeDef;
/*@}*/ /* end of group NANO1xx_TK */


/*------------- Timer Control Register (TIMER) -----------------------------*/
/** @addtogroup NANO1xx_TIMER NANO1xx TIMER Control Register (TIMER)
  @{
*/
typedef struct
{
  __IO uint32_t  CTL;						/*!< Offset: 0x0000   Timer Control Register        			         */
  __IO uint32_t  PRECNT;					/*!< Offset: 0x0004   Timer Pre-Scale Counter Register      			 */
  __IO uint32_t  CMPR;						/*!< Offset: 0x0008   Timer Compare Register             			     */
  __IO uint32_t  IER;						/*!< Offset: 0x000C   Timer Interrupt Enable Register 				     */
  __IO uint32_t  ISR;						/*!< Offset: 0x0010   Timer Interrupt Status Register        			 */
  __I  uint32_t  DR;						/*!< Offset: 0x0014   Timer Data Register      				  			 */
  __I  uint32_t  TCAP;						/*!< Offset: 0x0018   Timer Capture Data Register 			  			 */
} TIMER_TypeDef;
/*@}*/ /* end of group NANO1xx_TIMER */


/*----------------------------- PWM Controller ----------------------------*/
/** @addtogroup NANO1xx_PWM NANO1xx PWM Control Register (PWM)
  @{
*/
typedef struct
{
  __IO uint32_t  PRES;						/*!< Offset: 0x0000   PWM Prescaler Register 				 	  		 */
  __IO uint32_t  CLKSEL;					/*!< Offset: 0x0004   PWM Clock Select Register 			 	  		 */
  __IO uint32_t  CTL;						/*!< Offset: 0x0008   PWM Ccontrol Register 				 	  		 */
  __IO uint32_t  INTEN;						/*!< Offset: 0x000C   PWM Interrupt Enable Register 		 	  		 */
  __IO uint32_t  INTSTS;					/*!< Offset: 0x0010   PWM Interrupt Indication Register		 	  		 */
  __IO uint32_t  OE;						/*!< Offset: 0x0014   PWM Output Enable Register			 	  		 */
       uint32_t  RESERVED0;					/*!< Offset: 0x0018   Reserved                                           */
  __IO uint32_t  DUTY0;						/*!< Offset: 0x001C   PWM Counter/Comparator Register 0		 	  		 */
       uint32_t  RESERVED1[2];				/*!< Offset: 0x0020 ~ 0x0024 Reserved	                                 */
  __IO uint32_t  DUTY1;						/*!< Offset: 0x0028  PWM Counter/Comparator Register 1		 	  		 */
       uint32_t  RESERVED2[2];				/*!< Offset: 0x002C ~ 0x0030 Reserved	                                 */
  __IO uint32_t  DUTY2;						/*!< Offset: 0x0034  PWM Counter/Comparator Register 2		 	  		 */
       uint32_t  RESERVED3[2];				/*!< Offset: 0x0038 ~ 0x003C Reserved	                                 */
  __IO uint32_t  DUTY3;						/*!< Offset: 0x0040  PWM Counter/Comparator Register 3		 	  		 */
       uint32_t  RESERVED4[4];				/*!< Offset: 0x0044 ~ 0x0050 Reserved	                                 */
  __IO uint32_t  CAPCTL;					/*!< Offset: 0x0054  Capture Control Register				 	  		 */
  __IO uint32_t  CAPINTEN;					/*!< Offset: 0x0058  Capture Interrupt Enable Register			 	 	 */
  __IO uint32_t  CAPINTSTS;					/*!< Offset: 0x005C  Capture Interrupt Indication Register		 	 	 */
  __I  uint32_t  CRL0;						/*!< Offset: 0x0060  Capture Rising Latch Register 0			 	 	 */
  __I  uint32_t  CFL0;						/*!< Offset: 0x0064  Capture Falling Latch Register 0			 	 	 */
  __I  uint32_t  CRL1;						/*!< Offset: 0x0068  Capture Rising Latch Register 1			 	 	 */
  __I  uint32_t  CFL1;						/*!< Offset: 0x006C  Capture Falling Latch Register 1			 	 	 */
  __I  uint32_t  CRL2;						/*!< Offset: 0x0070  Capture Rising Latch Register 2			 	 	 */
  __I  uint32_t  CFL2;						/*!< Offset: 0x0074  Capture Falling Latch Register 2			 	 	 */
  __I  uint32_t  CRL3;						/*!< Offset: 0x0078  Capture Rising Latch Register 3			 	 	 */
  __I  uint32_t  CFL3;						/*!< Offset: 0x007C  Capture Falling Latch Register 3			 	 	 */
  __I  uint32_t  CH0PDMA;					/*!< Offset: 0x0080  PDMA channel 0 captured data				 	 	 */
  __I  uint32_t  CH2PDMA;					/*!< Offset: 0x0084  PDMA channel 1 captured data				 	 	 */
} PWM_TypeDef;
/*@}*/ /* end of group NANO1xx_PWM */


/*-------------------- Serial Peripheral Interface (SPI) ----------------------*/
/** @addtogroup NANO1xx_SPI NANO1xx Serial Peripheral Interface (SPI)
  @{
 */
typedef struct
{
  __IO uint32_t  CTL;						/*!< Offset: 0x0000   SPI Control Register                               */
  __IO uint32_t  STATUS;					/*!< Offset: 0x0004   SPI Status  Register                               */
  __IO uint32_t  CLKDIV;					/*!< Offset: 0x0008   SPI Serial Clock Divider Register                  */
  __IO uint32_t  SSR;						/*!< Offset: 0x000C   SPI Slave Select Register                          */
  __I  uint32_t  RX0;						/*!< Offset: 0x0010   SPI Receive Data FIFO Register 0                   */
  __I  uint32_t  RX1;						/*!< Offset: 0x0014   SPI Receive Data FIFO Register 1                   */
       uint32_t  RESERVED0;
       uint32_t  RESERVED1;
  __O  uint32_t  TX0;						/*!< Offset: 0x0020   SPI Transmit Data FIFO Register 0                  */
  __O  uint32_t  TX1;						/*!< Offset: 0x0024   SPI Transmit Data FIFO Register 1                  */
       uint32_t  RESERVED2;
       uint32_t  RESERVED3;
       uint32_t  RESERVED4;
  __IO uint32_t  VARCLK;					/*!< Offset: 0x0034   SPI Variable Clock Pattern Flag Register           */
  __IO uint32_t  PDMA;						/*!< Offset: 0x0038   SPI PDMA Control Register                          */
  __IO uint32_t  FF_CTL;					/*!< Offset: 0x003C   SPI FIFO Control Register                          */
} SPI_TypeDef;
/*@}*/ /* end of group NANO1xx_SPI */


/*------------- UART -----------------------------*/
/** @addtogroup NANO1xx_UART NANO1xx UART Control Register (UART)
  @{
*/
typedef struct
{
  union {
  __I  uint32_t  RBR;		/*!< Offset: 0x0000   UART Receive Buffer Register  	       */
  __O  uint32_t  THR;		/*!< Offset: 0x0000   UART Transmit Holding Register  	       */
  };
  __IO uint32_t  CTL;		/*!< Offset: 0x0004   UART Control State Register	  	       */
  __IO uint32_t  TLCTL;		/*!< Offset: 0x0008   UART Transfer Line Control Register       */
  __IO uint32_t  IER;		/*!< Offset: 0x000C   UART Interrupt Enable Register	       */
  __IO uint32_t  ISR;		/*!< Offset: 0x0010   UART Interrupt Status Register	       */
  __IO uint32_t  TRSR;		/*!< Offset: 0x0014   UART Transfer State Status Register       */
  __IO uint32_t  FSR;		/*!< Offset: 0x0018   UART FIFO State Status Register	       */
  __IO uint32_t  MCSR;		/*!< Offset: 0x001C   UART Modem State Status Register	       */
  __IO uint32_t  TMCTL;		/*!< Offset: 0x0020   UART Time-Out Control State Register       */
  __IO uint32_t  BAUD;		/*!< Offset: 0x0024   UART Baud Rate Divisor Register     		  */
  uint32_t  RESERVED0[2];
  __IO uint32_t  IRCR;		/*!< Offset: 0x0030   UART IrDA Control Register	     		  */
  __IO uint32_t  ALT_CTL;	/*!< Offset: 0x0034   UART Alternate Control State Register		  */
  __IO uint32_t  FUN_SEL;	/*!< Offset: 0x0038   UART Function Select Register				  */
} UART_TypeDef;
/*@}*/ /* end of group NANO1xx_UART */		    



/*
 * @brief GPIO shadow registers
 */
/*------------- GPIO Shadow Registers (SHADOW) -----------------------------*/
/** @addtogroup NANO1xx_SHADOW NANO1xx GPIO Shadow Register (SHADOW)
  @{
*/ 
typedef struct
{
   __I uint32_t  GPA_SHADOW;				/*!< Offset: 0x0000   GPIO Port A Pin Value Shadow Register 	  		 */
   __I uint32_t  GPB_SHADOW;				/*!< Offset: 0x0004   GPIO Port B Pin Value Shadow Register 	  		 */
   __I uint32_t  GPC_SHADOW;				/*!< Offset: 0x0008   GPIO Port C Pin Value Shadow Register 	  		 */
   __I uint32_t  GPD_SHADOW;				/*!< Offset: 0x000C   GPIO Port D Pin Value Shadow Register 	  		 */
   __I uint32_t  GPE_SHADOW;				/*!< Offset: 0x0010   GPIO Port E Pin Value Shadow Register 	  		 */
   __I uint32_t  GPF_SHADOW;				/*!< Offset: 0x0014   GPIO Port F Pin Value Shadow Register 	  		 */
} SHADOW_TypeDef;
/*@}*/ /* end of group NANO1xx_SHADOW */



/*------------- I2C Serial Interface Controller (Master/Slave) (I2C) -----------------------------*/
/** @addtogroup NANO1xx_I2C NANO1xx I2C Controller Register (I2C)
  @{
*/
typedef struct
{
   __IO uint32_t  CON;					/*!< Offset: 0x0000   I2C Control Register                               */
   __IO uint32_t  INTSTS;				/*!< Offset: 0x0004   I2C Control Flag Register                          */
   __I  uint32_t  STATUS;				/*!< Offset: 0x0008   I2C Status Register                                */
   __IO uint32_t  DIV;					/*!< Offset: 0x000C   I2C Clock Divided Register                         */
   __IO uint32_t  TOUT;					/*!< Offset: 0x0010   I2C Time Out Control Register                      */
   __IO uint32_t  DATA;					/*!< Offset: 0x0014   I2C Data Register                                  */
   __IO uint32_t  SADDR0;				/*!< Offset: 0x0018   Slave Address Register 0                           */
   __IO uint32_t  SADDR1;				/*!< Offset: 0x001C   Slave Address Register 1                           */
        uint32_t  RESERVED0;
        uint32_t  RESERVED1;
   __IO uint32_t  SAMASK0;				/*!< Offset: 0x0028   Slave Address Mask Register 0                      */
   __IO uint32_t  SAMASK1;				/*!< Offset: 0x002C   Slave Address Mask Register 1                      */
   		uint32_t  RESERVED2[4];
   __IO uint32_t  WKUPCON;				/*!< Offset: 0x003C   I2C Wake-up Control Register                       */
   __IO uint32_t  WKUPSTS;				/*!< Offset: 0x0040   I2C Wake-up Status Register                        */
  
} I2C_TypeDef;
/*@}*/ /* end of group NANO1xx_I2C */


/*------------- Smart Card Control Register (SC) -----------------------------*/
/** @addtogroup NANO1xx_SC NANO1xx Smart Card Control Register (SC)
  @{
*/
typedef struct
{
  union {
  __I  uint32_t  RBR;						/*!< Offset: 0x0000  SC Receiving Buffer Register				 	 	 */
  __O  uint32_t  THR;						/*!< Offset: 0x0000  SC Transmit Holding Register				 	 	 */
  };
  __IO uint32_t  CTL;						/*!< Offset: 0x0004  SC Control Register						 	 	 */
  __IO uint32_t  ALTCTL;					/*!< Offset: 0x0008  SC Alternate Control Register				 	 	 */
  __IO uint32_t  EGTR;						/*!< Offset: 0x000C  SC Extend Guard Time Register				 	 	 */
  __IO uint32_t  RFTMR;						/*!< Offset: 0x0010  SC Receive Buffer Time-Out Register		 	 	 */
  __IO uint32_t  ETUCR;						/*!< Offset: 0x0014  SC ETU Control Register					 	 	 */
  __IO uint32_t  IER;						/*!< Offset: 0x0018  SC Interrupt Enable Register				 	 	 */
  __IO uint32_t  ISR;						/*!< Offset: 0x001C  SC Interrupt Status Register				 	 	 */
  __IO uint32_t  TRSR;						/*!< Offset: 0x0020  SC Transfer Status Register				 	 	 */
  __IO uint32_t  PINCSR;					/*!< Offset: 0x0024  SC Pin Control State Register				 	 	 */
  __IO uint32_t  TMR0;						/*!< Offset: 0x0028  SC Internal Timer Control Register 0		 	 	 */
  __IO uint32_t  TMR1;						/*!< Offset: 0x002C  SC Internal Timer Control Register 1		 	 	 */
  __IO uint32_t  TMR2;						/*!< Offset: 0x0030  SC Internal Timer Control Register 2		 	 	 */
  __IO uint32_t  UACTL;						/*!< Offset: 0x0034  SC UART Mode Control Register				 	 	 */
  __I  uint32_t  TDRA;
  __I  uint32_t  TDRB;  
} SC_TypeDef;
/*@}*/ /* end of group NANO1xx_SC */


#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

/*@}*/ /* end of group NANO1xx_Peripherals */


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/** @addtogroup NANO1xx_MemoryMap NANO1xx Memory Mapping
  @{
*/

/* Peripheral and SRAM base address */
//#define <DeviceAbbreviation>_FLASH_BASE       (0x00000000UL)           /*!< (FLASH     ) Base Address */
#define SRAM_BASE            (0x20000000UL)                              /*!< (SRAM      ) Base Address */
#define PERIPH_BASE          (0x40000000UL)                              /*!< (Peripheral) Base Address */


/* Peripheral memory map */
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x100000)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x10000000)

#define WDT_BASE	          (APB1PERIPH_BASE + 0x04000)
#define WWDT_BASE	          (APB1PERIPH_BASE + 0x04100)
#define RTC_BASE	          (APB1PERIPH_BASE + 0x08000)
#define TIMER0_BASE	          (APB1PERIPH_BASE + 0x10000)
#define TIMER1_BASE	          (APB1PERIPH_BASE + 0x10100)
#define I2C0_BASE	          (APB1PERIPH_BASE + 0x20000)
#define SPI0_BASE	          (APB1PERIPH_BASE + 0x30000)
#define PWM0_BASE	          (APB1PERIPH_BASE + 0x40000)
#define UART0_BASE	          (APB1PERIPH_BASE + 0x50000)
#define USBD_BASE	          (APB1PERIPH_BASE + 0x60000)
#define USBD_SRAM_BASE	      (APB1PERIPH_BASE + 0x60100)
#define DAC_BASE	          (APB1PERIPH_BASE + 0xA0000)
#define LCD_BASE	          (APB1PERIPH_BASE + 0xB0000)
#define TK_BASE	              (APB1PERIPH_BASE + 0xC0000)
#define SPI2_BASE             (APB1PERIPH_BASE + 0xD0000)
#define ADC_BASE	          (APB1PERIPH_BASE + 0xE0000)

#define TIMER2_BASE	          (APB2PERIPH_BASE + 0x10000)
#define TIMER3_BASE	          (APB2PERIPH_BASE + 0x10100)
#define SHADOW_BASE	   		  (APB1PERIPH_BASE + 0x10200)
#define I2C1_BASE	          (APB2PERIPH_BASE + 0x20000)
#define SPI1_BASE	          (APB2PERIPH_BASE + 0x30000)
#define PWM1_BASE	          (APB2PERIPH_BASE + 0x40000)
#define UART1_BASE	          (APB2PERIPH_BASE + 0x50000)
#define SC0_BASE	          (APB2PERIPH_BASE + 0x90000)
#define I2S_BASE	          (APB2PERIPH_BASE + 0xA0000)
#define SC1_BASE	          (APB2PERIPH_BASE + 0xB0000)
#define SC2_BASE	          (APB2PERIPH_BASE + 0xC0000)

#define GCR_BASE	          (AHBPERIPH_BASE + 0x00000)
#define CLK_BASE	          (AHBPERIPH_BASE + 0x00200)
#define INTID_BASE	          (AHBPERIPH_BASE + 0x00300)
#define GPIOA_BASE	          (AHBPERIPH_BASE + 0x04000)
#define GPIOB_BASE	          (AHBPERIPH_BASE + 0x04040)
#define GPIOC_BASE	          (AHBPERIPH_BASE + 0x04080)
#define GPIOD_BASE	          (AHBPERIPH_BASE + 0x040C0)
#define GPIOE_BASE	          (AHBPERIPH_BASE + 0x04100)
#define GPIOF_BASE	          (AHBPERIPH_BASE + 0x04140)
#define GPIODBNCE_BASE	      (AHBPERIPH_BASE + 0x04180)
#define GPIOBITA_BASE	      (AHBPERIPH_BASE + 0x04200)
#define GPIOBITB_BASE	      (AHBPERIPH_BASE + 0x04240)
#define GPIOBITC_BASE	      (AHBPERIPH_BASE + 0x04280)
#define GPIOBITD_BASE	      (AHBPERIPH_BASE + 0x042C0)
#define GPIOBITE_BASE	      (AHBPERIPH_BASE + 0x04300)
#define GPIOBITF_BASE	      (AHBPERIPH_BASE + 0x04340)
#define VDMA_BASE	          (AHBPERIPH_BASE + 0x08000)
#define PDMA1_BASE	          (AHBPERIPH_BASE + 0x08100)
#define PDMA2_BASE	          (AHBPERIPH_BASE + 0x08200)
#define PDMA3_BASE	          (AHBPERIPH_BASE + 0x08300)
#define PDMA4_BASE	          (AHBPERIPH_BASE + 0x08400)
#define PDMA5_BASE	          (AHBPERIPH_BASE + 0x08500)
#define PDMA6_BASE	          (AHBPERIPH_BASE + 0x08600)
#define PDMACRC_BASE          (AHBPERIPH_BASE + 0x08E00)
#define PDMAGCR_BASE	      (AHBPERIPH_BASE + 0x08F00)
#define FMC_BASE	          (AHBPERIPH_BASE + 0x0C000)
#define EBI_BASE	          (AHBPERIPH_BASE + 0x10000)

/*@}*/ /* end of group <Device>_MemoryMap */


/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
/** @addtogroup NANO1xx_PeripheralDecl NANO1xx Peripheral Declaration
  @{
*/
#define WDT                   ((WDT_TypeDef *) WDT_BASE)
#define WWDT                  ((WWDT_TypeDef *) WWDT_BASE)
#define RTC                   ((RTC_TypeDef *) RTC_BASE)
#define TIMER0                ((TIMER_TypeDef *) TIMER0_BASE)
#define TIMER1   	          ((TIMER_TypeDef *) TIMER1_BASE)
#define TIMER2                ((TIMER_TypeDef *) TIMER2_BASE)
#define TIMER3   	          ((TIMER_TypeDef *) TIMER3_BASE)
#define SHADOW                ((SHADOW_TypeDef *) SHADOW_BASE)
#define I2C0                  ((I2C_TypeDef *) I2C0_BASE)
#define I2C1                  ((I2C_TypeDef *) I2C1_BASE)
#define SPI0                  ((SPI_TypeDef *) SPI0_BASE)
#define SPI1                  ((SPI_TypeDef *) SPI1_BASE)
#define SPI2                  ((SPI_TypeDef *) SPI2_BASE)
#define PWM0                  ((PWM_TypeDef *) PWM0_BASE)
#define PWM1                  ((PWM_TypeDef *) PWM1_BASE)
#define UART0                 ((UART_TypeDef *) UART0_BASE)
#define UART1                 ((UART_TypeDef *) UART1_BASE)
#define USBD                  ((USBD_TypeDef *) USBD_BASE)
#define DAC                   ((DAC_TypeDef *) DAC_BASE)
#define LCD                   ((LCD_TypeDef *) LCD_BASE)
#define TK                    ((TK_TypeDef *) TK_BASE)
#define ADC                   ((ADC_TypeDef *) ADC_BASE)
#define SC0                    ((SC_TypeDef *) SC0_BASE)
#define I2S                   ((I2S_TypeDef *) I2S_BASE)
#define SC1                    ((SC_TypeDef *) SC1_BASE)
#define SC2                    ((SC_TypeDef *) SC2_BASE)


#define GCR                   ((GCR_TypeDef *) GCR_BASE)
#define CLK                   ((CLK_TypeDef *) CLK_BASE)
#define INTID                 ((INTID_TypeDef *) INTID_BASE)
#define GPIOA                 ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB                 ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC                 ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD                 ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE                 ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF                 ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIODBNCE             ((GPIODBNCE_TypeDef *) GPIODBNCE_BASE)
#define GPIOBITA              ((GPIOBIT_TypeDef *) GPIOBITA_BASE)
#define GPIOBITB              ((GPIOBIT_TypeDef *) GPIOBITB_BASE)
#define GPIOBITC              ((GPIOBIT_TypeDef *) GPIOBITC_BASE)
#define GPIOBITD              ((GPIOBIT_TypeDef *) GPIOBITD_BASE)
#define GPIOBITE              ((GPIOBIT_TypeDef *) GPIOBITE_BASE)
#define GPIOBITF              ((GPIOBIT_TypeDef *) GPIOBITF_BASE)
#define VDMA                  ((VDMA_TypeDef *) VDMA_BASE)
#define PDMA1                 ((PDMA_TypeDef *) PDMA1_BASE)
#define PDMA2                 ((PDMA_TypeDef *) PDMA2_BASE)
#define PDMA3                 ((PDMA_TypeDef *) PDMA3_BASE)
#define PDMA4                 ((PDMA_TypeDef *) PDMA4_BASE)
#define PDMA5                 ((PDMA_TypeDef *) PDMA5_BASE)
#define PDMA6                 ((PDMA_TypeDef *) PDMA6_BASE)
#define PDMACRC               ((PDMACRC_TypeDef *) PDMACRC_BASE)
#define PDMAGCR               ((PDMAGCR_TypeDef *) PDMAGCR_BASE)
#define FMC                   ((FMC_TypeDef *) FMC_BASE)
#define EBI                   ((EBI_TypeDef *) EBI_BASE)

/*@}*/ /* end of group NANO1xx_PeripheralDecl */


/******************************************************************************/
/*                Device Specific Constants                                   */
/******************************************************************************/
/** @addtogroup NANO1xx_Exported_Constants NANO1xx Exported Constants
  NANO1xx Device Specific Constantss
  @{
*/

/******************************************************************************/
/*                GCR                                                         */
/******************************************************************************/
/********************* Bit definition of RSTSRC register **********************/
#define GCR_RSTSRC_POR				((uint32_t)0x00000001)			/*!<Power on reset flag */
#define GCR_RSTSRC_RESET			((uint32_t)0x00000002)			/*!<RESET pin reset flag */
#define GCR_RSTSRC_WDT				((uint32_t)0x00000004)			/*!<Watchdog timer reset flag */
#define GCR_RSTSRC_BOD				((uint32_t)0x00000010)			/*!<BOD reset flag */
#define GCR_RSTSRC_SYS				((uint32_t)0x00000020)			/*!<System reset flag */
#define GCR_RSTSRC_CPU				((uint32_t)0x00000080)			/*!<CPU reset flag */

/********************* Bit definition of IPRSTCTL1 register **********************/
#define GCR_IPRSTCTL1_CHIP			((uint32_t)0x00000001)			/*!<Chip oneshot reset */
#define GCR_IPRSTCTL1_CPU			((uint32_t)0x00000002)			/*!<CPU oneshot reset */
#define GCR_IPRSTCTL1_DMA			((uint32_t)0x00000004)			/*!<DMA reset */
#define GCR_IPRSTCTL1_EBI			((uint32_t)0x00000008)			/*!<EBI reset */

/********************* Bit definition of IPRSTCTL2 register **********************/
#define GCR_IPRSTCTL2_GPIO			((uint32_t)0x00000002)			/*!<GPIO reset */
#define GCR_IPRSTCTL2_TMR0			((uint32_t)0x00000004)			/*!<Timer 0 reset */
#define GCR_IPRSTCTL2_TMR1			((uint32_t)0x00000008)			/*!<Timer 1 reset */
#define GCR_IPRSTCTL2_TMR2			((uint32_t)0x00000010)			/*!<Timer 2 reset */
#define GCR_IPRSTCTL2_TMR3			((uint32_t)0x00000020)			/*!<Timer 3 reset */
#define GCR_IPRSTCTL2_SC2			((uint32_t)0x00000080)			/*!<SC 2 reset */
#define GCR_IPRSTCTL2_I2C0			((uint32_t)0x00000100)			/*!<I2C 0 reset */
#define GCR_IPRSTCTL2_I2C1			((uint32_t)0x00000200)			/*!<I2C 1 reset */
#define GCR_IPRSTCTL2_SPI0			((uint32_t)0x00001000)			/*!<SPI 0 reset */
#define GCR_IPRSTCTL2_SPI1			((uint32_t)0x00002000)			/*!<SPI 1 reset */
#define GCR_IPRSTCTL2_SPI2			((uint32_t)0x00004000)			/*!<SPI 2 reset */
#define GCR_IPRSTCTL2_UART0			((uint32_t)0x00010000)			/*!<UART 0 reset */
#define GCR_IPRSTCTL2_UART1			((uint32_t)0x00020000)			/*!<UART 1 reset */
#define GCR_IPRSTCTL2_PWM0			((uint32_t)0x00100000)			/*!<PWM 0 reset */
#define GCR_IPRSTCTL2_PWM1			((uint32_t)0x00200000)			/*!<PWM 1 reset */
#define GCR_IPRSTCTL2_TK			((uint32_t)0x01000000)			/*!<TK reset */
#define GCR_IPRSTCTL2_DAC			((uint32_t)0x02000000)			/*!<DAC reset */
#define GCR_IPRSTCTL2_LCD			((uint32_t)0x04000000)			/*!<LCD reset */
#define GCR_IPRSTCTL2_USBD			((uint32_t)0x08000000)			/*!<USB device reset */
#define GCR_IPRSTCTL2_ADC			((uint32_t)0x10000000)			/*!<ADC reset */
#define GCR_IPRSTCTL2_I2S			((uint32_t)0x20000000)			/*!<I2S reset */
#define GCR_IPRSTCTL2_SC0			((uint32_t)0x40000000)			/*!<SC0 reset */
#define GCR_IPRSTCTL2_SC1			((uint32_t)0x80000000)			/*!<SC1 reset */

/********************* Bit definition of BODCTL register **********************/
#define GCR_BODCTL_17EN				((uint32_t)0x00000001)			/*!<Detector 1.7V Function Enable */
#define GCR_BODCTL_20EN				((uint32_t)0x00000002)			/*!<Detector 2.0V Function Enable */
#define GCR_BODCTL_25EN				((uint32_t)0x00000004)			/*!<Detector 2.5V Function Enable */
#define GCR_BODCTL_17RSTEN			((uint32_t)0x00000010)			/*!<1.7V Reset Enable */
#define GCR_BODCTL_20RSTEN			((uint32_t)0x00000020)			/*!<2.0V Reset Enable */
#define GCR_BODCTL_25RSTEN			((uint32_t)0x00000040)			/*!<2.5V Reset Enable */
#define GCR_BODCTL_17INTEN			((uint32_t)0x00000100)			/*!<1.7V Interrupt Enable */
#define GCR_BODCTL_20INTEN			((uint32_t)0x00000200)			/*!<2.0V Interrupt Enable */
#define GCR_BODCTL_25INTEN			((uint32_t)0x00000400)			/*!<2.5V Interrupt Enable */

/********************* Bit definition of VREFCTL register **********************/
#define GCR_VREFCTL_BGP_EN			((uint32_t)0x00000001)			/*!<Band-gap Enable */
#define GCR_VREFCTL_REG_EN			((uint32_t)0x00000002)			/*!<Regulator Enable */
#define GCR_VREFCTL_SEL25			((uint32_t)0x00000004)			/*!<Regulator Output Voltage 2.5V */
#define GCR_VREFCTL_SEL18			((uint32_t)0x00000000)			/*!<Regulator Output Voltage 1.8V */
#define GCR_VREFCTL_EXTMODE			((uint32_t)0x00000008)			/*!<Regulator External Mode */

/********************* Bit definition of IRCTRIMCTL register **********************/
#define GCR_IRCTRIMCTL_SEL_MASK		((uint32_t)0x00000003)			/*!<Trim frequency selection mask */
#define GCR_IRCTRIMCTL_TRIM_DISABLE	((uint32_t)0x00000000)			/*!<Disable auto-trim function */
#define GCR_IRCTRIMCTL_TRIM_11M		((uint32_t)0x00000001)			/*!<Trim HIRC to 11.0592 MHz */
#define GCR_IRCTRIMCTL_TRIM_12M		((uint32_t)0x00000002)			/*!<Trim HIRC to 12 MHz */
#define GCR_IRCTRIMCTL_TRIM_12288K	((uint32_t)0x00000003)			/*!<Trim HIRC to 12.288 MHz */

#define GCR_IRCTRIMCTL_LOOP_MASK	((uint32_t)0x00000030)			/*!<Trim calculation loop mask */
#define GCR_IRCTRIMCTL_LOOP_4CLK	((uint32_t)0x00000000)			/*!<Based on average difference in 4 x 32.768 kHz clock */
#define GCR_IRCTRIMCTL_LOOP_8CLK	((uint32_t)0x00000010)			/*!<Based on average difference in 8 x 32.768 kHz clock */
#define GCR_IRCTRIMCTL_LOOP_16CLK	((uint32_t)0x00000020)			/*!<Based on average difference in 16 x 32.768 kHz clock */
#define GCR_IRCTRIMCTL_LOOP_32CLK	((uint32_t)0x00000030)			/*!<Based on average difference in 32 x 32.768 kHz clock */

#define GCR_IRCTRIMCTL_RETRY_COUNT	((uint32_t)0x000000C0)			/*!<Trim value update limitation count mask */
#define GCR_IRCTRIMCTL_RETRY_64		((uint32_t)0x00000000)			/*!<Trim retry count limitation is 64 */
#define GCR_IRCTRIMCTL_RETRY_128	((uint32_t)0x00000040)			/*!<Trim retry count limitation is 128 */
#define GCR_IRCTRIMCTL_RETRY_256	((uint32_t)0x00000080)			/*!<Trim retry count limitation is 256 */
#define GCR_IRCTRIMCTL_RETRY_512	((uint32_t)0x000000C0)			/*!<Trim retry count limitation is 512 */

#define GCR_IRCTRIMCTL_CLKERR_STOP	((uint32_t)0x00000100)			/*!<Clock error stop enable */

/********************* Bit definition of IRCTRIMIEN register **********************/
#define GCR_IRCTRIMIEN_FAIL_EN		((uint32_t)0x00000002)			/*!<Trim failure interrupt enable */
#define GCR_IRCTRIMIEN_32KERR_EN	((uint32_t)0x00000004)			/*!<32.768 kHz Clock Error Interrupt Enable */

/********************* Bit definition of IRCTRIMINT register **********************/
#define GCR_IRCTRIMINT_FREQLOCK		((uint32_t)0x00000001)			/*!<HIRC frequency lock status */
#define GCR_IRCTRIMINT_FAIL_INT		((uint32_t)0x00000002)			/*!<Trim failure interrupt status */
#define GCR_IRCTRIMINT_32KERR_ENNT	((uint32_t)0x00000004)			/*!<32.768 kHz Clock Error Interrupt Status */

/********************* Bit definition of PA_L_MFP register **********************/
#define PA7_MFP_GPA7				((uint32_t)0x00000000)			/*!<PA7 Pin Function - GPIOA[7] */
#define PA7_MFP_ADC_CH7				((uint32_t)0x10000000)			/*!<PA7 Pin Function - ADC input channel 7 */
#define PA7_MFP_EBI_AD6				((uint32_t)0x20000000)			/*!<PA7 Pin Function - EBI AD[6] */
#define PA7_MFP_TMR2_CAP			((uint32_t)0x30000000)			/*!<PA7 Pin Function - Timer 2 capture event */
#define PA7_MFP_SC2_DAT				((uint32_t)0x40000000)			/*!<PA7 Pin Function - SmartCard 2 data pin */
#define PA7_MFP_PWM0_CH2			((uint32_t)0x50000000)			/*!<PA7 Pin Function - PWM0 Channel 2 */
#define PA7_MFP_LCD_S36				((uint32_t)0x70000000)			/*!<PA7 Pin Function - LCD SEG 36 */
#define PA7_MFP_LCD_S18				((uint32_t)0x70000000)			/*!<PA7 Pin Function - LCD SEG 18 */
#define PA7_MFP_MASK				((uint32_t)0x70000000)			/*!<PA7 Pin Function Mask */

#define PA6_MFP_GPA6				((uint32_t)0x00000000)			/*!<PA6 Pin Function - GPIOA[6] */
#define PA6_MFP_ADC_CH6				((uint32_t)0x01000000)			/*!<PA6 Pin Function - ADC input channel 6 */
#define PA6_MFP_EBI_AD7				((uint32_t)0x02000000)			/*!<PA6 Pin Function - EBI AD[7] */
#define PA6_MFP_TMR3_CAP			((uint32_t)0x03000000)			/*!<PA6 Pin Function - Timer 3 Capture event */
#define PA6_MFP_SC2_CLK				((uint32_t)0x04000000)			/*!<PA6 Pin Function - SmartCard 2 clock */
#define PA6_MFP_PWM0_CH3			((uint32_t)0x05000000)			/*!<PA6 Pin Function - PWM0 Channel 3 */
#define PA6_MFP_LCD_S37				((uint32_t)0x07000000)			/*!<PA6 Pin Function - LCD SEG 37 */
#define PA6_MFP_LCD_S19				((uint32_t)0x07000000)			/*!<PA6 Pin Function - LCD SEG 19 */
#define PA6_MFP_MASK				((uint32_t)0x07000000)			/*!<PA6 Pin Function Mask */

#define PA5_MFP_GPA5				((uint32_t)0x00000000)			/*!<PA5 Pin Function - GPIOA[5] */
#define PA5_MFP_ADC_CH5				((uint32_t)0x00100000)			/*!<PA5 Pin Function - ADC input channel 5 */
#define PA5_MFP_EBI_AD8				((uint32_t)0x00200000)			/*!<PA5 Pin Function - EBI AD[8] */
#define PA5_MFP_SC2_RST				((uint32_t)0x00400000)			/*!<PA5 Pin Function - SmartCard2 RST */
#define PA5_MFP_I2C0_SCL			((uint32_t)0x00500000)			/*!<PA5 Pin Function - I2C0 clock */
#define PA5_MFP_LCD_S38				((uint32_t)0x00700000)			/*!<PA5 Pin Function - LCD SEG 38 */
#define PA5_MFP_LCD_S20				((uint32_t)0x00700000)			/*!<PA5 Pin Function - LCD SEG 20 */
#define PA5_MFP_MASK				((uint32_t)0x00700000)			/*!<PA5 Pin Function Mask */

#define PA4_MFP_GPA4				((uint32_t)0x00000000)			/*!<PA4 Pin Function - GPIOA[4] */
#define PA4_MFP_ADC_CH4				((uint32_t)0x00010000)			/*!<PA4 Pin Function - ADC input channel 4 */
#define PA4_MFP_EBI_AD9				((uint32_t)0x00020000)			/*!<PA4 Pin Function - EBI AD[9] */
#define PA4_MFP_SC2_PWR				((uint32_t)0x00040000)			/*!<PA4 Pin Function - SmartCard 2 power */
#define PA4_MFP_I2C0_SDA			((uint32_t)0x00050000)			/*!<PA4 Pin Function - I2C0 DATA */
#define PA4_MFP_LCD_S39				((uint32_t)0x00070000)			/*!<PA4 Pin Function - LCD SEG 39 */
#define PA4_MFP_LCD_S21				((uint32_t)0x00070000)			/*!<PA4 Pin Function - LCD SEG 21 */
#define PA4_MFP_MASK				((uint32_t)0x00070000)			/*!<PA4 Pin Function Mask */

#define PA3_MFP_GPA3				((uint32_t)0x00000000)			/*!<PA3 Pin Function - GPIOA[3] */
#define PA3_MFP_ADC_CH3				((uint32_t)0x00001000)			/*!<PA3 Pin Function - ADC input channel 3 */
#define PA3_MFP_EBI_AD10			((uint32_t)0x00002000)			/*!<PA3 Pin Function - EBI AD[10] */
#define PA3_MFP_UART1_RX			((uint32_t)0x00005000)			/*!<PA3 Pin Function - UART 1 RX */
#define PA3_MFP_LCD_S22				((uint32_t)0x00007000)			/*!<PA3 Pin Function - LCD SEG 22 */
#define PA3_MFP_MASK				((uint32_t)0x00007000)			/*!<PA3 Pin Function Mask */

#define PA2_MFP_GPA2				((uint32_t)0x00000000)			/*!<PA2 Pin Function - GPIOA[2] */
#define PA2_MFP_ADC_CH2				((uint32_t)0x00000100)			/*!<PA2 Pin Function - ADC input channel 2 */
#define PA2_MFP_EBI_AD11			((uint32_t)0x00000200)			/*!<PA2 Pin Function - EBI AD[11] */
#define PA2_MFP_UART1_TX			((uint32_t)0x00000500)			/*!<PA2 Pin Function - UART1 TX */
#define PA2_MFP_LCD_S23				((uint32_t)0x00000700)			/*!<PA2 Pin Function - LCD SEG 23 */
#define PA2_MFP_MASK				((uint32_t)0x00000700)			/*!<PA2 Pin Function Mask */

#define PA1_MFP_GPA1				((uint32_t)0x00000000)			/*!<PA1 Pin Function - GPIOA[1] */
#define PA1_MFP_ADC_CH1				((uint32_t)0x00000010)			/*!<PA1 Pin Function - ADC input channel 1 */
#define PA1_MFP_EBI_AD12			((uint32_t)0x00000020)			/*!<PA1 Pin Function - EBI AD[12] */
#define PA1_MFP_TK9					((uint32_t)0x00000060)			/*!<PA1 Pin Function - Touch key 9 */
#define PA1_MFP_MASK				((uint32_t)0x00000070)			/*!<PA1 Pin Function Mask */

#define PA0_MFP_GPA0				((uint32_t)0x00000000)			/*!<PA0 Pin Function - GPIOA[0] */
#define PA0_MFP_ADC_CH0				((uint32_t)0x00000001)			/*!<PA0 Pin Function - ADC input channel 0 */
#define PA0_MFP_SC2_CD				((uint32_t)0x00000004)			/*!<PA0 Pin Function - SmartCard 2 card detect */
#define PA0_MFP_TK8					((uint32_t)0x00000006)			/*!<PA0 Pin Function - Touch key 8 */
#define PA0_MFP_MASK				((uint32_t)0x00000007)			/*!<PA0 Pin Function Mask */

/********************* Bit definition of PA_H_MFP register **********************/
#define PA15_MFP_GPA15				((uint32_t)0x00000000)			/*!<PA15 Pin Function - GPIOA[15] */
#define PA15_MFP_PWM0_CH3			((uint32_t)0x10000000)			/*!<PA15 Pin Function - PWM0 Channel 3 */
#define PA15_MFP_I2S_MCLK			((uint32_t)0x20000000)			/*!<PA15 Pin Function - I2S MCLK */
#define PA15_MFP_TMR3_CAP			((uint32_t)0x30000000)			/*!<PA15 Pin Function - Timer3 capture event */
#define PA15_MFP_SC0_PWR			((uint32_t)0x40000000)			/*!<PA15 Pin Function - SmartCard 0 power */
#define PA15_MFP_UART0_TX			((uint32_t)0x60000000)			/*!<PA15 Pin Function - UART0 TX */
#define PA15_MFP_LCD_S27			((uint32_t)0x70000000)			/*!<PA15 Pin Function - LCD SEG 27 */
#define PA15_MFP_MASK				((uint32_t)0x70000000)			/*!<PA15 Pin Function Mask */

#define PA14_MFP_GPA14				((uint32_t)0x00000000)			/*!<PA14 Pin Function - GPIOA[14] */
#define PA14_MFP_PWM0_CH2			((uint32_t)0x01000000)			/*!<PA14 Pin Function - PWM0 Channel 2 */
#define PA14_MFP_EBI_AD15			((uint32_t)0x02000000)			/*!<PA14 Pin Function - EBI AD[15] */
#define PA14_MFP_TMR2_CAP			((uint32_t)0x03000000)			/*!<PA14 Pin Function - Timer2 capture event */
#define PA14_MFP_UART0_RX			((uint32_t)0x06000000)			/*!<PA14 Pin Function - UART0 RX */
#define PA14_MFP_LCD_S26			((uint32_t)0x07000000)			/*!<PA14 Pin Function - LCD SEG 26 */
#define PA14_MFP_MASK				((uint32_t)0x07000000)			/*!<PA14 Pin Function Mask */

#define PA13_MFP_GPA13				((uint32_t)0x00000000)			/*!<PA13 Pin Function - GPIOA[13] */
#define PA13_MFP_PWM0_CH1			((uint32_t)0x00100000)			/*!<PA13 Pin Function - PWM0 Channel 1 */
#define PA13_MFP_EBI_AD14			((uint32_t)0x00200000)			/*!<PA13 Pin Function - EBI AD[14] */
#define PA13_MFP_TMR1_CAP			((uint32_t)0x00300000)			/*!<PA13 Pin Function - Timer1 capture event */
#define PA13_MFP_I2C0_SCL			((uint32_t)0x00500000)			/*!<PA13 Pin Function - I2C0 clock */
#define PA13_MFP_TK11				((uint32_t)0x00600000)			/*!<PA13 Pin Function - Touch key 11 */
#define PA13_MFP_TK9				((uint32_t)0x00600000)			/*!<PA13 Pin Function - Touch key 9 */
#define PA13_MFP_LCD_S25			((uint32_t)0x00700000)			/*!<PA13 Pin Function - LCD SEG 25 */
#define PA13_MFP_MASK				((uint32_t)0x00700000)			/*!<PA13 Pin Function Mask */

#define PA12_MFP_GPA12				((uint32_t)0x00000000)			/*!<PA12 Pin Function - GPIOA[12] */
#define PA12_MFP_PWM0_CH0			((uint32_t)0x00010000)			/*!<PA12 Pin Function - PWM0 Channel 0 */
#define PA12_MFP_EBI_AD13			((uint32_t)0x00020000)			/*!<PA12 Pin Function - EBI AD[13] */
#define PA12_MFP_TMR0_CAP			((uint32_t)0x00030000)			/*!<PA12 Pin Function - Timer0 capture event */
#define PA12_MFP_I2C0_SDA			((uint32_t)0x00050000)			/*!<PA12 Pin Function - I2C0 DATA */
#define PA12_MFP_TK10				((uint32_t)0x00060000)			/*!<PA12 Pin Function - Touch key 10 */
#define PA12_MFP_LCD_S24			((uint32_t)0x00070000)			/*!<PA12 Pin Function - LCD SEG 24 */
#define PA12_MFP_MASK				((uint32_t)0x00070000)			/*!<PA12 Pin Function Mask */

#define PA11_MFP_GPA11				((uint32_t)0x00000000)			/*!<PA11 Pin Function - GPIOA[11] */
#define PA11_MFP_I2C1_SCL			((uint32_t)0x00001000)			/*!<PA11 Pin Function - I2C1 clock */
#define PA11_MFP_EBI_NRE			((uint32_t)0x00002000)			/*!<PA11 Pin Function - EBI nRE */
#define PA11_MFP_SC0_RST			((uint32_t)0x00003000)			/*!<PA11 Pin Function - SmartCard0 RST */
#define PA11_MFP_SPI2_MOSI0			((uint32_t)0x00004000)			/*!<PA11 Pin Function - SPI2 MOSI[0] */
#define PA11_MFP_LCD_S23			((uint32_t)0x00007000)			/*!<PA11 Pin Function - LCD SEG 23 */
#define PA11_MFP_LCD_S9				((uint32_t)0x00007000)			/*!<PA11 Pin Function - LCD SEG 9 */
#define PA11_MFP_MASK				((uint32_t)0x00007000)			/*!<PA11 Pin Function Mask */

#define PA10_MFP_GPA10				((uint32_t)0x00000000)			/*!<PA10 Pin Function - GPIOA[10] */
#define PA10_MFP_I2C1_SDA			((uint32_t)0x00000100)			/*!<PA10 Pin Function - I2C1 DATA */
#define PA10_MFP_EBI_NWE			((uint32_t)0x00000200)			/*!<PA10 Pin Function - EBI nWE */
#define PA10_MFP_SC0_PWR			((uint32_t)0x00000300)			/*!<PA10 Pin Function - SmartCard0 Power */
#define PA10_MFP_SPI2_MISO0			((uint32_t)0x00000400)			/*!<PA10 Pin Function - SPI2 MISO[0] */
#define PA10_MFP_LCD_S22			((uint32_t)0x00000700)			/*!<PA10 Pin Function - LCD SEG 22 */
#define PA10_MFP_LCD_S8				((uint32_t)0x00000700)			/*!<PA10 Pin Function - LCD SEG 8 */
#define PA10_MFP_MASK				((uint32_t)0x00000700)			/*!<PA10 Pin Function Mask */

#define PA9_MFP_GPA9				((uint32_t)0x00000000)			/*!<PA9 Pin Function - GPIOA[9] */
#define PA9_MFP_I2C0_SCL			((uint32_t)0x00000010)			/*!<PA9 Pin Function - I2C0 clock */
#define PA9_MFP_SC0_DAT				((uint32_t)0x00000030)			/*!<PA9 Pin Function - SmartCard0 DATA */
#define PA9_MFP_SPI2_SCLK			((uint32_t)0x00000040)			/*!<PA9 Pin Function - SPI2 SCLK */
#define PA9_MFP_LCD_S21				((uint32_t)0x00000070)			/*!<PA9 Pin Function - LCD SEG 21 */
#define PA9_MFP_LCD_S7				((uint32_t)0x00000070)			/*!<PA9 Pin Function - LCD SEG 7 */
#define PA9_MFP_MASK				((uint32_t)0x00000070)			/*!<PA9 Pin Function Mask */

#define PA8_MFP_GPA8				((uint32_t)0x00000000)			/*!<PA8 Pin Function - GPIOA[8] */
#define PA8_MFP_I2C0_SDA			((uint32_t)0x00000001)			/*!<PA8 Pin Function - I2C0 DATA */
#define PA8_MFP_SC0_CLK				((uint32_t)0x00000003)			/*!<PA8 Pin Function - SmartCard0 clock */
#define PA8_MFP_SPI2_SS0			((uint32_t)0x00000004)			/*!<PA8 Pin Function - SPI2 1st chip selection */
#define PA8_MFP_LCD_S20				((uint32_t)0x00000007)			/*!<PA8 Pin Function - LCD SEG 20 */
#define PA8_MFP_LCD_S6				((uint32_t)0x00000007)			/*!<PA8 Pin Function - LCD SEG 6 */
#define PA8_MFP_MASK				((uint32_t)0x00000007)			/*!<PA8 Pin Function Mask */

/********************* Bit definition of PB_L_MFP register **********************/
#define PB7_MFP_GPB7				((uint32_t)0x00000000)			/*!<PB7 Pin Function - GPIOB[7] */
#define PB7_MFP_UART1_CTS			((uint32_t)0x10000000)			/*!<PB7 Pin Function - UART1 CTSn */
#define PB7_MFP_EBI_NCS				((uint32_t)0x20000000)			/*!<PB7 Pin Function - EBI nCS */
#define PB7_MFP_SPI2_MOSI0			((uint32_t)0x40000000)			/*!<PB7 Pin Function - SPI2 MOSI[0] */
#define PB7_MFP_LCD_S10				((uint32_t)0x70000000)			/*!<PB7 Pin Function - LCD SEG 10 */
#define PB7_MFP_LCD_S2				((uint32_t)0x70000000)			/*!<PB7 Pin Function - LCD SEG 2 */
#define PB7_MFP_MASK				((uint32_t)0x70000000)			/*!<PB7 Pin Function Mask */

#define PB6_MFP_GPB6				((uint32_t)0x00000000)			/*!<PB6 Pin Function - GPIOB[6] */
#define PB6_MFP_UART1_RTS			((uint32_t)0x01000000)			/*!<PB6 Pin Function - UART1 RTSn */
#define PB6_MFP_EBI_ALE				((uint32_t)0x02000000)			/*!<PB6 Pin Function - EBI ALE */
#define PB6_MFP_SPI2_MISO0			((uint32_t)0x04000000)			/*!<PB6 Pin Function - SPI2 MISO[0] */
#define PB6_MFP_LCD_S11				((uint32_t)0x07000000)			/*!<PB6 Pin Function - LCD SEG 11 */
#define PB6_MFP_LCD_S3				((uint32_t)0x07000000)			/*!<PB6 Pin Function - LCD SEG 3 */
#define PB6_MFP_MASK				((uint32_t)0x07000000)			/*!<PB6 Pin Function Mask */

#define PB5_MFP_GPB5				((uint32_t)0x00000000)			/*!<PB5 Pin Function - GPIOB[5] */
#define PB5_MFP_UART1_TX			((uint32_t)0x00100000)			/*!<PB5 Pin Function - UART1 TX */
#define PB5_MFP_SC0_RST				((uint32_t)0x00300000)			/*!<PB5 Pin Function - SmartCard0 RST */
#define PB5_MFP_SPI2_SCLK			((uint32_t)0x00400000)			/*!<PB5 Pin Function - SPI2 SCLK */
#define PB5_MFP_LCD_S12				((uint32_t)0x00700000)			/*!<PB5 Pin Function - LCD SEG 12 */
#define PB5_MFP_LCD_S4				((uint32_t)0x00700000)			/*!<PB5 Pin Function - LCD SEG 4 */
#define PB5_MFP_MASK				((uint32_t)0x00700000)			/*!<PB5 Pin Function Mask */

#define PB4_MFP_GPB4				((uint32_t)0x00000000)			/*!<PB4 Pin Function - GPIOB[4] */
#define PB4_MFP_UART1_RX			((uint32_t)0x00010000)			/*!<PB4 Pin Function - UART1 RX */
#define PB4_MFP_SC0_CD				((uint32_t)0x00030000)			/*!<PB4 Pin Function - SmartCard0 card detection */
#define PB4_MFP_SPI2_SS0			((uint32_t)0x00040000)			/*!<PB4 Pin Function - SPI2 chip selection 0 */
#define PB4_MFP_LCD_S13				((uint32_t)0x00070000)			/*!<PB4 Pin Function - LCD SEG 13 */
#define PB4_MFP_LCD_S5				((uint32_t)0x00070000)			/*!<PB4 Pin Function - LCD SEG 5 */
#define PB4_MFP_MASK				((uint32_t)0x00070000)			/*!<PB4 Pin Function Mask */

#define PB3_MFP_GPB3				((uint32_t)0x00000000)			/*!<PB3 Pin Function - GPIOB[3] */
#define PB3_MFP_UART0_CTS			((uint32_t)0x00001000)			/*!<PB3 Pin Function - UART0 CTSn */
#define PB3_MFP_EBI_NWRH			((uint32_t)0x00002000)			/*!<PB3 Pin Function - EBI nWRH */
#define PB3_MFP_SPI1_SS0			((uint32_t)0x00003000)			/*!<PB3 Pin Function - SPI1 chip selection 0 */
#define PB3_MFP_LCD_S4				((uint32_t)0x00007000)			/*!<PB3 Pin Function - LCD SEG 4 */
#define PB3_MFP_LCD_COM2			((uint32_t)0x00007000)			/*!<PB3 Pin Function - LCD COM 2 */
#define PB3_MFP_MASK				((uint32_t)0x00007000)			/*!<PB3 Pin Function Mask */

#define PB2_MFP_GPB2				((uint32_t)0x00000000)			/*!<PB2 Pin Function - GPIOB[2] */
#define PB2_MFP_UART0_RTS			((uint32_t)0x00000100)			/*!<PB2 Pin Function - UART0 RTSn */
#define PB2_MFP_EBI_NWRL			((uint32_t)0x00000200)			/*!<PB2 Pin Function - EBI nWRL */
#define PB2_MFP_SPI1_SCLK			((uint32_t)0x00000300)			/*!<PB2 Pin Function - SPI1 SCLK */
#define PB2_MFP_LCD_S5				((uint32_t)0x00000700)			/*!<PB2 Pin Function - LCD SEG 5 */
#define PB2_MFP_LCD_COM3			((uint32_t)0x00000700)			/*!<PB2 Pin Function - LCD COM 3 */
#define PB2_MFP_MASK				((uint32_t)0x00000700)			/*!<PB2 Pin Function Mask */

#define PB1_MFP_GPB1				((uint32_t)0x00000000)			/*!<PB1 Pin Function - GPIOB[1] */
#define PB1_MFP_UART0_TX			((uint32_t)0x00000010)			/*!<PB1 Pin Function - UART0 TX */
#define PB1_MFP_SPI1_MISO0			((uint32_t)0x00000030)			/*!<PB1 Pin Function - SPI1 MISO[0] */
#define PB1_MFP_LCD_S6				((uint32_t)0x00000070)			/*!<PB1 Pin Function - LCD SEG 6 */
#define PB1_MFP_LCD_S0				((uint32_t)0x00000070)			/*!<PB1 Pin Function - LCD SEG 0 */
#define PB1_MFP_MASK				((uint32_t)0x00000070)			/*!<PB1 Pin Function Mask */

#define PB0_MFP_GPB0				((uint32_t)0x00000000)			/*!<PB0 Pin Function - GPIOB[0] */
#define PB0_MFP_UART0_RX			((uint32_t)0x00000001)			/*!<PB0 Pin Function - UART0 RX */
#define PB0_MFP_SPI1_MOSI0			((uint32_t)0x00000003)			/*!<PB0 Pin Function - SPI1 MOSI[0] */
#define PB0_MFP_LCD_S7				((uint32_t)0x00000007)			/*!<PB0 Pin Function - LCD SEG 7 */
#define PB0_MFP_LCD_S1				((uint32_t)0x00000007)			/*!<PB0 Pin Function - LCD SEG 1 */
#define PB0_MFP_MASK				((uint32_t)0x00000007)			/*!<PB0 Pin Function Mask */

/********************* Bit definition of PB_H_MFP register **********************/
#define PB15_MFP_GPB15				((uint32_t)0x00000000)			/*!<PB15 Pin Function - GPIOB[15] */
#define PB15_MFP_EXT_INT1			((uint32_t)0x10000000)			/*!<PB15 Pin Function - External interrupt 1 */
#define PB15_MFP_SNOOPER			((uint32_t)0x30000000)			/*!<PB15 Pin Function - Snooper pin */
#define PB15_MFP_SC1_CD				((uint32_t)0x40000000)			/*!<PB15 Pin Function - SmartCard1 card detect */
#define PB15_MFP_LCD_S31			((uint32_t)0x70000000)			/*!<PB15 Pin Function - LCD SEG 31 */
#define PB15_MFP_LCD_S14			((uint32_t)0x70000000)			/*!<PB15 Pin Function - LCD SEG 14 */
#define PB15_MFP_MASK				((uint32_t)0x70000000)			/*!<PB15 Pin Function Mask */

#define PB14_MFP_GPB14				((uint32_t)0x00000000)			/*!<PB14 Pin Function - GPIOB[14] */
#define PB14_MFP_EXT_INT0			((uint32_t)0x01000000)			/*!<PB14 Pin Function - External interrupt 0 */
#define PB14_MFP_SC2_CD				((uint32_t)0x03000000)			/*!<PB14 Pin Function - SmartCard 2 card detect */
#define PB14_MFP_SPI2_SS1			((uint32_t)0x04000000)			/*!<PB14 Pin Function - SPI2 2nd chip selection */
#define PB14_MFP_LCD_S26			((uint32_t)0x07000000)			/*!<PB14 Pin Function - LCD SEG 26 */
#define PB14_MFP_LCD_S12			((uint32_t)0x07000000)			/*!<PB14 Pin Function - LCD SEG 12 */
#define PB14_MFP_MASK				((uint32_t)0x07000000)			/*!<PB14 Pin Function Mask */

#define PB13_MFP_GPB13				((uint32_t)0x00000000)			/*!<PB13 Pin Function - GPIOB[13] */
#define PB13_MFP_EBI_AD1			((uint32_t)0x00200000)			/*!<PB13 Pin Function - EBI AD[1] */
#define PB13_MFP_LCD_S25			((uint32_t)0x00700000)			/*!<PB13 Pin Function - LCD SEG 25 */
#define PB13_MFP_LCD_S11			((uint32_t)0x00700000)			/*!<PB13 Pin Function - LCD SEG 11 */
#define PB13_MFP_MASK				((uint32_t)0x00700000)			/*!<PB13 Pin Function Mask */

#define PB12_MFP_GPB12				((uint32_t)0x00000000)			/*!<PB12 Pin Function - GPIOB[12] */
#define PB12_MFP_EBI_AD0			((uint32_t)0x00020000)			/*!<PB12 Pin Function - EBI AD[0] */
#define PB12_MFP_FRQDIV_CLK			((uint32_t)0x00040000)			/*!<PB12 Pin Function - FRQDIV_CLK */
#define PB12_MFP_LCD_S24			((uint32_t)0x00070000)			/*!<PB12 Pin Function - LCD SEG 24 */
#define PB12_MFP_LCD_S10			((uint32_t)0x00070000)			/*!<PB12 Pin Function - LCD SEG 10 */
#define PB12_MFP_MASK				((uint32_t)0x00070000)			/*!<PB12 Pin Function Mask */

#define PB11_MFP_GPB11				((uint32_t)0x00000000)			/*!<PB11 Pin Function - GPIOB[11] */
#define PB11_MFP_PWM1_CH0			((uint32_t)0x00001000)			/*!<PB11 Pin Function - PWM1 Channel 0 */
#define PB11_MFP_TMR3_EXT			((uint32_t)0x00002000)			/*!<PB11 Pin Function - Timer3 external event input */
#define PB11_MFP_TMR3_TOGGLE_OUT	((uint32_t)0x00002000)			/*!<PB11 Pin Function - Timer3 toggle output */
#define PB11_MFP_SC2_DAT			((uint32_t)0x00004000)			/*!<PB11 Pin Function - SmartCard2 DATA */
#define PB11_MFP_SPI0_MISO0			((uint32_t)0x00005000)			/*!<PB11 Pin Function - SPI 0 MISO[0] */
#define PB11_MFP_LCD_V3				((uint32_t)0x00007000)			/*!<PB11 Pin Function - LCD V3 */
#define PB11_MFP_MASK				((uint32_t)0x00007000)			/*!<PB11 Pin Function Mask */

#define PB10_MFP_GPB10				((uint32_t)0x00000000)			/*!<PB10 Pin Function - GPIOB[10] */
#define PB10_MFP_SPI0_SS1			((uint32_t)0x00000100)			/*!<PB10 Pin Function - SPI0 chip selection 1 */
#define PB10_MFP_TMR2_EXT			((uint32_t)0x00000200)			/*!<PB10 Pin Function - Timer2 external event input */
#define PB10_MFP_TMR2_TOGGLE_OUT	((uint32_t)0x00000200)			/*!<PB10 Pin Function - Timer2 toggle output */
#define PB10_MFP_SC2_CLK			((uint32_t)0x00000400)			/*!<PB10 Pin Function - SmartCard2 clock */
#define PB10_MFP_SPI0_MOSI0			((uint32_t)0x00000500)			/*!<PB10 Pin Function - SPI0 MOSI[0] */
#define PB10_MFP_LCD_V2				((uint32_t)0x00000700)			/*!<PB10 Pin Function - LCD V2 */
#define PB10_MFP_MASK				((uint32_t)0x00000700)			/*!<PB10 Pin Function Mask */

#define PB9_MFP_GPB9				((uint32_t)0x00000000)			/*!<PB9 Pin Function - GPIOB[9] */
#define PB9_MFP_SPI1_SS1			((uint32_t)0x00000010)			/*!<PB9 Pin Function - SPI1 chip selection 1 */
#define PB9_MFP_TMR1_EXT			((uint32_t)0x00000020)			/*!<PB9 Pin Function - Timer1 external event input */
#define PB9_MFP_TMR1_TOGGLE_OUT		((uint32_t)0x00000020)			/*!<PB9 Pin Function - Timer1 toggle output */
#define PB9_MFP_SC2_RST				((uint32_t)0x00000040)			/*!<PB9 Pin Function - SmartCard2 RST */
#define PB9_MFP_EXT_INT0			((uint32_t)0x00000050)			/*!<PB9 Pin Function - External interrupt 0 */
#define PB9_MFP_LCD_V1				((uint32_t)0x00000070)			/*!<PB9 Pin Function - LCD V1 */
#define PB9_MFP_MASK				((uint32_t)0x00000070)			/*!<PB9 Pin Function Mask */

#define PB8_MFP_GPB8				((uint32_t)0x00000000)			/*!<PB8 Pin Function - GPIOB[8] */
#define PB8_MFP_ADC_EXT				((uint32_t)0x00000001)			/*!<PB8 Pin Function - ADC external trigger */
#define PB8_MFP_TMR0_EXT			((uint32_t)0x00000002)			/*!<PB8 Pin Function - Timer0 external event input */
#define PB8_MFP_TMR0_TOGGLE_OUT		((uint32_t)0x00000002)			/*!<PB8 Pin Function - Timer0 toggle output */
#define PB8_MFP_EXT_INT0			((uint32_t)0x00000003)			/*!<PB8 Pin Function - External interrupt 0 */
#define PB8_MFP_SC2_PWR				((uint32_t)0x00000004)			/*!<PB8 Pin Function - SmartCard 2 power */
#define PB8_MFP_LCD_S30				((uint32_t)0x00000007)			/*!<PB8 Pin Function - LCD SEG 30 */
#define PB8_MFP_LCD_S13				((uint32_t)0x00000007)			/*!<PB8 Pin Function - LCD SEG 13 */
#define PB8_MFP_MASK				((uint32_t)0x00000007)			/*!<PB8 Pin Function Mask */

/********************* Bit definition of PC_L_MFP register **********************/
#define PC7_MFP_GPC7				((uint32_t)0x00000000)			/*!<PC7 Pin Function - GPIOC[7] */
#define PC7_MFP_DA_OUT1				((uint32_t)0x10000000)			/*!<PC7 Pin Function - DA out1 */
#define PC7_MFP_EBI_AD5				((uint32_t)0x20000000)			/*!<PC7 Pin Function - EBI AD[5] */
#define PC7_MFP_TMR1_CAP			((uint32_t)0x30000000)			/*!<PC7 Pin Function - Timer1 capture event */
#define PC7_MFP_PWM0_CH1			((uint32_t)0x50000000)			/*!<PC7 Pin Function - PWM0 Channel 1 */
#define PC7_MFP_LCD_S17				((uint32_t)0x70000000)			/*!<PC7 Pin Function - LCD SEG 17 */
#define PC7_MFP_MASK				((uint32_t)0x70000000)			/*!<PC7 Pin Function Mask */

#define PC6_MFP_GPC6				((uint32_t)0x00000000)			/*!<PC6 Pin Function - GPIOC[6] */
#define PC6_MFP_DA_OUT0				((uint32_t)0x01000000)			/*!<PC6 Pin Function - DA out0 */
#define PC6_MFP_EBI_AD4				((uint32_t)0x02000000)			/*!<PC6 Pin Function - EBI AD[4] */
#define PC6_MFP_TMR0_CAP			((uint32_t)0x03000000)			/*!<PC6 Pin Function - Timer0 Capture event */
#define PC6_MFP_SC1_CD				((uint32_t)0x04000000)			/*!<PC6 Pin Function - SmartCard1 card detection */
#define PC6_MFP_PWM0_CH0			((uint32_t)0x05000000)			/*!<PC6 Pin Function - PWM0 Channel 0 */
#define PC6_MFP_MASK				((uint32_t)0x07000000)			/*!<PC6 Pin Function Mask */

#define PC5_MFP_GPC5				((uint32_t)0x00000000)			/*!<PC5 Pin Function - GPIOC[5] */
#define PC5_MFP_SPI0_MOSI1			((uint32_t)0x00100000)			/*!<PC5 Pin Function - SPI0 MOSI[1] */
#define PC5_MFP_LCD_COM3			((uint32_t)0x00700000)			/*!<PC5 Pin Function - LCD COM 3 */
#define PC5_MFP_MASK				((uint32_t)0x00700000)			/*!<PC5 Pin Function Mask */

#define PC4_MFP_GPC4				((uint32_t)0x00000000)			/*!<PC4 Pin Function - GPIOC[4] */
#define PC4_MFP_SPI0_MISO1			((uint32_t)0x00010000)			/*!<PC4 Pin Function - SPI0 MISO[1] */
#define PC4_MFP_LCD_COM2			((uint32_t)0x00070000)			/*!<PC4 Pin Function - LCD COM 2 */
#define PC4_MFP_MASK				((uint32_t)0x00070000)			/*!<PC4 Pin Function Mask */

#define PC3_MFP_GPC3				((uint32_t)0x00000000)			/*!<PC3 Pin Function - GPIOC[3] */
#define PC3_MFP_SPI0_MOSI0			((uint32_t)0x00001000)			/*!<PC3 Pin Function - SPI0 MOSI[0] */
#define PC3_MFP_I2S_DOUT			((uint32_t)0x00002000)			/*!<PC3 Pin Function - I2S Dout */
#define PC3_MFP_SC1_RST				((uint32_t)0x00004000)			/*!<PC3 Pin Function - SmartCard1 RST */
#define PC3_MFP_LCD_COM1			((uint32_t)0x00007000)			/*!<PC3 Pin Function - LCD COM 1 */
#define PC3_MFP_MASK				((uint32_t)0x00007000)			/*!<PC3 Pin Function Mask */

#define PC2_MFP_GPC2				((uint32_t)0x00000000)			/*!<PC2 Pin Function - GPIOC[2] */
#define PC2_MFP_SPI0_MISO0			((uint32_t)0x00000100)			/*!<PC2 Pin Function - SPI0 MISO[0] */
#define PC2_MFP_I2S_DIN				((uint32_t)0x00000200)			/*!<PC2 Pin Function - I2S Din */
#define PC2_MFP_SC1_PWR				((uint32_t)0x00000400)			/*!<PC2 Pin Function - SmartCard1 Power */
#define PC2_MFP_LCD_COM0			((uint32_t)0x00000700)			/*!<PC2 Pin Function - LCD COM 0 */
#define PC2_MFP_MASK				((uint32_t)0x00000700)			/*!<PC2 Pin Function Mask */

#define PC1_MFP_GPC1				((uint32_t)0x00000000)			/*!<PC1 Pin Function - GPIOC[1] */
#define PC1_MFP_SPI0_SCLK			((uint32_t)0x00000010)			/*!<PC1 Pin Function - SPI0 SCLK */
#define PC1_MFP_I2S_BCLK			((uint32_t)0x00000020)			/*!<PC1 Pin Function - I2S BCLK */
#define PC1_MFP_SC1_DAT				((uint32_t)0x00000040)			/*!<PC1 Pin Function - SmartCard1 DATA */
#define PC1_MFP_LCD_DH2				((uint32_t)0x00000070)			/*!<PC1 Pin Function - LCD DH2 */
#define PC1_MFP_MASK				((uint32_t)0x00000070)			/*!<PC1 Pin Function Mask */

#define PC0_MFP_GPC0				((uint32_t)0x00000000)			/*!<PC0 Pin Function - GPIOC[0] */
#define PC0_MFP_SPI0_SS0			((uint32_t)0x00000001)			/*!<PC0 Pin Function - SPI0 chip selection 0 */
#define PC0_MFP_I2S_WS				((uint32_t)0x00000002)			/*!<PC0 Pin Function - I2S WS */
#define PC0_MFP_SC1_CLK				((uint32_t)0x00000004)			/*!<PC0 Pin Function - SmartCard1 clock */
#define PC0_MFP_LCD_DH1				((uint32_t)0x00000007)			/*!<PC0 Pin Function - LCD DH1 */
#define PC0_MFP_MASK				((uint32_t)0x00000007)			/*!<PC0 Pin Function Mask */

/********************* Bit definition of PC_H_MFP register **********************/
#define PC15_MFP_GPC15				((uint32_t)0x00000000)			/*!<PC15 Pin Function - GPIOC[15] */
#define PC15_MFP_EBI_AD3			((uint32_t)0x20000000)			/*!<PC15 Pin Function - EBI AD[3] */
#define PC15_MFP_TMR0_CAP			((uint32_t)0x30000000)			/*!<PC15 Pin Function - Timer0 capture event */
#define PC15_MFP_PWM1_CH2			((uint32_t)0x40000000)			/*!<PC15 Pin Function - PWM1 Channel 2 */
#define PC15_MFP_LCD_S33			((uint32_t)0x70000000)			/*!<PC15 Pin Function - LCD SEG 33 */
#define PC15_MFP_LCD_S16			((uint32_t)0x70000000)			/*!<PC15 Pin Function - LCD SEG 16 */
#define PC15_MFP_MASK				((uint32_t)0x70000000)			/*!<PC15 Pin Function Mask */

#define PC14_MFP_GPC14				((uint32_t)0x00000000)			/*!<PC14 Pin Function - GPIOC[14] */
#define PC14_MFP_EBI_AD2			((uint32_t)0x02000000)			/*!<PC14 Pin Function - EBI AD[2] */
#define PC14_MFP_PWM1_CH3			((uint32_t)0x04000000)			/*!<PC14 Pin Function - PWM1 Channel 3 */
#define PC14_MFP_LCD_S32			((uint32_t)0x07000000)			/*!<PC14 Pin Function - LCD SEG 32 */
#define PC14_MFP_LCD_S15			((uint32_t)0x07000000)			/*!<PC14 Pin Function - LCD SEG 15 */
#define PC14_MFP_MASK				((uint32_t)0x07000000)			/*!<PC14 Pin Function Mask */

#define PC13_MFP_GPC13				((uint32_t)0x00000000)			/*!<PC13 Pin Function - GPIOC[13] */
#define PC13_MFP_SPI1_MOSI1			((uint32_t)0x00100000)			/*!<PC13 Pin Function - SPI1 MOSI[1] */
#define PC13_MFP_PWM1_CH1			((uint32_t)0x00200000)			/*!<PC13 Pin Function - PWM1 Channel 1 */
#define PC13_MFP_SNOOPER			((uint32_t)0x00400000)			/*!<PC13 Pin Function - Snooper pin */
#define PC13_MFP_EXT_INT1			((uint32_t)0x00500000)			/*!<PC13 Pin Function - External interrupt 1 */
#define PC13_MFP_I2C0_SCL			((uint32_t)0x00600000)			/*!<PC13 Pin Function - I2C0 clock */
#define PC13_MFP_MASK				((uint32_t)0x00700000)			/*!<PC13 Pin Function Mask */

#define PC12_MFP_GPC12				((uint32_t)0x00000000)			/*!<PC12 Pin Function - GPIOC[12] */
#define PC12_MFP_SPI1_MISO1			((uint32_t)0x00010000)			/*!<PC12 Pin Function - SPI1 MISO[1] */
#define PC12_MFP_PWM1_CH0			((uint32_t)0x00020000)			/*!<PC12 Pin Function - PWM1 Channel 0 */
#define PC12_MFP_EXT_INT0			((uint32_t)0x00050000)			/*!<PC12 Pin Function - External interrupt 0 */
#define PC12_MFP_I2C0_SDA			((uint32_t)0x00060000)			/*!<PC12 Pin Function - I2C0 DATA */
#define PC12_MFP_MASK				((uint32_t)0x00070000)			/*!<PC12 Pin Function Mask */

#define PC11_MFP_GPC11				((uint32_t)0x00000000)			/*!<PC11 Pin Function - GPIOC[11] */
#define PC11_MFP_SPI1_MOSI0			((uint32_t)0x00001000)			/*!<PC11 Pin Function - SPI1 MOSI[0] */
#define PC11_MFP_UART1_TX			((uint32_t)0x00005000)			/*!<PC11 Pin Function - UART1 TX */
#define PC11_MFP_TK15				((uint32_t)0x00006000)			/*!<PC11 Pin Function - Touch key 15 */
#define PC11_MFP_LCD_S31			((uint32_t)0x00007000)			/*!<PC11 Pin Function - LCD SEG 31 */
#define PC11_MFP_MASK				((uint32_t)0x00007000)			/*!<PC11 Pin Function Mask */

#define PC10_MFP_GPC10				((uint32_t)0x00000000)			/*!<PC10 Pin Function - GPIOC[10] */
#define PC10_MFP_SPI1_MISO0			((uint32_t)0x00000100)			/*!<PC10 Pin Function - SPI1 MISO[0] */
#define PC10_MFP_UART1_RX			((uint32_t)0x00000500)			/*!<PC10 Pin Function - UART1 RX */
#define PC10_MFP_TK14				((uint32_t)0x00000600)			/*!<PC10 Pin Function - Touch key 14 */
#define PC10_MFP_LCD_S30			((uint32_t)0x00000700)			/*!<PC10 Pin Function - LCD SEG 30 */
#define PC10_MFP_MASK				((uint32_t)0x00000700)			/*!<PC10 Pin Function Mask */

#define PC9_MFP_GPC9				((uint32_t)0x00000000)			/*!<PC9 Pin Function - GPIOC[9] */
#define PC9_MFP_SPI1_SCLK			((uint32_t)0x00000010)			/*!<PC9 Pin Function - SPI1 SCLK */
#define PC9_MFP_I2C1_SCL			((uint32_t)0x00000050)			/*!<PC9 Pin Function - I2C1 clock */
#define PC9_MFP_TK13				((uint32_t)0x00000060)			/*!<PC9 Pin Function - Touch key 13 */
#define PC9_MFP_LCD_S29				((uint32_t)0x00000070)			/*!<PC9 Pin Function - LCD SEG 29 */
#define PC9_MFP_MASK				((uint32_t)0x00000070)			/*!<PC9 Pin Function Mask */

#define PC8_MFP_GPC8				((uint32_t)0x00000000)			/*!<PC8 Pin Function - GPIOC[8] */
#define PC8_MFP_SPI1_SS0			((uint32_t)0x00000001)			/*!<PC8 Pin Function - SPI1 SS[0] */
#define PC8_MFP_EBI_XCLK			((uint32_t)0x00000002)			/*!<PC8 Pin Function - EBI XCLK */
#define PC8_MFP_I2C1_SDA			((uint32_t)0x00000005)			/*!<PC8 Pin Function - I2C1 DATA */
#define PC8_MFP_TK12				((uint32_t)0x00000006)			/*!<PC8 Pin Function - Touch key 12 */
#define PC8_MFP_LCD_S28				((uint32_t)0x00000007)			/*!<PC8 Pin Function - LCD SEG 28 */
#define PC8_MFP_MASK				((uint32_t)0x00000007)			/*!<PC8 Pin Function Mask */

/********************* Bit definition of PD_L_MFP register **********************/
#define PD7_MFP_GPD7				((uint32_t)0x00000000)			/*!<PD7 Pin Function - GPIOD[7] */
#define PD7_MFP_LCD_S2				((uint32_t)0x70000000)			/*!<PD7 Pin Function - LCD SEG 2 */
#define PD7_MFP_MASK				((uint32_t)0x70000000)			/*!<PD7 Pin Function Mask */

#define PD6_MFP_GPD6				((uint32_t)0x00000000)			/*!<PD6 Pin Function - GPIOD[6] */
#define PD6_MFP_LCD_S3				((uint32_t)0x07000000)			/*!<PD6 Pin Function - LCD SEG 3 */
#define PD6_MFP_MASK				((uint32_t)0x07000000)			/*!<PD6 Pin Function Mask */

#define PD5_MFP_GPD5				((uint32_t)0x00000000)			/*!<PD5 Pin Function - GPIOD[5] */
#define PD5_MFP_I2S_DOUT			((uint32_t)0x00200000)			/*!<PD5 Pin Function - I2S Dout */
#define PD5_MFP_SPI2_MOSI1			((uint32_t)0x00300000)			/*!<PD5 Pin Function - SPI2 MOSI[1] */
#define PD5_MFP_TK5					((uint32_t)0x00600000)			/*!<PD5 Pin Function - Touch key 5 */
#define PD5_MFP_LCD_S34				((uint32_t)0x00700000)			/*!<PD5 Pin Function - LCD SEG 34 */
#define PD5_MFP_MASK				((uint32_t)0x00700000)			/*!<PD5 Pin Function Mask */

#define PD4_MFP_GPD4				((uint32_t)0x00000000)			/*!<PD4 Pin Function - GPIOD[4] */
#define PD4_MFP_I2S_DIN				((uint32_t)0x00020000)			/*!<PD4 Pin Function - I2S Din */
#define PD4_MFP_SPI2_MISO1			((uint32_t)0x00030000)			/*!<PD4 Pin Function - SPI2 MISO[1] */
#define PD4_MFP_SC1_CD				((uint32_t)0x00040000)			/*!<PD4 Pin Function - SmartCard1 card detection */
#define PD4_MFP_TK4					((uint32_t)0x00060000)			/*!<PD4 Pin Function - Touch key 4 */
#define PD4_MFP_LCD_S35				((uint32_t)0x00070000)			/*!<PD4 Pin Function - LCD SEG 35 */
#define PD4_MFP_MASK				((uint32_t)0x00070000)			/*!<PD4 Pin Function Mask */

#define PD3_MFP_GPD3				((uint32_t)0x00000000)			/*!<PD3 Pin Function - GPIOD[3] */
#define PD3_MFP_UART1_CTS			((uint32_t)0x00001000)			/*!<PD3 Pin Function - UART1 CTSn */
#define PD3_MFP_I2S_BCLK			((uint32_t)0x00002000)			/*!<PD3 Pin Function - I2S BCLK */
#define PD3_MFP_SPI2_MOSI0			((uint32_t)0x00003000)			/*!<PD3 Pin Function - SPI2 MOSI[0] */
#define PD3_MFP_SC1_RST				((uint32_t)0x00004000)			/*!<PD3 Pin Function - SmartCard1 reset */
#define PD3_MFP_ADC_CH11			((uint32_t)0x00005000)			/*!<PD3 Pin Function - ADC input channel 11 */
#define PD3_MFP_TK3					((uint32_t)0x00006000)			/*!<PD3 Pin Function - Touch key 3 */
#define PD3_MFP_MASK				((uint32_t)0x00007000)			/*!<PD3 Pin Function Mask */

#define PD2_MFP_GPD2				((uint32_t)0x00000000)			/*!<PD2 Pin Function - GPIOD[2] */
#define PD2_MFP_UART1_RTS			((uint32_t)0x00000100)			/*!<PD2 Pin Function - UART1 RTSn */
#define PD2_MFP_I2S_WS				((uint32_t)0x00000200)			/*!<PD2 Pin Function - I2S WS */
#define PD2_MFP_SPI2_MISO0			((uint32_t)0x00000300)			/*!<PD2 Pin Function - SPI2 MISO[0] */
#define PD2_MFP_SC1_PWR				((uint32_t)0x00000400)			/*!<PD2 Pin Function - SmartCard1 power */
#define PD2_MFP_ADC_CH10			((uint32_t)0x00000500)			/*!<PD2 Pin Function - ADC input channel 10 */
#define PD2_MFP_TK2					((uint32_t)0x00000600)			/*!<PD2 Pin Function - Touch key 2 */
#define PD2_MFP_MASK				((uint32_t)0x00000700)			/*!<PD2 Pin Function Mask */

#define PD1_MFP_GPD1				((uint32_t)0x00000000)			/*!<PD1 Pin Function - GPIOD[1] */
#define PD1_MFP_UART1_TX			((uint32_t)0x00000010)			/*!<PD1 Pin Function - UART1 TX */
#define PD1_MFP_SPI2_SCLK			((uint32_t)0x00000030)			/*!<PD1 Pin Function - SPI2 SCLK */
#define PD1_MFP_SC1_DAT				((uint32_t)0x00000040)			/*!<PD1 Pin Function - SmartCard1 DATA */
#define PD1_MFP_ADC_CH9				((uint32_t)0x00000050)			/*!<PD1 Pin Function - ADC input channel 9 */
#define PD1_MFP_TK1					((uint32_t)0x00000060)			/*!<PD1 Pin Function - Touch key 1 */
#define PD1_MFP_MASK				((uint32_t)0x00000070)			/*!<PD1 Pin Function Mask */

#define PD0_MFP_GPD0				((uint32_t)0x00000000)			/*!<PD0 Pin Function - GPIOD[0] */
#define PD0_MFP_UART1_RX			((uint32_t)0x00000001)			/*!<PD0 Pin Function - UART1 RX */
#define PD0_MFP_SPI2_SS0			((uint32_t)0x00000003)			/*!<PD0 Pin Function - SPI2 chip selection 0 */
#define PD0_MFP_SC1_CLK				((uint32_t)0x00000004)			/*!<PD0 Pin Function - SmartCard1 clock */
#define PD0_MFP_ADC_CH8				((uint32_t)0x00000005)			/*!<PD0 Pin Function - ADC input channel 8 */
#define PD0_MFP_TK0					((uint32_t)0x00000006)			/*!<PD0 Pin Function - Touch key 0 */
#define PD0_MFP_MASK				((uint32_t)0x00000007)			/*!<PD0 Pin Function Mask */

/********************* Bit definition of PD_H_MFP register **********************/
#define PD15_MFP_GPD15				((uint32_t)0x00000000)			/*!<PD15 Pin Function - GPIOD[15] */
#define PD15_MFP_LCD_S0				((uint32_t)0x70000000)			/*!<PD15 Pin Function - LCD SEG 0 */
#define PD15_MFP_MASK				((uint32_t)0x70000000)			/*!<PD15 Pin Function Mask */

#define PD14_MFP_GPD14				((uint32_t)0x00000000)			/*!<PD14 Pin Function - GPIOD[14] */
#define PD14_MFP_LCD_S1				((uint32_t)0x07000000)			/*!<PD14 Pin Function - LCD SEG 1 */
#define PD14_MFP_MASK				((uint32_t)0x07000000)			/*!<PD14 Pin Function Mask */

#define PD13_MFP_GPD13				((uint32_t)0x00000000)			/*!<PD13 Pin Function - GPIOD[13] */
#define PD13_MFP_LCD_S14			((uint32_t)0x00700000)			/*!<PD13 Pin Function - LCD SEG 14 */
#define PD13_MFP_MASK				((uint32_t)0x00700000)			/*!<PD13 Pin Function Mask */

#define PD12_MFP_GPD12				((uint32_t)0x00000000)			/*!<PD12 Pin Function - GPIOD[12] */
#define PD12_MFP_LCD_S15			((uint32_t)0x00070000)			/*!<PD12 Pin Function - LCD SEG 15 */
#define PD12_MFP_MASK				((uint32_t)0x00070000)			/*!<PD12 Pin Function Mask */

#define PD11_MFP_GPD11				((uint32_t)0x00000000)			/*!<PD11 Pin Function - GPIOD[11] */
#define PD11_MFP_LCD_S16			((uint32_t)0x00007000)			/*!<PD11 Pin Function - LCD SEG 16 */
#define PD11_MFP_MASK				((uint32_t)0x00007000)			/*!<PD11 Pin Function Mask */

#define PD10_MFP_GPD10				((uint32_t)0x00000000)			/*!<PD10 Pin Function - GPIOD[10] */
#define PD10_MFP_LCD_S17			((uint32_t)0x00000700)			/*!<PD10 Pin Function - LCD SEG 17 */
#define PD10_MFP_MASK				((uint32_t)0x00000700)			/*!<PD10 Pin Function Mask */

#define PD9_MFP_GPD9				((uint32_t)0x00000000)			/*!<PD9 Pin Function - GPIOD[9] */
#define PD9_MFP_LCD_S18				((uint32_t)0x00000070)			/*!<PD9 Pin Function - LCD SEG 18 */
#define PD9_MFP_MASK				((uint32_t)0x00000070)			/*!<PD9 Pin Function Mask */

#define PD8_MFP_GPD8				((uint32_t)0x00000000)			/*!<PD8 Pin Function - GPIOD[8] */
#define PD8_MFP_LCD_S19				((uint32_t)0x00000007)			/*!<PD8 Pin Function - LCD SEG 19 */
#define PD8_MFP_MASK				((uint32_t)0x00000007)			/*!<PD8 Pin Function Mask */

/********************* Bit definition of PE_L_MFP register **********************/
#define PE7_MFP_GPE7				((uint32_t)0x00000000)			/*!<PE7 Pin Function - GPIOE[7] */
#define PE7_MFP_LCD_S8				((uint32_t)0x70000000)			/*!<PE7 Pin Function - LCD SEG 8 */
#define PE7_MFP_MASK				((uint32_t)0x70000000)			/*!<PE7 Pin Function Mask */

#define PE6_MFP_GPE6				((uint32_t)0x00000000)			/*!<PE6 Pin Function - GPIOE[6] */
#define PE6_MFP_MASK				((uint32_t)0x07000000)			/*!<PE6 Pin Function Mask */

#define PE5_MFP_GPE5				((uint32_t)0x00000000)			/*!<PE5 Pin Function - GPIOE[5] */
#define PE5_MFP_PWM1_CH1			((uint32_t)0x00100000)			/*!<PE5 Pin Function - PWM1 Channel 1 */
#define PE5_MFP_MASK				((uint32_t)0x00700000)			/*!<PE5 Pin Function Mask */

#define PE4_MFP_GPE4				((uint32_t)0x00000000)			/*!<PE4 Pin Function - GPIOE[4] */
#define PE4_MFP_SPI0_MISO0			((uint32_t)0x00060000)			/*!<PE4 Pin Function - SPI0 MISO[0] */
#define PE4_MFP_MASK				((uint32_t)0x00070000)			/*!<PE4 Pin Function Mask */

#define PE3_MFP_GPE3				((uint32_t)0x00000000)			/*!<PE3 Pin Function - GPIOE[3] */
#define PE3_MFP_SPI0_MISO0			((uint32_t)0x00006000)			/*!<PE3 Pin Function - SPI0 MISO[0] */
#define PE3_MFP_MASK				((uint32_t)0x00007000)			/*!<PE3 Pin Function Mask */

#define PE2_MFP_GPE2				((uint32_t)0x00000000)			/*!<PE2 Pin Function - GPIOE[2] */
#define PE2_MFP_SPI0_SCLK			((uint32_t)0x00000600)			/*!<PE2 Pin Function - SPI0 SCLK */
#define PE2_MFP_MASK				((uint32_t)0x00000700)			/*!<PE2 Pin Function Mask */

#define PE1_MFP_GPE1				((uint32_t)0x00000000)			/*!<PE1 Pin Function - GPIOE[1] */
#define PE1_MFP_PWM1_CH3			((uint32_t)0x00000010)			/*!<PE1 Pin Function - PWM1 Channel 3 */
#define PE1_MFP_SPI0_SS0			((uint32_t)0x00000060)			/*!<PE1 Pin Function - SPI0 chip selection 0 */
#define PE1_MFP_MASK				((uint32_t)0x00000070)			/*!<PE1 Pin Function Mask */

#define PE0_MFP_GPE0				((uint32_t)0x00000000)			/*!<PE0 Pin Function - GPIOE[0] */
#define PE0_MFP_PWM1_CH2			((uint32_t)0x00000001)			/*!<PE0 Pin Function - PWM1 Channel 2 */
#define PE0_MFP_I2S_MCLK			((uint32_t)0x00000002)			/*!<PE0 Pin Function - I2S MCLK */
#define PE0_MFP_MASK				((uint32_t)0x00000007)			/*!<PE0 Pin Function Mask */

/********************* Bit definition of PE_H_MFP register **********************/
#define PE15_MFP_GPE15				((uint32_t)0x00000000)			/*!<PE15 Pin Function - GPIOE[15] */
#define PE15_MFP_LCD_S29			((uint32_t)0x70000000)			/*!<PE15 Pin Function - LCD SEG 29 */
#define PE15_MFP_MASK				((uint32_t)0x70000000)			/*!<PE15 Pin Function Mask */

#define PE14_MFP_GPA14				((uint32_t)0x00000000)			/*!<PE14 Pin Function - GPIOE[14] */
#define PE14_MFP_LCD_S28			((uint32_t)0x07000000)			/*!<PE14 Pin Function - LCD SEG 28 */
#define PE14_MFP_MASK				((uint32_t)0x07000000)			/*!<PE14 Pin Function Mask */

#define PE13_MFP_GPE13				((uint32_t)0x00000000)			/*!<PE13 Pin Function - GPIOE[13] */
#define PE13_MFP_LCD_S27			((uint32_t)0x00700000)			/*!<PE13 Pin Function - LCD SEG 27 */
#define PE13_MFP_MASK				((uint32_t)0x00700000)			/*!<PE13 Pin Function Mask */

#define PE12_MFP_GPE12				((uint32_t)0x00000000)			/*!<PE12 Pin Function - GPIOE[12] */
#define PE12_MFP_UART1_CTS			((uint32_t)0x00070000)			/*!<PE12 Pin Function - UART1 CTSn */
#define PE12_MFP_MASK				((uint32_t)0x00070000)			/*!<PE12 Pin Function Mask */

#define PE11_MFP_GPE11				((uint32_t)0x00000000)			/*!<PE11 Pin Function - GPIOE[11] */
#define PE11_MFP_UART1_RTS			((uint32_t)0x00007000)			/*!<PE11 Pin Function - UART1 RTSn */
#define PE11_MFP_MASK				((uint32_t)0x00007000)			/*!<PE11 Pin Function Mask */

#define PE10_MFP_GPE10				((uint32_t)0x00000000)			/*!<PE10 Pin Function - GPIOE[10] */
#define PE10_MFP_UART1_TX			((uint32_t)0x00000700)			/*!<PE10 Pin Function - UART1 TX */
#define PE10_MFP_MASK				((uint32_t)0x00000700)			/*!<PE10 Pin Function Mask */

#define PE9_MFP_GPE9				((uint32_t)0x00000000)			/*!<PE9 Pin Function - GPIOE[9] */
#define PE9_MFP_UART1_RX			((uint32_t)0x00000070)			/*!<PE9 Pin Function - UART1 RX */
#define PE9_MFP_MASK				((uint32_t)0x00000070)			/*!<PE9 Pin Function Mask */

#define PE8_MFP_GPE8				((uint32_t)0x00000000)			/*!<PE8 Pin Function - GPIOA[8] */
#define PE8_MFP_LCD_S9				((uint32_t)0x00000007)			/*!<PE8 Pin Function - LCD SEG 9 */
#define PE8_MFP_MASK				((uint32_t)0x00000007)			/*!<PE8 Pin Function Mask */

/********************* Bit definition of PF_L_MFP register **********************/
#define PF5_MFP_GPF5				((uint32_t)0x00000000)			/*!<PF5 Pin Function - GPIOF[5] */
#define PF5_MFP_I2C0_SCL			((uint32_t)0x00100000)			/*!<PF5 Pin Function - I2C0 clock */
#define PF5_MFP_TK7					((uint32_t)0x00600000)			/*!<PF5 Pin Function - Touch key 7 */
#define PF5_MFP_MASK				((uint32_t)0x00700000)			/*!<PF5 Pin Function Mask */

#define PF4_MFP_GPF4				((uint32_t)0x00000000)			/*!<PF4 Pin Function - GPIOF[4] */
#define PF4_MFP_I2C0_SDA			((uint32_t)0x00010000)			/*!<PF4 Pin Function - I2C0 DATA */
#define PF4_MFP_TK6					((uint32_t)0x00060000)			/*!<PF4 Pin Function - Touch key 6 */
#define PF4_MFP_MASK				((uint32_t)0x00070000)			/*!<PF4 Pin Function Mask */

#define PF3_MFP_GPF3				((uint32_t)0x00000000)			/*!<PF3 Pin Function - GPIOF[3] */
#define PF3_MFP_HXT_IN				((uint32_t)0x00007000)			/*!<PF3 Pin Function - HXT IN */
#define PF3_MFP_MASK				((uint32_t)0x00007000)			/*!<PF3 Pin Function Mask */

#define PF2_MFP_GPF2				((uint32_t)0x00000000)			/*!<PF2 Pin Function - GPIOF[2] */
#define PF2_MFP_HXT_OUT				((uint32_t)0x00000700)			/*!<PF2 Pin Function - HXT OUT */
#define PF2_MFP_MASK				((uint32_t)0x00000700)			/*!<PF2 Pin Function Mask */

#define PF1_MFP_GPF1				((uint32_t)0x00000000)			/*!<PF1 Pin Function - GPIOF[1] */
#define PF1_MFP_FRQDIV_CLK			((uint32_t)0x00000040)			/*!<PF1 Pin Function - FRQDIV_CLK */
#define PF1_MFP_EXT_INT1			((uint32_t)0x00000050)			/*!<PF1 Pin Function - External interrupt 1 */
#define PF1_MFP_ICE_CLK				((uint32_t)0x00000070)			/*!<PF1 Pin Function - ICE CLOCK */
#define PF1_MFP_MASK				((uint32_t)0x00000070)			/*!<PF1 Pin Function Mask */

#define PF0_MFP_GPF0				((uint32_t)0x00000000)			/*!<PF0 Pin Function - GPIOF[0] */
#define PF0_MFP_EXT_INT0			((uint32_t)0x00000005)			/*!<PF0 Pin Function - External interrupt 0 */
#define PF0_MFP_ICE_DAT				((uint32_t)0x00000007)			/*!<PF0 Pin Function - ICE DATA */
#define PF0_MFP_MASK				((uint32_t)0x00000007)			/*!<PF0 Pin Function Mask */


/******************************************************************************/
/*                CLK                                                         */
/******************************************************************************/
/********************* Bit definition of PWRCTL register **********************/
#define CLK_PWRCTL_HXT_EN			((uint32_t)0x00000001)			/*!<Enable high speed crystal */
#define CLK_PWRCTL_LXT_EN			((uint32_t)0x00000002)			/*!<Enable low speed crystal */
#define CLK_PWRCTL_HIRC_EN			((uint32_t)0x00000004)			/*!<Enable internal high speed oscillator */
#define CLK_PWRCTL_LIRC_EN			((uint32_t)0x00000008)			/*!<Enable internal low speed oscillator */
#define CLK_PWRCTL_DELY_EN			((uint32_t)0x00000010)			/*!<Enable the wake-up delay counter */
#define CLK_PWRCTL_WAKEINT_EN		((uint32_t)0x00000020)			/*!<Enable the wake-up interrupt */
#define CLK_PWRCTL_PWRDOWN_EN		((uint32_t)0x00000040)			/*!<Power down enable bit */
#define CLK_PWRCTL_HXT_SELXT		((uint32_t)0x00000100)			/*!<High frequency crystal loop back path Enabled */
#define CLK_PWRCTL_HXT_GAIN			((uint32_t)0x00000200)			/*!<High frequency crystal Gain control Enabled */
#define CLK_PWRCTL_LXT_SCNT			((uint32_t)0x00000400)			/*!<Delay 8192 LXT before LXT output */


/********************* Bit definition of AHBCLK register **********************/
#define CLK_AHBCLK_GPIO_EN			((uint32_t)0x00000001)			/*!<GPIO clock enable */
#define CLK_AHBCLK_DMA_EN			((uint32_t)0x00000002)			/*!<DMA clock enable */
#define CLK_AHBCLK_ISP_EN			((uint32_t)0x00000004)			/*!<Flash ISP controller clock enable */
#define CLK_AHBCLK_EBI_EN			((uint32_t)0x00000008)			/*!<EBI clock enable */
#define CLK_AHBCLK_SRAM_EN			((uint32_t)0x00000010)			/*!<SRAM Controller Clock Enable */
#define CLK_AHBCLK_TICK_EN			((uint32_t)0x00000020)			/*!<System Tick Clock Enable */

/********************* Bit definition of APBCLK register **********************/
#define CLK_APBCLK_WDT_EN			((uint32_t)0x00000001)			/*!<Watchdog clock enable */
#define CLK_APBCLK_RTC_EN			((uint32_t)0x00000002)			/*!<RTC clock enable */
#define CLK_APBCLK_TMR0_EN			((uint32_t)0x00000004)			/*!<Timer 0 clock enable */
#define CLK_APBCLK_TMR1_EN			((uint32_t)0x00000008)			/*!<Timer 1 clock enable */
#define CLK_APBCLK_TMR2_EN			((uint32_t)0x00000010)			/*!<Timer 2 clock enable */
#define CLK_APBCLK_TMR3_EN			((uint32_t)0x00000020)			/*!<Timer 3 clock enable */
#define CLK_APBCLK_FDIV_EN			((uint32_t)0x00000040)			/*!<Frequency Divider Output clock enable */
#define CLK_APBCLK_SC2_EN			((uint32_t)0x00000080)			/*!<SmartCard 2 Clock Enable Control */
#define CLK_APBCLK_I2C0_EN			((uint32_t)0x00000100)			/*!<I2C 0 clock enable */
#define CLK_APBCLK_I2C1_EN			((uint32_t)0x00000200)			/*!<I2C 1 clock enable */
#define CLK_APBCLK_SPI0_EN			((uint32_t)0x00001000)			/*!<SPI 0 clock enable */
#define CLK_APBCLK_SPI1_EN			((uint32_t)0x00002000)			/*!<SPI 1 clock enable */
#define CLK_APBCLK_SPI2_EN			((uint32_t)0x00004000)			/*!<SPI 2 clock enable */
#define CLK_APBCLK_UART0_EN			((uint32_t)0x00010000)			/*!<UART 0 clock enable */
#define CLK_APBCLK_UART1_EN			((uint32_t)0x00020000)			/*!<UART 1 clock enable */
#define CLK_APBCLK_PWM0_CH01_EN		((uint32_t)0x00100000)			/*!<PWM0 Channel 0 and Channel 1 Clock Enable Control */
#define CLK_APBCLK_PWM0_CH23_EN		((uint32_t)0x00200000)			/*!<PWM0 Channel 2 and Channel 3 Clock Enable Control */
#define CLK_APBCLK_PWM1_CH01_EN		((uint32_t)0x00400000)			/*!<PWM1 Channel 0 and Channel 1 Clock Enable Control */
#define CLK_APBCLK_PWM1_CH23_EN		((uint32_t)0x00800000)			/*!<PWM1 Channel 2 and Channel 3 Clock Enable Control */
#define CLK_APBCLK_TK_EN			((uint32_t)0x01000000)			/*!<Touch key Clock Enable Control */
#define CLK_APBCLK_DAC_EN			((uint32_t)0x02000000)			/*!<DAC Clock Enable Control */
#define CLK_APBCLK_LCD_EN			((uint32_t)0x04000000)			/*!<LCD controller Clock Enable Control */
#define CLK_APBCLK_USBD_EN			((uint32_t)0x08000000)			/*!<USB device clock enable */
#define CLK_APBCLK_ADC_EN			((uint32_t)0x10000000)			/*!<ADC clock enable */
#define CLK_APBCLK_I2S_EN			((uint32_t)0x20000000)			/*!<I2S clock enable */
#define CLK_APBCLK_SC0_EN			((uint32_t)0x40000000)			/*!<SmartCard 0 Clock Enable Control */
#define CLK_APBCLK_SC1_EN			((uint32_t)0x80000000)			/*!<SmartCard 1 Clock Enable Control */

/********************* Bit definition of CLKSTATUS register **********************/
#define CLK_CLKSTATUS_HXT_STB		((uint32_t)0x00000001)			/*!<External high speed crystal clock source stable flag */
#define CLK_CLKSTATUS_LXT_STB		((uint32_t)0x00000002)			/*!<External low speed crystal clock source stable flag */
#define CLK_CLKSTATUS_PLL_STB		((uint32_t)0x00000004)			/*!<Internal PLL clock source stable flag */
#define CLK_CLKSTATUS_LIRC_STB		((uint32_t)0x00000008)			/*!<Internal low speed oscillator clock source stable flag */
#define CLK_CLKSTATUS_HIRC_STB		((uint32_t)0x00000010)			/*!<Internal high speed oscillator clock source stable flag */
#define CLK_CLKSTATUS_CLK_SW_FAIL	((uint32_t)0x00000080)			/*!<Clock switch fail flag */


/********************* Bit definition of CLKSEL0 register **********************/
#define CLK_CLKSEL0_HCLK_MASK		((uint32_t)0x00000007)			/*!<HCLK clock source selection bit mask */
#define CLK_CLKSEL0_HCLK_HXT		((uint32_t)0x00000000)			/*!<Select HCLK clock source from high speed crystal */
#define CLK_CLKSEL0_HCLK_LXT		((uint32_t)0x00000001)			/*!<Select HCLK clock source from low speed crystal */
#define CLK_CLKSEL0_HCLK_PLL		((uint32_t)0x00000002)			/*!<Select HCLK clock source from PLL */
#define CLK_CLKSEL0_HCLK_LIRC		((uint32_t)0x00000003)			/*!<Select HCLK clock source from low speed oscillator */
#define CLK_CLKSEL0_HCLK_HIRC		((uint32_t)0x00000007)			/*!<Select HCLK clock source from high speed oscillator */

/********************* Bit definition of CLKSEL1 register **********************/
#define CLK_CLKSEL1_UART_MASK		((uint32_t)0x00000003)			/*!<UART clock source selection bit mask */
#define CLK_CLKSEL1_UART_HXT		((uint32_t)0x00000000)			/*!<Select UART clock source from high speed crystal */
#define CLK_CLKSEL1_UART_LXT		((uint32_t)0x00000001)			/*!<Select UART clock source from low speed crystal */
#define CLK_CLKSEL1_UART_PLL		((uint32_t)0x00000002)			/*!<Select UART clock source from PLL */
#define CLK_CLKSEL1_UART_HIRC		((uint32_t)0x00000003)			/*!<Select UART clock source from igh speed oscillator */

#define CLK_CLKSEL1_ADC_MASK		((uint32_t)0x0000000C)			/*!<ADC clock source selection bit mask */
#define CLK_CLKSEL1_ADC_HXT			((uint32_t)0x00000000)			/*!<Select ADC clock source from high speed crystal */
#define CLK_CLKSEL1_ADC_LXT			((uint32_t)0x00000004)			/*!<Select ADC clock source from low speed crystal */
#define CLK_CLKSEL1_ADC_PLL			((uint32_t)0x00000008)			/*!<Select ADC clock source from PLL */
#define CLK_CLKSEL1_ADC_HIRC		((uint32_t)0x0000000C)			/*!<Select ADC clock source from high speed oscillator */

#define CLK_CLKSEL1_PWM0_CH01_MASK	((uint32_t)0x00000030)			/*!<PWM0 channel 0 and channel 1 Clock Source Selection */
#define CLK_CLKSEL1_PWM0_CH01_HXT	((uint32_t)0x00000000)			/*!<Select PWM0_CH01 clock source from high speed crystal */
#define CLK_CLKSEL1_PWM0_CH01_LXT	((uint32_t)0x00000010)			/*!<Select PWM0_CH01 clock source from low speed crystal */
#define CLK_CLKSEL1_PWM0_CH01_HCLK	((uint32_t)0x00000020)			/*!<Select PWM0_CH01 clock source from HCLK */
#define CLK_CLKSEL1_PWM0_CH01_HIRC	((uint32_t)0x00000030)			/*!<Select PWM0_CH01 clock source from high speed oscillator */

#define CLK_CLKSEL1_PWM0_CH23_MASK	((uint32_t)0x000000C0)			/*!<PWM0 channel 2 and channel 3 Clock Source Selection */
#define CLK_CLKSEL1_PWM0_CH23_HXT	((uint32_t)0x00000000)			/*!<Select PWM0_CH23 clock source from high speed crystal */
#define CLK_CLKSEL1_PWM0_CH23_LXT	((uint32_t)0x00000040)			/*!<Select PWM0_CH23 clock source from low speed crystal */
#define CLK_CLKSEL1_PWM0_CH23_HCLK	((uint32_t)0x00000080)			/*!<Select PWM0_CH23 clock source from HCLK */
#define CLK_CLKSEL1_PWM0_CH23_HIRC	((uint32_t)0x000000C0)			/*!<Select PWM0_CH23 clock source from high speed oscillator */

#define CLK_CLKSEL1_TMR0_MASK		((uint32_t)0x00000700)			/*!<TMR0 clock source selection bit mask */
#define CLK_CLKSEL1_TMR0_HXT		((uint32_t)0x00000000)			/*!<Select TMR0 clock source from high speed crystal */
#define CLK_CLKSEL1_TMR0_LXT		((uint32_t)0x00000100)			/*!<Select TMR0 clock source from low speed crystal */
#define CLK_CLKSEL1_TMR0_LIRC		((uint32_t)0x00000200)			/*!<Select TMR0 clock source from low speed oscillator */
#define CLK_CLKSEL1_TMR0_EXT		((uint32_t)0x00000300)			/*!<Select TMR0 clock source from external trigger */
#define CLK_CLKSEL1_TMR0_HIRC		((uint32_t)0x00000700)			/*!<Select TMR0 clock source from high speed oscillator */

#define CLK_CLKSEL1_TMR1_MASK		((uint32_t)0x00007000)			/*!<TMR1 clock source selection bit mask */
#define CLK_CLKSEL1_TMR1_HXT		((uint32_t)0x00000000)			/*!<Select TMR1 clock source from high speed crystal */
#define CLK_CLKSEL1_TMR1_LXT		((uint32_t)0x00001000)			/*!<Select TMR1 clock source from low speed crystal */
#define CLK_CLKSEL1_TMR1_LIRC		((uint32_t)0x00002000)			/*!<Select TMR1 clock source from low speed oscillator  */
#define CLK_CLKSEL1_TMR1_EXT		((uint32_t)0x00003000)			/*!<Select TMR1 clock source from external trigger */
#define CLK_CLKSEL1_TMR1_HIRC		((uint32_t)0x00007000)			/*!<Select TMR1 clock source from high speed oscillator */

#define CLK_CLKSEL1_TK_MASK			((uint32_t)0x00030000)			/*!<TK clock source selection bit mask */
#define CLK_CLKSEL1_TK_HXT			((uint32_t)0x00000000)			/*!<Select TK clock source from high speed crystal */
#define CLK_CLKSEL1_TK_PLL			((uint32_t)0x00010000)			/*!<Select TK clock source from PLL */
#define CLK_CLKSEL1_TK_HIRC			((uint32_t)0x00020000)			/*!<Select TK clock source from high speed oscillator */

#define CLK_CLKSEL1_LCD_MASK        ((uint32_t)0x00040000)			/*!<LCD clock source selection bit mask */
#define CLK_CLKSEL1_LCD_LXT			((uint32_t)0x00000000)			/*!<Select LCD clock source from low speed crystal */
#define CLK_CLKSEL1_LCD_LIRC		((uint32_t)0x00040000)			/*!<Select LCD clock source from LIRC */

/********************* Bit definition of CLKSEL2 register **********************/
#define CLK_CLKSEL2_FRQDIV_MASK		((uint32_t)0x0000000C)			/*!<FRQDIV clock source selection bit mask */
#define CLK_CLKSEL2_FRQDIV_HXT		((uint32_t)0x00000000)			/*!<Select FRQDIV clock source from high speed crystal */
#define CLK_CLKSEL2_FRQDIV_LXT		((uint32_t)0x00000004)			/*!<Select FRQDIV clock source from low speed crystal */
#define CLK_CLKSEL2_FRQDIV_HCLK		((uint32_t)0x00000008)			/*!<Select FRQDIV clock source from HCLK */
#define CLK_CLKSEL2_FRQDIV_HIRC		((uint32_t)0x0000000C)			/*!<Select FRQDIV clock source from high speed oscillator */

#define CLK_CLKSEL2_PWM1_CH01_MASK	((uint32_t)0x00000030)			/*!<PWM1_CH01 clock source selection bit mask */
#define CLK_CLKSEL2_PWM1_CH01_HXT	((uint32_t)0x00000000)			/*!<Select PWM1_CH01 clock source from high speed crystal */
#define CLK_CLKSEL2_PWM1_CH01_LXT	((uint32_t)0x00000010)			/*!<Select PWM1_CH01 clock source from low speed crystal */
#define CLK_CLKSEL2_PWM1_CH01_HCLK	((uint32_t)0x00000020)			/*!<Select PWM1_CH01 clock source from HCLK */
#define CLK_CLKSEL2_PWM1_CH01_HIRC	((uint32_t)0x00000030)			/*!<Select PWM1_CH01 clock source from high speed oscillator */

#define CLK_CLKSEL2_PWM1_CH23_MASK	((uint32_t)0x000000C0)			/*!<PWM1_CH23 clock source selection bit mask */
#define CLK_CLKSEL2_PWM1_CH23_HXT	((uint32_t)0x00000000)			/*!<Select PWM1_CH23 clock source from high speed crystal */
#define CLK_CLKSEL2_PWM1_CH23_LXT	((uint32_t)0x00000040)			/*!<Select PWM1_CH23 clock source from low speed crystal */
#define CLK_CLKSEL2_PWM1_CH23_HCLK	((uint32_t)0x00000080)			/*!<Select PWM1_CH23 clock source from HCLK */
#define CLK_CLKSEL2_PWM1_CH23_HIRC	((uint32_t)0x000000C0)			/*!<Select PWM1_CH23 clock source from high speed oscillator */

#define CLK_CLKSEL2_TMR2_MASK		((uint32_t)0x00000700)			/*!<TMR2 clock source selection bit mask */
#define CLK_CLKSEL2_TMR2_HXT		((uint32_t)0x00000000)			/*!<Select TMR2 clock source from high speed crystal */
#define CLK_CLKSEL2_TMR2_LXT		((uint32_t)0x00000100)			/*!<Select TMR2 clock source from low speed crystal */
#define CLK_CLKSEL2_TMR2_LIRC		((uint32_t)0x00000200)			/*!<Select TMR2 clock source from low speed oscillator */
#define CLK_CLKSEL2_TMR2_EXT		((uint32_t)0x00000300)			/*!<Select TMR2 clock source from external trigger */
#define CLK_CLKSEL2_TMR2_HIRC		((uint32_t)0x00000700)			/*!<Select TMR2 clock source from high speed oscillator */

#define CLK_CLKSEL2_TMR3_MASK		((uint32_t)0x00007000)			/*!<TMR3 clock source selection bit mask */
#define CLK_CLKSEL2_TMR3_HXT		((uint32_t)0x00000000)			/*!<Select TMR3 clock source from high speed crystal */
#define CLK_CLKSEL2_TMR3_LXT		((uint32_t)0x00001000)			/*!<Select TMR3 clock source from low speed crystal */
#define CLK_CLKSEL2_TMR3_LIRC		((uint32_t)0x00002000)			/*!<Select TMR3 clock source from low speed oscillator  */
#define CLK_CLKSEL2_TMR3_EXT		((uint32_t)0x00003000)			/*!<Select TMR3 clock source from external trigger */
#define CLK_CLKSEL2_TMR3_HIRC		((uint32_t)0x00007000)			/*!<Select TMR3 clock source from high speed oscillator */

#define CLK_CLKSEL2_I2S_MASK		((uint32_t)0x00030000)			/*!<I2S clock source selection bit mask */
#define CLK_CLKSEL2_I2S_HXT			((uint32_t)0x00000000)			/*!<Select I2S clock source from high speed crystal */
#define CLK_CLKSEL2_I2S_PLL			((uint32_t)0x00010000)			/*!<Select I2S clock source from PLL */
#define CLK_CLKSEL2_I2S_HIRC		((uint32_t)0x00020000)			/*!<Select I2S clock source from high speed oscillator */

#define CLK_CLKSEL2_SC_MASK			((uint32_t)0x000C0000)			/*!<SC clock source selection bit mask */
#define CLK_CLKSEL2_SC_HXT			((uint32_t)0x00000000)			/*!<Select SC clock source from high speed crystal */
#define CLK_CLKSEL2_SC_PLL			((uint32_t)0x00040000)			/*!<Select SC clock source from PLL */
#define CLK_CLKSEL2_SC_HIRC			((uint32_t)0x00080000)			/*!<Select SC clock source from high speed oscillator */

#define CLK_CLKSEL2_SPI0_MASK		((uint32_t)0x00100000)			/*!<SPI 0 clock source selection bit mask */
#define CLK_CLKSEL2_SPI0_PLL		((uint32_t)0x00000000)			/*!<Select SPI 0 clock source from PLL */
#define CLK_CLKSEL2_SPI0_HCLK		((uint32_t)0x00100000)			/*!<Select SPI 0 clock source from HCLK */

#define CLK_CLKSEL2_SPI1_MASK		((uint32_t)0x00200000)			/*!<SPI 1 clock source selection bit mask */
#define CLK_CLKSEL2_SPI1_PLL		((uint32_t)0x00000000)			/*!<Select SPI 1 clock source from PLL */
#define CLK_CLKSEL2_SPI1_HCLK		((uint32_t)0x00200000)			/*!<Select SPI 1 clock source from HCLK */

#define CLK_CLKSEL2_SPI2_MASK		((uint32_t)0x00400000)			/*!<SPI 2 clock source selection bit mask */
#define CLK_CLKSEL2_SPI2_PLL		((uint32_t)0x00000000)			/*!<Select SPI 2 clock source from PLL */
#define CLK_CLKSEL2_SPI2_HCLK		((uint32_t)0x00400000)			/*!<Select SPI 2 clock source from HCLK */

/********************* Bit definition of CLKDIV0 register **********************/
#define CLK_CLKDIV0_HCLK_MASK		((uint32_t)0x0000000F)			/*!<HCLK clock divider bit mask */
#define HCLK_CLK_DIVIDER(x)			(x & 0x0F)

#define CLK_CLKDIV0_USB_MASK		((uint32_t)0x000000F0)			/*!<USB clock divider bit mask */
#define USB_CLK_DIVIDER(x)			((x & 0x0F) << 4)

#define CLK_CLKDIV0_UART_MASK		((uint32_t)0x00000F00)			/*!<UART clock divider bit mask */
#define UART_CLK_DIVIDER(x)			((x & 0x0F) << 8)

#define CLK_CLKDIV0_I2S_MASK		((uint32_t)0x0000F000)			/*!<I2S clock divider bit mask */
#define I2S_CLK_DIVIDER(x)			((x & 0x0F) << 12)

#define CLK_CLKDIV0_ADC_MASK		((uint32_t)0x00FF0000)			/*!<ADC clock divider bit mask */
#define ADC_CLK_DIVIDER(x)			((x & 0xFF) << 16)

#define CLK_CLKDIV0_TK_MASK			((uint32_t)0x0F000000)			/*!<TK clock divider bit mask */
#define TK_CLK_DIVIDER(x)			((x & 0x0F) << 24)

#define CLK_CLKDIV0_SC0_MASK		((uint32_t)0xF0000000)			/*!<SC0 clock divider bit mask */
#define SC0_CLK_DIVIDER(x)			((x & 0x0F) << 28)

/********************* Bit definition of CLKDIV1 register **********************/
#define CLK_CLKDIV1_SC1_MASK		((uint32_t)0x0000000F)			/*!<SC1 clock divider bit mask */
#define SC1_CLK_DIVIDER(x)			(x & 0x0F)

#define CLK_CLKDIV1_SC2_MASK		((uint32_t)0x000000F0)			/*!<SC2 clock divider bit mask */
#define SC2_CLK_DIVIDER(x)			((x & 0x0F) << 4)

/********************* Bit definition of PLLCTL register **********************/
#define CLK_PLLCTL_FB_DIVIDER_MASK	((uint32_t)0x0000003F)		/*!<PLL Feedback divider bit mask */
#define CLK_PLLCTL_IN_DIVIDER_MASK	((uint32_t)0x00000300)		/*!<PLL Input divider bit mask */
#define CLK_PLLCTL_OUT_DV			((uint32_t)0x00001000)		/*!<PLL Output Divider Control */
#define CLK_PLLCTL_PD				((uint32_t)0x00010000)		/*!<PLL Power down mode */

#define CLK_PLLCTL_PLLSRC_MASK		((uint32_t)0x00020000)		/*!<PLL clock source bit mask */
#define CLK_PLLCTL_PLLSRC_HIRC		((uint32_t)0x00020000)		/*!<PLL clock source from high speed oscillator */
#define CLK_PLLCTL_PLLSRC_HXT		((uint32_t)0x00000000)		/*!<PLL clock source from high speed crystal */

/********************* Bit definition of FRQDIV register **********************/
#define CLK_FRQDIV_SEL_MASK			((uint32_t)0x0000000F)		/*!<Divider output frequency selection bits mask */
#define CLK_FRQDIV_EN				((uint32_t)0x00000010)		/*!<Frequency divider enable bit */

/********************* Bit definition of WK_INTSTS register **********************/
#define CLK_WK_INTSTS_IS			((uint32_t)0x00000001)		/*!<Wake-up Interrupt Sstatus in chip Power-down Mode */


/************************** PLL Control Setting ******************************/
/* PLL Constants */
#define PLL_NOT_SUPPORTED		0xFFFF

/* PLL seting for 4M input clock */
#define PLL_IN_4M_OUT_45M		PLL_NOT_SUPPORTED
#define PLL_IN_4M_OUT_48M		0x0110
#define PLL_IN_4M_OUT_90M		PLL_NOT_SUPPORTED
#define PLL_IN_4M_OUT_96M		0x0010
#define PLL_IN_4M_OUT_120M		0x001c
#define PLL_IN_4M_OUT_128M		0xFFFF	
/* PLL seting for 8M input clock */
#define PLL_IN_8M_OUT_45M		PLL_NOT_SUPPORTED
#define PLL_IN_8M_OUT_48M		0x0210
#define PLL_IN_8M_OUT_90M		PLL_NOT_SUPPORTED
#define PLL_IN_8M_OUT_96M		0x0110
#define PLL_IN_8M_OUT_120M		PLL_NOT_SUPPORTED
#define PLL_IN_8M_OUT_128M		0x0120	
/* PLL seting for 12M input clock */
#define PLL_IN_12M_OUT_45M		PLL_NOT_SUPPORTED
#define PLL_IN_12M_OUT_48M		0x0200
#define PLL_IN_12M_OUT_90M		PLL_NOT_SUPPORTED
#define PLL_IN_12M_OUT_96M		0x0100
#define PLL_IN_12M_OUT_120M		0x0230
#define PLL_IN_12M_OUT_128M		PLL_NOT_SUPPORTED
/* PLL seting for 15M input clock */
#define PLL_IN_15M_OUT_45M		0x0310
#define PLL_IN_15M_OUT_48M		PLL_NOT_SUPPORTED
#define PLL_IN_15M_OUT_90M		0x0210
#define PLL_IN_15M_OUT_96M		PLL_NOT_SUPPORTED
#define PLL_IN_15M_OUT_120M		0x0220
#define PLL_IN_15M_OUT_128M		PLL_NOT_SUPPORTED
/* PLL seting for 24M input clock */
#define PLL_IN_24M_OUT_45M		PLL_NOT_SUPPORTED
#define PLL_IN_24M_OUT_48M		0x1320
#define PLL_IN_24M_OUT_90M		PLL_NOT_SUPPORTED
#define PLL_IN_24M_OUT_96M		0x0320
#define PLL_IN_24M_OUT_120M		0x0330
#define PLL_IN_24M_OUT_128M		PLL_NOT_SUPPORTED


/******************************************************************************/
/*                GPIO                                                        */
/******************************************************************************/
/********************* Bit definition of PMD register **********************/
#define GPIO_PMD_PMD0_INPUT			((uint32_t)0x00000000)			/*!<Pin 0 input mode             */
#define GPIO_PMD_PMD0_OUTPUT		((uint32_t)0x00000001)			/*!<Pin 0 output mode            */
#define GPIO_PMD_PMD0_OPEN_DRAIN    ((uint32_t)0x00000002)			/*!<Pin 0 open drain mode        */
#define GPIO_PMD_PMD0_MASK		    ((uint32_t)0x00000003)			/*!<Mask setting of Pin 0 Mode   */
#define GPIO_PMD_PMD1_INPUT			((uint32_t)0x00000000)			/*!<Pin 1 input mode             */
#define GPIO_PMD_PMD1_OUTPUT		((uint32_t)0x00000004)			/*!<Pin 1 output mode            */
#define GPIO_PMD_PMD1_OPEN_DRAIN    ((uint32_t)0x00000008)			/*!<Pin 1 open drain mode        */
#define GPIO_PMD_PMD1_MASK		    ((uint32_t)0x0000000C)			/*!<Mask setting of Pin 1 Mode   */
#define GPIO_PMD_PMD2_INPUT			((uint32_t)0x00000000)			/*!<Pin 2 input mode             */
#define GPIO_PMD_PMD2_OUTPUT		((uint32_t)0x00000010)			/*!<Pin 2 output mode            */
#define GPIO_PMD_PMD2_OPEN_DRAIN    ((uint32_t)0x00000020)			/*!<Pin 2 open drain mode        */
#define GPIO_PMD_PMD2_MASK		    ((uint32_t)0x00000030)			/*!<Mask setting of Pin 2 Mode   */
#define GPIO_PMD_PMD3_INPUT			((uint32_t)0x00000000)			/*!<Pin 3 input mode             */
#define GPIO_PMD_PMD3_OUTPUT		((uint32_t)0x00000040)			/*!<Pin 3 output mode            */
#define GPIO_PMD_PMD3_OPEN_DRAIN    ((uint32_t)0x00000080)			/*!<Pin 3 open drain mode        */
#define GPIO_PMD_PMD3_MASK		    ((uint32_t)0x000000C0)			/*!<Mask setting of Pin 3 Mode   */
#define GPIO_PMD_PMD4_INPUT			((uint32_t)0x00000000)			/*!<Pin 4 input mode             */
#define GPIO_PMD_PMD4_OUTPUT		((uint32_t)0x00000100)			/*!<Pin 4 output mode            */
#define GPIO_PMD_PMD4_OPEN_DRAIN    ((uint32_t)0x00000200)			/*!<Pin 4 open drain mode        */
#define GPIO_PMD_PMD4_MASK		    ((uint32_t)0x00000300)			/*!<Mask setting of Pin 4 Mode   */
#define GPIO_PMD_PMD5_INPUT			((uint32_t)0x00000000)			/*!<Pin 5 input mode             */
#define GPIO_PMD_PMD5_OUTPUT		((uint32_t)0x00000400)			/*!<Pin 5 output mode            */
#define GPIO_PMD_PMD5_OPEN_DRAIN    ((uint32_t)0x00000800)			/*!<Pin 5 open drain mode        */
#define GPIO_PMD_PMD5_MASK		    ((uint32_t)0x00000C00)			/*!<Mask setting of Pin 5 Mode   */
#define GPIO_PMD_PMD6_INPUT			((uint32_t)0x00000000)			/*!<Pin 6 input mode             */
#define GPIO_PMD_PMD6_OUTPUT		((uint32_t)0x00001000)			/*!<Pin 6 output mode            */
#define GPIO_PMD_PMD6_OPEN_DRAIN    ((uint32_t)0x00002000)			/*!<Pin 6 open drain mode        */
#define GPIO_PMD_PMD6_MASK		    ((uint32_t)0x00003000)			/*!<Mask setting of Pin 6 Mode   */
#define GPIO_PMD_PMD7_INPUT			((uint32_t)0x00000000)			/*!<Pin 7 input mode             */
#define GPIO_PMD_PMD7_OUTPUT		((uint32_t)0x00004000)			/*!<Pin 7 output mode            */
#define GPIO_PMD_PMD7_OPEN_DRAIN    ((uint32_t)0x00008000)			/*!<Pin 7 open drain mode        */
#define GPIO_PMD_PMD7_MASK		    ((uint32_t)0x0000C000)			/*!<Mask setting of Pin 7 Mode   */
#define GPIO_PMD_PMD8_INPUT			((uint32_t)0x00000000)			/*!<Pin 8 input mode             */
#define GPIO_PMD_PMD8_OUTPUT		((uint32_t)0x00010000)			/*!<Pin 8 output mode            */
#define GPIO_PMD_PMD8_OPEN_DRAIN    ((uint32_t)0x00020000)			/*!<Pin 8 open drain mode        */
#define GPIO_PMD_PMD8_MASK		    ((uint32_t)0x00030000)			/*!<Mask setting of Pin 8 Mode   */
#define GPIO_PMD_PMD9_INPUT			((uint32_t)0x00000000)			/*!<Pin 9 input mode             */
#define GPIO_PMD_PMD9_OUTPUT		((uint32_t)0x00040000)			/*!<Pin 9 output mode            */
#define GPIO_PMD_PMD9_OPEN_DRAIN    ((uint32_t)0x00080000)			/*!<Pin 9 open drain mode        */
#define GPIO_PMD_PMD9_MASK		    ((uint32_t)0x000C0000)			/*!<Mask setting of Pin 9 Mode   */
#define GPIO_PMD_PMD10_INPUT	    ((uint32_t)0x00000000)			/*!<Pin 10 input mode             */
#define GPIO_PMD_PMD10_OUTPUT		((uint32_t)0x00100000)			/*!<Pin 10 output mode            */
#define GPIO_PMD_PMD10_OPEN_DRAIN   ((uint32_t)0x00200000)			/*!<Pin 10 open drain mode        */
#define GPIO_PMD_PMD10_MASK		    ((uint32_t)0x00300000)			/*!<Mask setting of Pin 10 Mode   */
#define GPIO_PMD_PMD11_INPUT	    ((uint32_t)0x00000000)			/*!<Pin 11 input mode             */
#define GPIO_PMD_PMD11_OUTPUT		((uint32_t)0x00400000)			/*!<Pin 11 output mode            */
#define GPIO_PMD_PMD11_OPEN_DRAIN   ((uint32_t)0x00800000)			/*!<Pin 11 open drain mode        */
#define GPIO_PMD_PMD11_MASK		    ((uint32_t)0x00C00000)			/*!<Mask setting of Pin 11 Mode   */
#define GPIO_PMD_PMD12_INPUT	    ((uint32_t)0x00000000)			/*!<Pin 12 input mode             */
#define GPIO_PMD_PMD12_OUTPUT		((uint32_t)0x01000000)			/*!<Pin 12 output mode            */
#define GPIO_PMD_PMD12_OPEN_DRAIN   ((uint32_t)0x02000000)			/*!<Pin 12 open drain mode        */
#define GPIO_PMD_PMD12_MASK		    ((uint32_t)0x03000000)			/*!<Mask setting of Pin 12 Mode   */
#define GPIO_PMD_PMD13_INPUT	    ((uint32_t)0x00000000)			/*!<Pin 13 input mode             */
#define GPIO_PMD_PMD13_OUTPUT		((uint32_t)0x04000000)			/*!<Pin 13 output mode            */
#define GPIO_PMD_PMD13_OPEN_DRAIN   ((uint32_t)0x08000000)			/*!<Pin 13 open drain mode        */
#define GPIO_PMD_PMD13_MASK		    ((uint32_t)0x0C000000)			/*!<Mask setting of Pin 13 Mode   */
#define GPIO_PMD_PMD14_INPUT	    ((uint32_t)0x00000000)			/*!<Pin 14 input mode             */
#define GPIO_PMD_PMD14_OUTPUT		((uint32_t)0x10000000)			/*!<Pin 14 output mode            */
#define GPIO_PMD_PMD14_OPEN_DRAIN   ((uint32_t)0x20000000)			/*!<Pin 14 open drain mode        */
#define GPIO_PMD_PMD14_MASK		    ((uint32_t)0x30000000)			/*!<Mask setting of Pin 14 Mode   */
#define GPIO_PMD_PMD15_INPUT	    ((uint32_t)0x00000000)			/*!<Pin 15 input mode             */
#define GPIO_PMD_PMD15_OUTPUT		((uint32_t)0x40000000)			/*!<Pin 15 output mode            */
#define GPIO_PMD_PMD15_OPEN_DRAIN   ((uint32_t)0x80000000)			/*!<Pin 15 open drain mode        */
#define GPIO_PMD_PMD15_MASK		    ((uint32_t)0xC0000000)			/*!<Mask setting of Pin 15 Mode   */

/********************* Bit definition of OFFD register **********************/
#define GPIO_OFFD_OFFD_0		    ((uint32_t)0x00010000)			/*!<Pin 0 digital input path disable */
#define GPIO_OFFD_OFFD_1		    ((uint32_t)0x00020000)			/*!<Pin 1 digital input path disable */
#define GPIO_OFFD_OFFD_2		    ((uint32_t)0x00040000)			/*!<Pin 2 digital input path disable */
#define GPIO_OFFD_OFFD_3		    ((uint32_t)0x00080000)			/*!<Pin 3 digital input path disable */
#define GPIO_OFFD_OFFD_4		    ((uint32_t)0x00100000)			/*!<Pin 4 digital input path disable */
#define GPIO_OFFD_OFFD_5		    ((uint32_t)0x00200000)			/*!<Pin 5 digital input path disable */
#define GPIO_OFFD_OFFD_6		    ((uint32_t)0x00400000)			/*!<Pin 6 digital input path disable */
#define GPIO_OFFD_OFFD_7		    ((uint32_t)0x00800000)			/*!<Pin 7 digital input path disable */
#define GPIO_OFFD_OFFD_8		    ((uint32_t)0x01000000)			/*!<Pin 8 digital input path disable */
#define GPIO_OFFD_OFFD_9		    ((uint32_t)0x02000000)			/*!<Pin 9 digital input path disable */
#define GPIO_OFFD_OFFD_10		    ((uint32_t)0x04000000)			/*!<Pin 10 digital input path disable */
#define GPIO_OFFD_OFFD_11		    ((uint32_t)0x08000000)			/*!<Pin 11 digital input path disable */
#define GPIO_OFFD_OFFD_12		    ((uint32_t)0x10000000)			/*!<Pin 12 digital input path disable */
#define GPIO_OFFD_OFFD_13		    ((uint32_t)0x20000000)			/*!<Pin 13 digital input path disable */
#define GPIO_OFFD_OFFD_14		    ((uint32_t)0x40000000)			/*!<Pin 14 digital input path disable */
#define GPIO_OFFD_OFFD_15		    ((uint32_t)0x80000000)			/*!<Pin 15 digital input path disable */
#define GPIO_OFFD_OFFD_MASK		    ((uint32_t)0xFFFF0000)			/*!<Mask settings of Pin Digital Input Path Disable */

/********************* Bit definition of DOUT register ************************/
#define GPIO_DOUT_DOUT_0		    ((uint32_t)0x00000001)			/*!<Pin 0 drive High */
#define GPIO_DOUT_DOUT_1		    ((uint32_t)0x00000002)			/*!<Pin 1 drive High */
#define GPIO_DOUT_DOUT_2		    ((uint32_t)0x00000004)			/*!<Pin 2 drive High */
#define GPIO_DOUT_DOUT_3		    ((uint32_t)0x00000008)			/*!<Pin 3 drive High */
#define GPIO_DOUT_DOUT_4		    ((uint32_t)0x00000010)			/*!<Pin 4 drive High */
#define GPIO_DOUT_DOUT_5		    ((uint32_t)0x00000020)			/*!<Pin 5 drive High */
#define GPIO_DOUT_DOUT_6		    ((uint32_t)0x00000040)			/*!<Pin 6 drive High */
#define GPIO_DOUT_DOUT_7		    ((uint32_t)0x00000080)			/*!<Pin 7 drive High */
#define GPIO_DOUT_DOUT_8		    ((uint32_t)0x00000100)			/*!<Pin 8 drive High */
#define GPIO_DOUT_DOUT_9		    ((uint32_t)0x00000200)			/*!<Pin 9 drive High */
#define GPIO_DOUT_DOUT_10		    ((uint32_t)0x00000400)			/*!<Pin 10 drive High */
#define GPIO_DOUT_DOUT_11		    ((uint32_t)0x00000800)			/*!<Pin 11 drive High */
#define GPIO_DOUT_DOUT_12		    ((uint32_t)0x00001000)			/*!<Pin 12 drive High */
#define GPIO_DOUT_DOUT_13		    ((uint32_t)0x00002000)			/*!<Pin 13 drive High */
#define GPIO_DOUT_DOUT_14		    ((uint32_t)0x00004000)			/*!<Pin 14 drive High */
#define GPIO_DOUT_DOUT_15		    ((uint32_t)0x00008000)			/*!<Pin 15 drive High */
#define GPIO_DOUT_DOUT_MASK		    ((uint32_t)0x0000FFFF)			/*!<Mask settings of Port Data Optput Value */

/********************* Bit definition of DMASK register ***********************/
#define GPIO_DMASK_DMASK_0		    ((uint32_t)0x00000001)			/*!<Pin 0 DOUT Mask enable */
#define GPIO_DMASK_DMASK_1		    ((uint32_t)0x00000002)			/*!<Pin 1 DOUT Mask enable */
#define GPIO_DMASK_DMASK_2		    ((uint32_t)0x00000004)			/*!<Pin 2 DOUT Mask enable */
#define GPIO_DMASK_DMASK_3		    ((uint32_t)0x00000008)			/*!<Pin 3 DOUT Mask enable */
#define GPIO_DMASK_DMASK_4		    ((uint32_t)0x00000010)			/*!<Pin 4 DOUT Mask enable */
#define GPIO_DMASK_DMASK_5		    ((uint32_t)0x00000020)			/*!<Pin 5 DOUT Mask enable */
#define GPIO_DMASK_DMASK_6		    ((uint32_t)0x00000040)			/*!<Pin 6 DOUT Mask enable */
#define GPIO_DMASK_DMASK_7		    ((uint32_t)0x00000080)			/*!<Pin 7 DOUT Mask enable */
#define GPIO_DMASK_DMASK_8		    ((uint32_t)0x00000100)			/*!<Pin 8 DOUT Mask enable */
#define GPIO_DMASK_DMASK_9		    ((uint32_t)0x00000200)			/*!<Pin 9 DOUT Mask enable */
#define GPIO_DMASK_DMASK_10		    ((uint32_t)0x00000400)			/*!<Pin 10 DOUT Mask enable */
#define GPIO_DMASK_DMASK_11		    ((uint32_t)0x00000800)			/*!<Pin 11 DOUT Mask enable */
#define GPIO_DMASK_DMASK_12		    ((uint32_t)0x00001000)			/*!<Pin 12 DOUT Mask enable */
#define GPIO_DMASK_DMASK_13		    ((uint32_t)0x00002000)			/*!<Pin 13 DOUT Mask enable */
#define GPIO_DMASK_DMASK_14		    ((uint32_t)0x00004000)			/*!<Pin 14 DOUT Mask enable */
#define GPIO_DMASK_DMASK_15		    ((uint32_t)0x00008000)			/*!<Pin 15 DOUT Mask enable */
#define GPIO_DMASK_DMASK_MASK	    ((uint32_t)0x0000FFFF)			/*!<Mask settings of Port Data Output Write Mask */

/********************* Bit definition of PIN register *************************/
#define GPIO_PIN_PIN_0		        ((uint32_t)0x00000001)			/*!<Indicates Pin 0 PIN value is High */
#define GPIO_PIN_PIN_1		        ((uint32_t)0x00000002)			/*!<Indicates Pin 1 PIN value is High */
#define GPIO_PIN_PIN_2		        ((uint32_t)0x00000004)			/*!<Indicates Pin 2 PIN value is High */
#define GPIO_PIN_PIN_3		        ((uint32_t)0x00000008)			/*!<Indicates Pin 3 PIN value is High */
#define GPIO_PIN_PIN_4		        ((uint32_t)0x00000010)			/*!<Indicates Pin 4 PIN value is High */
#define GPIO_PIN_PIN_5		        ((uint32_t)0x00000020)			/*!<Indicates Pin 5 PIN value is High */
#define GPIO_PIN_PIN_6		        ((uint32_t)0x00000040)			/*!<Indicates Pin 6 PIN value is High */
#define GPIO_PIN_PIN_7		        ((uint32_t)0x00000080)			/*!<Indicates Pin 7 PIN value is High */
#define GPIO_PIN_PIN_8		        ((uint32_t)0x00000100)			/*!<Indicates Pin 8 PIN value is High */
#define GPIO_PIN_PIN_9		        ((uint32_t)0x00000200)			/*!<Indicates Pin 9 PIN value is High */
#define GPIO_PIN_PIN_10		        ((uint32_t)0x00000400)			/*!<Indicates Pin 10 PIN value is High */
#define GPIO_PIN_PIN_11		        ((uint32_t)0x00000800)			/*!<Indicates Pin 11 PIN value is High */
#define GPIO_PIN_PIN_12		        ((uint32_t)0x00001000)			/*!<Indicates Pin 12 PIN value is High */
#define GPIO_PIN_PIN_13		        ((uint32_t)0x00002000)			/*!<Indicates Pin 13 PIN value is High */
#define GPIO_PIN_PIN_14		        ((uint32_t)0x00004000)			/*!<Indicates Pin 14 PIN value is High */
#define GPIO_PIN_PIN_15		        ((uint32_t)0x00008000)			/*!<Indicates Pin 15 PIN value is High */

/********************* Bit definition of DBEN register ************************/
#define GPIO_DBEN_DBEN_0		    ((uint32_t)0x00000001)			/*!<Pin 0 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_1		    ((uint32_t)0x00000002)			/*!<Pin 1 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_2		    ((uint32_t)0x00000004)			/*!<Pin 2 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_3		    ((uint32_t)0x00000008)			/*!<Pin 3 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_4		    ((uint32_t)0x00000010)			/*!<Pin 4 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_5		    ((uint32_t)0x00000020)			/*!<Pin 5 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_6		    ((uint32_t)0x00000040)			/*!<Pin 6 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_7		    ((uint32_t)0x00000080)			/*!<Pin 7 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_8		    ((uint32_t)0x00000100)			/*!<Pin 8 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_9		    ((uint32_t)0x00000200)			/*!<Pin 9 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_10		    ((uint32_t)0x00000400)			/*!<Pin 10 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_11		    ((uint32_t)0x00000800)			/*!<Pin 11 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_12		    ((uint32_t)0x00001000)			/*!<Pin 12 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_13		    ((uint32_t)0x00002000)			/*!<Pin 13 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_14		    ((uint32_t)0x00004000)			/*!<Pin 14 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_15		    ((uint32_t)0x00008000)			/*!<Pin 15 Input Signal De-bounce Enable */
#define GPIO_DBEN_DBEN_MASK	        ((uint32_t)0x0000FFFF)			/*!<Mask settings of Port Input Signal De-bounce Enable */

/********************* Bit definition of IMD register *************************/
#define GPIO_IMD_EDGE_0				((uint32_t)0x00000000)			/*!<Pin 0 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_0			((uint32_t)0x00000001)			/*!<Pin 0 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_1		        ((uint32_t)0x00000000)			/*!<Pin 1 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_1		    ((uint32_t)0x00000002)			/*!<Pin 1 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_2		        ((uint32_t)0x00000000)			/*!<Pin 2 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_2		    ((uint32_t)0x00000004)			/*!<Pin 2 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_3		        ((uint32_t)0x00000000)			/*!<Pin 3 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_3		    ((uint32_t)0x00000008)			/*!<Pin 3 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_4		        ((uint32_t)0x00000000)			/*!<Pin 4 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_4		    ((uint32_t)0x00000010)			/*!<Pin 4 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_5		        ((uint32_t)0x00000000)			/*!<Pin 5 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_5		    ((uint32_t)0x00000020)			/*!<Pin 5 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_6		        ((uint32_t)0x00000000)			/*!<Pin 6 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_6		    ((uint32_t)0x00000040)			/*!<Pin 6 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_7		        ((uint32_t)0x00000000)			/*!<Pin 7 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_7		    ((uint32_t)0x00000080)			/*!<Pin 7 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_8		        ((uint32_t)0x00000000)			/*!<Pin 8 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_8		    ((uint32_t)0x00000100)			/*!<Pin 8 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_9		        ((uint32_t)0x00000000)			/*!<Pin 9 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_9		    ((uint32_t)0x00000200)			/*!<Pin 9 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_10		    ((uint32_t)0x00000000)			/*!<Pin 10 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_10		    ((uint32_t)0x00000400)			/*!<Pin 10 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_11		    ((uint32_t)0x00000000)			/*!<Pin 11 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_11		    ((uint32_t)0x00000800)			/*!<Pin 11 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_12		    ((uint32_t)0x00000000)			/*!<Pin 12 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_12		    ((uint32_t)0x00001000)			/*!<Pin 12 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_13		    ((uint32_t)0x00000000)			/*!<Pin 13 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_13		    ((uint32_t)0x00002000)			/*!<Pin 13 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_14		    ((uint32_t)0x00000000)			/*!<Pin 14 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_14		    ((uint32_t)0x00004000)			/*!<Pin 14 Level Detection Interrupt Enable */
#define GPIO_IMD_EDGE_15		    ((uint32_t)0x00000000)			/*!<Pin 15 Edge Detection Interrupt Enable */
#define GPIO_IMD_LEVEL_15		    ((uint32_t)0x00008000)			/*!<Pin 15 Level Detection Interrupt Enable */
#define GPIO_IMD_IMD_MASK	        ((uint32_t)0x0000FFFF)			/*!<Mask settings of Port Edge or Level Detection Interrupt Control */

/********************* Bit definition of IER register *************************/
#define GPIO_IER_IF_EN_0		    ((uint32_t)0x00000001)			/*!<Pin 0 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_1		    ((uint32_t)0x00000002)			/*!<Pin 1 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_2		    ((uint32_t)0x00000004)			/*!<Pin 2 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_3		    ((uint32_t)0x00000008)			/*!<Pin 3 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_4		    ((uint32_t)0x00000010)			/*!<Pin 4 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_5		    ((uint32_t)0x00000020)			/*!<Pin 5 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_6		    ((uint32_t)0x00000040)			/*!<Pin 6 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_7		    ((uint32_t)0x00000080)			/*!<Pin 7 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_8		    ((uint32_t)0x00000100)			/*!<Pin 8 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_9		    ((uint32_t)0x00000200)			/*!<Pin 9 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_10		    ((uint32_t)0x00000400)			/*!<Pin 10 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_11		    ((uint32_t)0x00000800)			/*!<Pin 11 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_12		    ((uint32_t)0x00001000)			/*!<Pin 12 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_13		    ((uint32_t)0x00002000)			/*!<Pin 13 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_14		    ((uint32_t)0x00004000)			/*!<Pin 14 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_15		    ((uint32_t)0x00008000)			/*!<Pin 15 Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IF_EN_MASK		    ((uint32_t)0x0000FFFF)			/*!<Mask settings of Port Interrupt Enable by Input Falling Edge or Input Level Low */
#define GPIO_IER_IR_EN_0		    ((uint32_t)0x00010000)			/*!<Pin 0 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_1		    ((uint32_t)0x00020000)			/*!<Pin 1 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_2		    ((uint32_t)0x00040000)			/*!<Pin 2 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_3		    ((uint32_t)0x00080000)			/*!<Pin 3 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_4		    ((uint32_t)0x00100000)			/*!<Pin 4 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_5		    ((uint32_t)0x00200000)			/*!<Pin 5 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_6		    ((uint32_t)0x00400000)			/*!<Pin 6 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_7		    ((uint32_t)0x00800000)			/*!<Pin 7 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_8		    ((uint32_t)0x01000000)			/*!<Pin 8 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_9		    ((uint32_t)0x02000000)			/*!<Pin 9 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_10		    ((uint32_t)0x04000000)			/*!<Pin 10 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_11		    ((uint32_t)0x08000000)			/*!<Pin 11 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_12		    ((uint32_t)0x10000000)			/*!<Pin 12 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_13		    ((uint32_t)0x20000000)			/*!<Pin 13 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_14		    ((uint32_t)0x40000000)			/*!<Pin 14 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_15		    ((uint32_t)0x80000000)			/*!<Pin 15 Interrupt Enable by Input Rising Edge or Input Level High */
#define GPIO_IER_IR_EN_MASK		    ((uint32_t)0xFFFF0000)			/*!<Mask settings of Port Interrupt Enable by Input Rising Edge or Input Level High */

/********************* Bit definition of ISR register *************************/
#define GPIO_ISR_ISR_0		        ((uint32_t)0x00000001)			/*!<Indicates Pin 0 generate an interrupt */
#define GPIO_ISR_ISR_1		        ((uint32_t)0x00000002)			/*!<Indicates Pin 1 generate an interrupt */
#define GPIO_ISR_ISR_2		        ((uint32_t)0x00000004)			/*!<Indicates Pin 2 generate an interrupt */
#define GPIO_ISR_ISR_3		        ((uint32_t)0x00000008)			/*!<Indicates Pin 3 generate an interrupt */
#define GPIO_ISR_ISR_4		        ((uint32_t)0x00000010)			/*!<Indicates Pin 4 generate an interrupt */
#define GPIO_ISR_ISR_5		        ((uint32_t)0x00000020)			/*!<Indicates Pin 5 generate an interrupt */
#define GPIO_ISR_ISR_6		        ((uint32_t)0x00000040)			/*!<Indicates Pin 6 generate an interrupt */
#define GPIO_ISR_ISR_7		        ((uint32_t)0x00000080)			/*!<Indicates Pin 7 generate an interrupt */
#define GPIO_ISR_ISR_8		        ((uint32_t)0x00000100)			/*!<Indicates Pin 8 generate an interrupt */
#define GPIO_ISR_ISR_9		        ((uint32_t)0x00000200)			/*!<Indicates Pin 9 generate an interrupt */
#define GPIO_ISR_ISR_10		        ((uint32_t)0x00000400)			/*!<Indicates Pin 10 generate an interrupt */
#define GPIO_ISR_ISR_11		        ((uint32_t)0x00000800)			/*!<Indicates Pin 11 generate an interrupt */
#define GPIO_ISR_ISR_12		        ((uint32_t)0x00001000)			/*!<Indicates Pin 12 generate an interrupt */
#define GPIO_ISR_ISR_13		        ((uint32_t)0x00002000)			/*!<Indicates Pin 13 generate an interrupt */
#define GPIO_ISR_ISR_14		        ((uint32_t)0x00004000)			/*!<Indicates Pin 14 generate an interrupt */
#define GPIO_ISR_ISR_15		        ((uint32_t)0x00008000)			/*!<Indicates Pin 15 generate an interrupt */

/********************* Bit definition of PUEN register *************************/
#define GPIO_PUEN_PUEN_0		    ((uint32_t)0x00000001)			/*!<Pin 0 Pull-Up Enable */
#define GPIO_PUEN_PUEN_1		    ((uint32_t)0x00000002)			/*!<Pin 1 Pull-Up Enable */
#define GPIO_PUEN_PUEN_2		    ((uint32_t)0x00000004)			/*!<Pin 2 Pull-Up Enable */
#define GPIO_PUEN_PUEN_3		    ((uint32_t)0x00000008)			/*!<Pin 3 Pull-Up Enable */
#define GPIO_PUEN_PUEN_4		    ((uint32_t)0x00000010)			/*!<Pin 4 Pull-Up Enable */
#define GPIO_PUEN_PUEN_5		    ((uint32_t)0x00000020)			/*!<Pin 5 Pull-Up Enable */
#define GPIO_PUEN_PUEN_6		    ((uint32_t)0x00000040)			/*!<Pin 6 Pull-Up Enable */
#define GPIO_PUEN_PUEN_7		    ((uint32_t)0x00000080)			/*!<Pin 7 Pull-Up Enable */
#define GPIO_PUEN_PUEN_8		    ((uint32_t)0x00000100)			/*!<Pin 8 Pull-Up Enable */
#define GPIO_PUEN_PUEN_9		    ((uint32_t)0x00000200)			/*!<Pin 9 Pull-Up Enable */
#define GPIO_PUEN_PUEN_10		    ((uint32_t)0x00000400)			/*!<Pin 10 Pull-Up Enable */
#define GPIO_PUEN_PUEN_11		    ((uint32_t)0x00000800)			/*!<Pin 11 Pull-Up Enable */
#define GPIO_PUEN_PUEN_12		    ((uint32_t)0x00001000)			/*!<Pin 12 Pull-Up Enable */
#define GPIO_PUEN_PUEN_13		    ((uint32_t)0x00002000)			/*!<Pin 13 Pull-Up Enable */
#define GPIO_PUEN_PUEN_14		    ((uint32_t)0x00004000)			/*!<Pin 14 Pull-Up Enable */
#define GPIO_PUEN_PUEN_15		    ((uint32_t)0x00008000)			/*!<Pin 15 Pull-Up Enable */

/********************* Bit definition of DBNCECON register ********************/
#define GPIO_DBNCECON_DBCLKSEL_0    ((uint32_t)0x00000000)			/*!<Sample interrupt input once per 1 clocks            */
#define GPIO_DBNCECON_DBCLKSEL_1    ((uint32_t)0x00000001)			/*!<Sample interrupt input once per 2 clocks            */
#define GPIO_DBNCECON_DBCLKSEL_2    ((uint32_t)0x00000002)			/*!<Sample interrupt input once per 4 clocks            */
#define GPIO_DBNCECON_DBCLKSEL_3    ((uint32_t)0x00000003)			/*!<Sample interrupt input once per 8 clocks            */
#define GPIO_DBNCECON_DBCLKSEL_4    ((uint32_t)0x00000004)			/*!<Sample interrupt input once per 16 clocks           */
#define GPIO_DBNCECON_DBCLKSEL_5    ((uint32_t)0x00000005)			/*!<Sample interrupt input once per 32 clocks           */
#define GPIO_DBNCECON_DBCLKSEL_6    ((uint32_t)0x00000006)			/*!<Sample interrupt input once per 64 clocks           */
#define GPIO_DBNCECON_DBCLKSEL_7    ((uint32_t)0x00000007)			/*!<Sample interrupt input once per 128 clocks          */
#define GPIO_DBNCECON_DBCLKSEL_8    ((uint32_t)0x00000008)			/*!<Sample interrupt input once per 256 clocks          */
#define GPIO_DBNCECON_DBCLKSEL_9    ((uint32_t)0x00000009)			/*!<Sample interrupt input once per (2*256) clocks      */
#define GPIO_DBNCECON_DBCLKSEL_10   ((uint32_t)0x0000000A)			/*!<Sample interrupt input once per (4*256) clocks      */
#define GPIO_DBNCECON_DBCLKSEL_11   ((uint32_t)0x0000000B)			/*!<Sample interrupt input once per (8*256) clocks      */
#define GPIO_DBNCECON_DBCLKSEL_12   ((uint32_t)0x0000000C)			/*!<Sample interrupt input once per (16*256) clocks     */
#define GPIO_DBNCECON_DBCLKSEL_13   ((uint32_t)0x0000000D)			/*!<Sample interrupt input once per (32*256) clocks     */
#define GPIO_DBNCECON_DBCLKSEL_14   ((uint32_t)0x0000000E)			/*!<Sample interrupt input once per (64*256) clocks     */
#define GPIO_DBNCECON_DBCLKSEL_15   ((uint32_t)0x0000000F)			/*!<Sample interrupt input once per (128*256) clocks    */
#define GPIO_DBNCECON_DBCLKSEL_MASK ((uint32_t)0x0000000F)			/*!<Mask settings of De-bounce sampling cycle selection */
#define GPIO_DBNCECON_DBCLKSRC_HCLK ((uint32_t)0x00000000)			/*!<De-bounce counter clock source is the HCLK            */
#define GPIO_DBNCECON_DBCLKSRC_10K  ((uint32_t)0x00000010)			/*!<De-bounce counter clock source is the internal 10 kHz */
#define GPIO_DBNCECON_ICLK_ON       ((uint32_t)0x00000020)			/*!<Interrupt clock on */


/******************************************************************************/
/*                SHADOW                                                      */
/******************************************************************************/
/********************* Bit definition of GPASHADOW register **********************/
#define SHADOW_GPA_PIN0 			((uint32_t)0x00000001)			/*!<GPIO Port A Pin 0 */
#define SHADOW_GPA_PIN1 			((uint32_t)0x00000002)			/*!<GPIO Port A Pin 1 */
#define SHADOW_GPA_PIN2 			((uint32_t)0x00000004)			/*!<GPIO Port A Pin 2 */
#define SHADOW_GPA_PIN3 			((uint32_t)0x00000008)			/*!<GPIO Port A Pin 3 */
#define SHADOW_GPA_PIN4 			((uint32_t)0x00000010)			/*!<GPIO Port A Pin 4 */
#define SHADOW_GPA_PIN5 			((uint32_t)0x00000020)			/*!<GPIO Port A Pin 5 */
#define SHADOW_GPA_PIN6 			((uint32_t)0x00000040)			/*!<GPIO Port A Pin 6 */
#define SHADOW_GPA_PIN7 			((uint32_t)0x00000080)			/*!<GPIO Port A Pin 7 */
#define SHADOW_GPA_PIN8 			((uint32_t)0x00000100)			/*!<GPIO Port A Pin 8 */
#define SHADOW_GPA_PIN9 			((uint32_t)0x00000200)			/*!<GPIO Port A Pin 9 */
#define SHADOW_GPA_PIN10 			((uint32_t)0x00000400)			/*!<GPIO Port A Pin 10 */
#define SHADOW_GPA_PIN11 			((uint32_t)0x00000800)			/*!<GPIO Port A Pin 11 */
#define SHADOW_GPA_PIN12 			((uint32_t)0x00001000)			/*!<GPIO Port A Pin 12 */
#define SHADOW_GPA_PIN13 			((uint32_t)0x00002000)			/*!<GPIO Port Pin 13 */
#define SHADOW_GPA_PIN14 			((uint32_t)0x00004000)			/*!<GPIO Port Pin 14 */
#define SHADOW_GPA_PIN15 			((uint32_t)0x00008000)			/*!<GPIO Port Pin 15 */

/********************* Bit definition of GPBSHADOW register **********************/
#define SHADOW_GPB_PIN0 			((uint32_t)0x00000001)			/*!<GPIO Port Pin 0 */
#define SHADOW_GPB_PIN1 			((uint32_t)0x00000002)			/*!<GPIO Port Pin 1 */
#define SHADOW_GPB_PIN2 			((uint32_t)0x00000004)			/*!<GPIO Port Pin 2 */
#define SHADOW_GPB_PIN3 			((uint32_t)0x00000008)			/*!<GPIO Port Pin 3 */
#define SHADOW_GPB_PIN4 			((uint32_t)0x00000010)			/*!<GPIO Port Pin 4 */
#define SHADOW_GPB_PIN5 			((uint32_t)0x00000020)			/*!<GPIO Port Pin 5 */
#define SHADOW_GPB_PIN6 			((uint32_t)0x00000040)			/*!<GPIO Port Pin 6 */
#define SHADOW_GPB_PIN7 			((uint32_t)0x00000080)			/*!<GPIO Port Pin 7 */
#define SHADOW_GPB_PIN8 			((uint32_t)0x00000100)			/*!<GPIO Port Pin 8 */
#define SHADOW_GPB_PIN9 			((uint32_t)0x00000200)			/*!<GPIO Port Pin 9 */
#define SHADOW_GPB_PIN10 			((uint32_t)0x00000400)			/*!<GPIO Port Pin 10 */
#define SHADOW_GPB_PIN11 			((uint32_t)0x00000800)			/*!<GPIO Port Pin 11 */
#define SHADOW_GPB_PIN12 			((uint32_t)0x00001000)			/*!<GPIO Port Pin 12 */
#define SHADOW_GPB_PIN13 			((uint32_t)0x00002000)			/*!<GPIO Port Pin 13 */
#define SHADOW_GPB_PIN14 			((uint32_t)0x00004000)			/*!<GPIO Port Pin 14 */
#define SHADOW_GPB_PIN15 			((uint32_t)0x00008000)			/*!<GPIO Port Pin 15 */

/********************* Bit definition of GPCSHADOW register **********************/
#define SHADOW_GPC_PIN0 			((uint32_t)0x00000001)			/*!<GPIO Port Pin 0 */
#define SHADOW_GPC_PIN1 			((uint32_t)0x00000002)			/*!<GPIO Port Pin 1 */
#define SHADOW_GPC_PIN2 			((uint32_t)0x00000004)			/*!<GPIO Port Pin 2 */
#define SHADOW_GPC_PIN3 			((uint32_t)0x00000008)			/*!<GPIO Port Pin 3 */
#define SHADOW_GPC_PIN4 			((uint32_t)0x00000010)			/*!<GPIO Port Pin 4 */
#define SHADOW_GPC_PIN5 			((uint32_t)0x00000020)			/*!<GPIO Port Pin 5 */
#define SHADOW_GPC_PIN6 			((uint32_t)0x00000040)			/*!<GPIO Port Pin 6 */
#define SHADOW_GPC_PIN7 			((uint32_t)0x00000080)			/*!<GPIO Port Pin 7 */
#define SHADOW_GPC_PIN8 			((uint32_t)0x00000100)			/*!<GPIO Port Pin 8 */
#define SHADOW_GPC_PIN9 			((uint32_t)0x00000200)			/*!<GPIO Port Pin 9 */
#define SHADOW_GPC_PIN10 			((uint32_t)0x00000400)			/*!<GPIO Port Pin 10 */
#define SHADOW_GPC_PIN11 			((uint32_t)0x00000800)			/*!<GPIO Port Pin 11 */
#define SHADOW_GPC_PIN12 			((uint32_t)0x00001000)			/*!<GPIO Port Pin 12 */
#define SHADOW_GPC_PIN13 			((uint32_t)0x00002000)			/*!<GPIO Port Pin 13 */
#define SHADOW_GPC_PIN14 			((uint32_t)0x00004000)			/*!<GPIO Port Pin 14 */
#define SHADOW_GPC_PIN15 			((uint32_t)0x00008000)			/*!<GPIO Port Pin 15 */

/********************* Bit definition of GPDSHADOW register **********************/
#define SHADOW_GPD_PIN0 			((uint32_t)0x00000001)			/*!<GPIO Port Pin 0 */
#define SHADOW_GPD_PIN1 			((uint32_t)0x00000002)			/*!<GPIO Port Pin 1 */
#define SHADOW_GPD_PIN2 			((uint32_t)0x00000004)			/*!<GPIO Port Pin 2 */
#define SHADOW_GPD_PIN3 			((uint32_t)0x00000008)			/*!<GPIO Port Pin 3 */
#define SHADOW_GPD_PIN4 			((uint32_t)0x00000010)			/*!<GPIO Port Pin 4 */
#define SHADOW_GPD_PIN5 			((uint32_t)0x00000020)			/*!<GPIO Port Pin 5 */
#define SHADOW_GPD_PIN6 			((uint32_t)0x00000040)			/*!<GPIO Port Pin 6 */
#define SHADOW_GPD_PIN7 			((uint32_t)0x00000080)			/*!<GPIO Port Pin 7 */
#define SHADOW_GPD_PIN8 			((uint32_t)0x00000100)			/*!<GPIO Port Pin 8 */
#define SHADOW_GPD_PIN9 			((uint32_t)0x00000200)			/*!<GPIO Port Pin 9 */
#define SHADOW_GPD_PIN10 			((uint32_t)0x00000400)			/*!<GPIO Port Pin 10 */
#define SHADOW_GPD_PIN11 			((uint32_t)0x00000800)			/*!<GPIO Port Pin 11 */
#define SHADOW_GPD_PIN12 			((uint32_t)0x00001000)			/*!<GPIO Port Pin 12 */
#define SHADOW_GPD_PIN13 			((uint32_t)0x00002000)			/*!<GPIO Port Pin 13 */
#define SHADOW_GPD_PIN14 			((uint32_t)0x00004000)			/*!<GPIO Port Pin 14 */
#define SHADOW_GPD_PIN15 			((uint32_t)0x00008000)			/*!<GPIO Port Pin 15 */

/********************* Bit definition of GPESHADOW register **********************/
#define SHADOW_GPE_PIN0 			((uint32_t)0x00000001)			/*!<GPIO Port Pin 0 */
#define SHADOW_GPE_PIN1 			((uint32_t)0x00000002)			/*!<GPIO Port Pin 1 */
#define SHADOW_GPE_PIN2 			((uint32_t)0x00000004)			/*!<GPIO Port Pin 2 */
#define SHADOW_GPE_PIN3 			((uint32_t)0x00000008)			/*!<GPIO Port Pin 3 */
#define SHADOW_GPE_PIN4 			((uint32_t)0x00000010)			/*!<GPIO Port Pin 4 */
#define SHADOW_GPE_PIN5 			((uint32_t)0x00000020)			/*!<GPIO Port Pin 5 */
#define SHADOW_GPE_PIN6 			((uint32_t)0x00000040)			/*!<GPIO Port Pin 6 */
#define SHADOW_GPE_PIN7 			((uint32_t)0x00000080)			/*!<GPIO Port Pin 7 */
#define SHADOW_GPE_PIN8 			((uint32_t)0x00000100)			/*!<GPIO Port Pin 8 */
#define SHADOW_GPE_PIN9 			((uint32_t)0x00000200)			/*!<GPIO Port Pin 9 */
#define SHADOW_GPE_PIN10 			((uint32_t)0x00000400)			/*!<GPIO Port Pin 10 */
#define SHADOW_GPE_PIN11 			((uint32_t)0x00000800)			/*!<GPIO Port Pin 11 */
#define SHADOW_GPE_PIN12 			((uint32_t)0x00001000)			/*!<GPIO Port Pin 12 */
#define SHADOW_GPE_PIN13 			((uint32_t)0x00002000)			/*!<GPIO Port Pin 13 */
#define SHADOW_GPE_PIN14 			((uint32_t)0x00004000)			/*!<GPIO Port Pin 14 */
#define SHADOW_GPE_PIN15 			((uint32_t)0x00008000)			/*!<GPIO Port Pin 15 */

/********************* Bit definition of GPFSHADOW register **********************/
#define SHADOW_GPF_PIN0 			((uint32_t)0x00000001)			/*!<GPIO Port Pin 0 */
#define SHADOW_GPF_PIN1 			((uint32_t)0x00000002)			/*!<GPIO Port Pin 1 */
#define SHADOW_GPF_PIN2 			((uint32_t)0x00000004)			/*!<GPIO Port Pin 2 */
#define SHADOW_GPF_PIN3 			((uint32_t)0x00000008)			/*!<GPIO Port Pin 3 */
#define SHADOW_GPF_PIN4 			((uint32_t)0x00000010)			/*!<GPIO Port Pin 4 */
#define SHADOW_GPF_PIN5 			((uint32_t)0x00000020)			/*!<GPIO Port Pin 5 */


/******************************************************************************/
/*                VDMA                                                        */
/******************************************************************************/
/********************* Bit definition of CSR register **********************/
#define VDMA_CSR_CEN				((uint32_t)0x00000001)			/*!<VDMA Channel Enable */
#define VDMA_CSR_SWRST				((uint32_t)0x00000002)			/*!<VDMA Software Engine Reset */
#define VDMA_CSR_STRIDE_EN			((uint32_t)0x00000400)			/*!<VDMA Stride Mode Enable */
#define VDMA_CSR_DIR_INC			((uint32_t)0x00000000)			/*!<VDMA Transfer Source/Destination Address Direction Select - incremented */
#define VDMA_CSR_DIR_DEC			((uint32_t)0x00000800)			/*!<VDMA Transfer Source/Destination Address Direction Select - decremented */
#define VDMA_CSR_TRIG_EN			((uint32_t)0x00800000)			/*!<VDMA Channel Trigger */

/********************* Bit definition of IER register **********************/
#define VDMA_IER_TABORT				((uint32_t)0x00000001)			/*!<VDMA Read/Write Target Abort Interrupt Enable */
#define VDMA_IER_TD					((uint32_t)0x00000002)			/*!<VDMA Transfer Done Interrupt Enable */

/********************* Bit definition of ISR register **********************/
#define VDMA_ISR_TABORT				((uint32_t)0x00000001)			/*!<VDMA Read/Write Target Abort Interrupt Status Flag */
#define VDMA_ISR_TD					((uint32_t)0x00000002)			/*!<VDMA Transfer Done Interrupt Status Flag */

/********************* Bit definition of SASOCR register **********************/
#define VDMA_SASOCR_SASTOBL_MASK	((uint32_t)0x0000FFFF)			/*!<VDMA Source Address Stride Offset Byte Length Mask */
#define VDMA_SASOCR_STBC_MASK		((uint32_t)0xFFFF0000)			/*!<VDMA Stride Transfer Byte Count Mask */

/********************* Bit definition of DASOCR register **********************/
#define VDMA_DASOCR_DASTOBL_MASK	((uint32_t)0x0000FFFF)			/*!<VDMA Destination Address Stride Offset Byte Length Mask */


/******************************************************************************/
/*                PDMA                                                        */
/******************************************************************************/
/********************* Bit definition of CSR register **********************/
#define PDMA_CSR_CEN				((uint32_t)0x00000001)			/*!<PDMA Channel Enable */
#define PDMA_CSR_SWRST				((uint32_t)0x00000002)			/*!<PDMA Software Engine Reset */
#define PDMA_CSR_MEM2MEM			((uint32_t)0x00000000)			/*!<PDMA Memory to Memory mode (Memory-to-Memory) */
#define PDMA_CSR_IP2MEM				((uint32_t)0x00000004)			/*!<PDMA IP to Memory mode (APB-to-Memory) */
#define PDMA_CSR_MEM2IP				((uint32_t)0x00000008)			/*!<PDMA Memory to IP mode (Memory-to-APB) */
#define PDMA_CSR_SAD_INC			((uint32_t)0x00000000)			/*!<PDMA Transfer Source Address Direction Selection - incremented */
#define PDMA_CSR_SAD_FIX			((uint32_t)0x00000020)			/*!<PDMA Transfer Source Address Direction Selection - fixed */
#define PDMA_CSR_SAD_WRAP			((uint32_t)0x00000030)			/*!<PDMA Transfer Source Address Direction Selection - wrap around */
#define PDMA_CSR_DAD_INC			((uint32_t)0x00000000)			/*!<PDMA Transfer Destination Address Direction Selection - incremented */
#define PDMA_CSR_DAD_FIX			((uint32_t)0x00000080)			/*!<PDMA Transfer Destination Address Direction Selection - fixed */
#define PDMA_CSR_DAD_WRAP			((uint32_t)0x000000C0)			/*!<PDMA Transfer Destination Address Direction Selection - wrap around */
#define PDMA_CSR_TOEN				((uint32_t)0x00001000)			/*!<PDMA Time-out Enable */
#define PDMA_CSR_APB_TWS_8BIT		((uint32_t)0x00080000)			/*!<PDMA Peripheral Transfer Width - 8-bit */
#define PDMA_CSR_APB_TWS_16BIT		((uint32_t)0x00100000)			/*!<PDMA Peripheral Transfer Width - 16-bit */
#define PDMA_CSR_APB_TWS_32BIT		((uint32_t)0x00000000)			/*!<PDMA Peripheral Transfer Width - 32-bit */
#define PDMA_CSR_APB_TWS_MASK		((uint32_t)0x00180000)			/*!<PDMA Peripheral Transfer Width Mask */
#define PDMA_CSR_TRIG_EN			((uint32_t)0x00800000)			/*!<PDMA Channel Trigger */

/********************* Bit definition of IER register **********************/
#define PDMA_IER_TABORT				((uint32_t)0x00000001)			/*!<PDMA Read/Write Target Abort Interrupt Enable */
#define PDMA_IER_TD					((uint32_t)0x00000002)			/*!<PDMA Transfer Done Interrupt Enable */
#define PDMA_IER_WRA_BCR_DONE		((uint32_t)0x00000004)			/*!<PDMA Wrap Around Byte Count Interrupt Enable - CBCR = 0 */
#define PDMA_IER_WRA_BCR_HALF		((uint32_t)0x00000010)			/*!<PDMA Wrap Around Byte Count Interrupt Enable - 1/2 CBCR */
#define PDMA_IER_TO					((uint32_t)0x00000040)			/*!<PDMA Time-Out Interrupt Enable */

/********************* Bit definition of ISR register **********************/
#define PDMA_ISR_TABORT				((uint32_t)0x00000001)			/*!<PDMA Read/Write Target Abort Interrupt Status Flag */
#define PDMA_ISR_TD					((uint32_t)0x00000002)			/*!<PDMA Transfer Done Interrupt Status Flag */
#define PDMA_ISR_WRA_BCR_DONE		((uint32_t)0x00000004)			/*!<PDMA Wrap Around Byte Count Interrupt Status Flag - CBCR = 0 */
#define PDMA_ISR_WRA_BCR_HALF		((uint32_t)0x00000010)			/*!<PDMA Wrap Around Byte Count Interrupt Status Flag - 1/2 CBCR */
#define PDMA_ISR_TO					((uint32_t)0x00000040)			/*!<PDMA Time-Out Interrupt Status Flag */

/********************* Bit definition of TCR register **********************/
#define PDMA_TCR_MASK				((uint32_t)0x0000FFFF)			/*!<PDMA Timer Count Setting Register Mask */

/******************************************************************************/
/*                CRC                                                         */
/******************************************************************************/
/********************* Bit definition of CTL register **********************/
#define PDMACRC_CTL_CEN					((uint32_t)0x00000001)			/*!<CRC Channel Enable */
#define PDMACRC_CTL_RST					((uint32_t)0x00000002)			/*!<CRC Engine Reset */
#define PDMACRC_CTL_WDATA_RVS			((uint32_t)0x01000000)			/*!<Write Data Order Reverse */
#define PDMACRC_CTL_CHECKSUM_RVS		((uint32_t)0x02000000)			/*!<Checksum Reverse */
#define PDMACRC_CTL_WDATA_COM			((uint32_t)0x04000000)			/*!<Write Data Complement */
#define PDMACRC_CTL_CHECKSUM_COM		((uint32_t)0x08000000)			/*!<Checksum Complement */
#define PDMACRC_CTL_CPU_WDLEN_8BIT		((uint32_t)0x00000000)			/*!<CPU Write Data Length 8-bit */
#define PDMACRC_CTL_CPU_WDLEN_16BIT		((uint32_t)0x10000000)			/*!<CPU Write Data Length 16-bit */
#define PDMACRC_CTL_CPU_WDLEN_32BIT		((uint32_t)0x20000000)			/*!<CPU Write Data Length 32-bit */
#define PDMACRC_CTL_CPU_WDLEN_MASK		((uint32_t)0x30000000)			/*!<CPU Write Data Length Mask */
#define PDMACRC_CTL_MODE_CCITT			((uint32_t)0x00000000)			/*!<CRC Polynomial Mode CCITT */
#define PDMACRC_CTL_MODE_CRC8			((uint32_t)0x40000000)			/*!<CRC Polynomial Mode CRC8 */
#define PDMACRC_CTL_MODE_CRC16			((uint32_t)0x80000000)			/*!<CRC Polynomial Mode CRC16 */
#define PDMACRC_CTL_MODE_CRC32			((uint32_t)0xC0000000)			/*!<CRC Polynomial Mode CRC32 */
#define PDMACRC_CTL_MODE_MASK			((uint32_t)0xC0000000)			/*!<CRC Polynomial Mode Mask */
#define PDMACRC_CTL_TRIG_EN				((uint32_t)0x00800000)			/*!<PDMACRC Trigger */

/********************* Bit definition of DMAIER register **********************/
#define PDMACRC_DMAIER_TABORT			((uint32_t)0x00000001)			/*!<PDMACRC Read/Write Target Abort Interrupt Enable */
#define PDMACRC_DMAIER_TD				((uint32_t)0x00000002)			/*!<PDMACRC Transfer Done Interrupt Enable */

/********************* Bit definition of DMAISR register **********************/
#define PDMACRC_DMAISR_TABORT			((uint32_t)0x00000001)			/*!<PDMACRC Read/Write Target Abort Interrupt Flag */
#define PDMACRC_DMAISR_TD				((uint32_t)0x00000002)			/*!<PDMACRC Transfer Done Interrupt Flag */

/******************************************************************************/
/*                PDMAGCR                                                     */
/******************************************************************************/
/********************* Bit definition of CSR register **********************/
#define PDMAGCR_CSR_CLK0				((uint32_t)0x00000100)			/*!<DMA Controller Channel 0 Clock Enable Control */
#define PDMAGCR_CSR_CLK1				((uint32_t)0x00000200)			/*!<DMA Controller Channel 1 Clock Enable Control */
#define PDMAGCR_CSR_CLK2				((uint32_t)0x00000400)			/*!<DMA Controller Channel 2 Clock Enable Control */
#define PDMAGCR_CSR_CLK3				((uint32_t)0x00000800)			/*!<DMA Controller Channel 3 Clock Enable Control */
#define PDMAGCR_CSR_CLK4				((uint32_t)0x00001000)			/*!<DMA Controller Channel 4 Clock Enable Control */
#define PDMAGCR_CSR_CLK5				((uint32_t)0x00002000)			/*!<DMA Controller Channel 5 Clock Enable Control */
#define PDMAGCR_CSR_CLK6				((uint32_t)0x00004000)			/*!<DMA Controller Channel 6 Clock Enable Control */
#define PDMAGCR_CSR_CRCCLK				((uint32_t)0x01000000)			/*!<DMA Controller CRC Clock Enable Control */

/********************* Bit definition of PDSSR0 register **********************/
#define PDMA_CH1_SPI0_TX			((uint32_t)0x00000000)			/*!<Channel 1 Connect to SPI0_TX */
#define PDMA_CH1_SPI1_TX			((uint32_t)0x00000100)			/*!<Channel 1 Connect to SPI1_TX */
#define PDMA_CH1_UART0_TX			((uint32_t)0x00000200)			/*!<Channel 1 Connect to UART0_TX */
#define PDMA_CH1_UART1_TX			((uint32_t)0x00000300)			/*!<Channel 1 Connect to UART1_TX */
#define PDMA_CH1_USB_TX				((uint32_t)0x00000400)			/*!<Channel 1 Connect to USB_TX */
#define PDMA_CH1_I2S_TX				((uint32_t)0x00000500)			/*!<Channel 1 Connect to I2S_TX */
#define PDMA_CH1_DAC0_TX			((uint32_t)0x00000600)			/*!<Channel 1 Connect to DAC0_TX */
#define PDMA_CH1_DAC1_TX			((uint32_t)0x00000700)			/*!<Channel 1 Connect to DAC1_TX */
#define PDMA_CH1_SPI2_TX			((uint32_t)0x00000800)			/*!<Channel 1 Connect to SPI2_TX */
#define PDMA_CH1_TMR0_CH0			((uint32_t)0x00000900)			/*!<Channel 1 Connect to TMR0_CH0 */
#define PDMA_CH1_TMR0_CH1			((uint32_t)0x00000A00)			/*!<Channel 1 Connect to TMR0_CH1 */
#define PDMA_CH1_TMR1_CH0			((uint32_t)0x00000B00)			/*!<Channel 1 Connect to TMR1_CH0 */
#define PDMA_CH1_TMR1_CH1			((uint32_t)0x00000C00)			/*!<Channel 1 Connect to TMR1_CH1 */

#define PDMA_CH1_SPI0_RX			((uint32_t)0x00001000)			/*!<Channel 1 Connect to SPI0_RX */
#define PDMA_CH1_SPI1_RX			((uint32_t)0x00001100)			/*!<Channel 1 Connect to SPI1_RX */
#define PDMA_CH1_UART0_RX			((uint32_t)0x00001200)			/*!<Channel 1 Connect to UART0_RX */
#define PDMA_CH1_UART1_RX			((uint32_t)0x00001300)			/*!<Channel 1 Connect to UART1_RX */
#define PDMA_CH1_USB_RX				((uint32_t)0x00001400)			/*!<Channel 1 Connect to USB_RX */
#define PDMA_CH1_I2S_RX				((uint32_t)0x00001500)			/*!<Channel 1 Connect to I2S_RX */
#define PDMA_CH1_ADC				((uint32_t)0x00001600)			/*!<Channel 1 Connect to ADC */
#define PDMA_CH1_SPI2_RX			((uint32_t)0x00001800)			/*!<Channel 1 Connect to SPI2_RX */
#define PDMA_CH1_PWM0_CH0			((uint32_t)0x00001900)			/*!<Channel 1 Connect to PWM0_CH0 */
#define PDMA_CH1_PWM0_CH2			((uint32_t)0x00001A00)			/*!<Channel 1 Connect to PWM0_CH2 */
#define PDMA_CH1_PWM1_CH0			((uint32_t)0x00001B00)			/*!<Channel 1 Connect to PWM1_CH0 */
#define PDMA_CH1_PWM1_CH2			((uint32_t)0x00001C00)			/*!<Channel 1 Connect to PWM1_CH2 */
#define PDMA_CH1_DISABLE			((uint32_t)0x00001F00)			/*!<Disable Channel 1 */
#define PDMA_CH1_MASK				((uint32_t)0x00001F00)			/*!<Channel 1 Service Selection Mask */

#define PDMA_CH2_SPI0_TX			((uint32_t)0x00000000)			/*!<Channel 2 Connect to SPI0_TX */
#define PDMA_CH2_SPI1_TX			((uint32_t)0x00010000)			/*!<Channel 2 Connect to SPI1_TX */
#define PDMA_CH2_UART0_TX			((uint32_t)0x00020000)			/*!<Channel 2 Connect to UART0_TX */
#define PDMA_CH2_UART1_TX			((uint32_t)0x00030000)			/*!<Channel 2 Connect to UART1_TX */
#define PDMA_CH2_USB_TX				((uint32_t)0x00040000)			/*!<Channel 2 Connect to USB_TX */
#define PDMA_CH2_I2S_TX				((uint32_t)0x00050000)			/*!<Channel 2 Connect to I2S_TX */
#define PDMA_CH2_DAC0_TX			((uint32_t)0x00060000)			/*!<Channel 2 Connect to DAC0_TX */
#define PDMA_CH2_DAC1_TX			((uint32_t)0x00070000)			/*!<Channel 2 Connect to DAC1_TX */
#define PDMA_CH2_SPI2_TX			((uint32_t)0x00080000)			/*!<Channel 2 Connect to SPI2_TX */
#define PDMA_CH2_TMR0_CH0			((uint32_t)0x00090000)			/*!<Channel 2 Connect to TMR0_CH0 */
#define PDMA_CH2_TMR0_CH1			((uint32_t)0x000A0000)			/*!<Channel 2 Connect to TMR0_CH1 */
#define PDMA_CH2_TMR1_CH0			((uint32_t)0x000B0000)			/*!<Channel 2 Connect to TMR1_CH0 */
#define PDMA_CH2_TMR1_CH1			((uint32_t)0x000C0000)			/*!<Channel 2 Connect to TMR1_CH1 */

#define PDMA_CH2_SPI0_RX			((uint32_t)0x00100000)			/*!<Channel 2 Connect to SPI0_RX */
#define PDMA_CH2_SPI1_RX			((uint32_t)0x00110000)			/*!<Channel 2 Connect to SPI1_RX */
#define PDMA_CH2_UART0_RX			((uint32_t)0x00120000)			/*!<Channel 2 Connect to UART0_RX */
#define PDMA_CH2_UART1_RX			((uint32_t)0x00130000)			/*!<Channel 2 Connect to UART1_RX */
#define PDMA_CH2_USB_RX				((uint32_t)0x00140000)			/*!<Channel 2 Connect to USB_RX */
#define PDMA_CH2_I2S_RX				((uint32_t)0x00150000)			/*!<Channel 2 Connect to I2S_RX */
#define PDMA_CH2_ADC				((uint32_t)0x00160000)			/*!<Channel 2 Connect to ADC */
#define PDMA_CH2_SPI2_RX			((uint32_t)0x00180000)			/*!<Channel 2 Connect to SPI2_RX */
#define PDMA_CH2_PWM0_CH0			((uint32_t)0x00190000)			/*!<Channel 2 Connect to PWM0_CH0 */
#define PDMA_CH2_PWM0_CH2			((uint32_t)0x001A0000)			/*!<Channel 2 Connect to PWM0_CH2 */
#define PDMA_CH2_PWM1_CH0			((uint32_t)0x001B0000)			/*!<Channel 2 Connect to PWM1_CH0 */
#define PDMA_CH2_PWM1_CH2			((uint32_t)0x001C0000)			/*!<Channel 2 Connect to PWM1_CH2 */
#define PDMA_CH2_DISABLE			((uint32_t)0x001F0000)			/*!<Disable Channel 2 */
#define PDMA_CH2_MASK				((uint32_t)0x001F0000)			/*!<Channel 2 Service Selection Mask */

#define PDMA_CH3_SPI0_TX			((uint32_t)0x00000000)			/*!<Channel 3 Connect to SPI0_TX */
#define PDMA_CH3_SPI1_TX			((uint32_t)0x01000000)			/*!<Channel 3 Connect to SPI1_TX */
#define PDMA_CH3_UART0_TX			((uint32_t)0x02000000)			/*!<Channel 3 Connect to UART0_TX */
#define PDMA_CH3_UART1_TX			((uint32_t)0x03000000)			/*!<Channel 3 Connect to UART1_TX */
#define PDMA_CH3_USB_TX				((uint32_t)0x04000000)			/*!<Channel 3 Connect to USB_TX */
#define PDMA_CH3_I2S_TX				((uint32_t)0x05000000)			/*!<Channel 3 Connect to I2S_TX */
#define PDMA_CH3_DAC0_TX			((uint32_t)0x06000000)			/*!<Channel 3 Connect to DAC0_TX */
#define PDMA_CH3_DAC1_TX			((uint32_t)0x07000000)			/*!<Channel 3 Connect to DAC1_TX */
#define PDMA_CH3_SPI2_TX			((uint32_t)0x08000000)			/*!<Channel 3 Connect to SPI2_TX */
#define PDMA_CH3_TMR0_CH0			((uint32_t)0x09000000)			/*!<Channel 3 Connect to TMR0_CH0 */
#define PDMA_CH3_TMR0_CH1			((uint32_t)0x0A000000)			/*!<Channel 3 Connect to TMR0_CH1 */
#define PDMA_CH3_TMR1_CH0			((uint32_t)0x0B000000)			/*!<Channel 3 Connect to TMR1_CH0 */
#define PDMA_CH3_TMR1_CH1			((uint32_t)0x0C000000)			/*!<Channel 3 Connect to TMR1_CH1 */

#define PDMA_CH3_SPI0_RX			((uint32_t)0x10000000)			/*!<Channel 3 Connect to SPI0_RX */
#define PDMA_CH3_SPI1_RX			((uint32_t)0x11000000)			/*!<Channel 3 Connect to SPI1_RX */
#define PDMA_CH3_UART0_RX			((uint32_t)0x12000000)			/*!<Channel 3 Connect to UART0_RX */
#define PDMA_CH3_UART1_RX			((uint32_t)0x13000000)			/*!<Channel 3 Connect to UART1_RX */
#define PDMA_CH3_USB_RX				((uint32_t)0x14000000)			/*!<Channel 3 Connect to USB_RX */
#define PDMA_CH3_I2S_RX				((uint32_t)0x15000000)			/*!<Channel 3 Connect to I2S_RX */
#define PDMA_CH3_ADC				((uint32_t)0x16000000)			/*!<Channel 3 Connect to ADC */
#define PDMA_CH3_SPI2_RX			((uint32_t)0x18000000)			/*!<Channel 3 Connect to SPI2_RX */
#define PDMA_CH3_PWM0_CH0			((uint32_t)0x19000000)			/*!<Channel 3 Connect to PWM0_CH0 */
#define PDMA_CH3_PWM0_CH2			((uint32_t)0x1A000000)			/*!<Channel 3 Connect to PWM0_CH2 */
#define PDMA_CH3_PWM1_CH0			((uint32_t)0x1B000000)			/*!<Channel 3 Connect to PWM1_CH0 */
#define PDMA_CH3_PWM1_CH2			((uint32_t)0x1C000000)			/*!<Channel 3 Connect to PWM1_CH2 */
#define PDMA_CH3_DISABLE			((uint32_t)0x1F000000)			/*!<Disable Channel 3 */
#define PDMA_CH3_MASK				((uint32_t)0x1F000000)			/*!<Channel 3 Service Selection Mask */

/********************* Bit definition of PDSSR1 register **********************/
#define PDMA_CH4_SPI0_TX			((uint32_t)0x00000000)			/*!<Channel 4 Connect to SPI0_TX */
#define PDMA_CH4_SPI1_TX			((uint32_t)0x00000001)			/*!<Channel 4 Connect to SPI1_TX */
#define PDMA_CH4_UART0_TX			((uint32_t)0x00000002)			/*!<Channel 4 Connect to UART0_TX */
#define PDMA_CH4_UART1_TX			((uint32_t)0x00000003)			/*!<Channel 4 Connect to UART1_TX */
#define PDMA_CH4_USB_TX				((uint32_t)0x00000004)			/*!<Channel 4 Connect to USB_TX */
#define PDMA_CH4_I2S_TX				((uint32_t)0x00000005)			/*!<Channel 4 Connect to I2S_TX */
#define PDMA_CH4_DAC0_TX			((uint32_t)0x00000006)			/*!<Channel 4 Connect to DAC0_TX */
#define PDMA_CH4_DAC1_TX			((uint32_t)0x00000007)			/*!<Channel 4 Connect to DAC1_TX */
#define PDMA_CH4_SPI2_TX			((uint32_t)0x00000008)			/*!<Channel 4 Connect to SPI2_TX */
#define PDMA_CH4_TMR0_CH0			((uint32_t)0x00000009)			/*!<Channel 4 Connect to TMR0_CH0 */
#define PDMA_CH4_TMR0_CH1			((uint32_t)0x0000000A)			/*!<Channel 4 Connect to TMR0_CH1 */
#define PDMA_CH4_TMR1_CH0			((uint32_t)0x0000000B)			/*!<Channel 4 Connect to TMR1_CH0 */
#define PDMA_CH4_TMR1_CH1			((uint32_t)0x0000000C)			/*!<Channel 4 Connect to TMR1_CH1 */

#define PDMA_CH4_SPI0_RX			((uint32_t)0x00000010)			/*!<Channel 4 Connect to SPI0_RX */
#define PDMA_CH4_SPI1_RX			((uint32_t)0x00000011)			/*!<Channel 4 Connect to SPI1_RX */
#define PDMA_CH4_UART0_RX			((uint32_t)0x00000012)			/*!<Channel 4 Connect to UART0_RX */
#define PDMA_CH4_UART1_RX			((uint32_t)0x00000013)			/*!<Channel 4 Connect to UART1_RX */
#define PDMA_CH4_USB_RX				((uint32_t)0x00000014)			/*!<Channel 4 Connect to USB_RX */
#define PDMA_CH4_I2S_RX				((uint32_t)0x00000015)			/*!<Channel 4 Connect to I2S_RX */
#define PDMA_CH4_ADC				((uint32_t)0x00000016)			/*!<Channel 4 Connect to ADC */
#define PDMA_CH4_SPI2_RX			((uint32_t)0x00000018)			/*!<Channel 4 Connect to SPI2_RX */
#define PDMA_CH4_PWM0_CH0			((uint32_t)0x00000019)			/*!<Channel 4 Connect to PWM0_CH0 */
#define PDMA_CH4_PWM0_CH2			((uint32_t)0x0000001A)			/*!<Channel 4 Connect to PWM0_CH2 */
#define PDMA_CH4_PWM1_CH0			((uint32_t)0x0000001B)			/*!<Channel 4 Connect to PWM1_CH0 */
#define PDMA_CH4_PWM1_CH2			((uint32_t)0x0000001C)			/*!<Channel 4 Connect to PWM1_CH2 */
#define PDMA_CH4_DISABLE			((uint32_t)0x0000001F)			/*!<Disable Channel 4 */
#define PDMA_CH4_MASK				((uint32_t)0x0000001F)			/*!<Channel 4 Service Selection Mask */

#define PDMA_CH5_SPI0_TX			((uint32_t)0x00000000)			/*!<Channel 5 Connect to SPI0_TX */
#define PDMA_CH5_SPI1_TX			((uint32_t)0x00000100)			/*!<Channel 5 Connect to SPI1_TX */
#define PDMA_CH5_UART0_TX			((uint32_t)0x00000200)			/*!<Channel 5 Connect to UART0_TX */
#define PDMA_CH5_UART1_TX			((uint32_t)0x00000300)			/*!<Channel 5 Connect to UART1_TX */
#define PDMA_CH5_USB_TX				((uint32_t)0x00000400)			/*!<Channel 5 Connect to USB_TX */
#define PDMA_CH5_I2S_TX				((uint32_t)0x00000500)			/*!<Channel 5 Connect to I2S_TX */
#define PDMA_CH5_DAC0_TX			((uint32_t)0x00000600)			/*!<Channel 5 Connect to DAC0_TX */
#define PDMA_CH5_DAC1_TX			((uint32_t)0x00000700)			/*!<Channel 5 Connect to DAC1_TX */
#define PDMA_CH5_SPI2_TX			((uint32_t)0x00000800)			/*!<Channel 5 Connect to SPI2_TX */
#define PDMA_CH5_TMR0_CH0			((uint32_t)0x00000900)			/*!<Channel 5 Connect to TMR0_CH0 */
#define PDMA_CH5_TMR0_CH1			((uint32_t)0x00000A00)			/*!<Channel 5 Connect to TMR0_CH1 */
#define PDMA_CH5_TMR1_CH0			((uint32_t)0x00000B00)			/*!<Channel 5 Connect to TMR1_CH0 */
#define PDMA_CH5_TMR1_CH1			((uint32_t)0x00000C00)			/*!<Channel 5 Connect to TMR1_CH1 */

#define PDMA_CH5_SPI0_RX			((uint32_t)0x00001000)			/*!<Channel 5 Connect to SPI0_RX */
#define PDMA_CH5_SPI1_RX			((uint32_t)0x00001100)			/*!<Channel 5 Connect to SPI1_RX */
#define PDMA_CH5_UART0_RX			((uint32_t)0x00001200)			/*!<Channel 5 Connect to UART0_RX */
#define PDMA_CH5_UART1_RX			((uint32_t)0x00001300)			/*!<Channel 5 Connect to UART1_RX */
#define PDMA_CH5_USB_RX				((uint32_t)0x00001400)			/*!<Channel 5 Connect to USB_RX */
#define PDMA_CH5_I2S_RX				((uint32_t)0x00001500)			/*!<Channel 5 Connect to I2S_RX */
#define PDMA_CH5_ADC				((uint32_t)0x00001600)			/*!<Channel 5 Connect to ADC */
#define PDMA_CH5_SPI2_RX			((uint32_t)0x00001800)			/*!<Channel 5 Connect to SPI2_RX */
#define PDMA_CH5_PWM0_CH0			((uint32_t)0x00001900)			/*!<Channel 5 Connect to PWM0_CH0 */
#define PDMA_CH5_PWM0_CH2			((uint32_t)0x00001A00)			/*!<Channel 5 Connect to PWM0_CH2 */
#define PDMA_CH5_PWM1_CH0			((uint32_t)0x00001B00)			/*!<Channel 5 Connect to PWM1_CH0 */
#define PDMA_CH5_PWM1_CH2			((uint32_t)0x00001C00)			/*!<Channel 5 Connect to PWM1_CH2 */
#define PDMA_CH5_DISABLE			((uint32_t)0x00001F00)			/*!<Disable Channel 5 */
#define PDMA_CH5_MASK				((uint32_t)0x00001F00)			/*!<Channel 5 Service Selection Mask */

#define PDMA_CH6_SPI0_TX			((uint32_t)0x00000000)			/*!<Channel 6 Connect to SPI0_TX */
#define PDMA_CH6_SPI1_TX			((uint32_t)0x00010000)			/*!<Channel 6 Connect to SPI1_TX */
#define PDMA_CH6_UART0_TX			((uint32_t)0x00020000)			/*!<Channel 6 Connect to UART0_TX */
#define PDMA_CH6_UART1_TX			((uint32_t)0x00030000)			/*!<Channel 6 Connect to UART1_TX */
#define PDMA_CH6_USB_TX				((uint32_t)0x00040000)			/*!<Channel 6 Connect to USB_TX */
#define PDMA_CH6_I2S_TX				((uint32_t)0x00050000)			/*!<Channel 6 Connect to I2S_TX */
#define PDMA_CH6_DAC0_TX			((uint32_t)0x00060000)			/*!<Channel 6 Connect to DAC0_TX */
#define PDMA_CH6_DAC1_TX			((uint32_t)0x00070000)			/*!<Channel 6 Connect to DAC1_TX */
#define PDMA_CH6_SPI2_TX			((uint32_t)0x00080000)			/*!<Channel 6 Connect to SPI2_TX */
#define PDMA_CH6_TMR0_CH0			((uint32_t)0x00090000)			/*!<Channel 6 Connect to TMR0_CH0 */
#define PDMA_CH6_TMR0_CH1			((uint32_t)0x000A0000)			/*!<Channel 6 Connect to TMR0_CH1 */
#define PDMA_CH6_TMR1_CH0			((uint32_t)0x000B0000)			/*!<Channel 6 Connect to TMR1_CH0 */
#define PDMA_CH6_TMR1_CH1			((uint32_t)0x000C0000)			/*!<Channel 6 Connect to TMR1_CH1 */

#define PDMA_CH6_SPI0_RX			((uint32_t)0x00100000)			/*!<Channel 6 Connect to SPI0_RX */
#define PDMA_CH6_SPI1_RX			((uint32_t)0x00110000)			/*!<Channel 6 Connect to SPI1_RX */
#define PDMA_CH6_UART0_RX			((uint32_t)0x00120000)			/*!<Channel 6 Connect to UART0_RX */
#define PDMA_CH6_UART1_RX			((uint32_t)0x00130000)			/*!<Channel 6 Connect to UART1_RX */
#define PDMA_CH6_USB_RX				((uint32_t)0x00140000)			/*!<Channel 6 Connect to USB_RX */
#define PDMA_CH6_I2S_RX				((uint32_t)0x00150000)			/*!<Channel 6 Connect to I2S_RX */
#define PDMA_CH6_ADC				((uint32_t)0x00160000)			/*!<Channel 6 Connect to ADC */
#define PDMA_CH6_SPI2_RX			((uint32_t)0x00180000)			/*!<Channel 6 Connect to SPI2_RX */
#define PDMA_CH6_PWM0_CH0			((uint32_t)0x00190000)			/*!<Channel 6 Connect to PWM0_CH0 */
#define PDMA_CH6_PWM0_CH2			((uint32_t)0x001A0000)			/*!<Channel 6 Connect to PWM0_CH2 */
#define PDMA_CH6_PWM1_CH0			((uint32_t)0x001B0000)			/*!<Channel 6 Connect to PWM1_CH0 */
#define PDMA_CH6_PWM1_CH2			((uint32_t)0x001C0000)			/*!<Channel 6 Connect to PWM1_CH2 */
#define PDMA_CH6_DISABLE			((uint32_t)0x001F0000)			/*!<Disable Channel 6 */
#define PDMA_CH6_MASK				((uint32_t)0x001F0000)			/*!<Channel 6 Service Selection Mask */

/********************* Bit definition of ISR register **********************/
#define PDMAGCR_ISR_INTR0			((uint32_t)0x00000001)			/*!<Interrupt Pin Status Of Channel 0 */
#define PDMAGCR_ISR_INTR1			((uint32_t)0x00000002)			/*!<Interrupt Pin Status Of Channel 1 */
#define PDMAGCR_ISR_INTR2			((uint32_t)0x00000004)			/*!<Interrupt Pin Status Of Channel 2 */
#define PDMAGCR_ISR_INTR3			((uint32_t)0x00000008)			/*!<Interrupt Pin Status Of Channel 3 */
#define PDMAGCR_ISR_INTR4			((uint32_t)0x00000010)			/*!<Interrupt Pin Status Of Channel 4 */
#define PDMAGCR_ISR_INTR5			((uint32_t)0x00000020)			/*!<Interrupt Pin Status Of Channel 5 */
#define PDMAGCR_ISR_INTR6			((uint32_t)0x00000040)			/*!<Interrupt Pin Status Of Channel 6 */
#define PDMAGCR_ISR_CRC				((uint32_t)0x00010000)			/*!<Interrupt Pin Status Of CRC */


/******************************************************************************/
/*                FMC                                                         */
/******************************************************************************/
/********************* Bit definition of ISPCON register **********************/
#define FMC_ISPCON_ISPEN			((uint32_t)0x00000001)			/*!<ISP function enable */
#define FMC_ISPCON_BS				((uint32_t)0x00000002)			/*!<Boot Select */
#define FMC_ISPCON_APUEN			((uint32_t)0x00000008)			/*!<User-Hidden-Block Update Enable (Write-protection Bit) */
#define FMC_ISPCON_CFGUEN			((uint32_t)0x00000010)			/*!<Enable Config-bits Update by ISP (Write-protection Bit) */
#define FMC_ISPCON_LDUEN			((uint32_t)0x00000020)			/*!<LDROM Update Enable (Write-protection Bit) */
#define FMC_ISPCON_ISPFF			((uint32_t)0x00000040)			/*!<ISP Fail Flag (Write-protection Bit) */

/********************* Bit definition of ISPCMD register **********************/
#define FMC_ISPCMD_CMD_MASK			((uint32_t)0x0000003F)			/*!<ISP Command Mask */
#define FMC_ISPCMD_READ				((uint32_t)0x00000000)			/*!<ISP Command - Read */
#define FMC_ISPCMD_VECMAP			((uint32_t)0x0000002E)			/*!<ISP Command - Vector Page Re-Map */
#define FMC_ISPCMD_PROGRAM			((uint32_t)0x00000021)			/*!<ISP Command - Program */
#define FMC_ISPCMD_PAGE_ERASE		((uint32_t)0x00000022)			/*!<ISP Command - Page Erase */
#define FMC_ISPCMD_READ_CID			((uint32_t)0x0000000B)			/*!<ISP Command - Read CID */
#define FMC_ISPCMD_READ_DID			((uint32_t)0x0000000C)			/*!<ISP Command - Read DID */
#define FMC_ISPCMD_READ_UID			((uint32_t)0x00000004)			/*!<ISP Command - Read UID */

/********************* Bit definition of ISPTRG register **********************/
#define FMC_ISPTRG_ISPGO			((uint32_t)0x00000001)			/*!<ISP Start Trigger */

/********************* Bit definition of ISPSTA register **********************/
#define FMC_ISPSTA_ISPBUSY			((uint32_t)0x00000001)			/*!<ISP Busy */
#define FMC_ISPSTA_CBS_MASK			((uint32_t)0x00000006)			/*!<Boot Selection bit mask */
#define FMC_ISPSTA_CBS_11			((uint32_t)0x00000006)			/*!<Boot from APROM w/o vecmap */
#define FMC_ISPSTA_CBS_10			((uint32_t)0x00000004)			/*!<Boot from APROM with vecmap */
#define FMC_ISPSTA_CBS_01			((uint32_t)0x00000002)			/*!<Boot from LDROM w/o vecmap */
#define FMC_ISPSTA_CBS_00			((uint32_t)0x00000000)			/*!<Boot from LDROM with vecmap */
#define FMC_ISPSTA_ISPFF			((uint32_t)0x00000040)			/*!<ISP fail flag */
#define FMC_ISPSTA_VECMAP_MASK		((uint32_t)0x001FFF00)			/*!<Vector page mapping address mask */


/******************************************************************************/
/*                EBI                                                         */
/******************************************************************************/
/********************* Bit definition of EBICON register **********************/
#define EBI_EBICON_ExtEN			((uint32_t)0x00000001)			/*!<EBI enable */
#define EBI_EBICON_ExtBW16			((uint32_t)0x00000002)			/*!<EBI data width, 0: 8-bit; 1: 16-bit */
#define EBI_EBICON_MCLKDIV_MASK		((uint32_t)0x00000700)			/*!<EBI external output clock divider mask */
#define EBI_EBICON_MCLKDIV_HCLK_1	((uint32_t)0x00000000)			/*!<MCLK = HCLK/1 */
#define EBI_EBICON_MCLKDIV_HCLK_2	((uint32_t)0x00000100)			/*!<MCLK = HCLK/2 */
#define EBI_EBICON_MCLKDIV_HCLK_4	((uint32_t)0x00000200)			/*!<MCLK = HCLK/4 */
#define EBI_EBICON_MCLKDIV_HCLK_8	((uint32_t)0x00000300)			/*!<MCLK = HCLK/8 */
#define EBI_EBICON_MCLKDIV_HCLK_16	((uint32_t)0x00000400)			/*!<MCLK = HCLK/16 */
#define EBI_EBICON_MCLKDIV_HCLK_32	((uint32_t)0x00000500)			/*!<MCLK = HCLK/32 */
#define EBI_EBICON_MCLKEN			((uint32_t)0x00000800)			/*!<External clock enable */
#define EBI_EBICON_ExtALE_MASK		((uint32_t)0x00070000)			/*!<Expand time of ALE mask */

/********************* Bit definition of EXTIME register **********************/
#define EBI_EXTIME_ExttAAC_MASK		((uint32_t)0x0000001F)			/*!<Data access time mask */
#define EBI_EXTIME_ExttAHD_MASK		((uint32_t)0x00000700)			/*!<Data access hold time mask */
#define EBI_EXTIME_ExtlW2X_MASK		((uint32_t)0x0000F000)			/*!<Idle state cycle of write mask */
#define EBI_EXTIME_ExtlR2W_MASK		((uint32_t)0x000F0000)			/*!<Idle state cycle between read-write mask */
#define EBI_EXTIME_ExtlR2R_MASK		((uint32_t)0x0F000000)			/*!<Idle state cycle between read-read mask */


/******************************************************************************/
/*                WDT                                                         */
/******************************************************************************/
/********************* Bit definition of CTL register **********************/
#define WDT_CTL_WTR					((uint32_t)0x00000001)			/*!<Clear Watchdog Timer */
#define WDT_CTL_WTRE				((uint32_t)0x00000002)			/*!<Watchdog Timer Reset Function Enable */
#define WDT_CTL_WTWKE				((uint32_t)0x00000004)			/*!<Watchdog Timer Wake-Up Function Enable */
#define WDT_CTL_WTE					((uint32_t)0x00000008)			/*!<Watchdog Timer Enable */
#define WDT_CTL_WTIS_2POW4			((uint32_t)0x00000000)			/*!<Watchdog Timer Interval Select ((2 ^ 4) + 1) Clk */
#define WDT_CTL_WTIS_2POW6			((uint32_t)0x00000010)			/*!<Watchdog Timer Interval Select ((2 ^ 6) + 1) Clk */
#define WDT_CTL_WTIS_2POW8			((uint32_t)0x00000020)			/*!<Watchdog Timer Interval Select ((2 ^ 8) + 1) Clk */
#define WDT_CTL_WTIS_2POW10			((uint32_t)0x00000030)			/*!<Watchdog Timer Interval Select ((2 ^ 10) + 1) Clk */
#define WDT_CTL_WTIS_2POW12			((uint32_t)0x00000040)			/*!<Watchdog Timer Interval Select ((2 ^ 12) + 1) Clk */
#define WDT_CTL_WTIS_2POW14			((uint32_t)0x00000050)			/*!<Watchdog Timer Interval Select ((2 ^ 14) + 1) Clk */
#define WDT_CTL_WTIS_2POW16			((uint32_t)0x00000060)			/*!<Watchdog Timer Interval Select ((2 ^ 16) + 1) Clk */
#define WDT_CTL_WTIS_2POW18			((uint32_t)0x00000070)			/*!<Watchdog Timer Interval Select ((2 ^ 18) + 1) Clk */
#define WDT_CTL_WTIS_MASK			((uint32_t)0x00000070)			/*!<Watchdog Timer Interval Select Mask */
#define WDT_CTL_WTRDSEL_1026CLK		((uint32_t)0x00000000)			/*!<Watchdog Reset Delay Period 1026 Clk */
#define WDT_CTL_WTRDSEL_130CLK		((uint32_t)0x00000100)			/*!<Watchdog Reset Delay Period 130 Clk */
#define WDT_CTL_WTRDSEL_18CLK		((uint32_t)0x00000200)			/*!<Watchdog Reset Delay Period 18 Clk */
#define WDT_CTL_WTRDSEL_3CLK		((uint32_t)0x00000300)			/*!<Watchdog Reset Delay Period 3 Clk */
#define WDT_CTL_WTRDSEL_MASK		((uint32_t)0x00000300)			/*!<Watchdog Reset Delay Period Mask */

/********************* Bit definition of IER register **********************/
#define WDT_IER_WDTIE				((uint32_t)0x00000001)			/*!<Watchdog Timer Interrupt Enable */

/********************* Bit definition of ISR register **********************/
#define WDT_ISR_WDTIS				((uint32_t)0x00000001)			/*!<Watchdog Timer Interrupt Enable */
#define WDT_ISR_WDTRSTIS			((uint32_t)0x00000002)			/*!<Watchdog Timer Reset Status */
#define WDT_ISR_WDTWAKEIS			((uint32_t)0x00000004)			/*!<Watchdog Timer Wake-Up Status */

/******************************************************************************/
/*                WWDT                                                        */
/******************************************************************************/
/********************* Bit definition of CR register **********************/
#define WWDT_CR_WWDTEN				((uint32_t)0x00000001)			/*!<Window Watchdog Enable */
#define WWDT_CR_PERIODSEL_1			((uint32_t)0x00000000)			/*!<WWDT Pre-scale Period Set to 1 */
#define WWDT_CR_PERIODSEL_2			((uint32_t)0x00000100)			/*!<WWDT Pre-scale Period Set to 2 */
#define WWDT_CR_PERIODSEL_4			((uint32_t)0x00000200)			/*!<WWDT Pre-scale Period Set to 4 */
#define WWDT_CR_PERIODSEL_8			((uint32_t)0x00000300)			/*!<WWDT Pre-scale Period Set to 8 */
#define WWDT_CR_PERIODSEL_16		((uint32_t)0x00000400)			/*!<WWDT Pre-scale Period Set to 16 */
#define WWDT_CR_PERIODSEL_32		((uint32_t)0x00000500)			/*!<WWDT Pre-scale Period Set to 32 */
#define WWDT_CR_PERIODSEL_64		((uint32_t)0x00000600)			/*!<WWDT Pre-scale Period Set to 64 */
#define WWDT_CR_PERIODSEL_128		((uint32_t)0x00000700)			/*!<WWDT Pre-scale Period Set to 128 */
#define WWDT_CR_PERIODSEL_192		((uint32_t)0x00000800)			/*!<WWDT Pre-scale Period Set to 192 */
#define WWDT_CR_PERIODSEL_256		((uint32_t)0x00000900)			/*!<WWDT Pre-scale Period Set to 256 */
#define WWDT_CR_PERIODSEL_384		((uint32_t)0x00000A00)			/*!<WWDT Pre-scale Period Set to 384 */
#define WWDT_CR_PERIODSEL_512		((uint32_t)0x00000B00)			/*!<WWDT Pre-scale Period Set to 512 */
#define WWDT_CR_PERIODSEL_768		((uint32_t)0x00000C00)			/*!<WWDT Pre-scale Period Set to 768 */
#define WWDT_CR_PERIODSEL_1024		((uint32_t)0x00000D00)			/*!<WWDT Pre-scale Period Set to 1024 */
#define WWDT_CR_PERIODSEL_1536		((uint32_t)0x00000E00)			/*!<WWDT Pre-scale Period Set to 1536 */
#define WWDT_CR_PERIODSEL_2048		((uint32_t)0x00000F00)			/*!<WWDT Pre-scale Period Set to 2048 */
#define WWDT_CR_PERIODSEL_MASK		((uint32_t)0x00000F00)			/*!<WWDT Pre-scale Period Select Mask */
#define WWDT_CR_WINCMP_MASK			((uint32_t)0x003F0000)			/*!<WWDT Window Compare Register */
#define WWDT_CR_DBGEN				((uint32_t)0x80000000)			/*!<WWDT Debug Enable */

/********************* Bit definition of IER register **********************/
#define WWDT_IER_IE					((uint32_t)0x00000001)			/*!<WWDT Interrupt Enable */

/********************* Bit definition of STS register **********************/
#define WWDT_STS_IF					((uint32_t)0x00000001)			/*!<WWDT Compare Match Interrupt Flag */
#define WWDT_STS_RF					((uint32_t)0x00000002)			/*!<WWDT Reset Flag */

/********************* Bit definition of VAL register **********************/
#define WWDT_VAL_VAL_MASK			((uint32_t)0x0000003F)			/*!<WWDT Counter Value Mask */


/******************************************************************************/
/*                RTC                                                         */
/******************************************************************************/
/********************* Bit definition of AER register **********************/
#define RTC_AER_AER					((uint32_t)0x0000FFFF)			/*!<RTC Register Access Enable Password (Write only) */
#define RTC_AER_ENF					((uint32_t)0x00010000)			/*!<RTC Register Access Enable Flag (Read only) */
/********************* Bit definition of FCR register **********************/
#define RTC_FCR_FRACTION			((uint32_t)0x0000003F)			/*!<Fraction Part */
#define RTC_FCR_INTEGER				((uint32_t)0x00000F00)			/*!<Integer Part */
/********************* Bit definition of TLR register **********************/
#define RTC_TLR_SEC					((uint32_t)0x0000000F)			/*!<1 Sec Time Digit (0~9) */
#define RTC_TLR_TEN_SEC				((uint32_t)0x00000070)			/*!<10 Sec Time Digit (0~5) */
#define RTC_TLR_MIN					((uint32_t)0x00000F00)			/*!<1 Min Time Digit (0~9) */
#define RTC_TLR_TEN_MIN				((uint32_t)0x00007000)			/*!<10 Min Time Digit (0~5) */
#define RTC_TLR_HR					((uint32_t)0x000F0000)			/*!<1 Hour Time Digit (0~9) */
#define RTC_TLR_TEN_HR				((uint32_t)0x00300000)			/*!<10 Hour Time Digit (0~2) */
/********************* Bit definition of CLR register **********************/
#define RTC_CLR_DAY					((uint32_t)0x0000000F)			/*!<1 Day Calendar Digit (0~9) */
#define RTC_CLR_TEN_DAY				((uint32_t)0x00000030)			/*!<10 Day Calendar Digit (0~3) */
#define RTC_CLR_MON					((uint32_t)0x00000F00)			/*!<1 Month Calendar Digit (0~9) */
#define RTC_CLR_TEN_MON				((uint32_t)0x00001000)			/*!<10 Month Calendar Digit (0~1) */
#define RTC_CLR_YEAR				((uint32_t)0x000F0000)			/*!<1 Year Calendar Digit (0~9) */
#define RTC_CLR_TEN_YEAR			((uint32_t)0x00F00000)			/*!<10 Year Calendar Digit (0~9) */
/********************* Bit definition of TSSR register **********************/
#define RTC_TSSR_24hr				((uint32_t)0x00000001)			/*!<24-Hour / 12-Hour Mode Selection */
/********************* Bit definition of DWR register **********************/
#define RTC_DWR_DWR					((uint32_t)0x00000007)			/*!<Day of the Week Register */
/********************* Bit definition of TAR register **********************/
#define RTC_TAR_SEC					((uint32_t)0x0000000F)			/*!<1 Sec Time Digit of Alarm Setting (0~9) */
#define RTC_TAR_TEN_SEC				((uint32_t)0x00000070)			/*!<10 Sec Time Digit of Alarm Setting (0~5) */
#define RTC_TAR_MIN					((uint32_t)0x00000F00)			/*!<1 Min Time Digit of Alarm Setting (0~9) */
#define RTC_TAR_TEN_MIN				((uint32_t)0x00007000)			/*!<10 Min Time Digit of Alarm Setting (0~5) */
#define RTC_TAR_HR					((uint32_t)0x000F0000)			/*!<1 Hour Time Digit of Alarm Setting (0~9) */
#define RTC_TAR_TEN_HR				((uint32_t)0x00300000)			/*!<10 Hour Time Digit of Alarm Setting (0~2) */
/********************* Bit definition of CAR register **********************/
#define RTC_CAR_DAY					((uint32_t)0x0000000F)			/*!<1 Day Calendar Digit of Alarm Setting (0~9) */
#define RTC_CAR_TEN_DAY				((uint32_t)0x00000030)			/*!<10 Day Calendar Digit of Alarm Setting (0~3) */
#define RTC_CAR_MON					((uint32_t)0x00000F00)			/*!<1 Month Calendar Digit of Alarm Setting (0~9) */
#define RTC_CAR_TEN_MON				((uint32_t)0x00001000)			/*!<10 Month Calendar Digit of Alarm Setting (0~1) */
#define RTC_CAR_YEAR				((uint32_t)0x000F0000)			/*!<1 Year Calendar Digit of Alarm Setting (0~9) */
#define RTC_CAR_TEN_YEAR			((uint32_t)0x00F00000)			/*!<10 Year Calendar Digit of Alarm Setting (0~9) */
/********************* Bit definition of LIR register **********************/
#define RTC_LIR_LIR					((uint32_t)0x00000001)			/*!<Leap Year Indication REGISTER (Read only) */
/********************* Bit definition of RIER register **********************/
#define RTC_RIER_AIER				((uint32_t)0x00000001)			/*!<Alarm Interrupt Enable */
#define RTC_RIER_TIER				((uint32_t)0x00000002)			/*!<Time Tick Interrupt and Wake-up by Tick Enable */
#define RTC_RIER_SNOOPIER			((uint32_t)0x00000004)			/*!<Snooper Pin Event Detection Interrupt Enable */
/********************* Bit definition of RIIR register **********************/
#define RTC_RIIR_AIS				((uint32_t)0x00000001)			/*!<RTC Alarm Interrupt Status */
#define RTC_RIIR_TIS				((uint32_t)0x00000002)			/*!<RTC Time Tick Interrupt Status */
#define RTC_RIIR_SNOOPIS			((uint32_t)0x00000004)			/*!<Snooper Pin Event Detection Interrupt Status */
/********************* Bit definition of TTR register **********************/
#define RTC_TTR_TTR				((uint32_t)0x00000007)			/*!<Time Tick Register */
#define RTC_TTR_TWKE			((uint32_t)0x00000008)			/*!<RTC Timer Wake-up CPU Function Enable Bit */
/********************* Bit definition of SPRCTL register **********************/
#define RTC_SPRCTL_SNOOPEN		((uint32_t)0x00000001)			/*!<Snooper Pin Event Detection Enable */
#define RTC_SPRCTL_SNOOPEDGE	((uint32_t)0x00000002)			/*!<Snooper Active Edge Selection */
#define RTC_SPRCTL_SPRRDY		((uint32_t)0x00000080)			/*!<SPR Register Ready */


/******************************************************************************/
/*                TIMER                                                       */
/******************************************************************************/
/********************* Bit definition of CTL register **********************/
#define TIMER_CTL_TMREN				((uint32_t)0x00000001)			/*!<Timer Counter Enable Bit */
#define TIMER_CTL_SWRST				((uint32_t)0x00000002)			/*!<Software Reset */
#define TIMER_CTL_WAKEEN			((uint32_t)0x00000004)			/*!<Wake-up Enable */
#define TIMER_CTL_DBGACKEN			((uint32_t)0x00000005)			/*!<ICE Debug Mode Acknowledge Ineffective Enable */
#define TIMER_CTL_MODESEL_ONESHOT	((uint32_t)0x00000000)			/*!<Timer Operate in One Shot Mode */
#define TIMER_CTL_MODESEL_PERIODIC	((uint32_t)0x00000010)			/*!<Timer Operate in Periodic Mode */
#define TIMER_CTL_MODESEL_TOGGLE	((uint32_t)0x00000020)			/*!<Timer Operate in Toggle Mode */
#define TIMER_CTL_MODESEL_CONTINUOUS	((uint32_t)0x00000030)		/*!<Timer Operate in Continuous Mode */
#define TIMER_CTL_MODESEL_MASK		((uint32_t)0x00000030)			/*!<Timer Operating Mode Select Mask */
#define TIMER_CTL_TMRACT			((uint32_t)0x00000080)			/*!<Timer Active Status Bit */
#define TIMER_CTL_ADCTEEN			((uint32_t)0x00000100)			/*!<TMR_IS or TCAP_IS Trigger ADC Enable */
#define TIMER_CTL_DACTEEN			((uint32_t)0x00000200)			/*!<TMR_IS or TCAP_IS Trigger DAC Enable */
#define TIMER_CTL_PDMA_TEEN			((uint32_t)0x00000400)			/*!<TMR_IS or TCAP_IS Trigger PDMA Enable */
#define TIMER_CTL_CAPTRGEN			((uint32_t)0x00000800)			/*!<TCAP_IS Trigger Mode Enable */
#define TIMER_CTL_TIMEOUTTRGEN			((uint32_t)0x00000000)			/*!<TMRS Trigger Mode Enable */
#define TIMER_CTL_EVNTEN			((uint32_t)0x00001000)			/*!<Event Counting Mode Enable */
#define TIMER_CTL_EVNTEDGE_R			((uint32_t)0x00002000)			/*!<Event Counting Mode Edge Selection */
#define TIMER_CTL_EVNTEDGE_F			((uint32_t)0x00000000)			/*!<Event Counting Mode Edge Selection */
#define TIMER_CTL_EVNTDEBEN			((uint32_t)0x00004000)			/*!<External Event De-bounce Enable */
#define TIMER_CTL_TKWAKEEN			((uint32_t)0x00008000)			/*!<Wake-up Touch-Key Enable */
#define TIMER_CTL_TCAPEN			((uint32_t)0x00010000)			/*!<TCapture Pin Functional Enable */
#define TIMER_CTL_TCAPMODE			((uint32_t)0x00020000)			/*!<TCapture Pin Function Mode Selection */
#define TIMER_CTL_TCAPEDGE_FF		((uint32_t)0x00000000)			/*!<TCapture Pin Falling-Falling Detect Selection */
#define TIMER_CTL_TCAPEDGE_RR		((uint32_t)0x00040000)			/*!<TCapture Pin Rising-Rising Detect Selection */
#define TIMER_CTL_TCAPEDGE_FR		((uint32_t)0x00080000)			/*!<TCapture Pin Falling-Rising Detect Selection */
#define TIMER_CTL_TCAPEDGE_RF		((uint32_t)0x000C0000)			/*!<TCapture Pin Falling-Rising Detect Selection */
#define TIMER_CTL_TCAPEDGE_MASK		((uint32_t)0x000C0000)			/*!<TCapture Pin Edge Detect Selection Mask */
#define TIMER_CTL_CAPCNTMOD			((uint32_t)0x00100000)			/*!<Timer Capture Counting Mode Selection */
#define TIMER_CTL_CAPCNTMOD_FREE_CNT		((uint32_t)0x00000000)			/*!<Free Counting Mode */
#define TIMER_CTL_CAPCNTMOD_TRIGGER_CNT		((uint32_t)0x00100000)			/*!<Trigger Counting Mode */
#define TIMER_CTL_TCAPDEBEN			((uint32_t)0x00400000)			/*!<TCapture Pin De-bounce Enable */
#define TIMER_CTL_INTRTRGEN			((uint32_t)0x01000000)			/*!<Inter-Timer Trigger Mode Enable */

/********************* Bit definition of PRECNT register **********************/
#define TIMER_PRECNT_PRESCALECNT_MASK	((uint32_t)0x000000FF)		/*!<Pre-scale Counter */

/********************* Bit definition of CMPR register **********************/
#define TIMER_CMPR_TMRCMP_MASK		((uint32_t)0x00FFFFFF)			/*!<Timer Compared Value */

/********************* Bit definition of IER register **********************/
#define TIMER_IER_TMRIE				((uint32_t)0x00000001)			/*!<Timer Interrupt Enable */
#define TIMER_IER_TCAPIE			((uint32_t)0x00000002)			/*!<Timer Capture Function Interrupt Enable */

/********************* Bit definition of ISR register **********************/
#define TIMER_ISR_TMRIS				((uint32_t)0x00000001)			/*!<Timer Interrupt Status */
#define TIMER_ISR_TCAPIS			((uint32_t)0x00000002)			/*!<Timer Capture Function Interrupt Status */
#define TIMER_ISR_TMRWAKESTS		((uint32_t)0x00000010)			/*!<Timer Wake-up Status */
#define TIMER_ISR_NCAPDETSTS		((uint32_t)0x00000020)			/*!<New Capture Detected Status */

/********************* Bit definition of DR register **********************/
#define TIMER_DR_TDR_MASK			((uint32_t)0x00FFFFFF)			/*!<Timer Data Mask */

/********************* Bit definition of TCAP register **********************/
#define TIMER_TCAP_CAP_MASK			((uint32_t)0x00FFFFFF)			/*!<Timer Capture Data Mask */



/******************************************************************************/
/*                I2C                                                         */
/******************************************************************************/
/********************* Bit definition of CON register **********************/
#define I2C_CON_IPEN			((uint32_t)0x00000001)			/*!<I2C Function Enable */
#define I2C_CON_ACK				((uint32_t)0x00000002)			/*!<Assert Acknowledge Control Bit */
#define I2C_CON_STOP			((uint32_t)0x00000004)			/*!<I2C STOP Control Bit */
#define I2C_CON_START			((uint32_t)0x00000008)			/*!<I2C START Control Bit */
#define I2C_CON_I2C_STS			((uint32_t)0x00000010)			/*!<I2C Status */
#define I2C_CON_INTEN			((uint32_t)0x00000080)			/*!<Enable Interrupt */

/********************* Bit definition of INTSTS register **********************/
#define I2C_INTSTS_INTSTS		((uint32_t)0x00000001)			/*!<Interrupt Status */
#define I2C_INTSTS_TIF			((uint32_t)0x00000002)			/*!<Time Out Status */

/********************* Bit definition of TOUT register **********************/
#define I2C_TOUT_TOUTEN			((uint32_t)0x00000001)			/*!<Time-Out Enable */
#define I2C_TOUT_DIV4			((uint32_t)0x00000002)			/*!<Time-Out counter input clock is divided by 4 */

/********************* Bit definition of SADDR0 register **********************/
#define I2C_SADDR0_GC			((uint32_t)0x00000001)			/*!<General Call Function */
#define I2C_SADDR0_ADDR_MASK	((uint32_t)0x000000FE)			/*!<I2C Address Register */

/********************* Bit definition of SADDR1 register **********************/
#define I2C_SADDR1_GC			((uint32_t)0x00000001)			/*!<General Call Function */
#define I2C_SADDR1_ADDR_MASK	((uint32_t)0x000000FE)			/*!<I2C Address Register */

/********************* Bit definition of WKUPCON register **********************/
#define I2C_WKUPCON_WKUPEN		((uint32_t)0x00000001)			/*!<I2C Wakeup Enable */

/********************* Bit definition of WKUPSTS register **********************/
#define I2C_WKUPSTS_WKUPIF		((uint32_t)0x00000001)			/*!<I2C Wakeup Flag */



/******************************************************************************/
/*                           SPI                                              */
/******************************************************************************/
/********************* Bit definition of SPI_CTL register **********************/
#define SPI_CTL_GO_BUSY					((uint32_t)0x00000001)			/*!<Go and Busy Status */
#define SPI_CTL_RX_NEG					((uint32_t)0x00000002)			/*!<Receive on Negative Edge */
#define SPI_CTL_TX_NEG					((uint32_t)0x00000004)			/*!<Transmit on Negative Edge */
#define SPI_CTL_TX_BIT_LEN_8BIT			((uint32_t)0x00000040)			/*!<8-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_9BIT			((uint32_t)0x00000048)			/*!<9-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_10BIT		((uint32_t)0x00000050)			/*!<10-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_11BIT		((uint32_t)0x00000058)			/*!<11-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_12BIT		((uint32_t)0x00000060)			/*!<12-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_13BIT		((uint32_t)0x00000068)			/*!<13-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_14BIT		((uint32_t)0x00000070)			/*!<14-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_15BIT		((uint32_t)0x00000078)			/*!<15-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_16BIT		((uint32_t)0x00000080)			/*!<16-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_17BIT		((uint32_t)0x00000088)			/*!<17-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_18BIT		((uint32_t)0x00000090)			/*!<18-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_19BIT		((uint32_t)0x00000098)			/*!<19-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_20BIT		((uint32_t)0x000000A0)			/*!<20-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_21BIT		((uint32_t)0x000000A8)			/*!<21-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_22BIT		((uint32_t)0x000000B0)			/*!<22-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_23BIT		((uint32_t)0x000000B8)			/*!<23-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_24BIT		((uint32_t)0x000000C0)			/*!<24-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_25BIT		((uint32_t)0x000000C8)			/*!<25-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_26BIT		((uint32_t)0x000000D0)			/*!<26-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_27BIT		((uint32_t)0x000000D8)			/*!<27-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_28BIT		((uint32_t)0x000000E0)			/*!<28-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_29BIT		((uint32_t)0x000000E8)			/*!<29-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_30BIT		((uint32_t)0x000000F0)			/*!<30-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_31BIT		((uint32_t)0x000000F8)			/*!<31-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_32BIT		((uint32_t)0x00000000)			/*!<32-bit Transmit Length */
#define SPI_CTL_TX_BIT_LEN_MASK			((uint32_t)0x000000F8)			/*!<Transmit Bit Length */
#define SPI_CTL_LSB						((uint32_t)0x00000400)			/*!<Send LSB First */
#define SPI_CTL_CLKP					((uint32_t)0x00000800)			/*!<Clock Polarity */
#define SPI_CTL_SP_CYCLE				((uint32_t)0x0000F000)			/*!<Suspend Interval */
#define SPI_CTL_INTEN					((uint32_t)0x00020000)			/*!<Interrupt Enable */
#define SPI_CTL_SLAVE					((uint32_t)0x00040000)			/*!<Slave Mode Enable */
#define SPI_CTL_REORDER					((uint32_t)0x00080000)			/*!<Byte Reorder Function Enable */
#define SPI_CTL_FIFOM					((uint32_t)0x00200000)			/*!<FIFO Mode Enable */
#define SPI_CTL_TWOB					((uint32_t)0x00400000)			/*!<Two Bit Transfer Mode Enable */
#define SPI_CTL_VARCLK_EN				((uint32_t)0x00800000)			/*!<Variable Clock Enable */
#define SPI_CTL_DUAL_IO_DIR				((uint32_t)0x10000000)			/*!<Dual IO Mode Direction Control */
#define SPI_CTL_DUAL_IO_EN				((uint32_t)0x20000000)			/*!<Dual IO Mode Enable */
#define SPI_CTL_WKEUP_EN				((uint32_t)0x80000000)			/*!<Wake-Up Enable */

/********************* Bit definition of STATUS register **********************/
#define SPI_STATUS_RX_EMPTY				((uint32_t)0x00000001)			/*!<Receive FIFO Buffer Empty Flag */
#define SPI_STATUS_RX_FULL				((uint32_t)0x00000002)			/*!<Receive FIFO Buffer Full Flag */
#define SPI_STATUS_TX_EMPTY				((uint32_t)0x00000004)			/*!<Transmit FIFO Buffer Empty Flag */
#define SPI_STATUS_TX_FULL				((uint32_t)0x00000008)			/*!<Transmit FIFO Buffer Full Flag */
#define SPI_STATUS_SLV_START_INTSTS		((uint32_t)0x00000040)			/*!<Interrupt Flag */
#define SPI_STATUS_INTSTS				((uint32_t)0x00000080)			/*!<Slave Start Interrupt Status Flag */
#define SPI_STATUS_RXINT_STS			((uint32_t)0x00000100)			/*!<Receive FIFO Threshold Interrupt Flag */
#define SPI_STATUS_RX_OVER_RUN			((uint32_t)0x00000200)			/*!<Receive FIFO Overrun Interrupt Flag */
#define SPI_STATUS_TXINT_STS			((uint32_t)0x00000400)			/*!<Transmit FIFO Threshold Interrupt Flag */
#define SPI_STATUS_TIMEOUT_STS			((uint32_t)0x00001000)			/*!<Time-out Interrupt Flag */
#define SPI_STATUS_RX_FIFO_CNT			((uint32_t)0x000F0000)			/*!<Receive FIFO Data Count */
#define SPI_STATUS_TX_FIFO_CNT			((uint32_t)0x00F00000)			/*!<Transmit FIFO Data Count */

/********************* Bit definition of CLKDIV register **********************/
#define SPI_CLKDIV_DIVIDER1_MASK			((uint32_t)0x000000FF)			/*!<Clock Divider 1*/
#define SPI_CLKDIV_DIVIDER2_MASK			((uint32_t)0x00FF0000)			/*!<Clock Divider 2*/

/********************* Bit definition of SSR register **********************/
#define SPI_SSR_SSR_SS0					((uint32_t)0x00000001)			/*!<Slave Select Bit 0 */
#define SPI_SSR_SSR_SS1					((uint32_t)0x00000002)			/*!<Slave Select Bit 1 */
#define SPI_SSR_SSR_MASK				((uint32_t)0x00000003)			/*!<Slave Select Control */
#define SPI_SSR_SS_LVL					((uint32_t)0x00000004)			/*!<Slave Select Active Level Setting */
#define SPI_SSR_AUTOSS					((uint32_t)0x00000008)			/*!<Automatic Slave Select Function Enable */
#define SPI_SSR_SS_LTRIG				((uint32_t)0x00000010)			/*!<Slave Select Level Trigger */
#define SPI_SSR_NOSLVSEL				((uint32_t)0x00000020)			/*!<Slave 3-wire Mode Enable */
#define SPI_SSR_SLV_ABORT				((uint32_t)0x00000100)			/*!<Abort in Slave Mode with No Slave Selected */
#define SPI_SSR_SSTA_INTEN				((uint32_t)0x00000200)			/*!<Slave Start Interrupt Enable */
#define SPI_SSR_SS_INT_OPT				((uint32_t)0x00010000)			/*!<Slave Select Interrupt Option */

/********************* Bit definition of PDMA register **********************/
#define SPI_PDMA_TX_DMA_EN				((uint32_t)0x00000001)			/*!<Transmit PDMA Enable */
#define SPI_PDMA_RX_DMA_EN				((uint32_t)0x00000002)			/*!<Receive PDMA Enable */
#define SPI_PDMA_PDMA_RST				((uint32_t)0x00000004)			/*!<PDMA Reset */

/********************* Bit definition of FFCTL register **********************/
#define SPI_FFCTL_RX_CLR				((uint32_t)0x00000001)			/*!<Clear Receive FIFO Buffer */
#define SPI_FFCTL_TX_CLR				((uint32_t)0x00000002)			/*!<Clear Transmit FIFO Buffer */
#define SPI_FFCTL_RXINT_EN				((uint32_t)0x00000004)			/*!<Receive FIFO Threshold Interrupt Enable */
#define SPI_FFCTL_TXINT_EN				((uint32_t)0x00000008)			/*!<Transmit FIFO Threshold Interrupt Enable */
#define SPI_FFCTL_RXOV_INTEN			((uint32_t)0x00000010)			/*!<Receive FIFO Overrun Interrupt Enable */
#define SPI_FFCTL_TIMEOUT_INTEN			((uint32_t)0x00000080)			/*!<Receive FIFO Time-out Interrupt Enable */
#define SPI_FFCTL_RX_THRESHOLD_L1		((uint32_t)0x01000000)			/*!<Receive FIFO Threshold, Level 1 */
#define SPI_FFCTL_RX_THRESHOLD_L2		((uint32_t)0x02000000)			/*!<Receive FIFO Threshold, Level 2 */
#define SPI_FFCTL_RX_THRESHOLD_L3		((uint32_t)0x03000000)			/*!<Receive FIFO Threshold, Level 3 */
#define SPI_FFCTL_RX_THRESHOLD_L4		((uint32_t)0x04000000)			/*!<Receive FIFO Threshold, Level 4 */
#define SPI_FFCTL_RX_THRESHOLD_L5		((uint32_t)0x05000000)			/*!<Receive FIFO Threshold, Level 5 */
#define SPI_FFCTL_RX_THRESHOLD_L6		((uint32_t)0x06000000)			/*!<Receive FIFO Threshold, Level 6 */
#define SPI_FFCTL_RX_THRESHOLD_L7		((uint32_t)0x07000000)			/*!<Receive FIFO Threshold, Level 7 */
#define SPI_FFCTL_RX_THRESHOLD_MASK		((uint32_t)0x07000000)			/*!<Receive FIFO Threshold */
#define SPI_FFCTL_TX_THRESHOLD_L1		((uint32_t)0x10000000)			/*!<Transmit FIFO Threshold, Level 1 */
#define SPI_FFCTL_TX_THRESHOLD_L2		((uint32_t)0x20000000)			/*!<Transmit FIFO Threshold, Level 2 */
#define SPI_FFCTL_TX_THRESHOLD_L3		((uint32_t)0x30000000)			/*!<Transmit FIFO Threshold, Level 3 */
#define SPI_FFCTL_TX_THRESHOLD_L4		((uint32_t)0x40000000)			/*!<Transmit FIFO Threshold, Level 4 */
#define SPI_FFCTL_TX_THRESHOLD_L5		((uint32_t)0x50000000)			/*!<Transmit FIFO Threshold, Level 5 */
#define SPI_FFCTL_TX_THRESHOLD_L6		((uint32_t)0x60000000)			/*!<Transmit FIFO Threshold, Level 6 */
#define SPI_FFCTL_TX_THRESHOLD_L7		((uint32_t)0x70000000)			/*!<Transmit FIFO Threshold, Level 7 */
#define SPI_FFCTL_TX_THRESHOLD_MASK		((uint32_t)0x70000000)			/*!<Transmit FIFO Threshold */



/******************************************************************************/
/*                PWM                                                         */
/******************************************************************************/
/********************* Bit definition of PRES register **********************/
#define PWM_PRES_CP01_MASK			((uint32_t)0x000000FF)			/*!<Clock prescaler 0 for PWM Channel 0 & 1 */
#define PWM_PRES_CP23_MASK			((uint32_t)0x0000FF00)			/*!<Clock prescaler 2 for PWM Channel 2 & 3 */
#define PWM_PRES_DZ01_MASK			((uint32_t)0x00FF0000)			/*!<Dead zone interval register for pair of CH0 and CH1 */
#define PWM_PRES_DZ23_MASK			((uint32_t)0xFF000000)			/*!<Dead zone interval register for pair of CH2 and CH3 */

/********************* Bit definition of CLKSEL register **********************/
#define PWM_CLKSEL_CLKSEL_DIV2		((uint32_t)0x00000000)			/*!<Input Clock Divided by 2 */
#define PWM_CLKSEL_CLKSEL_DIV4		((uint32_t)0x00000001)			/*!<Input Clock Divided by 4 */
#define PWM_CLKSEL_CLKSEL_DIV8		((uint32_t)0x00000002)			/*!<Input Clock Divided by 8 */
#define PWM_CLKSEL_CLKSEL_DIV16		((uint32_t)0x00000003)			/*!<Input Clock Divided by 16 */
#define PWM_CLKSEL_CLKSEL_DIV1		((uint32_t)0x00000004)			/*!<Input Clock Divided by 1 */
#define PWM_CLKSEL_CLKSEL_MASK		((uint32_t)0x0000000F)			/*!<Clock Source Selection Mask */

#define PWM_CLKSEL_CLKSEL0_DIV2		((uint32_t)0x00000000)			/*!<Channel 0 Input Clock Divided by 2 */
#define PWM_CLKSEL_CLKSEL0_DIV4		((uint32_t)0x00000001)			/*!<Channel 0 Input Clock Divided by 4 */
#define PWM_CLKSEL_CLKSEL0_DIV8		((uint32_t)0x00000002)			/*!<Channel 0 Input Clock Divided by 8 */
#define PWM_CLKSEL_CLKSEL0_DIV16	((uint32_t)0x00000003)			/*!<Channel 0 Input Clock Divided by 16 */
#define PWM_CLKSEL_CLKSEL0_DIV1		((uint32_t)0x00000004)			/*!<Channel 0 Input Clock Divided by 1 */
#define PWM_CLKSEL_CLKSEL0_MASK		((uint32_t)0x0000000F)			/*!<Channel 0 Clock Source Selection Mask */
#define PWM_CLKSEL_CLKSEL1_DIV2		((uint32_t)0x00000000)			/*!<Channel 1 Input Clock Divided by 2 */
#define PWM_CLKSEL_CLKSEL1_DIV4		((uint32_t)0x00000010)			/*!<Channel 1 Input Clock Divided by 4 */
#define PWM_CLKSEL_CLKSEL1_DIV8		((uint32_t)0x00000020)			/*!<Channel 1 Input Clock Divided by 8 */
#define PWM_CLKSEL_CLKSEL1_DIV16	((uint32_t)0x00000030)			/*!<Channel 1 Input Clock Divided by 16 */
#define PWM_CLKSEL_CLKSEL1_DIV1		((uint32_t)0x00000040)			/*!<Channel 1 Input Clock Divided by 1 */
#define PWM_CLKSEL_CLKSEL1_MASK		((uint32_t)0x000000F0)			/*!<Channel 1 Clock Source Selection Mask  */
#define PWM_CLKSEL_CLKSEL2_DIV2		((uint32_t)0x00000000)			/*!<Channel 2 Input Clock Divided by 2 */
#define PWM_CLKSEL_CLKSEL2_DIV4		((uint32_t)0x00000100)			/*!<Channel 2 Input Clock Divided by 4 */
#define PWM_CLKSEL_CLKSEL2_DIV8		((uint32_t)0x00000200)			/*!<Channel 2 Input Clock Divided by 8 */
#define PWM_CLKSEL_CLKSEL2_DIV16	((uint32_t)0x00000300)			/*!<Channel 2 Input Clock Divided by 16 */
#define PWM_CLKSEL_CLKSEL2_DIV1		((uint32_t)0x00000400)			/*!<Channel 2 Input Clock Divided by 1 */
#define PWM_CLKSEL_CLKSEL2_MASK		((uint32_t)0x00000F00)			/*!<Channel 2 Clock Source Selection Mask  */
#define PWM_CLKSEL_CLKSEL3_DIV2		((uint32_t)0x00000000)			/*!<Channel 3 Input Clock Divided by 2 */
#define PWM_CLKSEL_CLKSEL3_DIV4		((uint32_t)0x00001000)			/*!<Channel 3 Input Clock Divided by 4 */
#define PWM_CLKSEL_CLKSEL3_DIV8		((uint32_t)0x00002000)			/*!<Channel 3 Input Clock Divided by 8 */
#define PWM_CLKSEL_CLKSEL3_DIV16	((uint32_t)0x00003000)			/*!<Channel 3 Input Clock Divided by 16 */
#define PWM_CLKSEL_CLKSEL3_DIV1		((uint32_t)0x00004000)			/*!<Channel 3 Input Clock Divided by 1 */
#define PWM_CLKSEL_CLKSEL3_MASK		((uint32_t)0x0000F000)			/*!<Channel 3 Clock Source Selection Mask  */

/********************* Bit definition of CTL register **********************/
#define PWM_CTL_CHMOD_AUTO_RELOAD		((uint32_t)0x00000008)			/*!<PWM Auto Reload Mode */
#define PWM_CTL_CHMOD_ONE_SHOT			((uint32_t)0x00000000)			/*!<PWM One Shot Mode */


#define PWM_CTL_CH0EN				((uint32_t)0x00000001)			/*!<PWM Channel 0 Enable/Disable Start Run */
#define PWM_CTL_CH0INV				((uint32_t)0x00000004)			/*!<PWM Channel 0 Output Inverter ON/OFF */
#define PWM_CTL_CH0MOD				((uint32_t)0x00000008)			/*!<PWM Channel 0 Continuous/One-Shot Mode */
#define PWM_CTL_DZEN01				((uint32_t)0x00000010)			/*!<Dead Zone 0 Generator Enable/Disable */
#define PWM_CTL_DZEN23				((uint32_t)0x00000020)			/*!<Dead Zone 2 Generator Enable/Disable */
#define PWM_CTL_CH1EN				((uint32_t)0x00000100)			/*!<PWM Channel 1 Enable/Disable Start Run */
#define PWM_CTL_CH1INV				((uint32_t)0x00000400)			/*!<PWM Channel 1 Output Inverter ON/OFF */
#define PWM_CTL_CH1MOD				((uint32_t)0x00000800)			/*!<PWM Channel 1 Continuous/One-Shot Mode */
#define PWM_CTL_CH2EN				((uint32_t)0x00010000)			/*!<PWM Channel 2 Enable/Disable Start Run */
#define PWM_CTL_CH2INV				((uint32_t)0x00040000)			/*!<PWM Channel 2 Output Inverter ON/OFF */
#define PWM_CTL_CH2MOD				((uint32_t)0x00080000)			/*!<PWM Channel 2 Continuous/One-Shot Mode */
#define PWM_CTL_CH3EN				((uint32_t)0x01000000)			/*!<PWM Channel 3 Enable/Disable Start Run */
#define PWM_CTL_CH3INV				((uint32_t)0x04000000)			/*!<PWM Channel 3 Output Inverter ON/OFF */
#define PWM_CTL_CH3MOD				((uint32_t)0x08000000)			/*!<PWM Channel 3 Continuous/One-Shot Mode */

/********************* Bit definition of INTEN register **********************/
#define PWM_INTEN_TMIE0				((uint32_t)0x00000001)			/*!<PWM Channel 0 Interrupt Enable */
#define PWM_INTEN_TMIE1				((uint32_t)0x00000002)			/*!<PWM Channel 1 Interrupt Enable */
#define PWM_INTEN_TMIE2				((uint32_t)0x00000004)			/*!<PWM Channel 2 Interrupt Enable */
#define PWM_INTEN_TMIE3				((uint32_t)0x00000008)			/*!<PWM Channel 3 Interrupt Enable */

/********************* Bit definition of INTSTS register **********************/
#define PWM_INTSTS_TMINT0			((uint32_t)0x00000001)			/*!<PWM Channel 0 Interrupt Flag */
#define PWM_INTSTS_TMINT1			((uint32_t)0x00000002)			/*!<PWM Channel 1 Interrupt Flag */
#define PWM_INTSTS_TMINT2			((uint32_t)0x00000004)			/*!<PWM Channel 2 Interrupt Flag */
#define PWM_INTSTS_TMINT3			((uint32_t)0x00000008)			/*!<PWM Channel 3 Interrupt Flag */
#define PWM_INTSTS_DUTY0SYNCFLAG	((uint32_t)0x00000010)			/*!<Duty0 Synchronize Flag */
#define PWM_INTSTS_DUTY1SYNCFLAG	((uint32_t)0x00000020)			/*!<Duty1 Synchronize Flag */
#define PWM_INTSTS_DUTY2SYNCFLAG	((uint32_t)0x00000040)			/*!<Duty2 Synchronize Flag */
#define PWM_INTSTS_DUTY3SYNCFLAG	((uint32_t)0x00000080)			/*!<Duty3 Synchronize Flag */
#define PWM_INTSTS_PRESSYNCFLAG		((uint32_t)0x00000100)			/*!<Prescale Synchronize Flag */

/********************* Bit definition of OE register **********************/
#define PWM_OE_CH0					((uint32_t)0x00000001)			/*!<PWM Channel 0 Output Enable */
#define PWM_OE_CH1					((uint32_t)0x00000002)			/*!<PWM Channel 1 Output Enable */
#define PWM_OE_CH2					((uint32_t)0x00000004)			/*!<PWM Channel 2 Output Enable */
#define PWM_OE_CH3					((uint32_t)0x00000008)			/*!<PWM Channel 3 Output Enable */

/********************* Bit definition of DUTY register **********************/
#define PWM_DUTY_CN_MASK			((uint32_t)0x0000FFFF)			/*!<PWM Counter/Timer Loaded Value */
#define PWM_DUTY_CM_MASK			((uint32_t)0xFFFF0000)			/*!<PWM Comparator Mask */

/********************* Bit definition of CAPCTL register **********************/
#define PWM_CAPCTL_PDMACAPMOD_CRL	((uint32_t)0x00000010)			/*!<PDMA Transfer CRL */
#define PWM_CAPCTL_PDMACAPMOD_CFL	((uint32_t)0x00000020)			/*!<PDMA Transfer CFL*/
#define PWM_CAPCTL_PDMACAPMOD_BOTH	((uint32_t)0x00000030)			/*!<PDMA Transfer Both CRL and CFL */
#define PWM_CAPCTL_PDMACAPMOD_MASK	((uint32_t)0x00000030)			/*!<PDMA Transfer Mask*/

#define PWM_CAPCTL_CAPRELOADREN		((uint32_t)0x00000040)			/*!<Reload CNR When Capture Rising Event Comes */
#define PWM_CAPCTL_CAPRELOADFEN		((uint32_t)0x00000080)			/*!<Reload CNR When Capture Falling Event Comes */
#define PWM_CAPCTL_CAPRELOAD_MASK	((uint32_t)0x000000C0)			/*!<Reload MASK When Capture Rising Event Comes */

#define PWM_CAPCTL_CHRFORDER_R		((uint32_t)0x00001000)			/*!<Select CRL as First Captured Data PDMA Transferred */
#define PWM_CAPCTL_CHRFORDER_F		((uint32_t)0x00000000)			/*!<Select CFL as First Captured Data PDMA Transferred */


#define PWM_CAPCTL_INV0				((uint32_t)0x00000001)			/*!<Channel 0 Inverter ON/OFF */
#define PWM_CAPCTL_CAPCH0EN			((uint32_t)0x00000002)			/*!<Capture Channel 0 Transition Enable/Disable */
#define PWM_CAPCTL_CAPCH0PADEN		((uint32_t)0x00000004)			/*!<Capture Channel 0 Input Enable */
#define PWM_CAPCTL_CH0PDMAEN		((uint32_t)0x00000008)			/*!<Channel 0 PDMA Enable */
#define PWM_CAPCTL_PDMACAPMOD0_CRL0	((uint32_t)0x00000010)			/*!<PDMA Transfer CRL0 */
#define PWM_CAPCTL_PDMACAPMOD0_CFL0	((uint32_t)0x00000020)			/*!<PDMA Transfer CFL0*/
#define PWM_CAPCTL_PDMACAPMOD0_BOTH	((uint32_t)0x00000030)			/*!<PDMA Transfer Both CRL0 and CFL0 */
#define PWM_CAPCTL_PDMACAPMOD0_MASK	((uint32_t)0x00000030)			/*!<PDMA Transfer Mask*/
#define PWM_CAPCTL_CAPRELOADREN0	((uint32_t)0x00000040)			/*!<Reload CNR0 When CH0 Capture Rising Event Comes */
#define PWM_CAPCTL_CAPRELOADFEN0	((uint32_t)0x00000080)			/*!<Reload CNR0 When CH0 Capture Falling Event Comes */
#define PWM_CAPCTL_INV1				((uint32_t)0x00000100)			/*!<Channel 1 Inverter ON/OFF */
#define PWM_CAPCTL_CAPCH1EN			((uint32_t)0x00000200)			/*!<Capture Channel 1 Transition Enable/Disable */
#define PWM_CAPCTL_CAPCH1PADEN		((uint32_t)0x00000400)			/*!<Capture Channel 1 Input Enable */
#define PWM_CAPCTL_CH0RFORDER		((uint32_t)0x00001000)			/*!<Select the First Captured Data PDMA Transferred */
#define PWM_CAPCTL_CH01CASK			((uint32_t)0x00002000)			/*!<Cascade Channel 0 and Channel 1 PWM Timer for Capturing Usage */
#define PWM_CAPCTL_CAPRELOADREN1	((uint32_t)0x00004000)			/*!<Reload CNR1 When CH1 Capture Rising Event Comes */
#define PWM_CAPCTL_CAPRELOADFEN1	((uint32_t)0x00008000)			/*!<Reload CNR1 When CH1 Capture Falling Event Comes  */
#define PWM_CAPCTL_INV2				((uint32_t)0x00010000)			/*!<Channel 2 Inverter ON/OFF */
#define PWM_CAPCTL_CAPCH2EN			((uint32_t)0x00020000)			/*!<Capture Channel 2 Transition Enable/Disable */
#define PWM_CAPCTL_CAPCH2PADEN		((uint32_t)0x00040000)			/*!<Capture Channel 2 Input Enable */
#define PWM_CAPCTL_CH2PDMAEN		((uint32_t)0x00080000)			/*!<Channel 2 PDMA Enable */
#define PWM_CAPCTL_PDMACAPMOD2_CRL2	((uint32_t)0x00100000)			/*!<PDMA Transfer CRL2 */
#define PWM_CAPCTL_PDMACAPMOD2_CFL2	((uint32_t)0x00200000)			/*!<PDMA Transfer CFL2*/
#define PWM_CAPCTL_PDMACAPMOD2_BOTH	((uint32_t)0x00300000)			/*!<PDMA Transfer Both CRL2 and CFL2 */
#define PWM_CAPCTL_PDMACAPMOD2_MASK	((uint32_t)0x00300000)			/*!<PDMA Transfer Mask*/
#define PWM_CAPCTL_CAPRELOADREN2	((uint32_t)0x00400000)			/*!<Reload CNR2 When CH2 Capture Rising Event Comes */
#define PWM_CAPCTL_CAPRELOADFEN2	((uint32_t)0x00800000)			/*!<Reload CNR2 When CH2 Capture Falling Event Comes */
#define PWM_CAPCTL_INV3				((uint32_t)0x01000000)			/*!<Channel 3 Inverter ON/OFF */
#define PWM_CAPCTL_CAPCH3EN			((uint32_t)0x02000000)			/*!<Capture Channel 3 Transition Enable/Disable */
#define PWM_CAPCTL_CAPCH3PADEN		((uint32_t)0x04000000)			/*!<Capture Channel 3 Input Enable */
#define PWM_CAPCTL_CH3RFORDER		((uint32_t)0x10000000)			/*!<Select the First Captured Data PDMA Transferred */
#define PWM_CAPCTL_CH23CASK			((uint32_t)0x20000000)			/*!<Cascade Channel 2 and Channel 3 PWM Timer for Capturing Usage */
#define PWM_CAPCTL_CAPRELOADREN3	((uint32_t)0x40000000)			/*!<Reload CNR1 When CH3 Capture Rising Event Comes */
#define PWM_CAPCTL_CAPRELOADFEN3	((uint32_t)0x80000000)			/*!<Reload CNR1 When CH3 capture Falling Event Comesg  */

/********************* Bit definition of CAPINTEN register **********************/
#define PWM_CAPINTEN_CRLIE			((uint32_t)0x00000001)			/*!<Rising Latch Interrupt Enable ON/OFF */
#define PWM_CAPINTEN_CFLIE			((uint32_t)0x00000002)			/*!<Falling Latch Interrupt Enable ON/OFF */

#define PWM_CAPINTEN_CRLIE0			((uint32_t)0x00000001)			/*!<Channel 0 Rising Latch Interrupt Enable ON/OFF */
#define PWM_CAPINTEN_CFLIE0			((uint32_t)0x00000002)			/*!<Channel 0 Falling Latch Interrupt Enable ON/OFF */
#define PWM_CAPINTEN_CRLIE1			((uint32_t)0x00000100)			/*!<Channel 1 Rising Latch Interrupt Enable ON/OFF */
#define PWM_CAPINTEN_CFLIE1			((uint32_t)0x00000200)			/*!<Channel 1 Falling Latch Interrupt Enable ON/OFF */
#define PWM_CAPINTEN_CRLIE2			((uint32_t)0x00010000)			/*!<Channel 2 Rising Latch Interrupt Enable ON/OFF */
#define PWM_CAPINTEN_CFLIE2			((uint32_t)0x00020000)			/*!<Channel 2 Falling Latch Interrupt Enable ON/OFF */
#define PWM_CAPINTEN_CRLIE3			((uint32_t)0x01000000)			/*!<Channel 3 Rising Latch Interrupt Enable ON/OFF */
#define PWM_CAPINTEN_CFLIE3			((uint32_t)0x02000000)			/*!<Channel 3 Falling Latch Interrupt Enable ON/OFF */

/********************* Bit definition of CAPINTSTS register **********************/
#define PWM_CAPINTSTS_CRLI			((uint32_t)0x00000002)			/*!<PWM_CRL Latched Indicator Bit */
#define PWM_CAPINTSTS_CFLI			((uint32_t)0x00000004)			/*!<PWM_CFL Latched Indicator Bit */

#define PWM_CAPINTSTS_CAPIF0		((uint32_t)0x00000001)			/*!<Capture 0 Interrupt Indication Flag */
#define PWM_CAPINTSTS_CRLI0			((uint32_t)0x00000002)			/*!<PWM_CRL0 Latched Indicator Bit */
#define PWM_CAPINTSTS_CFLI0			((uint32_t)0x00000004)			/*!<PWM_CFL0 Latched Indicator Bit */
#define PWM_CAPINTSTS_CAPOVR0		((uint32_t)0x00000008)			/*!<Capture Rising Flag over run for channel 0 */
#define PWM_CAPINTSTS_CAPOVF0		((uint32_t)0x00000010)			/*!<Capture Falling Flag over run for channel 0 */
#define PWM_CAPINTSTS_CAPIF1		((uint32_t)0x00000100)			/*!<Capture 1 Interrupt Indication Flag */
#define PWM_CAPINTSTS_CRLI1			((uint32_t)0x00000200)			/*!<PWM_CRL1 Latched Indicator Bit */
#define PWM_CAPINTSTS_CFLI1			((uint32_t)0x00000400)			/*!<PWM_CFL1 Latched Indicator Bit */
#define PWM_CAPINTSTS_CAPOVR1		((uint32_t)0x00000800)			/*!<Capture Rising Flag over run for channel 1 */
#define PWM_CAPINTSTS_CAPOVF1		((uint32_t)0x00001000)			/*!<Capture Falling Flag over run for channel 1 */
#define PWM_CAPINTSTS_CAPIF2		((uint32_t)0x00010000)			/*!<Capture 2 Interrupt Indication Flag */
#define PWM_CAPINTSTS_CRLI2			((uint32_t)0x00020000)			/*!<PWM_CRL2 Latched Indicator Bit */
#define PWM_CAPINTSTS_CFLI2			((uint32_t)0x00040000)			/*!<PWM_CFL2 Latched Indicator Bit */
#define PWM_CAPINTSTS_CAPOVR2		((uint32_t)0x00080000)			/*!<Capture Rising Flag over run for channel 2 */
#define PWM_CAPINTSTS_CAPOVF2		((uint32_t)0x00100000)			/*!<Capture Falling Flag over run for channel 2 */
#define PWM_CAPINTSTS_CAPIF3		((uint32_t)0x01000000)			/*!<Capture 3 Interrupt Indication Flag */
#define PWM_CAPINTSTS_CRLI3			((uint32_t)0x02000000)			/*!<PWM_CRL3 Latched Indicator Bit */
#define PWM_CAPINTSTS_CFLI3			((uint32_t)0x04000000)			/*!<PWM_CFL3 Latched Indicator Bit */
#define PWM_CAPINTSTS_CAPOVR3		((uint32_t)0x08000000)			/*!<Capture Rising Flag over run for channel 3 */
#define PWM_CAPINTSTS_CAPOVF3		((uint32_t)0x10000000)			/*!<Capture Falling Flag over run for channel 3 */




/******************************************************************************/
/*                UART                                                        */
/******************************************************************************/
/********************* Bit definition of CTL register **********************/
#define UART_CTL_RX_RST				((uint32_t)0x00000001)			/*!<RX Software Reset */
#define UART_CTL_TX_RST				((uint32_t)0x00000002)			/*!<TX Software Reset */
#define UART_CTL_RX_DIS				((uint32_t)0x00000004)			/*!<Receiver Disable Register */
#define UART_CTL_TX_DIS				((uint32_t)0x00000008)			/*!<Transfer Disable Register */
#define UART_CTL_AUTO_RTS_EN		((uint32_t)0x00000010)			/*!<RTS Auto-Flow Control Enable */
#define UART_CTL_AUTO_CTS_EN		((uint32_t)0x00000020)			/*!<CTS Auto-Flow Control Enable */
#define UART_CTL_DMA_RX_EN			((uint32_t)0x00000040)			/*!<RX DMA Enable */
#define UART_CTL_DMA_TX_EN			((uint32_t)0x00000080)			/*!<TX DMA Enable */
#define UART_CTL_WAKE_CTS_EN		((uint32_t)0x00000100)			/*!<CTS Wake-Up Function Enable */
#define UART_CTL_ABAUD_EN			((uint32_t)0x00001000)			/*!<Auto-Baud Rate Detect Enable */
/********************* Bit definition of TLCTL register **********************/
#define UART_TLCTL_DATA_LEN			((uint32_t)0x00000003)			/*!<Data Length */
#define UART_TLCTL_NSB				((uint32_t)0x00000004)			/*!<Number of STOP Bit Length */
#define UART_TLCTL_PBE				((uint32_t)0x00000008)			/*!<Parity Bit Enable */
#define UART_TLCTL_EPE				((uint32_t)0x00000010)			/*!<Even Parity Enable */
#define UART_TLCTL_SPE				((uint32_t)0x00000020)			/*!<Stick Parity Enable */
#define UART_TLCTL_BCB				((uint32_t)0x00000040)			/*!<Break Control Bit */
#define UART_TLCTL_RFITL			((uint32_t)0x00000300)			/*!<RX-FIFO Interrupt (INT_RDA) Trigger Level */
#define UART_TLCTL_RTS_TRI_LEV		((uint32_t)0x00003000)			/*!<RTSn Trigger Level (For Auto-flow Control Use) */
/********************* Bit definition of IER register **********************/
#define UART_IER_RDA_IE				((uint32_t)0x00000001)			/*!<Receive Data Available Interrupt Enable */
#define UART_IER_THRE_IE			((uint32_t)0x00000002)			/*!<Transmit Holding Register Empty Interrupt Enable */
#define UART_IER_RLS_IE				((uint32_t)0x00000004)			/*!<Receive Line Status Interrupt Enable */
#define UART_IER_MODEM_IE			((uint32_t)0x00000008)			/*!<Modem Status Interrupt Enable */
#define UART_IER_RTO_IE				((uint32_t)0x00000010)			/*!<RX Time-Out Interrupt Enable */
#define UART_IER_BUF_ERR_IE			((uint32_t)0x00000020)			/*!<Buffer Error Interrupt Enable */
#define UART_IER_WAKE_IE			((uint32_t)0x00000040)			/*!<Wake-Up Interrupt Enable */
#define UART_IER_ABAUD_IE			((uint32_t)0x00000080)			/*!<Auto-Baud Rate Interrupt Enable */
#define UART_IER_LIN_IE				((uint32_t)0x00000100)			/*!<LIN Interrupt Enable */
/********************* Bit definition of ISR register **********************/
#define UART_ISR_RDA_IS				((uint32_t)0x00000001)			/*!<Receive Data Available Interrupt Flag (Read Only) */
#define UART_ISR_THRE_IS			((uint32_t)0x00000002)			/*!<Transmit Holding Register Empty Interrupt Flag (Read Only) */
#define UART_ISR_RLS_IS				((uint32_t)0x00000004)			/*!<Receive Line Interrupt Status Flag (Read Only) */
#define UART_ISR_MODEM_IS			((uint32_t)0x00000008)			/*!<MODEM Interrupt Status Flag (Read Only) */
#define UART_ISR_RTO_IS				((uint32_t)0x00000010)			/*!<RX Time-Out Interrupt Status Flag (Read Only) */
#define UART_ISR_BUF_ERR_IS			((uint32_t)0x00000020)			/*!<Buffer Error Interrupt Status Flag (Read Only) */
#define UART_ISR_WAKE_IS			((uint32_t)0x00000040)			/*!<Wake-Up Interrupt Status Flag (Read Only) */
#define UART_ISR_ABAUD_IS			((uint32_t)0x00000080)			/*!<Auto-Baud Rate Interrupt Status Flag (Read Only) */
#define UART_ISR_LIN_IS				((uint32_t)0x00000100)			/*!<LIN Interrupt Status Flag (Read Only) */
/********************* Bit definition of TRSR register **********************/
#define UART_TRSR_RS485_ADDET_F		((uint32_t)0x00000001)			/*!<RS-485 Address Byte Detection Status Flag (Read Only) */
#define UART_TRSR_ABAUD_F			((uint32_t)0x00000002)			/*!<Auto-Baud Rate Interrupt (Read Only) */
#define UART_TRSR_ABAUD_TOUT_F		((uint32_t)0x00000004)			/*!<Auto-Baud Rate Time-Out Interrupt (Read Only) */
#define UART_TRSR_LIN_TX_F			((uint32_t)0x00000008)			/*!<LIN TX Interrupt Flag (Read Only) */
#define UART_TRSR_LIN_RX_F			((uint32_t)0x00000010)			/*!<LIN RX Interrupt Flag (Read Only) */
#define UART_TRSR_BIT_ERR_F			((uint32_t)0x00000020)			/*!<Bit Error Detect Status Flag (Read Only) */
#define UART_TRSR_LIN_RX_SYNC_ERR_F	((uint32_t)0x00000100)			/*!<LIN RX SYNC Error Flag (Read Only) */
/********************* Bit definition of FSR register **********************/
#define UART_FSR_RX_OVER_F			((uint32_t)0x00000001)			/*!<RX Overflow Error Status Flag (Read Only) */
#define UART_FSR_RX_EMPTY_F			((uint32_t)0x00000002)			/*!<Receiver FIFO Empty (Read Only) */
#define UART_FSR_RX_FULL_F			((uint32_t)0x00000004)			/*!<Receiver FIFO Full (Read Only) */
#define UART_FSR_PE_F				((uint32_t)0x00000010)			/*!<Parity Error State Status Flag (Read Only) */
#define UART_FSR_FE_F				((uint32_t)0x00000020)			/*!<Framing Error Status Flag (Read Only) */
#define UART_FSR_BI_F				((uint32_t)0x00000040)			/*!<Break Status Flag (Read Only) */
#define UART_FSR_TX_OVER_F			((uint32_t)0x00000100)			/*!<TX Overflow Error Interrupt Status Flag (Read Only) */
#define UART_FSR_TX_EMPTY_F			((uint32_t)0x00000200)			/*!<Transmitter FIFO Empty  (Read Only) */
#define UART_FSR_TX_FULL_F			((uint32_t)0x00000400)			/*!<Transmitter FIFO Full  (Read Only) */
#define UART_FSR_TE_F				((uint32_t)0x00000800)			/*!<Transmitter Empty Status Flag (Read Only) */
#define UART_FSR_RX_POINTER_F		((uint32_t)0x001F0000)			/*!<RX-FIFO Pointer (Read Only) */
#define UART_FSR_TX_POINTER_F		((uint32_t)0x1F000000)			/*!<TX-FIFO Pointer (Read Only) */
/********************* Bit definition of MCSR register **********************/
#define UART_MCSR_LEV_RTS			((uint32_t)0x00000001)			/*!<RTS Trigger Level */
#define UART_MCSR_RTS_ST			((uint32_t)0x00000002)			/*!<RTS Pin State (Read Only) */
#define UART_MCSR_LEV_CTS			((uint32_t)0x00010000)			/*!<CTS Trigger Level */
#define UART_MCSR_CTS_ST			((uint32_t)0x00020000)			/*!<CTS Pin Status (Read Only) */
#define UART_MCSR_DCT_F				((uint32_t)0x00040000)			/*!<Detect CTS State Change Status Flag (Read Only) */
/********************* Bit definition of TMCTL register **********************/
#define UART_TMCTL_TOIC				((uint32_t)0x000001FF)			/*!<Time-Out Comparator */
#define UART_TMCTL_DLY				((uint32_t)0x00FF0000)			/*!<TX Delay Time Value */
/********************* Bit definition of BAUD register **********************/
#define UART_BAUD_BRD				((uint32_t)0x0000FFFF)			/*!<Baud Rate Divider */
#define UART_BAUD_DIV_16_EN			((uint32_t)0x80000000)			/*!<Divider 16 Enable */
/********************* Bit definition of IRCR register **********************/
#define UART_IRCR_TX_SELECT			((uint32_t)0x00000002)			/*!<TX_SELECT */
#define UART_IRCR_INV_TX			((uint32_t)0x00000020)			/*!<Inverse TX output signal */
#define UART_IRCR_INV_RX			((uint32_t)0x00000040)			/*!<Inverse RX input signal */
/********************* Bit definition of ALT_CTL register **********************/
#define UART_ALT_CTL_LIN_TX_BCNT	((uint32_t)0x00000007)			/*!<LIN TX Break Field Count Register */
#define UART_ALT_CTL_LIN_HEAD_SEL	((uint32_t)0x00000030)			/*!<LIN Header Selection */
#define UART_ALT_CTL_LIN_RX_EN		((uint32_t)0x00000040)			/*!<LIN RX Enable */
#define UART_ALT_CTL_LIN_TX_EN		((uint32_t)0x00000080)			/*!<LIN TX Header Trigger Enable */
#define UART_ALT_CTL_BIT_ERR_EN		((uint32_t)0x00000100)			/*!<Bit Error Detect Enable */
#define UART_ALT_CTL_RS485_NMM		((uint32_t)0x00010000)			/*!<RS-485 Normal Multi-Drop Operation Mode (RS-485 NMM Mode) */
#define UART_ALT_CTL_RS485_AAD		((uint32_t)0x00020000)			/*!<RS-485 Auto Address Detection Operation Mode (RS-485 AAD Mode) */
#define UART_ALT_CTL_RS485_AUD		((uint32_t)0x00040000)			/*!<RS-485 Auto Direction Mode (RS-485 AUD Mode) */
#define UART_ALT_CTL_RS485_ADD_EN	((uint32_t)0x00080000)			/*!<RS-485 Address Detection Enable */
#define UART_ALT_CTL_ADDR_PID_MATCH	((uint32_t)0xFF000000)			/*!<Address / PID Match Value Register */
/********************* Bit definition of FUN_SEL register **********************/
#define UART_FUN_SEL_FUN_SEL		((uint32_t)0x00000003)			/*!<Function Select Enable */


/******************************************************************************/
/*                USB                                                         */
/******************************************************************************/
/********************* Bit definition of CTRL register **********************/
#define USB_CTRL_USB_EN				((uint32_t)0x00000001)			/*!<USB Function Enable */
#define USB_CTRL_PHY_EN				((uint32_t)0x00000002)			/*!<PHY Transceiver Enable */
#define USB_CTRL_PWRDB				((uint32_t)0x00000004)			/*!<Power down PHY Transceiver, Low Active */
#define USB_CTRL_DPPU_EN			((uint32_t)0x00000008)			/*!<Pull-Up Resistor on USB_DP Enable */
#define USB_CTRL_DRVSE0				((uint32_t)0x00000010)			/*!<Force USB PHY Transceiver to Drive SE0 */
#define USB_CTRL_RWAKEUP			((uint32_t)0x00000100)			/*!<Force USB bus to K state, used for remote wake-up */
#define USB_CTRL_WAKEUP_EN			((uint32_t)0x00000200)			/*!<Wake-Up Function Enable */

/********************* Bit definition of BUSSTS register **********************/
#define USB_BUSSTS_USBRST			((uint32_t)0x00000001)			/*!<USB Reset Status */
#define USB_BUSSTS_SUSPEND			((uint32_t)0x00000002)			/*!<USB Suspend Status */
#define USB_BUSSTS_RESUME			((uint32_t)0x00000004)			/*!<USB Resume Status */
#define USB_BUSSTS_TIMEOUT			((uint32_t)0x00000008)			/*!<USB Time Out Flag */
#define USB_BUSSTS_FLDET_IN			((uint32_t)0x00000010)			/*!<USB Device Floating Detection - plug-in */
#define USB_BUSSTS_FLDET_OUT		((uint32_t)0x00000000)			/*!<USB Device Floating Detection - plug-out */

/********************* Bit definition of INTEN register **********************/
#define USB_INTEN_BUSEVT			((uint32_t)0x00000001)			/*!<USB Bus Event Interrupt Enable */
#define USB_INTEN_USBEVT			((uint32_t)0x00000002)			/*!<USB Event Interrupt Enable */
#define USB_INTEN_FLDET				((uint32_t)0x00000004)			/*!<USB Floating Detect Interrupt Enable */
#define USB_INTEN_WAKEUP			((uint32_t)0x00000008)			/*!<USB Wake-up Interrupt Enable */

/********************* Bit definition of INTSTS register **********************/
#define USB_INTSTS_BUS_STS			((uint32_t)0x00000001)			/*!<USB BUS Interrupt Status */
#define USB_INTSTS_USB_STS			((uint32_t)0x00000002)			/*!<USB USB Interrupt Status */
#define USB_INTSTS_FLD_STS			((uint32_t)0x00000004)			/*!<USB Floating Interrupt Status */
#define USB_INTSTS_WKEUP_STS		((uint32_t)0x00000008)			/*!<USB Wake-up Interrupt Status */
#define USB_INTSTS_EPEVT0			((uint32_t)0x00010000)			/*!<USB Event Status on EP0 */
#define USB_INTSTS_EPEVT1			((uint32_t)0x00020000)			/*!<USB Event Status on EP1 */
#define USB_INTSTS_EPEVT2			((uint32_t)0x00040000)			/*!<USB Event Status on EP2 */
#define USB_INTSTS_EPEVT3			((uint32_t)0x00080000)			/*!<USB Event Status on EP3 */
#define USB_INTSTS_EPEVT4			((uint32_t)0x00100000)			/*!<USB Event Status on EP4 */
#define USB_INTSTS_EPEVT5			((uint32_t)0x00200000)			/*!<USB Event Status on EP5 */
#define USB_INTSTS_EPEVT6			((uint32_t)0x00400000)			/*!<USB Event Status on EP6 */
#define USB_INTSTS_EPEVT7			((uint32_t)0x00800000)			/*!<USB Event Status on EP7 */
#define USB_INTSTS_SETUP			((uint32_t)0x80000000)			/*!<USB Setup Event Status */

/********************* Bit definition of DADDR register **********************/
#define USB_DADDR_MASK				((uint32_t)0x0000007F)			/*!<USB device¡¦s function address Mask */

/********************* Bit definition of EPSTS register **********************/
#define USB_EPSTS_EPSTS0_INACK		((uint32_t)0x00000100)			/*!<USB Endpoint 0 Bus Status - In Ack */
#define USB_EPSTS_EPSTS0_OUTD0ACK	((uint32_t)0x00000200)			/*!<USB Endpoint 0 Bus Status - Out Data0 Ack */
#define USB_EPSTS_EPSTS0_OUTD1ACK	((uint32_t)0x00000600)			/*!<USB Endpoint 0 Bus Status - Out Data1 Ack */
#define USB_EPSTS_EPSTS0_SETUPACK	((uint32_t)0x00000300)			/*!<USB Endpoint 0 Bus Status - Setup Ack */
#define USB_EPSTS_EPSTS0_ISOEND		((uint32_t)0x00000700)			/*!<USB Endpoint 0 Bus Status - Isochronous transfer end */

#define USB_EPSTS_EPSTS1_INACK		((uint32_t)0x00001000)			/*!<USB Endpoint 1 Bus Status - In Ack */
#define USB_EPSTS_EPSTS1_OUTD0ACK	((uint32_t)0x00002000)			/*!<USB Endpoint 1 Bus Status - Out Data0 Ack */
#define USB_EPSTS_EPSTS1_OUTD1ACK	((uint32_t)0x00006000)			/*!<USB Endpoint 1 Bus Status - Out Data1 Ack */
#define USB_EPSTS_EPSTS1_SETUPACK	((uint32_t)0x00003000)			/*!<USB Endpoint 1 Bus Status - Setup Ack */
#define USB_EPSTS_EPSTS1_ISOEND		((uint32_t)0x00007000)			/*!<USB Endpoint 1 Bus Status - Isochronous transfer end */

#define USB_EPSTS_EPSTS2_INACK		((uint32_t)0x00010000)			/*!<USB Endpoint 2 Bus Status - In Ack */
#define USB_EPSTS_EPSTS2_OUTD0ACK	((uint32_t)0x00020000)			/*!<USB Endpoint 2 Bus Status - Out Data0 Ack */
#define USB_EPSTS_EPSTS2_OUTD1ACK	((uint32_t)0x00060000)			/*!<USB Endpoint 2 Bus Status - Out Data1 Ack */
#define USB_EPSTS_EPSTS2_SETUPACK	((uint32_t)0x00030000)			/*!<USB Endpoint 2 Bus Status - Setup Ack */
#define USB_EPSTS_EPSTS2_ISOEND		((uint32_t)0x00070000)			/*!<USB Endpoint 2 Bus Status - Isochronous transfer end */

#define USB_EPSTS_EPSTS3_INACK		((uint32_t)0x00100000)			/*!<USB Endpoint 3 Bus Status - In Ack */
#define USB_EPSTS_EPSTS3_OUTD0ACK	((uint32_t)0x00200000)			/*!<USB Endpoint 3 Bus Status - Out Data0 Ack */
#define USB_EPSTS_EPSTS3_OUTD1ACK	((uint32_t)0x00600000)			/*!<USB Endpoint 3 Bus Status - Out Data1 Ack */
#define USB_EPSTS_EPSTS3_SETUPACK	((uint32_t)0x00300000)			/*!<USB Endpoint 3 Bus Status - Setup Ack */
#define USB_EPSTS_EPSTS3_ISOEND		((uint32_t)0x00700000)			/*!<USB Endpoint 3 Bus Status - Isochronous transfer end */

#define USB_EPSTS_EPSTS4_INACK		((uint32_t)0x01000000)			/*!<USB Endpoint 4 Bus Status - In Ack */
#define USB_EPSTS_EPSTS4_OUTD0ACK	((uint32_t)0x02000000)			/*!<USB Endpoint 4 Bus Status - Out Data0 Ack */
#define USB_EPSTS_EPSTS4_OUTD1ACK	((uint32_t)0x06000000)			/*!<USB Endpoint 4 Bus Status - Out Data1 Ack */
#define USB_EPSTS_EPSTS4_SETUPACK	((uint32_t)0x03000000)			/*!<USB Endpoint 4 Bus Status - Setup Ack */
#define USB_EPSTS_EPSTS4_ISOEND		((uint32_t)0x07000000)			/*!<USB Endpoint 4 Bus Status - Isochronous transfer end */

#define USB_EPSTS_EPSTS5_INACK		((uint32_t)0x10000000)			/*!<USB Endpoint 5 Bus Status - In Ack */
#define USB_EPSTS_EPSTS5_OUTD0ACK	((uint32_t)0x20000000)			/*!<USB Endpoint 5 Bus Status - Out Data0 Ack */
#define USB_EPSTS_EPSTS5_OUTD1ACK	((uint32_t)0x60000000)			/*!<USB Endpoint 5 Bus Status - Out Data1 Ack */
#define USB_EPSTS_EPSTS5_SETUPACK	((uint32_t)0x30000000)			/*!<USB Endpoint 5 Bus Status - Setup Ack */
#define USB_EPSTS_EPSTS5_ISOEND		((uint32_t)0x70000000)			/*!<USB Endpoint 5 Bus Status - Isochronous transfer end */

/********************* Bit definition of EPSTS2 register **********************/
#define USB_EPSTS_EPSTS6_INACK		((uint32_t)0x00000001)			/*!<USB Endpoint 6 Bus Status - In Ack */
#define USB_EPSTS_EPSTS6_OUTD0ACK	((uint32_t)0x00000002)			/*!<USB Endpoint 6 Bus Status - Out Data0 Ack */
#define USB_EPSTS_EPSTS6_OUTD1ACK	((uint32_t)0x00000006)			/*!<USB Endpoint 6 Bus Status - Out Data1 Ack */
#define USB_EPSTS_EPSTS6_SETUPACK	((uint32_t)0x00000003)			/*!<USB Endpoint 6 Bus Status - Setup Ack */
#define USB_EPSTS_EPSTS6_ISOEND		((uint32_t)0x00000007)			/*!<USB Endpoint 6 Bus Status - Isochronous transfer end */

#define USB_EPSTS_EPSTS7_INACK		((uint32_t)0x00000010)			/*!<USB Endpoint 7 Bus Status - In Ack */
#define USB_EPSTS_EPSTS7_OUTD0ACK	((uint32_t)0x00000020)			/*!<USB Endpoint 7 Bus Status - Out Data0 Ack */
#define USB_EPSTS_EPSTS7_OUTD1ACK	((uint32_t)0x00000060)			/*!<USB Endpoint 7 Bus Status - Out Data1 Ack */
#define USB_EPSTS_EPSTS7_SETUPACK	((uint32_t)0x00000030)			/*!<USB Endpoint 7 Bus Status - Setup Ack */
#define USB_EPSTS_EPSTS7_ISOEND		((uint32_t)0x00000070)			/*!<USB Endpoint 7 Bus Status - Isochronous transfer end */


/********************* Bit definition of CFG register **********************/
#define USB_CFG_EPNUM_MASK			((uint32_t)0x0000000F)			/*!<USB Endpoint Number Mask */
#define USB_CFG_ISOCH				((uint32_t)0x00000010)			/*!<USB Isochronous Endpoint */
#define USB_CFG_EPMODE_DISABLE		((uint32_t)0x00000000)			/*!<USB Endpoint is disabled */
#define USB_CFG_EPMODE_OUT			((uint32_t)0x00000020)			/*!<USB Out endpoint */
#define USB_CFG_EPMODE_IN			((uint32_t)0x00000040)			/*!<USB In endpoint */
#define USB_CFG_DSQ_SYNC_DATA0		((uint32_t)0x00000000)			/*!<USB Data Sequence Synchronization - Data0 */
#define USB_CFG_DSQ_SYNC_DATA1		((uint32_t)0x00000080)			/*!<USB Data Sequence Synchronization - Data1 */
#define USB_CFG_CSTALL				((uint32_t)0x00000100)			/*!<Clear the device to response STALL handshake in setup stage */
#define USB_CFG_SSTALL				((uint32_t)0x00000200)			/*!<Set the device to respond STALL automatically */

/********************* Bit definition of PDMA register **********************/
#define USB_PDMA_MEM2USB			((uint32_t)0x00000000)			/*!<The PDMA will read data from memory to USB buffer */
#define USB_PDMA_USB2MEM			((uint32_t)0x00000001)			/*!<The PDMA will read data from USB buffer to memory */
#define USB_PDMA_PDMA_TRG			((uint32_t)0x00000002)			/*!<Active PDMA Function */
#define USB_PDMA_BYTEM_BYTE			((uint32_t)0x00000004)			/*!<CPU access USB SRAM Size Mode Select - Byte mode */
#define USB_PDMA_BYTEM_WORD			((uint32_t)0x00000000)			/*!<CPU access USB SRAM Size Mode Select - Word mode */
#define USB_PDMA_PDMA_RST			((uint32_t)0x00000008)			/*!<PDMA Reset */


/******************************************************************************/
/*                DAC                                                         */
/******************************************************************************/
/********************* Bit definition of CTL register **********************/
#define DAC_CTL_EN					((uint32_t)0x00000001)			/*!<DAC Enable */
#define DAC_CTL_IE					((uint32_t)0x00000002)			/*!<DAC Interrupt Enable */
#define DAC_CTL_LSEL_DATA_WRITE		((uint32_t)0x00000000)			/*!<DAC Loads on DACx_DAT Write */
#define DAC_CTL_LSEL_PDMA			((uint32_t)0x00000010)			/*!<DAC Loads on PDMa ACK */
#define DAC_CTL_LSEL_TIMER0_CH0		((uint32_t)0x00000020)			/*!<DAC Loads on Timer0 Ch0 Event */
#define DAC_CTL_LSEL_TIMER0_CH1		((uint32_t)0x00000030)			/*!<DAC Loads on Timer0 Ch1 Event */
#define DAC_CTL_LSEL_TIMER1_CH0		((uint32_t)0x00000040)			/*!<DAC Loads on Timer1 Ch0 Event */
#define DAC_CTL_LSEL_TIMER1_CH1		((uint32_t)0x00000050)			/*!<DAC Loads on Timer1 Ch1 Event */
#define DAC_CTL_LSEL_MASK			((uint32_t)0x000000F0)			/*!<DAC Load Select Mask */
#define DAC_CTL_PWONSTBCNT_MASK		((uint32_t)0x003FFF00)			/*!<DAC Power on Stable Count Mask */

/********************* Bit definition of DATA register **********************/
#define DAC_DATA_DATA_MASK			((uint32_t)0x00000FFF)			/*!<DAC Data Mask */

/********************* Bit definition of STS register **********************/
#define DAC_STS_IFG					((uint32_t)0x00000001)			/*!<DAC Interrupt Flag */
#define DAC_STS_STFG				((uint32_t)0x00000002)			/*!<DAC Start Flag */
#define DAC_STS_BUSY  				((uint32_t)0x00000004)			/*!<DAC Loads Busy Bit */

/********************* Bit definition of COMCTL register **********************/
#define DAC_COMCTL_WAITDACCONV_MASK	((uint32_t)0x000000FF)			/*!<Wait DAC Conversion Complete Count */
#define DAC_COMCTL_GRP				((uint32_t)0x00000100)			/*!<Group DAC0 and DAC1 */
#define DAC_COMCTL_REFSEL_AVDD		((uint32_t)0x00000000)			/*!<AVDDReference Voltage */
#define DAC_COMCTL_REFSEL_INT_VREF	((uint32_t)0x00000200)			/*!<Internal VREF as Reference Voltage */
#define DAC_COMCTL_REFSEL_VREF		((uint32_t)0x00000400)			/*!<VREF as Reference Voltage */
#define DAC_COMCTL_REFSEL_MASK		((uint32_t)0x00000600)			/*!<Reference Voltage Selection Mask */



/******************************************************************************/
/*                LCD                                                         */
/******************************************************************************/
/********************* Bit definition of CTL register **********************/
#define LCD_CTL_EN					((uint32_t)0x00000001)			/*!<LCD Enable */
#define LCD_CTL_MUX					((uint32_t)0x0000000E)			/*!<Mux select */
#define LCD_CTL_FREQ 				((uint32_t)0x00000070)			/*!<LCD Frequency Selection */
#define LCD_CTL_BLINK				((uint32_t)0x00000080)			/*!<LCD Blinking Enable */
#define LCD_CTL_PDDISP_EN			((uint32_t)0x00000100)			/*!<Power Down Display Enable */
#define LCD_CTL_PDINT_EN			((uint32_t)0x00000200)			/*!<Power Down Interrupt Enable */
/********************* Bit definition of DISPCTL register **********************/
#define LCD_DISPCTL_CPUMP_EN		((uint32_t)0x00000001)			/*!<Charge Pump Enable */
#define LCD_DISPCTL_BIAS_SEL		((uint32_t)0x00000006)			/*!<Bias Selection */
#define LCD_DISPCTL_IBRL_EN			((uint32_t)0x00000010)			/*!<Internal bias reference ladder enable */
#define LCD_DISPCTL_BV_SEL			((uint32_t)0x00000040)			/*!<Bias Voltage type select */
#define LCD_DISPCTL_CPUMP_VOL_SET	((uint32_t)0x00000700)			/*!<Charge Pump Voltage select */
#define LCD_DISPCTL_CPUMP_FREQ		((uint32_t)0x00003800)			/*!<Charge Pump Frequency select */
/********************* Bit definition of MEM_0 register **********************/
#define LCD_MEM_0_SEG_0				((uint32_t)0x0000003F)			/*!<SEG0 DATA for COM */
#define LCD_MEM_0_SEG_1				((uint32_t)0x00003F00)			/*!<SEG1 DATA for COM */
#define LCD_MEM_0_SEG_2				((uint32_t)0x003F0000)			/*!<SEG2 DATA for COM */
#define LCD_MEM_0_SEG_3				((uint32_t)0x3F000000)			/*!<SEG3 DATA for COM */
/********************* Bit definition of MEM_1 register **********************/
#define LCD_MEM_1_SEG_4				((uint32_t)0x0000003F)			/*!<SEG4 DATA for COM */
#define LCD_MEM_1_SEG_5				((uint32_t)0x00003F00)			/*!<SEG5 DATA for COM */
#define LCD_MEM_1_SEG_6				((uint32_t)0x003F0000)			/*!<SEG6 DATA for COM */
#define LCD_MEM_1_SEG_7				((uint32_t)0x3F000000)			/*!<SEG7 DATA for COM */
/********************* Bit definition of MEM_2 register **********************/
#define LCD_MEM_2_SEG_8				((uint32_t)0x0000003F)			/*!<SEG8 DATA for COM */
#define LCD_MEM_2_SEG_9				((uint32_t)0x00003F00)			/*!<SEG9 DATA for COM */
#define LCD_MEM_2_SEG_10			((uint32_t)0x003F0000)			/*!<SEG10 DATA for COM */
#define LCD_MEM_2_SEG_11			((uint32_t)0x3F000000)			/*!<SEG11 DATA for COM */
/********************* Bit definition of MEM_3 register **********************/
#define LCD_MEM_3_SEG_12			((uint32_t)0x0000003F)			/*!<SEG12 DATA for COM */
#define LCD_MEM_3_SEG_13			((uint32_t)0x00003F00)			/*!<SEG13 DATA for COM */
#define LCD_MEM_3_SEG_14			((uint32_t)0x003F0000)			/*!<SEG14 DATA for COM */
#define LCD_MEM_3_SEG_15			((uint32_t)0x3F000000)			/*!<SEG15 DATA for COM */
/********************* Bit definition of MEM_4 register **********************/
#define LCD_MEM_4_SEG_16			((uint32_t)0x0000003F)			/*!<SEG16 DATA for COM */
#define LCD_MEM_4_SEG_17			((uint32_t)0x00003F00)			/*!<SEG17 DATA for COM */
#define LCD_MEM_4_SEG_18			((uint32_t)0x003F0000)			/*!<SEG18 DATA for COM */
#define LCD_MEM_4_SEG_19			((uint32_t)0x3F000000)			/*!<SEG19 DATA for COM */
/********************* Bit definition of MEM_5 register **********************/
#define LCD_MEM_5_SEG_20			((uint32_t)0x0000003F)			/*!<SEG20 DATA for COM */
#define LCD_MEM_5_SEG_21			((uint32_t)0x00003F00)			/*!<SEG21 DATA for COM */
#define LCD_MEM_5_SEG_22			((uint32_t)0x003F0000)			/*!<SEG22 DATA for COM */
#define LCD_MEM_5_SEG_23			((uint32_t)0x3F000000)			/*!<SEG23 DATA for COM */
/********************* Bit definition of MEM_6 register **********************/
#define LCD_MEM_6_SEG_24			((uint32_t)0x0000003F)			/*!<SEG24 DATA for COM */
#define LCD_MEM_6_SEG_25			((uint32_t)0x00003F00)			/*!<SEG25 DATA for COM */
#define LCD_MEM_6_SEG_26			((uint32_t)0x003F0000)			/*!<SEG26 DATA for COM */
#define LCD_MEM_6_SEG_27			((uint32_t)0x3F000000)			/*!<SEG27 DATA for COM */
/********************* Bit definition of MEM_7 register **********************/
#define LCD_MEM_7_SEG_28			((uint32_t)0x0000003F)			/*!<SEG28 DATA for COM */
#define LCD_MEM_7_SEG_29			((uint32_t)0x00003F00)			/*!<SEG29 DATA for COM */
#define LCD_MEM_7_SEG_30			((uint32_t)0x003F0000)			/*!<SEG30 DATA for COM */
#define LCD_MEM_7_SEG_31			((uint32_t)0x3F000000)			/*!<SEG31 DATA for COM */
/********************* Bit definition of MEM_8 register **********************/
#define LCD_MEM_8_SEG_32			((uint32_t)0x0000003F)			/*!<SEG32 DATA for COM */
#define LCD_MEM_8_SEG_33			((uint32_t)0x00003F00)			/*!<SEG33 DATA for COM */
#define LCD_MEM_8_SEG_34			((uint32_t)0x003F0000)			/*!<SEG34 DATA for COM */
#define LCD_MEM_8_SEG_35			((uint32_t)0x3F000000)			/*!<SEG35 DATA for COM */
/********************* Bit definition of MEM_9 register **********************/
#define LCD_MEM_9_SEG_36			((uint32_t)0x0000003F)			/*!<SEG36 DATA for COM */
#define LCD_MEM_9_SEG_37			((uint32_t)0x00003F00)			/*!<SEG37 DATA for COM */
#define LCD_MEM_9_SEG_38			((uint32_t)0x003F0000)			/*!<SEG38 DATA for COM */
#define LCD_MEM_9_SEG_39			((uint32_t)0x3F000000)			/*!<SEG39 DATA for COM */
/********************* Bit definition of FCR register **********************/
#define LCD_FCR_FCEN				((uint32_t)0x00000001)			/*!<LCD Frame Counter Enable */
#define LCD_FCR_FCINTEN				((uint32_t)0x00000002)			/*!<LCD Frame Counter Interrupt Enable */
#define LCD_FCR_PRESCL				((uint32_t)0x0000000C)			/*!<Frame Counter Pre-scaler Value */
#define LCD_FCR_FCV					((uint32_t)0x000003F0)			/*!<Frame Counter Top Value */
/********************* Bit definition of FCSTS register **********************/
#define LCD_FCSTS_FCSTS				((uint32_t)0x00000001)			/*!<LCD Frame Counter Status */
#define LCD_FCSTS_PDSTS				((uint32_t)0x00000002)			/*!<Power-down Interrupt Status */


/******************************************************************************/
/*                TK                                                          */
/******************************************************************************/
/********************* Bit definition of CTL1 register **********************/
#define TK_CTL1_SEL_TK8_0			((uint32_t)0x00000001)			/*!<TK8/TK0 Scan Select */
#define TK_CTL1_SEL_TK9_1			((uint32_t)0x00000002)			/*!<TK9/TK1 Scan Select */
#define TK_CTL1_SEL_TK10_2			((uint32_t)0x00000004)			/*!<TK10/TK2 Scan Select */
#define TK_CTL1_SEL_TK11_3			((uint32_t)0x00000008)			/*!<TK11/TK3 Scan Select */
#define TK_CTL1_SEL_TK12_4			((uint32_t)0x00000010)			/*!<TK12/TK4 Scan Select */
#define TK_CTL1_SEL_TK13_5			((uint32_t)0x00000020)			/*!<TK13/TK5 Scan Select */
#define TK_CTL1_SEL_TK14_6			((uint32_t)0x00000040)			/*!<TK14/TK6 Scan Select */
#define TK_CTL1_SEL_TK15_7			((uint32_t)0x00000080)			/*!<TK15/TK7 Scan Select */
#define TK_CTL1_EN_TK8_0			((uint32_t)0x00000100)			/*!<TK8/TK0 Scan Enable */
#define TK_CTL1_EN_TK9_1			((uint32_t)0x00000200)			/*!<TK9/TK1 Scan Enable */
#define TK_CTL1_EN_TK10_2			((uint32_t)0x00000400)			/*!<TK10/TK2 Scan Enable */
#define TK_CTL1_EN_TK11_3			((uint32_t)0x00000800)			/*!<TK11/TK3 Scan Enable */
#define TK_CTL1_EN_TK12_4			((uint32_t)0x00001000)			/*!<TK12/TK4 Scan Enable */
#define TK_CTL1_EN_TK13_5			((uint32_t)0x00002000)			/*!<TK13/TK5 Scan Enable */
#define TK_CTL1_EN_TK14_6			((uint32_t)0x00004000)			/*!<TK14/TK6 Scan Enable */
#define TK_CTL1_EN_TK15_7			((uint32_t)0x00008000)			/*!<TK15/TK7 Scan Enable */
#define TK_CTL1_START_BY_SW			((uint32_t)0x00010000)			/*!<Start Scan By Software */
#define TK_CTL1_START_BY_TMR		((uint32_t)0x00020000)			/*!<Start Scan By Timer Enable */
#define TK_CTL1_SEN_SEL_10BIT		((uint32_t)0x00000000)			/*!<Touch Key 10 Bit Sensitivity */
#define TK_CTL1_SEN_SEL_12BIT		((uint32_t)0x20000000)			/*!<Touch Key 12 Bit Sensitivity */
#define TK_CTL1_SEN_SEL_14BIT		((uint32_t)0x40000000)			/*!<Touch Key 14 Bit Sensitivity */
#define TK_CTL1_SEN_SEL_16BIT		((uint32_t)0x60000000)			/*!<Touch Key 16 Bit Sensitivity */
#define TK_CTL1_SEN_SEL_MASK		((uint32_t)0x60000000)			/*!<Touch Key Sensitivity Mask */
#define TK_CTL1_EN					((uint32_t)0x80000000)			/*!<Touch Key Scan Enable Bit */

/********************* Bit definition of CTL2 register **********************/
#define TK_CTL2_TK8_0_CLKDIV_MASK	((uint32_t)0x0000000F)			/*!<TK8/TK0 Time-Base Clock Frequency Select Mask */
#define TK_CTL2_TK8_0_CURCTL_MASK	((uint32_t)0x000000F0)			/*!<TK8/TK0 Charge/Discharge Current Control Mask*/
#define TK_CTL2_TK9_1_CLKDIV_MASK	((uint32_t)0x00000F00)			/*!<TK9/TK1 Time-Base Clock Frequency Select Mask */
#define TK_CTL2_TK9_1_CURCTL_MASK	((uint32_t)0x0000F000)			/*!<TK9/TK1 Charge/Discharge Current Control Mask */
#define TK_CTL2_TK10_2_CLKDIV_MASK	((uint32_t)0x000F0000)			/*!<TK10/TK2 Time-Base Clock Frequency Select Mask */
#define TK_CTL2_TK10_2_CURCTL_MASK	((uint32_t)0x00F00000)			/*!<TK10/TK2 Charge/Discharge Current Control Mask */
#define TK_CTL2_TK11_3_CLKDIV_MASK	((uint32_t)0x0F000000)			/*!<TK11/TK3 Time-Base Clock Frequency Select Mask */
#define TK_CTL2_TK11_3_CURCTL_MASK	((uint32_t)0xF0000000)			/*!<TK11/TK3 Charge/Discharge Current Control Mask */

/********************* Bit definition of CTL3 register **********************/
#define TK_CTL3_TK12_4_CLKDIV_MASK	((uint32_t)0x0000000F)			/*!<TK12/TK4 Time-Base Clock Frequency Select Mask */
#define TK_CTL3_TK12_4_CURCTL_MASK	((uint32_t)0x000000F0)			/*!<TK12/TK4 Charge/Discharge Current Control Mask*/
#define TK_CTL3_TK13_5_CLKDIV_MASK	((uint32_t)0x00000F00)			/*!<TK13/TK5 Time-Base Clock Frequency Select Mask */
#define TK_CTL3_TK13_5_CURCTL_MASK	((uint32_t)0x0000F000)			/*!<TK13/TK5 Charge/Discharge Current Control Mask */
#define TK_CTL3_TK14_6_CLKDIV_MASK	((uint32_t)0x000F0000)			/*!<TK14/TK6 Time-Base Clock Frequency Select Mask */
#define TK_CTL3_TK14_6_CURCTL_MASK	((uint32_t)0x00F00000)			/*!<TK14/TK6 Charge/Discharge Current Control Mask */
#define TK_CTL3_TK15_7_CLKDIV_MASK	((uint32_t)0x0F000000)			/*!<TK15/TK7 Time-Base Clock Frequency Select Mask */
#define TK_CTL3_TK15_7_CURCTL_MASK	((uint32_t)0xF0000000)			/*!<TK15/TK7 Charge/Discharge Current Control Mask */

/********************* Bit definition of STAT register **********************/
#define TK_STAT_BUSY				((uint32_t)0x00000001)			/*!<Touch Key Busy */
#define TK_STAT_COMPLETE			((uint32_t)0x00000002)			/*!<Touch Key Scan Complete */
#define TK_STAT_SEN_MATCH_LEVEL_MASK	((uint32_t)0x000000F0)		/*!<Sensitivity Match Level Mask */
#define TK_STAT_TK8_0_SEN_FAIL		((uint32_t)0x00000100)			/*!<TK8/TK0 Sensing Fail */
#define TK_STAT_TK9_1_SEN_FAIL		((uint32_t)0x00000200)			/*!<TK9/TK1 Sensing Fail */
#define TK_STAT_TK10_2_SEN_FAIL		((uint32_t)0x00000400)			/*!<TK10/TK2 Sensing Fail */
#define TK_STAT_TK11_3_SEN_FAIL		((uint32_t)0x00000800)			/*!<TK11/TK3 Sensing Fail */
#define TK_STAT_TK12_4_SEN_FAIL		((uint32_t)0x00001000)			/*!<TK12/TK4 Sensing Fail */
#define TK_STAT_TK13_5_SEN_FAIL		((uint32_t)0x00002000)			/*!<TK13/TK5 Sensing Fail */
#define TK_STAT_TK14_6_SEN_FAIL		((uint32_t)0x00004000)			/*!<TK14/TK6 Sensing Fail */
#define TK_STAT_TK15_7_SEN_FAIL		((uint32_t)0x00008000)			/*!<TK15/TK7 Sensing Fail */
#define TK_STAT_TK8_0_THC_INT		((uint32_t)0x00010000)			/*!<TK8/TK0 Threshold Control Status */
#define TK_STAT_TK9_1_THC_INT		((uint32_t)0x00020000)			/*!<TK9/TK1 Threshold Control Status */
#define TK_STAT_TK10_2_THC_INT		((uint32_t)0x00040000)			/*!<TK10/TK2 Threshold Control Status */
#define TK_STAT_TK11_3_THC_INT		((uint32_t)0x00080000)			/*!<TK11/TK3 Threshold Control Status */
#define TK_STAT_TK12_4_THC_INT		((uint32_t)0x00100000)			/*!<TK12/TK4 Threshold Control Status */
#define TK_STAT_TK13_5_THC_INT		((uint32_t)0x00200000)			/*!<TK13/TK5 Threshold Control Status */
#define TK_STAT_TK14_6_THC_INT		((uint32_t)0x00400000)			/*!<TK14/TK6 Threshold Control Status */
#define TK_STAT_TK15_7_THC_INT		((uint32_t)0x00800000)			/*!<TK15/TK7 Threshold Control Status */

/********************* Bit definition of DATA1 register **********************/
#define TK_DATA1_TK8_0_DATA			((uint32_t)0x0000FFFF)			/*!<TK8/TK0 Sensing Result Data Mask */
#define TK_DATA1_TK9_1_DATA			((uint32_t)0xFFFF0000)			/*!<TK9/TK1 Sensing Result Data Mask */

/********************* Bit definition of DATA2 register **********************/
#define TK_DATA2_TK10_2_DATA		((uint32_t)0x0000FFFF)			/*!<TK10/TK2 Sensing Result Data Mask */
#define TK_DATA2_TK11_3_DATA		((uint32_t)0xFFFF0000)			/*!<TK11/TK3 Sensing Result Data Mask */

/********************* Bit definition of DATA3 register **********************/
#define TK_DATA3_TK12_4_DATA		((uint32_t)0x0000FFFF)			/*!<TK12/TK4 Sensing Result Data Mask */
#define TK_DATA3_TK13_5_DATA		((uint32_t)0xFFFF0000)			/*!<TK13/TK5 Sensing Result Data Mask */

/********************* Bit definition of DATA4 register **********************/
#define TK_DATA4_TK14_6_DATA		((uint32_t)0x0000FFFF)			/*!<TK14/TK6 Sensing Result Data Mask */
#define TK_DATA4_TK15_7_DATA		((uint32_t)0xFFFF0000)			/*!<TK15/TK7 Sensing Result Data Mask */

/********************* Bit definition of INTEN register **********************/
#define TK_INTEN_SCAN_THC			((uint32_t)0x00000001)			/*!<Touch Key Scan Complete with Threshold Interrupt Enable */
#define TK_INTEN_SCAN_COMPLETE		((uint32_t)0x00000002)			/*!<Touch Key Scan Complete Interrupt Enable */
#define TK_INTEN_SEN_FAIL			((uint32_t)0x00000004)			/*!<Touch Key Sensing Fail Interrupt Enable */

/********************* Bit definition of THC register **********************/
#define TK_THC_LOW_MASK				((uint32_t)0x0000FFFF)			/*!<Low Threshold Control Data */
#define TK_THC_HIGH_MASK			((uint32_t)0xFFFF0000)			/*!<High Threshold Control Data */


/******************************************************************************/
/*                ADC                                                         */
/******************************************************************************/
/********************* Bit definition of RESULT register **********************/
#define ADC_RESULT_RSLT_MASK		((uint32_t)0x00000FFF)			/*!<A/D Conversion Result Mask */
#define ADC_RESULT_VALID			((uint32_t)0x00010000)			/*!<Data Valid Flag */
#define ADC_RESULT_OVERRUN			((uint32_t)0x00020000)			/*!<Over Run Flag */

/********************* Bit definition of CR register **********************/
#define ADC_CR_ADEN					((uint32_t)0x00000001)			/*!<A/D Converter Enable */
#define ADC_CR_ADIE					((uint32_t)0x00000002)			/*!<A/D Interrupt Enable */
#define ADC_CR_ADMD_SINGEL			((uint32_t)0x00000000)			/*!<A/D Converter Single Mode */
#define ADC_CR_ADMD_S_CYCLE			((uint32_t)0x00000008)			/*!<A/D Converter Single Cycle Mode */
#define ADC_CR_ADMD_CONTINUOUS		((uint32_t)0x0000000C)			/*!<A/D Converter Continuous Mode */
#define ADC_CR_ADMD_MASK			((uint32_t)0x0000000C)			/*!<A/D Converter Operation Mode Mask */
#define ADC_CR_TRGCOND_L_LEV		((uint32_t)0x00000000)			/*!<External Low Level Trigger */
#define ADC_CR_TRGCOND_H_LEV		((uint32_t)0x00000040)			/*!<External High Level Trigger */
#define ADC_CR_TRGCOND_F_EDGE		((uint32_t)0x00000080)			/*!<External Falling Edge Trigger */
#define ADC_CR_TRGCOND_R_EDGE		((uint32_t)0x000000C0)			/*!<External Rising Edge Trigger */
#define ADC_CR_TRGCOND_MASK			((uint32_t)0x000000C0)			/*!<External Trigger Condition Mask */
#define ADC_CR_TRGEN				((uint32_t)0x00000100)			/*!<External Trigger Enable */
#define ADC_CR_PTEN					((uint32_t)0x00000200)			/*!<PDMA Transfer Enable */
#define ADC_CR_DIFF					((uint32_t)0x00000400)			/*!<Differential mode select */
#define ADC_CR_SINGLE_END				((uint32_t)0x00000000)			/*!<Single end mode select */
#define ADC_CR_ADST					((uint32_t)0x00000800)			/*!<A/D Conversion Start */
#define ADC_CR_TMSEL_TMR0_CH0		((uint32_t)0x00000000)			/*!<Select TMR0 CH0 as A/D Trigger Source */
#define ADC_CR_TMSEL_TMR0_CH1		((uint32_t)0x00001000)			/*!<Select TMR0 CH1 as A/D Trigger Source */
#define ADC_CR_TMSEL_TMR1_CH0		((uint32_t)0x00002000)			/*!<Select TMR1 CH0 as A/D Trigger Source */
#define ADC_CR_TMSEL_TMR1_CH1		((uint32_t)0x00003000)			/*!<Select TMR1 CH1 as A/D Trigger Source */
#define ADC_CR_TMSEL_MASK			((uint32_t)0x00003000)			/*!<Timer Trigger Source Mask */
#define ADC_CR_TMTRGMOD				((uint32_t)0x00008000)			/*!<Timer Event Trigger ADC Conversion */
#define ADC_CR_REFSEL_AVDD			((uint32_t)0x00000000)			/*!<AVDD as Reference */
#define ADC_CR_REFSEL_INT_REF		((uint32_t)0x00010000)			/*!<INT VREF as Reference */
#define ADC_CR_REFSEL_VREF			((uint32_t)0x00020000)			/*!<VREF as Reference */
#define ADC_CR_REFSEL_MASK			((uint32_t)0x00030000)			/*!<Reference Voltage Source Selection Mask*/
#define ADC_CR_RESSEL_6BIT			((uint32_t)0x00000000)			/*!<ADC 6 Bit Resolution */
#define ADC_CR_RESSEL_8BIT			((uint32_t)0x00040000)			/*!<ADC 8 Bit Resolution */
#define ADC_CR_RESSEL_10BIT			((uint32_t)0x00080000)			/*!<ADC 10 Bit Resolution */
#define ADC_CR_RESSEL_12BIT			((uint32_t)0x000C0000)			/*!<ADC 12 Bit Resolution */
#define ADC_CR_RESSEL_MASK			((uint32_t)0x000C0000)			/*!<Resolution Selection Mask */
#define ADC_CR_TMPDMACNT_MASK		((uint32_t)0xFF000000)			/*!<PDMA Count Mask */

/********************* Bit definition of CHER register **********************/
#define ADC_CHER_CHEN_0				((uint32_t)0x00000001)			/*!<Analog Input Channel 0 Enable (PA.0) */
#define ADC_CHER_CHEN_1				((uint32_t)0x00000002)			/*!<Analog Input Channel 1 Enable (PA.1) */
#define ADC_CHER_CHEN_2				((uint32_t)0x00000004)			/*!<Analog Input Channel 2 Enable (PA.2) */
#define ADC_CHER_CHEN_3				((uint32_t)0x00000008)			/*!<Analog Input Channel 3 Enable (PA.3) */
#define ADC_CHER_CHEN_4				((uint32_t)0x00000010)			/*!<Analog Input Channel 4 Enable (PA.4) */
#define ADC_CHER_CHEN_5				((uint32_t)0x00000020)			/*!<Analog Input Channel 5 Enable (PA.5) */
#define ADC_CHER_CHEN_6				((uint32_t)0x00000040)			/*!<Analog Input Channel 6 Enable (PA.6) */
#define ADC_CHER_CHEN_7				((uint32_t)0x00000080)			/*!<Analog Input Channel 7 Enable (PA.7) */
#define ADC_CHER_CHEN_8				((uint32_t)0x00000100)			/*!<Analog Input Channel 8 Enable (PD.0) */
#define ADC_CHER_CHEN_9				((uint32_t)0x00000200)			/*!<Analog Input Channel 9 Enable (PD.1) */
#define ADC_CHER_CHEN_10			((uint32_t)0x00000400)			/*!<Analog Input Channel 10 Enable (PD.2) */
#define ADC_CHER_CHEN_11			((uint32_t)0x00000800)			/*!<Analog Input Channel 11 Enable (PD.3) */
#define ADC_CHER_CHEN_12			((uint32_t)0x00001000)			/*!<Analog Input Channel 12 Enable (DAC0) */
#define ADC_CHER_CHEN_13			((uint32_t)0x00002000)			/*!<Analog Input Channel 13 Enable (DAC1) */
#define ADC_CHER_CHEN_14			((uint32_t)0x00004000)			/*!<Analog Input Channel 14 Enable (VTEMP) */
#define ADC_CHER_CHEN_15			((uint32_t)0x00008000)			/*!<Analog Input Channel 15 Enable (Int_VREF) */
#define ADC_CHER_CHEN_16			((uint32_t)0x00010000)			/*!<Analog Input Channel 16 Enable (AVDD) */
#define ADC_CHER_CHEN_17			((uint32_t)0x00020000)			/*!<Analog Input Channel 17 Enable (AVSS) */

/********************* Bit definition of CMPR register **********************/
#define ADC_CMPR_CMPEN				((uint32_t)0x00000001)			/*!<Compare Enable */
#define ADC_CMPR_CMPIE				((uint32_t)0x00000002)			/*!<Compare Interrupt Enable */
#define ADC_CMPR_CMPCOND_GE			((uint32_t)0x00000004)			/*!<Compare Condition Greater or Equal To*/
#define ADC_CMPR_CMPCOND_LT			((uint32_t)0x00000000)			/*!<Compare Condition Less Than*/
#define ADC_CMPR_CMPCH_MASK			((uint32_t)0x000000F8)			/*!<Compare Channel Selection Mask */
#define ADC_CMPR_CMPMATCNT_MASK		((uint32_t)0x00000F00)			/*!<Compare Match Count Mask */
#define ADC_CMPR_CMPD_DATA_MASK			((uint32_t)0x0FFF0000)			/*!<Comparison Data Mask */

/********************* Bit definition of SR register **********************/
#define ADC_SR_ADF					((uint32_t)0x00000001)			/*!<A/D Conversion End Flag */
#define ADC_SR_CMPF0				((uint32_t)0x00000002)			/*!<Compare Complete Flag 0 */
#define ADC_SR_CMPF1				((uint32_t)0x00000004)			/*!<Compare Complete Flag 1 */
#define ADC_SR_BUSY					((uint32_t)0x00000008)			/*!<Busy/Idle State */
#define ADC_SR_CHANNEL_MASK			((uint32_t)0x000001F0)			/*!<Current Conversion Channel Mask */
#define ADC_SR_INITRDY				((uint32_t)0x00010000)			/*!<ADC Power Up Sequence Is Complete */

/********************* Bit definition of PDMA register **********************/
#define ADC_SR_PDMA_DATA_MASK	((uint32_t)0x00000FFF)				/*!<ADC PDMA Current Transfer Data Register Mask */

/********************* Bit definition of PWRCTL register **********************/
#define ADC_PWRCTL_PUPRDY			((uint32_t)0x00000001)			/*!<ADC Power Up Sequence Is Completed and Ready for Conversion */
#define ADC_PWRCTL_PWDCALEN			((uint32_t)0x00000002)			/*!<Power Up Calibration Function Enable */
#define ADC_PWRCTL_PWDCALDIS			((uint32_t)0x00000000)			/*!<Power Up Calibration Function Disable */
#define ADC_PWRCTL_PWDMOD_PD		((uint32_t)0x00000000)			/*!<Power Down Mode */
#define ADC_PWRCTL_PWDMOD_STBY		((uint32_t)0x00000008)			/*!<Stand By Mode */

/********************* Bit definition of CALEN register **********************/
#define ADC_CALCTL_CALEN			((uint32_t)0x00000001)			/*!<Enable Calibraion Function  */
#define ADC_CALCTL_CALSTART			((uint32_t)0x00000002)			/*!<Start Calibration Functional Blcok */
#define ADC_CALCTL_CALDONE			((uint32_t)0x00000004)			/*!<Calibrate Functional Block Complete */
#define ADC_CALCTL_CALSEL			((uint32_t)0x00000008)			/*!<Select Calibration Functional Block */
#define ADC_CALCTL_LOAD				((uint32_t)0x00000000)			/*!<Select Laod Calibration Word */

/********************* Bit definition of CALWORD register **********************/
#define ADC_CALWORD_WORD_MASK		((uint32_t)0x0000003F)			/*!<Calibration Word Mask */

/********************* Bit definition of SMPLCNT0 register **********************/
#define ADC_SMPLCNT_0CLK		((uint32_t)0x00000000)			/*!<Sampling Count 0 Clock */
#define ADC_SMPLCNT_1CLK		((uint32_t)0x00000001)			/*!<Sampling Count 1 Clock */
#define ADC_SMPLCNT_2CLK		((uint32_t)0x00000002)			/*!<Sampling Count 2 Clock */
#define ADC_SMPLCNT_4CLK		((uint32_t)0x00000003)			/*!<Sampling Count 4 Clock */
#define ADC_SMPLCNT_8CLK		((uint32_t)0x00000004)			/*!<Sampling Count 8 Clock */
#define ADC_SMPLCNT_16CLK		((uint32_t)0x00000005)			/*!<Sampling Count 16 Clock */
#define ADC_SMPLCNT_32CLK		((uint32_t)0x00000006)			/*!<Sampling Count 32 Clock */
#define ADC_SMPLCNT_64CLK		((uint32_t)0x00000007)			/*!<Sampling Count 64 Clock */
#define ADC_SMPLCNT_128CLK		((uint32_t)0x00000008)			/*!<Sampling Count 128 Clock */
#define ADC_SMPLCNT_256CLK		((uint32_t)0x00000009)			/*!<Sampling Count 256 Clock */
#define ADC_SMPLCNT_512CLK		((uint32_t)0x0000000A)			/*!<Sampling Count 512 Clock */
#define ADC_SMPLCNT_1024CLK		((uint32_t)0x0000000B)			/*!<Sampling Count 1024 Clock */
#define ADC_SMPLCNT_Mask		((uint32_t)0x0000000F)			/*!<Sampling Count Mask */
#define ADC_SMPLCNT0_CH0_0CLK		((uint32_t)0x00000000)			/*!<Channel 0 Sampling Count 0 Clock */
#define ADC_SMPLCNT0_CH0_1CLK		((uint32_t)0x00000001)			/*!<Channel 0 Sampling Count 1 Clock */
#define ADC_SMPLCNT0_CH0_2CLK		((uint32_t)0x00000002)			/*!<Channel 0 Sampling Count 2 Clock */
#define ADC_SMPLCNT0_CH0_4CLK		((uint32_t)0x00000003)			/*!<Channel 0 Sampling Count 4 Clock */
#define ADC_SMPLCNT0_CH0_8CLK		((uint32_t)0x00000004)			/*!<Channel 0 Sampling Count 8 Clock */
#define ADC_SMPLCNT0_CH0_16CLK		((uint32_t)0x00000005)			/*!<Channel 0 Sampling Count 16 Clock */
#define ADC_SMPLCNT0_CH0_32CLK		((uint32_t)0x00000006)			/*!<Channel 0 Sampling Count 32 Clock */
#define ADC_SMPLCNT0_CH0_64CLK		((uint32_t)0x00000007)			/*!<Channel 0 Sampling Count 64 Clock */
#define ADC_SMPLCNT0_CH0_128CLK		((uint32_t)0x00000008)			/*!<Channel 0 Sampling Count 128 Clock */
#define ADC_SMPLCNT0_CH0_256CLK		((uint32_t)0x00000009)			/*!<Channel 0 Sampling Count 256 Clock */
#define ADC_SMPLCNT0_CH0_512CLK		((uint32_t)0x0000000A)			/*!<Channel 0 Sampling Count 512 Clock */
#define ADC_SMPLCNT0_CH0_1024CLK	((uint32_t)0x0000000B)			/*!<Channel 0 Sampling Count 1024 Clock */
#define ADC_SMPLCNT0_CH1_0CLK		((uint32_t)0x00000000)			/*!<Channel 1 Sampling Count 0 Clock */
#define ADC_SMPLCNT0_CH1_1CLK		((uint32_t)0x00000010)			/*!<Channel 1 Sampling Count 1 Clock */
#define ADC_SMPLCNT0_CH1_2CLK		((uint32_t)0x00000020)			/*!<Channel 1 Sampling Count 2 Clock */
#define ADC_SMPLCNT0_CH1_4CLK		((uint32_t)0x00000030)			/*!<Channel 1 Sampling Count 4 Clock */
#define ADC_SMPLCNT0_CH1_8CLK		((uint32_t)0x00000040)			/*!<Channel 1 Sampling Count 8 Clock */
#define ADC_SMPLCNT0_CH1_16CLK		((uint32_t)0x00000050)			/*!<Channel 1 Sampling Count 16 Clock */
#define ADC_SMPLCNT0_CH1_32CLK		((uint32_t)0x00000060)			/*!<Channel 1 Sampling Count 32 Clock */
#define ADC_SMPLCNT0_CH1_64CLK		((uint32_t)0x00000070)			/*!<Channel 1 Sampling Count 64 Clock */
#define ADC_SMPLCNT0_CH1_128CLK		((uint32_t)0x00000080)			/*!<Channel 1 Sampling Count 128 Clock */
#define ADC_SMPLCNT0_CH1_256CLK		((uint32_t)0x00000090)			/*!<Channel 1 Sampling Count 256 Clock */
#define ADC_SMPLCNT0_CH1_512CLK		((uint32_t)0x000000A0)			/*!<Channel 1 Sampling Count 512 Clock */
#define ADC_SMPLCNT0_CH1_1024CLK	((uint32_t)0x000000B0)			/*!<Channel 1 Sampling Count 1024 Clock */
#define ADC_SMPLCNT0_CH2_0CLK		((uint32_t)0x00000000)			/*!<Channel 2 Sampling Count 0 Clock */
#define ADC_SMPLCNT0_CH2_1CLK		((uint32_t)0x00000100)			/*!<Channel 2 Sampling Count 1 Clock */
#define ADC_SMPLCNT0_CH2_2CLK		((uint32_t)0x00000200)			/*!<Channel 2 Sampling Count 2 Clock */
#define ADC_SMPLCNT0_CH2_4CLK		((uint32_t)0x00000300)			/*!<Channel 2 Sampling Count 4 Clock */
#define ADC_SMPLCNT0_CH2_8CLK		((uint32_t)0x00000400)			/*!<Channel 2 Sampling Count 8 Clock */
#define ADC_SMPLCNT0_CH2_16CLK		((uint32_t)0x00000500)			/*!<Channel 2 Sampling Count 16 Clock */
#define ADC_SMPLCNT0_CH2_32CLK		((uint32_t)0x00000600)			/*!<Channel 2 Sampling Count 32 Clock */
#define ADC_SMPLCNT0_CH2_64CLK		((uint32_t)0x00000700)			/*!<Channel 2 Sampling Count 64 Clock */
#define ADC_SMPLCNT0_CH2_128CLK		((uint32_t)0x00000800)			/*!<Channel 2 Sampling Count 128 Clock */
#define ADC_SMPLCNT0_CH2_256CLK		((uint32_t)0x00000900)			/*!<Channel 2 Sampling Count 256 Clock */
#define ADC_SMPLCNT0_CH2_512CLK		((uint32_t)0x00000A00)			/*!<Channel 2 Sampling Count 512 Clock */
#define ADC_SMPLCNT0_CH2_1024CLK	((uint32_t)0x00000B00)			/*!<Channel 2 Sampling Count 1024 Clock */
#define ADC_SMPLCNT0_CH3_0CLK		((uint32_t)0x00000000)			/*!<Channel 3 Sampling Count 0 Clock */
#define ADC_SMPLCNT0_CH3_1CLK		((uint32_t)0x00001000)			/*!<Channel 3 Sampling Count 1 Clock */
#define ADC_SMPLCNT0_CH3_2CLK		((uint32_t)0x00002000)			/*!<Channel 3 Sampling Count 2 Clock */
#define ADC_SMPLCNT0_CH3_4CLK		((uint32_t)0x00003000)			/*!<Channel 3 Sampling Count 4 Clock */
#define ADC_SMPLCNT0_CH3_8CLK		((uint32_t)0x00004000)			/*!<Channel 3 Sampling Count 8 Clock */
#define ADC_SMPLCNT0_CH3_16CLK		((uint32_t)0x00005000)			/*!<Channel 3 Sampling Count 16 Clock */
#define ADC_SMPLCNT0_CH3_32CLK		((uint32_t)0x00006000)			/*!<Channel 3 Sampling Count 32 Clock */
#define ADC_SMPLCNT0_CH3_64CLK		((uint32_t)0x00007000)			/*!<Channel 3 Sampling Count 64 Clock */
#define ADC_SMPLCNT0_CH3_128CLK		((uint32_t)0x00008000)			/*!<Channel 3 Sampling Count 128 Clock */
#define ADC_SMPLCNT0_CH3_256CLK		((uint32_t)0x00009000)			/*!<Channel 3 Sampling Count 256 Clock */
#define ADC_SMPLCNT0_CH3_512CLK		((uint32_t)0x0000A000)			/*!<Channel 3 Sampling Count 512 Clock */
#define ADC_SMPLCNT0_CH3_1024CLK	((uint32_t)0x0000B000)			/*!<Channel 3 Sampling Count 1024 Clock */
#define ADC_SMPLCNT0_CH4_0CLK		((uint32_t)0x00000000)			/*!<Channel 4 Sampling Count 0 Clock */
#define ADC_SMPLCNT0_CH4_1CLK		((uint32_t)0x00010000)			/*!<Channel 4 Sampling Count 1 Clock */
#define ADC_SMPLCNT0_CH4_2CLK		((uint32_t)0x00020000)			/*!<Channel 4 Sampling Count 2 Clock */
#define ADC_SMPLCNT0_CH4_4CLK		((uint32_t)0x00030000)			/*!<Channel 4 Sampling Count 4 Clock */
#define ADC_SMPLCNT0_CH4_8CLK		((uint32_t)0x00040000)			/*!<Channel 4 Sampling Count 8 Clock */
#define ADC_SMPLCNT0_CH4_16CLK		((uint32_t)0x00050000)			/*!<Channel 4 Sampling Count 16 Clock */
#define ADC_SMPLCNT0_CH4_32CLK		((uint32_t)0x00060000)			/*!<Channel 4 Sampling Count 32 Clock */
#define ADC_SMPLCNT0_CH4_64CLK		((uint32_t)0x00070000)			/*!<Channel 4 Sampling Count 64 Clock */
#define ADC_SMPLCNT0_CH4_128CLK		((uint32_t)0x00080000)			/*!<Channel 4 Sampling Count 128 Clock */
#define ADC_SMPLCNT0_CH4_256CLK		((uint32_t)0x00090000)			/*!<Channel 4 Sampling Count 256 Clock */
#define ADC_SMPLCNT0_CH4_512CLK		((uint32_t)0x000A0000)			/*!<Channel 4 Sampling Count 512 Clock */
#define ADC_SMPLCNT0_CH4_1024CLK	((uint32_t)0x000B0000)			/*!<Channel 4 Sampling Count 1024 Clock */
#define ADC_SMPLCNT0_CH5_0CLK		((uint32_t)0x00000000)			/*!<Channel 5 Sampling Count 0 Clock */
#define ADC_SMPLCNT0_CH5_1CLK		((uint32_t)0x00100000)			/*!<Channel 5 Sampling Count 1 Clock */
#define ADC_SMPLCNT0_CH5_2CLK		((uint32_t)0x00200000)			/*!<Channel 5 Sampling Count 2 Clock */
#define ADC_SMPLCNT0_CH5_4CLK		((uint32_t)0x00300000)			/*!<Channel 5 Sampling Count 4 Clock */
#define ADC_SMPLCNT0_CH5_8CLK		((uint32_t)0x00400000)			/*!<Channel 5 Sampling Count 8 Clock */
#define ADC_SMPLCNT0_CH5_16CLK		((uint32_t)0x00500000)			/*!<Channel 5 Sampling Count 16 Clock */
#define ADC_SMPLCNT0_CH5_32CLK		((uint32_t)0x00600000)			/*!<Channel 5 Sampling Count 32 Clock */
#define ADC_SMPLCNT0_CH5_64CLK		((uint32_t)0x00700000)			/*!<Channel 5 Sampling Count 64 Clock */
#define ADC_SMPLCNT0_CH5_128CLK		((uint32_t)0x00800000)			/*!<Channel 5 Sampling Count 128 Clock */
#define ADC_SMPLCNT0_CH5_256CLK		((uint32_t)0x00900000)			/*!<Channel 5 Sampling Count 256 Clock */
#define ADC_SMPLCNT0_CH5_512CLK		((uint32_t)0x00A00000)			/*!<Channel 5 Sampling Count 512 Clock */
#define ADC_SMPLCNT0_CH5_1024CLK	((uint32_t)0x00B00000)			/*!<Channel 5 Sampling Count 1024 Clock */
#define ADC_SMPLCNT0_CH6_0CLK		((uint32_t)0x00000000)			/*!<Channel 6 Sampling Count 0 Clock */
#define ADC_SMPLCNT0_CH6_1CLK		((uint32_t)0x01000000)			/*!<Channel 6 Sampling Count 1 Clock */
#define ADC_SMPLCNT0_CH6_2CLK		((uint32_t)0x02000000)			/*!<Channel 6 Sampling Count 2 Clock */
#define ADC_SMPLCNT0_CH6_4CLK		((uint32_t)0x03000000)			/*!<Channel 6 Sampling Count 4 Clock */
#define ADC_SMPLCNT0_CH6_8CLK		((uint32_t)0x04000000)			/*!<Channel 6 Sampling Count 8 Clock */
#define ADC_SMPLCNT0_CH6_16CLK		((uint32_t)0x05000000)			/*!<Channel 6 Sampling Count 16 Clock */
#define ADC_SMPLCNT0_CH6_32CLK		((uint32_t)0x06000000)			/*!<Channel 6 Sampling Count 32 Clock */
#define ADC_SMPLCNT0_CH6_64CLK		((uint32_t)0x07000000)			/*!<Channel 6 Sampling Count 64 Clock */
#define ADC_SMPLCNT0_CH6_128CLK		((uint32_t)0x08000000)			/*!<Channel 6 Sampling Count 128 Clock */
#define ADC_SMPLCNT0_CH6_256CLK		((uint32_t)0x09000000)			/*!<Channel 6 Sampling Count 256 Clock */
#define ADC_SMPLCNT0_CH6_512CLK		((uint32_t)0x0A000000)			/*!<Channel 6 Sampling Count 512 Clock */
#define ADC_SMPLCNT0_CH6_1024CLK	((uint32_t)0x0B000000)			/*!<Channel 6 Sampling Count 1024 Clock */
#define ADC_SMPLCNT0_CH7_0CLK		((uint32_t)0x00000000)			/*!<Channel 7 Sampling Count 0 Clock */
#define ADC_SMPLCNT0_CH7_1CLK		((uint32_t)0x10000000)			/*!<Channel 7 Sampling Count 1 Clock */
#define ADC_SMPLCNT0_CH7_2CLK		((uint32_t)0x20000000)			/*!<Channel 7 Sampling Count 2 Clock */
#define ADC_SMPLCNT0_CH7_4CLK		((uint32_t)0x30000000)			/*!<Channel 7 Sampling Count 4 Clock */
#define ADC_SMPLCNT0_CH7_8CLK		((uint32_t)0x40000000)			/*!<Channel 7 Sampling Count 8 Clock */
#define ADC_SMPLCNT0_CH7_16CLK		((uint32_t)0x50000000)			/*!<Channel 7 Sampling Count 16 Clock */
#define ADC_SMPLCNT0_CH7_32CLK		((uint32_t)0x60000000)			/*!<Channel 7 Sampling Count 32 Clock */
#define ADC_SMPLCNT0_CH7_64CLK		((uint32_t)0x70000000)			/*!<Channel 7 Sampling Count 64 Clock */
#define ADC_SMPLCNT0_CH7_128CLK		((uint32_t)0x80000000)			/*!<Channel 7 Sampling Count 128 Clock */
#define ADC_SMPLCNT0_CH7_256CLK		((uint32_t)0x90000000)			/*!<Channel 7 Sampling Count 256 Clock */
#define ADC_SMPLCNT0_CH7_512CLK		((uint32_t)0xA0000000)			/*!<Channel 7 Sampling Count 512 Clock */
#define ADC_SMPLCNT0_CH7_1024CLK	((uint32_t)0xB0000000)			/*!<Channel 7 Sampling Count 1024 Clock */

/********************* Bit definition of SMPLCNT1 register **********************/
#define ADC_SMPLCNT1_CH8_0CLK		((uint32_t)0x00000000)			/*!<Channel 8 Sampling Count 0 Clock */
#define ADC_SMPLCNT1_CH8_1CLK		((uint32_t)0x00000001)			/*!<Channel 8 Sampling Count 1 Clock */
#define ADC_SMPLCNT1_CH8_2CLK		((uint32_t)0x00000002)			/*!<Channel 8 Sampling Count 2 Clock */
#define ADC_SMPLCNT1_CH8_4CLK		((uint32_t)0x00000003)			/*!<Channel 8 Sampling Count 4 Clock */
#define ADC_SMPLCNT1_CH8_8CLK		((uint32_t)0x00000004)			/*!<Channel 8 Sampling Count 8 Clock */
#define ADC_SMPLCNT1_CH8_16CLK		((uint32_t)0x00000005)			/*!<Channel 8 Sampling Count 16 Clock */
#define ADC_SMPLCNT1_CH8_32CLK		((uint32_t)0x00000006)			/*!<Channel 8 Sampling Count 32 Clock */
#define ADC_SMPLCNT1_CH8_64CLK		((uint32_t)0x00000007)			/*!<Channel 8 Sampling Count 64 Clock */
#define ADC_SMPLCNT1_CH8_128CLK		((uint32_t)0x00000008)			/*!<Channel 8 Sampling Count 128 Clock */
#define ADC_SMPLCNT1_CH8_256CLK		((uint32_t)0x00000009)			/*!<Channel 8 Sampling Count 256 Clock */
#define ADC_SMPLCNT1_CH8_512CLK		((uint32_t)0x0000000A)			/*!<Channel 8 Sampling Count 512 Clock */
#define ADC_SMPLCNT1_CH8_1024CLK	((uint32_t)0x0000000B)			/*!<Channel 8 Sampling Count 1024 Clock */
#define ADC_SMPLCNT1_CH9_0CLK		((uint32_t)0x00000000)			/*!<Channel 9 Sampling Count 0 Clock */
#define ADC_SMPLCNT1_CH9_1CLK		((uint32_t)0x00000010)			/*!<Channel 9 Sampling Count 1 Clock */
#define ADC_SMPLCNT1_CH9_2CLK		((uint32_t)0x00000020)			/*!<Channel 9 Sampling Count 2 Clock */
#define ADC_SMPLCNT1_CH9_4CLK		((uint32_t)0x00000030)			/*!<Channel 9 Sampling Count 4 Clock */
#define ADC_SMPLCNT1_CH9_8CLK		((uint32_t)0x00000040)			/*!<Channel 9 Sampling Count 8 Clock */
#define ADC_SMPLCNT1_CH9_16CLK		((uint32_t)0x00000050)			/*!<Channel 9 Sampling Count 16 Clock */
#define ADC_SMPLCNT1_CH9_32CLK		((uint32_t)0x00000060)			/*!<Channel 9 Sampling Count 32 Clock */
#define ADC_SMPLCNT1_CH9_64CLK		((uint32_t)0x00000070)			/*!<Channel 9 Sampling Count 64 Clock */
#define ADC_SMPLCNT1_CH9_128CLK		((uint32_t)0x00000080)			/*!<Channel 9 Sampling Count 128 Clock */
#define ADC_SMPLCNT1_CH9_256CLK		((uint32_t)0x00000090)			/*!<Channel 9 Sampling Count 256 Clock */
#define ADC_SMPLCNT1_CH9_512CLK		((uint32_t)0x000000A0)			/*!<Channel 9 Sampling Count 512 Clock */
#define ADC_SMPLCNT1_CH9_1024CLK	((uint32_t)0x000000B0)			/*!<Channel 9 Sampling Count 1024 Clock */
#define ADC_SMPLCNT1_CH10_0CLK		((uint32_t)0x00000000)			/*!<Channel 10 Sampling Count 0 Clock */
#define ADC_SMPLCNT1_CH10_1CLK		((uint32_t)0x00000100)			/*!<Channel 10 Sampling Count 1 Clock */
#define ADC_SMPLCNT1_CH10_2CLK		((uint32_t)0x00000200)			/*!<Channel 10 Sampling Count 2 Clock */
#define ADC_SMPLCNT1_CH10_4CLK		((uint32_t)0x00000300)			/*!<Channel 10 Sampling Count 4 Clock */
#define ADC_SMPLCNT1_CH10_8CLK		((uint32_t)0x00000400)			/*!<Channel 10 Sampling Count 8 Clock */
#define ADC_SMPLCNT1_CH10_16CLK		((uint32_t)0x00000500)			/*!<Channel 10 Sampling Count 16 Clock */
#define ADC_SMPLCNT1_CH10_32CLK		((uint32_t)0x00000600)			/*!<Channel 10 Sampling Count 32 Clock */
#define ADC_SMPLCNT1_CH10_64CLK		((uint32_t)0x00000700)			/*!<Channel 10 Sampling Count 64 Clock */
#define ADC_SMPLCNT1_CH10_128CLK	((uint32_t)0x00000800)			/*!<Channel 10 Sampling Count 128 Clock */
#define ADC_SMPLCNT1_CH10_256CLK	((uint32_t)0x00000900)			/*!<Channel 10 Sampling Count 256 Clock */
#define ADC_SMPLCNT1_CH10_512CLK	((uint32_t)0x00000A00)			/*!<Channel 10 Sampling Count 512 Clock */
#define ADC_SMPLCNT1_CH10_1024CLK	((uint32_t)0x00000B00)			/*!<Channel 10 Sampling Count 1024 Clock */
#define ADC_SMPLCNT1_CH11_0CLK		((uint32_t)0x00000000)			/*!<Channel 11 Sampling Count 0 Clock */
#define ADC_SMPLCNT1_CH11_1CLK		((uint32_t)0x00001000)			/*!<Channel 11 Sampling Count 1 Clock */
#define ADC_SMPLCNT1_CH11_2CLK		((uint32_t)0x00002000)			/*!<Channel 11 Sampling Count 2 Clock */
#define ADC_SMPLCNT1_CH11_4CLK		((uint32_t)0x00003000)			/*!<Channel 11 Sampling Count 4 Clock */
#define ADC_SMPLCNT1_CH11_8CLK		((uint32_t)0x00004000)			/*!<Channel 11 Sampling Count 8 Clock */
#define ADC_SMPLCNT1_CH11_16CLK		((uint32_t)0x00005000)			/*!<Channel 11 Sampling Count 16 Clock */
#define ADC_SMPLCNT1_CH11_32CLK		((uint32_t)0x00006000)			/*!<Channel 11 Sampling Count 32 Clock */
#define ADC_SMPLCNT1_CH11_64CLK		((uint32_t)0x00007000)			/*!<Channel 11 Sampling Count 64 Clock */
#define ADC_SMPLCNT1_CH11_128CLK	((uint32_t)0x00008000)			/*!<Channel 11 Sampling Count 128 Clock */
#define ADC_SMPLCNT1_CH11_256CLK	((uint32_t)0x00009000)			/*!<Channel 11 Sampling Count 256 Clock */
#define ADC_SMPLCNT1_CH11_512CLK	((uint32_t)0x0000A000)			/*!<Channel 11 Sampling Count 512 Clock */
#define ADC_SMPLCNT1_CH11_1024CLK	((uint32_t)0x0000B000)			/*!<Channel 11 Sampling Count 1024 Clock */
#define ADC_SMPLCNT1_CHINT_0CLK		((uint32_t)0x00000000)			/*!<Internal Channels Sampling Count 0 Clock */
#define ADC_SMPLCNT1_CHINT_1CLK		((uint32_t)0x00010000)			/*!<Internal Channels Sampling Count 1 Clock */
#define ADC_SMPLCNT1_CHINT_2CLK		((uint32_t)0x00020000)			/*!<Internal Channels Sampling Count 2 Clock */
#define ADC_SMPLCNT1_CHINT_4CLK		((uint32_t)0x00030000)			/*!<Internal Channels Sampling Count 4 Clock */
#define ADC_SMPLCNT1_CHINT_8CLK		((uint32_t)0x00040000)			/*!<Internal Channels Sampling Count 8 Clock */
#define ADC_SMPLCNT1_CHINT_16CLK	((uint32_t)0x00050000)			/*!<Internal Channels Sampling Count 16 Clock */
#define ADC_SMPLCNT1_CHINT_32CLK	((uint32_t)0x00060000)			/*!<Internal Channels Sampling Count 32 Clock */
#define ADC_SMPLCNT1_CHINT_64CLK	((uint32_t)0x00070000)			/*!<Internal Channels Sampling Count 64 Clock */
#define ADC_SMPLCNT1_CHINT_128CLK	((uint32_t)0x00080000)			/*!<Internal Channels Sampling Count 128 Clock */
#define ADC_SMPLCNT1_CHINT_256CLK	((uint32_t)0x00090000)			/*!<Internal Channels Sampling Count 256 Clock */
#define ADC_SMPLCNT1_CHINT_512CLK	((uint32_t)0x000A0000)			/*!<Internal Channels Sampling Count 512 Clock */
#define ADC_SMPLCNT1_CHINT_1024CLK	((uint32_t)0x000B0000)			/*!<Internal Channels Sampling Count 1024 Clock */


/******************************************************************************/
/*                SC                                                          */
/******************************************************************************/
/********************* Bit definition of RBR register **********************/
#define SC_RBR_RBR_MASK				((uint32_t)0x000000FF)			/*!<Receive Buffer Register */
/********************* Bit definition of THR register **********************/
#define SC_THR_THR_MASK				((uint32_t)0x000000FF)			/*!<Transmit Holding Register */

/********************* Bit definition of CTL register **********************/
#define SC_CTL_SC_CEN				((uint32_t)0x00000001)			/*!<SC Engine Enable */
#define SC_CTL_DIS_RX				((uint32_t)0x00000002)			/*!<Disable RX transition */
#define SC_CTL_DIS_TX				((uint32_t)0x00000004)			/*!<Disable TX transition */
#define SC_CTL_AUTO_CON_EN			((uint32_t)0x00000008)			/*!<Auto Convention Enable */
#define SC_CTL_CON_SEL_DIRECT_CON	((uint32_t)0x00000000)			/*!<Convention Select */
#define SC_CTL_CON_SEL_INVERSE_CON	((uint32_t)0x00000030)			/*!<Convention Select */
#define SC_CTL_CON_SEL_MASK			((uint32_t)0x00000030)			/*!<Convention Select mask*/
#define SC_CTL_RX_FTRI_LEV_1BYTE	((uint32_t)0x00000000)			/*!<RX Buffer Trigger Level 1 byte*/
#define SC_CTL_RX_FTRI_LEV_2BYTES	((uint32_t)0x00000040)			/*!<RX Buffer Trigger Level 2 bytes*/
#define SC_CTL_RX_FTRI_LEV_3BYTES	((uint32_t)0x00000080)			/*!<RX Buffer Trigger Level 3 bytes*/
#define SC_CTL_RX_FTRI_LEV_MASK		((uint32_t)0x000000C0)			/*!<RX Buffer Trigger Level Mask*/
#define SC_CTL_BGT_MASK				((uint32_t)0x00001F00)			/*!<Block Guard Time Mask*/
#define SC_CTL_TMR_SEL_DISABLE		((uint32_t)0x00000000)			/*!<Timer Selection. Disable all internal timer function */
#define SC_CTL_TMR_SEL_24_BITS		((uint32_t)0x00002000)			/*!<Timer Selection. Enable internal 24 bits timer */
#define SC_CTL_TMR_SEL_24_8_BITS	((uint32_t)0x00004000)			/*!<Timer Selection. Enable internal 24 bits timer and 8 bits internal timer */
#define SC_CTL_TMR_SEL_24_8_8_BITS	((uint32_t)0x00006000)			/*!<Timer Selection. Enable internal 24 bits timer and two 8 bits timers */
#define SC_CTL_TMR_SEL_MASK			((uint32_t)0x00006000)			/*!<Timer Selection Mask */
#define SC_CTL_SLEN					((uint32_t)0x00008000)			/*!<Stop Bit Length */
#define SC_CTL_RX_ERETRY_MASK		((uint32_t)0x00070000)			/*!<RX Error Retry Register */
#define SC_CTL_RX_ERETRY_EN			((uint32_t)0x00080000)			/*!<RX Error Retry Enable Register */
#define SC_CTL_TX_ERETRY_MASK		((uint32_t)0x00700000)			/*!<TX Error Retry Register */
#define SC_CTL_TX_ERETRY_EN			((uint32_t)0x00800000)			/*!<TX Error Retry Enable Register */
#define SC_CTL_CD_DEB_SEL_IN_384_REMOVAL_128 ((uint32_t)0x00000000)			/*!<Card Detect De-Bounce Select Register. Card insert once per 384 engine clocks and de-bounce sample card removal once per 128 engine clocks.*/
#define SC_CTL_CD_DEB_SEL_IN_192_REMOVAL_64	 ((uint32_t)0x01000000)			/*!<Card Detect De-Bounce Select Register. Card insert once per 192 engine clocks and de-bounce sample card removal once per 64 engine clocks. */
#define SC_CTL_CD_DEB_SEL_IN_96_REMOVAL_32	 ((uint32_t)0x02000000)			/*!<Card Detect De-Bounce Select Register. Card insert once per 96 engine clocks and de-bounce sample card removal once per 32 engine clocks. */
#define SC_CTL_CD_DEB_SEL_IN_48_REMOVAL_16	 ((uint32_t)0x03000000)			/*!<Card Detect De-Bounce Select Register. Card insert once per 48 engine clocks and de-bounce sample card removal once per 16 engine clocks. */
#define SC_CTL_CD_DEB_SEL_MASK				 ((uint32_t)0x03000000)			/*!<Card Detect De-Bounce Select Register Mask */

/********************* Bit definition of ALTCTL register **********************/
#define SC_ALTCTL_TX_RST			((uint32_t)0x00000001)			/*!<TX Software Reset */
#define SC_ALTCTL_RX_RST			((uint32_t)0x00000002)			/*!<RX Software Reset */
#define SC_ALTCTL_DACT_EN			((uint32_t)0x00000004)			/*!<Deactivation Sequence Generator Enable */
#define SC_ALTCTL_ACT_EN			((uint32_t)0x00000008)			/*!<Activation Sequence Generator Enable */
#define SC_ALTCTL_WARST_EN			((uint32_t)0x00000010)			/*!<Warm Reset Sequence Generator Enable */
#define SC_ALTCTL_TMR0_SEN			((uint32_t)0x00000020)			/*!<Internal Timer0 Start Enable */
#define SC_ALTCTL_TMR1_SEN			((uint32_t)0x00000040)			/*!<Internal Timer1 Start Enable */
#define SC_ALTCTL_TMR2_SEN			((uint32_t)0x00000080)			/*!<Internal Timer2 Start Enable */
#define SC_ALTCTL_INIT_SEL_MASK		((uint32_t)0x00000300)			/*!<Initial Timing Selection Mask*/
#define SC_ALTCTL_RX_BGT_EN			((uint32_t)0x00001000)			/*!<Receiver Block Guard Time Function Enable */
#define SC_ALTCTL_TMR0_ATV			((uint32_t)0x00002000)			/*!<Internal Timer0 Active State (Read Only) */
#define SC_ALTCTL_TMR1_ATV			((uint32_t)0x00004000)			/*!<Internal Timer1 Active State (Read Only) */
#define SC_ALTCTL_TMR2_ATV			((uint32_t)0x00008000)			/*!<Internal Timer2 Active State (Read Only) */

/********************* Bit definition of EGTR register **********************/
#define SC_EGTR_EGT_MASK			((uint32_t)0x000000FF)			/*!<Extended Guard Time Mask */

/********************* Bit definition of RFTMR register **********************/
#define SC_RFTMR_RFTM_MASK			((uint32_t)0x000001FF)			/*!<SC Receiver Buffer Time-Out Register (ETU Base) Mask */

/********************* Bit definition of ETUCR register **********************/
#define SC_ETUCR_ETU_RDIV_MASK		((uint32_t)0x00000FFF)			/*!<ETU Rate Divider Mask*/
#define SC_ETUCR_COMPEN_EN			((uint32_t)0x00008000)			/*!<Compensation Mode Enable */

/********************* Bit definition of IER register **********************/
#define SC_IER_RDA_IE				((uint32_t)0x00000001)			/*!<Receive Data Reach Interrupt Enable */
#define SC_IER_TBE_IE				((uint32_t)0x00000002)			/*!<Transmit Buffer Empty Interrupt Enable */
#define SC_IER_TERR_IE				((uint32_t)0x00000004)			/*!<Transfer Error Interrupt Enable */
#define SC_IER_TMR0_IE				((uint32_t)0x00000008)			/*!<Timer0 Interrupt Enable */
#define SC_IER_TMR1_IE				((uint32_t)0x00000010)			/*!<Timer1 Interrupt Enable */
#define SC_IER_TMR2_IE				((uint32_t)0x00000020)			/*!<Timer2 Interrupt Enable */
#define SC_IER_BGT_IE				((uint32_t)0x00000040)			/*!<Block Guard Time Interrupt Enable */
#define SC_IER_CD_IE				((uint32_t)0x00000080)			/*!<Card Detect Interrupt Enable */
#define SC_IER_INIT_IE				((uint32_t)0x00000100)			/*!<Initial End Interrupt Enable */
#define SC_IER_RTMR_IE				((uint32_t)0x00000200)			/*!<Receiver Buffer Time-Out Interrupt Enable  */
#define SC_IER_ACON_ERR_IE			((uint32_t)0x00000400)			/*!<Auto convention Error Interrupt Enable */

/********************* Bit definition of ISR register **********************/
#define SC_ISR_RDA_IS				((uint32_t)0x00000001)			/*!<Receive Data Reach Interrupt Status Flag (Read Only) */
#define SC_ISR_TBE_IS				((uint32_t)0x00000002)			/*!<Transmit Buffer Empty Interrupt Status Flag (Read Only) */
#define SC_ISR_TERR_IS				((uint32_t)0x00000004)			/*!<Transfer Error Interrupt Status Flag (Read Only) */
#define SC_ISR_TMR0_IS				((uint32_t)0x00000008)			/*!<Timer0 Interrupt Status Flag (Read Only) */
#define SC_ISR_TMR1_IS				((uint32_t)0x00000010)			/*!<Timer1 Interrupt Status Flag (Read Only) */
#define SC_ISR_TMR2_IS				((uint32_t)0x00000020)			/*!<Timer2 Interrupt Status Flag (Read Only) */
#define SC_ISR_BGT_IS				((uint32_t)0x00000040)			/*!<Block Guard Time Interrupt Status Flag (Read Only) */
#define SC_ISR_CD_IS				((uint32_t)0x00000080)			/*!<Card Detect Interrupt Status Flag (Read Only) */
#define SC_ISR_INIT_IS				((uint32_t)0x00000100)			/*!<Initial End Interrupt Status Flag (Read Only) */
#define SC_ISR_RTMR_IS				((uint32_t)0x00000200)			/*!<Receiver buffer Time-Out Interrupt Status Flag (Read Only) */
#define SC_ISR_ACON_ERR_IS			((uint32_t)0x00000400)			/*!<Auto Convention Error Interrupt Status Flag (Read Only) */

/********************* Bit definition of TRSR register **********************/
#define SC_TRSR_RX_OVER_F			((uint32_t)0x00000001)			/*!<RX Overflow Error Status Flag (Read Only)  */
#define SC_TRSR_RX_EMPTY_F			((uint32_t)0x00000002)			/*!<Receiver Buffer Empty Status Flag(Read Only) */
#define SC_TRSR_RX_FULL_F			((uint32_t)0x00000004)			/*!<Receiver Buffer Full Status Flag (Read Only) */
#define SC_TRSR_RX_EPA_F			((uint32_t)0x00000010)			/*!<Receiver Parity Error Status Flag (Read Only) */
#define SC_TRSR_RX_EFR_F			((uint32_t)0x00000020)			/*!<Receiver Frame Error Status Flag (Read Only) */
#define SC_TRSR_RX_EBR_F			((uint32_t)0x00000040)			/*!<Receiver Break Error Status Flag (Read Only) */
#define SC_TRSR_TX_OVER_F			((uint32_t)0x00000100)			/*!<TX Overflow Error Interrupt Status Flag (Read Only) */
#define SC_TRSR_TX_EMPTY_F			((uint32_t)0x00000200)			/*!<Transmit buffer Empty Status Flag (Read Only) */
#define SC_TRSR_TX_FULL_F			((uint32_t)0x00000400)			/*!<Transmit buffer Full Status flag (Read Only) */
#define SC_TRSR_RX_POINT_F_MASK		((uint32_t)0x00070000)			/*!<Receiver Buffer Pointer Status Flag (Read Only) Mask */
#define SC_TRSR_RX_ERETRY_F			((uint32_t)0x00200000)			/*!<Receiver Retry Error (Read Only) */
#define SC_TRSR_RX_OVER_ERETRY		((uint32_t)0x00400000)			/*!<Receiver Over Retry Error (Read Only) */
#define SC_TRSR_RX_ATV				((uint32_t)0x00800000)			/*!<Receiver In Active Status Flag (Read Only) */
#define SC_TRSR_TX_POINT_F_MASK		((uint32_t)0x07000000)			/*!<Transmit Buffer Pointer Status Flag (Read Only) Mask */
#define SC_TRSR_TX_ERETRY_F			((uint32_t)0x20000000)			/*!<Transmitter Retry Error (Read Only) */
#define SC_TRSR_TX_OVER_ERETRY		((uint32_t)0x40000000)			/*!<Transmitter Over Retry Error (Read Only) */
#define SC_TRSR_TX_ATV				((uint32_t)0x80000000)			/*!<Transmit In Active Status Flag (Read Only) */

/********************* Bit definition of PINCSR register **********************/
#define SC_PINCSR_POW_EN			((uint32_t)0x00000001)			/*!<SC_POW_EN Pin Signal */
#define SC_PINCSR_SC_RST			((uint32_t)0x00000002)			/*!<SC_RST Pin Signal */
#define SC_PINCSR_CD_REM_F			((uint32_t)0x00000004)			/*!<Card Detect Removal Status Of SC_CD Pin (Read Only) */
#define SC_PINCSR_CD_INS_F			((uint32_t)0x00000008)			/*!<Card Detect Insert Status Of SC_CD Pin (Read Only) */
#define SC_PINCSR_CD_PIN_ST			((uint32_t)0x00000010)			/*!<Card Detect Status Of SC_CD Pin Status (Read Only) */
#define SC_PINCSR_CLK_STOP_LEV		((uint32_t)0x00000020)			/*!<SC Clock Stop Level */
#define SC_PINCSR_CLK_KEEP			((uint32_t)0x00000040)			/*!<SC Clock Enable  */
#define SC_PINCSR_ADAC_CD_EN		((uint32_t)0x00000080)			/*!<Auto Deactivation When Card Removal */
#define SC_PINCSR_SC_OEN_ST			((uint32_t)0x00000100)			/*!<SC Data Output Enable Pin Status (Read Only) */
#define SC_PINCSR_SC_DATA_O			((uint32_t)0x00000200)			/*!<SC Data Output Pin */
#define SC_PINCSR_CD_LEV			((uint32_t)0x00000400)			/*!<Card Detect Level */
#define SC_PINCSR_POW_INV			((uint32_t)0x00000800)			/*!<SC_POW Pin Inverse */
#define SC_PINCSR_SC_DATA_I_ST		((uint32_t)0x00010000)			/*!<SC Data Input Pin Status (Read Only) */

/********************* Bit definition of TMR0 register **********************/
#define SC_TMR_MODE_DC				((uint32_t)0x00000000)		        /*!<Timer Operation Mode 0 */
#define SC_TMR_MODE_DC_S_TO			((uint32_t)0x01000000)		        /*!<Timer Operation Mode 1 */
#define SC_TMR_MODE_DC_RX_S_TO			((uint32_t)0x02000000)		        /*!<Timer Operation Mode 2 */
#define SC_TMR_MODE_DC_INITIAL_END		((uint32_t)0x03000000)		        /*!<Timer Operation Mode 3 */
#define SC_TMR_MODE_DC_RELOAD			((uint32_t)0x04000000)		        /*!<Timer Operation Mode 4 */
#define SC_TMR_MODE_DC_RELOAD_S_TO		((uint32_t)0x05000000)		        /*!<Timer Operation Mode 5 */
#define SC_TMR_MODE_DC_RELOAD_RX_S_TO		((uint32_t)0x06000000)		        /*!<Timer Operation Mode 6 */
#define SC_TMR_MODE_DC_RELOAD_S_S		((uint32_t)0x07000000)		        /*!<Timer Operation Mode 7 */
#define SC_TMR_MODE_UC				((uint32_t)0x08000000)		        /*!<Timer Operation Mode 8 */
#define SC_TMR_MODE_DC_RELOAD_C_S		((uint32_t)0x0F000000)		        /*!<Timer Operation Mode 15 */
#define SC_TMR0_CNT_MASK			((uint32_t)0x00FFFFFF)			/*!<Timer 0 Counter Value Register (ETU Base) Mask*/
#define SC_TMR0_MODE_MASK			((uint32_t)0x0F000000)			/*!<Timer 0 Operation Mode Selection Mask */

/********************* Bit definition of TMR1 register **********************/
#define SC_TMR1_CNT_MASK			((uint32_t)0x000000FF)			/*!<Timer 1 Counter Value Register (ETU Base) Mask */
#define SC_TMR1_MODE_MASK			((uint32_t)0x0F000000)			/*!<Timer 1 Operation Mode Selection Mask */

/********************* Bit definition of TMR2 register **********************/
#define SC_TMR2_CNT_MASK			((uint32_t)0x000000FF)			/*!<Timer 2 Counter Value Register (ETU Base) Mask */
#define SC_TMR2_MODE_MASK			((uint32_t)0x0F000000)			/*!<Timer 2 Operation Mode Selection Mask */

/********************* Bit definition of UACTL register **********************/
#define SC_UACTL_UA_MODE_EN			((uint32_t)0x00000001)			/*!<UART Mode Enable */
#define SC_UACTL_DATA_LEN_8_BITS	((uint32_t)0x00000000)			/*!<Data Length 8 bits */
#define SC_UACTL_DATA_LEN_7_BITS	((uint32_t)0x00000010)			/*!<Data Length 7 bits */
#define SC_UACTL_DATA_LEN_6_BITS	((uint32_t)0x00000020)			/*!<Data Length 6 bits */
#define SC_UACTL_DATA_LEN_5_BITS	((uint32_t)0x00000030)			/*!<Data Length 5 bits */
#define SC_UACTL_DATA_LEN_MASK		((uint32_t)0x00000030)			/*!<Data Length Mask */
#define SC_UACTL_PBDIS				((uint32_t)0x00000040)			/*!<Parity Bit Disable */
#define SC_UACTL_OPE				((uint32_t)0x00000080)			/*!<Odd Parity Enable */

/********************* Bit definition of TDRA register **********************/
#define SC_TDRA_TDR0_MASK			((uint32_t)0x00FFFFFF)			/*!<Timer0 Current Data Register Mask(Read Only) */

/********************* Bit definition of TDRB register **********************/
#define SC_TDRB_TDR1_MASK			((uint32_t)0x000000FF)			/*!<Timer1 Current Data Register Mask(Read Only) */
#define SC_TDRB_TDR2_MASK			((uint32_t)0x0000FF00)			/*!<Timer2 Current Data Register Mask(Read Only) */


/******************************************************************************/
/*                I2S                                                         */
/******************************************************************************/
/********************* Bit definition of CON register **********************/
#define	I2S_CON_I2SEN				((uint32_t)0x00000001)			/*!<I2S enable */
#define	I2S_CON_TXEN				((uint32_t)0x00000002)			/*!<Data transmit enable */
#define	I2S_CON_RXEN				((uint32_t)0x00000004)			/*!<Data receive enable */
#define	I2S_CON_MUTE				((uint32_t)0x00000008)			/*!<Transmit mute enable */
#define	I2S_CON_WORDWIDTH_8BIT		((uint32_t)0x00000000)			/*!<Word length, 8-bit */
#define	I2S_CON_WORDWIDTH_16BIT		((uint32_t)0x00000010)			/*!<Word length, 16-bit */
#define	I2S_CON_WORDWIDTH_24BIT		((uint32_t)0x00000020)			/*!<Word length, 24-bit */
#define	I2S_CON_WORDWIDTH_32BIT		((uint32_t)0x00000030)			/*!<Word length, 32-bit */
#define	I2S_CON_WORDWIDTH_MASK		((uint32_t)0x00000030)			/*!<Word length Mask */
#define	I2S_CON_MONO				((uint32_t)0x00000040)			/*!<Stereo or mono format select */
#define	I2S_CON_FORMAT				((uint32_t)0x00000080)			/*!<I2S or MSB-justified format select */
#define	I2S_CON_SLAVE				((uint32_t)0x00000100)			/*!<Slave or master mode */
#define	I2S_CON_TXTH_1_WORD			((uint32_t)0x00000200)			/*!<Transmit FIFO threshold level, 1 WORD */
#define	I2S_CON_TXTH_2_WORD			((uint32_t)0x00000400)			/*!<Transmit FIFO threshold level, 2 WORD */
#define	I2S_CON_TXTH_3_WORD			((uint32_t)0x00000600)			/*!<Transmit FIFO threshold level, 3 WORD */
#define	I2S_CON_TXTH_4_WORD			((uint32_t)0x00000800)			/*!<Transmit FIFO threshold level, 4 WORD */
#define	I2S_CON_TXTH_5_WORD			((uint32_t)0x00000A00)			/*!<Transmit FIFO threshold level, 5 WORD */
#define	I2S_CON_TXTH_6_WORD			((uint32_t)0x00000C00)			/*!<Transmit FIFO threshold level, 6 WORD */
#define	I2S_CON_TXTH_7_WORD			((uint32_t)0x00000E00)			/*!<Transmit FIFO threshold level, 7 WORD */
#define	I2S_CON_TXTH_MASK			((uint32_t)0x00000E00)			/*!<Transmit FIFO threshold level Mask */
#define	I2S_CON_RXTH_1_WORD			((uint32_t)0x00001000)			/*!<Receive FIFO threshold level, 1 WORD */
#define	I2S_CON_RXTH_2_WORD			((uint32_t)0x00002000)			/*!<Receive FIFO threshold level, 2 WORD */
#define	I2S_CON_RXTH_3_WORD			((uint32_t)0x00003000)			/*!<Receive FIFO threshold level, 3 WORD */
#define	I2S_CON_RXTH_4_WORD			((uint32_t)0x00004000)			/*!<Receive FIFO threshold level, 4 WORD */
#define	I2S_CON_RXTH_5_WORD			((uint32_t)0x00005000)			/*!<Receive FIFO threshold level, 5 WORD */
#define	I2S_CON_RXTH_6_WORD			((uint32_t)0x00006000)			/*!<Receive FIFO threshold level, 6 WORD */
#define	I2S_CON_RXTH_7_WORD			((uint32_t)0x00007000)			/*!<Receive FIFO threshold level, 7 WORD */
#define	I2S_CON_RXTH_MASK			((uint32_t)0x00007000)			/*!<Receive FIFO threshold level Mask */
#define	I2S_CON_MCLKEN				((uint32_t)0x00008000)			/*!<Master clock enable */
#define	I2S_CON_RCHZCEN				((uint32_t)0x00010000)			/*!<Right channel zero-cross detection enable */
#define	I2S_CON_LCHZCEN				((uint32_t)0x00020000)			/*!<Left channel zero-cross detect enable */
#define	I2S_CON_CLR_TXFIFO			((uint32_t)0x00040000)			/*!<Clear transmit FIFO */
#define	I2S_CON_CLR_RXFIFO			((uint32_t)0x00080000)			/*!<Clear receive FIFO */
#define	I2S_CON_TXDMA				((uint32_t)0x00100000)			/*!<Enable transmit DMA */
#define	I2S_CON_RXDMA				((uint32_t)0x00200000)			/*!<Enable receive DMA */
#define	I2S_CON_RXLCH				((uint32_t)0x00800000)			/*!<Select Mono Source Channel */

/********************* Bit definition of CLKDIV register **********************/
#define	I2S_CLKDIV_MCLK_DIV_MASK	((uint32_t)0x00000007)			/*!<Master clock divider Mask */
#define	I2S_CLKDIV_BCLK_DIV_MASK	((uint32_t)0x0000FF00)			/*!<Bit clock divider Mask */

/********************* Bit definition of INTEN register **********************/
#define	I2S_IE_RXUDFIE				((uint32_t)0x00000001)			/*!<Receive FIFO underflow interrupt enable */
#define	I2S_IE_RXOVFIE				((uint32_t)0x00000002)			/*!<Receive FIFO overflow interrupt enable */
#define	I2S_IE_RXTHIE				((uint32_t)0x00000004)			/*!<Receive FIFO threshold level interrupt enable */
#define	I2S_IE_TXUDFIE				((uint32_t)0x00000100)			/*!<Transmit FIFO underflow interrupt enable */
#define	I2S_IE_TXOVFIE				((uint32_t)0x00000200)			/*!<Transmit FIFO overflow interrupt enable */
#define	I2S_IE_TXTHIE				((uint32_t)0x00000400)			/*!<Transmit FIFO threshold level interrupt enable */
#define	I2S_IE_RZCIE				((uint32_t)0x00000800)			/*!<Right channel zero-cross interrupt enable */
#define	I2S_IE_LZCIE				((uint32_t)0x00001000)			/*!<Left channel zero-cross interrupt enable */

/********************* Bit definition of I2S_STATUS register **********************/
#define	I2S_STATUS_I2SINT			((uint32_t)0x00000001)			/*!<I2S interrupt flag */
#define	I2S_STATUS_I2SRXINT			((uint32_t)0x00000002)			/*!<Receive interrupt flag */
#define	I2S_STATUS_I2STXINT			((uint32_t)0x00000004)			/*!<Transmit interrupt flag */
#define	I2S_STATUS_RIGHT			((uint32_t)0x00000008)			/*!<Right or left channel select */
#define	I2S_STATUS_RXUDF			((uint32_t)0x00000100)			/*!<Receive FIFO underflow flag */
#define	I2S_STATUS_RXOVF			((uint32_t)0x00000200)			/*!<Receive FIFO overflow flag */
#define	I2S_STATUS_RXTHF			((uint32_t)0x00000400)			/*!<Receive FIFO threshold flag */
#define	I2S_STATUS_RXFULL			((uint32_t)0x00000800)			/*!<Receive FIFO full flag */
#define	I2S_STATUS_RXEMPTY			((uint32_t)0x00001000)			/*!<Receive FIFO empty flag */
#define	I2S_STATUS_TXUDF			((uint32_t)0x00010000)			/*!<Transmit FIFO underflow flag */
#define	I2S_STATUS_TXOVF			((uint32_t)0x00020000)			/*!<Transmit FIFO overflow flag */
#define	I2S_STATUS_TXTHF			((uint32_t)0x00040000)			/*!<Transmit FIFO threshold flag */
#define	I2S_STATUS_TXFULL			((uint32_t)0x00080000)			/*!<Transmit FIFO full flag */
#define	I2S_STATUS_TXEMPTY			((uint32_t)0x00100000)			/*!<Transmit FIFO empty flag */
#define	I2S_STATUS_TXBUSY			((uint32_t)0x00200000)			/*!<Transmit under busy */
#define	I2S_STATUS_RZCF				((uint32_t)0x00400000)			/*!<Right channnel zero-cross flag */
#define	I2S_STATUS_LZCF				((uint32_t)0x00800000)			/*!<Left channnel zero-cross flag */
#define	I2S_STATUS_RX_LEVEL_MASK	((uint32_t)0x0F000000)			/*!<Receive FIFO level Mask */
#define	I2S_STATUS_TX_LEVEL_MASK	((uint32_t)0xF0000000)			/*!<Transmit FIFO level Mask */

/*@}*/ /* end of group NANO1xx_Exported_Constants */


/******************************************************************************/
/*                Device Specific Macros                                      */
/******************************************************************************/
/** @addtogroup NANO1xx_Exported_Macros NANO1xx Exported Macros
  NANO1xx Device Specific Macros
  @{
*/
#define UNLOCKREG(x)				{GCR->RegLockAddr = 0x59; GCR->RegLockAddr = 0x16; GCR->RegLockAddr = 0x88;}  /*!< Unlock protected register */
#define LOCKREG(x)          		GCR->RegLockAddr = 0x00  /*!< Lock protected register */  

/******************************************************************************/
/*                Multi-Function Pin                                          */
/******************************************************************************/
/*!PortA14->UART0 TX; PortA15->UART0 RX */
#define MFP_UART0_TO_PORTA()		(GCR->PA_H_MFP = GCR->PA_H_MFP &~(PA15_MFP_MASK|PA14_MFP_MASK) | (PA15_MFP_UART0_TX|PA14_MFP_UART0_RX))

/*!<PortA3->UART1 TX; PortA2->UART1 RX */
#define MFP_UART1_TO_PORTA()		(GCR->PA_L_MFP = GCR->PA_L_MFP &~(PA3_MFP_MASK|PA2_MFP_MASK)|(PA2_MFP_UART1_TX|PA3_MFP_UART1_RX))

/*!<PortB0->UART0 RX; PortB1->UART0 TX */
#define MFP_UART0_TO_PORTB()		(GCR->PB_L_MFP = GCR->PB_L_MFP &~(PB0_MFP_MASK|PB1_MFP_MASK)|(PB0_MFP_UART0_RX|PB1_MFP_UART0_TX))
/*!<PortB0->UART0 RX; PortB1->UART0 TX; PortB2->RTS; PortB3->CTS */
#define MFP_FULL_UART0_TO_PORTB()	(GCR->PB_L_MFP = GCR->PB_L_MFP &~0x0000ffff|0x00001111)

/*!<PortB4->UART1 RX; PortB5->UART1 TX */
#define MFP_UART1_TO_PORTB()		(GCR->PB_L_MFP = GCR->PB_L_MFP &~(PB4_MFP_MASK|PB5_MFP_MASK)|(PB4_MFP_UART1_RX|PB5_MFP_UART1_TX))
/*!<PortB4->UART0 RX; PortB5->UART0 TX; PortB6->RTS; PortB7->CTS */
#define MFP_FULL_UART1_TO_PORTB()	(GCR->PB_L_MFP = GCR->PB_L_MFP &~0xffff0000|0x11110000)

/*!Enable 8-BIT EBI: PA6~7->AD7~6, PA10->nWE, PA11->nRE, PB6->ALE, PB7->nCS, PB12~13->AD0~1, PC6~7->AD4~5, PC14~15->AD2~3, PC8->XCLK */
#define MFP_8BIT_EBI()		{ \
								GCR->PA_L_MFP = GCR->PA_L_MFP &~(PA6_MFP_MASK|PA7_MFP_MASK)|(PA6_MFP_EBI_AD7|PA7_MFP_EBI_AD6); \
                             	GCR->PA_H_MFP = GCR->PA_H_MFP &~(PA10_MFP_MASK|PA11_MFP_MASK)|(PA10_MFP_EBI_NWE|PA11_MFP_EBI_NRE); \
							 	GCR->PB_L_MFP = GCR->PB_L_MFP &~(PB6_MFP_MASK|PB7_MFP_MASK)|(PB6_MFP_EBI_ALE|PB7_MFP_EBI_NCS); \
							 	GCR->PB_H_MFP = GCR->PB_H_MFP &~(PB12_MFP_MASK|PB13_MFP_MASK)|(PB12_MFP_EBI_AD0|PB13_MFP_EBI_AD1); \
							 	GCR->PC_L_MFP = GCR->PC_L_MFP &~(PC6_MFP_MASK|PC7_MFP_MASK)|(PC6_MFP_EBI_AD4|PC7_MFP_EBI_AD5); \
							 	GCR->PC_H_MFP = GCR->PC_H_MFP &~0xff00000f|0x22000002; \
							}

/*!Enable 16-BIT EBI:  PA6~7->AD7~6, PA10->nWE, PA11->nRE, PB6->ALE, PB7->nCS, PB12~13->AD0~1, PC6~7->AD4~5, PC14~15->AD2~3, PC8->XCLK
                       PA1~5->AD12~8, PA12~14->AD13~15, PB2->nWRL, PB3->nWRH */
#define MFP_16BIT_EBI()		{ \
								GCR->PA_L_MFP = GCR->PA_L_MFP &~0xfffffff0|0x22222220; \
								GCR->PA_H_MFP = GCR->PA_H_MFP &~0x0fffff00|0x02222200; \
								GCR->PB_L_MFP = GCR->PB_L_MFP &~0xff00ff00|0x22002200; \
								GCR->PB_H_MFP = GCR->PB_H_MFP &~(PB12_MFP_MASK|PB13_MFP_MASK)|(PB12_MFP_EBI_AD0|PB13_MFP_EBI_AD1); \
								GCR->PC_L_MFP = GCR->PC_L_MFP &~(PC6_MFP_MASK|PC7_MFP_MASK)|(PC6_MFP_EBI_AD4|PC7_MFP_EBI_AD5); \
								GCR->PC_H_MFP = GCR->PC_H_MFP &~0xff00000f|0x22000002; \
							}

/*!<Enable SmartCard0: PA8->CLK, PA9->DAT, PA10->PWR, PA11->RST, PB4->Card Detection */
#define MFP_SC0_TO_PORTA()	{ \
								GCR->PA_H_MFP = GCR->PA_H_MFP &~0x0000ffff|0x00003333; \
								GCR->PB_L_MFP = GCR->PB_L_MFP &~PB4_MFP_MASK|PB4_MFP_SC0_CD; \
							}

/*!<Enable SmartCard1: PC0->CLK, PC1->DAT, PC2->PWR, PC3->RST, PC6->Card Detection */
#define MFP_SC1_TO_PORTC()	(GCR->PC_L_MFP = GCR->PC_L_MFP &~0x0f00ffff|0x04004444)

/*!<Enable SmartCard1: PD0->CLK, PD1->DAT, PD2->PWR, PD3->RST, PD4->Card Detection */
#define MFP_SC1_TO_PORTD()	(GCR->PD_L_MFP = GCR->PD_L_MFP &~0x000fffff|0x00044444)

/*!<Enable SmartCard2: PA0->Card Detection, PA4->PWR, PA5->RST, PA6->CLK, PA7->DAT */
#define MFP_SC2_TO_PORTA()	(GCR->PA_L_MFP = GCR->PA_L_MFP &~0xffff000f|0x44440004)

/*!<Enable SmartCard2: PB8->PWR, PB9->RST, PB10->CLK, PB11->DAT, PB14->Card Detection */
#define MFP_SC2_TO_PORTB()	(GCR->PB_H_MFP = GCR->PB_H_MFP &~0x0f00ffff|0x04004444)

/*!<Enable I2S: GPA15->mclk; GPC0->ws, GPC1->bclk, GPC2->din, GPC3->dout */
#define MFP_I2S_TO_PORTC()	{ \
								GCR->PA_H_MFP = GCR->PA_H_MFP &~PA15_MFP_MASK|PA15_MFP_I2S_MCLK; \
								GCR->PC_L_MFP = GCR->PC_L_MFP &~0x0000ffff|0x00002222; \
							}

/*!<Enable I2S: GPE0->mclk; GPD2->ws, GPD3->bclk, GPD4->din, GPD5->dout */
#define MFP_I2S_TO_PORTD()	{ \
								GCR->PE_L_MFP = GCR->PE_L_MFP &~PE0_MFP_MASK|PE0_MFP_I2S_MCLK; \
								GCR->PD_L_MFP = GCR->PD_L_MFP & ~0x00ffff00|0x00222200; \
							}

/*!<Enable GPB14->EXT0 */
#define MFP_EXT_INT0_TO_PB14()	(GCR->PB_H_MFP = GCR->PB_H_MFP & ~PB14_MFP_MASK | PB14_MFP_EXT_INT0)

/*!<Enable GPB9->EXT0 */
#define MFP_EXT_INT0_TO_PB9()	(GCR->PB_H_MFP = GCR->PB_H_MFP & ~PB9_MFP_MASK | PB9_MFP_EXT_INT0)

/*!<Enable GPB8->EXT0 */
#define MFP_EXT_INT0_TO_PB8()	(GCR->PB_H_MFP = GCR->PB_H_MFP & ~PB8_MFP_MASK | PB8_MFP_EXT_INT0)

/*!<Enable GPC12->EXT0 */
#define MFP_EXT_INT0_TO_PC12()	(GCR->PC_H_MFP = GCR->PC_H_MFP & ~PC12_MFP_MASK | PC12_MFP_EXT_INT0)

/*!<Enable GPF0->EXT0 */
#define MFP_EXT_INT0_TO_PF0()	(GCR->PF_L_MFP = GCR->PF_L_MFP & ~PF0_MFP_MASK | PF0_MFP_EXT_INT0)

/*!<Enable GPB15->EXT1 */
#define MFP_EXT_INT1_TO_PB15()	(GCR->PB_H_MFP = GCR->PB_H_MFP & ~PB15_MFP_MASK | PB15_MFP_EXT_INT1)

/*!<Enable GPC13->EXT1 */
#define MFP_EXT_INT1_TO_PC13()	(GCR->PC_H_MFP = GCR->PC_H_MFP & ~PC13_MFP_MASK | PC13_MFP_EXT_INT1)

/*!<Enable GPF1->EXT1 */
#define MFP_EXT_INT1_TO_PF1()	(GCR->PF_L_MFP = GCR->PF_L_MFP & ~PF1_MFP_MASK | PF1_MFP_EXT_INT1)

/*!<Enable SPI0: GPB10->SS1, GPC0->SS0; GPC1->CLK, GPC2->MISO0, GPC3->MOSI0, GPC4->MISO1, GPC5->MOSI1 */
#define MFP_SPI0_TO_PORTC1(){ \
								GCR->PB_H_MFP = GCR->PB_H_MFP &~PB10_MFP_MASK|PB10_MFP_SPI0_SS1; \
								GCR->PC_L_MFP = GCR->PC_L_MFP & ~0x00ffffff|0x00111111; \
							}

/*!<Enable SPI0: GPB10->MOSI0, GPB11->MISO0, GPC0->SS0; GPC1->CLK, GPC4->MISO1, GPC5->MOSI1, NO SS1 */
#define MFP_SPI0_TO_PORTC2(){ \
								GCR->PB_H_MFP = GCR->PB_H_MFP &~(PB10_MFP_MASK|PB11_MFP_MASK)|(PB10_MFP_SPI0_MOSI0|PB11_MFP_SPI0_MISO0); \
								GCR->PC_L_MFP = GCR->PC_L_MFP & ~0x00ff00ff|0x00110011; \
							}

/*!<Enable SPI0: GPB10->SS1, GPE1->SS0; GPE2->CLK, GPE3->MISO0, GPE4->MOSI0, GPC4->MISO1, GPC5->MOSI1 */
#define MFP_SPI0_TO_PORTE()	{ \
								GCR->PB_H_MFP = GCR->PB_H_MFP &~PB10_MFP_MASK|PB10_MFP_SPI0_SS1; \
								GCR->PC_L_MFP = GCR->PC_L_MFP & ~0x00ff0000|0x00110000; \
								GCR->PE_L_MFP = GCR->PE_L_MFP & ~0x000ffff0|0x00066660; \
							}

/*!<Enable SPI1: GPB0->MOSI0, GPB1->MISO0; GPB2->CLK, GPB3->SS0, GPB9->SS1, GPC12->MISO1, GPC13->MOSI1 */
#define MFP_SPI1_TO_PORTB()	{ \
								GCR->PB_H_MFP = GCR->PB_H_MFP &~PB9_MFP_MASK|PB9_MFP_SPI1_SS1; \
								GCR->PB_L_MFP = GCR->PB_L_MFP & ~0x0000ffff|0x00003333; \
								GCR->PC_H_MFP = GCR->PC_H_MFP & ~0x00ff0000|0x00110000; \
							}

/*!<Enable SPI1: GPB9->SS1, GPC8->SS0; GPC9->CLK, GPC10->MISO0, GPC11->MOSI0, GPC12->MISO1, GPC13->MOSI1 */
#define MFP_SPI1_TO_PORTC()	{ \
								GCR->PB_H_MFP = GCR->PB_H_MFP &~PB9_MFP_MASK|PB9_MFP_SPI1_SS1; \
								GCR->PC_H_MFP = GCR->PC_L_MFP & ~0x00ffffff|0x00111111; \
							}

/*!<Enable SPI2: GPA9->CLK, GPA8->SS0, GPA10->MISO0, GPA11->MOSI0, GPD4->MISO1, GPD5->MOSI1, GPB14->SS1 */
#define MFP_SPI2_TO_PORTA()	{ \
								GCR->PB_H_MFP = GCR->PB_H_MFP &~PB14_MFP_MASK|PB14_MFP_SPI2_SS1; \
								GCR->PD_L_MFP = GCR->PD_L_MFP & ~0x00ff0000|0x00330000; \
								GCR->PA_H_MFP = GCR->PA_H_MFP & ~0x0000ffff|0x00004444; \
							}

/*!<Enable SPI2: GPB5->CLK, GPB4->SS0, GPB6->MISO0, GPB7->MOSI0, GPD4->MISO1, GPD5->MOSI1, GPB14->SS1 */
#define MFP_SPI2_TO_PORTB()	{ \
								GCR->PB_H_MFP = GCR->PB_H_MFP &~PB14_MFP_MASK|PB14_MFP_SPI2_SS1; \
								GCR->PD_L_MFP = GCR->PD_L_MFP & ~0x00ff0000|0x00330000; \
								GCR->PB_L_MFP = GCR->PB_L_MFP & ~0xffff0000|0x44440000; \
							}

/*!<Enable SPI2: GPD1->CLK, GPD0->SS0, GPD2->MISO0, GPD3->MOSI0, GPD4->MISO1, GPD5->MOSI1, GPB14->SS1 */
#define MFP_SPI2_TO_PORTD()	{ \
								GCR->PB_H_MFP = GCR->PB_H_MFP &~PB14_MFP_MASK|PB14_MFP_SPI2_SS1; \
								GCR->PD_L_MFP = GCR->PD_L_MFP & ~0x00ffffff|0x00333333; \
							}

/*!<Enable LCD for 100/128-Pin Package */
#define MFP_LCD_TYPEA()	{ \
							GCR->PA_L_MFP |= 0x77770000;	/* seg 36 ~ 39 */\
							GCR->PA_H_MFP |= 0x7777;		/* seg 20 ~ 23 */\
							GCR->PB_L_MFP = 0x77777777;		/* seg 10 ~ 13, 4 ~ 7 */\
							GCR->PB_H_MFP = 0x77777777;		/* LCD V1 ~ V3, seg 30 ~ 31, 24 ~ 26 */\
							GCR->PC_L_MFP |= 0x777777;		/* LCD COM3 ~ COM0, DH1/DH2 */\
							GCR->PC_H_MFP |= 0x77000000;	/* seg 32 ~ 33 */\
							GCR->PD_L_MFP |= 0x77770000;	/* seg 2 ~ 3, 34 ~ 35 */\
							GCR->PD_H_MFP = 0x77777777;		/* seg 0 ~ 1, 14 ~ 19 */\
							GCR->PE_L_MFP |= 0x70000000;	/* seg 8 */\
							GCR->PE_H_MFP |= 0x77700007;	/* seg 9, 27 ~ 29 */\
							GPIOB->OFFD |= 0xE000000;	/* V1, V2 and V3 */\
							GPIOC->OFFD |= 0x30000;	/* DH1 and DH2 */\
							GPIOC->OFFD |= 0x3C0000;	/* COM0~3 */\
							GPIOA->OFFD |= 0x0FF00000;	/* SEG0~39 */\
							GPIOB->OFFD |= 0xF1FF0000;\
							GPIOC->OFFD |= 0xC0000000;\
							GPIOD->OFFD |= 0xFFF00000;\
							GPIOE->OFFD |= 0xE1800000;\
						}

/*!<Enable LCD for 64-Pin Package */
#define MFP_LCD_TYPEB()	{ \
							GCR->PA_L_MFP |= 0x77777700;	/* seg 18 ~ 23 */\
							GCR->PA_H_MFP = 0x77777777;		/* seg 6 ~ 9, 24 ~ 27 */\
							GCR->PB_L_MFP = 0x77777777;		/* COM2, COM3, seg 0 ~ 5 */\
							GCR->PB_H_MFP = 0x77777777;		/* LCD V1 ~ V3, seg 10 ~ 14 */\
							GCR->PC_L_MFP |= 0x70007777;	/* LCD COM1 ~ COM0, DH1/DH2, seg 17 */\
							GCR->PC_H_MFP |= 0x77007777;	/* seg 28 ~ 31, 15 ~ 16 */\
							GPIOB->OFFD |= 0xE000000;	/* V1, V2 and V3 */\
							GPIOC->OFFD |= 0x30000;	/* DH1 and DH2 */\
							GPIOC->OFFD |= 0xC0000;	/* COM0~3 */\
							GPIOB->OFFD |= 0xC0000;\
							GPIOA->OFFD |= 0xFFFC0000;	/* SEG0~39 */\
							GPIOB->OFFD |= 0xF1F30000;\
							GPIOC->OFFD |= 0xCF800000;\
						}

/*@}*/ /* end of group NANO1xx_Exported_Macros */


/******************************************************************************/
/*                Legacy Constants                                            */
/******************************************************************************/
/** @addtogroup NANO1xx_legacy_Constants NANO1xx Legacy Constants
  NANO1xx Legacy Constants
  @{
*/
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


#ifndef E_SUCCESS
#define E_SUCCESS 	(0)
#endif
#ifndef NULL
#define NULL        (0)
#endif

#define TRUE	   	(1)
#define FALSE   	(0)

#define ENABLE     (1)
#define DISABLE    (0)

/* Define one bit mask */
#define BIT0	(0x00000001)
#define BIT1	(0x00000002)
#define BIT2	(0x00000004)
#define BIT3	(0x00000008)
#define BIT4	(0x00000010)
#define BIT5	(0x00000020)
#define BIT6	(0x00000040)
#define BIT7	(0x00000080)
#define BIT8	(0x00000100)
#define BIT9	(0x00000200)
#define BIT10	(0x00000400)
#define BIT11	(0x00000800)
#define BIT12	(0x00001000)
#define BIT13	(0x00002000)
#define BIT14	(0x00004000)
#define BIT15	(0x00008000)
#define BIT16	(0x00010000)
#define BIT17	(0x00020000)
#define BIT18	(0x00040000)
#define BIT19	(0x00080000)
#define BIT20	(0x00100000)
#define BIT21	(0x00200000)
#define BIT22	(0x00400000)
#define BIT23	(0x00800000)
#define BIT24	(0x01000000)
#define BIT25	(0x02000000)
#define BIT26	(0x04000000)
#define BIT27	(0x08000000)
#define BIT28	(0x10000000)
#define BIT29	(0x20000000)
#define BIT30	(0x40000000)
#define BIT31	(0x80000000)

/*@}*/ /* end of group NANO1xx_legacy_Constants */


/*@}*/ /* end of group NANO1xx_Definitions */

#ifdef __cplusplus
}
#endif

#endif  /* __NANO1xx_H */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
