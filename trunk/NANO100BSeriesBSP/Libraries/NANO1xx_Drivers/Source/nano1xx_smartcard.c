/******************************************************************************
 * @file     nano1xx_smartcard.c
 * @brief    NANO1xx Smartcard driver source file
 * @version  1.0.2
 * @date     02, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nano1xx_sys.h"
#include "nano1xx_smartcard.h"
#include "nano1xx_assert.h"
#include "nano1xx_gpio.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_SMARTCARD_Driver SMARTCARD Driver
  @{
*/


/** @addtogroup NANO1xx_SMARTCARD_EXPORTED_VARIABLES SMARTCARD Exported Variables
  @{
*/
/* global buffer is used for IST to receive */
uint8_t g_Port0_RxBUF[MIN_BUFFER_SIZE];
uint8_t g_Port1_RxBUF[MIN_BUFFER_SIZE];
uint8_t g_Port2_RxBUF[MIN_BUFFER_SIZE];

/* global buffer is used for IST to transmit */
uint8_t g_Port0_TxBUF[MIN_BUFFER_SIZE];
uint8_t g_Port1_TxBUF[MIN_BUFFER_SIZE];
uint8_t g_Port2_TxBUF[MIN_BUFFER_SIZE];

/* global variable for SmartCard Device */
S_SC_DEV_T sc_device[SMARTCARD_NUM];

/* global variables for receiving ATR from smartcard */
/*volatile*/ uint32_t atr_len0=0, atr_remain0=2/* TS & T0 */;
/*volatile*/ int32_t ifbyte_flag0=-1/* interface bytes flag */, tck0=0, ifcount0=0;

/*volatile*/ uint32_t atr_len1=0, atr_remain1=2/* TS & T0 */;
/*volatile*/ int32_t ifbyte_flag1=-1/* interface bytes flag */, tck1=0, ifcount1=0;

/*volatile*/ uint32_t atr_len2=0, atr_remain2=2/* TS & T0 */;
/*volatile*/ int32_t ifbyte_flag2=-1/* interface bytes flag */, tck2=0, ifcount2=0;

/* ATR is returned within ATR_TOTAL_TIME ETUs defined by EMV */
/*volatile*/ int32_t atr_time0=ATR_TOTAL_TIME, atr_time1=ATR_TOTAL_TIME, atr_time2=ATR_TOTAL_TIME;
/*volatile*/ int32_t atr_check_time0=0,atr_check_time1=0,atr_check_time2=0;
/*volatile*/ int32_t atr_total_time_start_flag0 = 0;
/*volatile*/ int32_t atr_total_time_start_flag1 = 0;
/*volatile*/ int32_t atr_total_time_start_flag2 = 0;

/* ATR: spend one waiting to check if other redundant byte */
/*volatile*/ int32_t atr_final_chk0=0, atr_final_chk1=0, atr_final_chk2=0;


/* To store the expected data from ICC, driver will keep it for next reading request */
/*volatile*/ uint8_t g_SC0UnexpectedInBuf[MIN_BUFFER_SIZE], g_SC1UnexpectedInBuf[MIN_BUFFER_SIZE], g_SC2UnexpectedInBuf[MIN_BUFFER_SIZE];
/*volatile*/ uint8_t g_SC0UnexpectedBufLen = 0, g_SC1UnexpectedBufLen = 0, g_SC2UnexpectedBufLen = 0;


/* re-fill ETU divider */
/*volatile*/ uint32_t g_SC0ETUDivider = 372;
/*volatile*/ uint32_t g_SC1ETUDivider = 372;
/*volatile*/ uint32_t g_SC2ETUDivider = 372;

/*@}*/ /* end of group NANO1xx_SMARTCARD_EXPORTED_VARIABLES */


/** @addtogroup NANO1xx_SMARTCARD_EXPORTED_FUNCTIONS SMARTCARD Exported Functions
  @{
*/


/**
  * @brief   Retuen string describe error number
  * @param   errno Error number
  * @return  Error string
  */
uint8_t * SMARTCARD_ERRMSG(long errno)
{
        if(errno == SC_ERR_CARD_REMOVED)
                return "SC_ERR_CARD_REMOVED";
        if(errno == SC_ERR_OVER_RUN)
                return "SC_ERR_OVER_RUN";
        if(errno == SC_ERR_PARITY_ERROR)
                return "SC_ERR_PARITY_ERROR";
        if(errno == SC_ERR_NO_STOP)
                return "SC_ERR_NO_STOP";
        if(errno == SC_ERR_SILENT_BYTE)
                return "SC_ERR_SILENT_BYTE";
        if(errno == SC_ERR_CMD)
                return "SC_ERR_CMD";
        if(errno == SC_ERR_UNSUPPORTEDCARD)
                return "SC_ERR_UNSUPPORTEDCARD";
        if(errno == SC_ERR_READ)
                return "SC_ERR_READ";
        if(errno == SC_ERR_WRITE)
                return "SC_ERR_WRITE";
        if(errno == SC_ERR_TIME0OUT)
                return "SC_ERR_TIME0OUT";
        if(errno == SC_ERR_TIME1OUT)
                return "SC_ERR_TIME1OUT";
        if(errno == SC_ERR_TIME2OUT)
                return "SC_ERR_TIME2OUT";
        if(errno == PROTOCOL_T0_ERROR)
                return "PROTOCOL_T0_ERROR";
        if(errno == SC_ERR_AUTOCONVENTION)
                return "SC_ERR_AUTOCONVENTION";
        if(errno == SC_ERR_BGTIMEOUT)
                return "SC_ERR_BGTIMEOUT";
        if(errno == SC_EPROTOCOL)
                return "SC_EPROTOCOL";
        if(errno == T1_ERROR_CHKSUM)
                return "T1_ERROR_CHKSUM";
        if(errno == PROTOCOL_T1_ERROR)
                return "PROTOCOL_T1_ERROR";
        if(errno == ATR_ERR_INVALID_PARAMETER)
                return "ATR_ERR_INVALID_PARAMETER";

        return "No Such Error Messages";
}

/**
  * @brief  Check if SmartCard slot is presented
  * @param  portno Indicate which port to be used (SC0, SC1, SC2)
  * @return Card present or not
  * @retval SCARD_PRESENT SmartCard is presented
  * @retval SCARD_ABSENT  SmartCard is absent
  */
int32_t SMARTCARD_CheckCardPresent(SC_TypeDef *portno)
{
        S_SC_DEV_T *dev;
        int32_t port = 0;

        if(portno==SC0)
                port = 0;
        else if(portno==SC1)
                port = 1;
        else if(portno==SC2)
                port = 2;

        dev= (S_SC_DEV_T *)(uint32_t)&sc_device[port];

        return dev->bCardRemoved;
}

/**
  * @brief  Driver always returns card is present. This function generally is called when
  *         no card detection pin in HW environment
  * @param  portno Indicate which port to be used (SC0, SC1, SC2)
  * @return None
  */
void SMARTCARD_IgnoreCardAbsent(SC_TypeDef *portno)
{
        S_SC_DEV_T *dev;
        int32_t port = 0;


        if(portno==SC0)
                port = 0;
        else if(portno==SC1)
                port = 1;
        else if(portno==SC2)
                port = 2;

        dev= (S_SC_DEV_T *)(uint32_t)&sc_device[port];

        dev->bCardRemoved = SCARD_PRESENT;

}


/**
  * @brief  Enable/Disable smartcard clock
  * @param  dev  Pointer of data structure of smartcard information
  * @param  onoff   1:Enable clock; 0:Disable clock
  * @return None
  */
void SMARTCARD_SetClockState(S_SC_DEV_T *dev, int8_t onoff)
{
        SC_TypeDef *SC = dev->base;

        assert_param((onoff==0) || (onoff==1));

        if ( onoff == 1 )
                SC->PINCSR |= SC_PINCSR_CLK_KEEP;
        else
                SC->PINCSR &= ~SC_PINCSR_CLK_KEEP;
}

/**
  * @brief  Set the clock stop level according the clock_stop_level field of structure.
  * @param  dev  Pointer of data structure of smartcard information
  * @return None
  */
static void SMARTCARD_ClockStopLevel(S_SC_DEV_T *dev)
{
        SC_TypeDef *SC = dev->base;
        E_CLOCK_STOP clock_level = dev->clock_stop_level;

        if ( clock_level == CLOCKSTOP_LOW )
                SC->PINCSR &= ~SC_PINCSR_CLK_STOP_LEV;
        else if ( clock_level == CLOCKSTOP_HIGH )
                SC->PINCSR |= SC_PINCSR_CLK_STOP_LEV;
}

/**
  * @brief  Enable Tx error retry function and set the number of times of Tx error retry.
  *         Tx Error retry means that transmission has parity error, hardware will re-transmit again.
  * @param  dev    The pointer of data structure of smartcard information
  * @param  errNum The number of times of Tx error retry. The range of value is from 0 to 8.
  * @return None
  */
static void SMARTCARD_SetTxRetry(S_SC_DEV_T *dev, uint32_t errNum)
{
        SC_TypeDef *SC = dev->base;

        assert_param( errNum<9 );

        if(errNum == 0) {       // disable Tx error retry
                SC->CTL &= ~(SC_CTL_TX_ERETRY_MASK | SC_CTL_TX_ERETRY_EN);
        } else {
                SC->CTL = (SC->CTL & ~SC_CTL_TX_ERETRY_MASK) | ((errNum - 1) << 20) | SC_CTL_TX_ERETRY_EN;
        }

}

/**
  * @brief  Enable Rx error retry function and set the number of times of Rx error retry.
  *         Rx Error retry means that receiver detects a parity error, hardware will pull I/O LOW to notice the transmitter
  * @param  dev    The pointer of data structure of smartcard information
  * @param  errNum The number of times of Rx error retry. The range of value is from 0 to 8.
  * @return None
  */
static void SMARTCARD_SetRxRetry(S_SC_DEV_T *dev, uint32_t errNum)
{
        SC_TypeDef *SC = dev->base;

        assert_param( errNum<9 );

        if(errNum == 0) {       // disable Rx error retry
                SC->CTL &= ~(SC_CTL_RX_ERETRY_MASK | SC_CTL_RX_ERETRY_EN);
        } else {
          SC->CTL = (SC->CTL & ~SC_CTL_RX_ERETRY_MASK) | ((errNum - 1) << 16) | SC_CTL_RX_ERETRY_EN;
        }
}

/**
  * @brief  Reset the Tx/Rx FIFO
  * @param  dev  Pointer of data structure of smartcard information
  * @return None
  */
void SMARTCARD_ClearFIFO(S_SC_DEV_T *dev)
{
        SC_TypeDef *SC = dev->base;

        /* Check if the end of initial end sequence */
        while(SC->ALTCTL & SC_ALTCTL_WARST_EN) ;
        while(SC->ALTCTL & SC_ALTCTL_ACT_EN) ;
        while(SC->ALTCTL & SC_ALTCTL_DACT_EN) ;
        SC->ALTCTL |= (SC_ALTCTL_TX_RST | SC_ALTCTL_RX_RST);

}

/**
  * @brief  Turn on and configure the output voltage according the voltage field of structure.
  * @param  dev     The pointer of data structure of smartcard information
  * @return None
  * @note   This is an empty API, implementation is base on hardware design
  */
void SMARTCARD_VoltageConfig(S_SC_DEV_T *dev)
{

        /* select power supply */
        switch(dev->voltage) {
        case SC_ISO_OPERATIONCLASS_A:           /* 5v */
                // TODO: configure VCC to 5V according to hardware design

                break;

        case SC_ISO_OPERATIONCLASS_B:           /* 3v */
                // TODO: configure VCC to 3V according to hardware design

                break;
        default:
        case SC_ISO_OPERATIONCLASS_C:           /* 1.8v */
                // TODO: configure VCC to 1.8V according to hardware design

                break;
        }

}

/**
  * @brief  Determine the active low/high of power pin
  * @param  portno Indicate which port to be used (SC0, SC1, SC2)
  * @param  active 1: active HIGH, 0: active LOW
  * @return None
  */
void SMARTCARD_PowerActiveLevel(SC_TypeDef *portno, int8_t active)
{
#ifdef SC_SEQ_MANUAL
        S_SC_DEV_T *dev;
        int32_t port = 0;

        if(portno==SC0)
                port = 0;
        else if(portno==SC1)
                port = 1;
        else if(portno==SC2)
                port = 2;

        dev = (S_SC_DEV_T *)((uint32_t)&sc_device[port]);

        portno->CTL &= ~SC_CTL_SC_CEN;
        portno->PINCSR &= ~SC_PINCSR_POW_INV;

        portno->CTL |= SC_CTL_SC_CEN;
        if(active==1) {
                portno->PINCSR &= ~SC_PINCSR_POW_EN;    // power pin low in default
                dev->ActivePowerPin = 1;
        } else if(active==0) {
                portno->PINCSR |= SC_PINCSR_POW_EN;     // power pin high in default
                dev->ActivePowerPin = 0;
        }
#else

        portno->CTL &= ~SC_CTL_SC_CEN;
        portno->CTL |= SC_CTL_SC_CEN;
#endif

}


/**
  * @brief  De-activation sequence generator
  * @param  dev The pointer of data structure of smartcard information
  * @return None
  */
void SMARTCARD_DeactivationCmd(S_SC_DEV_T *dev)
{
        SC_TypeDef *SC = dev->base;

#ifdef  SC_SEQ_MANUAL
        // Wait for Tx & Rx get into idle mode
        while(SC->TRSR & SC_TRSR_TX_ATV) ;
        SC->ALTCTL |= SC_ALTCTL_TX_RST;
        while(SC->TRSR & SC_TRSR_RX_ATV) ;
        SC->ALTCTL |= SC_ALTCTL_RX_RST;
        // Reset
        SC->PINCSR &= ~SC_PINCSR_SC_RST;
        while(SC->PINCSR & SC_PINCSR_SC_RST) ;  // wait HW reset pin really becomes LOW
        // Clock
        SC->PINCSR &= ~SC_PINCSR_CLK_KEEP;
        // I/O
        SC->PINCSR &= ~SC_PINCSR_SC_DATA_O;             // output LOW
        while(SC->PINCSR & SC_PINCSR_SC_DATA_O) ;       // wait HW I/O pin really becomes LOW
        // Power
        if(dev->ActivePowerPin)
                SC->PINCSR &= ~SC_PINCSR_POW_EN;
        else
                SC->PINCSR |= SC_PINCSR_POW_EN;
#else
        SC->ALTCTL |= SC_ALTCTL_DACT_EN;
#endif
}
/**
  * @brief  Activation sequence generator
  * @param  dev The pointer of data structure of smartcard information
  * @return None
  */
void SMARTCARD_ActivationCmd(S_SC_DEV_T *dev)
{
        SC_TypeDef *SC = dev->base;

#ifdef  SC_SEQ_MANUAL
        // disable Timer1 interrupt and use polling method to check time-out happened
        SC->IER &= ~SC_IER_TMR1_IE;

        // Power
        if(dev->ActivePowerPin)
                SC->PINCSR |= SC_PINCSR_POW_EN;
        else
                SC->PINCSR &= ~SC_PINCSR_POW_EN;
        // Clock
        SMARTCARD_TimerCountSet(dev, SC_TIMER1, SC_TMR_MODE_DC, 1);
        SMARTCARD_TimerStart(dev, SC_TIMER1);
        while(((SC->ISR & SC_ISR_TMR1_IS) == 0x00) && (dev->errno != SC_ERR_CARD_REMOVED)) ;
        SC->ISR = SC_ISR_TMR1_IS;
        SC->PINCSR |= SC_PINCSR_CLK_KEEP;
        // I/O
        SC->PINCSR |= SC_PINCSR_SC_DATA_O;              // output HIGH
        // Reset
        //SMARTCARD_TimerCountSet(dev, SC_TIMER1, TIMER_DC, 108);       // 108*372 = 40176 clocks
        // [2011.11.24]
        /* EMV Certification: low clock cycles number(39814) from clk high to rst high (cold reset)  */
        SMARTCARD_TimerCountSet(dev, SC_TIMER1, SC_TMR_MODE_DC, 109);   // 108*372 = 40176 clocks
        SMARTCARD_TimerStart(dev, SC_TIMER1);
        while(((SC->ISR & SC_ISR_TMR1_IS) == 0x00) && (dev->errno != SC_ERR_CARD_REMOVED)) ;
        SC->ISR = SC_ISR_TMR1_IS;
        // enable Timer1 interrupt
        SC->IER |= SC_IER_TMR1_IE;
        SC->PINCSR |= SC_PINCSR_SC_RST;
#if 1 	// was out side
        dev->errno = 0;
        dev->op_state = SC_OP_ATR_READ;
#endif
        // wait 42036 clock for ATR
        SMARTCARD_TimerCountSet(dev, SC_TIMER0, SC_TMR_MODE_DC, (42000/372) + 13);
        SMARTCARD_TimerStart(dev, SC_TIMER0);
#else
        SC->ALTCTL |= SC_ALTCTL_ACT_EN;
#endif
}
/**
  * @brief  WarmReset sequence generator
  * @param  dev  The pointer of data structure of smartcard information
  * @return None
  */
void SMARTCARD_WarmResetCmd(S_SC_DEV_T *dev)
{
        SC_TypeDef *SC = dev->base;

#ifdef  SC_SEQ_MANUAL
        int32_t cardno = 0;
        if(dev->base==SC0)
                cardno = 0;
        else if(dev->base==SC1)
                cardno = 1;
        else if(dev->base==SC2)
                cardno = 2;

        // Wait for Tx & Rx get into idle mode
        while((!(SC->TRSR & SC_TRSR_TX_EMPTY_F)) || (SC->TRSR & SC_TRSR_TX_ATV)) ;
        while((!(SC->TRSR & SC_TRSR_RX_EMPTY_F)) || (SC->TRSR & SC_TRSR_RX_ATV)) ;
        // Reset
        while(sc_device[cardno].op_state != SC_OP_NOP) ;                // wait for idle mode
        SC->CTL |= SC_CTL_DIS_RX;                               // disable RX before RST falls
        SC->PINCSR &= ~SC_PINCSR_SC_RST;
        sc_device[cardno].errno = 0;
        SMARTCARD_TimerCountSet(dev, SC_TIMER1, SC_TMR_MODE_DC, (40000/(SC->ETUCR & SC_ETUCR_ETU_RDIV_MASK))+1);     // exceeds 40000 clocks
        SMARTCARD_TimerStart(dev, SC_TIMER1);
        while(sc_device[cardno].errno != SC_ERR_TIME1OUT && sc_device[cardno].errno != SC_ERR_CARD_REMOVED) ;
        SC->PINCSR |= SC_PINCSR_SC_RST;
        SC->CTL &= ~SC_CTL_DIS_RX;                              // enable RX after RST rises
#if 1 	// was out side
        dev->errno = 0;
        dev->op_state = SC_OP_ATR_READ;
#endif
        // wait 42036 clock for ATR
        SMARTCARD_TimerCountSet(dev, SC_TIMER0, SC_TMR_MODE_DC, (42000/372) + 13);
        SMARTCARD_TimerStart(dev, SC_TIMER0);
#else
        SC->ALTCTL |= SC_ALTCTL_WARST_EN;
#endif
}
/**
  * @brief   Set Guard Time
  * @param   dev  The pointer of data structure of smartcard information
  * @return  None
  */
static void SMARTCARD_SetGuardTime(S_SC_DEV_T *dev)
{
        SC_TypeDef *SC = dev->base;
        uint32_t gt = dev->CardCapabilities.GT;

        assert_param( !(gt<11 || gt>267) );

        /* [EMV 2000] EMV in T=0, minmun GT is 12 */
        if(dev->CardCapabilities.Protocol.Selected == SCARD_PROTOCOL_T0) {
                if(gt==11)
                        gt = 12;
                else {
                        SC->CTL &= ~SC_CTL_SLEN;        // 2 EUT stop bit
                        SC->EGTR = (SC->EGTR&~(0xFF)) | (gt-12);
                }
        }

        if(dev->CardCapabilities.Protocol.Selected == SCARD_PROTOCOL_T1) {
                if(gt==11) {
                        SC->CTL |= SC_CTL_SLEN; // 1 EUT stop bit
                        SC->EGTR = (SC->EGTR&~(0xFF));
                } else {
                        SC->CTL |= SC_CTL_SLEN; // 1 EUT stop bit
                        SC->EGTR = (SC->EGTR&~(0xFF)) | (gt-11);
                }
        }
}
/**
  * @brief  Set Block Guard Time
  * @param  dev  The pointer of data structure of smartcard information
  * @param  bgt  Block Guard time (ETU-based)
  * @return None
  */
static void SMARTCARD_SetBlockGuardTime(S_SC_DEV_T *dev, uint32_t bgt)
{
        SC_TypeDef *SC = dev->base;

        assert_param( !(bgt<1 || bgt>32) );

        SC->CTL = (SC->CTL & ~SC_CTL_BGT_MASK) | ((bgt - 1) << 8);
}

/**
  * @brief  Enable the checking of block guard time function
  * @param  dev  The pointer of data structure of smartcard information
  * @return None
  */
void SMARTCARD_StartBlockGuardTime( S_SC_DEV_T *dev )
{
        SC_TypeDef *SC = dev->base;
        SC->ALTCTL |= SC_ALTCTL_RX_BGT_EN;
}

/**
  * @brief   Stop the block guard time
  * @param   dev  The pointer of data structure of smartcard information
  * @return  None
  */
void SMARTCARD_StopBlockGuardTime( S_SC_DEV_T *dev )
{
        SC_TypeDef *SC = dev->base;
        SC->ALTCTL &= ~SC_ALTCTL_RX_BGT_EN;
}

/**
  * @brief   Check if Fi and Di is valid
  * @param   fi Fi
  * @param   di Di
  * @return  Parameter is valid or not
  * @retval  SC_STATUS_SUCCESS fi/di is valid
  * @retval  ATR_ERR_INVALID_PARAMETER is not support
  */
static int32_t SMARTCARD_MatchReader(int32_t fi, int32_t di)
{
        int32_t i;

        i = fi/di;

        if( (i % 31) == 0)
                return SC_STATUS_SUCCESS;
        else if( (i % 32) == 0)
                return SC_STATUS_SUCCESS;
        else
                return ATR_ERR_INVALID_PARAMETER;

        // TODO: Maybe do more things here...
        //...
}


/**
  * @brief  Set the baudrate according to clock rate conversion integer and baud rate adjustment integer
  * @param  dev  The pointer of data structure of smartcard information
  * @param  fi   The clock rate conversion integer from card
  * @param  di   The baud rate adjustment integer from card
  * @return None
  */
static void SMARTCARD_SetBaudrate(S_SC_DEV_T *dev, int32_t fi, int32_t di)
{
        uint32_t etudiv;
        /*double*/uint32_t fi_val, di_val,/* tmp, tmp2,*/ remainder;
        SC_TypeDef *SC = dev->base;

        fi_val = ClockRateConversion[fi].F;
        di_val = BitRateAdjustment[di].DNumerator;
        remainder = ((fi_val * 10) / di_val) % 10;
        //tmp = fi_val/di_val;
        //remainder = modf(tmp, &tmp2) * 10;

        // if the first decimal point of etu divider is greater than 5,
        // HW enable the compensation function
        if(remainder >= 5) {
                etudiv = (ClockRateConversion[fi].F / BitRateAdjustment[di].DNumerator);
                SC->ETUCR = SC_ETUCR_COMPEN_EN | etudiv;
        } else {
                etudiv = (ClockRateConversion[fi].F / BitRateAdjustment[di].DNumerator) - 1;
                SC->ETUCR = etudiv;
        }

        if(SC==SC0)
                g_SC0ETUDivider = etudiv;
        else if(SC==SC1)
                g_SC1ETUDivider = etudiv;
        else if(SC==SC2)
                g_SC2ETUDivider = etudiv;

}


/**
  * @brief  According ATR information, basic settings will be set the new value about ETU, clock stop level, guard time and block guard time.
  * @param  dev   The pointer of data structure of smartcard information
  * @return None
  */
void SMARTCARD_SetReaderParameter(S_SC_DEV_T *dev)
{
        /* set reader baud rate and update dev->clock , so we can calcute etu */
        ENTER();

        SMARTCARD_SetBaudrate(dev, dev->CardCapabilities.Fl, dev->CardCapabilities.Dl);

        // TODO: set GT, BGT, clock stop level or apply the new settings

        SMARTCARD_ClockStopLevel( dev );

        SMARTCARD_SetGuardTime( dev );

        if(dev->CardCapabilities.Protocol.Selected == SCARD_PROTOCOL_T0) {
                SMARTCARD_SetBlockGuardTime(dev, 16);   //BGT = 16
        } else if(dev->CardCapabilities.Protocol.Selected == SCARD_PROTOCOL_T1) {
                SMARTCARD_SetBlockGuardTime(dev, 22);   //BGT = 22
                /* set send-sequence & more-data bit */
                dev->T1.SSN = 1;        // default value
                dev->T1.RSN = 0;        // defalut value
                dev->T1.IBLOCK_REC = 0; // default value
        }

        /* Tx & Rx Error Retry Settings */
        /* [EMV 2000] */
        if(dev->CardCapabilities.Protocol.Selected == SCARD_PROTOCOL_T0) {
                SMARTCARD_SetTxRetry(dev, 4);           // Tx retry 4 times
                //SMARTCARD_SetRxRetry(dev, 8);         // Rx retry 8 times
                // [2011.11.25 night]
                /* EMV Certification */
                SMARTCARD_SetRxRetry(dev, 4);           // Rx retry 4 times
        }

        LEAVE();

}


/**
  * @brief  Reset the Tx/Rx FIFO & clock & initail default parameter
  * @param  dev  The pointer of data structure of smartcard information
  * @return None
  */
void SMARTCARD_ResetReader(S_SC_DEV_T *dev)
{
        SC_TypeDef *SC = dev->base;
        IRQn_Type SC_IRQn = (dev->base==SC0)?SC0_IRQn:((dev->base==SC1)?SC1_IRQn:SC2_IRQn);

        ENTER();

        // TODO: Set basic HW settings & protocol default value

        /* Software Reset Tx & Rx transition */
        SMARTCARD_ClearFIFO(dev);

        /* Rx FIFO threshold */
        //SC->CTL = (SC->CTL & ~SC_CTL_RX_FTRI_LEV_MASK) | SC_CTL_RX_FTRI_LEV_3BYTE;      // trigger level 3 bytes
        SC->CTL = (SC->CTL & ~SC_CTL_RX_FTRI_LEV_MASK) | SC_CTL_RX_FTRI_LEV_1BYTE;         // trigger level 1 bytes

        /* Rx Time-out setting */
        SC->RFTMR = 0;          // disable Rx FIFO time-out function

        /* Card detect de-bounce setting */
        SC->CTL &= ~SC_CTL_CD_DEB_SEL_MASK;

        /* in default, guard time = 12 etu */
        dev->CardCapabilities.GT = 12;
        SMARTCARD_SetGuardTime( dev );

        /* initial end sequence timing setting */
        SC->ALTCTL |= SC_ALTCTL_INIT_SEL_MASK;

        /* Tx & Rx Error Retry Settings */
        /* [ISO 7816-3] */
        //SMARTCARD_SetTxRetry(dev, 8);         // Tx retry 8 times
        //SMARTCARD_SetRxRetry(dev, 8);         // Rx retry 8 times

        /*
         * Tx & Rx Error Retry Settings
         * [EMV 2000]
         * EMV 2000 does NOT use ERROR retry function during ATR
         */
        SMARTCARD_SetTxRetry(dev, 0);           // Tx retry 0 times
        SMARTCARD_SetRxRetry(dev, 0);           // Rx retry 0 times

        // clear any error status
        // ...

        /* enable auto de-activation sequence */
#ifndef SC_SEQ_MANUAL
        SC->PINCSR |= SC_PINCSR_ADAC_CD_EN;     // auto de-activation when card removal
#endif

        /* enable auto convention */
        SC->CTL |= SC_CTL_AUTO_CON_EN;
        /* enable interrupt  */
        SC->IER |= (SC_IER_RDA_IE |
                SC_IER_TERR_IE |
                SC_IER_TMR0_IE |
                SC_IER_TMR1_IE |
                SC_IER_TMR2_IE |
                SC_IER_BGT_IE |
                SC_IER_CD_IE |
                SC_IER_INIT_IE |
                SC_IER_RTMR_IE |
                SC_IER_ACON_ERR_IE);


        NVIC_DisableIRQ(SC_IRQn);
        dev->errno = 0;
        /* enable SmartCard INT NVIC */
        NVIC_EnableIRQ(SC_IRQn);

        /* in defalut, all timer 24 or 8 or 8 turn on */
        SMARTCARD_TimerSelect(dev, SC_CTL_TMR_SEL_24_8_8_BITS);


        //dev->CardCapabilities.etu = 93;                       // for APDU test card
        //dev->CardCapabilities.GT = 12;                        // for APDU test card
        //dev->CardCapabilities.Protocol.Selected = SCARD_PROTOCOL_RAW; // for APDU test card

        /* Get SC clock */
        if( (CLK->CLKSEL2 & CLK_CLKSEL2_SC_MASK) == CLK_CLKSEL2_SC_HXT )  // form HXT
                dev->clock = (__XTAL/1000);
        //else if( (CLK->CLKSEL2 & CLK_CLKSEL2_SC_MASK) == CLK_CLKSEL2_SC_PLL )     // form PLL... why would anyone use PLL as clk source?
        //        dev->clock = CHECK_PLLOUT_CLK()/1000;
        else if( (CLK->CLKSEL2 & CLK_CLKSEL2_SC_MASK) >= CLK_CLKSEL2_SC_HIRC )    // form HIRC
                dev->clock = __IRC12M/1000;

        if(dev->base==SC0)
                dev->clock /= ((CLK->CLKDIV0 >> 28) + 1);
        else if(dev->base==SC1)
                dev->clock /= ((CLK->CLKDIV1 & CLK_CLKDIV1_SC1_MASK) + 1);
        else
                dev->clock /= ((CLK->CLKDIV1 >> 4) + 1);

        //printf("SC clock      = %d KHz \n", dev->clock);
        //dev->clock = 6000;    // 6 Mhz
        //dev->clock = 4000;    // 4 Mhz
        //dev->clock = 3375;    // 3.375 Mhz
        //dev->clock = 2000;    // 2 Mhz
        dev->CardCapabilities.Fl = 1;
        dev->CardCapabilities.Dl = 1;
        dev->CardCapabilities.N = 0;

        dev->CardCapabilities.T0.WI = 10;

        dev->CardCapabilities.T1.IFSC = 32;
        dev->CardCapabilities.T1.CWI = T1_CWI_DEFAULT;
        dev->CardCapabilities.T1.BWI = T1_BWI_DEFAULT;


        LEAVE();

}


/**
  * @brief   To do the activation sequence and start receiving and parsing ATR information.
  * @param   dev         The pointer of data structure of smartcard information
  * @param   resettype   Type of reset, ex: SC_COLDRESET, SC_WARMRESET
  * @return  The result status of check ATR information
  * @retval  0  Card avtivation success
  * @retval  < 0 Error occured during card activation
  */
static int32_t SMARTCARD_ActivateCard(S_SC_DEV_T *dev, int resettype)
{
        // remain is the remaining ATR length expecting, this value would be update at run time, offset stores distance to the next TD
        //int32_t remain = 2 /* TS & T0 */, offset = -1, tckflag = 0, length = 0;
        //uint8_t *atrbuffer;
        //SC_TypeDef *SC = dev->base;

        ENTER();

        SMARTCARD_ResetReader(dev);

        //SMARTCARD_PowerOn();

        // TODO: set the default ETU clock & Interrupt enable & close all timer
        SMARTCARD_SetBaudrate(dev, 1, 1);        // default 1 ETU == 372 cycles

        memset(&dev->CardCapabilities, 0, sizeof(SCARD_CARD_CAPABILITIES));
        memset(&dev->T0, 0, sizeof(T0_DATA));
        memset(&dev->T1, 0, sizeof(T1_DATA));

        // stop timer0-2
        SMARTCARD_TimerStop(dev, (E_SC_TIMER)(SC_TIMER0 | SC_TIMER1 |SC_TIMER2));


        /* reset the global variable for receiving ATR */
        if(dev->base==SC0) {
                atr_len0=0, atr_remain0=2/* TS & T0 */;
                ifbyte_flag0=-1/* interface bytes flag */, tck0=0, ifcount0=0;
                atr_time0 = ATR_TOTAL_TIME;
                atr_check_time0 = 0;
                atr_final_chk0 = 0;
                atr_total_time_start_flag0 = 0;
        } else if(dev->base==SC1) {
                atr_len1=0, atr_remain1=2/* TS & T0 */;
                ifbyte_flag1=-1/* interface bytes flag */, tck1=0, ifcount1=0;
                atr_time1 = ATR_TOTAL_TIME;
                atr_check_time1 = 0;
                atr_final_chk1 = 0;
                atr_total_time_start_flag1 = 0;
        } else if(dev->base==SC2) {
                atr_len2=0, atr_remain2=2/* TS & T0 */;
                ifbyte_flag2=-1/* interface bytes flag */, tck2=0, ifcount2=0;
                atr_time2 = ATR_TOTAL_TIME;
                atr_check_time2 = 0;
                atr_final_chk2 = 0;
                atr_total_time_start_flag2 = 0;
        }

        /* read ATR */
        dev->CardCapabilities.ATR.Length = 0;

#ifndef SC_SEQ_MANUAL
        // wait for 40000 clks after pull RST high
        /* [ISO 7816-3] */
        //SMARTCARD_TimerCountSet(dev, SC_TIMER0, TIMER_DC_INITIAL_END, (40000/372) + 1);
        //SMARTCARD_TimerStart(dev, SC_TIMER0);
        // wait for 42000 clks after pull RST high
        /* [EMV 2000] */
        SMARTCARD_TimerCountSet(dev, SC_TIMER0, TIMER_DC_INITIAL_END, (42000/372) + 1);
        SMARTCARD_TimerStart(dev, SC_TIMER0);
#endif

        // TODO: Do the activation or warm-reset sequence
        if(resettype == SC_COLDRESET)
                SMARTCARD_ActivationCmd( dev );
        else if(resettype == SC_WARMRESET)
                SMARTCARD_WarmResetCmd( dev );
#if 0	 // move into  SMARTCARD_ActivationCmd() & SMARTCARD_WarmResetCmd(). out them here may be too late
        // start IST to receive ATR
        dev->errno = 0;
        dev->op_state = SC_OP_ATR_READ;
#endif
        // TODO: wait the ISR to receive ATR done.
        while(dev->op_state==SC_OP_ATR_READ ) {
                // [2011.11.28]
                /* EMV Certification */
                if(dev->errno == SC_ERR_TIME2OUT || dev->errno == SC_ERR_TIME0OUT)  // 2012-04-12 Add time0out avoid user insert card in wrong direction  --ya
                        break;
        }
        if(dev->errno < 0) {
                // [2011.11.28]
                /* EMV Certification */
                if(dev->errno == SC_ERR_TIME2OUT ||
                                dev->errno == SC_ERR_TIME0OUT ||
                                (dev->base->PINCSR & SC_PINCSR_CD_PIN_ST) == 0)  // 2012-04-12 Add time0out avoid user insert card in wrong direction  --ya
                        SMARTCARD_DeactivationCmd(dev);
                printf("Failed to read ATR..Error Msg:%s \n", SMARTCARD_ERRMSG(dev->errno));
        } else

#ifdef SC_ATR_DEBUG
        {
                int32_t j;
                printf("ATR : Length : %d\n", dev->CardCapabilities.ATR.Length);
                for( j = 0; j < dev->CardCapabilities.ATR.Length; j ++) {
                        printf("%02x ", dev->CardCapabilities.ATR.Buffer[j]);
                }
                printf("\n");
        }
#endif

        LEAVE();

        return dev->errno;
}


/**
  * @brief  Parse ATR characters and check its integrity
  * @param  dev  The pointer of data structure of smartcard information
  * @param  resettype Indicate reset type. Could be ether SC_COLDRESET or SC_WARMRESET
  * @return The result status of parsing process
  * @retval SC_ERR_UNSUPPORTEDCARD      Card is not supported
  * @retval SC_STATUS_SUCCESS           Success
  * @retval ATR_ERR_UNRECOGNIZED_MEDIA  Invalid to the length of ATR or TS byte or ATR checksum or Fi/Di
  * @retval ATR_ERR_INVALID_PARAMETER   Content of ATR has wrong parameter
  */
static int32_t SMARTCARD_ParseATR(S_SC_DEV_T * dev, int resettype)
{
        int32_t i, retval;
        uint8_t td;
        uint8_t *atr = dev->CardCapabilities.ATR.Buffer;
        uint8_t /*N,*/ T, Fi, Di;
        unsigned long protocolTypes = 0;

        struct {
                uint8_t data;
                uint8_t present;
        } ibyte[4][4];

        ENTER();

        memset(&ibyte, 0, sizeof(ibyte));

        atr++;  // skip: TS->T0
        td = *atr++;

        i = 0;

        while(1) {

                /* TAi present */
                if( td & 0x10) {
                        ibyte[i][ATR_INTERFACE_BYTE_TA].data = *atr++;
                        ibyte[i][ATR_INTERFACE_BYTE_TA].present = 1;
                }

                /* TBi present */
                if( td & 0x20) {
                        ibyte[i][ATR_INTERFACE_BYTE_TB].data = *atr++;
                        ibyte[i][ATR_INTERFACE_BYTE_TB].present = 1;
                }

                /* TCi present */
                if( td & 0x40) {
                        ibyte[i][ATR_INTERFACE_BYTE_TC].data = *atr++;
                        ibyte[i][ATR_INTERFACE_BYTE_TC].present = 1;
                }

                /* TDi present */
                if( td & 0x80) {
                        ibyte[i][ATR_INTERFACE_BYTE_TD].data = *atr++;
                        ibyte[i][ATR_INTERFACE_BYTE_TD].present = 1;
                        td = ibyte[i][ATR_INTERFACE_BYTE_TD].data;
                        protocolTypes |= 1 << (td & 0x0F);
                        i++;
                } else
                        break;

        }


#if EMV_ATR_CHECK
        /**********************************************************************************************************************/
        /* Checking the integrity of ATR, this process meets EMV 4.2 specification */
        /* Reject ATR if TA1 is not int the range '11' to '13' */
        if(ibyte[0][ATR_INTERFACE_BYTE_TA].present == 1) {
                if(ibyte[0][ATR_INTERFACE_BYTE_TA].data < 0x11 || ibyte[0][ATR_INTERFACE_BYTE_TA].data > 0x13)
                        return ATR_ERR_INVALID_PARAMETER;
        }

        /* In response to the cold-reset, TB1 only could be 0x00 */
        if(resettype == SC_COLDRESET) {
                if(ibyte[0][ATR_INTERFACE_BYTE_TB].present == 1)
                        if(ibyte[0][ATR_INTERFACE_BYTE_TB].data != 0x00)
                                return ATR_ERR_INVALID_PARAMETER;
                if(ibyte[0][ATR_INTERFACE_BYTE_TB].present == 0)
                        return ATR_ERR_INVALID_PARAMETER;
        }

        /* Reject ATR containing TB2 */
        if(ibyte[1][ATR_INTERFACE_BYTE_TB].present == 1)
                return ATR_ERR_INVALID_PARAMETER;


        /* ATR must contain TB3 in T=1 */
        if(dev->CardCapabilities.Protocol.Selected == SCARD_PROTOCOL_T1) {
                if(ibyte[2][ATR_INTERFACE_BYTE_TB].present == 0)
                        return ATR_ERR_INVALID_PARAMETER;
        }

        /* Bit [5] of TA2 must be equal to 0x0 */
        if(ibyte[1][ATR_INTERFACE_BYTE_TA].present == 1) {
                if((ibyte[1][ATR_INTERFACE_BYTE_TA].data&0x10) == 0x10)
                        return ATR_ERR_INVALID_PARAMETER;
        }

        /* Reject an ATR that TC2 is equal to 0x00 */
        if(ibyte[1][ATR_INTERFACE_BYTE_TC].present == 1 && ibyte[1][ATR_INTERFACE_BYTE_TC].data == 0x00)
                return ATR_ERR_INVALID_PARAMETER;


        /* TD1's l.s. nibble must be 0x0 or 0x1 */
        if(ibyte[0][ATR_INTERFACE_BYTE_TD].present == 1) {
                if((ibyte[0][ATR_INTERFACE_BYTE_TD].data&0xF) > 0x1) {
                        return ATR_ERR_INVALID_PARAMETER;
                }
        }

        /* TD2's l.s. nibble must be 0x1 or 0xE if TD1's l.s. nibble is 0x0 */
        if(ibyte[1][ATR_INTERFACE_BYTE_TD].present == 1) {
                if((ibyte[1][ATR_INTERFACE_BYTE_TD].data&0xF)!=0x1 && (ibyte[1][ATR_INTERFACE_BYTE_TD].data&0xF)!=0xE)
                        return ATR_ERR_INVALID_PARAMETER;

                if((ibyte[1][ATR_INTERFACE_BYTE_TD].data&0xF) == 0xE) {
                        if((ibyte[0][ATR_INTERFACE_BYTE_TD].data&0xF) != 0x0)
                                return ATR_ERR_INVALID_PARAMETER;
                }
        }

        /* Reject TA3 having a value in the range 0x0~0xF or 0xFF */
        if(ibyte[2][ATR_INTERFACE_BYTE_TA].present == 1) {
                if(ibyte[2][ATR_INTERFACE_BYTE_TA].data >= 0x0 &&
                                ibyte[2][ATR_INTERFACE_BYTE_TA].data <= 0xF) {
                        return ATR_ERR_INVALID_PARAMETER;
                }
                if(ibyte[2][ATR_INTERFACE_BYTE_TA].data == 0xFF) {
                        return ATR_ERR_INVALID_PARAMETER;
                }
        }

        /* Reject ATR not containing TB3 or BWI greater than 4 or CWI greater than 5 */
        /* And reject ATR if fitting the formula : 2 to the power of CWI is equal or less than (N+1) */
        if(dev->CardCapabilities.Protocol.Selected == SCARD_PROTOCOL_T1) {
                if(ibyte[2][ATR_INTERFACE_BYTE_TB].present == 1) {
                        if(((ibyte[2][ATR_INTERFACE_BYTE_TB].data&0xF0)>>4) > 0x4)
                                return ATR_ERR_INVALID_PARAMETER;

                        if((ibyte[2][ATR_INTERFACE_BYTE_TB].data&0xF) > 0x5)
                                return ATR_ERR_INVALID_PARAMETER;

                        i = 1;
                        retval = (ibyte[2][ATR_INTERFACE_BYTE_TB].data&0xF);
                        while(retval--)
                                i *= 2;
                        /* if TC1 is equal to 0xFF, N as -1 that is always valid */
                        if(ibyte[0][ATR_INTERFACE_BYTE_TC].data!=0xFF)
                                if( i <= (ibyte[0][ATR_INTERFACE_BYTE_TC].data+1))
                                        return ATR_ERR_INVALID_PARAMETER;

                } else return ATR_ERR_INVALID_PARAMETER;
        }

        /* Reject ATR if TC3 is not equal to 0x00 */
        if(ibyte[2][ATR_INTERFACE_BYTE_TC].present == 1) {
                if(ibyte[2][ATR_INTERFACE_BYTE_TC].data != 0x00) {
                        return ATR_ERR_INVALID_PARAMETER;
                }
        }
        /* End of checking the integrity of ATR */
        /**********************************************************************************************************************/
#endif

        Fi = Di = 1;

        /* set Fi and Di if TA1 present */
        if(ibyte[0][ATR_INTERFACE_BYTE_TA].present) {
                Fi = (ibyte[0][ATR_INTERFACE_BYTE_TA].data >> 4) & 0x0f;
                Di = ibyte[0][ATR_INTERFACE_BYTE_TA].data  & 0x0f;
        }

        //N = 0;

        //if(ibyte[0][ATR_INTERFACE_BYTE_TC].present)
        //      N = ibyte[0][ATR_INTERFACE_BYTE_TC].data;

        T = 0;

        /* check TA2 indicates which mode in the card */
        /* in specific mode */
        if(dev->CurrentState == SCARD_SPECIFIC) {
                T = ibyte[1][ATR_INTERFACE_BYTE_TA].data & 0x0f;

                if(ibyte[1][ATR_INTERFACE_BYTE_TA].data & 0x10) { /* Bit5 = 1, using default value */
                        Fi = 1;
                        Di= 1;          /* default value */
                }

                if(SMARTCARD_MatchReader(ClockRateConversion[Fi].F, BitRateAdjustment[Di].DNumerator) == 0) {   /* reader supported mode? */
                        /* do nothing, */
                        /* Match, reader is ready to work */
                        ;
                } else {
                        if(!(ibyte[1][ATR_INTERFACE_BYTE_TA].data & 0x80)) {    /* Bit8 indicate if being capable to change mode */
                                retval = SMARTCARD_WarmReset(dev);      /* change to negotiable mode */
                                if(retval != 0)
                                        return retval;
                                //Fi = 1; Di = 1; T = 0;
                                //retval = SMARTCARD_DoPPS(dev, Fi, Di, T);     /* parameter and protocol select */
                                //if(retval != 0)
                                //      return retval;
                        }
                        //else          // mask for APDU test card
                        //      return SC_ERR_UNSUPPORTEDCARD;
                }

        }

        /* in negotiable mode */
        else if(dev->CurrentState == SCARD_NEGOTIABLE) {
                if (ibyte[0][ATR_INTERFACE_BYTE_TD].present)     // use "first offered transmission protocol"
                        T = ibyte[0][ATR_INTERFACE_BYTE_TD].data & 0x0f;
                else
                        T = 0;          // means protocol T=0

                /* if the values are the default, no PPS should be done */
                if( (Fi==0) && (Di==1) ) {
                        if(SMARTCARD_MatchReader(ClockRateConversion[Fi].F, BitRateAdjustment[Di].DNumerator) == 0) {   /* reader supported mode? */
                                /* do nothing, */
                                /* Match, reader is ready to work */
                                ;
                        } else return SC_ERR_UNSUPPORTEDCARD;
                } else {

                        retval = SMARTCARD_DoPPS(dev, Fi, Di, T);               /* parameter and protocol select */
                        if(retval < 0) {
                                /* PPS failed */
                                printf("DoPPS is failed!!\n");
                                if(retval == SC_ERR_CARD_REMOVED)
                                        return(SC_ERR_CARD_REMOVED);
                        } else {
                                dev->pps_complete = 1;
                                SmartcardUpdateCardCapabilities(dev);
                        }

                }

        }

        SMARTCARD_SetReaderParameter(dev);


        LEAVE();

        return SC_STATUS_SUCCESS;
}

/**
  * @brief   The Cold-reset process that makes activation sequence and gets ATR.
  * @param   dev  The pointer of data structure of smartcard information
  * @return  The result status of check procedure
  * @retval  SC_STATUS_SUCCESS           Success
  * @retval  ATR_ERR_UNRECOGNIZED_MEDIA  Invalid to the length of ATR or TS byte or ATR checksum or Fi/Di
  * @retval  SC_ERR_UNSUPPORTEDCARD      Card is not supported
  * @retval  ATR_ERR_INVALID_PARAMETER   Content of ATR has wrong parameter
  * @retval  ATR_ERR_INVALID_TCK         Checksum of ATR is wrong
  * @retval  SC_ERR_AUTOCONVENTION       TS byte is wrong
  * @retval  SC_ERR_TIME0OUT             Time out
  * @retval  SC_ERR_READ                 Parity error or others during reading ATR
  */
int32_t SMARTCARD_ColdReset(S_SC_DEV_T *dev)
{
        ENTER();

        //SMARTCARD_DeactivationCmd(dev);          // de-activation
        //SMARTCARD_PowerOff(dev);

        SMARTCARD_VoltageConfig(dev);
        dev->errno = SMARTCARD_ActivateCard(dev, SC_COLDRESET);

        if(dev->errno != 0)
                return (dev->errno);

        dev->pps_complete = 0;
        dev->errno = SmartcardUpdateCardCapabilities(dev);

        if(dev->errno != 0)
                return (dev->errno);

        dev->errno = SMARTCARD_ParseATR(dev, SC_COLDRESET);

        if(dev->errno != 0)
                printf("SMARTCARD_ColdReset:: ParseATR..Error Msg:%s \n", SMARTCARD_ERRMSG(dev->errno));

        dev->openflag = 1;

        LEAVE();
        return (dev->errno);
}


/**
  * @brief  The Warm-reset process that makes warm-reset sequence and gets ATR.
  * @param  dev  The pointer of data structure of smartcard information
  * @return The result status of check procedure
  * @retval SC_STATUS_SUCCESS            Success
  * @retval ATR_ERR_UNRECOGNIZED_MEDIA   Invalid to the length of ATR or TS byte or ATR checksum or Fi/Di
  * @retval SC_ERR_UNSUPPORTEDCARD       Card is not supported
  * @retval ATR_ERR_INVALID_PARAMETER    Content of ATR has wrong parameter
  * @retval ATR_ERR_INVALID_TCK          Checksum of ATR is wrong
  * @retval SC_ERR_AUTOCONVENTION        TS byte is wrong
  * @retval SC_ERR_TIME0OUT              Time out
  * @retval SC_ERR_READ                  Parity error or others during reading ATR
  */
int32_t SMARTCARD_WarmReset(S_SC_DEV_T *dev)
{

        ENTER();

        dev->errno = SMARTCARD_ActivateCard(dev, SC_WARMRESET);

        if(dev->errno != 0)
                return (dev->errno);

        dev->pps_complete = 0;
        dev->errno = SmartcardUpdateCardCapabilities(dev);

        if(dev->errno != 0)
                return (dev->errno);

        dev->errno = SMARTCARD_ParseATR(dev, SC_WARMRESET);

        if(dev->errno != 0)
                printf("SMARTCARD_WarmReset:: ParseATR..Error Msg:%s \n", SMARTCARD_ERRMSG(dev->errno));

        LEAVE();
        return (dev->errno);

}

/**
  * @brief  The selection of transmission parameters and protocol
  * @param  dev  The pointer of data structure of smartcard information
  * @param  Fi   Clock rate conversion interger from card
  * @param  Di   Baud rate adjustment integer from card
  * @param  T    Transmission protocol
  * @return The result of processes. Status has many situations in writing/reading procedure.
  *
  */
int32_t SMARTCARD_DoPPS(S_SC_DEV_T *dev, int32_t Fi, int32_t Di, int32_t T)
{
        uint8_t buf[4];
        uint8_t *rbuf;
        int32_t len = 4, rPck=0;
        SC_TypeDef *SC = dev->base;

        ENTER();

        buf[0] = 0xff;                                                           /* PPSS: PPS request use 0xFF */
        buf[1] = 0x10 |(T & 0x0f);                                      /* PPS0: PPS1 will be transmited + T transmission protocol */
        buf[2] = ((Fi<<4)&0xf0) | (Di & 0x0f);     /* PPS1: */
        buf[3] = (buf[0] ^ buf[1] ^ buf[2]);            /* PCK */

        SMARTCARD_ClearFIFO(dev);  // clear FIFO

        printf("Sending PPS : %02x %02x %02x %02x\n",
               buf[0], buf[1], buf[2], buf[3]);

        SMARTCARD_TimerStop(dev, (E_SC_TIMER)(SC_TIMER0 | SC_TIMER1 |SC_TIMER2));   // stop timer0-2


        // Waiting time check
        // the waiting time of PPS is fixed at 9600 ETUs
        SMARTCARD_TimerCountSet(dev, SC_TIMER0, SC_TMR_MODE_DC_RELOAD_S_S, 9600);
        SMARTCARD_TimerStart(dev, SC_TIMER0);


        /* Start to do PPS exchange */
        dev->errno = 0;
        dev->snd_buf = buf;
        dev->snd_len = len;
        dev->snd_pos = 0;
        dev->op_state = SC_OP_WRITE;
        SC->IER  |= SC_IER_TBE_IE;
        while(dev->op_state == SC_OP_WRITE && !(dev->errno));
        dev->snd_buf = NULL;

        if(dev->errno != 0) {
                printf("SMARTCARD_DoPPS:: PPS Write..Error Msg:%s \n", SMARTCARD_ERRMSG(dev->errno));
                return dev->errno;
        }

        /* Read Procedure bytes */
        dev->errno = 0;
        rbuf = dev->rcv_buf;
        dev->rcv_pos = 0;
        dev->rcv_len = len;
        dev->rcv_cnt = 0;
        dev->op_state = SC_OP_READ;
        while(dev->op_state == SC_OP_READ && (dev->bCardRemoved == SCARD_PRESENT) && !(dev->errno));

        SMARTCARD_TimerStop(dev, SC_TIMER0);    // Stop timer

        if(dev->errno != 0) {
                printf("SMARTCARD_DoPPS:: PPS Read..Error Msg:%s \n", SMARTCARD_ERRMSG(dev->errno));
                return dev->errno;
        }

        printf("Received PPS : %02x %02x %02x %02x\n",
               rbuf[0], rbuf[1], rbuf[2], rbuf[3]);

        if(rbuf[0] != buf[0])   /* PPSS */
                return PROTOCOL_PPS_INVALID;

        rPck = rbuf[0];
        if( (rbuf[1]&0x0f) == (buf[1] &0x0f) &&         /* success */  /* PPS0 */
                        ((rbuf[1] & 0xf0) == 0x10 ||(rbuf[1] & 0xf0) == 0x00)) {
                rPck ^= rbuf[1];

                printf("PPS Request Success, \n"/*now baudrate is %d\n", 9600 * Dtab[Di]*/);
        } else
                return PROTOCOL_PPS_INVALID;

        if (rbuf[2] == buf[2])  /* PPS1 */
                rPck ^= rbuf[2];
        else
                return PROTOCOL_PPS_INVALID;

        if (rbuf[3] != rPck)  /* PCK */
                return PROTOCOL_PPS_INVALID;


        LEAVE();
        return SC_STATUS_SUCCESS;
}


/**
  * @brief  ATR information, which be read after activation process,
  * @param  portno  Indicate which port to be used (SC0, SC1, SC2)
  * @param  buf     The pointer of received buffer by application
  * @param  length  The data length we needed to read
  * @return The data length of received ATR buffer
  */
int32_t SMARTCARD_GetATRBuffer(SC_TypeDef *portno, uint8_t *buf, int16_t length)
{
        S_SC_DEV_T *dev;
        int32_t port = 0;

        assert_param(CHECK_SC_PORT(portno));

        if(portno==SC0)
                port = 0;
        else if(portno==SC1)
                port = 1;
        else if(portno==SC2)
                port = 2;

        dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[port]);

        if(dev->openflag == 0)
                return(/*SC_EIO*/ 0);    // upper layer could get confused between length and error code, so return 0 instead.

        memcpy(buf, dev->CardCapabilities.ATR.Buffer,
               (length<dev->CardCapabilities.ATR.Length)?length:dev->CardCapabilities.ATR.Length);

        return dev->CardCapabilities.ATR.Length;
}


/**
  * @brief   Transmit the packet according to protocol T=0 or T=1
  * @param   portno     Indicate which port to be used (SC0, SC1, SC2)
  * @param   buf        The pointer of transmitted buffer
  * @param   len        The length of transmitted buffer
  * @param   outbuf     The pointer of received buffer
  * @param   outlen     The length of received data
  * @return  The result of processes. Status has many situations in writing/reading procedure.
  * @retval  SC_STATUS_SUCCESS Success
  * @retval  SC_EBUSY Smart card interface is doing something
  * @retval  SC_EIO Card removed or unsupported card
  * @retval  Other The error code of transmission and receiver.
  */
int32_t SMARTCARD_Write(SC_TypeDef *portno, uint8_t *buf, uint32_t len, uint8_t *outbuf, uint32_t *outlen )
{
        S_SC_DEV_T *dev;
        int32_t ret = SC_STATUS_SUCCESS, port = 0;

        ENTER();

        assert_param(CHECK_SC_PORT(portno));
        assert_param( len!=0 );

        if(portno==SC0)
                port = 0;
        else if(portno==SC1)
                port = 1;
        else if(portno==SC2)
                port = 2;

        dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[port]);

        if(dev->op_state)       // card busy
                return SC_EBUSY;

        if(SMARTCARD_CheckCardPresent(portno) != SCARD_PRESENT)
                return SC_EIO;

        if(dev->openflag == 0)
                return(SC_EIO);

        if(len > MAX_BUF_LEN)
                len = MAX_BUF_LEN;

        // Call write means we give up data remain in dev->buf
        dev->snd_pos = 0;
        dev->rcv_pos = 0;

        if(portno == SC0)
                memset(&g_Port0_RxBUF[0], 0x00, sizeof(g_Port0_RxBUF));
        else if(portno == SC1)
                memset(&g_Port1_RxBUF[0], 0x00, sizeof(g_Port1_RxBUF));
        else if(portno == SC2)
                memset(&g_Port2_RxBUF[0], 0x00, sizeof(g_Port2_RxBUF));

        //      dispatch on the selected protocol
        switch( dev->CardCapabilities.Protocol.Selected ) {
        case SCARD_PROTOCOL_T0:
                ret = CBT0Transmit( dev, buf, len, outbuf, outlen );
                break;

        case SCARD_PROTOCOL_T1:
                if(portno == SC0) {
                        memset(g_Port0_TxBUF, 0x00, sizeof(g_Port0_TxBUF));
                        dev->snd_buf = &g_Port0_TxBUF[0];
                        dev->rcv_buf = &g_Port0_RxBUF[0];
                } else if(portno == SC1) {
                        memset(g_Port1_TxBUF, 0x00, sizeof(g_Port1_TxBUF));
                        dev->snd_buf = &g_Port1_TxBUF[0];
                        dev->rcv_buf = &g_Port1_RxBUF[0];
                } else if(portno == SC2) {
                        memset(g_Port2_TxBUF, 0x00, sizeof(g_Port2_TxBUF));
                        dev->snd_buf = &g_Port2_TxBUF[0];
                        dev->rcv_buf = &g_Port2_RxBUF[0];
                }

                ret = CBT1Transmit( dev, buf, len, outbuf, outlen );
                break;

        case SCARD_PROTOCOL_RAW:
                //ret = CBRawTransmit( dev, buf, len, len, outbuf, outlen);
                break;

        default:
                ret = SC_EPROTOCOL;
                break;
        }

        if(ret != SC_STATUS_SUCCESS) {
                /* stop all */
                SMARTCARD_StopBlockGuardTime( dev );            // stop block guard time
                SMARTCARD_TimerStop(dev, (E_SC_TIMER)(SC_TIMER0 | SC_TIMER1 |SC_TIMER2));   // stop timer0-2
                printf("Failed to Transfer T=0..Error Msg:%s \n", SMARTCARD_ERRMSG(ret));
        }

        LEAVE();

        return ret;

}


/**
  * @brief  This function could activate the smartcard device and use the appropriate the voltage.
  *         If any procedure is failed, do de-activation to end the process.
  * @param  portno  Indicate which port to be used (SC0, SC1, SC2)
  * @return Success or not
  * @retval SC_STATUS_SUCCESS           Successful
  * @retval SC_EBUSY                    Interface already opened
  * @retval SC_EIO                      Card removed or unsupported card
  * @retval ATR_ERR_INVALID_PARAMETER   Content of ATR has wrong parameter
  * @retval ATR_ERR_INVALID_TCK         Checksum of ATR is wrong
  * @retval SC_ERR_AUTOCONVENTION       TS byte is wrong
  * @retval SC_ERR_TIME0OUT             Time out
  * @retval SC_ERR_READ                 Parity error or others during reading ATR
  */
int32_t SMARTCARD_Open(SC_TypeDef *portno)
{
        int32_t retval = SC_STATUS_SUCCESS, i, port = 0;
        S_SC_DEV_T *dev;
        SC_TypeDef *SC;
        //IRQn_Type SC_IRQn;

        ENTER();

        assert_param(CHECK_SC_PORT(portno));

        if(portno==SC0)
                port = 0;
        else if(portno==SC1)
                port = 1;
        else if(portno==SC2)
                port = 2;

        dev = (S_SC_DEV_T *)((uint32_t)&sc_device[port]);

        if( dev->openflag != 0 )                /* a card slot can open only once */
                return(SC_EBUSY);

        dev->base = portno;

        SC = portno;
        //SC_IRQn = (portno==0)?SC0_IRQn:SC0_IRQn;

        SMARTCARD_ResetReader(dev);             // initiate some parameters

        /* point to receive buffer */
        if(portno == SC0) {
                dev->rcv_buf = (uint8_t *)&g_Port0_RxBUF;//(uint8_t *)malloc(MAX_BUF_LEN * sizeof(uint8_t));
                dev->snd_buf = (uint8_t *)&g_Port0_TxBUF;
        } else if(portno == SC1) {
                dev->rcv_buf = (uint8_t *)&g_Port1_RxBUF;//(uint8_t *)malloc(MAX_BUF_LEN * sizeof(uint8_t));
                dev->snd_buf = (uint8_t *)&g_Port1_TxBUF;
        } else if(portno == SC2) {
                dev->rcv_buf = (uint8_t *)&g_Port2_RxBUF;//(uint8_t *)malloc(MAX_BUF_LEN * sizeof(uint8_t));
                dev->snd_buf = (uint8_t *)&g_Port2_TxBUF;
        }

        if((dev->bCardRemoved==SCARD_ABSENT) || (dev->bCardRemoved==SCARD_UNKNOWN)) {
                printf("Card missing\n");
                retval = SC_EIO;
                goto _exit_open;
        } else {
                /* default: 1.8V start */
                dev->voltage = SC_ISO_OPERATIONCLASS_C;

                for( i = 0; i < 1/*3*/; i++) {
                        if((retval = SMARTCARD_ColdReset(dev)) != 0) {
                                if(i==1)
                                        dev->voltage = SC_ISO_OPERATIONCLASS_B;  // 1.8V failed, use 3V
                                if(i==2)
                                        dev->voltage = SC_ISO_OPERATIONCLASS_A;  // 3V failed, use 5V
                                continue;
                        }
                        break;
                }

                if(retval != 0) {
                        // TODO: disable Group mask
                        printf("Cold-Reset is failed...\n");
                        SC->IER = SC_IER_CD_IE;         // Keep the card detection interrupt. Modified by SM

                }
        }

_exit_open:
        dev->openflag = 1;
        dev->errno = 0;  // clear error code happened during cold reset

        printf("Open Done\n");

        LEAVE();

        return retval;

}


/**
  * @brief  Disable interrupt and do de-activation
  * @param  portno  Indicate which port to be used (SC0, SC1, SC2)
  * @return This function always return success
  * @retval SC_STATUS_SUCCESS  Success
  */
int32_t SMARTCARD_Close(SC_TypeDef *portno)
{
        S_SC_DEV_T *dev;
        IRQn_Type SC_IRQn;
        SC_TypeDef *SC;
        int32_t port = 0;

        ENTER();

        assert_param(CHECK_SC_PORT(portno));

        SC_IRQn = (portno==SC0)?SC0_IRQn:((portno==SC1)?SC1_IRQn:SC2_IRQn);

        SC = portno;
        if(portno==SC0)
                port = 0;
        else if(portno==SC1)
                port = 1;
        else if(portno==SC2)
                port = 2;

        dev = (S_SC_DEV_T *)((uint32_t)&sc_device[port]);
        //if(dev->openflag == 0)
        //      return(SC_EIO);

        // TODO: disable group mask

        /* Disable NVIC SmartCard interrupt */
        NVIC_DisableIRQ(SC_IRQn);

        SMARTCARD_DeactivationCmd( dev );          // de-activation
        //SMARTCARD_PowerOff();

        SC->IER = 0;            // disable all interrupt

        //free(dev->rcv_buf);

        dev->openflag = 0;

        LEAVE();

        return SC_STATUS_SUCCESS;
}



/**
  * @brief   nitialization process configures smartcard port and enables its clock and engine clock
  * @param   portno  Indicate which port to be used (SC0, SC1, SC2)
  * @param   edge    Indicate card detect polatiry. 1 means rising edge, 0 means falling edge.
  * @return  None
  */
void SMARTCARD_Init(SC_TypeDef *portno, int8_t edge)
{
        S_SC_DEV_T *dev;
        SC_TypeDef *SC;
        IRQn_Type SC_IRQn;
        int32_t port = 0;

        assert_param(CHECK_SC_PORT(portno));

        SC = portno;
        SC_IRQn = (portno==SC0)?SC0_IRQn:((portno==SC1)?SC1_IRQn:SC2_IRQn);

        if(portno==SC0)
                port = 0;
        else if(portno==SC1)
                port = 1;
        else if(portno==SC2)
                port = 2;


        dev = (S_SC_DEV_T *)((uint32_t)&sc_device[port]);
        memset(dev, 0, sizeof(S_SC_DEV_T));
        dev->base = portno;     //ya

        if(portno==SC0)
                CLK->APBCLK |= CLK_APBCLK_SC0_EN;
        else if(portno==SC1)
                CLK->APBCLK |= CLK_APBCLK_SC1_EN;
        else if(portno==SC2)
                CLK->APBCLK |= CLK_APBCLK_SC2_EN;

        /* Card is Absent in default */
        dev->bCardRemoved = SCARD_ABSENT;

        if(edge)
                portno->PINCSR |= SC_PINCSR_CD_LEV;             // this must be set before enable engine
        else
                portno->PINCSR &= ~SC_PINCSR_CD_LEV;

        // enable smartcard engine
        SC->CTL |= SC_CTL_SC_CEN;

        /* enable SmartCard INT NVIC */
        NVIC_EnableIRQ(SC_IRQn);

        // enable card detection interrupt
        SC->IER |= SC_IER_CD_IE;

        printf("\n[%s] : SC%d Init...\n", __func__, port);


}

/**
  * @brief   DeInitialization a smartcard interface
  * @param   portno  Indicate which port to be DeInit (SC0, SC1, SC2)
  * @return  None
  */
void SMARTCARD_DeInit(SC_TypeDef *portno)
{
        IRQn_Type SC_IRQn = (portno == SC0) ? SC0_IRQn : ((portno == SC1) ? SC1_IRQn : SC2_IRQn);
        if(portno==SC0)
                CLK->APBCLK &= ~CLK_APBCLK_SC0_EN;
        else if(portno==SC1)
                CLK->APBCLK &= ~CLK_APBCLK_SC1_EN;
        else if(portno==SC2)
                CLK->APBCLK &= ~CLK_APBCLK_SC2_EN;

        NVIC_DisableIRQ(SC_IRQn);
}

/**
  * @brief  Enable Timer Interrupt
  * @param  dev  The pointer of data structure of smartcard information
  * @param  no   Timer number. (SC_TIMER0, SC_TIMER1, SC_TIMER2)
  * @return None
  */
void SMARTCARD_TimerINTEnable(S_SC_DEV_T *dev, E_SC_TIMER no)
{
        SC_TypeDef *SC = dev->base;
        if( no & SC_TIMER0) SC->IER |= SC_IER_TMR0_IE;
        if( no & SC_TIMER1) SC->IER |= SC_IER_TMR1_IE;
        if( no & SC_TIMER2) SC->IER |= SC_IER_TMR2_IE;
}

/**
  * @brief   Disable Timer Interrupt
  * @param   dev     The pointer of data structure of smartcard information
  * @param   no      Tmer number. (SC_TIMER0, SC_TIMER1, SC_TIMER2)
  * @return  None
  */
void SMARTCARD_TimerINTDisable(S_SC_DEV_T *dev, E_SC_TIMER no)
{
        SC_TypeDef *SC = dev->base;
        if( no & SC_TIMER0) SC->IER &= ~SC_IER_TMR0_IE;
        if( no & SC_TIMER1) SC->IER &= ~SC_IER_TMR1_IE;
        if( no & SC_TIMER2) SC->IER &= ~SC_IER_TMR2_IE;
}

/**
  * @brief  Select which type of versatile timer, 24-bit, 24-8-bit or 24-8-8-bit
  * @param  dev     The pointer of data structure of smartcard information
  * @param  type    Could be SC_TMR_24_Bit, SC_TMR_24_8_Bit, SC_TMR_24_8_8_Bit
  * @return None
  */
void SMARTCARD_TimerSelect(S_SC_DEV_T *dev, uint32_t type)
{
        SC_TypeDef *SC = dev->base;
        SC->CTL = (SC->CTL & ~SC_CTL_TMR_SEL_MASK) | type;
}

/**
  * @brief  Enable Timer starting, counter will count when condition match
  * @param  dev   The pointer of data structure of smartcard information
  * @param  no    Timer number. (SC_TIMER0, SC_TIMER1, SC_TIMER2)
  * @return None
  */
void SMARTCARD_TimerStart(S_SC_DEV_T *dev, E_SC_TIMER no)
{
        SC_TypeDef *SC = dev->base;
        if(no & SC_TIMER0) {
                SC->ISR = SC_ISR_TMR0_IS;
                SC->ALTCTL |= SC_ALTCTL_TMR0_SEN;
        }
        if(no & SC_TIMER1) {
                SC->ISR = SC_ISR_TMR1_IS;
                SC->ALTCTL |= SC_ALTCTL_TMR1_SEN;
        }
        if(no & SC_TIMER2) {
                SC->ISR = SC_ISR_TMR2_IS;
                SC->ALTCTL |= SC_ALTCTL_TMR2_SEN;
        }
}

/**
  * @brief  Stop Timer counting
  * @param  dev   The pointer of data structure of smartcard information
  * @param  no    Timer number. (SC_TIMER0, SC_TIMER1, SC_TIMER2)
  * @return None
  */
void SMARTCARD_TimerStop(S_SC_DEV_T *dev, E_SC_TIMER no)
{
        SC_TypeDef *SC = dev->base;
        if(no & SC_TIMER0)
                SC->ALTCTL &= ~SC_ALTCTL_TMR0_SEN;
        if(no & SC_TIMER1)
                SC->ALTCTL &= ~SC_ALTCTL_TMR1_SEN;
        if(no & SC_TIMER2)
                SC->ALTCTL &= ~SC_ALTCTL_TMR2_SEN;
}

/**
  * @brief  Stop All Timer counting
  * @param  dev    The pointer of data structure of smartcard information
  * @return None
  */
void SMARTCARD_TimerAllStop( S_SC_DEV_T *dev )
{
        SC_TypeDef *SC = dev->base;
        // disable all internal timer function

        SC->ALTCTL &= ~(SC_ALTCTL_TMR0_SEN | SC_ALTCTL_TMR1_SEN | SC_ALTCTL_TMR2_SEN);

}

/**
  * @brief  Specified Timer uses specified operation mode and set ETU number to count.
  * @param  dev      The pointer of data structure of smartcard information
  * @param  no       Timer number. (SC_TIMER0, SC_TIMER1, SC_TIMER2)
  * @param  op_mode  Timer operating mode
  * @param  cnt      Amount count (ETU-based)
  * @return None
  */
void SMARTCARD_TimerCountSet(S_SC_DEV_T *dev, E_SC_TIMER no, uint32_t op_mode, uint32_t cnt)
{
        SC_TypeDef *SC = dev->base;
        uint32_t reg = op_mode | (0xFFFFFF & (cnt-1));

        if(no == SC_TIMER0) {
                SC->TMR0 = reg;
        } else if(no == SC_TIMER1) {
                SC->TMR1 = reg;
        } else if(no == SC_TIMER2) {
                SC->TMR2 = reg;
        }
}


/**
  * @brief  In receiving mode and just waiting for data in, this function uses to detect the first data byte if time-out
  * @param  dev     The pointer of data structure of smartcard information
  * @param  no      Timer number. (SC_TIMER0, SC_TIMER1, SC_TIMER2)
  * @param  op_mode E_SC_TIMER_OPMODE
  * @param  cnt     Amount count (ETU-based)
  * @return None
  */
void SMARTCARD_WaitFirstReceivedData(S_SC_DEV_T *dev, E_SC_TIMER no, uint32_t op_mode, uint32_t cnt)
{
        int32_t timeOutCount = 0, remainderCount = 0;
        int32_t errorflag = 0;

        /* Because TIMER1 or TIMER2 Max. ETU count is 256 */
        if(no != SC_TIMER0) {
                timeOutCount = cnt / 256;
                remainderCount = cnt % 256;
        }

        if(no == SC_TIMER0)
                errorflag = SC_ERR_TIME0OUT;
        else if(no == SC_TIMER1)
                errorflag = SC_ERR_TIME1OUT;
        else if(no == SC_TIMER2)
                errorflag = SC_ERR_TIME2OUT;

        while(timeOutCount) {
                dev->errno = 0;
                SMARTCARD_TimerCountSet(dev, no, op_mode, 256);
                SMARTCARD_TimerStart(dev, no);

                while( !(dev->errno) ) {
                        if(dev->errno == errorflag)
                                break;
                        else {
                                // if received the first data ?
                                if(dev->rcv_cnt > 0) {
                                        SMARTCARD_TimerStop(dev, no);
                                        return ;
                                }

                                // if had any other errors ?
                                if( (dev->errno!=errorflag) && (dev->errno!=0))
                                        return ;
                        }
                }
                timeOutCount--;
        }

        if(remainderCount) {
                dev->errno = 0;
                SMARTCARD_TimerCountSet(dev, no, op_mode, remainderCount);
                SMARTCARD_TimerStart(dev, no);

                while( !(dev->errno) ) {
                        if(dev->errno == errorflag)
                                break;
                        else {
                                // if received the first data ?
                                if(dev->rcv_cnt > 0) {
                                        SMARTCARD_TimerStop(dev, no);
                                        return ;
                                }

                                // if had any other errors ?
                                if( (dev->errno!=errorflag) && (dev->errno!=0))
                                        return ;
                        }
                }
        }
        /* really time-out happened on first byte */
        dev->op_state = SC_OP_NOP;
        dev->errno = errorflag;

}

/**
  * @brief  Enable smartcard uart interface
  * @param  portno   The pointer of smartcard interface
  * @param  sParam   Configuration parameter
  * @return None
  */
void SMARTCARD_UartOpen(SC_TypeDef *portno, S_SC_UART *sParam)
{
	S_SC_DEV_T *dev;
	int32_t port = 0, div;
		
	assert_param(CHECK_SC_PORT(portno));
	assert_param(sParam != NULL);
	assert_param(sParam->u16TimeOut < 0x200);
	assert_param((sParam->u32CharLen & ~SC_UACTL_DATA_LEN_MASK) == 0);
	assert_param((sParam->u32StopBitLen & ~SC_CTL_SLEN) == 0);
	assert_param((sParam->u32Parity & ~(SC_UACTL_PBDIS | SC_UACTL_OPE)) == 0);
	assert_param((sParam->u32RxTriggerLevel & ~SC_CTL_RX_FTRI_LEV_MASK) == 0);

	if(portno==SC0)
		port = 0;
	else if(portno==SC1)
		port = 1;
	else
		port = 2;
	
	dev= (S_SC_DEV_T *)(uint32_t)&sc_device[port];	
	dev->uart = 1;

        /* Get SC clock */
        if( (CLK->CLKSEL2 & CLK_CLKSEL2_SC_MASK) == CLK_CLKSEL2_SC_HXT )  // form HXT
                dev->clock = __XTAL;
        else if( (CLK->CLKSEL2 & CLK_CLKSEL2_SC_MASK) >= CLK_CLKSEL2_SC_HIRC )    // form HIRC
                dev->clock = __IRC12M;

        if(dev->base==SC0)
                dev->clock /= ((CLK->CLKDIV0 >> 28) + 1);
        else if(dev->base==SC1)
                dev->clock /= ((CLK->CLKDIV1 & CLK_CLKDIV1_SC1_MASK) + 1);
        else
                dev->clock /= ((CLK->CLKDIV1 >> 4) + 1);

	div = dev->clock / sParam->u32BaudRate - 1;
	if((dev->clock % sParam->u32BaudRate) > (sParam->u32BaudRate / 2))
		div++;
	portno->ETUCR = div;
	portno->RFTMR = sParam->u16TimeOut;
	portno->CTL = (portno->CTL & ~(SC_CTL_RX_FTRI_LEV_MASK | SC_CTL_SLEN)) | sParam->u32StopBitLen | sParam->u32RxTriggerLevel;	
	portno->UACTL = sParam->u32Parity | sParam->u32CharLen | SC_UACTL_UA_MODE_EN;
	
	portno->ALTCTL = 3;		// reset FIFO. what ever inside is garbage before configuration finished.

	return;
}

/**
  * @brief  Disable smartcard uart interface
  * @param  portno   The pointer of smartcard interface
  * @return None
  */
void SMARTCARD_UartClose(SC_TypeDef *portno)
{
	S_SC_DEV_T *dev;
	int32_t port = 0;
		
	assert_param(CHECK_SC_PORT(portno));
	SMARTCARD_UartDisableInt(portno);

	if(portno==SC0)
		port = 0;
	else if(portno==SC1)
		port = 1;
	else if(portno==SC2)
		port = 2;
	
	dev= (S_SC_DEV_T *)(uint32_t)&sc_device[port];	
	dev->uart = 0;
	portno->UACTL = 0;	
	
	return;
}

/**
  * @brief  Read data from Sc UART interface
  * @param  portno The pointer of smartcard interface
  * @param  pu8RxBuf Pointer to buffers to store receive data
  * @param  u32ReadBytes Available buffer length to store data
  * @return Number of bytes received
  */
int32_t SMARTCARD_UartRead(SC_TypeDef *portno, uint8_t *pu8RxBuf, uint32_t u32ReadBytes)
{

    uint32_t  u32Count, u32delayno;

	assert_param(CHECK_SC_PORT(portno));
	assert_param(pu8RxBuf != NULL);

	for (u32Count=0; u32Count < u32ReadBytes; u32Count++)
	{
		u32delayno = 0;
		while (portno->TRSR & SC_TRSR_RX_EMPTY_F) /* Check RX empty => failed */	
		{
			u32delayno++;
			if ( u32delayno >= 0x40000000 )
				goto out;
		}
		pu8RxBuf[u32Count] = portno->RBR;					    /* Get Data from UART RX  */
	}

out:
	return(u32Count);
}

/**
  * @brief  Write data to Sc UART interface
  * @param  portno The pointer of smartcard interface
  * @param  pu8TxBuf Pointer to buffers to send
  * @param  u32WriteBytes Number of bytes to send
  * @return Number of bytes sent
  */
int32_t SMARTCARD_UartWrite(SC_TypeDef *portno, uint8_t *pu8TxBuf, uint32_t u32WriteBytes)
{

    uint32_t  u32Count, u32delayno;

	assert_param(CHECK_SC_PORT(portno));
	assert_param(pu8TxBuf != NULL);

	for (u32Count=0; u32Count < u32WriteBytes; u32Count++)
	{
		u32delayno = 0;
		while (portno->TRSR & SC_TRSR_TX_FULL_F)  /* Wait Tx empty and Time-out manner */
		{
			u32delayno++;
			if ( u32delayno >= 0x40000000 )     	
				goto out;     	
				
		}
		portno->THR = pu8TxBuf[u32Count]; /* Send UART Data from buffer */
	}

out:
	return(u32Count);

}

/**
  * @brief  Enable SC UART interrupt
  * @param  portno   The pointer of smartcard interface
  * @return None
  */
void SMARTCARD_UartEnableInt(SC_TypeDef *portno)
{
	IRQn_Type SC_IRQn = (portno==SC0)?SC0_IRQn:((portno==SC1)?SC1_IRQn:SC2_IRQn);

	portno->IER = 0x203;	// Enable tx, rx, and rx timeout
	NVIC_EnableIRQ(SC_IRQn);
}

/**
  * @brief  Disable SC UART interrupt
  * @param  portno   The pointer of smartcard interface
  * @return None
  */
void SMARTCARD_UartDisableInt(SC_TypeDef *portno)
{
	IRQn_Type SC_IRQn = (portno==SC0)?SC0_IRQn:((portno==SC1)?SC1_IRQn:SC2_IRQn);

	portno->IER = 0;
	NVIC_DisableIRQ(SC_IRQn);
}

/*@}*/ /* end of group NANO1xx_SMARTCARD_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO1xx_SMARTCARD_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/



