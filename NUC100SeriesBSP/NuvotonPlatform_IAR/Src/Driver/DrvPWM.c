/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include "DrvPWM.h"
#include "DrvSYS.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define PWMA_CNR0        PWMA_BASE + 0x0C
#define PWMA_CMR0        PWMA_BASE + 0x10
#define PWMA_PDR0        PWMA_BASE + 0x14
#define PWMA_CRLR0       PWMA_BASE + 0x58
#define PWMA_CFLR0       PWMA_BASE + 0x5C

#define PWMB_CNR0        PWMB_BASE + 0x0C
#define PWMB_CMR0        PWMB_BASE + 0x10
#define PWMB_PDR0        PWMB_BASE + 0x14
#define PWMB_CRLR0       PWMB_BASE + 0x58
#define PWMB_CFLR0       PWMB_BASE + 0x5C

#define CCR_MASK         0x000F000F

/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static S_DRVPWM_CALLBACK_T g_sDrvPWMAHandler = {0};
static S_DRVPWM_CALLBACK_T g_sDrvPWMBHandler = {0};

static int32_t s_i32Option = 0;
 
/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_GetVersion                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               The DrvPWM version number                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to get the DrvPWM version number                                    */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPWM_GetVersion(void)
{
    return DRVPWM_VERSION_NUM;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     <PWMA_IRQHandler>                                                                         */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               ISR to handle PWMA interrupt event                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void PWMA_IRQHandler(void)
{    
    uint32_t u32pwmIntFlag, u32CapIntFlag0, u32CapIntFlag1;
    
    /* Handle PWMA Timer function */    
    u32pwmIntFlag = *((__IO uint32_t *) &PWMA->PIIR);
    
    if (u32pwmIntFlag & BIT0)
    {
        *((__IO uint32_t *) &PWMA->PIIR) = BIT0;         
        if (g_sDrvPWMAHandler.pfnPWM0CallBack != NULL)
        {                           
            g_sDrvPWMAHandler.pfnPWM0CallBack();
        }   
    }   

    if (u32pwmIntFlag & BIT1)
    {
        *((__IO uint32_t *) &PWMA->PIIR) = BIT1;         
        if (g_sDrvPWMAHandler.pfnPWM1CallBack != NULL)
        {
            g_sDrvPWMAHandler.pfnPWM1CallBack();
        }   
    }   
    
    if (u32pwmIntFlag & BIT2)
    {
        *((__IO uint32_t *) &PWMA->PIIR) = BIT2;         
        if (g_sDrvPWMAHandler.pfnPWM2CallBack != NULL)
        {
            g_sDrvPWMAHandler.pfnPWM2CallBack();
        }   
    }

    if (u32pwmIntFlag & BIT3)
    {
        *((__IO uint32_t *) &PWMA->PIIR) = BIT3;         
        if (g_sDrvPWMAHandler.pfnPWM3CallBack != NULL)
        {
            g_sDrvPWMAHandler.pfnPWM3CallBack();
        }   
    }
    
    /* Handle PWMA Capture function */
    u32CapIntFlag0 = *((__IO uint32_t *) &PWMA->CCR0);
    u32CapIntFlag1 = *((__IO uint32_t *) &PWMA->CCR2);

    if (u32CapIntFlag0 & BIT4) 
    {
        if (s_i32Option)
            outp32(&PWMA->CCR0, inp32(&PWMA->CCR0) & (CCR_MASK | BIT4));
        else
            outp32(&PWMA->CCR0, inp32(&PWMA->CCR0) & ~BIT20);   

        if (g_sDrvPWMAHandler.pfnCAP0CallBack != NULL)
        {
            g_sDrvPWMAHandler.pfnCAP0CallBack();
        }
    }
    
    if (u32CapIntFlag0 & BIT20) 
    {
        if (s_i32Option)
            outp32(&PWMA->CCR0, inp32(&PWMA->CCR0) & (CCR_MASK | BIT20));
        else
            outp32(&PWMA->CCR0, inp32(&PWMA->CCR0) & ~BIT4);        

        if (g_sDrvPWMAHandler.pfnCAP1CallBack != NULL)
        {
            g_sDrvPWMAHandler.pfnCAP1CallBack();
        }
    }

    if (u32CapIntFlag1 & BIT4) 
    {
        if (s_i32Option)
            outp32(&PWMA->CCR2, inp32(&PWMA->CCR2) & (CCR_MASK | BIT4));
        else
            outp32(&PWMA->CCR2, inp32(&PWMA->CCR2) & ~BIT20);

        if (g_sDrvPWMAHandler.pfnCAP2CallBack != NULL)
        {
            g_sDrvPWMAHandler.pfnCAP2CallBack();
        }
    }
    
    if (u32CapIntFlag1 & BIT20) 
    {
        if (s_i32Option)
            outp32(&PWMA->CCR2, inp32(&PWMA->CCR2) & (CCR_MASK | BIT20));
        else
            outp32(&PWMA->CCR2, inp32(&PWMA->CCR2) & ~BIT4);

        if (g_sDrvPWMAHandler.pfnCAP3CallBack != NULL)
        {
            g_sDrvPWMAHandler.pfnCAP3CallBack();
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     <PWMB_IRQHandler>                                                                         */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               ISR to handle PWMB interrupt event                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void PWMB_IRQHandler(void)
{    
    uint32_t u32pwmIntFlag, u32CapIntFlag0, u32CapIntFlag1;

    /* Handle PWMB Timer function */
    u32pwmIntFlag = *((__IO uint32_t *) &PWMB->PIIR);
    
    if (u32pwmIntFlag & BIT0)
    {
        *((__IO uint32_t *) &PWMB->PIIR) = BIT0;         
        if (g_sDrvPWMBHandler.pfnPWM0CallBack != NULL)
        {
            g_sDrvPWMBHandler.pfnPWM0CallBack();
        }   
    }   

    if (u32pwmIntFlag & BIT1)
    {
        *((__IO uint32_t *) &PWMB->PIIR) = BIT1;         
        if (g_sDrvPWMBHandler.pfnPWM1CallBack != NULL)
        {
            g_sDrvPWMBHandler.pfnPWM1CallBack();
        }   
    }   
    
    if (u32pwmIntFlag & BIT2)
    {
        *((__IO uint32_t *) &PWMB->PIIR) = BIT2;         
        if (g_sDrvPWMBHandler.pfnPWM2CallBack != NULL)
        {
            g_sDrvPWMBHandler.pfnPWM2CallBack();
        }   
    }

    if (u32pwmIntFlag & BIT3)
    {
        *((__IO uint32_t *) &PWMB->PIIR) = BIT3;         
        if (g_sDrvPWMBHandler.pfnPWM3CallBack != NULL)
        {
            g_sDrvPWMBHandler.pfnPWM3CallBack();
        }   
    }

    /* Handle PWMB Capture function */
    u32CapIntFlag0 = *((__IO uint32_t *) &PWMB->CCR0);
    u32CapIntFlag1 = *((__IO uint32_t *) &PWMB->CCR2);

    if (u32CapIntFlag0 & BIT4) 
    {
        if (s_i32Option)
            outp32(&PWMB->CCR0, inp32(&PWMB->CCR0) & (CCR_MASK | BIT4));
        else
            outp32(&PWMB->CCR0, inp32(&PWMB->CCR0) & ~BIT20); 
        
        if (g_sDrvPWMBHandler.pfnCAP0CallBack != NULL)
        {
            g_sDrvPWMBHandler.pfnCAP0CallBack();
        }
    }
    
    if (u32CapIntFlag0 & BIT20) 
    {
        if (s_i32Option)
            outp32(&PWMB->CCR0, inp32(&PWMB->CCR0) & (CCR_MASK | BIT20));
        else
            outp32(&PWMB->CCR0, inp32(&PWMB->CCR0) & ~BIT4);  
                    
        if (g_sDrvPWMBHandler.pfnCAP1CallBack != NULL)
        {
            g_sDrvPWMBHandler.pfnCAP1CallBack();
        }
    }

    if (u32CapIntFlag1 & BIT4) 
    {
        if (s_i32Option)
            outp32(&PWMB->CCR2, inp32(&PWMB->CCR2) & (CCR_MASK | BIT4));
        else
            outp32(&PWMB->CCR2, inp32(&PWMB->CCR2) & ~BIT20);
                   
        if (g_sDrvPWMBHandler.pfnCAP2CallBack != NULL)
        {
            g_sDrvPWMBHandler.pfnCAP2CallBack();
        }
    }
    
    if (u32CapIntFlag1 & BIT20) 
    {
        if (s_i32Option)
            outp32(&PWMB->CCR2, inp32(&PWMB->CCR2) & (CCR_MASK | BIT20));
        else
            outp32(&PWMB->CCR2, inp32(&PWMB->CCR2) & ~BIT4);
                    
        if (g_sDrvPWMBHandler.pfnCAP3CallBack != NULL)
        {
            g_sDrvPWMBHandler.pfnCAP3CallBack();
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_IsTimerEnabled                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Timer   - [in]       DRVPWM_TIMER0 / DRVPWM_TIMER1 / DRVPWM_TIMER2 / DRVPWM_TIMER3      */
/*                                      DRVPWM_TIMER4 / DRVPWM_TIMER5 / DRVPWM_TIMER6 / DRVPWM_TIMER7      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               0      disable                                                                            */
/*               1      enable                                                                             */
/* Description:                                                                                            */
/*               This function is used to get PWMA/PWMB specified timer enable/disable state               */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPWM_IsTimerEnabled(uint8_t u8Timer)
{
    int32_t status;

    status = 0;
    
    switch (u8Timer)
    {
        case DRVPWM_TIMER0:
        case DRVPWM_TIMER1:
        case DRVPWM_TIMER2:
        case DRVPWM_TIMER3:
            status = ( inp32(&PWMA->PCR) & (1 << (u8Timer<<3) ) )? 1 : 0;
            break;      
        case DRVPWM_TIMER4:
        case DRVPWM_TIMER5:
        case DRVPWM_TIMER6:
        case DRVPWM_TIMER7:
            status = ( inp32(&PWMB->PCR) & (1 << ((u8Timer-DRVPWM_TIMER4)<<3) ) )? 1 : 0;
            break;  
    }
    return status;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_SetTimerCounter                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Timer        - [in]      DRVPWM_TIMER0 / DRVPWM_TIMER1 / DRVPWM_TIMER2 / DRVPWM_TIMER3  */
/*                                          DRVPWM_TIMER4 / DRVPWM_TIMER5 / DRVPWM_TIMER6 / DRVPWM_TIMER7  */
/*               u16Counter     - [in]      Timer counter : 0~65535                                        */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               This function is used to set the PWM0~7 specified timer counter                           */
/* Note:         If the counter is set to 0, the timer will stop.                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPWM_SetTimerCounter(uint8_t u8Timer, uint16_t u16Counter)
{   
    u8Timer &= 0x0F;
    
    if (u8Timer >= DRVPWM_TIMER4)
    {
        *((__IO uint32_t *) (PWMB_CNR0 + (u8Timer - DRVPWM_TIMER4) * 12)) = u16Counter;
    }
    else
    {
        *((__IO uint32_t *) (PWMA_CNR0 + (u8Timer & 0x7) * 12)) = u16Counter;   
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_GetTimerCounter                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Timer        - [in]      DRVPWM_TIMER0 / DRVPWM_TIMER1 / DRVPWM_TIMER2 / DRVPWM_TIMER3  */
/*                                          DRVPWM_TIMER4 / DRVPWM_TIMER5 / DRVPWM_TIMER6 / DRVPWM_TIMER7  */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               The specified timer counter value                                                         */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to get the PWM0~7 specified timer counter value                     */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPWM_GetTimerCounter(uint8_t u8Timer)
{
    uint32_t u32Reg = 0;
    
    u8Timer &= 0x0F;
    
    if (u8Timer >= DRVPWM_TIMER4)
    {
        u32Reg = *((__IO uint32_t *) (PWMB_PDR0 + (u8Timer - DRVPWM_TIMER4) * 12));
    }
    else
    {
        u32Reg = *((__IO uint32_t *) (PWMA_PDR0 + (u8Timer & 0x7) * 12));   
    }

    return u32Reg;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_EnableInt                                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Timer        - [in]      DRVPWM_TIMER0 / DRVPWM_TIMER1 / DRVPWM_TIMER2 / DRVPWM_TIMER3  */
/*                                          DRVPWM_TIMER4 / DRVPWM_TIMER5 / DRVPWM_TIMER6 / DRVPWM_TIMER7  */
/*                                          DRVPWM_CAP0   / DRVPWM_CAP1   / DRVPWM_CAP2   / DRVPWM_CAP3    */
/*                                          DRVPWM_CAP4   / DRVPWM_CAP5   / DRVPWM_CAP6   / DRVPWM_CAP7    */
/*               u8Int          - [in]      DRVPWM_CAP_RISING_INT/DRVPWM_CAP_FALLING_INT/DRVPWM_CAP_ALL_INT*/
/*                                          (The parameter is valid only when capture function)            */
/*               pfncallback    - [in]      The call back function for specified timer / capture           */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to Enable the PWM0~7 timer/capture interrupt                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPWM_EnableInt(uint8_t u8Timer, uint8_t u8Int, PFN_DRVPWM_CALLBACK pfncallback)
{
    int32_t Rflag, Fflag;   

    Rflag = u8Int & 0x01;
    Fflag = u8Int & 0x02;
        
    switch (u8Timer)
    {   
        case DRVPWM_TIMER0:
            PWMA->PIER.PWMIE0 = 1;
            g_sDrvPWMAHandler.pfnPWM0CallBack = pfncallback;
            break;
        case DRVPWM_CAP0:           
            if (Rflag)
            {
                if (s_i32Option)
                    outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & CCR_MASK) | BIT1);
                else
                    outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & ~(BIT4 | BIT20)) | BIT1); 
            }
            if (Fflag)
            {
                if (s_i32Option)
                    outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & CCR_MASK) | BIT2);
                else
                    outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & ~(BIT4 | BIT20)) | BIT2);
            }
            g_sDrvPWMAHandler.pfnCAP0CallBack = pfncallback;
            break;  
        case DRVPWM_TIMER1:
            PWMA->PIER.PWMIE1 = 1;
            g_sDrvPWMAHandler.pfnPWM1CallBack = pfncallback;
            break;
        case DRVPWM_CAP1:           
            if (Rflag)
            {
                if (s_i32Option)
                    outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & CCR_MASK) | BIT17);
                else
                    outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & ~(BIT4 | BIT20)) | BIT17);            
            }
            if (Fflag)
            {
                if (s_i32Option)
                    outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & CCR_MASK) | BIT18);
                else
                    outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & ~(BIT4 | BIT20)) | BIT18);            
            }
            g_sDrvPWMAHandler.pfnCAP1CallBack = pfncallback;
            break;
        case DRVPWM_TIMER2:
            PWMA->PIER.PWMIE2 = 1;
            g_sDrvPWMAHandler.pfnPWM2CallBack = pfncallback;
            break;
        case DRVPWM_CAP2:           
            if (Rflag)
            {
                if (s_i32Option)
                    outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & CCR_MASK) | BIT1);
                else
                    outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & ~(BIT4 | BIT20)) | BIT1);             
            }
            if (Fflag)
            {
                if (s_i32Option)
                    outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & CCR_MASK) | BIT2);
                else
                    outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & ~(BIT4 | BIT20)) | BIT2);         
            }
            g_sDrvPWMAHandler.pfnCAP2CallBack = pfncallback;
            break;
        case DRVPWM_TIMER3:
            PWMA->PIER.PWMIE3 = 1;
            g_sDrvPWMAHandler.pfnPWM3CallBack = pfncallback;
            break;
        case DRVPWM_CAP3:           
            if (Rflag)
            {
                if (s_i32Option)
                    outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & CCR_MASK) | BIT17);
                else
                    outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & ~(BIT4 | BIT20)) | BIT17);                
            }
            if (Fflag)
            {
                if (s_i32Option)
                    outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & CCR_MASK) | BIT18);
                else
                    outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & ~(BIT4 | BIT20)) | BIT18);                
            }
            g_sDrvPWMAHandler.pfnCAP3CallBack = pfncallback;
            break;          
            
        case DRVPWM_TIMER4:
            PWMB->PIER.PWMIE0 = 1;
            g_sDrvPWMBHandler.pfnPWM0CallBack = pfncallback;
            break;
        case DRVPWM_CAP4:           
            if (Rflag)
            {
                if (s_i32Option)
                    outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & CCR_MASK) | BIT1);
                else
                    outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & ~(BIT4 | BIT20)) | BIT1); 
            }
            if (Fflag)
            {
                if (s_i32Option)
                    outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & CCR_MASK) | BIT2);
                else
                    outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & ~(BIT4 | BIT20)) | BIT2);
            }

            g_sDrvPWMBHandler.pfnCAP0CallBack = pfncallback;
            break;  
        case DRVPWM_TIMER5:
            PWMB->PIER.PWMIE1 = 1;
            g_sDrvPWMBHandler.pfnPWM1CallBack = pfncallback;
            break;
        case DRVPWM_CAP5:           
            if (Rflag)
            {
                if (s_i32Option)
                    outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & CCR_MASK) | BIT17);
                else
                    outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & ~(BIT4 | BIT20)) | BIT17);            
            }
            if (Fflag)
            {
                if (s_i32Option)
                    outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & CCR_MASK) | BIT18);
                else
                    outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & ~(BIT4 | BIT20)) | BIT18);            
            }

            g_sDrvPWMBHandler.pfnCAP1CallBack = pfncallback;
            break;
        case DRVPWM_TIMER6:
            PWMB->PIER.PWMIE2 = 1;
            g_sDrvPWMBHandler.pfnPWM2CallBack = pfncallback;
            break;
        case DRVPWM_CAP6:           
            if (Rflag)
            {
                if (s_i32Option)
                    outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & CCR_MASK) | BIT1);
                else
                    outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & ~(BIT4 | BIT20)) | BIT1);             
            }
            if (Fflag)
            {
                if (s_i32Option)
                    outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & CCR_MASK) | BIT2);
                else
                    outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & ~(BIT4 | BIT20)) | BIT2);         
            }
            g_sDrvPWMBHandler.pfnCAP2CallBack = pfncallback;
            break;
        case DRVPWM_TIMER7:
            PWMB->PIER.PWMIE3 = 1;
            g_sDrvPWMBHandler.pfnPWM3CallBack = pfncallback;
            break;
        case DRVPWM_CAP7:           
            if (Rflag)
            {
                if (s_i32Option)
                    outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & CCR_MASK) | BIT17);
                else
                    outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & ~(BIT4 | BIT20)) | BIT17);                
            }
            if (Fflag)
            {
                if (s_i32Option)
                    outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & CCR_MASK) | BIT18);
                else
                    outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & ~(BIT4 | BIT20)) | BIT18);                
            }

            g_sDrvPWMBHandler.pfnCAP3CallBack = pfncallback;
            break;              
    }                           
        
    if (u8Timer & 0x04)
    {
        NVIC_SetPriority(PWMB_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(PWMB_IRQn);
    }
    else
    {
        NVIC_SetPriority(PWMA_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(PWMA_IRQn);           
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_DisableInt                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Timer        - [in]      DRVPWM_TIMER0 / DRVPWM_TIMER1 / DRVPWM_TIMER2 / DRVPWM_TIMER3  */
/*                                          DRVPWM_TIMER4 / DRVPWM_TIMER5 / DRVPWM_TIMER6 / DRVPWM_TIMER7  */
/*                                          DRVPWM_CAP0   / DRVPWM_CAP1   / DRVPWM_CAP2   / DRVPWM_CAP3    */
/*                                          DRVPWM_CAP4   / DRVPWM_CAP5   / DRVPWM_CAP6   / DRVPWM_CAP7    */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to disable the PWM0~7 timer/capture interrupt                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPWM_DisableInt(uint8_t u8Timer)
{       
    switch(u8Timer)
    {   
        case DRVPWM_TIMER0:
            PWMA->PIER.PWMIE0 = 0;
            outp32(&PWMA->PIIR, BIT0);
            g_sDrvPWMAHandler.pfnPWM0CallBack = NULL;
            break;
        case DRVPWM_TIMER1:
            PWMA->PIER.PWMIE1 = 0;
            outp32(&PWMA->PIIR, BIT1);
            g_sDrvPWMAHandler.pfnPWM1CallBack = NULL;
            break;          
        case DRVPWM_TIMER2:
            PWMA->PIER.PWMIE2 = 0;
            outp32(&PWMA->PIIR, BIT2);
            g_sDrvPWMAHandler.pfnPWM2CallBack = NULL;
            break;      
        case DRVPWM_TIMER3:
            PWMA->PIER.PWMIE3 = 0;
            outp32(&PWMA->PIIR, BIT3);
            g_sDrvPWMAHandler.pfnPWM3CallBack = NULL;
            break;
        case DRVPWM_TIMER4:
            PWMB->PIER.PWMIE0 = 0;
            outp32(&PWMB->PIIR, BIT0);
            g_sDrvPWMBHandler.pfnPWM0CallBack = NULL;
            break;
        case DRVPWM_TIMER5:
            PWMB->PIER.PWMIE1 = 0;
            outp32(&PWMB->PIIR, BIT1);
            g_sDrvPWMBHandler.pfnPWM1CallBack = NULL;
            break;          
        case DRVPWM_TIMER6:
            PWMB->PIER.PWMIE2 = 0;
            outp32(&PWMB->PIIR, BIT2);
            g_sDrvPWMBHandler.pfnPWM2CallBack = NULL;
            break;      
        case DRVPWM_TIMER7:
            PWMB->PIER.PWMIE3 = 0;
            outp32(&PWMB->PIIR, BIT3);
            g_sDrvPWMBHandler.pfnPWM3CallBack = NULL;
            break;
        case DRVPWM_CAP0:
            if (s_i32Option)
                outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & CCR_MASK) & ~(BIT1 | BIT2));
            else
                outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & ~(BIT4 | BIT20)) & ~(BIT1 | BIT2));   
            g_sDrvPWMAHandler.pfnCAP0CallBack = NULL;   
            break;              
        case DRVPWM_CAP1:
            if (s_i32Option)
                outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & CCR_MASK) & ~(BIT17 | BIT18));
            else
                outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & ~(BIT4 | BIT20)) & ~(BIT17 | BIT18));
            g_sDrvPWMAHandler.pfnCAP1CallBack = NULL;   
            break;      
        case DRVPWM_CAP2:
            if (s_i32Option)
                outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & CCR_MASK) & ~(BIT1 | BIT2));
            else
                outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & ~(BIT4 | BIT20)) & ~(BIT1 | BIT2));
            g_sDrvPWMAHandler.pfnCAP2CallBack = NULL;   
            break;
        case DRVPWM_CAP3:
            if (s_i32Option)
                outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & CCR_MASK) & ~(BIT17 | BIT18));
            else
                outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & ~(BIT4 | BIT20)) & ~(BIT17 | BIT18));
            g_sDrvPWMAHandler.pfnCAP3CallBack = NULL;   
            break; 
        case DRVPWM_CAP4:   
            if (s_i32Option)
                outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & CCR_MASK) & ~(BIT1 | BIT2));
            else
                outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & ~(BIT4 | BIT20)) & ~(BIT1 | BIT2));   

            g_sDrvPWMBHandler.pfnCAP0CallBack = NULL;   
            break;              
        case DRVPWM_CAP5:
            if (s_i32Option)
                outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & CCR_MASK) & ~(BIT17 | BIT18));
            else
                outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & ~(BIT4 | BIT20)) & ~(BIT17 | BIT18));

            g_sDrvPWMBHandler.pfnCAP1CallBack = NULL;   
            break;      
        case DRVPWM_CAP6:
            if (s_i32Option)
                outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & CCR_MASK) & ~(BIT1 | BIT2));
            else
                outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & ~(BIT4 | BIT20)) & ~(BIT1 | BIT2));

            g_sDrvPWMBHandler.pfnCAP2CallBack = NULL;   
            break;
        case DRVPWM_CAP7:
            if (s_i32Option)
                outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & CCR_MASK) & ~(BIT17 | BIT18));
            else
                outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & ~(BIT4 | BIT20)) & ~(BIT17 | BIT18));

            g_sDrvPWMBHandler.pfnCAP3CallBack = NULL;   
            break; 
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_ClearInt                                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Timer        - [in]      DRVPWM_TIMER0 / DRVPWM_TIMER1 / DRVPWM_TIMER2 / DRVPWM_TIMER3  */
/*                                          DRVPWM_TIMER4 / DRVPWM_TIMER5 / DRVPWM_TIMER6 / DRVPWM_TIMER7  */
/*                                          DRVPWM_CAP0   / DRVPWM_CAP1   / DRVPWM_CAP2   / DRVPWM_CAP3    */
/*                                          DRVPWM_CAP4   / DRVPWM_CAP5   / DRVPWM_CAP6   / DRVPWM_CAP7    */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to clear the PWM0~7 timer/capture interrupt flag                    */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPWM_ClearInt(uint8_t u8Timer)
{
    switch (u8Timer)
    {
        case DRVPWM_TIMER0:
        case DRVPWM_TIMER1:
        case DRVPWM_TIMER2:
        case DRVPWM_TIMER3:
            outp32(&PWMA->PIIR, (1 << u8Timer) );
            break;
        case DRVPWM_TIMER4:
        case DRVPWM_TIMER5:
        case DRVPWM_TIMER6:
        case DRVPWM_TIMER7:
            outp32(&PWMB->PIIR, (1 << (u8Timer-DRVPWM_TIMER4)) );
            break;

        case DRVPWM_CAP0:
            if (s_i32Option)
                outp32(&PWMA->CCR0, inp32(&PWMA->CCR0) & (CCR_MASK | BIT4));
            else
                outp32(&PWMA->CCR0, inp32(&PWMA->CCR0) & ~BIT20);
            break;
        case DRVPWM_CAP1:
            if (s_i32Option)
                outp32(&PWMA->CCR0, inp32(&PWMA->CCR0) & (CCR_MASK | BIT20));
            else
                outp32(&PWMA->CCR0, inp32(&PWMA->CCR0) & ~BIT4);
            break;
        case DRVPWM_CAP2:
            if (s_i32Option)
                outp32(&PWMA->CCR2, inp32(&PWMA->CCR2) & (CCR_MASK | BIT4));
            else
                outp32(&PWMA->CCR2, inp32(&PWMA->CCR2) & ~BIT20);
            break;
        case DRVPWM_CAP3:
            if (s_i32Option)
                outp32(&PWMA->CCR2, inp32(&PWMA->CCR2) & (CCR_MASK | BIT20));
            else
                outp32(&PWMA->CCR2, inp32(&PWMA->CCR2) & ~BIT4);
            break;
        case DRVPWM_CAP4:
            if (s_i32Option)
                outp32(&PWMB->CCR0, inp32(&PWMB->CCR0) & (CCR_MASK | BIT4));
            else
                outp32(&PWMB->CCR0, inp32(&PWMB->CCR0) & ~BIT20);
            break;
        case DRVPWM_CAP5:
            if (s_i32Option)
                outp32(&PWMB->CCR0, inp32(&PWMB->CCR0) & (CCR_MASK | BIT20));
            else
                outp32(&PWMB->CCR0, inp32(&PWMB->CCR0) & ~BIT4);
            break;
        case DRVPWM_CAP6:
            if (s_i32Option)
                outp32(&PWMB->CCR2, inp32(&PWMB->CCR2) & (CCR_MASK | BIT4));
            else
                outp32(&PWMB->CCR2, inp32(&PWMB->CCR2) & ~BIT20);
            break;
        case DRVPWM_CAP7:
            if (s_i32Option)
                outp32(&PWMB->CCR2, inp32(&PWMB->CCR2) & (CCR_MASK | BIT20));
            else
                outp32(&PWMB->CCR2, inp32(&PWMB->CCR2) & ~BIT4);
            break;
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_GetIntFlag                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Timer        - [in]      DRVPWM_TIMER0 / DRVPWM_TIMER1 / DRVPWM_TIMER2 / DRVPWM_TIMER3  */
/*                                          DRVPWM_TIMER4 / DRVPWM_TIMER5 / DRVPWM_TIMER6 / DRVPWM_TIMER7  */
/*                                          DRVPWM_CAP0   / DRVPWM_CAP1   / DRVPWM_CAP2   / DRVPWM_CAP3    */
/*                                          DRVPWM_CAP4   / DRVPWM_CAP5   / DRVPWM_CAP6   / DRVPWM_CAP7    */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               0      FALSE                                                                              */
/*               1      TRUE                                                                               */
/* Description:                                                                                            */
/*               This function is used to get the PWM0~7 timer/capture interrupt flag                      */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPWM_GetIntFlag(uint8_t u8Timer)
{
    int32_t status = 0;
    
    switch (u8Timer)
    {
        case DRVPWM_TIMER0:
        case DRVPWM_TIMER1:
        case DRVPWM_TIMER2:
        case DRVPWM_TIMER3:
            status = ( inp32(&PWMA->PIIR) & (1<<u8Timer) )? 1 : 0;
            break;
        case DRVPWM_TIMER4:
        case DRVPWM_TIMER5:
        case DRVPWM_TIMER6:
        case DRVPWM_TIMER7:
            status = ( inp32(&PWMB->PIIR) & (1<<(u8Timer-DRVPWM_TIMER4)) )? 1 : 0;
            break;

        case DRVPWM_CAP0:
            status = PWMA->CCR0.CAPIF0;
            break;
        case DRVPWM_CAP1:
            status = PWMA->CCR0.CAPIF1;             
            break;
        case DRVPWM_CAP2:
            status = PWMA->CCR2.CAPIF2;
            break;
        case DRVPWM_CAP3:
            status = PWMA->CCR2.CAPIF3;
            break;
        case DRVPWM_CAP4:
            status = PWMB->CCR0.CAPIF0;
            break;
        case DRVPWM_CAP5:
            status = PWMB->CCR0.CAPIF1;             
            break;
        case DRVPWM_CAP6:
            status = PWMB->CCR2.CAPIF2;
            break;
        case DRVPWM_CAP7:
            status = PWMB->CCR2.CAPIF3;
            break;
    }   

    return status;      
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_GetRisingCounter                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Capture      - [in]      DRVPWM_CAP0 / DRVPWM_CAP1 / DRVPWM_CAP2 / DRVPWM_CAP3          */
/*                                          DRVPWM_CAP4 / DRVPWM_CAP5 / DRVPWM_CAP6 / DRVPWM_CAP7          */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               The value which latches the counter when there・s a rising transition                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to get value which latches the counter when there・s a rising       */       
/*               transition                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
uint16_t DrvPWM_GetRisingCounter(uint8_t u8Capture)
{
    uint32_t u32Reg = 0;
    
    if (u8Capture >= DRVPWM_CAP4)
    {
        u32Reg = *((__IO uint32_t *) (PWMB_CRLR0 + ((u8Capture - DRVPWM_CAP4) << 3)));
    }
    else
    {
        u32Reg = *((__IO uint32_t *) (PWMA_CRLR0 + ((u8Capture & 0x7) << 3)));  
    }

    return u32Reg;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_GetFallingCounter                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Capture      - [in]      DRVPWM_CAP0 / DRVPWM_CAP1 / DRVPWM_CAP2 / DRVPWM_CAP3          */
/*                                          DRVPWM_CAP4 / DRVPWM_CAP5 / DRVPWM_CAP6 / DRVPWM_CAP7          */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               The value which latches the counter when there・s a falling transition                    */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to get value which latches the counter when there・s a falling      */       
/*               transition                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
uint16_t DrvPWM_GetFallingCounter(uint8_t u8Capture)
{
    uint32_t u32Reg = 0;
    
    if (u8Capture >= DRVPWM_CAP4)
    {
        u32Reg = *((__IO uint32_t *) (PWMB_CFLR0 + ((u8Capture - DRVPWM_CAP4) << 3)));
    }
    else
    {
        u32Reg = *((__IO uint32_t *) (PWMA_CFLR0 + ((u8Capture & 0x7) << 3)));  
    }

    return u32Reg;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_GetCaptureIntStatus                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Capture      - [in]      DRVPWM_CAP0 / DRVPWM_CAP1 / DRVPWM_CAP2 / DRVPWM_CAP3          */
/*                                          DRVPWM_CAP4 / DRVPWM_CAP5 / DRVPWM_CAP6 / DRVPWM_CAP7          */
/*               u8IntType      - [in]      DRVPWM_CAP_RISING_FLAG / DRVPWM_CAP_FALLING_FLAG               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               Check if there・s a rising / falling transition                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to get the rising / falling transition indicator flag               */       
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPWM_GetCaptureIntStatus(uint8_t u8Capture, uint8_t u8IntType)
{   
    int32_t status;

    status = 0; 

    switch (u8Capture)
    {
        case DRVPWM_CAP0:
            status = (u8IntType == DRVPWM_CAP_RISING_FLAG)? PWMA->CCR0.CRLRI0 : PWMA->CCR0.CFLRI0;
            break;
        case DRVPWM_CAP1:
            status = (u8IntType == DRVPWM_CAP_RISING_FLAG)? PWMA->CCR0.CRLRI1 : PWMA->CCR0.CFLRI1;
            break;
        case DRVPWM_CAP2:
            status = (u8IntType == DRVPWM_CAP_RISING_FLAG)? PWMA->CCR2.CRLRI2 : PWMA->CCR2.CFLRI2;
            break;
        case DRVPWM_CAP3:
            status = (u8IntType == DRVPWM_CAP_RISING_FLAG)? PWMA->CCR2.CRLRI3 : PWMA->CCR2.CFLRI3;
            break;
        case DRVPWM_CAP4:
            status = (u8IntType == DRVPWM_CAP_RISING_FLAG)? PWMB->CCR0.CRLRI0 : PWMB->CCR0.CFLRI0;
            break;
        case DRVPWM_CAP5:
            status = (u8IntType == DRVPWM_CAP_RISING_FLAG)? PWMB->CCR0.CRLRI1 : PWMB->CCR0.CFLRI1;
            break;
        case DRVPWM_CAP6:
            status = (u8IntType == DRVPWM_CAP_RISING_FLAG)? PWMB->CCR2.CRLRI2 : PWMB->CCR2.CFLRI2;
            break;
        case DRVPWM_CAP7:
            status = (u8IntType == DRVPWM_CAP_RISING_FLAG)? PWMB->CCR2.CRLRI3 : PWMB->CCR2.CFLRI3;
            break;
    }
    
    return status;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_ClearCaptureIntStatus                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Capture      - [in]      DRVPWM_CAP0 / DRVPWM_CAP1 / DRVPWM_CAP2 / DRVPWM_CAP3          */
/*                                          DRVPWM_CAP4 / DRVPWM_CAP5 / DRVPWM_CAP6 / DRVPWM_CAP7          */
/*               u8IntType      - [in]      DRVPWM_CAP_RISING_FLAG/DRVPWM_CAP_FALLING_FLAG                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               Clear the rising / falling transition interrupt flag                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to clear the rising / falling transition indicator flag             */       
/*---------------------------------------------------------------------------------------------------------*/
void DrvPWM_ClearCaptureIntStatus(uint8_t u8Capture, uint8_t u8IntType)
{
    if (s_i32Option)
    { 
        switch (u8Capture)
        {
            case DRVPWM_CAP0:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMA->CCR0, inp32(&PWMA->CCR0) & (CCR_MASK | BIT6))) : (outp32(&PWMA->CCR0, inp32(&PWMA->CCR0) & (CCR_MASK | BIT7)));
                break;
            case DRVPWM_CAP1:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMA->CCR0, inp32(&PWMA->CCR0) & (CCR_MASK | BIT22))) : (outp32(&PWMA->CCR0, inp32(&PWMA->CCR0) & (CCR_MASK | BIT23)));
                break;
            case DRVPWM_CAP2:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMA->CCR2, inp32(&PWMA->CCR2) & (CCR_MASK | BIT6))) : (outp32(&PWMA->CCR2, inp32(&PWMA->CCR2) & (CCR_MASK | BIT7)));
                break;
            case DRVPWM_CAP3:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMA->CCR2, inp32(&PWMA->CCR2) & (CCR_MASK | BIT22))) : (outp32(&PWMA->CCR2, inp32(&PWMA->CCR2) & (CCR_MASK | BIT23)));
                break;

            case DRVPWM_CAP4:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMB->CCR0, inp32(&PWMB->CCR0) & (CCR_MASK | BIT6))) : (outp32(&PWMB->CCR0, inp32(&PWMB->CCR0) & (CCR_MASK | BIT7)));
                break;
            case DRVPWM_CAP5:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMB->CCR0, inp32(&PWMB->CCR0) & (CCR_MASK | BIT22))) : (outp32(&PWMB->CCR0, inp32(&PWMB->CCR0) & (CCR_MASK | BIT23)));
                break;
            case DRVPWM_CAP6:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMB->CCR2, inp32(&PWMB->CCR2) & (CCR_MASK | BIT6))) : (outp32(&PWMB->CCR2, inp32(&PWMB->CCR2) & (CCR_MASK | BIT7)));
                break;
            case DRVPWM_CAP7:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMB->CCR2, inp32(&PWMB->CCR2) & (CCR_MASK | BIT22))) : (outp32(&PWMB->CCR2, inp32(&PWMB->CCR2) & (CCR_MASK | BIT23)));
                break;
        }
    }
    else
    {
        switch (u8Capture)
        {
            case DRVPWM_CAP0:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & ~(BIT4 | BIT20)) & ~BIT6)) : (outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & ~(BIT4 | BIT20)) & ~BIT7));
                break;
            case DRVPWM_CAP1:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & ~(BIT4 | BIT20)) & ~BIT22)) : (outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & ~(BIT4 | BIT20)) & ~BIT23));
                break;
            case DRVPWM_CAP2:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & ~(BIT4 | BIT20)) & ~BIT6)) : (outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & ~(BIT4 | BIT20)) & ~BIT7));
                break;
            case DRVPWM_CAP3:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & ~(BIT4 | BIT20)) & ~BIT22)) : (outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & ~(BIT4 | BIT20)) & ~BIT23));
                break;

            case DRVPWM_CAP4:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & ~(BIT4 | BIT20)) & ~BIT6)) : (outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & ~(BIT4 | BIT20)) & ~BIT7));
                break;
            case DRVPWM_CAP5:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & ~(BIT4 | BIT20)) & ~BIT22)) : (outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & ~(BIT4 | BIT20)) & ~BIT23));
                break;
            case DRVPWM_CAP6:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & ~(BIT4 | BIT20)) & ~BIT6)) : (outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & ~(BIT4 | BIT20)) & ~BIT7));
                break;
            case DRVPWM_CAP7:
                (u8IntType == DRVPWM_CAP_RISING_FLAG)? (outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & ~(BIT4 | BIT20)) & ~BIT22)) : (outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & ~(BIT4 | BIT20)) & ~BIT23));
                break;
        }   
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_Open                                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable PWM engine clock and reset PWM                                                     */       
/*---------------------------------------------------------------------------------------------------------*/
void DrvPWM_Open(void)
{ 
    outp32(&SYSCLK->APBCLK, inp32(&SYSCLK->APBCLK) | 0x00F00000);
    outp32(&SYS->IPRSTC2, inp32(&SYS->IPRSTC2) | 0x00300000);
    outp32(&SYS->IPRSTC2, inp32(&SYS->IPRSTC2) & ~0x00300000);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_Close                                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable PWM engine clock and the Capture Input/PWM Output Enable function                 */       
/*---------------------------------------------------------------------------------------------------------*/
void DrvPWM_Close(void)
{
    outp32(&PWMA->POE, 0);
    PWMA->CAPENR = 0;

    outp32(&PWMB->POE, 0);
    PWMB->CAPENR = 0;

    NVIC_DisableIRQ(PWMA_IRQn);
    NVIC_DisableIRQ(PWMB_IRQn);
    
    outp32(&SYS->IPRSTC2, inp32(&SYS->IPRSTC2) | 0x00300000);
    outp32(&SYS->IPRSTC2, inp32(&SYS->IPRSTC2) & ~0x00300000);

    outp32(&SYSCLK->APBCLK, inp32(&SYSCLK->APBCLK) & ~0x00F00000);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_EnableDeadZone                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Timer           - [in]   DRVPWM_TIMER0 or DRVPWM_TIMER1 / DRVPWM_TIMER2 or DRVPWM_TIMER3*/
/*                                          DRVPWM_TIMER4 or DRVPWM_TIMER5 / DRVPWM_TIMER6 or DRVPWM_TIMER7*/
/*               u8Length          - [in]   Dead Zone Length : 0~255                                       */
/*               i32EnableDeadZone - [in]   Enable DeadZone (1) / Diasble DeadZone (0)                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to set the dead zone length and enable/disable Dead Zone function   */       
/*---------------------------------------------------------------------------------------------------------*/
void DrvPWM_EnableDeadZone(uint8_t u8Timer, uint8_t u8Length, int32_t i32EnableDeadZone)
{
    switch (u8Timer & 0x07)
    {
        case DRVPWM_TIMER0:
        case DRVPWM_TIMER1:
            PWMA->PPR.DZI01  = u8Length;
            PWMA->PCR.DZEN01 = i32EnableDeadZone;
            break;
        case DRVPWM_TIMER2:
        case DRVPWM_TIMER3:
            PWMA->PPR.DZI23  = u8Length;
            PWMA->PCR.DZEN23 = i32EnableDeadZone;
            break;
        case DRVPWM_TIMER4:
        case DRVPWM_TIMER5:
            PWMB->PPR.DZI01  = u8Length;
            PWMB->PCR.DZEN01 = i32EnableDeadZone;
            break;
        case DRVPWM_TIMER6:
        case DRVPWM_TIMER7:
            PWMB->PPR.DZI23  = u8Length;
            PWMB->PCR.DZEN23 = i32EnableDeadZone;
            break;
    }   
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_Enable                                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Timer        - [in]      DRVPWM_TIMER0 / DRVPWM_TIMER1 / DRVPWM_TIMER2 / DRVPWM_TIMER3  */
/*                                          DRVPWM_TIMER4 / DRVPWM_TIMER5 / DRVPWM_TIMER6 / DRVPWM_TIMER7  */
/*                                          DRVPWM_CAP0   / DRVPWM_CAP1   / DRVPWM_CAP2   / DRVPWM_CAP3    */
/*                                          DRVPWM_CAP4   / DRVPWM_CAP5   / DRVPWM_CAP6   / DRVPWM_CAP7    */
/*               i32Enable      - [in]      Enable  (1) / Disable  (0)                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to enable/disable PWM0~7 timer/capture function                     */       
/*---------------------------------------------------------------------------------------------------------*/
void DrvPWM_Enable(uint8_t u8Timer, int32_t i32Enable)
{
    switch (u8Timer & 0x07)
    {
        case DRVPWM_TIMER0:
        case DRVPWM_TIMER1:
        case DRVPWM_TIMER2:
        case DRVPWM_TIMER3:
            outp32(&PWMA->PCR, (inp32(&PWMA->PCR) & ~(1 << ((u8Timer & 0x07) << 3) )) | (i32Enable?(1 << ((u8Timer & 0x07) << 3) ):0) );
            break;
        case DRVPWM_TIMER4:
        case DRVPWM_TIMER5:
        case DRVPWM_TIMER6:
        case DRVPWM_TIMER7:
            outp32(&PWMB->PCR, (inp32(&PWMB->PCR) & ~(1 << (((u8Timer & 0x07)-DRVPWM_TIMER4)<< 3) )) | (i32Enable?(1 << (((u8Timer & 0x07)-DRVPWM_TIMER4) << 3) ):0) );
            break;
    }   

    if (u8Timer & 0x10)
    {
        switch (u8Timer)
        {
            case DRVPWM_CAP0:
                if (s_i32Option)
                    outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & CCR_MASK) | BIT3);
                else
                    outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & ~(BIT4 | BIT20)) | BIT3); 
                break;
            case DRVPWM_CAP1:
                if (s_i32Option)
                    outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & CCR_MASK) | BIT19);
                else
                    outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & ~(BIT4 | BIT20)) | BIT19);    
                break;
            case DRVPWM_CAP2:
                if (s_i32Option)
                    outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & CCR_MASK) | BIT3);
                else
                    outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & ~(BIT4 | BIT20)) | BIT3);     
                break;                                                                    
            case DRVPWM_CAP3:
                if (s_i32Option)
                    outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & CCR_MASK) | BIT19);
                else
                    outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & ~(BIT4 | BIT20)) | BIT19);
                break;
            case DRVPWM_CAP4:
                if (s_i32Option)
                    outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & CCR_MASK) | BIT3);
                else
                    outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & ~(BIT4 | BIT20)) | BIT3); 
                break;
            case DRVPWM_CAP5:
                if (s_i32Option)
                    outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & CCR_MASK) | BIT19);
                else
                    outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & ~(BIT4 | BIT20)) | BIT19);    
                break;
            case DRVPWM_CAP6:
                if (s_i32Option)
                    outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & CCR_MASK) | BIT3);
                else
                    outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & ~(BIT4 | BIT20)) | BIT3);     
                break;                                                                    
            case DRVPWM_CAP7:
                if (s_i32Option)
                    outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & CCR_MASK) | BIT19);
                else
                    outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & ~(BIT4 | BIT20)) | BIT19);
                break;  
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_SetTimerClk                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Timer                        DRVPWM_TIMER0/DRVPWM_TIMER1/DRVPWM_TIMER2/DRVPWM_TIMER3    */
/*                                              DRVPWM_TIMER4/DRVPWM_TIMER5/DRVPWM_TIMER6/DRVPWM_TIMER7    */
/*                                              DRVPWM_CAP0/DRVPWM_CAP1/DRVPWM_CAP2/DRVPWM_CAP3            */
/*                                              DRVPWM_CAP4/DRVPWM_CAP5/DRVPWM_CAP6/DRVPWM_CAP7            */
/*               S_DRVPWM_TIME_DATA_T *sPt                                                                 */
/*               u32Frequency                   Frequency (Hz)                                             */
/*               u8HighPulseRatio               High Pulse Ratio                                           */
/*               u8Mode                         DRVPWM_ONE_SHOT_MODE / DRVPWM_AUTO_RELOAD_MODE             */
/*               bInverter                      Inverter Enable  (TRUE) / Inverter Disable  (FALSE)        */
/*               u8ClockSelector                Clock Selector                                             */
/*                                              DRVPWM_CLOCK_DIV_1/DRVPWM_CLOCK_DIV_2/DRVPWM_CLOCK_DIV_4   */
/*                                              DRVPWM_CLOCK_DIV_8/DRVPWM_CLOCK_DIV_16                     */
/*                                              (The parameter takes effect when u8Frequency = 0)          */
/*               u8PreScale                     Prescale (1 ~ 255).                                        */ 
/*                                              The PWM input clock = PWM source clock / (u8PreScale + 1)  */ 
/*                                              Note: If the value is set to 0, the timer will stop.       */
/*                                              (The parameter takes effect when u8Frequency = 0)          */
/*               u32Duty                        Pulse duty                                                 */                             
/*                                              (The parameter takes effect when u8Frequency = 0 or        */
/*                                              u8Timer = DRVPWM_CAP0/DRVPWM_CAP1/DRVPWM_CAP2/DRVPWM_CAP3  */
/*                                                        DRVPWM_CAP4/DRVPWM_CAP5/DRVPWM_CAP6/DRVPWM_CAP7) */       
/*                                                                                                         */
/* Returns:                                                                                                */
/*               The actual frequency (Hz)                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to configure the frequency/pulse/mode/inverter function             */       
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPWM_SetTimerClk(uint8_t u8Timer, S_DRVPWM_TIME_DATA_T *sPt)
{
    uint32_t    u32Frequency;
    uint32_t    u32Freqout;
    uint16_t    u16Duty, u16cnrData, u16cmrData;
    uint8_t     u8EngineClk = 0;
         
    switch (u8Timer & 0x07)
    {
        case DRVPWM_TIMER0:
        case DRVPWM_TIMER1:
            u8EngineClk = SYSCLK->CLKSEL1.PWM01_S;              
            break;
        case DRVPWM_TIMER2:
        case DRVPWM_TIMER3:
            u8EngineClk = SYSCLK->CLKSEL1.PWM23_S;
            break;      
        case DRVPWM_TIMER4:
        case DRVPWM_TIMER5:
            u8EngineClk = SYSCLK->CLKSEL2.PWM45_S;              
            break;
        case DRVPWM_TIMER6:
        case DRVPWM_TIMER7:
            u8EngineClk = SYSCLK->CLKSEL2.PWM67_S;
            break;  
    }


    if (u8EngineClk == 0)        /* external 12MHz crystal clock    */
    {
        u32Freqout = __XTAL;    
    }
    else if(u8EngineClk == 1)    /* external 32KHz crystal clock */
    {
        u32Freqout = __RTC_XTAL;
    }
    else if(u8EngineClk == 2)    /* HCLK clock */
    {
        u32Freqout = SystemCoreClock;
    }
    else                         /* internal 22MHz oscillator clock */
    {
        u32Freqout = __IRC22M;  
    }

    switch (u8Timer)
    {
        case DRVPWM_TIMER0:
        case DRVPWM_TIMER1:
        case DRVPWM_TIMER2:
        case DRVPWM_TIMER3:
            outp32(&PWMA->PCR, (inp32(&PWMA->PCR) & ~(1 << (((u8Timer ) << 3) + 2) )) |(sPt->i32Inverter?(1 << ((u8Timer << 3) + 2)):0));
            break;
        case DRVPWM_TIMER4:
        case DRVPWM_TIMER5:
        case DRVPWM_TIMER6:
        case DRVPWM_TIMER7:
            outp32(&PWMB->PCR, (inp32(&PWMB->PCR) & ~(1 << (((u8Timer-DRVPWM_TIMER4) << 3) + 2) )) |(sPt->i32Inverter?(1 << (((u8Timer-DRVPWM_TIMER4) << 3) + 2)):0));
            break;

        case DRVPWM_CAP0:
            if (s_i32Option)
                outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & CCR_MASK) | (sPt->i32Inverter? BIT0 : 0) );
            else
                outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & ~(BIT4 | BIT20)) | (sPt->i32Inverter? BIT0 : 0)); 
            break;
        case DRVPWM_CAP1:
            if (s_i32Option)
                outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & CCR_MASK) | (sPt->i32Inverter? BIT16 : 0));
            else
                outp32(&PWMA->CCR0, (inp32(&PWMA->CCR0) & ~(BIT4 | BIT20)) | (sPt->i32Inverter? BIT16 : 0));    
            break;
        case DRVPWM_CAP2:
            if (s_i32Option)
                outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & CCR_MASK) | (sPt->i32Inverter? BIT0 : 0));
            else
                outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & ~(BIT4 | BIT20)) | (sPt->i32Inverter? BIT0 : 0)); 
            break;
        case DRVPWM_CAP3:
            if (s_i32Option)
                outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & CCR_MASK) | (sPt->i32Inverter? BIT16 : 0));
            else
                outp32(&PWMA->CCR2, (inp32(&PWMA->CCR2) & ~(BIT4 | BIT20)) | (sPt->i32Inverter? BIT16 : 0));
            break;
        case DRVPWM_CAP4:
            if (s_i32Option)
                outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & CCR_MASK) | (sPt->i32Inverter? BIT0 : 0) );
            else
                outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & ~(BIT4 | BIT20)) | (sPt->i32Inverter? BIT0 : 0)); 
            break;
        case DRVPWM_CAP5:
            if (s_i32Option)
                outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & CCR_MASK) | (sPt->i32Inverter? BIT16 : 0));
            else
                outp32(&PWMB->CCR0, (inp32(&PWMB->CCR0) & ~(BIT4 | BIT20)) | (sPt->i32Inverter? BIT16 : 0));
            break;
        case DRVPWM_CAP6:
            if (s_i32Option)
                outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & CCR_MASK) | (sPt->i32Inverter? BIT0 : 0));
            else
                outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & ~(BIT4 | BIT20)) | (sPt->i32Inverter? BIT0 : 0));
            break;
        case DRVPWM_CAP7:
            if (s_i32Option)
                outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & CCR_MASK) | (sPt->i32Inverter? BIT16 : 0));
            else
                outp32(&PWMB->CCR2, (inp32(&PWMB->CCR2) & ~(BIT4 | BIT20)) | (sPt->i32Inverter? BIT16 : 0));
            break;
    } 

    switch (u8Timer & 0x07)
    {
        case DRVPWM_TIMER0:
        case DRVPWM_TIMER1:
        case DRVPWM_TIMER2:
        case DRVPWM_TIMER3:
            outp32(&PWMA->PCR, (inp32(&PWMA->PCR) & ~(1 <<(((u8Timer & 0x07) << 3) + 3))) | ( sPt->u8Mode? (1 <<(((u8Timer & 0x07) << 3) + 3)):0));
            break;
        case DRVPWM_TIMER4:
        case DRVPWM_TIMER5:
        case DRVPWM_TIMER6:
        case DRVPWM_TIMER7:
            outp32(&PWMB->PCR, (inp32(&PWMB->PCR) & ~(1 <<((((u8Timer & 0x07)-DRVPWM_TIMER4) << 3) + 3))) | ( sPt->u8Mode? (1 <<((((u8Timer & 0x07)-DRVPWM_TIMER4) << 3) + 3)):0));         
            break;
    } 
            
    
    if (sPt->u32Frequency == 0)
    {   
        uint8_t u8Divider = 1;
        uint32_t u32duty;
        
        switch (u8Timer & 0x07)
        {
            case DRVPWM_TIMER0:
            case DRVPWM_TIMER1:
                PWMA->PPR.CP01 = sPt->u8PreScale;
                break;
            case DRVPWM_TIMER2:
            case DRVPWM_TIMER3:
                PWMA->PPR.CP23 = sPt->u8PreScale;
                break;
            case DRVPWM_TIMER4:
            case DRVPWM_TIMER5:
                PWMB->PPR.CP01 = sPt->u8PreScale;
                break;
            case DRVPWM_TIMER6:
            case DRVPWM_TIMER7:
                PWMB->PPR.CP23 = sPt->u8PreScale;
                break;
        } 
        
        u32duty = sPt->u32Duty * sPt->u8HighPulseRatio / 100 - 1;
        
        switch (u8Timer & 0x07)
        {
            case DRVPWM_TIMER0:
            case DRVPWM_TIMER1:
            case DRVPWM_TIMER2:
            case DRVPWM_TIMER3:
                outp32(&PWMA->CSR, (inp32(&PWMA->CSR) & ~(0x7 << ((u8Timer & 0x07)<<2) )) | ((sPt->u8ClockSelector & 0x7) << ((u8Timer & 0x07)<<2)));   
                outp32(PWMA_CNR0 + (u8Timer & 0x07) * 12, (sPt->u32Duty - 1));
                outp32(PWMA_CMR0 + (u8Timer & 0x07) * 12, u32duty);
                break;
            case DRVPWM_TIMER4:
            case DRVPWM_TIMER5:
            case DRVPWM_TIMER6:
            case DRVPWM_TIMER7:
                outp32(&PWMB->CSR, (inp32(&PWMB->CSR) & ~(0x7 << (((u8Timer & 0x07)-DRVPWM_TIMER4)<<2) )) | ((sPt->u8ClockSelector & 0x7) << (((u8Timer & 0x07)-DRVPWM_TIMER4)<<2)));   
                outp32(PWMB_CNR0 + ((u8Timer & 0x07)-DRVPWM_TIMER4) * 12, (sPt->u32Duty - 1));
                outp32(PWMB_CMR0 + ((u8Timer & 0x07)-DRVPWM_TIMER4) * 12, u32duty);
                break;
        }
                
        switch (sPt->u8ClockSelector)
        {
            case DRVPWM_CLOCK_DIV_1:
                u8Divider = 1;
                break;
            case DRVPWM_CLOCK_DIV_2:
                u8Divider = 2;          
                break;          
            case DRVPWM_CLOCK_DIV_4:
                u8Divider = 4;          
                break;          
            case DRVPWM_CLOCK_DIV_8:
                u8Divider = 8;          
                break;          
            case DRVPWM_CLOCK_DIV_16: 
                u8Divider = 16;         
                break;      
        }       
        
        u32Frequency = u32Freqout / (sPt->u8PreScale + 1) / u8Divider / sPt->u32Duty;
    }
    else
    {
        uint8_t  u8Divider;
        uint16_t u16PreScale;
        
        u32Frequency =  u32Freqout / sPt->u32Frequency;     
        
        if (u32Frequency > 0x10000000)
            return 0;
            
        u8Divider = 1;          
            
        if (u32Frequency < 0x20000)
            u16PreScale = 2;    
        else
        {
            u16PreScale = u32Frequency / 65536; 
                            
            if (u32Frequency / u16PreScale > 65536)
                u16PreScale++;
            
            if (u16PreScale > 256)
            {
                uint8_t u8i = 0;
                
                u16PreScale = 256;
                u32Frequency = u32Frequency / u16PreScale;
                
                u8Divider = u32Frequency / 65536;
                
                if(u32Frequency / u8Divider > 65536)
                    u8Divider++;
                
                while(1)    
                {
                    if((1 << u8i++) > u8Divider)
                        break;
                }
                
                u8Divider = 1 << (u8i - 1);
                
                if (u8Divider > 16)
                    return 0;   
                    
                u32Frequency = u32Frequency * u16PreScale;
            }       
                    
        }
        u16Duty = (uint16_t )(u32Frequency/u16PreScale/u8Divider);
        
        u32Frequency = (u32Freqout / u16PreScale / u8Divider) / u16Duty;    
                
        switch (u8Timer & 0x07)
        {
            case DRVPWM_TIMER0:
            case DRVPWM_TIMER1:
                PWMA->PPR.CP01 = u16PreScale - 1;
                break;
            case DRVPWM_TIMER2:
            case DRVPWM_TIMER3:
                PWMA->PPR.CP23 = u16PreScale - 1;
                break;
            case DRVPWM_TIMER4:
            case DRVPWM_TIMER5:
                PWMB->PPR.CP01 = u16PreScale - 1;
                break;
            case DRVPWM_TIMER6:
            case DRVPWM_TIMER7:
                PWMB->PPR.CP23 = u16PreScale - 1;
                break;
        }
        
        switch(u8Divider)
        {
            case 1:
                u8Divider = DRVPWM_CLOCK_DIV_1;
                break;
            case 2:
                u8Divider = DRVPWM_CLOCK_DIV_2;  
                break;          
            case 4:
                u8Divider = DRVPWM_CLOCK_DIV_4;  
                break;          
            case 8:
                u8Divider = DRVPWM_CLOCK_DIV_8;  
                break;          
            case 16:
                u8Divider = DRVPWM_CLOCK_DIV_16; 
                break;      
        }               
                    
        if (u8Timer & 0x10)
        {
            u16cnrData = sPt->u32Duty - 1; 
            u16cmrData = sPt->u32Duty * sPt->u8HighPulseRatio / 100 - 1;        
        }
        else
        {
            u16cnrData = u16Duty - 1;
            u16cmrData = u16Duty * sPt->u8HighPulseRatio / 100 - 1;
        }
        
        switch (u8Timer & 0x07)
        {
            case DRVPWM_TIMER0:
            case DRVPWM_TIMER1:
            case DRVPWM_TIMER2:
            case DRVPWM_TIMER3:
                outp32(&PWMA->CSR, (inp32(&PWMA->CSR) & ~(0x7 << ((u8Timer & 0x07)<<2) )) | ((u8Divider & 0x7) << ((u8Timer & 0x07)<<2)));  
                outp32(PWMA_CNR0 + (u8Timer & 0x07) * 12, u16cnrData);          
                outp32(PWMA_CMR0 + (u8Timer & 0x07) * 12, u16cmrData);    
                break;
            case DRVPWM_TIMER4:
            case DRVPWM_TIMER5:
            case DRVPWM_TIMER6:
            case DRVPWM_TIMER7:
                outp32(&PWMB->CSR, (inp32(&PWMB->CSR) & ~(0x7 << (((u8Timer & 0x07)-DRVPWM_TIMER4)<<2) )) | ((u8Divider & 0x7) << (((u8Timer & 0x07)-DRVPWM_TIMER4)<<2)));  
                outp32(PWMB_CNR0 + ((u8Timer & 0x07)-DRVPWM_TIMER4) * 12, u16cnrData);          
                outp32(PWMB_CMR0 + ((u8Timer & 0x07)-DRVPWM_TIMER4) * 12, u16cmrData);  
                break;
        }
    }
    
    return u32Frequency;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_SetTimerIO                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8Timer        - [in]      DRVPWM_TIMER0 / DRVPWM_TIMER1 / DRVPWM_TIMER2 / DRVPWM_TIMER3  */
/*                                          DRVPWM_TIMER4 / DRVPWM_TIMER5 / DRVPWM_TIMER6 / DRVPWM_TIMER7  */
/*                                          DRVPWM_CAP0   / DRVPWM_CAP1   / DRVPWM_CAP2   / DRVPWM_CAP3    */
/*                                          DRVPWM_CAP4   / DRVPWM_CAP5   / DRVPWM_CAP6   / DRVPWM_CAP7    */
/*               i32Enable      - [in]      Enable  (1) / Diasble  (0)                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to enable/disable PWM0~7 timer/capture I/O function                 */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPWM_SetTimerIO(uint8_t u8Timer, int32_t i32Enable)
{
    if (i32Enable)
    {
        if (u8Timer & 0x10)
        {
            if (u8Timer & 0x04)
                PWMB->CAPENR = PWMB->CAPENR | (1 << (u8Timer - DRVPWM_CAP4));
            else
                PWMA->CAPENR = PWMA->CAPENR | (1 << (u8Timer & 0x07));          
        }
        else
        {
            if (u8Timer & 0x04)
                outp32(&PWMB->POE, inp32(&PWMB->POE)  | (1 << (u8Timer-DRVPWM_TIMER4)));
            else
                outp32(&PWMA->POE, inp32(&PWMA->POE)  | (1 << u8Timer));    
        }
    }
    else
    {
        if (u8Timer & 0x10)         
        {
            if (u8Timer & 0x04)
                PWMB->CAPENR = PWMB->CAPENR & ~(1 << (u8Timer - DRVPWM_CAP4));
            else
                PWMA->CAPENR = PWMA->CAPENR & ~(1 << (u8Timer & 0x07));         
        }
        else
        {
            if (u8Timer & 0x04)
                outp32(&PWMB->POE, inp32(&PWMB->POE)  & ~(1 << (u8Timer-DRVPWM_TIMER4)));
            else
                outp32(&PWMA->POE, inp32(&PWMA->POE)  & ~(1 << u8Timer));           
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_SelectClockSource                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*           u8Timer                - [in] DRVPWM_TIMER0 / DRVPWM_TIMER1 / DRVPWM_TIMER2 / DRVPWM_TIMER3   */
/*                                         DRVPWM_TIMER4 / DRVPWM_TIMER5 / DRVPWM_TIMER6 / DRVPWM_TIMER7   */
/*           u8ClockSourceSelector  - [in] DRVPWM_EXT_12M/DRVPWM_EXT_32K/DRVPWM_HCLK/DRVPWM_INTERNAL_22M   */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to select PWM clock source                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPWM_SelectClockSource(uint8_t u8Timer, uint8_t u8ClockSourceSelector)
{
    switch (u8Timer & 0x07)
    {
        case DRVPWM_TIMER0:
        case DRVPWM_TIMER1:
            SYSCLK->CLKSEL1.PWM01_S = u8ClockSourceSelector;                
            break;
        case DRVPWM_TIMER2:
        case DRVPWM_TIMER3:
            SYSCLK->CLKSEL1.PWM23_S = u8ClockSourceSelector;
            break;      
        case DRVPWM_TIMER4:
        case DRVPWM_TIMER5:
            SYSCLK->CLKSEL2.PWM45_S = u8ClockSourceSelector;
            break;
        case DRVPWM_TIMER6:
        case DRVPWM_TIMER7:
            SYSCLK->CLKSEL2.PWM67_S = u8ClockSourceSelector;
            break;  
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvPWM_SelectClearLatchFlagOption                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*           i32option  - [in]   0 : Select option to clear the Capture Latch Indicators by writing a '0'  */
/*                               1 : Select option to clear the Capture Latch Indicators by writing a '1'  */
/*                                                                                                         */
/* Returns:                                                                                                */
/*           0:    Success                                                                                 */
/*         < 0:    This option does NOT be supported for this chip                                         */
/*                                                                                                         */
/* Description:                                                                                            */
/*           This function is used to select how to clear Capture rising & falling Latch Indicator         */
/*                                                                                                         */
/* Note:                                                                                                   */
/*           Only NuMicro NUC1x0xxxCx Series (Ex. NUC140VE3CN) and Low Density Series support              */
/*           this function. Please refer to TRM in detail                                                  */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPWM_SelectClearLatchFlagOption(int32_t i32option)
{
    PWMA->PBCR.BCn = 0;
    if (PWMA->PBCR.BCn)
    {
        return E_DRVPWM_ERR_NO_SUPPORT;
    }
    else
    {
        PWMA->PBCR.BCn = i32option;
        PWMB->PBCR.BCn = i32option;
        s_i32Option = i32option;        
        return E_SUCCESS;
    }
}

