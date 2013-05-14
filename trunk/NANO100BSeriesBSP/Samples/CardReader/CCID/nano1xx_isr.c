/******************************************************************************
 * @file     nano1xx_isr.c
 * @brief    This file contains interrupt service routines
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include "nano1xx.h"
#include "nano1xx_usb.h"
#include "nano1xx_smartcard.h"

extern S_SC_DEV_T sc_device[SMARTCARD_NUM];
extern void SC_CardState(void);

extern uint32_t atr_len0, atr_remain0;
extern uint32_t atr_len1, atr_remain1;
extern uint32_t atr_len2, atr_remain2;
extern int32_t atr_check_time0,atr_check_time1,atr_check_time2;
extern int32_t atr_final_chk0, atr_final_chk1, atr_final_chk2;
extern int32_t atr_time0, atr_time1, atr_time2;
extern int32_t atr_total_time_start_flag0;
extern int32_t atr_total_time_start_flag1;
extern int32_t atr_total_time_start_flag2;
extern int32_t ifbyte_flag0, tck0, ifcount0;
extern int32_t ifbyte_flag1, tck1, ifcount1;
extern int32_t ifbyte_flag2, tck2, ifcount2;
extern uint8_t g_SC0UnexpectedInBuf[MIN_BUFFER_SIZE], g_SC1UnexpectedInBuf[MIN_BUFFER_SIZE], g_SC2UnexpectedInBuf[MIN_BUFFER_SIZE];
extern uint8_t g_SC0UnexpectedBufLen, g_SC1UnexpectedBufLen, g_SC2UnexpectedBufLen;

/******************************************************************************/
/*       DO _NOT_ EDIT IRQ HANDLERS UNLESS YOU KNOW WHAT YOU ARE DOING        */
/******************************************************************************/

/**
  * @brief  USBD_IRQHandler, USB interrupt handler.
  * @param  None.
  * @retval None.
  */
void USBD_IRQHandler(void)
{
	uint32_t u32INTSTS = USBD->INTSTS;

	if (u32INTSTS & USB_INTSTS_FLD_STS)
	{
	    /* Handle the USB attached/detached event */
		USB_FloatDetect();
	}
	else if(u32INTSTS & USB_INTSTS_BUS_STS)
	{
	    /* Handle the USB bus event: Reset, Suspend, and Resume */
		USB_BusEvent();
	}
	else if(u32INTSTS & USB_INTSTS_USB_STS)
	{
	    /* Handle the USB Protocol/Clase event */
		USB_UsbEvent(u32INTSTS);
    }
	else if (u32INTSTS & USB_INTSTS_WKEUP_STS)
	{
		/* Clear wakeup event. write one clear */
		USBD->INTSTS |= USB_INTSTS_WKEUP_STS;
	}
}


/**
  * @brief  The interrupt services routine of smartcard port 0
  * @param  None
  * @retval None
  */
void SC0_IRQHandler(void)
{
        uint32_t pincsr;
        uint8_t *atrbuffer;
        SC_TypeDef *SC = SC0;
        uint32_t len, cnt, pos;  // avoid IAR compilation waraning

        S_SC_DEV_T *dev= (S_SC_DEV_T *)(uint32_t)&sc_device[0];

        atrbuffer = dev->CardCapabilities.ATR.Buffer;

        //SCISTDEBUG("Interrupt ISR : SC_ISR : [%02x] \n", SC->ISR);

        if(SC->ISR & SC_ISR_CD_IS) {	/* toggle detect by card */
                SCISTDEBUG("IST0: Card Present Int : SC_ISR : [%02x]\n", SC->ISR);
                SC->ISR = SC_ISR_CD_IS;	// clear CD_IS bit
                pincsr = SC->PINCSR;
                if((pincsr & (SC_PINCSR_CD_REM_F | SC_PINCSR_CD_INS_F)) ==
                                (SC_PINCSR_CD_REM_F | SC_PINCSR_CD_INS_F)) {  // CD_INS_F & CD_REM_F both trigger
                        SCISTDEBUG("IST0: Card status is not sure\n");
                        // [2011.11.30]
                        /* protect writing operation forever */
                        SC->IER &= ~SC_IER_TBE_IE;
                        SC->PINCSR |= (SC_PINCSR_CD_REM_F | SC_PINCSR_CD_INS_F);
                        dev->bCardRemoved = SCARD_UNKNOWN;
                        dev->errno = SC_ERR_CARD_REMOVED;
                        dev->op_state = SC_OP_NOP;
                        if(dev->openflag) {
#ifdef SC_SEQ_MANUAL
                                SMARTCARD_DeactivationCmd( dev );       // de-activation, use HW auto de-activation
#endif
                                SMARTCARD_ClearFIFO(dev);			// clear FIFO
                                SMARTCARD_TimerAllStop(dev);
                                dev->errno = SC_ERR_CARD_REMOVED;
                                dev->op_state = SC_OP_NOP;
                                dev->openflag = 0;
                                dev->CardCapabilities.ATR.Length = 0;
                                //SC->IER &= ~SC_IER_TBE_IE;
                                return ;    // leave ISR

                        }
                } else if( (pincsr & SC_PINCSR_CD_INS_F) &&
                                (((pincsr & SC_PINCSR_CD_PIN_ST) << 6) == (pincsr & SC_PINCSR_CD_LEV))) { // CD_INS_F
                        SC->PINCSR |= SC_PINCSR_CD_INS_F;	// clear CD_INS_F flag
                        SCISTDEBUG("IST0: Card Inserted\n");

                        dev->bCardRemoved = SCARD_PRESENT;
			SC_CardState();
                } else if( (pincsr & SC_PINCSR_CD_REM_F) &&
                                (((pincsr & SC_PINCSR_CD_PIN_ST) << 6) != (pincsr & SC_PINCSR_CD_LEV))) { // CD_REM_F
                        SC->PINCSR |= SC_PINCSR_CD_REM_F;	// clear CD_REM_F flag
                        dev->bCardRemoved = SCARD_ABSENT;
                        SCISTDEBUG("IST0: Card Removed\n");
			SC_CardState();

                        if(/*dev->openflag*/1) { //don't care open or not. need to configure pin state right anyway
                                // [2011.11.30]
                                /* protect writing operation forever */
                                SC->IER &= ~SC_IER_TBE_IE;
#ifdef SC_SEQ_MANUAL
                                SMARTCARD_DeactivationCmd( dev );	   // de-activation, use HW auto de-activation
#endif
                                SMARTCARD_ClearFIFO(dev);			// clear FIFO
                                SMARTCARD_TimerAllStop(dev);
                                dev->errno = SC_ERR_CARD_REMOVED;
                                dev->op_state = SC_OP_NOP;
                                dev->openflag = 0;
                                dev->CardCapabilities.ATR.Length = 0;
                                //SC->IER &= ~SC_IER_TBE_IE;
                                return ;	// leave ISR

                        }
                } else {
                        dev->bCardRemoved = SCARD_ABSENT;
                        dev->errno = SC_ERR_CARD_REMOVED;
                        SCISTDEBUG("IST0: Card status is not sure and HW has something wrong...\n");
                }
        }

        /* Timer0-2 & BGT Time-Out */
        if(SC->ISR & SC_ISR_TMR0_IS) { // Timer0 time-out
		if(SC->IER & SC_IER_TMR0_IE) {
	                SC->ISR = SC_ISR_TMR0_IS;
	                if(atr_final_chk0==1)
	                        atr_final_chk0 = 0;
	                else
	                        dev->errno = SC_ERR_TIME0OUT;
	                dev->op_state = SC_OP_NOP;
	                SCISTDEBUG("IST0: Timer0 time-out\n");
		}
        }
        if(SC->ISR & SC_ISR_TMR1_IS) { // Timer1 time-out
		if(SC->IER & SC_IER_TMR1_IE) {
	                SC->ISR = SC_ISR_TMR1_IS;
	                dev->errno = SC_ERR_TIME1OUT;
	                dev->op_state = SC_OP_NOP;
	                SCISTDEBUG("IST0: Timer1 time-out\n");
		}
        }
        if(SC->ISR & SC_ISR_TMR2_IS) { // Timer2 time-out
		if(SC->IER & SC_IER_TMR2_IE) {
	                SC->ISR = SC_ISR_TMR2_IS;
	                if(atr_check_time0==0) {
	                        SMARTCARD_TimerStop(dev, SC_TIMER2);
	                        dev->errno = SC_ERR_TIME2OUT;
	                        //dev->op_state = SC_OP_NOP;		// mask for CCID TPDU exchange
	                } else {
	                        if((atr_time0-256)>0)
	                                atr_time0 -= 256;
	                        else
	                                atr_check_time0 = 0;
	
	                        // [2011.11.28]
	                        /* EMV Certification - mask this, time mode use DC_RELOAD */
	                        //printf("atr_time0=%d \n", atr_time0);
	                        //SMARTCARD_TimerCountSet(dev, SC_TIMER2, TIMER_DC, (atr_time0>=256)?256:atr_time0);
	                        //SMARTCARD_TimerStart(dev, SC_TIMER2);
	                }
	                //SCISTDEBUG("IST0: Timer2 time-out\n");
		}
        }

        if(SC->ISR & SC_ISR_BGT_IS) {	// Block Guard Time-out
                SC->ISR = SC_ISR_BGT_IS;
                dev->errno = SC_ERR_BGTIMEOUT;
                dev->op_state = SC_OP_NOP;
                SCISTDEBUG("IST0: Block Guard Time-out\n");
        }


        /* initial end interrupt */
        if(SC->ISR & SC_ISR_INIT_IS) {
                SC->ISR = SC_ISR_INIT_IS;	// clear INIT_IS flag
                SCISTDEBUG("IST0: Initial End\n");
        }

        /* auto convention error interrrup	*/
        if(SC->ISR & SC_ISR_ACON_ERR_IS) {
                SC->ISR = SC_ISR_ACON_ERR_IS;
                dev->errno = SC_ERR_AUTOCONVENTION;
                dev->op_state = SC_OP_NOP;
                SCISTDEBUG("IST0: Auto Convention Error\n");
        }

        /* transmit buffer empty interrupt */
        if( SC->ISR & SC_ISR_TBE_IS ) {
                if( dev->op_state==SC_OP_WRITE ) {
                        //SCISTDEBUG("SC_TBE Empty, pos, len, data : %02x, %02x, %02x\n",
                        //		dev->snd_pos, dev->snd_len, dev->snd_buf[dev->snd_pos]);
                        len = dev->snd_len;
                        if((dev->snd_pos+1) >= len) {
                                // disable TX interrupt, disable action must be here, otherwise rapid interrupt will be like a loop in IST
                                // send the last byte out
                                // [2011.11.24]
                                /* EMV Certification: delay Tx speed */
                                //DrvSYS_Delay(6);
                                SC_PUT_ONE_BYTE(dev->snd_buf[dev->snd_pos++]);
                                SC->IER &= ~SC_IER_TBE_IE;
                                dev->op_state = SC_OP_NOP;
                        }

                        /* actual write */
                        len = dev->snd_len;
                        if( (dev->snd_pos+1) < len)
                                SC_PUT_ONE_BYTE(dev->snd_buf[dev->snd_pos++]);

                } else {
                        /* We'll get in here if there's no data in FIFO and Tx interrupt
                        enabled the first time, so do nothing here */
                        ;
                }
        }

        /* Rx FIFO data time out, what a good methord */
        /* Receive Data Reach Interrupt : RDR data ready */
        if(SC->ISR & (SC_ISR_RDA_IS | SC_ISR_RTMR_IS)) {
                if(dev->op_state == SC_OP_READ) {
                        // [2011.11.25]
                        /* EMV Certification */
                        if(dev->CardCapabilities.Protocol.Selected == SCARD_PROTOCOL_T1)
                                SMARTCARD_TimerCountSet(dev, SC_TIMER0, SC_TMR_MODE_DC_RELOAD_S_S, (dev->CardCapabilities.T1.CWT / dev->CardCapabilities.etu) + 4 + 1);

                        if( SC->ISR & SC_ISR_RDA_IS ) {
                                cnt = dev->rcv_cnt;
                                pos = dev->rcv_pos;
                                dev->rcv_buf[pos + cnt] = SC_GET_ONE_BYTE();
                                dev->rcv_cnt++;
                                //SCISTDEBUG("%02x ", dev->rcv_buf[dev->rcv_pos + dev->rcv_cnt - 1]);
                                cnt = dev->rcv_cnt;
                                if(cnt >= dev->rcv_len || dev->rcv_pos + cnt >= MAX_BUF_LEN) {
                                        dev->op_state = SC_OP_NOP;
                                        //break;  // don't receive data we don't want
                                }

                        }
                }

                /* Read ATR ISR */
                else if(dev->op_state == SC_OP_ATR_READ) {
                        // stop checking timer & start to check waiting time 9600
                        SMARTCARD_TimerStop(dev, SC_TIMER0);
                        /* [ISO 7816-3] */
                        //SMARTCARD_TimerCountSet(dev, SC_TIMER0, TIMER_DC, 9627);  // default waiting time 9600 ETUs
                        /* [EMV2000] */
                        SMARTCARD_TimerCountSet(dev, SC_TIMER0, SC_TMR_MODE_DC, 9600+480);
                        SMARTCARD_TimerStart(dev, SC_TIMER0);

                        if(atr_total_time_start_flag0 == 0) {
                                atr_total_time_start_flag0 = 1;
                                /* start counting total time for ATR session */
                                SMARTCARD_TimerStop(dev, SC_TIMER2);
                                SMARTCARD_TimerCountSet(dev, SC_TIMER2, SC_TMR_MODE_DC_RELOAD, 256);
                                atr_check_time0 = 1;
                                SMARTCARD_TimerStart(dev, SC_TIMER2);
                        }

                        if( (SC->ISR & SC_ISR_RDA_IS) && atr_remain0) {
                                /*
                                 * atr_len==0 : TS
                                 * atr_len==1 : T0
                                 */
                                atrbuffer[atr_len0] = SC_GET_ONE_BYTE();
                                //SCISTDEBUG("IST0: ATR[%d]=0x%x remain=%d \n", atr_len0, atrbuffer[atr_len0], atr_remain0);
                                atr_remain0--;
                                ifbyte_flag0--;

                                if(atr_len0 == 1) {
                                        atr_remain0 += (atrbuffer[atr_len0] & 0xf); // Historical byte
                                        ifbyte_flag0 = 0; // T0 contains Y(x) as well
                                }

                                if( ifbyte_flag0 == 0 ) {
                                        if(atrbuffer[atr_len0] & 0x10) {
                                                ++atr_remain0;
                                                ++ifbyte_flag0;
                                        }
                                        if(atrbuffer[atr_len0] & 0x20) {
                                                ++atr_remain0;
                                                ++ifbyte_flag0;
                                        }
                                        if(atrbuffer[atr_len0] & 0x40) {
                                                ++atr_remain0;
                                                ++ifbyte_flag0;
                                        }
                                        if(atrbuffer[atr_len0] & 0x80) {
                                                ++atr_remain0;
                                                ++ifbyte_flag0;
                                                if((tck0 == 0) && (atr_len0 != 1) && ((atrbuffer[atr_len0] & 0xf) != 0)) {
                                                        ++atr_remain0; //tck exist
                                                        tck0 = 1;
                                                }
                                        } else {
                                                /* Here, it's special case for APDU test card */
                                                if((tck0 == 0) && (atr_len0 != 1) && ((atrbuffer[atr_len0] & 0xf) != 0)) {
                                                        ++atr_remain0; //tck exist
                                                        tck0 = 1;
                                                }
                                                ifbyte_flag0 = -1;
                                        }
                                }

                                atr_len0++; 	 /* increase the length of ATR */
                        }

                        if(atr_remain0 == 0) {	/* receive ATR done */
                                dev->CardCapabilities.ATR.Length = atr_len0;
                                SMARTCARD_TimerStop(dev, SC_TIMER0);
                                SMARTCARD_TimerCountSet(dev, SC_TIMER0, SC_TMR_MODE_DC, 480);
                                SMARTCARD_TimerStart(dev, SC_TIMER0);
                                SMARTCARD_TimerStop(dev, SC_TIMER2);
                                atr_final_chk0 = 1;
                                if( (SC->ISR & SC_ISR_RDA_IS) && atr_final_chk0) {
                                        SMARTCARD_TimerStop(dev, SC_TIMER0);
                                        dev->errno = ATR_ERR_INVALID_PARAMETER;
                                        dev->op_state = SC_OP_NOP;
                                        atr_final_chk0 = 0;
                                }
                        }
                } else {
                        // Disgard these data
#ifdef SC_IST_DEBUG
                        SCISTDEBUG("\nIST0: Unknown data==>%02x \n", SC_GET_ONE_BYTE());
#else
                        //SC_GET_ONE_BYTE();
                        g_SC0UnexpectedInBuf[g_SC0UnexpectedBufLen++] = SC_GET_ONE_BYTE();
#endif
                        //SMARTCARD_ClearFIFO(dev);  // clear FIFO
                }
        }


        /* Transmit Error: break eror, frame error, Rx/Tx over flow, parity error, invaild stop */
        if((SC->ISR & SC_ISR_TERR_IS) && (SC->IER & SC_IER_TERR_IE)) {
                //if(dev->op_state)
                {

                        //SCISTDEBUG("IST0: SC_TRSR: %02x\n", SC->TRSR);

                        if(SC->TRSR & SC_TRSR_RX_OVER_F) {
                                SC->TRSR = SC_TRSR_RX_OVER_F;
                                dev->errno = SC_ERR_READ;
                                if(dev->CardCapabilities.Protocol.Selected != SCARD_PROTOCOL_T1)
                                        dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST0: Rx Over Flow\n");
                        }

                        if(SC->TRSR & SC_TRSR_TX_OVER_F) {
                                SC->TRSR = SC_TRSR_TX_OVER_F;
                                dev->errno = SC_ERR_WRITE;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST0: Tx Over Flow\n");
                        }

                        if(SC->TRSR & SC_TRSR_RX_EPA_F) {
                                SC->TRSR = SC_TRSR_RX_EPA_F;
                                dev->errno = SC_ERR_PARITY_ERROR;
                                SC->ALTCTL |= SC_ALTCTL_RX_RST;
                                if(dev->CardCapabilities.Protocol.Selected != SCARD_PROTOCOL_T1)	// for ATR reception
                                        dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST0: Rx Parity Error\n");
                        }

                        if(SC->TRSR & SC_TRSR_RX_EBR_F) {
                                SC->TRSR = SC_TRSR_RX_EBR_F;
                                SC->ALTCTL |= SC_ALTCTL_RX_RST;
                                dev->errno = SC_ERR_READ;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST0: Rx Break Error\n");
                        }

                        if(SC->TRSR & SC_TRSR_RX_EFR_F) {
                                SC->TRSR = SC_TRSR_RX_EFR_F;
                                SC->ALTCTL |= SC_ALTCTL_RX_RST;
                                dev->errno = SC_ERR_READ;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST0: Rx Frame Error\n");
                        }

                        if(SC->TRSR & SC_TRSR_TX_OVER_F) {
                                if(dev->op_state == SC_OP_WRITE)
                                        SC->IER &= ~SC_IER_TBE_IE;
                                SC->TRSR = (SC_TRSR_TX_OVER_F | SC_TRSR_TX_ERETRY_F);
                                SC->ALTCTL |= SC_ALTCTL_TX_RST;			// Tx software reset
                                dev->errno = SC_ERR_WRITE;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST0: Tx Over Retry Error Retry \n");
                        }

                        if(SC->TRSR & SC_TRSR_RX_OVER_F) {
                                SC->TRSR = (SC_TRSR_RX_OVER_F | SC_TRSR_RX_ERETRY_F);
                                dev->errno = SC_ERR_READ;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST0: Rx Over Retry Error Retry \n");
                        }

                }

        }

        return ;

}


/**
  * @brief  The interrupt services routine of smartcard port 1
  * @param  None
  * @retval None
  */
void SC1_IRQHandler(void)
{
        uint32_t pincsr;
        uint8_t *atrbuffer;
        SC_TypeDef *SC = SC1;
        uint32_t len, cnt, pos;  // avoid IAR compilation waraning

        S_SC_DEV_T *dev= (S_SC_DEV_T *)(uint32_t)&sc_device[1];

        atrbuffer = dev->CardCapabilities.ATR.Buffer;

        //SCISTDEBUG("Interrupt ISR : SC_ISR : [%02x] \n", SC->ISR);

        if(SC->ISR & SC_ISR_CD_IS) {	/* toggle detect by card */
                SCISTDEBUG("IST1: Card Present Int : SC_ISR : [%02x]\n", SC->ISR);
                SC->ISR = SC_ISR_CD_IS;	// clear CD_IS bit
                pincsr = SC->PINCSR;
                if((pincsr & (SC_PINCSR_CD_REM_F | SC_PINCSR_CD_INS_F)) ==
                (SC_PINCSR_CD_REM_F | SC_PINCSR_CD_INS_F)) {  // CD_INS_F & CD_REM_F both trigger
                SCISTDEBUG("IST1: Card status is not sure\n");
                        // [2011.11.30]
                        /* protect writing operation forever */
                        SC->IER &= ~SC_IER_TBE_IE;
                        SC->PINCSR |= (SC_PINCSR_CD_REM_F | SC_PINCSR_CD_INS_F);
                        dev->bCardRemoved = SCARD_ABSENT;//SCARD_UNKNOWN;
                        dev->errno = SC_ERR_CARD_REMOVED;
                        dev->op_state = SC_OP_NOP;
                        if(dev->openflag) {
#ifdef SC_SEQ_MANUAL
                                SMARTCARD_DeactivationCmd( dev );       // de-activation, use HW auto de-activation
#endif
                                SMARTCARD_ClearFIFO(dev);			// clear FIFO
                                SMARTCARD_TimerAllStop(dev);
                                dev->errno = SC_ERR_CARD_REMOVED;
                                dev->op_state = SC_OP_NOP;
                                dev->openflag = 0;
                                dev->CardCapabilities.ATR.Length = 0;
                                //SC->IER &= ~SC_IER_TBE_IE;
                                return ;    // leave ISR

                        }

                } else if((pincsr & SC_PINCSR_CD_INS_F) &&
                                (((pincsr & SC_PINCSR_CD_PIN_ST) << 6) == (pincsr & SC_PINCSR_CD_LEV))) { // CD_INS_F
                        SC->PINCSR |= SC_PINCSR_CD_INS_F;	// clear CD_INS_F flag
                        SCISTDEBUG("IST1: Card Inserted\n");

                        dev->bCardRemoved = SCARD_PRESENT;
			SC_CardState();
                } else if ((pincsr & SC_PINCSR_CD_REM_F) &&
                (((pincsr & SC_PINCSR_CD_PIN_ST) << 6) != (pincsr & SC_PINCSR_CD_LEV))) { // CD_REM_F
                        SC->PINCSR |= SC_PINCSR_CD_REM_F;	// clear CD_REM_F flag
                        dev->bCardRemoved = SCARD_ABSENT;
                        SCISTDEBUG("IST1: Card Removed\n");
			SC_CardState();

                        if(1) {
                                // [2011.11.30]
                                /* protect writing operation forever */
                                SC->IER &= ~SC_IER_TBE_IE;
#ifdef SC_SEQ_MANUAL
                                SMARTCARD_DeactivationCmd( dev );	   // de-activation, use HW auto de-activation
#endif
                                SMARTCARD_ClearFIFO(dev);			// clear FIFO
                                SMARTCARD_TimerAllStop(dev);
                                dev->errno = SC_ERR_CARD_REMOVED;
                                dev->errno = SC_ERR_CARD_REMOVED;
                                dev->op_state = SC_OP_NOP;
                                dev->openflag = 0;
                                dev->CardCapabilities.ATR.Length = 0;
                                //SC->IER &= ~SC_IER_TBE_IE;
                                return ;	// leave ISR

                        }
                } else {
                        dev->bCardRemoved = SCARD_ABSENT;
                        dev->errno = SC_ERR_CARD_REMOVED;
                        SCISTDEBUG("IST1: Card status is not sure and HW has something wrong...\n");
                }
        }

        /* Timer0-2 & BGT Time-Out */
        if(SC->ISR & SC_ISR_TMR0_IS) { // Timer0 time-out
                SC->ISR = SC_ISR_TMR0_IS;
                if(atr_final_chk1==1)
                        atr_final_chk1 = 0;
                else
                        dev->errno = SC_ERR_TIME0OUT;
                dev->op_state = SC_OP_NOP;
                SCISTDEBUG("IST1: Timer0 time-out\n");
        }
        if(SC->ISR & SC_ISR_TMR1_IS) { // Timer1 time-out
                SC->ISR = SC_ISR_TMR1_IS;
                dev->errno = SC_ERR_TIME1OUT;
                dev->op_state = SC_OP_NOP;
                SCISTDEBUG("IST1: Timer1 time-out\n");
        }
        if(SC->ISR & SC_ISR_TMR2_IS) { // Timer2 time-out
                SC->ISR = SC_ISR_TMR2_IS;
                if(atr_check_time1==0) {
                        SMARTCARD_TimerStop(dev, SC_TIMER2);
                        dev->errno = SC_ERR_TIME2OUT;
                        //dev->op_state = SC_OP_NOP;		// mask for CCID TPDU exchange
                } else {
                        if((atr_time1 - 256)>0)
                                atr_time1 -= 256;
                        else
                                atr_check_time1 = 0;

                        // [2011.11.28]
                        /* EMV Certification - mask this, time mode use DC_RELOAD */
                        //SMARTCARD_TimerCountSet(dev, SC_TIMER2, TIMER_DC, (atr_time1>=256)?256:atr_time1);
                        //SMARTCARD_TimerStart(dev, SC_TIMER2);
                }
                //SCISTDEBUG("IST1: Timer2 time-out\n");
        }
        if(SC->ISR & SC_ISR_BGT_IS) {	// Block Guard Time-out
                SC->ISR = SC_ISR_BGT_IS;
                dev->errno = SC_ERR_BGTIMEOUT;
                dev->op_state = SC_OP_NOP;
                SCISTDEBUG("IST1: Block Guard Time-out\n");
        }


        /* initial end interrupt */
        if(SC->ISR & SC_ISR_INIT_IS) {
                SC->ISR = SC_ISR_INIT_IS;	// clear INIT_IS flag
                SCISTDEBUG("IST1: Initial End\n");
        }

        /* auto convention error interrrup	*/
        if(SC->ISR & SC_ISR_ACON_ERR_IS) {
                SC->ISR = SC_ISR_ACON_ERR_IS;
                dev->errno = SC_ERR_AUTOCONVENTION;
                dev->op_state = SC_OP_NOP;
                SCISTDEBUG("IST1: Auto Convention Error\n");
        }

        /* transmit buffer empty interrupt */
        if(SC->ISR & SC_ISR_TBE_IS ) {
                if(dev->op_state == SC_OP_WRITE) {
                        //SCISTDEBUG("SC_TBE Empty, pos, len, data : %02x, %02x, %02x\n",
                        //		dev->snd_pos, dev->snd_len, dev->snd_buf[dev->snd_pos]);
                        len = dev->snd_len;
                        if((dev->snd_pos+1) >= len) {
                                // disable TX interrupt, disable action must be here, otherwise rapid interrupt will be like a loop in IST
                                // send the last byte out
                                // [2011.11.24]
                                /* EMV Certification: delay Tx speed */
                                //DrvSYS_Delay(6);
                                SC_PUT_ONE_BYTE(dev->snd_buf[dev->snd_pos++]);
                                SC->IER &= ~SC_IER_TBE_IE;
                                dev->op_state = SC_OP_NOP;
                        }

                        /* actual write */
                        len = dev->snd_len;
                        if( (dev->snd_pos+1) < len)
                                SC_PUT_ONE_BYTE(dev->snd_buf[dev->snd_pos++]);

                } else {
                        /* We'll get in here if there's no data in FIFO and Tx interrupt
                        enabled the first time, so do nothing here */
                        ;
                }
        }

        /* Rx FIFO data time out, what a good methord */
        /* Receive Data Reach Interrupt : RDR data ready */
        if(SC->ISR & (SC_ISR_RDA_IS | SC_ISR_RTMR_IS)) {
                if(dev->op_state == SC_OP_READ) {
                        // [2011.11.25]
                        /* EMV Certification */
                        if(dev->CardCapabilities.Protocol.Selected == SCARD_PROTOCOL_T1)
                                SMARTCARD_TimerCountSet(dev, SC_TIMER0, SC_TMR_MODE_DC_RELOAD_S_S, (dev->CardCapabilities.T1.CWT / dev->CardCapabilities.etu) + 4 + 1);

                        if( SC->ISR & SC_ISR_RDA_IS ) {
                                pos = dev->rcv_pos;
                                cnt = dev->rcv_cnt;
                                dev->rcv_buf[pos + cnt] = SC_GET_ONE_BYTE();
                                dev->rcv_cnt++;
                                //SCISTDEBUG("%02x ", dev->rcv_buf[dev->rcv_pos + dev->rcv_cnt - 1]);
                                cnt = dev->rcv_cnt;
                                if(cnt >= dev->rcv_len || dev->rcv_pos + cnt >= MAX_BUF_LEN) {
                                        dev->op_state = SC_OP_NOP;
                                        //break;  // don't receive data we don't want
                                }

                        }
                }

                /* Read ATR ISR */
                else if(dev->op_state == SC_OP_ATR_READ) {
                        // stop checking timer & start to check waiting time 9600
                        SMARTCARD_TimerStop(dev, SC_TIMER0);
                        /* [ISO 7816-3] */
                        //SMARTCARD_TimerCountSet(dev, SC_TIMER0, TIMER_DC, 9627);  // default waiting time 9600 ETUs
                        /* [EMV2000] */
                        SMARTCARD_TimerCountSet(dev, SC_TIMER0, SC_TMR_MODE_DC, 9600+480);
                        SMARTCARD_TimerStart(dev, SC_TIMER0);

                        if(atr_total_time_start_flag1 == 0) {
                                atr_total_time_start_flag1 = 1;
                                /* start counting total time for ATR session */
                                SMARTCARD_TimerStop(dev, SC_TIMER2);
                                SMARTCARD_TimerCountSet(dev, SC_TIMER2, SC_TMR_MODE_DC_RELOAD, 256);
                                atr_check_time1 = 1;
                                SMARTCARD_TimerStart(dev, SC_TIMER2);
                        }

                        if( (SC->ISR & SC_ISR_RDA_IS) && atr_remain1) {
                                /*
                                 * atr_len==0 : TS
                                 * atr_len==1 : T0
                                 */
                                atrbuffer[atr_len1] = SC_GET_ONE_BYTE();
                                //SCISTDEBUG("IST1: ATR[%d]=0x%x remain=%d \n", atr_len1, atrbuffer[atr_len1], atr_remain1);
                                atr_remain1--;
                                ifbyte_flag1--;

                                if(atr_len1 == 1) {
                                        atr_remain1 += (atrbuffer[atr_len1] & 0xf); // Historical byte
                                        ifbyte_flag1 = 0; // T0 contains Y(x) as well
                                }

                                if( ifbyte_flag1 == 0 ) {
                                        if(atrbuffer[atr_len1] & 0x10) {
                                                ++atr_remain1;
                                                ++ifbyte_flag1;
                                        }
                                        if(atrbuffer[atr_len1] & 0x20) {
                                                ++atr_remain1;
                                                ++ifbyte_flag1;
                                        }
                                        if(atrbuffer[atr_len1] & 0x40) {
                                                ++atr_remain1;
                                                ++ifbyte_flag1;
                                        }
                                        if(atrbuffer[atr_len1] & 0x80) {
                                                ++atr_remain1;
                                                ++ifbyte_flag1;
                                                if((tck1 == 0) && (atr_len1 != 1) && ((atrbuffer[atr_len1] & 0xf) != 0)) {
                                                        ++atr_remain1; //tck exist
                                                        tck1 = 1;
                                                }
                                        } else {
                                                /* Here, it's special case for APDU test card */
                                                if((tck1 == 0) && (atr_len1 != 1) && ((atrbuffer[atr_len1] & 0xf) != 0)) {
                                                        ++atr_remain1; //tck exist
                                                        tck1 = 1;
                                                }
                                                ifbyte_flag1 = -1;
                                        }
                                }

                                atr_len1++; 	 /* increase the length of ATR */
                        }

                        if(atr_remain1 == 0) {	/* receive ATR done */
                                dev->CardCapabilities.ATR.Length = atr_len1;
                                SMARTCARD_TimerStop(dev, SC_TIMER0);
                                SMARTCARD_TimerCountSet(dev, SC_TIMER0, SC_TMR_MODE_DC, 480);
                                SMARTCARD_TimerStart(dev, SC_TIMER0);
                                SMARTCARD_TimerStop(dev, SC_TIMER2);
                                atr_final_chk1 = 1;
                                if( (SC->ISR & SC_ISR_RDA_IS) && atr_final_chk1) {
                                        SMARTCARD_TimerStop(dev, SC_TIMER0);
                                        dev->errno = ATR_ERR_INVALID_PARAMETER;
                                        dev->op_state = SC_OP_NOP;
                                        atr_final_chk1 = 0;
                                }
                        }
                } else {
                        // Disgard these data
#ifdef SC_IST_DEBUG
                        SCISTDEBUG("\nIST1: Unknown data==>%02x \n", SC_GET_ONE_BYTE());
#else
                        //SC_GET_ONE_BYTE();
                        g_SC1UnexpectedInBuf[g_SC1UnexpectedBufLen++] = SC_GET_ONE_BYTE();
#endif
                        //SMARTCARD_ClearFIFO(dev);  // clear FIFO
                }
        }


        /* Transmit Error: break eror, frame error, Rx/Tx over flow, parity error, invaild stop */
        if((SC->ISR & SC_ISR_TERR_IS) && (SC->IER & SC_IER_TERR_IE)) {
                //if(dev->op_state)
                {

                        //SCISTDEBUG("IST1: SC_TRSR: %02x\n", SC->TRSR);

                        if(SC->TRSR & SC_TRSR_RX_OVER_F) {
                                SC->TRSR = SC_TRSR_RX_OVER_F;
                                dev->errno = SC_ERR_READ;
                                if(dev->CardCapabilities.Protocol.Selected != SCARD_PROTOCOL_T1)
                                        dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST1: Rx Over Flow\n");
                        }

                        if(SC->TRSR & SC_TRSR_TX_OVER_F) {
                                SC->TRSR = SC_TRSR_TX_OVER_F;
                                dev->errno = SC_ERR_WRITE;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST1: Tx Over Flow\n");
                        }

                        if(SC->TRSR & SC_TRSR_RX_EPA_F) {
                                SC->TRSR = SC_TRSR_RX_EPA_F;
                                dev->errno = SC_ERR_PARITY_ERROR;
                                SC->ALTCTL |= SC_ALTCTL_RX_RST;
                                if(dev->CardCapabilities.Protocol.Selected != SCARD_PROTOCOL_T1)	// for ATR reception
                                        dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST1: Rx Parity Error\n");
                        }

                        if(SC->TRSR & SC_TRSR_RX_EBR_F) {
                                SC->TRSR = SC_TRSR_RX_EBR_F;
                                SC->ALTCTL |= SC_ALTCTL_RX_RST;
                                dev->errno = SC_ERR_READ;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST1: Rx Break Error\n");
                        }

                        if(SC->TRSR & SC_TRSR_RX_EFR_F) {
                                SC->TRSR = SC_TRSR_RX_EFR_F;
                                SC->ALTCTL |= SC_ALTCTL_RX_RST;
                                dev->errno = SC_ERR_READ;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST1: Rx Frame Error\n");
                        }

                        if(SC->TRSR & SC_TRSR_TX_OVER_F) {
                                if(dev->op_state == SC_OP_WRITE)
                                        SC->IER &= ~SC_IER_TBE_IE;
                                SC->TRSR = (SC_TRSR_TX_OVER_F | SC_TRSR_TX_ERETRY_F);
                                SC->ALTCTL |= SC_ALTCTL_TX_RST;			// Tx software reset
                                dev->errno = SC_ERR_WRITE;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST1: Tx Over Retry Error Retry \n");
                        }

                        if(SC->TRSR & SC_TRSR_RX_OVER_F) {
                                SC->TRSR = (SC_TRSR_RX_OVER_F | SC_TRSR_RX_ERETRY_F);
                                dev->errno = SC_ERR_READ;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST1: Rx Over Retry Error Retry \n");
                        }
                }

        }


        return ;

}


/**
  * @brief  The interrupt services routine of smartcard port 2
  * @param  None
  * @retval None
  */
void SC2_IRQHandler(void)
{
        uint32_t pincsr;
        uint8_t *atrbuffer;
        SC_TypeDef *SC = SC2;
        uint32_t len, cnt, pos;  // avoid IAR compilation waraning

        S_SC_DEV_T *dev= (S_SC_DEV_T *)(uint32_t)&sc_device[2];

        atrbuffer = dev->CardCapabilities.ATR.Buffer;

        //SCISTDEBUG("Interrupt ISR : SC_ISR : [%02x] \n", SC->ISR);

        if(SC->ISR & SC_ISR_CD_IS) {	/* toggle detect by card */
                SCISTDEBUG("IST2: Card Present Int : SC_ISR : [%02x]\n", SC->ISR);
                SC->ISR = SC_ISR_CD_IS;	// clear CD_IS bit
                pincsr = SC->PINCSR;
                if((pincsr & (SC_PINCSR_CD_REM_F | SC_PINCSR_CD_INS_F)) ==
                                (SC_PINCSR_CD_REM_F | SC_PINCSR_CD_INS_F)) {  // CD_INS_F & CD_REM_F both trigger
                        SCISTDEBUG("IST2: Card status is not sure\n");
                        // [2011.11.30]
                        /* protect writing operation forever */
                        SC->IER &= ~SC_IER_TBE_IE;
                        SC->PINCSR |= (SC_PINCSR_CD_REM_F | SC_PINCSR_CD_INS_F);
                        dev->bCardRemoved = SCARD_UNKNOWN;
                        dev->errno = SC_ERR_CARD_REMOVED;
                        dev->op_state = SC_OP_NOP;
                        if(dev->openflag) {
#ifdef SC_SEQ_MANUAL
                                SMARTCARD_DeactivationCmd( dev );       // de-activation, use HW auto de-activation
#endif
                                SMARTCARD_ClearFIFO(dev);			// clear FIFO
                                SMARTCARD_TimerAllStop(dev);
                                dev->errno = SC_ERR_CARD_REMOVED;
                                dev->op_state = SC_OP_NOP;
                                dev->openflag = 0;
                                dev->CardCapabilities.ATR.Length = 0;
                                //SC->IER &= ~SC_IER_TBE_IE;
                                return ;    // leave ISR

                        }
                } else if((pincsr & SC_PINCSR_CD_INS_F) &&
                                (((pincsr & SC_PINCSR_CD_PIN_ST) << 6) == (pincsr & SC_PINCSR_CD_LEV))) { // CD_INS_F
                        SC->PINCSR |= SC_PINCSR_CD_INS_F;	// clear CD_INS_F flag
                        SCISTDEBUG("IST2: Card Inserted\n");

                        dev->bCardRemoved = SCARD_PRESENT;
                } else if( (pincsr & SC_PINCSR_CD_REM_F) &&
                                (((pincsr & SC_PINCSR_CD_PIN_ST) << 6) != (pincsr & SC_PINCSR_CD_LEV))) { // CD_REM_F
                        SC->PINCSR |= SC_PINCSR_CD_REM_F;	// clear CD_REM_F flag
                        dev->bCardRemoved = SCARD_ABSENT;
                        SCISTDEBUG("IST2: Card Removed\n");

                        if(/*dev->openflag*/1) { //don't care open or not. need to configure pin state right anyway
                                // [2011.11.30]
                                /* protect writing operation forever */
                                SC->IER &= ~SC_IER_TBE_IE;
#ifdef SC_SEQ_MANUAL
                                SMARTCARD_DeactivationCmd( dev );	   // de-activation, use HW auto de-activation
#endif
                                SMARTCARD_ClearFIFO(dev);			// clear FIFO
                                SMARTCARD_TimerAllStop(dev);
                                dev->errno = SC_ERR_CARD_REMOVED;
                                dev->op_state = SC_OP_NOP;
                                dev->openflag = 0;
                                dev->CardCapabilities.ATR.Length = 0;
                                //SC->IER &= ~SC_IER_TBE_IE;
                                return ;	// leave ISR

                        }
                } else {
                        dev->bCardRemoved = SCARD_ABSENT;
                        dev->errno = SC_ERR_CARD_REMOVED;
                        SCISTDEBUG("IST2: Card status is not sure and HW has something wrong...\n");
                }
        }

        /* Timer0-2 & BGT Time-Out */
        if(SC->ISR & SC_ISR_TMR0_IS) { // Timer0 time-out
                SC->ISR = SC_ISR_TMR0_IS;
                if(atr_final_chk2==1)
                        atr_final_chk2 = 0;
                else
                        dev->errno = SC_ERR_TIME0OUT;
                dev->op_state = SC_OP_NOP;
                SCISTDEBUG("IST2: Timer0 time-out\n");
        }
        if(SC->ISR & SC_ISR_TMR1_IS) { // Timer1 time-out
                SC->ISR = SC_ISR_TMR1_IS;
                dev->errno = SC_ERR_TIME1OUT;
                dev->op_state = SC_OP_NOP;
                SCISTDEBUG("IST2: Timer1 time-out\n");
        }
        if(SC->ISR & SC_ISR_TMR2_IS) { // Timer2 time-out
                SC->ISR = SC_ISR_TMR2_IS;
                if(atr_check_time2==0) {
                        SMARTCARD_TimerStop(dev, SC_TIMER2);
                        dev->errno = SC_ERR_TIME2OUT;
                        //dev->op_state = SC_OP_NOP;		// mask for CCID TPDU exchange
                } else {
                        if((atr_time2-256)>0)
                                atr_time2 -= 256;
                        else
                                atr_check_time2 = 0;

                        // [2011.11.28]
                        /* EMV Certification - mask this, time mode use DC_RELOAD */
                        //SMARTCARD_TimerCountSet(dev, SC_TIMER2, TIMER_DC, (atr_time1>=256)?256:atr_time1);
                        //SMARTCARD_TimerStart(dev, SC_TIMER2);
                }
                SCISTDEBUG("IST2: Timer2 time-out\n");
        }
        if(SC->ISR & SC_ISR_BGT_IS) {	// Block Guard Time-out
                SC->ISR = SC_ISR_BGT_IS;
                dev->errno = SC_ERR_BGTIMEOUT;
                dev->op_state = SC_OP_NOP;
                SCISTDEBUG("IST2: Block Guard Time-out\n");
        }


        /* initial end interrupt */
        if(SC->ISR & SC_ISR_INIT_IS) {
                SC->ISR = SC_ISR_INIT_IS;	// clear INIT_IS flag
                SCISTDEBUG("IST2: Initial End\n");
        }

        /* auto convention error interrrup	*/
        if(SC->ISR & SC_ISR_ACON_ERR_IS) {
                SC->ISR = SC_ISR_ACON_ERR_IS;
                dev->errno = SC_ERR_AUTOCONVENTION;
                dev->op_state = SC_OP_NOP;
                SCISTDEBUG("IST2: Auto Convention Error\n");
        }

        /* transmit buffer empty interrupt */
        if(SC->ISR & SC_ISR_TBE_IS) {
                if(dev->op_state == SC_OP_WRITE) {
                        //SCISTDEBUG("SC_TBE Empty, pos, len, data : %02x, %02x, %02x\n",
                        //		dev->snd_pos, dev->snd_len, dev->snd_buf[dev->snd_pos]);
                        len = dev->snd_len;
                        if((dev->snd_pos+1) >= len) {
                                // disable TX interrupt, disable action must be here, otherwise rapid interrupt will be like a loop in IST
                                // send the last byte out
                                // [2011.11.24]
                                /* EMV Certification: delay Tx speed */
                                //DrvSYS_Delay(6);
                                SC_PUT_ONE_BYTE(dev->snd_buf[dev->snd_pos++]);
                                SC->IER &= ~SC_IER_TBE_IE;
                                dev->op_state = SC_OP_NOP;
                        }

                        /* actual write */
                        len = dev->snd_len;
                        if( (dev->snd_pos+1) < len)
                                SC_PUT_ONE_BYTE(dev->snd_buf[dev->snd_pos++]);

                } else {
                        /* We'll get in here if there's no data in FIFO and Tx interrupt
                        enabled the first time, so do nothing here */
                        ;
                }
        }

        /* Rx FIFO data time out, what a good methord */
        /* Receive Data Reach Interrupt : RDR data ready */
        if(SC->ISR & (SC_ISR_RDA_IS | SC_ISR_RTMR_IS)) {
                if(dev->op_state == SC_OP_READ) {
                        // [2011.11.25]
                        /* EMV Certification */
                        if(dev->CardCapabilities.Protocol.Selected == SCARD_PROTOCOL_T1)
                                SMARTCARD_TimerCountSet(dev, SC_TIMER0, SC_TMR_MODE_DC_RELOAD_S_S, (dev->CardCapabilities.T1.CWT / dev->CardCapabilities.etu) + 4 + 1);

                        if( SC->ISR & SC_ISR_RDA_IS ) {
                                pos = dev->rcv_pos;
                                cnt = dev->rcv_cnt;
                                dev->rcv_buf[pos + cnt] = SC_GET_ONE_BYTE();
                                dev->rcv_cnt++;
                                //SCISTDEBUG("%02x ", dev->rcv_buf[dev->rcv_pos + dev->rcv_cnt - 1]);
                                cnt = dev->rcv_cnt;
                                if(cnt >= dev->rcv_len || dev->rcv_pos + cnt >= MAX_BUF_LEN) {
                                        dev->op_state = SC_OP_NOP;
                                        //break;  // don't receive data we don't want
                                }

                        }
                }

                /* Read ATR ISR */
                else if(dev->op_state == SC_OP_ATR_READ) {
                        // stop checking timer & start to check waiting time 9600
                        SMARTCARD_TimerStop(dev, SC_TIMER0);
                        /* [ISO 7816-3] */
                        //SMARTCARD_TimerCountSet(dev, SC_TIMER0, TIMER_DC, 9627);  // default waiting time 9600 ETUs
                        /* [EMV2000] */
                        SMARTCARD_TimerCountSet(dev, SC_TIMER0, SC_TMR_MODE_DC, 9600+480);
                        SMARTCARD_TimerStart(dev, SC_TIMER0);

                        if(atr_total_time_start_flag2 == 0) {
                                atr_total_time_start_flag2 = 1;
                                /* start counting total time for ATR session */
                                SMARTCARD_TimerStop(dev, SC_TIMER2);
                                SMARTCARD_TimerCountSet(dev, SC_TIMER2, SC_TMR_MODE_DC_RELOAD, 256);
                                atr_check_time2 = 1;
                                SMARTCARD_TimerStart(dev, SC_TIMER2);
                        }

                        if( (SC->ISR & SC_ISR_RDA_IS) && atr_remain2) {
                                /*
                                 * atr_len==0 : TS
                                 * atr_len==1 : T0
                                 */
                                atrbuffer[atr_len2] = SC_GET_ONE_BYTE();
                                //SCISTDEBUG("IST2: ATR[%d]=0x%x remain=%d \n", atr_len1, atrbuffer[atr_len1], atr_remain1);
                                atr_remain2--;
                                ifbyte_flag2--;

                                if(atr_len2 == 1) {
                                        atr_remain2 += (atrbuffer[atr_len2] & 0xf); // Historical byte
                                        ifbyte_flag2 = 0; // T0 contains Y(x) as well
                                }

                                if( ifbyte_flag2 == 0 ) {
                                        if(atrbuffer[atr_len2] & 0x10) {
                                                ++atr_remain2;
                                                ++ifbyte_flag2;
                                        }
                                        if(atrbuffer[atr_len2] & 0x20) {
                                                ++atr_remain2;
                                                ++ifbyte_flag2;
                                        }
                                        if(atrbuffer[atr_len2] & 0x40) {
                                                ++atr_remain2;
                                                ++ifbyte_flag2;
                                        }
                                        if(atrbuffer[atr_len2] & 0x80) {
                                                ++atr_remain2;
                                                ++ifbyte_flag2;
                                                if((tck2 == 0) && (atr_len2 != 1) && ((atrbuffer[atr_len2] & 0xf) != 0)) {
                                                        ++atr_remain2; //tck exist
                                                        tck2 = 1;
                                                }
                                        } else {
                                                /* Here, it's special case for APDU test card */
                                                if((tck2 == 0) && (atr_len2 != 1) && ((atrbuffer[atr_len2] & 0xf) != 0)) {
                                                        ++atr_remain2; //tck exist
                                                        tck2 = 1;
                                                }
                                                ifbyte_flag2 = -1;
                                        }
                                }

                                atr_len2++; 	 /* increase the length of ATR */
                        }

                        if(atr_remain2 == 0) {	/* receive ATR done */
                                dev->CardCapabilities.ATR.Length = atr_len2;
                                SMARTCARD_TimerStop(dev, SC_TIMER0);
                                SMARTCARD_TimerCountSet(dev, SC_TIMER0, SC_TMR_MODE_DC, 480);
                                SMARTCARD_TimerStart(dev, SC_TIMER0);
                                SMARTCARD_TimerStop(dev, SC_TIMER2);
                                atr_final_chk2 = 1;
                                if( (SC->ISR & SC_ISR_RDA_IS) && atr_final_chk2) {
                                        SMARTCARD_TimerStop(dev, SC_TIMER0);
                                        dev->errno = ATR_ERR_INVALID_PARAMETER;
                                        dev->op_state = SC_OP_NOP;
                                        atr_final_chk2 = 0;
                                }
                        }
                } else {
                        // Disgard these data
#ifdef SC_IST_DEBUG
                        SCISTDEBUG("\nIST2: Unknown data==>%02x \n", SC_GET_ONE_BYTE());
#else
                        //SC_GET_ONE_BYTE();
                        g_SC2UnexpectedInBuf[g_SC2UnexpectedBufLen++] = SC_GET_ONE_BYTE();
#endif
                        //SMARTCARD_ClearFIFO(dev);  // clear FIFO
                }
        }


        /* Transmit Error: break eror, frame error, Rx/Tx over flow, parity error, invaild stop */
        if((SC->ISR & SC_ISR_TERR_IS) && (SC->IER & SC_IER_TERR_IE)) {
                //if(dev->op_state)
                {

                        //SCISTDEBUG("IST2: SC_TRSR: %02x\n", SC->TRSR);

                        if(SC->TRSR & SC_TRSR_RX_OVER_F) {
                                SC->TRSR = SC_TRSR_RX_OVER_F;
                                dev->errno = SC_ERR_READ;
                                if(dev->CardCapabilities.Protocol.Selected != SCARD_PROTOCOL_T1)
                                        dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST2: Rx Over Flow\n");
                        }

                        if(SC->TRSR & SC_TRSR_TX_OVER_F) {
                                SC->TRSR = SC_TRSR_TX_OVER_F;
                                dev->errno = SC_ERR_WRITE;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST2: Tx Over Flow\n");
                        }

                        if(SC->TRSR & SC_TRSR_RX_EPA_F) {
                                SC->TRSR = SC_TRSR_RX_EPA_F;
                                dev->errno = SC_ERR_PARITY_ERROR;
                                SC->ALTCTL |= SC_ALTCTL_RX_RST;
                                if(dev->CardCapabilities.Protocol.Selected != SCARD_PROTOCOL_T1)	// for ATR reception
                                        dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST2: Rx Parity Error\n");
                        }

                        if(SC->TRSR & SC_TRSR_RX_EBR_F) {
                                SC->TRSR = SC_TRSR_RX_EBR_F;
                                SC->ALTCTL |= SC_ALTCTL_RX_RST;
                                dev->errno = SC_ERR_READ;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST2: Rx Break Error\n");
                        }

                        if(SC->TRSR & SC_TRSR_RX_EFR_F) {
                                SC->TRSR = SC_TRSR_RX_EFR_F;
                                SC->ALTCTL |= SC_ALTCTL_RX_RST;
                                dev->errno = SC_ERR_READ;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST2: Rx Frame Error\n");
                        }

                        if(SC->TRSR & SC_TRSR_TX_OVER_F) {
                                if(dev->op_state == SC_OP_WRITE)
                                        SC->IER &= ~SC_IER_TBE_IE;
                                SC->TRSR = (SC_TRSR_TX_OVER_F | SC_TRSR_TX_ERETRY_F);
                                SC->ALTCTL |= SC_ALTCTL_TX_RST;			// Tx software reset
                                dev->errno = SC_ERR_WRITE;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST2: Tx Over Retry Error Retry \n");
                        }

                        if(SC->TRSR & SC_TRSR_RX_OVER_F) {
                                SC->TRSR = (SC_TRSR_RX_OVER_F | SC_TRSR_RX_ERETRY_F);
                                dev->errno = SC_ERR_READ;
                                dev->op_state = SC_OP_NOP;
                                SCISTDEBUG("IST2: Rx Over Retry Error Retry \n");
                        }

                }

        }


        return ;

}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

