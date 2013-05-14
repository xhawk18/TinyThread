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
#include "nano1xx_smartcard.h"

#define BUF_LEN	16
extern uint8_t rx_buf0[BUF_LEN];
extern volatile uint8_t rx_head0, rx_tail0;
extern uint8_t tx_buf0[BUF_LEN];
extern volatile uint8_t tx_head0, tx_tail0;
extern uint8_t rx_buf1[BUF_LEN];
extern volatile uint8_t rx_head1, rx_tail1;
extern uint8_t tx_buf1[BUF_LEN];
extern volatile uint8_t tx_head1, tx_tail1;
extern uint8_t rx_buf2[BUF_LEN];
extern volatile uint8_t rx_head2, rx_tail2;
extern uint8_t tx_buf2[BUF_LEN];
extern volatile uint8_t tx_head2, tx_tail2;

/**
  * @brief  The interrupt services routine of smartcard port 0
  * @param  None
  * @retval None
  */
void SC0_IRQHandler(void)
{
        SC_TypeDef *SC = SC0;

	if(SC->ISR & SC_ISR_CD_IS) {  // we don't need CD itnerrupt in UART mode.
		SC->ISR = SC_ISR_CD_IS;
		SC->IER &= ~SC_IER_CD_IE;
	}
        /* transmit buffer empty interrupt */
        if( SC->ISR & SC_ISR_TBE_IS ) {
		while(!(SC->TRSR & SC_TRSR_TX_FULL_F)) {
	                if(tx_head0 != tx_tail0) {
	                        SC->THR = tx_buf0[tx_head0++];
				tx_head0 %= BUF_LEN;
	                } else {
	                        SC->IER &= ~SC_IER_TBE_IE;   // nothing else to send. disabler interrupt
				break;
			}

		}
		SC->ISR = SC_ISR_TBE_IS;
        }

        /* Receive Data Reach Interrupt : RDR data ready */
        if(SC->ISR & (SC_ISR_RDA_IS | SC_ISR_RTMR_IS)) {
		while(!(SC->TRSR & SC_TRSR_RX_EMPTY_F)) {
			rx_buf0[rx_tail0] = SC->RBR;
			if((rx_tail0 + 1) % BUF_LEN != rx_head0)
				rx_tail0 = (rx_tail0 + 1) % BUF_LEN;

		}
		SC->ISR = SC_ISR_RDA_IS | SC_ISR_RTMR_IS;
        }

        return;
}


/**
  * @brief  The interrupt services routine of smartcard port 1
  * @param  None
  * @retval None
  */
void SC1_IRQHandler(void)
{
        SC_TypeDef *SC = SC1;

	if(SC->ISR & SC_ISR_CD_IS) {  // we don't need CD itnerrupt in UART mode.
		SC->ISR = SC_ISR_CD_IS;
		SC->IER &= ~SC_IER_CD_IE;
	}
        /* transmit buffer empty interrupt */
        if( SC->ISR & SC_ISR_TBE_IS ) {
		while(!(SC->TRSR & SC_TRSR_TX_FULL_F)) {
	                if(tx_head1 != tx_tail1) {
	                        SC->THR = tx_buf1[tx_head1++];
				tx_head1 %= BUF_LEN;
	                } else {
	                        SC->IER &= ~SC_IER_TBE_IE;   // nothing else to send. disabler interrupt
				break;
			}

		}
		SC->ISR = SC_ISR_TBE_IS;
        }

        /* Receive Data Reach Interrupt : RDR data ready */
        if(SC->ISR & (SC_ISR_RDA_IS | SC_ISR_RTMR_IS)) {
		while(!(SC->TRSR & SC_TRSR_RX_EMPTY_F)) {
			rx_buf1[rx_tail1] = SC->RBR;
			if((rx_tail1 + 1) % BUF_LEN != rx_head1)
				rx_tail1 = (rx_tail1 + 1) % BUF_LEN;

		}
		SC->ISR = SC_ISR_RDA_IS | SC_ISR_RTMR_IS;
        }

        return;

}


/**
  * @brief  The interrupt services routine of smartcard port 2
  * @param  None
  * @retval None
  */
void SC2_IRQHandler(void)
{
        SC_TypeDef *SC = SC2;

	if(SC->ISR & SC_ISR_CD_IS) {  // we don't need CD itnerrupt in UART mode.
		SC->ISR = SC_ISR_CD_IS;
		SC->IER &= ~SC_IER_CD_IE;
	}
        /* transmit buffer empty interrupt */
        if( SC->ISR & SC_ISR_TBE_IS ) {
		while(!(SC->TRSR & SC_TRSR_TX_FULL_F)) {
	                if(tx_head2 != tx_tail2) {
	                        SC->THR = tx_buf2[tx_head2++];
				tx_head2 %= BUF_LEN;
	                } else {
	                        SC->IER &= ~SC_IER_TBE_IE;   // nothing else to send. disabler interrupt
				break;
			}

		}
		SC->ISR = SC_ISR_TBE_IS;
        }

        /* Receive Data Reach Interrupt : RDR data ready */
        if(SC->ISR & (SC_ISR_RDA_IS | SC_ISR_RTMR_IS)) {
		while(!(SC->TRSR & SC_TRSR_RX_EMPTY_F)) {
			rx_buf2[rx_tail2] = SC->RBR;
			if((rx_tail2 + 1) % BUF_LEN != rx_head2)
				rx_tail2 = (rx_tail2 + 1) % BUF_LEN;

		}
		SC->ISR = SC_ISR_RDA_IS | SC_ISR_RTMR_IS;
        }

        return;
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

