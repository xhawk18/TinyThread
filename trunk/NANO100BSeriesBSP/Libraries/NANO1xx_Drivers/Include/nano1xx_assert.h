/******************************************************************************
 * @file     nano1xx_assert.h
 * @brief    NANO1xx Assert functon header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


#ifndef __NU_ASSERT_H
#define __NU_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif


#ifdef USE_ASSERT
#define	assert_param(expr)		{															\
									if (!(expr))											\
									{														\
										assert_error((uint8_t*)__FILE__, __LINE__);	        \
									}														\
								}

void assert_error(uint8_t* file, uint32_t line);
#else
#define	assert_param(expr)
#endif



#ifdef __cplusplus
}
#endif

#endif  // __NU_ASSERT_H


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
