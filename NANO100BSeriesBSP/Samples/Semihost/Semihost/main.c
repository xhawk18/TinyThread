/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx Semihost Sample code
 * @version  1.0.1
 * @date     04, December, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx.h"



/*---------------------------------------------------------------------------------------------------------*/
/* Main Function     				                                                                       */
/*---------------------------------------------------------------------------------------------------------*/

int32_t main()
{
	int8_t item;

	printf("\n Start SEMIHOST test: \n");
	
	while(1)
	{
		item = getchar();
    	printf("%c\n",item);
	}

}	





/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


	 
