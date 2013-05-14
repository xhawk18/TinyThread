/******************************************************************************
 * @file     main.c
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

#define KEY_ADDR    0x20003FFC      /* The location of signature for 16KB RAM size */
#define SIGNATURE   0x21557899  	/* The signature word is used by AP code to check if simple LD is finished */

extern uint32_t loaderImageBase;
extern uint32_t loaderImageLimit;


/**
  * @brief  Main funcion
  * @param  None
  * @retval 0: Success; <0: Failed
  */
int32_t main (void)
{
	int32_t  i32Err;
	uint32_t u32Data, i, u32ImageSize, j, *pu32Loader;
    uint8_t ch;

	UNLOCKREG();

	/* Enable FMC ISP function */
	FMC_Init();

	/* Read Company ID */
	u32Data = 0;
	FMC_ReadCID(&u32Data);
	if (u32Data != 0xda)
	{
		printf("Wrong CID: 0x%x\n", u32Data);
        goto lexit;
	}

    /* Check the signature to check if Simple LD code is finished or not */
    if (inpw(KEY_ADDR) == SIGNATURE)
    {
        /* Just clear SIGNATURE and finish the sample code if Simple LD code has been executed. */
        outpw(KEY_ADDR, 0);
        
		/* Read BS */
        printf("  Boot Mode .................................. ");
        if (FMC_GetBootSelect() == E_FMC_APROM)
            printf("[APROM]\n");
        else
        {
            printf("[LDROM]\n");
            printf("  WARNING: The driver sample code must execute in AP mode!\n");
        }
        goto lexit;
    }
    
    printf("\n\n");
    printf("+-------------------------------------------------------------------------+\n");
    printf("|            NANO1xx Flash Memory Controller Driver Sample Code           |\n");
    printf("+-------------------------------------------------------------------------+\n");
	printf("  NOTE: This sample must be applied to NANO1xx series equipped with 16KB RAM.\n");

    /* Read BS */
    printf("  Boot Mode .................................. ");
    if (FMC_GetBootSelect() == E_FMC_APROM)
        printf("[APROM]\n");
    else
    {
        printf("[LDROM]\n");
        printf("  WARNING: The driver sample code must execute in AP mode!\n");
        goto lexit;
    }

	/* Read Data Flash base address */
	u32Data = FMC_ReadDataFlashBaseAddr();
	printf("  Data Flash Base Address .................... [0x%08x]\n", u32Data);

    /* Check the data in LD ROM to avoid overwrite them */
    FMC_Read(LDROM_BASE, &u32Data);
    if (u32Data != 0xFFFFFFFF)
    {
        printf("\n  WARNING: There is code in LD ROM.\n  If you proceed, the code in LD ROM will be corrupted.\n");
        printf("  Continue? [y/n]:");
        ch = getchar();
        putchar(ch);
        if (ch != 'y')
            goto lexit;
        printf("\n\n");
    }

	/* Enable LDROM update */
	FMC_EnableLDUpdate();

    printf("  Erase LD ROM ............................... ");
	/* Page Erase LDROM */
    for (i = 0; i < 4096; i += PAGE_SIZE)
        FMC_Erase(LDROM_BASE + i);
	
	/* Erase Verify */
    i32Err = 0;
	for (i = LDROM_BASE; i < (LDROM_BASE+4096); i += 4) 
    {     
		FMC_Read(i, &u32Data);
        if(u32Data != 0xFFFFFFFF)
        {
        	printf(" u32Data = 0x%x\n", u32Data);
            i32Err = 1;
        }         
    }
    if (i32Err)
        printf("[FAIL]\n");
    else
        printf("[OK]\n");
	

    printf("  Program LD ROM test ........................ ");
	
	/* Program LD ROM and read out data to compare it */
    for (i = LDROM_BASE; i < (LDROM_BASE+4096); i += 4) 
    {
        FMC_Write(i, i);
    }

    i32Err = 0;
    for (i = LDROM_BASE; i < (LDROM_BASE+4096); i += 4) 
    {
		FMC_Read(i, &u32Data);
        if(u32Data != i)
        { 
           i32Err = 1;
        }        
    }
    if (i32Err)
        printf("[FAIL]\n");
    else
        printf("[OK]\n");


    /* Check LD image size */
    u32ImageSize = (uint32_t)&loaderImageLimit - (uint32_t)&loaderImageBase;
    if (u32ImageSize == 0)
    {
        printf("  ERROR: Loader Image is 0 bytes!\n");
        goto lexit;
    }

    if (u32ImageSize > 4096)
    {
        printf("  ERROR: Loader Image is larger than 4KBytes!\n");
        goto lexit;    
    }


    printf("  Program Simple LD Code ..................... ");
    pu32Loader = (uint32_t *)&loaderImageBase;
    for (i = 0; i < u32ImageSize; i += PAGE_SIZE)
    {
        FMC_Erase(LDROM_BASE + i);    
        for (j = 0; j < PAGE_SIZE; j += 4)
        {
            FMC_Write(LDROM_BASE + i + j, pu32Loader[(i + j) / 4]);
        }
    }

    /* Verify loader */
    i32Err = 0;
    for (i = 0; i < u32ImageSize; i += PAGE_SIZE)
    {
        for(j = 0; j < PAGE_SIZE; j += 4)
        {
            FMC_Read(LDROM_BASE + i + j, &u32Data);
            if (u32Data != pu32Loader[(i+j)/4])
                i32Err = 1;
            
            if (i + j >= u32ImageSize)
                break;
        }
    }

    if(i32Err)
    {
        printf("[FAIL]\n");
    }
    else
    {
        printf("[OK]\n");
        
        /* Reset CPU to boot to LD mode */
        printf("\n  >>> Reset to LD mode <<<\n");
        FMC_BootSelect(E_FMC_LDROM);
        GCR->IPRST_CTL1 = GCR_IPRSTCTL1_CPU;
    }


lexit:

 	/* Disable FMC ISP function */
	FMC_DeInit();

	/* Lock protected registers */
	LOCKREG();
    
	printf("\nFMC Sample Code Completed.\n");

}


#ifdef USE_ASSERT
/**
  * @brief  The function prints the source file name and line number where the assert_param() error
  *         occurs, and then stops in an infinite loop. User can add his own codes here if necessary.
  * @param[in] file Source file name
  * @param[in] line Line number
  * @return None
  */
void assert_error(uint8_t * file, uint32_t line)
{
 	GCR->PB_L_MFP = (GCR->PB_L_MFP & ~0x77) | (PB1_MFP_UART0_TX | PB0_MFP_UART0_RX);  /* Select multi-function pin for UART0 */
 	CLK->APBCLK |= CLK_APBCLK_UART0_EN;    /* Enable UART0 clock */
	CLK->CLKDIV0 &= ~CLK_CLKDIV0_UART_MASK;
	CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UART_MASK) | CLK_CLKSEL1_UART_HXT;  /* Select 12 Mhz XTAL */

 
    /* Set UART to 115200,n,8,1,none */
    UART0->BAUD = 0x67;             /* Baud Rate:115200 for 12MHz */
    UART0->TLCTL = 0x03;            /* Word len is 8 bits */          
	
	printf("[%s] line %d : wrong parameters.\r\n", file, line);

	/* Infinite loop */
	while(1) ;

}
#endif

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
