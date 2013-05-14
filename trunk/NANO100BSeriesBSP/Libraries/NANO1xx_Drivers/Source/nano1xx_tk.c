/******************************************************************************
 * @file     nano1xx_tk.c
 * @brief    NANO1xx Touch Key driver source file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "nano1xx_tk.h"
#include "nano1xx_assert.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_TouchKey_Driver Touch Key Driver
  @{
*/


/** @addtogroup NANO1xx_TK_EXPORTED_FUNCTIONS Touch Key Exported Functions
  @{
*/

/**
  * @brief  Enables TK circuit power and engine clock
  * @param  u32Width Touch Key counter width
  * @return None
  */
void TK_Init(uint32_t u32Width)
{
        assert_param((u32Width == TK_CTL1_SEN_SEL_10BIT) ||
		(u32Width == TK_CTL1_SEN_SEL_12BIT) ||
		(u32Width == TK_CTL1_SEN_SEL_14BIT) ||
		(u32Width == TK_CTL1_SEN_SEL_16BIT));

        CLK->APBCLK |= CLK_APBCLK_TK_EN;

        TK->CTL1 = (TK->CTL1 & ~TK_CTL1_SEN_SEL_MASK) | u32Width | TK_CTL1_EN;

}

/**
  * @brief  Disables TK circuit power and engine clock
  * @param  None
  * @return None
  */
void TK_DeInit(void)
{
	CLK->APBCLK &= ~CLK_APBCLK_TK_EN;
        TK->CTL1 &= ~TK_CTL1_EN;

}



/**
  * @brief  Set configuration on designate channel
  * @param  u8Ch Channel for conversion
  * @param  cfg Conversion configutation. Each channel might have its own configration
  * @return None
  */
void TK_ConfigChannel(uint8_t u8Ch, S_TK_CH_CFG *cfg)
{
        uint32_t i;

        assert_param(cfg != NULL);
        assert_param(u8Ch < TK_CH_NUM);
	assert_param((cfg->u8Level <= TK_MAX_CURRENT_LEVEL) && (cfg->u8Level >= TK_MIN_CURRENT_LEVEL));
	assert_param(cfg->u8Div <= TK_MAX_CLOCK_DIVIDER);
	assert_param(cfg->u16HighThld > cfg->u16LowThld);

        u8Ch = u8Ch & 0x7;

        i = *(unsigned int volatile *)(TK_BASE + 0x4 + 4 * (u8Ch >> 2));
        i &= ~(0xff << (8 *(u8Ch & 0x3)));
        i |= ((cfg->u8Level << 4) | (cfg->u8Div)) << (8 *(u8Ch & 0x3));
        *(unsigned int volatile *)(TK_BASE + 0x4 + 4 * (u8Ch >> 2)) = i;

        *(unsigned int volatile *)(TK_BASE + 0x24 + 4 * u8Ch) = (cfg->u16HighThld << 16) | (cfg->u16LowThld);

}


/**
  * @brief  Start touch key conversion on designate channel
  * @param  u16Mask Channel mask for conversion
  * @return None
  */
void TK_Start(uint16_t u16Mask)
{
	uint32_t reg;

        assert_param(u16Mask != 0);
        assert_param(((u16Mask & 0xff) & (u16Mask >> 8)) == 0);	// check mutual exclusive

	reg = TK->CTL1;

        reg = (reg & ~0xff00) | ((u16Mask & 0xff00) | ((u16Mask & 0xFF) << 8)); // en
        reg = (reg & ~0xff) | (((u16Mask & 0xff00) >> 8) & ~(u16Mask & 0xff)); // sel, 0~7 has higher priority

        TK->CTL1 = reg | TK_CTL1_START_BY_SW;


}


/**
  * @brief  Enable touch key interrupt
  * @param  u32Mask Set the condition(s) to trigger interrupt, and enable TK interrupt
  * @return None
  * @note   This API will clear pending interrupt status before enable it.
  */
void TK_EnableInterrupt(uint32_t u32Mask)
{
        assert_param(u32Mask & TK_INT_ALL != 0);


        TK_CLEAR_INTERRUPT;
        TK->INTEN = u32Mask & TK_INT_ALL;

        NVIC_EnableIRQ(TK_IRQn);

}

/**
  * @brief  Disable touch key interrupt
  * @param  u32Mask Clear the condition(s) to trigger interrupt, if all conditions are cleared, disable TK interrupt
  * @return None
  */
void TK_DisableInterrupt(uint32_t u32Mask)
{

        assert_param(u32Mask & TK_INT_ALL != 0);


        TK->INTEN &= ~(u32Mask & TK_INT_ALL);

        if(TK->INTEN == 0) {
                NVIC_DisableIRQ(TK_IRQn);
        }

}



/**
  * @brief  Read last convert data from a channel
  * @param  u8Ch Read latest convert data from channel ch
  * @return Last convert data of specified channel
  */
int16_t TK_ReadData(uint8_t u8Ch)
{
        assert_param(u8Ch < TK_CH_NUM);

        if(u8Ch & 1) {
                return((uint16_t)(*(unsigned int volatile *)(TK_BASE + 0x10 + 4 * ((u8Ch & 0x7) >> 1))  >> 16));
        } else {
                return((uint16_t)(*(unsigned int volatile *)(TK_BASE + 0x10 + 4 * ((u8Ch & 0x7) >> 1)) & 0xffff));
        }

}

/**
  * @brief  Read convert status.
  * @param  pu8Level Place to hold last convert sensitivity level
  * @param  pu8State Place to hold touch key sense failed status.
  * @param  pu8Thld Place to hold touch key threshold status.
  * @return void
  * @note   If application is not interested one of the report status both level and state, it could pass uninterested on with NULL.
  *         Threshold and sense failed will be cleared in interrupt handler, so it is recommanded application call this API within
  *         callback function if interrupt is enabled.
  */
void TK_ReadStatus(uint8_t *pu8Level, uint8_t *pu8State, uint8_t *pu8Thld)
{
        assert_param(!(pu8Level == NULL && pu8State == NULL && pu8Thld == NULL));

        if(pu8Level != NULL)
                *pu8Level = (uint8_t)((TK->STAT & 0xFF) >> 4);
        if(pu8State != NULL)
                *pu8State = (uint8_t)((TK->STAT & 0xFF00) >> 8);
        if(pu8Thld != NULL)
                *pu8Thld = (uint8_t)((TK->STAT & 0xFF0000) >> 16);

}

/**
  * @brief  Check TK engine busy and compelte status
  * @param  pu8Busy Place to hold busy status
  * @param  pu8Complete Place to hold complete status
  * @return void
  * @note   If application is not interested one of the report status both busy and complete, it could pass uninterested one with NULL.
  *         TK_STAT will be cleared in interrupt handler after execute callback func, so it is recommanded application call this API within
  *         callback function if interrupt is enabled.
  */
void TK_CheckBusyComplete(uint8_t *pu8Busy, uint8_t *pu8Complete)
{
        assert_param(!(pu8Busy == NULL && pu8Complete == NULL));

        if(pu8Busy != NULL)
                *pu8Busy = (uint8_t)(TK->STAT & 1);
        if(pu8Complete != NULL)
                *pu8Complete = (uint8_t)(TK->STAT & 0x2) >> 1;

}


/*@}*/ /* end of group NANO1xx_TK_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_TouchKey_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
