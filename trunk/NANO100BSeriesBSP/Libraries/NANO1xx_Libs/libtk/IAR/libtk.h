/**
  * COPYRIGHT 2011 Nuvoton Technology Corp.
  ******************************************************************************
  * @file    libtk.h
  * @author  NuMicro MCU Software Team
  * @version V0.0.1
  * @date    03/22/2011
  * @brief   NANO1XX TK library header file
  ******************************************************************************
  */ 

#ifndef _NANO1XX_LIBTK_H_
#define _NANO1XX_LIBTK_H_

#include "nano1xx.h"
#include "nano1xx_tk.h"

#ifdef  __cplusplus
extern "C"
{
#endif

#define LIBTK_STATE_IDLE		0xFF
#define LIBTK_STATE_SET0		0xFE
#define LIBTK_STATE_SET1		0xFD
#define LIBTK_STATE_TIMER		0xFC
#define LIBTK_MAX_COMPONENT       16

/**
 * @typedef libtk_resolution_e
 * @brief Indicates the resolution of slider and rotor components. Each 
 * @brief component can have its own setting.  
 */
typedef enum {
    LIBTK_RESOLUTION_4 = 2,
    LIBTK_RESOLUTION_8 = 3,
    LIBTK_RESOLUTION_16 = 4,
    LIBTK_RESOLUTION_32 = 5,
    LIBTK_RESOLUTION_64 = 6   
}libtk_resolution_e;

// TODO: define error value...

/** @typedef libtk_callback
 *  @brief Prototype of callback function for status change
 *  @details  LIBTK calls registered callback function of the component(s) which
 *            status changed during last conversion.
 *             
 *            First parameter is current status, 0xFFFF means finger off. 0 
 *            means key touched for key component. 0~resolution for rotor and 
 *            slider indicates current finger position
 *                 
 *            Second parameter is set by user application. Application can use 
 *            this parameter to identify the component if mutltiple components 
 *            share a single callback function  
 */
typedef void (*libtk_callback)(uint16_t status, uint16_t param);

/** @typedef libtk_channel_config_s
 *	@brief Channel attributes found by TK configuration tool
 */
typedef struct {
	uint16_t base;
	uint16_t diff;
	uint8_t current;
	uint8_t div; 
}libtk_channel_config_s;

/** Structure holds per channel status */
typedef  struct {
    uint8_t ch;                ///< channel #
    uint8_t state;             ///< last sense state 
	uint8_t count;			   ///< counter check for channel stuck in middle
	uint8_t count1;			   ///< counter check for channel lost focus
    uint16_t data;             ///< get from TK_ReadData()
    uint16_t base;             ///< default val while release (16 bit data)    
    uint16_t threshold;        ///< 1/4 diff between on and base
    S_TK_CH_CFG  cfg;
}tk_channel;

typedef enum {
	LIB_TK_CHECKED,
	LIB_TK_NOT_INIT,
	LIB_TK_NOT_ENABLED,
	LIB_TK_SENSE_FAILED,
	LIB_TK_NEED_CALIBRATION,
	LIB_TK_COMPLETE
}tk_chk_state_result;

extern int tk_add_key(uint8_t ch, libtk_callback cb, uint16_t param);
extern int tk_add_slider(uint8_t *ch, uint8_t num, libtk_resolution_e res, libtk_callback cb, uint16_t param);
extern int tk_add_rotor(uint8_t *ch, uint8_t num, libtk_resolution_e res, libtk_callback cb, uint16_t param);
extern int tk_disable_component(uint8_t id);
extern int tk_enable_component(uint8_t id);
extern int tk_start_sense(void);
extern void tk_start_calibration(void);
extern int tk_timer_trigger(uint8_t enable, uint8_t flag);
extern tk_chk_state_result tk_check_state(uint8_t id);



#ifdef  __cplusplus
}
#endif

#endif //#ifndef _NANO1XX_LIBTK_H_

/*** (C) COPYRIGHT 2011 Nuvoton Technology Corp. ***/
