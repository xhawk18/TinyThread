/******************************************************************************
 * @file     sample_ld.c
 * @brief    Nano1xx FMC Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_fmc.h"

#define KEY_ADDR    0x20003FFC  /* The location of signature for 16KB RAM size */
#define SIGNATURE   0x21557899  /* The signature word is used by AP code to check if simple LD is finished */

#define CONFIG0_TEST_CODE   0x0F9000FF
#define CONFIG1_TEST_CODE   0x9abcdef0


/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function									                                           			   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void)
{
	int32_t  	i32Err;
	uint32_t  	u32Data, i;
    uint32_t 	apBase, apLimit;
    uint32_t 	config0Bak, config1Bak;
    
    printf("+------------------------------------------------------+\n");
    printf("|       NANO1xx FMC Driver Sample Code for LDROM       |\n");
    printf("+------------------------------------------------------+\n");

	UNLOCKREG();

	/* Enable FMC ISP function */
	FMC_Init();

    /* Read BS */
    printf("  Boot Mode .......................... ");
    if (FMC_GetBootSelect() == E_FMC_APROM)
    {
        printf("[APROM]\n");     
        printf("  WARNING: This code should be in LD mode but it is in AP mode now.\n");
        while(1); 
    }
    else
        printf("[LDROM]\n");


	/* Read Data Flash base address */
	u32Data = FMC_ReadDataFlashBaseAddr();
	printf("  Data Flash Base Address ............ [0x%08x]\n", u32Data);
    
    apBase = 0x4000;
    apLimit= 0x8000;

    printf("  Erase AP ROM:0x%04x..0x%04x ........ ", apBase, apLimit-1);
	
	/* Page Erase LDROM */
    for (i = apBase; i < apLimit; i += PAGE_SIZE)
        FMC_Erase(i);
	
	/* Erase Verify */
    i32Err = 0;
	for (i = apBase; i < apLimit; i += 4) 
    {     
		FMC_Read(i, &u32Data);
        
        if(u32Data != 0xFFFFFFFF)
        { 
            i32Err = 1;
        }         
    }
    if (i32Err)
        printf("[FAIL]\n");
    else
        printf("[OK]\n");
	

    printf("  Program AP ROM:0x%04x..0x%04x ...... ", apBase, apLimit-1);
	/* Program AP ROM and read out data to compare it */
    for (i = apBase; i < apLimit; i += 4) 
    {
        FMC_Write(i, i);
    }

    i32Err = 0;
    for (i = apBase; i < apLimit; i += 4) 
    {
		FMC_Read(i, &u32Data);
        if (u32Data != i)
        { 
           i32Err = 1;
        }        
    }
    if (i32Err)
        printf("[FAIL]\n");
    else
        printf("[OK]\n");

    /* Read and backup the configuration settings */
    printf("  Read config0 ....................... ");
    FMC_Read(CONFIG_BASE, &config0Bak);
    printf("[0x%08x]\n", config0Bak);

    printf("  Read config1 ....................... ");
    FMC_Read(CONFIG_BASE+4, &config1Bak);
    printf("[0x%08x]\n", config1Bak);


    /* Configuration region write test */
    printf("  Erase config region ................ ");
    FMC_EnableConfigUpdate();
	FMC_Erase(CONFIG_BASE);
    FMC_Read(CONFIG_BASE, &u32Data);
    if (u32Data != 0xFFFFFFFF)
        printf("[FAIL]\n");
    else
        printf("[OK]\n");

    printf("  Program config region .............. ");
    FMC_Write(CONFIG_BASE, CONFIG0_TEST_CODE);
    FMC_Write(CONFIG_BASE+4, CONFIG1_TEST_CODE);
    i32Err = 0;    
    FMC_Read(CONFIG_BASE, &u32Data);
    if (u32Data != CONFIG0_TEST_CODE)
        i32Err = 1;
    FMC_Read(CONFIG_BASE+4, &u32Data);
    if (u32Data != CONFIG1_TEST_CODE)
        i32Err = 1;

    if (i32Err)
        printf("[FAIL]\n");
    else
        printf("[OK]\n");

    printf("  Restore config settings ............ ");
    FMC_Erase(CONFIG_BASE);
    FMC_Write(CONFIG_BASE, config0Bak);
    FMC_Write(CONFIG_BASE+4, config1Bak);
	FMC_DisableConfigUpdate();

    i32Err = 0;
    FMC_Read(CONFIG_BASE, &u32Data);
    if (u32Data != config0Bak)
        i32Err = 1;
    
    FMC_Read(CONFIG_BASE+4, &u32Data);
    if (u32Data != config1Bak)
        i32Err = 1;

    if (i32Err)
        printf("[FAIL]\n");
    else
        printf("[OK]\n");
   
    /* Write the signature to tell AP code that LD code has finished */
    outpw(KEY_ADDR, SIGNATURE);

	printf("\nFMC Simple LD Code Completed.\n");

    printf("\n  >>> Reset to back to AP mode <<< \n");

    /* Make sure message has printed out */
    for (i = 0; i < 0x10000; i++);
    
    FMC_BootSelect(E_FMC_APROM);
    GCR->IPRST_CTL1 = GCR_IPRSTCTL1_CPU;
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


