/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "DrvFMC.h"
#include "DrvSYS.h"


#define CONFIG0         0x00300000
#define CONFIG1         0x00300004

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_Write                                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32addr    - [in]      Flash address include APROM, LDROM, Data Flash, and Config         */
/*               u32data    - [in]      Word Data                                                          */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               0       Success                                                                           */
/*              <0       Failed when illegal condition occurs                                              */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Write data into Flash include APROM, LDROM, Data Flash, and Config                        */
/*               The corresponding functions in Config0 & Config1 are listed in FMC section of TRM         */
/*                                                                                                         */
/* Note:                                                                                                   */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function.                                                                            */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvFMC_Write(uint32_t u32addr, uint32_t u32data)
{
    FMC->ISPCMD.FCTRL = 1;
    FMC->ISPCMD.FCEN = 0;
    FMC->ISPCMD.FOEN = 1;
    FMC->ISPADR = u32addr;
    FMC->ISPDAT = u32data;
    __set_PRIMASK(1);
    FMC->ISPTRG.ISPGO = 1;
    __ISB();
    while (FMC->ISPTRG.ISPGO);
    __set_PRIMASK(0);
    
    if (FMC->ISPCON.ISPFF == 1)
    {
        FMC->ISPCON.ISPFF = 1;
        return E_DRVFMC_ERR_ISP_FAIL;
    }
    
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_Read                                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32addr    - [in]      Flash address include APROM, LDROM, Data Flash, and Config         */
/*               u32data    - [out]     Data                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               0       Success                                                                           */
/*              <0       Failed when illegal condition occurs                                              */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Read data from Flash include APROM, LDROM, Data Flash, and Config                         */
/*                                                                                                         */
/* Note:                                                                                                   */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function.                                                                            */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvFMC_Read(uint32_t u32addr, uint32_t * u32data)
{ 
    FMC->ISPCMD.FCTRL = 0;
    FMC->ISPCMD.FCEN = 0;
    FMC->ISPCMD.FOEN = 0;
    FMC->ISPADR = u32addr;
    FMC->ISPDAT = 0;
    __set_PRIMASK(1);
    FMC->ISPTRG.ISPGO = 1;    
    __ISB();
    while (FMC->ISPTRG.ISPGO);
    __set_PRIMASK(0);
    
    if (FMC->ISPCON.ISPFF == 1)
    {
        FMC->ISPCON.ISPFF = 1;
        return E_DRVFMC_ERR_ISP_FAIL;
    }
    
    *u32data = FMC->ISPDAT;
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_Erase                                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32addr    - [in]      Flash page base address or Config0 address                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               0       Success                                                                           */
/*              <0       Failed when illegal condition occurs                                              */
/*                                                                                                         */
/* Description:                                                                                            */
/*             Page Erase Flash include APROM, LDROM, Data Flash, and Config. The erase unit is 512 bytes  */
/*                                                                                                         */
/* Note:                                                                                                   */
/*             Please make sure that Register Write-Protection Function has been disabled before using     */
/*             this function.                                                                              */
/*             User can check the status of Register Write-Protection Function                             */
/*             with DrvSYS_IsProtectedRegLocked().                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvFMC_Erase(uint32_t u32addr)
{
    FMC->ISPCMD.FCTRL = 2;
    FMC->ISPCMD.FCEN = 0;
    FMC->ISPCMD.FOEN = 1;
    FMC->ISPADR = u32addr;
    __set_PRIMASK(1);
    FMC->ISPTRG.ISPGO = 1;  
    __ISB();
    while (FMC->ISPTRG.ISPGO);  
    __set_PRIMASK(0);
    
    if (FMC->ISPCON.ISPFF == 1)
    {
        FMC->ISPCON.ISPFF = 1;
        return E_DRVFMC_ERR_ISP_FAIL;
    }
    
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_EnableISP                                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable ISP function. This function will check if internal oscillator 22M is enabled or not*/                                                   
/*               If not, this function will enable 22M oscillator automatically.                           */
/*                                                                                                         */
/* Note:                                                                                                   */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function to enable ISP function.                                                     */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_EnableISP(void)
{
    if (SYSCLK->PWRCON.OSC22M_EN == 0)
    {
        SYSCLK->PWRCON.OSC22M_EN = 1;
        
        /* Wait 22M stable */
        DrvSYS_Delay(12);
    }

    SYSCLK->AHBCLK.ISP_EN = 1;
    FMC->ISPCON.ISPEN = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_DisableISP                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable ISP function.                                                                     */                                                   
/*                                                                                                         */
/* Note:                                                                                                   */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function to disable ISP function.                                                    */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_DisableISP(void)
{
    FMC->ISPCON.ISPEN = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_BootSelect                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               boot   - [in]      E_FMC_APROM / E_FMC_LDROM                                              */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Select next boot from APROM or LDROM                                                      */
/*                                                                                                         */
/* Note:                                                                                                   */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function.                                                                            */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_BootSelect(E_FMC_BOOTSELECT boot)
{
    FMC->ISPCON.BS = (boot)? 1: 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_GetBootSelect                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_FMC_APROM      The current boot select setting is in APROM                              */
/*               E_FMC_LDROM      The current boot select setting is in LDROM                              */
/*                                                                                                         */
/* Description:                                                                                            */
/*               To return current boot select setting.                                                    */
/*---------------------------------------------------------------------------------------------------------*/
E_FMC_BOOTSELECT DrvFMC_GetBootSelect(void)
{
    return (E_FMC_BOOTSELECT)FMC->ISPCON.BS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_EnableLDUpdate                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable LDROM update function                                                              */
/*                                                                                                         */
/* Note:                                                                                                   */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function to enable LD update function.                                               */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_EnableLDUpdate(void)
{   
    FMC->ISPCON.LDUEN = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_DisableLDUpdate                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable LDROM update function                                                             */
/*                                                                                                         */
/* Note:                                                                                                   */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function to disable LD update function.                                              */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_DisableLDUpdate(void)
{   
    FMC->ISPCON.LDUEN = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_EnableConfigUpdate                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable Config update function when MCU is running in APROM or LDROM                       */
/*                                                                                                         */
/* Note:                                                                                                   */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function to enable Config update function.                                           */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_EnableConfigUpdate(void)
{   
    FMC->ISPCON.CFGUEN = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_DisableConfigUpdate                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable Config update function when MCU is running in APROM or LDROM                      */
/*                                                                                                         */
/* Note:                                                                                                   */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function to disable Config update function.                                          */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_DisableConfigUpdate(void)
{   
    FMC->ISPCON.CFGUEN = 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_EnableAPUpdate                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable APROM update function. APROM can be updated when MCU is running in APROM.          */
/*                                                                                                         */
/* Note1:        Only NuMicro NUC1x0xxxCx Series (Ex. NUC140VE3CN) support this function.                  */
/*                                                                                                         */
/* Note2:        User must be very careful when this function is enabled.                                  */
/*                                                                                                         */
/* Note3:                                                                                                  */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function to enable AP update function.                                               */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_EnableAPUpdate(void)
{   
    FMC->ISPCON.APUEN = 1;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_DisableAPUpdate                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable APROM update function.                                                            */
/*                                                                                                         */
/* Note1:        Only NuMicro NUC1x0xxxCx Series (Ex. NUC140VE3CN) support this function.                  */
/*                                                                                                         */
/* Note2:                                                                                                  */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function to disable AP update function.                                              */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_DisableAPUpdate(void)
{   
    FMC->ISPCON.APUEN = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_EnablePowerSaving                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable flash power saving function                                                        */
/*                                                                                                         */
/* Note:                                                                                                   */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function to enable flash power saving function.                                      */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_EnablePowerSaving(void)
{   
    FMC->FATCON.FPSEN = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_DisablePowerSaving                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable flash power saving function                                                       */
/*                                                                                                         */
/* Note:                                                                                                   */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function to disable flash power saving function.                                     */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_DisablePowerSaving(void)
{   
    FMC->FATCON.FPSEN = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_EnableLowFreqOptMode                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable flash access low frequency optimization mode. It can improve flash access          */
/*               performance when CPU runs at low frequency.                                               */
/*                                                                                                         */
/* Note1:        Only NuMicro NUC1x0xxxCx Series (Ex. NUC140VE3CN) and Low Density Series support          */
/*               this function.                                                                            */
/* Note2:        Set this bit only when HCLK <= 25MHz.                                                     */
/*               If HCLK > 25MHz, CPU will fetch wrong code and cause fail result.                         */
/* Note3:                                                                                                  */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function to enable flash access low frequency optimization mode.                     */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_EnableLowFreqOptMode(void)
{   
    FMC->FATCON.LFOM = 1;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_DisableLowFreqOptMode                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable flash access low frequency optimization mode.                                     */
/*                                                                                                         */
/* Note1:        Only NuMicro NUC1x0xxxCx Series (Ex. NUC140VE3CN) and Low Density Series support          */
/*               this function.                                                                            */
/* Note2:                                                                                                  */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function to disable flash access low frequency optimization mode.                    */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_DisableLowFreqOptMode(void)
{   
    FMC->FATCON.LFOM = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_ReadDataFlashBaseAddr                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               Data Flash base address                                                                   */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Read Data Flash base address                                                              */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvFMC_ReadDataFlashBaseAddr(void)
{   
    return FMC->DFBADR;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_WriteConfig                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32data0   - [in]      Config0 data                                                       */
/*               u32data1   - [in]      Config1 data                                                       */
/*                                                                                                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               0       Success                                                                           */
/*              <0       Failed when illegal condition occurs                                              */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Erase Config and write data into Config0 and Config1                                      */
/*               The corresponding functions in Config0 & Config1 are listed in FMC section of TRM         */
/*                                                                                                         */
/* Note:                                                                                                   */
/*               Please make sure that Register Write-Protection Function has been disabled before using   */
/*               this function.                                                                            */
/*               User can check the status of Register Write-Protection Function                           */
/*               with DrvSYS_IsProtectedRegLocked().                                                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvFMC_WriteConfig(uint32_t u32data0, uint32_t u32data1)
{       
    if ( DrvFMC_Erase(CONFIG0) != E_SUCCESS )
        return E_DRVFMC_ERR_ISP_FAIL;
    
    if ( DrvFMC_Write(CONFIG0, u32data0) != E_SUCCESS )
        return E_DRVFMC_ERR_ISP_FAIL;

    return DrvFMC_Write(CONFIG1, u32data1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_GetVersion                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               The DrvFMC version number                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to get the DrvFMC version number                                    */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvFMC_GetVersion(void)
{
    return DRVFMC_VERSION_NUM;
}

