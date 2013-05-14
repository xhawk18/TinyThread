/******************************************************************************
 * @file     DataFlashProg.c
 * @brief    Nano1xx USB Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


/*---------------------------------------------------------------------------------------------------------*/
/* Includes of system headers                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "nano1xx_fmc.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

#define MASS_STORAGE_OFFSET       0x00008000  /* To avoid the code to write APROM */   
#define FLASH_PAGE_SIZE           512


uint32_t g_sectorBuf[FLASH_PAGE_SIZE/4];

uint32_t FMC_ReadPage(uint32_t u32startAddr, uint32_t * u32buff)
{
    uint32_t i, u32data;
	
	for (i = 0; i < FLASH_PAGE_SIZE/4; i++)
	{
		FMC_Read(u32startAddr + i*4, &u32data);
		u32buff[i] = u32data;
	}
	
	return 0; 
}

void DataFlashRead(uint32_t addr, uint32_t size, uint32_t buffer)
{
    /* This is low level read function of USB Mass Storage */
    int32_t len;

    /* Modify the address to MASS_STORAGE_OFFSET */
    addr += MASS_STORAGE_OFFSET;
    
    len = (int32_t)size;
	
	UNLOCKREG();
	//FMC_Init();
    
	while(len >= FLASH_PAGE_SIZE)
    {
	    FMC_ReadPage(addr, (uint32_t *)buffer);
        addr   += FLASH_PAGE_SIZE;
        buffer += FLASH_PAGE_SIZE;
        len  -= FLASH_PAGE_SIZE;
    }
	
	//FMC_DeInit();
	LOCKREG();
}


uint32_t FMC_ProgramPage(uint32_t u32startAddr, uint32_t * u32buff)
{
    uint32_t i;
	
	for (i = 0; i < FLASH_PAGE_SIZE/4; i++)
	{
		FMC_Write(u32startAddr + i*4, u32buff[i]);
	}
	
	return 0; 
}


void DataFlashWrite(uint32_t addr, uint32_t size, uint32_t buffer)
{
    /* This is low level write function of USB Mass Storage */
    int32_t len, i, offset;
    uint32_t *pu32;
    uint32_t alignAddr;

    /* Modify the address to MASS_STORAGE_OFFSET */
    addr += MASS_STORAGE_OFFSET;

    len = (int32_t)size;
    
	UNLOCKREG();
	//FMC_Init();

    if ( len == FLASH_PAGE_SIZE && ((addr & (FLASH_PAGE_SIZE-1)) == 0) )
    {
		FMC_Erase(addr);

		while (len >= FLASH_PAGE_SIZE)
		{
            FMC_ProgramPage(addr, (uint32_t *) buffer);
			len    -= FLASH_PAGE_SIZE;
            buffer += FLASH_PAGE_SIZE;
            addr   += FLASH_PAGE_SIZE;		
		}
    }
    else
    {
        do
        {
			alignAddr = addr & 0x1FE00;
            
             /* Get the sector offset*/
			offset = ( addr & (FLASH_PAGE_SIZE-1) );  
            
			if ( offset || (size < FLASH_PAGE_SIZE) )
            {
                /* Non 4k alignment. Note: It needs to avoid add MASS_STORAGE_OFFSET twice. */
                DataFlashRead(alignAddr - MASS_STORAGE_OFFSET, FLASH_PAGE_SIZE, (uint32_t)&g_sectorBuf[0]);

            }
            
            /* Update the data */
            pu32 = (uint32_t *)buffer;
            len = FLASH_PAGE_SIZE - offset;
			if (size < len)
                len = size;
            
			for (i=0;i<len/4;i++)
            {
                g_sectorBuf[offset/4 + i] = pu32[i];
            }
            
            FMC_Erase(alignAddr);
            
			for(i=0;i<16;i++)
            {
            	FMC_ProgramPage(alignAddr + (i << 8), (uint32_t *) g_sectorBuf + (i << 8));
			}

            size -= len;
            addr += len;
            buffer += len;
            
        } while (size > 0);       
    }
	
	//FMC_DeInit();
	LOCKREG();
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

