/******************************************************************************
 * @file     nano1xx_dac.c
 * @brief	 NANO1xx DAC driver source file
 * @version	 1.0.1
 * @date	 04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nano1xx_dac.h"
#include "nano1xx_assert.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_DAC_Driver DAC Driver
  @{
*/

/** @addtogroup NANO1xx_DAC_EXPORTED_FUNCTIONS DAC Exported Functions
  @{
*/

/**
  * @brief  Write data to specified channel for conversion
  * @param  u8Ch Channel for conversion
  * @param  u16Data Data for conversion, valid range are between 0~0xFFF
  * @return None
  */
void DAC_WriteData(uint8_t u8Ch, uint16_t u16Data)
{

        assert_param(u8Ch < DAC_CH_NUM);
        assert_param(u16Data <= 0xFFF);


        if(u8Ch == 0) {
                DAC->DATA0 = u16Data;
        } else {
                DAC->DATA1 = u16Data;
        }
}


/**
  * @brief  Set delay time for DAC to become stable after power on or enter idle state after conversion
  * @param  u8Ch DAC channel to be enabled
  * @param  u16PUPDelay Delay time for DAC to become stable after power up.
  * @param  u8ConvDelay Delay time for DAC convert complete.
  * @return None
  * @note   Last parameter (u8ConvDelay) affect both channels. u16PUPDelay * PCLK must be longer than 6us.
  *         u8ConvDelay * PCLK must be longer than 2us.
  */
void DAC_SetDelayTime(uint8_t u8Ch, uint16_t u16PUPDelay, uint8_t u8ConvDelay)
{

        assert_param(u8Ch <DAC_CH_NUM);
        assert_param(u8ConvDelay != 0);
        assert_param(u16PUPDelay != 0);
        assert_param(u16PUPDelay < (1 << 15));

        DAC->COMCTL = (DAC->COMCTL & ~DAC_COMCTL_WAITDACCONV_MASK) | u8ConvDelay;

        if(u8Ch == 0) {
                DAC->CTL0 = (DAC->CTL0 & ~DAC_CTL_PWONSTBCNT_MASK) | (u16PUPDelay << 8);
        } else {
                DAC->CTL1 = (DAC->CTL1 & ~DAC_CTL_PWONSTBCNT_MASK) | (u16PUPDelay << 8);
        }

}

/**
  * @brief  Set trigger source to load data of designated DAC channel
  * @param  u8Ch Channel to be configured
  * @param  u32LoadSel Trigger source to load data, valid options are DAC_CTL_LSEL_DATA_WRITE, DAC_CTL_LSEL_PDMA,
  *                  DAC_CTL_LSEL_TIMER0_CH0, DAC_CTL_LSEL_TIMER0_CH1, DAC_CTL_LSEL_TIMER1_CH0, DAC_CTL_LSEL_TIMER1_CH1
  * @return None
  */
void DAC_SetLoadSel(uint8_t u8Ch, uint32_t u32LoadSel)
{

        assert_param(u8Ch < DAC_CH_NUM);
	assert_param((u32LoadSel == DAC_CTL_LSEL_DATA_WRITE) ||
		     (u32LoadSel == DAC_CTL_LSEL_PDMA) ||
		     (u32LoadSel == DAC_CTL_LSEL_TIMER0_CH0) ||
		     (u32LoadSel == DAC_CTL_LSEL_TIMER0_CH1) ||
		     (u32LoadSel == DAC_CTL_LSEL_TIMER1_CH0) ||
		     (u32LoadSel == DAC_CTL_LSEL_TIMER1_CH1));

        if(u8Ch == 0) {
                DAC->CTL0 = (DAC->CTL0 & ~DAC_CTL_LSEL_MASK) | u32LoadSel;
        } else {
                DAC->CTL1 = (DAC->CTL1 & ~DAC_CTL_LSEL_MASK) | u32LoadSel;
        }

}

/**
  * @brief  Enable DAC interrupt on specified channel
  * @param  u8Ch DAC Channel to enable interrupt
  * @return None
  * @note   This API will clear pending interrupt status before enable it.
  */
void DAC_EnableInterrupt(uint8_t u8Ch)
{

        assert_param(u8Ch < DAC_CH_NUM);


        if(u8Ch == 0) {
                DAC_CLEAR_CH0_INTERRUPT;
                DAC->CTL0 |= DAC_CTL_IE;
        } else {
                DAC_CLEAR_CH1_INTERRUPT;
                DAC->CTL1 |= DAC_CTL_IE;
        }

        NVIC_EnableIRQ(DAC_IRQn);
}

/**
  * @brief  Disable DAC interrupt on specified channel
  * @param  u8Ch DAC Channel to disable interrupt
  * @return None
  */
void DAC_DisableInterrupt(uint8_t u8Ch)
{
        assert_param(u8Ch < DAC_CH_NUM);

        if(u8Ch == 0) {
                DAC->CTL0 &= ~DAC_CTL_IE;
        } else {
                DAC->CTL1 &= ~DAC_CTL_IE;
        }
        if(!(DAC->CTL0 & DAC_CTL_IE))
          if(!(DAC->CTL1 & DAC_CTL_IE))
                NVIC_DisableIRQ(DAC_IRQn);
}



/**
  * @brief  Get current status of specified channel
  * @param  u8Ch Channel to get current state from
  * @return DAC current state
  * @retval DAC_INIT DAC is still in init state, cannot convert any data
  * @retval DAC_BUSY DAC is converting current data
  * @retval DAC_IDLE DAC is in idle state and can convert new data
  */
E_DAC_STATE DAC_GetState(uint8_t u8Ch)
{
        uint32_t i;

        assert_param(u8Ch < DAC_CH_NUM);

        if(u8Ch == 0)
                i = DAC->STS0;
        else
                i = DAC->STS1;

        if(!(i & 0x2))
                return(DAC_INIT);
        else if(i & 0x4)
                return(DAC_BUSY);
        else
                return(DAC_IDLE);


}


/*@}*/ /* end of group NANO1xx_DAC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_DAC_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
