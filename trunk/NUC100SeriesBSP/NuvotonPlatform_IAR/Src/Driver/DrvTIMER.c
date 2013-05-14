/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------------------------------------*/
/* Includes of system headers                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include "DrvTIMER.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define TIMER_EVENT_COUNT   1
const uint32_t CH_OFFSET[] = {0x0, 0x20, 0x100000, 0x100020};


/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static TIMER_EVENT_T tTime0Event[TIMER_EVENT_COUNT],
                     tTime1Event[TIMER_EVENT_COUNT],
                     tTime2Event[TIMER_EVENT_COUNT],
                     tTime3Event[TIMER_EVENT_COUNT];

int32_t volatile bIsTimer0Initial     = FALSE,
                 bIsTimer1Initial     = FALSE,
                 bIsTimer2Initial     = FALSE,
                 bIsTimer3Initial     = FALSE,
                 bIsTimer0Used        = FALSE,
                 bIsTimer1Used        = FALSE,
                 bIsTimer2Used        = FALSE,
                 bIsTimer3Used        = FALSE,
                 bIsSetTime0Event     = FALSE,
                 bIsSetTime1Event     = FALSE,
                 bIsSetTime2Event     = FALSE,
                 bIsSetTime3Event     = FALSE;

static uint32_t volatile uTimer0Tick = 0,
                         uTimer1Tick = 0,
                         uTimer2Tick = 0,
                         uTimer3Tick = 0,
                         uTime0EventCount = 0,
                         uTime1EventCount = 0,
                         uTime2EventCount = 0,
                         uTime3EventCount = 0;

static uint32_t volatile _sys_uTimer0TickPerSecond,
                         _sys_uTimer1TickPerSecond,
                         _sys_uTimer2TickPerSecond, 
                         _sys_uTimer3TickPerSecond;

uint32_t volatile u32EXTClockFreq = 12000000;

static WDT_CALLBACK fnCallBack_WDT;

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GetTimerClock                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*                      E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3                           */   
/* Returns:                                                                                                */   
/*                  u32clk                                  The timer clock (Hz)                           */
/*                  E_DRVTIMER_CHANNEL                      Invalid Timer channel                          */
/*                  E_DRVTIMER_CLOCK_RATE                   Invalid Timer clock source                     */
/* Description:                                                                                            */
/*                  Get the timer clock from the specified timer channel.                                  */
/*---------------------------------------------------------------------------------------------------------*/
static uint32_t GetTimerClock(E_TIMER_CHANNEL ch)
{
    volatile uint8_t u8ClockSrc;
    volatile uint32_t u32clk = 0;
        
    if ((ch == E_TMR0) || (ch == E_TMR1) || (ch == E_TMR2) || (ch == E_TMR3))
    {
        u8ClockSrc = (inpw(&SYSCLK->CLKSEL1) >> (8+(ch*4))) & 0x7;

        if (u8ClockSrc == 0)           
        {
            u32clk = DrvSYS_GetExtClockFreq() ;     /* Option 0: Get External Clock From DrvSYS Setting */
        }
        else if(u8ClockSrc == 1)
        {
            u32clk = __RTC_XTAL;                    /* Option 1: 32K */
        }
        else if(u8ClockSrc == 2)
        {
            u32clk = DrvSYS_GetHCLKFreq();          /* Option 2: HCLK */
        }
        else if(u8ClockSrc == 7)
        {
            u32clk = __IRC22M;                      /* Option 7: 22MHz*/
        }else
        {
            return E_DRVTIMER_CLOCK_RATE;
        }
    }
    else 
        return E_DRVTIMER_CHANNEL;
            
    return u32clk;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CalTimerInitValue                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  u32ClockValue - [in]                                                                   */
/*                      Input the clock value of Timer                                                     */
/*                  u32TicksPerSec - [in]                                                                  */
/*                      Specify the ticks per second of Timer                                              */
/* Returns:                                                                                                */
/*                  Return 32 bits unsigned integer where                                                  */
/*                          bits [ 0:23] - The Timer Compare Value(TCMPR) for Timer                        */
/*                          bits [24:31] - The pre-scale value for Timer                                   */
/*                  (uint32_t)-1                            Out of range                                   */
/* Description:                                                                                            */
/*                  Calculate the Timer Compare Value and pre-scale value for Timer                        */
/*---------------------------------------------------------------------------------------------------------*/
static uint32_t CalTimerInitValue(uint32_t u32ClockValue, uint32_t u32TicksPerSec)
{
    uint32_t u32PreScale;
    uint32_t u32TCMPRValue;

    if ((u32ClockValue < 2) || (u32TicksPerSec == 0))
        return (uint32_t)-1;

    for (u32PreScale=1; u32PreScale<256; u32PreScale++)
    {
        u32TCMPRValue = u32ClockValue / (u32TicksPerSec * u32PreScale);

        /* The TCMPR value must > 1 */
        if ((u32TCMPRValue > 1) && (u32TCMPRValue < 0x1000000))
            return (((u32PreScale-1) << 24) | u32TCMPRValue);
    }
    
    return (uint32_t)-1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        TMR0_IRQHandler                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  None                                                                                   */
/* Returns:                                                                                                */
/*                  None                                                                                   */
/* Description:                                                                                            */
/*                  The TIMER0 default IRQ, declared in startup_NUC1xx.s                                   */
/*---------------------------------------------------------------------------------------------------------*/
void TMR0_IRQHandler(void)
{   
    if ((TIMER0->TCSR.IE == 1) && (TIMER0->TISR.TIF == 1))
        TIMER0->TISR.TIF = 1;

    if ((TIMER0->TEXCON.TEXEN == 1) && (TIMER0->TEXCON.TEXIEN == 1) && (TIMER0->TEXISR.TEXIF == 1))
        TIMER0->TEXISR.TEXIF = 1;

    uTimer0Tick++;

    if (tTime0Event[0].active)
    {
        tTime0Event[0].curTick--;
        if (tTime0Event[0].curTick == 0)
        {
            (*tTime0Event[0].funPtr)(tTime0Event[0].transParam);
            tTime0Event[0].curTick = tTime0Event[0].initTick;
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        TMR1_IRQHandler                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  None                                                                                   */
/* Returns:                                                                                                */
/*                  None                                                                                   */
/* Description:                                                                                            */
/*                  The TIMER1 default IRQ, declared in startup_NUC1xx.s                                   */
/*---------------------------------------------------------------------------------------------------------*/
void TMR1_IRQHandler(void)
{    
    if ((TIMER1->TCSR.IE == 1) && (TIMER1->TISR.TIF == 1))
        TIMER1->TISR.TIF = 1;

    if ((TIMER1->TEXCON.TEXEN == 1) && (TIMER1->TEXCON.TEXIEN == 1) && (TIMER1->TEXISR.TEXIF == 1))
        TIMER1->TEXISR.TEXIF = 1;

    uTimer1Tick++;

    if (tTime1Event[0].active)
    {
        tTime1Event[0].curTick--;
        if (tTime1Event[0].curTick == 0)
        {
            (*tTime1Event[0].funPtr)(tTime1Event[0].transParam);
            tTime1Event[0].curTick = tTime1Event[0].initTick;
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        TMR2_IRQHandler                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  None                                                                                   */
/* Returns:                                                                                                */
/*                  None                                                                                   */
/* Description:                                                                                            */
/*                  The TIMER2 default IRQ, declared in startup_NUC1xx.s                                   */
/*---------------------------------------------------------------------------------------------------------*/
void TMR2_IRQHandler(void)
{    
    if ((TIMER2->TCSR.IE == 1) && (TIMER2->TISR.TIF == 1))
        TIMER2->TISR.TIF = 1;

    if ((TIMER2->TEXCON.TEXEN == 1) && (TIMER2->TEXCON.TEXIEN == 1) && (TIMER2->TEXISR.TEXIF == 1))
        TIMER2->TEXISR.TEXIF = 1;

    uTimer2Tick++;

    if (tTime2Event[0].active)
    {
        tTime2Event[0].curTick--;
        if (tTime2Event[0].curTick == 0)
        {
            (*tTime2Event[0].funPtr)(tTime2Event[0].transParam);
            tTime2Event[0].curTick = tTime2Event[0].initTick;
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        TMR3_IRQHandler                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  None                                                                                   */
/* Returns:                                                                                                */
/*                  None                                                                                   */
/* Description:                                                                                            */
/*                  The TIMER3 default IRQ, declared in startup_NUC1xx.s                                   */
/*---------------------------------------------------------------------------------------------------------*/
void TMR3_IRQHandler(void)
{
    if ((TIMER3->TCSR.IE == 1) && (TIMER3->TISR.TIF == 1))
        TIMER3->TISR.TIF = 1;

    if ((TIMER3->TEXCON.TEXEN == 1) && (TIMER3->TEXCON.TEXIEN == 1) && (TIMER3->TEXISR.TEXIF == 1))
        TIMER3->TEXISR.TEXIF = 1;

    uTimer3Tick++;

    if (tTime3Event[0].active)
    {
        tTime3Event[0].curTick--;
        if (tTime3Event[0].curTick == 0)
        {
            (*tTime3Event[0].funPtr)(tTime3Event[0].transParam);
            tTime3Event[0].curTick = tTime3Event[0].initTick;
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_Init                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  None                                                                                   */
/* Returns:                                                                                                */
/*                  None                                                                                   */
/* Description:                                                                                            */
/*                  User must to call this function before any timer operations after system boot up.      */
/*---------------------------------------------------------------------------------------------------------*/
void DrvTIMER_Init(void)
{
    bIsTimer0Initial    = TRUE;
    bIsTimer1Initial    = TRUE;
    bIsTimer2Initial    = TRUE;
    bIsTimer3Initial    = TRUE;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_Open                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*                      E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3                           */   
/*                  uTicksPerSecond - [in]                                                                 */
/*                      This value means how many timer interrupt ticks in one second                      */
/*                  op_mode - [in]                                                                         */
/*                      E_TIMER_OPMODE, E_ONESHOT_MODE/E_PERIODIC_MODE/E_TOGGLE_MODE/E_CONTINUOUS_MODE     */
/* Returns:                                                                                                */
/*                  E_SUCCESS                               Operation successful                           */
/*                  E_DRVTIMER_CHANNEL                      Invalid Timer channel                          */
/*                  E_DRVTIMER_CLOCK_RATE                   Calculate initial value fail                   */
/* Description:                                                                                            */
/*                  Open the specified timer channel with specified operation mode.                        */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_Open(E_TIMER_CHANNEL ch, uint32_t uTicksPerSecond, E_TIMER_OPMODE op_mode)
{
    uint32_t i;
    uint32_t uRegTcmpr, uRegTcr = 0x0;

    switch (ch)
    {
        case E_TMR0:
        {
            if ((bIsTimer0Initial != TRUE) || (bIsTimer0Used != FALSE))
                return E_DRVTIMER_EIO;
            
            bIsTimer0Used = TRUE;
           
            SYSCLK->APBCLK.TMR0_EN = 1;
                                                              
            outpw((uint32_t)&TIMER0->TCSR, 0);                  /* disable timer */
           
            TIMER0->TISR.TIF = 1;                               /* write 1 to clear for safty */
                               
            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime0Event[i].active = FALSE;
            }

            uTimer0Tick = 0;            
            _sys_uTimer0TickPerSecond = uTicksPerSecond;
                    
            uRegTcmpr = CalTimerInitValue(GetTimerClock(E_TMR0), uTicksPerSecond); 
            if (uRegTcmpr == (uint32_t)-1)      
            {
                return E_DRVTIMER_CLOCK_RATE;           
            }

            TIMER0->TCMPR = (uRegTcmpr << 8) >> 8;
            outpw((uint32_t)&TIMER0->TCSR,  (uRegTcr|(uRegTcmpr>>24))|(op_mode<<27));               
            break;
        }

        case E_TMR1:
        {
            if ((bIsTimer1Initial != TRUE) || (bIsTimer1Used != FALSE))
                return E_DRVTIMER_EIO;

            bIsTimer1Used = TRUE;

            SYSCLK->APBCLK.TMR1_EN = 1;
            
            outpw((uint32_t)&TIMER1->TCSR, 0);                  /* disable timer */
            
            TIMER1->TISR.TIF = 1;                               /* write 1 to clear for safty */

            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime1Event[i].active = FALSE;
            }

            uTimer1Tick = 0;
            _sys_uTimer1TickPerSecond = uTicksPerSecond;

            uRegTcmpr = CalTimerInitValue(GetTimerClock(E_TMR1), uTicksPerSecond); 
            if(uRegTcmpr == (uint32_t)-1)       
            {
                return E_DRVTIMER_CLOCK_RATE;           
            }

            TIMER1->TCMPR = (uRegTcmpr << 8) >> 8;
            outpw((uint32_t)&TIMER1->TCSR,  (uRegTcr|(uRegTcmpr>>24))|(op_mode<<27));   
            break;
        }

        case E_TMR2:
        {
            if ((bIsTimer2Initial != TRUE) || (bIsTimer2Used != FALSE))
                return E_DRVTIMER_EIO;    

            bIsTimer2Used = TRUE;

            SYSCLK->APBCLK.TMR2_EN =1;
            
            outpw((uint32_t)&TIMER2->TCSR, 0);                  /* disable timer */

            TIMER2->TISR.TIF = 1;                               /* write 1 to clear for safty */

            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime2Event[i].active = FALSE;
            }

            uTimer2Tick = 0;
            _sys_uTimer2TickPerSecond = uTicksPerSecond;

            uRegTcmpr = CalTimerInitValue(GetTimerClock(E_TMR2), uTicksPerSecond); 
            if(uRegTcmpr == (uint32_t)-1)       
            {
                return E_DRVTIMER_CLOCK_RATE;           
            }

            TIMER2->TCMPR = (uRegTcmpr << 8) >> 8;  
            outpw((uint32_t)&TIMER2->TCSR,  (uRegTcr|(uRegTcmpr>>24))|(op_mode<<27));   
            break;
        }

        case E_TMR3:
        {
            if ((bIsTimer3Initial != TRUE) || (bIsTimer3Used != FALSE))
                return E_DRVTIMER_EIO;

            bIsTimer3Used = TRUE;

            SYSCLK->APBCLK.TMR3_EN = 1;

            outpw((uint32_t)&TIMER3->TCSR, 0);                  /* disable timer */

            TIMER3->TISR.TIF = 1;                               /* write 1 to clear for safty */

            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime3Event[i].active = FALSE;
            }

            uTimer3Tick = 0;
            _sys_uTimer3TickPerSecond = uTicksPerSecond;
            
            uRegTcmpr = CalTimerInitValue(GetTimerClock(E_TMR3), uTicksPerSecond); 
            if(uRegTcmpr == (uint32_t)-1)       
            {
                return E_DRVTIMER_CLOCK_RATE;           
            }

            TIMER3->TCMPR = (uRegTcmpr << 8) >> 8;
            outpw((uint32_t)&TIMER3->TCSR,  (uRegTcr|(uRegTcmpr>>24))|(op_mode<<27));    
            break;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ;
        }
    }

   	if (op_mode == E_TOGGLE_MODE)
    {
        switch (ch)
        {
            case E_TMR0:
            case E_TMR1:
            case E_TMR2:
    		case E_TMR3:
            {
		    	DrvGPIO_InitFunction((E_DRVGPIO_FUNC)((uint32_t)E_FUNC_TMR0 + (uint32_t)ch));	
            }
    
            default:
            {
                return E_DRVTIMER_CHANNEL ; 
            }
        }
    }

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        TIMER_Close                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*                      E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3                           */   
/* Returns:                                                                                                */
/*                  E_SUCCESS                               Operation successful                           */
/*                  E_DRVTIMER_CHANNEL                      Invalid Timer channel                          */
/* Description:                                                                                            */
/*                  The function is used to close the timer channel.                                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_Close(E_TIMER_CHANNEL ch)
{
    switch (ch)
    {
        case E_TMR0:
        {
            DrvTIMER_DisableInt(E_TMR0);
            TIMER0->TCSR.CRST   = 1;
            bIsTimer0Used       = FALSE;
            bIsSetTime0Event    = FALSE;
            break;
        }

        case E_TMR1:
        {
            DrvTIMER_DisableInt(E_TMR1);
            TIMER1->TCSR.CRST   = 1;
            bIsTimer1Used       = FALSE;
            bIsSetTime1Event    = FALSE;
            break;
        }

        case E_TMR2:
        {
            DrvTIMER_DisableInt(E_TMR2);
            TIMER2->TCSR.CRST   = 1;
            bIsTimer2Used       = FALSE;
            bIsSetTime2Event    = FALSE;
            break;
        }

        case E_TMR3:
        {
            DrvTIMER_DisableInt(E_TMR3);
            TIMER3->TCSR.CRST   = 1;
            bIsTimer3Used       = FALSE;
            bIsSetTime3Event    = FALSE;
            break;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ;
        }
    }

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_SetTimerEvent                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*                      E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3                           */   
/*                  uInterruptTicks - [in]                                                                 */
/*                      Number of timer interrupt occurred                                                 */
/*                  pTimerCallback  - [in]                                                                 */
/*                      The function pointer of the interrupt callback function                            */
/*                  parameter - [in]                                                                       */
/*                      A parameter of the callback function                                               */
/*                                                                                                         */
/* Returns:                                                                                                */        
/*                  uTimerEventNo                           The timer event number                         */
/*                  E_DRVTIMER_EVENT_FULL                   The timer event is full                        */
/* Description:                                                                                            */
/*                  Install the interrupt callback function of the specified timer channel.                */
/*                  And trigger timer callback functuion when interrupt occur specified times.             */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_SetTimerEvent(E_TIMER_CHANNEL ch, uint32_t uInterruptTicks, TIMER_CALLBACK pTimerCallback, uint32_t parameter)
{
    volatile int32_t i;
    int32_t uTimerEventNo = 0;

    switch (ch)
    {
        case E_TMR0:
        {
            if (uTime0EventCount >= TIMER_EVENT_COUNT)
                return E_DRVTIMER_EVENT_FULL;

            bIsSetTime0Event = TRUE;
            uTime0EventCount++;
            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                if (tTime0Event[i].active  == FALSE)
                {
                    tTime0Event[i].active       = TRUE;
                    tTime0Event[i].initTick     = uInterruptTicks;
                    tTime0Event[i].curTick      = uInterruptTicks;
                    tTime0Event[i].funPtr       = (TIMER_CALLBACK)pTimerCallback;
                    tTime0Event[i].transParam   = parameter;
                    uTimerEventNo = i;
                    break;
                }
            }
            break;
        }

        case E_TMR1:
        {
            if (uTime1EventCount >= TIMER_EVENT_COUNT)
                return E_DRVTIMER_EVENT_FULL;

            bIsSetTime1Event = TRUE;
            uTime1EventCount++;
            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                if (tTime1Event[i].active   == FALSE)
                {
                    tTime1Event[i].active       = TRUE;
                    tTime1Event[i].initTick     = uInterruptTicks;
                    tTime1Event[i].curTick      = uInterruptTicks;
                    tTime1Event[i].funPtr       = (TIMER_CALLBACK)pTimerCallback;
                    tTime1Event[i].transParam   = parameter;
                    uTimerEventNo = i;
                    break;
                }
            }
            break;
        }

        case E_TMR2:
        {
            if (uTime2EventCount >= TIMER_EVENT_COUNT)
                return E_DRVTIMER_EVENT_FULL;

            bIsSetTime2Event = TRUE;
            uTime2EventCount++;
            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                if (tTime2Event[i].active   == FALSE)
                {
                    tTime2Event[i].active       = TRUE;
                    tTime2Event[i].initTick     = uInterruptTicks;
                    tTime2Event[i].curTick      = uInterruptTicks;
                    tTime2Event[i].funPtr       = (TIMER_CALLBACK)pTimerCallback;
                    tTime2Event[i].transParam   = parameter;
                    uTimerEventNo = i;
                    break;
                }
            }
            break;
        }

        case E_TMR3:
        {
            if (uTime3EventCount >= TIMER_EVENT_COUNT)
                return E_DRVTIMER_EVENT_FULL;

            bIsSetTime3Event = TRUE;
            uTime3EventCount++;
            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                if (tTime3Event[i].active   == FALSE)
                {
                    tTime3Event[i].active       = TRUE;
                    tTime3Event[i].initTick     = uInterruptTicks;
                    tTime3Event[i].curTick      = uInterruptTicks;
                    tTime3Event[i].funPtr       = (TIMER_CALLBACK)pTimerCallback;
                    tTime3Event[i].transParam   = parameter;
                    uTimerEventNo = i;
                    break;
                }
            }
            break;
        }

        default:
        {
            break;
        }
    }

    return uTimerEventNo;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_ClearTimerEvent                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*                      E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3                           */   
/*                  uTimerEventNo - [in]                                                                   */ 
/*                      The timer event number                                                             */
/* Returns:                                                                                                */
/*                  None                                                                                   */
/* Description:                                                                                            */
/*                  Clear the timer event of the specified timer channel.                                  */
/*---------------------------------------------------------------------------------------------------------*/
void DrvTIMER_ClearTimerEvent(E_TIMER_CHANNEL ch, uint32_t uTimerEventNo)
{
    switch (ch)
    {
        case E_TMR0:
        {
            tTime0Event[uTimerEventNo].active = FALSE;
            uTime0EventCount--;
            if (uTime0EventCount == 0)
            {
                bIsSetTime0Event = FALSE;
            }
            break;
        }

        case E_TMR1:
        {
            tTime1Event[uTimerEventNo].active = FALSE;
            uTime1EventCount--;
            if (uTime1EventCount == 0)
            {
                bIsSetTime1Event = FALSE;
            }
            break;
        }

        case E_TMR2:
        {
            tTime2Event[uTimerEventNo].active = FALSE;
            uTime2EventCount--;
            if (uTime2EventCount == 0)
            {
                bIsSetTime2Event = FALSE;
            }
            break;
        }

        case E_TMR3:
        {
            tTime3Event[uTimerEventNo].active = FALSE;
            uTime3EventCount--;
            if (uTime3EventCount == 0)
            {
                bIsSetTime3Event = FALSE;
            }
            break;
        }

        default:
        {
            break;
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_EnableInt                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*                      E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3                           */   
/* Returns:                                                                                                */
/*                  E_SUCCESS                               Operation successful                           */
/*                  E_DRVTIMER_CHANNEL                      Invalid Timer channel                          */
/* Description:                                                                                            */
/*                  This function is used to enable the specified timer interrupt.                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_EnableInt(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TCSR.IE = 1;
            NVIC_SetPriority((IRQn_Type)((uint32_t)TMR0_IRQn + (uint32_t)ch), (1<<__NVIC_PRIO_BITS) - 2);
			NVIC_EnableIRQ((IRQn_Type)((uint32_t)TMR0_IRQn + (uint32_t)ch)); 
            return E_SUCCESS ;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_DisableInt                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*                      E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3                           */   
/* Returns:                                                                                                */
/*                  E_SUCCESS                               Operation successful                           */
/*                  E_DRVTIMER_CHANNEL                      Invalid Timer channel                          */
/* Description:                                                                                            */
/*                  This function is used to disable the specified timer interrupt                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_DisableInt(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TCSR.IE = 0;
			NVIC_DisableIRQ((IRQn_Type)((uint32_t)TMR0_IRQn + (uint32_t)ch)); 
            return E_SUCCESS ;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_GetIntFlag                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*                      E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3                           */   
/* Returns:                                                                                                */   
/*                  iIntStatus                              0:No interrupt / 1:Interrupt occurred          */
/*                  E_DRVTIMER_CHANNEL                      Invalid Timer channel                          */
/* Description:                                                                                            */
/*                  Get the interrupt flag status from the specified timer channel.                        */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_GetIntFlag(E_TIMER_CHANNEL ch)
{
	if (ch == E_TMR0 )
		return TIMER0->TISR.TIF;
	else if(ch == E_TMR1 )									 
		return TIMER1->TISR.TIF;
	else if(ch == E_TMR2 )
		return TIMER2->TISR.TIF;
	else if(ch == E_TMR3 )
		return TIMER3->TISR.TIF;
	else 
    	return E_DRVTIMER_CHANNEL;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_ClearIntFlag                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*                      E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3                           */   
/* Returns:                                                                                                */   
/*                  E_SUCCESS                               Operation successful                           */
/*                  E_DRVTIMER_CHANNEL                      Invalid Timer channel                          */
/* Description:                                                                                            */
/*                  Clear the interrupt flag of the specified timer channel.                               */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_ClearIntFlag(E_TIMER_CHANNEL ch)
{
    if (ch == E_TMR0 )
        TIMER0->TISR.TIF = 1;
    else if(ch == E_TMR1 )                                   
        TIMER1->TISR.TIF = 1;
    else if(ch == E_TMR2 )
        TIMER2->TISR.TIF = 1;
    else if(ch == E_TMR3 )
        TIMER3->TISR.TIF = 1;
    else 
        return E_DRVTIMER_CHANNEL;

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_Start                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*                      E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3                           */   
/* Returns:                                                                                                */
/*                  E_SUCCESS                               Operation successful                           */
/*                  E_DRVTIMER_CHANNEL                      Invalid Timer channel                          */
/* Description:                                                                                            */
/*                  Start to count the specified timer channel.                                            */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_Start(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TCSR.CEN = 1;
            return E_SUCCESS ;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_GetIntTicks                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*                      E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3                           */   
/* Returns:                                                                                                */
/*                  uTimerTick                              Return the interrupt ticks                     */
/*                  E_DRVTIMER_CHANNEL                      Invalid Timer channel                          */
/* Description:                                                                                            */
/*                  This function is used to get the number of interrupt occurred                          */
/*                  after the timer interrupt function is enabled.                          .              */
/*                  Thus DrvTIMER_EnableInt(ch) must been called in advance.                               */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvTIMER_GetIntTicks(E_TIMER_CHANNEL ch)
{
    switch (ch)
    {
        case E_TMR0:
        {
            return uTimer0Tick;  
        }

        case E_TMR1:
        {
            return uTimer1Tick;
        }   

        case E_TMR2:
        {
            return uTimer2Tick;
        }

        case E_TMR3:
        {
            return uTimer3Tick;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL;
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_ResetIntTicks                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*                      E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3                           */   
/* Returns:                                                                                                */
/*                  E_SUCCESS                               Operation successful                           */
/*                  E_DRVTIMER_CHANNEL                      Invalid Timer channel                          */
/* Description:                                                                                            */
/*                  This function is used to clear interrupt ticks to 0.                                   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_ResetIntTicks(E_TIMER_CHANNEL ch)
{
    switch (ch)
    {
        case E_TMR0:
        {
            uTimer0Tick = 0;
            break;
        }

        case E_TMR1:
        {
            uTimer1Tick = 0;
            break;
        }

        case E_TMR2:
        {
            uTimer2Tick = 0;
            break;
        }

        case E_TMR3:
        {
            uTimer3Tick = 0;
            break;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL;
        }
    }

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_Delay                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*                      E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3                           */   
/*                  uIntTicks - [in]                                                                       */
/*                      The delay ticks                                                                    */
/* Returns:                                                                                                */
/*                  None                                                                                   */
/* Description:                                                                                            */
/*                  This function is used to add a delay loop by specified interrupt ticks                 */
/*                  of the timer channel.                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void DrvTIMER_Delay(E_TIMER_CHANNEL ch, uint32_t uIntTicks)
{
    uint32_t volatile btime;

    btime = DrvTIMER_GetIntTicks(ch);
    
    while (1)
    {
        if ((DrvTIMER_GetIntTicks(ch) - btime) > uIntTicks)
        {
            break;
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_OpenCounter                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*                      E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3                           */   
/*                  uCounterBoundary - [in]                                                                */
/*                      The parameter is used to determine how many counts occurred will                   */
/*                      toggle once timer interrupt.                                                       */
/*                  op_mode - [in]                                                                         */
/*                      E_TIMER_OPMODE, E_ONESHOT_MODE/E_PERIODIC_MODE/E_CONTINUOUS_MODE                   */
/* Returns:                                                                                                */
/*                  E_SUCCESS                               Operation successful                           */
/*                  E_DRVTIMER_CHANNEL                      Invalid Timer channel                          */
/*                  E_DRVTIMER_EIO                          Timer has not been initialized                 */
/* Description:                                                                                            */
/*                  This function is used to open the timer channel with the specified operation mode.     */
/*                  And the counting source of timer is from the external event/counter.                   */
/*                  The TIMER clock source should be set as HCLK.                                          */
/*                  Only NuMicro NUC1x0xxxBx and NUC1x0xxxCx series support this function,                 */  
/*                  ex:NUC140RD2BN, NUC140VE3CN.                                                           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_OpenCounter(E_TIMER_CHANNEL ch, uint32_t uCounterBoundary, E_TIMER_OPMODE op_mode)
{
    uint32_t i;
	TIMER_T * tTMR;

    if ((bIsTimer0Initial == FALSE) || (bIsTimer1Initial == FALSE) || 
        (bIsTimer2Initial == FALSE) || (bIsTimer3Initial == FALSE))
    {
        return E_DRVTIMER_EIO;
    }

	if (op_mode == E_TOGGLE_MODE)
        return E_DRVTIMER_CHANNEL;
				    	
   	switch (ch)
    {
        case E_TMR0:
        {
            if (bIsTimer0Used != FALSE)
                return E_DRVTIMER_EIO;
            
            bIsTimer0Used = TRUE;
           
 		   	SYSCLK->APBCLK.TMR0_EN = 1;

			outpw((uint32_t)&TIMER0->TCSR ,0 );	/* Disable timer */
			
           	for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime0Event[i].active = FALSE;
            }
            uTimer0Tick = 0;
            break;
        }

        case E_TMR1:
        {
            if (bIsTimer1Used != FALSE)
                return E_DRVTIMER_EIO;
            				           
            bIsTimer1Used = TRUE;
           
 		   	SYSCLK->APBCLK.TMR1_EN = 1;

			outpw((uint32_t)&TIMER1->TCSR ,0 );	/* Disable timer */

           	for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime1Event[i].active = FALSE;
            }
            uTimer1Tick = 0;
            break;
        }

        case E_TMR2:
        {
            if (bIsTimer2Used != FALSE)
                return E_DRVTIMER_EIO;
            					           
            bIsTimer2Used = TRUE;
           
 		   	SYSCLK->APBCLK.TMR2_EN = 1;

			outpw((uint32_t)&TIMER2->TCSR ,0 );	/* Disable timer */

           	for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime2Event[i].active = FALSE;
            }
            uTimer2Tick = 0;
            break;
        }

		case E_TMR3:
        {
            if (bIsTimer3Used != FALSE)
                return E_DRVTIMER_EIO;
            						           
            bIsTimer3Used = TRUE;
           
 		   	SYSCLK->APBCLK.TMR3_EN = 1;

			outpw((uint32_t)&TIMER3->TCSR ,0 );	/* Disable timer */

           	for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime3Event[i].active = FALSE;
            }
      		uTimer3Tick = 0;
            break;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
        case E_TMR3:
        {
            /* TIMER clock source should be set as HCLK */
            DrvSYS_SelectIPClockSource((E_SYS_IP_CLKSRC)((uint32_t)E_SYS_TMR0_CLKSRC+(uint32_t)ch), 2);  

			DrvGPIO_InitFunction((E_DRVGPIO_FUNC)((uint32_t)E_FUNC_TMR0 + (uint32_t)ch));	/* Open external Timer Counter source */

        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);   
			tTMR->TISR.TIF      = 1;        /* Write 1 to clear for safty */			
 			tTMR->TCSR.MODE     = op_mode;	/* Set operation mode */
			tTMR->TCMPR         = uCounterBoundary;
			tTMR->TCSR.PRESCALE = 0;
			tTMR->TCSR.TDR_EN 	= 1;	
			tTMR->TCSR.IE       = 0;	
			tTMR->TCSR.CTB      = 1;
            break;			 
        }
    }

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_StartCounter                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/* Returns:                                                                                                */
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*                  Start counting of the specified timer channel.						                   */
/*                  Only NuMicro NUC1x0xxxBx and NUC1x0xxxCx series support this function,                 */  
/*                  ex:NUC140RD2BN, NUC140VE3CN.                                                           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_StartCounter(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);   
            if (tTMR->TCSR.IE == 0)
				tTMR->TCMPR = 0;	// If use Polling Mode, do not configure Timer Compare Register. 
			tTMR->TCSR.CRST 	= 1;					
			tTMR->TCSR.CEN 	    = 1;					
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_GetCounters                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/* Returns:                                                                                                */
/*					u32Counters								Return current counters			               */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*                  This function is used to get the current counters of the specified timer channel.      */
/*                  Only NuMicro NUC1x0xxxBx and NUC1x0xxxCx series support this function,                 */  
/*                  ex:NUC140RD2BN, NUC140VE3CN.                                                           */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvTIMER_GetCounters(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            return tTMR->TDR;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_OpenCapture                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/*                  mode - [in]                                                                            */
/*                      E_TIMER_RSTCAP_MODE,                                                               */
/*                          E_CAPTURE : Run capture function                                               */
/*                          E_RESET   : Reset counter value of specified timer channel                     */
/* Returns:                                                                                                */
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*                  This function is used to initial the external timer capture source and                 */
/*                  set to start catpure or reset specified timer counter.                                 */
/*                  The TIMER clock source should be set as HCLK.                                          */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_OpenCapture(E_TIMER_CHANNEL ch, E_TIMER_RSTCAP_MODE mode)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
            /* TIMER clock source should be set as HCLK */
            DrvSYS_SelectIPClockSource((E_SYS_IP_CLKSRC)((uint32_t)E_SYS_TMR0_CLKSRC+(uint32_t)ch), 2);  

            DrvGPIO_InitFunction((E_DRVGPIO_FUNC)((uint32_t)E_FUNC_T0EX + (uint32_t)ch));
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TEXCON.RSTCAPSEL = mode;
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_CloseCapture                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/* Returns:                                                                                                */
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*                  This function is used to close the external timer capture source.                      */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_CloseCapture(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
			outpw((uint32_t)&tTMR->TEXCON ,0 );	
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_SelectExternalMode                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/*                  mode - [in]                                                                            */
/*                      E_TIMER_RSTCAP_MODE,                                                               */
/*                          E_CAPTURE : Run capture function                                               */
/*                          E_RESET   : Reset counter value of specified timer channel                     */
/* Returns:                                                                                                */
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*                  This function is used to select to run capture function or reset the timer counter.    */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_SelectExternalMode(E_TIMER_CHANNEL ch, E_TIMER_RSTCAP_MODE mode)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TEXCON.RSTCAPSEL = mode;
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_SelectCaptureEdge                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/*                  edge - [in]                                                                            */
/*                      E_TIMER_TEX_EDGE,                                                                  */
/*                          E_EDGE_FALLING : 1 to 0 transition on TEX will be detected.                    */
/*                          E_EDGE_RISING  : 0 to 1 transition on TEX will be detected.                    */
/*                          E_EDGE_BOTH    : either 0 to 1 or 1 to 0 transition on TEX will be detected.   */
/* Returns:                                                                                                */
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*                  This function is used to configure the detect edge of timer capture mode.              */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_SelectCaptureEdge(E_TIMER_CHANNEL ch, E_TIMER_TEX_EDGE edge)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TEXCON.TEX_EDGE = edge;
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_EnableCaptureInt                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/* Returns:                                                                                                */
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*                  This function is used to enable the timer external interrupt function. If any          */
/*                  transition on TEX pin and matched with the E_TIMER_TEX_EDGE settings, system will      */
/*                  cause the external interrupt flag(TEXIF) to 1.                                         */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_EnableCaptureInt(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TEXCON.TEXIEN = 1;
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_DisableCaptureInt                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/* Returns:                                                                                                */
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*                  This function is used to disable the timer external interrupt function.                */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_DisableCaptureInt(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TEXCON.TEXIEN = 0;
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_EnableCapture                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/* Returns:                                                                                                */
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*                  This function is used to enable the specified capture function.                        */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_EnableCapture(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TEXCON.TEXEN = 1;
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_DisableCapture                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/* Returns:                                                                                                */
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*                  This function is used to disable the specified capture function.                       */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_DisableCapture(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TEXCON.TEXEN = 0;
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_GetCaptureData                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/* Returns:                                                                                                */
/*					Capture value							Return capture value    	                   */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*                  This function is used to get the capture value of the specified timer channel.         */
/*                  And the return data is valid only if the capture interrupt flag set to 1 by H.W.       */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvTIMER_GetCaptureData(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            return tTMR->TCAP;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     	DrvTIMER_GetCaptureIntFlag                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/* Returns:      																						   */	
/*					External interrupt flag 				0:No interrupt / 1:Interrupt occurred		   */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*               	Get the external interrupt flag status from the specified timer channel.	  		   */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_GetCaptureIntFlag(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            return tTMR->TEXISR.TEXIF;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     	DrvTIMER_ClearCaptureIntFlag                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/* Returns:      																						   */	
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*               	Clear the external interrupt flag of the specified timer channel.					   */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_ClearCaptureIntFlag(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TEXISR.TEXIF = 1;
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     	DrvTIMER_EnableCaptureDebounce                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/* Returns:      																						   */	
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*               	Enable the debounce function of specified external capture input source.               */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_EnableCaptureDebounce(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TEXCON.TEXDB = 1;
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     	DrvTIMER_DisableCaptureDebounce                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/* Returns:      																						   */	
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*               	Disable the debounce function of specified external capture input source.              */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_DisableCaptureDebounce(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TEXCON.TEXDB = 0;
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     	DrvTIMER_EnableCounterDebounce                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/* Returns:      																						   */	
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*               	Enable the debounce function of specified external counter input source.               */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_EnableCounterDebounce(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TEXCON.TCDB = 1;
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     	DrvTIMER_DisableCounterDebounce                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/* Returns:      																						   */	
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*               	Disable the debounce function of specified external counter input source.              */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_DisableCounterDebounce(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TEXCON.TCDB = 0;
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_SelectCounterDetectPhase                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ch - [in]                                                                              */
/*      			    E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3						   */	
/*                  phase - [in]                                                                           */
/*                      E_TIMER_TX_PHASE,                                                                  */
/*                          E_PHASE_FALLING : A falling edge of external counter pin will be counted.      */
/*                          E_PHASE_RISING  : A rising edge of external counter pin will be counted.       */
/* Returns:                                                                                                */
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVTIMER_CHANNEL						Invalid Timer channel        		           */
/* Description:                                                                                            */
/*                  This function is used to configure the counter detect phase of specified source.       */
/*                  Only NuMicro NUC1x0xxxCx series support this function, ex:NUC140VE3CN.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_SelectCounterDetectPhase(E_TIMER_CHANNEL ch, E_TIMER_TX_PHASE phase)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TEXCON.TX_PHASE = phase;
            return E_SUCCESS;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvTIMER_GetVersion                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  None                                                                                   */
/* Returns:                                                                                                */
/*                  DRVTIMER_VERSION_NUM                        Version number of Timer/WDT driver         */
/* Description:                                                                                            */
/*                 Get the version number of Timer/WDT driver.                                             */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvTIMER_GetVersion(void)
{
    return DRVTIMER_VERSION_NUM;
}






/*---------------------------------------------------------------------------------------------------------*/
/* Function:        WDT_IRQHandler                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  None                                                                                   */
/* Returns:                                                                                                */
/*                  None                                                                                   */
/* Description:                                                                                            */
/*                  The WatchDog Timer(WDT) default IRQ, declared in startup_NUC1xx.s                      */
/*                  All bits in WDT register are write-protected. User must to check the REGWRPROT bit is  */
/*                  enabled or disabled if write the specified WDT bit fail.                               */
/*---------------------------------------------------------------------------------------------------------*/
void WDT_IRQHandler(void)
{
    volatile uint32_t uWakeupFlag;

    uWakeupFlag = WDT->WTCR.WTWKF;  /* Stored WDT wakeup flag */

    WDT->WTCR.WTIF = 1;             /* Clear the WDT INT Flag */

    if (fnCallBack_WDT)        
    {
        fnCallBack_WDT(uWakeupFlag);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvWDT_Open                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  WDTlevel -[in]                                                                         */
/*                      E_WDT_INTERVAL, enumerate the WDT time-out interval.                               */
/*                      Refer to WDT_INTERVAL enumeration for detail time-out value.                       */
/* Returns:                                                                                                */
/*					E_SUCCESS								Operation successful   				           */
/*					E_DRVWDT_OPEN						    WDT open fail                  		           */
/* Description:                                                                                            */
/*                  Enable WDT engine clock and set WDT time-out interval.                                 */
/*                  All bits in WDT register are write-protected. User must to check the REGWRPROT bit is  */
/*                  enabled or disabled if write the specified WDT bit fail.                               */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvWDT_Open(E_WDT_INTERVAL WDTlevel)
{
	if ((SYS->REGWRPROT & 0x01) == 0)
    {
        /* The protected Registers are locked */
        return E_DRVWDT_OPEN;
    }

    SYSCLK->APBCLK.WDT_EN = 1;              /* Enable WatchDog Timer Clock */   
     
    DrvWDT_Ioctl(E_WDT_IOC_STOP_TIMER, 0);  /* Stop WDT first */

    WDT->WTCR.WTIS = WDTlevel;              /* Select WatchDog Timer Interval */

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvWDT_Close                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  None                                                                                   */
/* Returns:                                                                                                */
/*                  None                                                                                   */
/* Description:                                                                                            */
/*                  The function is used to stop/disable WDT relative functions.                           */
/*                  All bits in WDT register are write-protected. User must to check the REGWRPROT bit is  */
/*                  enabled or disabled if write the specified WDT bit fail.                               */
/*---------------------------------------------------------------------------------------------------------*/
void DrvWDT_Close(void)
{
    DrvWDT_Ioctl(E_WDT_IOC_STOP_TIMER, 0);      /* Stop WDT */
    DrvWDT_Ioctl(E_WDT_IOC_DISABLE_INT, 0);     /* Disable WDT Interrupt */
    DrvWDT_Ioctl(E_WDT_IOC_DISABLE_WAKEUP, 0);  /* Disable WDT wakeup fuction */              
    
    /* Add delay loop to make sure the internal WDT status is stable then disable WDT engine clock */ 
    if (DrvSYS_GetHCLKFreq() != __IRC10K)
    {        
        volatile uint32_t i=0x2000;
        while (i--);
    }

    SYSCLK->APBCLK.WDT_EN = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvWDT_InstallISR                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  pvWDTISR - [in]                                                                        */
/*                      The function pointer of the interrupt service routine                              */
/* Returns:                                                                                                */
/*                  None                                                                                   */
/* Description:                                                                                            */
/*                  The function is used to install WDT interrupt service routine.                         */
/*                  All bits in WDT register are write-protected. User must to check the REGWRPROT bit is  */
/*                  enabled or disabled if write the specified WDT bit fail.                               */
/*---------------------------------------------------------------------------------------------------------*/
void DrvWDT_InstallISR(WDT_CALLBACK pvWDTISR)
{
    WDT->WTCR.WTIE = 1;
    fnCallBack_WDT = (WDT_CALLBACK)pvWDTISR;    
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvWDT_Ioctl                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  uWDTCmd - [in]                                                                         */
/*                      E_WDT_CMD commands, it could be the one of the follow commands                     */
/*                          E_WDT_IOC_START_TIMER,                                                         */
/*                          E_WDT_IOC_STOP_TIMER,                                                          */
/*                          E_WDT_IOC_ENABLE_INT,                                                          */
/*                          E_WDT_IOC_DISABLE_INT,                                                         */
/*                          E_WDT_IOC_ENABLE_WAKEUP,                                                       */
/*                          E_WDT_IOC_DISABLE_WAKEUP,                                                      */
/*                          E_WDT_IOC_RESET_TIMER,                                                         */
/*                          E_WDT_IOC_ENABLE_RESET_FUNC,                                                   */
/*                          E_WDT_IOC_DISABLE_RESET_FUNC,                                                  */
/*                          E_WDT_IOC_SET_INTERVAL                                                         */
/*                  uArgument - [in]                                                                       */
/*                      Set the argument for the specified WDT command                                     */
/* Returns:                                                                                                */
/*                  E_SUCCESS                               Operation successful                           */
/*                  E_DRVWDT_CMD                            Invalid WDT command                            */
/* Description:                                                                                            */
/*                  The function is used to operate more WDT applications, it could be the                 */
/*                  start/stop the WDT, enable/disable WDT interrupt function, enable/disable WDT          */
/*                  time-out wake up function, enable/disable system reset when WDT time-out and           */
/*                  set the WDT time-out interval.                                                         */
/*                  All bits in WDT register are write-protected. User must to check the REGWRPROT bit is  */
/*                  enabled or disabled if write the specified WDT bit fail.                               */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvWDT_Ioctl(E_WDT_CMD uWDTCmd, uint32_t uArgument)
{
    switch (uWDTCmd)
    {
        case E_WDT_IOC_START_TIMER  :
        {
            WDT->WTCR.WTE = 1;
            break ;
        }

        case E_WDT_IOC_STOP_TIMER :
        {
            WDT->WTCR.WTE = 0;
            break ;
        }

        case E_WDT_IOC_ENABLE_INT :
        {
            WDT->WTCR.WTIE = 1;
            NVIC_SetPriority(WDT_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
            NVIC_EnableIRQ(WDT_IRQn); 
            break ;
        }

        case E_WDT_IOC_DISABLE_INT :
        {
            WDT->WTCR.WTIE = 0;
            NVIC_DisableIRQ(WDT_IRQn); 
            break ;
        }

        case E_WDT_IOC_ENABLE_WAKEUP :
        {
            WDT->WTCR.WTWKE = 1;            
            break ;
        }

        case E_WDT_IOC_DISABLE_WAKEUP :
        {
            WDT->WTCR.WTWKE = 0;            
            break ;
        }

        case E_WDT_IOC_RESET_TIMER:
        {
            WDT->WTCR.WTR = 1;
            break ;
        }

        case E_WDT_IOC_ENABLE_RESET_FUNC :
        {
            WDT->WTCR.WTRE = 1;
            break ;
        }

        case E_WDT_IOC_DISABLE_RESET_FUNC:
        {
            WDT->WTCR.WTRE = 0;
            break ;
        }

        case E_WDT_IOC_SET_INTERVAL:
        {
            WDT->WTCR.WTIS = (E_WDT_INTERVAL)uArgument;

            break ;
        }
       
        default :
        {
            return E_DRVWDT_CMD;
        }
    }

    return E_SUCCESS;
}

