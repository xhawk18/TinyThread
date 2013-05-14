/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include "DrvUSB.h"

S_DRVUSB_DEVICE gsUsbDevice;
PFN_DRVUSB_INTCALLBACK g_FnIntCallBack;
uint32_t CFG_EP_SETTING[6];
static uint8_t *g_UsbSramBase;

static int32_t DrvUSB_Init(void);
static void DrvUSB_UnInit(void);

void WordsCpy(void *dest, void *src, int32_t size)
{
    uint8_t *pu8Src, *pu8Dest;
    int32_t i;
    
    pu8Dest = (uint8_t *)dest;
    pu8Src  = (uint8_t *)src;
    
    for(i=0;i<size;i++)
        pu8Dest[i] = pu8Src[i]; 
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_GetVersion                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      Version number                                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Get USB driver's version                                                                           */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvUSB_GetVersion(void)
{
    return DRVUSB_VERSION_NUM;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_DataOutTrigger                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      USB interrupt handler                                                                              */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void USBD_IRQHandler(void)
{
     DrvUSB_PreDispatchEvent();
     if (g_FnIntCallBack)
         g_FnIntCallBack((void *)&gsUsbDevice);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_Open                                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid     user interrupt callback function, it will be call in UsbIntHandler                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      0           Success                                                                                */
/*      Otherwise   Error                                                                                  */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Open USB, include assign USB SRAM, init state and install                                          */
/*      interrupt handlerClose a file                                                                      */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvUSB_Open(void * pVoid)
{
    uint32_t i, j;
    uint8_t *p;
    
    DrvUSB_Init();
    
    if (pVoid != NULL)
        g_FnIntCallBack = (PFN_DRVUSB_INTCALLBACK)pVoid;
    
    gsUsbDevice.u16MiscEventFlags = 0;
    gsUsbDevice.u16EPEventFlags = 0;
    
    p = (uint8_t *)USB_SRAM_BASE;
    
    /* for setup packet */
    gsUsbDevice.sEpCrl[MAX_EP_ID].u32EpNum = 0x0;
    gsUsbDevice.sEpCrl[MAX_EP_ID].u32MaxPacketSize = 8;
    gsUsbDevice.sEpCrl[MAX_EP_ID].u8SramBuffer = p;
    p += 8;
    
    i = 0;
    while (sEpDescription[i].u32MaxPacketSize != 0)
    {       
        /* There may be some EPs which have the same EP address. We will find them out here in order to use same buffer. */
        for (j = 0; j < i; j++)
        {
            if ((sEpDescription[i].u32EpNum & 0xF) == (gsUsbDevice.sEpCrl[j].u32EpNum & 0xF))
                break;
        }
        
        gsUsbDevice.sEpCrl[i].u32EpNum = sEpDescription[i].u32EpNum;
        gsUsbDevice.sEpCrl[i].u32MaxPacketSize = sEpDescription[i].u32MaxPacketSize;
        
        /* Assign the transfer buffer */
        if (sEpDescription[i].u8SramBuffer == NULL)
        {               
            if (j < i)
            {
                /* Use the same buffer if they have the same EP address */
                gsUsbDevice.sEpCrl[i].u8SramBuffer = gsUsbDevice.sEpCrl[j].u8SramBuffer;
            }
            else
            {
                /* Assign a new buffer */
                gsUsbDevice.sEpCrl[i].u8SramBuffer = p;
                p += sEpDescription[i].u32MaxPacketSize;
            }
            
            /* Write back the assigned buffer */
            sEpDescription[i].u8SramBuffer = gsUsbDevice.sEpCrl[i].u8SramBuffer;
        }

        if ((sEpDescription[i].u32EpNum & 0x1f) == 0)
        {
            /* Only ctrl EP needs to support STALL auto clear */
            CFG_EP_SETTING[i] = (CFG_CSTALL | 
              ((sEpDescription[i].u32EpNum&EP_INPUT)?CFG_EPT_IN:CFG_EPT_OUT) 
              | (sEpDescription[i].u32EpNum&0x0F));
        }
        else
        {
            CFG_EP_SETTING[i] = ((sEpDescription[i].u32EpNum&EP_INPUT)?CFG_EPT_IN:CFG_EPT_OUT) 
              | (sEpDescription[i].u32EpNum&0x0F);
        }
        
        i++;
    }
    
    /* Update the remind buffer base */
    g_UsbSramBase = p;

    gsUsbDevice.eUsbState = eDRVUSB_DETACHED;
    gsUsbDevice.u32FLDET = USBD->FLDET.FLDET;
    DrvUSB_PreDispatchFDTEvent(&gsUsbDevice);
    DrvUSB_DispatchMiscEvent(&gsUsbDevice);
    
    NVIC_SetPriority (USBD_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
    NVIC_EnableIRQ(USBD_IRQn);

    return 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_PreDispatchEvent                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Pre dispatch event base on EVF register                                                            */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_PreDispatchEvent(void)
{
    gsUsbDevice.u32INTSTS = _DRVUSB_GET_EVENT_FLAG();

    if (gsUsbDevice.u32INTSTS & INTSTS_WAKEUP)
    {
        /* Clear wakeup event. write one clear */
        _DRVUSB_SET_EVENT_FLAG(INTSTS_WAKEUP);
        
        /* Pre-dispatch wakeup event. */
        DrvUSB_PreDispatchWakeupEvent(&gsUsbDevice);
    }
    else if (gsUsbDevice.u32INTSTS & INTSTS_FLDET)
    {
        gsUsbDevice.u32FLDET = USBD->FLDET.FLDET;
        /* Clear float-detection event. Write one clear */
        _DRVUSB_SET_EVENT_FLAG(INTSTS_FLDET);

        /* Pre-dispatch float-detection event. */
        DrvUSB_PreDispatchFDTEvent(&gsUsbDevice);
    }
    else if (gsUsbDevice.u32INTSTS & INTSTS_BUS)
    {
        gsUsbDevice.u32ATTR = *((__IO uint32_t *)(&USBD->ATTR));
        /* Clear bus event. Write one clear */
        _DRVUSB_SET_EVENT_FLAG(INTSTS_BUS);
        
        /* Pre-dispatch bus event. */
        DrvUSB_PreDispatchBusEvent(&gsUsbDevice);
    }
    else if (gsUsbDevice.u32INTSTS & INTSTS_USB)
    {
        gsUsbDevice.u32EPSTS = _DRVUSB_GET_EPSTS();
        
        /* Clear USB events individually instead of in total.
           Otherwise, incoming USB events may be cleared mistakenly.
           Pre-dispatch USB event. */
        DrvUSB_PreDispatchEPEvent(&gsUsbDevice);
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_DispatchEvent                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Dispatch Misc and EP event                                                                         */
/*      Misc event include  attach/detach/bus reset/bus suspend/                                           */
/*      bus resume and setup ACK, Misc event'handler is g_sBusOps[]                                        */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_DispatchEvent(void)
{
    DrvUSB_DispatchMiscEvent(&gsUsbDevice);
    DrvUSB_DispatchEPEvent(&gsUsbDevice);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_IsData0                                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      u32EpId   [in]  EP Id. The hardware id of specified endpoint                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      To check if the current DATA is DATA0. If it is false, then it                                     */
/*      should be DATA1.                                                                                   */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvUSB_IsData0(uint32_t u32EpId)
{
    int32_t bData0 = 0;

    if (u32EpId >= MAX_EP_ID)
    {
        bData0 = 0;
    }
    else
    {
        bData0 = gsUsbDevice.abData0[u32EpId];
    }

    return bData0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_GetUsbState                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Get current USB state E_DRVUSB_STATE                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
E_DRVUSB_STATE DrvUSB_GetUsbState(void)
{
    return (E_DRVUSB_STATE)gsUsbDevice.eUsbState;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_SetUsbState                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      eUsbState   [in]  The wanted state                                                                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Change current USB state                                                                           */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_SetUsbState(E_DRVUSB_STATE eUsbState)
{
    gsUsbDevice.eUsbState = eUsbState;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_Close                                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Close USB: uninstall interrupt handler, disable USB clock                                          */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_Close(void)
{    
    NVIC_DisableIRQ(USBD_IRQn);
    DrvUSB_UnInit();
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_GetEpIdentity                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      u32EpNum   [in]   EP number                                                                        */
/*      u32EPAttr  [in]   EP_OUTPUT/EP_INPUT                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      0~5         EP id                                                                                  */
/*      Otherwise   Error                                                                                  */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Get endpoint id base on endpoint number and direction. The                                         */
/*      endpoint id is used to identify the hardware endpoint resource.                                    */
/*      The range of endpoint id could be 0 ~ 5. The endpoint number is                                    */
/*      assigned by software and it could be 0 ~ 15. Host will access                                      */
/*      the device through relative endpoint number.                                                       */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvUSB_GetEpIdentity(uint32_t u32EpNum, uint32_t u32EpAttr)
{
    uint32_t i;

    S_DRVUSB_DEVICE *psDevice =&gsUsbDevice;
    
    for ( i = 0; i < MAX_EP_ID; i++)
    {
        if (psDevice->sEpCrl[i].u32EpNum == (u32EpNum | u32EpAttr))
            return i;
    }
    
    return E_DRVUSB_INVALID_EP_NUM;

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_GetEpId                                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      u32EpNum     EP number. bit7 is used to define direction.                                          */
/*                   1 = IN endpoint, 0 = OUT endpoint.                                                    */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      0~5         EP id                                                                                  */
/*      Otherwise   Error                                                                                  */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Get EP id base on EP number                                                                        */
/*      This EP number is different from DrvUSB_GetEpIdentity's. Because                                   */
/*      its argument includes direction. eg: 0x81                                                          */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvUSB_GetEpId(uint32_t u32EpNum)
{
    uint32_t i;
    S_DRVUSB_DEVICE *psDevice =&gsUsbDevice;
    
    for ( i = 0; i < MAX_EP_ID; i++)
    {
        if (psDevice->sEpCrl[i].u32EpNum == u32EpNum)
            return i;
    }
    
    return E_DRVUSB_INVALID_EP_NUM; 
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_DataOutTrigger                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      u32EpNum      EP number                                                                            */
/*      u32Size       Max size want to receive from USB                                                    */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      0           Success                                                                                */
/*      Otherwise   Error                                                                                  */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Trigger data out ready flag by write MXPLD register                                                */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvUSB_DataOutTrigger(uint32_t u32EpNum, uint32_t u32Size)
{
    uint32_t u32EpId;
    S_DRVUSB_DEVICE *psDevice = &gsUsbDevice;
    
    u32EpId = DrvUSB_GetEpIdentity(u32EpNum, EP_OUTPUT);
    
    if (u32Size > psDevice->sEpCrl[u32EpId].u32MaxPacketSize)
        return E_DRVUSB_SIZE_TOO_LONG;

        
    _DRVUSB_SET_EP_BUF(u32EpId, (uint32_t)psDevice->sEpCrl[u32EpId].u8SramBuffer);
    _DRVUSB_TRIG_EP(u32EpId, u32Size);
    
    return 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_GetOutData                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      u32EpNum   [in]    Endpoint number                                                                 */
/*      u32Size    [out]   Data size that received                                                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      USB SRAM address                                                                                   */
/*                                                                                                         */
/* Description:                                                                                            */
/*      This function will return the buffer pointer of u32EpNum's out                                     */
/*      USB SRAM buffer. User can use this pointer to get the data                                         */
/*      payload of current data out packet.                                                                */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t * DrvUSB_GetOutData(uint32_t u32EpNum, uint32_t *u32Size)
{
    uint32_t u32EpId;
    
    u32EpId = DrvUSB_GetEpIdentity(u32EpNum, EP_OUTPUT);
    *u32Size = _DRVUSB_GET_EP_DATA_SIZE(u32EpId);

    return sEpDescription[u32EpId].u8SramBuffer;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_DataIn                                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*     u32EpNum   [in]  EP number, send data from it                                                       */
/*     u8Buffer   [in]  Data buffer                                                                        */
/*     u32Size    [in]  Data size                                                                          */
/*                                                                              \                          */
/* Returns:                                                                                                */
/*     0           Success                                                                                 */
/*     Otherwise    error                                                                                  */
/*                                                                                                         */
/* Description:                                                                                            */
/*     Trigger ready flag for sending data                                                                 */
/*     after receive IN token from host, USB will send the data                                            */
/*     if u8Buffer == NULL && u32Size == 0 then send DATA1 always                                          */
/*     else DATA0 and DATA1 by turns                                                                       */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvUSB_DataIn(uint32_t u32EpNum, const uint8_t * u8Buffer, uint32_t u32Size)
{
    S_DRVUSB_DEVICE *psDevice = &gsUsbDevice;
    uint32_t u32EpId;
    
    u32EpId = DrvUSB_GetEpIdentity(u32EpNum, EP_INPUT);
    
    if (u32Size > psDevice->sEpCrl[u32EpId].u32MaxPacketSize)
        return E_DRVUSB_SIZE_TOO_LONG;

    
    if (u8Buffer && u32Size)
    {
        WordsCpy(psDevice->sEpCrl[u32EpId].u8SramBuffer, (void *)u8Buffer, u32Size);
    }
    
    _DRVUSB_SET_EP_BUF(u32EpId, (uint32_t)psDevice->sEpCrl[u32EpId].u8SramBuffer);
    
    if (u8Buffer == NULL && u32Size == 0)
        psDevice->abData0[u32EpId] = 0;
    else
        psDevice->abData0[u32EpId] = !psDevice->abData0[u32EpId];
    
    _DRVUSB_SET_EP_TOG_BIT(u32EpId, psDevice->abData0[u32EpId]);
    _DRVUSB_TRIG_EP(u32EpId, u32Size);
    
    return 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_BusResetCallback                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid    [in]  Parameter passed by g_sBusOps[]                                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Bus reset handler. After receiving bus reset event, this handler                                   */
/*      will be called. It will reset USB address, accept SETUP packet                                     */
/*      and initial the endpoints.                                                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_BusResetCallback(void * pVoid)
{
    S_DRVUSB_DEVICE *psDevice =&gsUsbDevice;
    int i;

    /* Reset function address of USB device */
    _DRVUSB_SET_FADDR(0x00);
    
    /* Disable IN NAK Flag */
    USBD->INTEN.INNAK_EN = 0;

    _DRVUSB_SET_SETUP_BUF(USB_BUF_SETUP);
    
    /* Initial USB EP according to EP description */    
    i = 0;
    while (sEpDescription[i].u32MaxPacketSize != 0)
    {
        /* Write one to clear IN/OUT ready flag */
        _DRVUSB_SET_CFGP(i, 0x01);
        
        /* Initial USB EP CFG Setting */
        _DRVUSB_SET_CFG(i, CFG_EP_SETTING[i]);
        _DRVUSB_SET_EP_BUF(i, (uint32_t)sEpDescription[i].u8SramBuffer);
        i++;
    }

    psDevice->u8UsbAddress = 0;
    psDevice->u8UsbConfiguration = 0;
    
    if(psDevice->eUsbState > eDRVUSB_DEFAULT)
        psDevice->eUsbState = eDRVUSB_DEFAULT;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_InstallClassDevice                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      sUsbClass  [in]   USB class structure pointer                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Register USB class device to USB driver                                                            */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void * DrvUSB_InstallClassDevice(S_DRVUSB_CLASS *sUsbClass)
{
    gsUsbDevice.psUsbClass = sUsbClass;

    return &gsUsbDevice;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_InstallCtrlHandler                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      device               [in]   USB driver device pointer                                              */
/*      psCtrlCallbackEntry  [in]   Handler structure pointer                                              */
/*      u32RegCnt            [in]   Handler structure size                                                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      E_SUCCESS               Success                                                                    */
/*      E_DRVUSB_NULL_POINTER   NULL function pointer                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Register ctrl pipe handler                                                                         */
/*      include SETUP ACK , IN ACK, OUT ACK handler                                                        */
/*      for Standard/Vendor/Class command                                                                  */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvUSB_InstallCtrlHandler(void * *device, 
                S_DRVUSB_CTRL_CALLBACK_ENTRY *psCtrlCallbackEntry,uint32_t u32RegCnt)
{
    S_DRVUSB_DEVICE *pDevice = (S_DRVUSB_DEVICE *)device;
    S_DRVUSB_CTRL_CALLBACK_ENTRY *psEntry;
    int i;

    if (u32RegCnt == 0)
    {
        return 0;
    }

    if (psCtrlCallbackEntry == 0)
    {
        return E_DRVUSB_NULL_POINTER;
    }
    
    pDevice->pCtrlCallback = psCtrlCallbackEntry;
    pDevice->CtrlCallbackSize = u32RegCnt;
    
    for (i = 0; i < u32RegCnt; i++)
    {
        psEntry = psCtrlCallbackEntry + i;
        
        if (psEntry->pfnCtrlDataInCallback == NULL)
            psEntry->pfnCtrlDataInCallback = DrvUSB_CtrlDataInDefault;
        if (psEntry->pfnCtrlDataOutCallback == NULL)
            psEntry->pfnCtrlDataOutCallback = DrvUSB_CtrlDataOutDefault;
    }

    return 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_CtrlSetupAck                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pArgu   [in]  Parameter passed by g_sBusOps[]                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      When SETUP ack interrupt happen, this function will be called.                                     */
/*      it will call SETUP handler that DrvUSB_InstallCtrlHandler registered                               */
/*      base on command category and command                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_CtrlSetupAck(void * pArgu)
{
    uint32_t i;
    E_DRVUSB_STATE eUsbState;
    S_DRVUSB_CTRL_CALLBACK_ENTRY *psEntry = 0;
    S_DRVUSB_DEVICE *psDevice = &gsUsbDevice;
    uint8_t * SetupBuffer;
    volatile int32_t u32Delay;

    DrvUSB_ClrCtrlReady();
    

    /* check if after DEFAULT state (RESET) */
    eUsbState = DrvUSB_GetUsbState();
    if (eUsbState < eDRVUSB_DEFAULT)
    {
        DrvUSB_ClrCtrlReadyAndTrigStall();
        return;
    }
    SetupBuffer = (uint8_t *)DrvUSB_GetSetupBuffer();
    
    u32Delay = 0x1000;
    while(u32Delay--);

    for (i = 0; i < 8; i++)
    {
        psDevice->au8Setup[i] = SetupBuffer[i];
    }
    u32Delay = 0x1000;
    while(u32Delay--);


    for (i = 0; i < psDevice->CtrlCallbackSize; i++)
    {
        psEntry = psDevice->pCtrlCallback + i;
        if (psEntry->pfnCtrlSetupCallback &&
            psEntry->u8RequestType == (psDevice->au8Setup[0] & 0x60) &&
            psEntry->u8Request == psDevice->au8Setup[1])
        {
            psEntry->pfnCtrlSetupCallback(psEntry->pVoid);
            return;
        }
    }
    DrvUSB_ClrCtrlReadyAndTrigStall();
    return;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_CtrlDataInAck                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pArgu   [in]  Parameter passed by g_sBusOps[]                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      When IN ack interrupt happen, this function will be called.                                        */
/*      it will call IN ACK handler that DrvUSB_InstallCtrlHandler registered                              */
/*      base on command category and command                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_CtrlDataInAck(void * pArgu)
{
    uint32_t i;
    S_DRVUSB_CTRL_CALLBACK_ENTRY *psEntry = 0;
    S_DRVUSB_DEVICE *psDevice = &gsUsbDevice;

    for (i = 0; i < psDevice->CtrlCallbackSize; i++)
    {
        psEntry = psDevice->pCtrlCallback + i;
        if (psEntry->pfnCtrlDataInCallback &&
            psEntry->u8RequestType == (psDevice->au8Setup[0] & 0x60) &&
            psEntry->u8Request == psDevice->au8Setup[1])
        {
            psEntry->pfnCtrlDataInCallback(psEntry->pVoid);
            return;
        }
    }
    return;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_CtrlDataOutAck                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pArgu   [in]  Parameter passed by g_sBusOps[]                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      When OUT ack interrupt happen, this function will be called.                                       */
/*      it will call OUT handler that DrvUSB_InstallCtrlHandler registered                                 */
/*      base on command category and command                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_CtrlDataOutAck(void * pArgu)
{
    uint32_t i;
    S_DRVUSB_CTRL_CALLBACK_ENTRY *psEntry = 0;
    S_DRVUSB_DEVICE *psDevice = &gsUsbDevice;

    for (i = 0; i < psDevice->CtrlCallbackSize; i++)
    {
        psEntry = psDevice->pCtrlCallback + i;
        if (psEntry->pfnCtrlDataOutCallback &&
            psEntry->u8RequestType == (psDevice->au8Setup[0] & 0x60) &&
            psEntry->u8Request == psDevice->au8Setup[1])
        {
            psEntry->pfnCtrlDataOutCallback(psEntry->pVoid);
            return;
        }
    }
    return;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_CtrlDataInDefault                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid    [in]  Parameter passed by DrvUSB_InstallCtrlHandler                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      IN ACK default handler                                                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_CtrlDataInDefault(void * pVoid)
{
    S_DRVUSB_DEVICE *pInfraDevice = &gsUsbDevice;
    
    if (pInfraDevice->au8Setup[0] & 0x80)
    {
        _DRVUSB_TRIG_EP(1, 0x00);
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_CtrlDataOutDefault                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid    [in]    Parameter passed by DrvUSB_InstallCtrlHandler                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      OUT ACK default handler. It is used to return zero data length                                     */
/*      packet when next IN token.                                                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_CtrlDataOutDefault(void * pVoid)
{
    S_DRVUSB_DEVICE *pInfraDevice = &gsUsbDevice;
    
    if ((pInfraDevice->au8Setup[0] & 0x80) == 0)
    {
        DrvUSB_DataIn(0, NULL, 0);
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_Reset                                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      u32EpNum   [in] EP number                                                                          */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Restore CFGx and CFGPx register                                                                    */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_Reset(uint32_t u32EpNum)
{
    uint32_t u32EpId;
    u32EpId = DrvUSB_GetEpIdentity(u32EpNum, EP_INPUT);
    
    if(u32EpId != E_DRVUSB_INVALID_EP_NUM)
    {
        _DRVUSB_SET_CFG(u32EpId,  CFG_EP_SETTING[u32EpId]);
        _DRVUSB_SET_CFGP(u32EpId, CFGP_CLRRDY);
    }

    u32EpId = DrvUSB_GetEpIdentity(u32EpNum, EP_OUTPUT);
    
    if(u32EpId != E_DRVUSB_INVALID_EP_NUM)
    {
        _DRVUSB_SET_CFG(u32EpId,  CFG_EP_SETTING[u32EpId]);
        _DRVUSB_SET_CFGP(u32EpId, CFGP_CLRRDY);
    }


}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_ClrCtrlReady                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Clear ctrl pipe ready flag that was set by MXPLD                                                   */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_ClrCtrlReady(void)
{
    uint32_t u32EpId;

    u32EpId = DrvUSB_GetEpIdentity(0, EP_OUTPUT);
    _DRVUSB_CLEAR_EP_READY(u32EpId);
    u32EpId = DrvUSB_GetEpIdentity(0, EP_INPUT);
    _DRVUSB_CLEAR_EP_READY(u32EpId);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_ClrCtrlReadyAndTrigStall                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Clear ctrl pipe ready flag that was set by MXPLD  and send stall                                   */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_ClrCtrlReadyAndTrigStall(void)
{
    uint32_t u32EpId;

    u32EpId = DrvUSB_GetEpIdentity(0, EP_OUTPUT);
    _DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(u32EpId);
    u32EpId = DrvUSB_GetEpIdentity(0, EP_INPUT);
    _DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(u32EpId);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_GetSetupBuffer                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      Setup buffer address                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Get setup buffer address in USB sram                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvUSB_GetSetupBuffer(void)
{
    return (uint32_t)gsUsbDevice.sEpCrl[MAX_EP_ID].u8SramBuffer;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_GetFreeSRAM                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      Free USB SRAM address                                                                              */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Get free USB SRAM buffer address after EP assign base on                                           */
/*      sEpDescription[i].u32MaxPacketSize in DrvUSB_Open                                                  */
/*      User can get this for dual buffer                                                                  */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvUSB_GetFreeSRAM(void)
{
    return (uint32_t)g_UsbSramBase;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_EnableSelfPower                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Enable self power attribution                                                                      */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_EnableSelfPower (void)
{   
    gsUsbDevice.bSelfPowered = 1; 
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_DisableSelfPower                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Disable self power attribution                                                                     */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_DisableSelfPower(void)
{   
    gsUsbDevice.bSelfPowered = 0; 
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_IsSelfPowerEnabled                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      1  : Self-Powereded                                                                                */
/*      0  : BUS-Powereded                                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Self-power is enabled or disabled                                                                  */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvUSB_IsSelfPowerEnabled(void)
{
    return gsUsbDevice.bSelfPowered;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_EnableRemoteWakeup                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Enable remote wakeup attribution                                                                   */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_EnableRemoteWakeup(void)
{   
    gsUsbDevice.bRemoteWakeup = 1;  
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_DisableRemoteWakeup                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Disable remote wakeup attribution                                                                  */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_DisableRemoteWakeup(void)
{   
    gsUsbDevice.bRemoteWakeup = 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_IsRemoteWakeupEnabled                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      1  : Support RemoteWakeup                                                                          */
/*      0  : Not support                                                                                   */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Remote wakeup is enabled or disabled                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvUSB_IsRemoteWakeupEnabled(void)
{
    return gsUsbDevice.bRemoteWakeup;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_SetMaxPower                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      u32MaxPower    -[in]  Max power value                                                              */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      0           Success                                                                                */
/*      Otherwise   Error                                                                                  */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Configure max power                                                                                */
/*      Maximum MaxPower 0xFA (500mA), default is 0x32 (100mA)                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvUSB_SetMaxPower(uint32_t u32MaxPower)
{   
    
    if (u32MaxPower > 0xFA)
    {
        return E_DRVUSB_VALUE_INVALID;
    }
    
    gsUsbDevice.u8MaxPower = (uint8_t)u32MaxPower;
    
    return 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_GetMaxPower                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Get current max power                                                                              */
/*      Maximum MaxPower 0xFA (500mA), default is 0x32 (100mA)                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvUSB_GetMaxPower(void)
{   
    return ((int32_t) gsUsbDevice.u8MaxPower);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_EnableUsb                                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      psDevice    -[in]  USB driver device pointer                                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Enable USB and PHY                                                                                 */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_EnableUsb(S_DRVUSB_DEVICE *psDevice)
{
    _DRVUSB_ENABLE_USB();   /* enable USB & PHY */
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_DisableUsb                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      psDevice    -[in]  USB driver device pointer                                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Disable USB and PHY                                                                                */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_DisableUsb(S_DRVUSB_DEVICE *psDevice)
{
    _DRVUSB_DISABLE_USB();  /* disable USB & PHY */
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_PreDispatchWakeupEvent                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      psDevice    -[in]  USB driver device pointer                                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      None                                                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_PreDispatchWakeupEvent(S_DRVUSB_DEVICE *psDevice)
{

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_PreDispatchFDTEvent                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      psDevice    -[in]  USB driver device pointer                                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Pre dispatch plug-in and plug-out event                                                            */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_PreDispatchFDTEvent(S_DRVUSB_DEVICE *psDevice)
{
    /* Clear all pending events on USB attach/detach to
       handle the scenario that the time sequence of event happening
       is different from that of event handling. */

    uint32_t u32FLDET = psDevice->u32FLDET;
    
    if (u32FLDET & 1)
    {
        /* attached */
        if (psDevice->eUsbState == eDRVUSB_DETACHED)
        {
            psDevice->eUsbState = eDRVUSB_ATTACHED;
            DrvUSB_EnableUsb(psDevice);
        }

        psDevice->u16MiscEventFlags |= DRVUSB_EVENT_FLAG_ATTACH;
    }
    else
    {
        /* detached */
        psDevice->eUsbState = eDRVUSB_DETACHED;
        DrvUSB_DisableUsb(psDevice);

        psDevice->u16MiscEventFlags |= DRVUSB_EVENT_FLAG_DETACH;
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_PreDispatchBusEvent                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      psDevice    -[in]  USB driver device pointer                                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Pre dispatch BUS event                                                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_PreDispatchBusEvent(S_DRVUSB_DEVICE *psDevice)
{
    uint32_t u32Attr = psDevice->u32ATTR;

    if (psDevice->eUsbState == eDRVUSB_DETACHED)
    {
        /* There should be something wrong */
        return;
    }

    if (u32Attr & ATTR_USBRST)
    {
        /* Bus reset */
        _DRVUSB_ENABLE_USB(); /* enable USB & PHY */
        psDevice->eUsbState = eDRVUSB_DEFAULT;
        psDevice->u16MiscEventFlags |= DRVUSB_EVENT_FLAG_BUS_RESET;
    }
    else if (u32Attr & ATTR_SUSPEND)
    {
        /* Bus suspend */
        _DRVUSB_DISABLE_PHY(); /* disable PHY */
        if (psDevice->eUsbState >= eDRVUSB_ATTACHED)
        {
            psDevice->eUsbState |= eDRVUSB_SUSPENDED;
        }
        psDevice->u16MiscEventFlags |= DRVUSB_EVENT_FLAG_BUS_SUSPEND;

    }
    else if (u32Attr & ATTR_RESUME)
    {
        /* Bus resume */
        _DRVUSB_ENABLE_USB(); /* enable PHY */
        if (psDevice->eUsbState >= eDRVUSB_ATTACHED)
        {
            psDevice->eUsbState &= ~eDRVUSB_SUSPENDED;
        }
        psDevice->u16MiscEventFlags |= DRVUSB_EVENT_FLAG_BUS_RESUME;
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_PreDispatchEPEvent                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      psDevice    -[in]  USB driver device pointer                                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Pre dispatch EP event                                                                              */
/*      EP event include  IN ACK/IN NAK/OUT ACK/ISO end,                                                   */
/*      EP event'handler is g_sUsbOps[]                                                                    */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_PreDispatchEPEvent(S_DRVUSB_DEVICE *psDevice)
{
    uint32_t u32EpNum;
    uint32_t u32EpId;
    uint32_t u32INTSTS = psDevice->u32INTSTS;
    uint32_t u32EPSTS = psDevice->u32EPSTS;
    uint32_t u32PacketType;
    uint32_t u32ISOPacketType;
    int32_t i;
        
    if (psDevice->eUsbState == eDRVUSB_DETACHED)
    {
        /* There should be something wrong. */
        return;
    }

    /* Only care EP events and Setup event */
    u32INTSTS &= (INTSTS_EPTF0 | INTSTS_EPTF1 | INTSTS_EPTF2 | INTSTS_EPTF3 | INTSTS_EPTF4 | INTSTS_EPTF5 | INTSTS_SETUP);

    if (u32INTSTS & INTSTS_SETUP)
    {
        /* Record the event to procee it later */
        psDevice->u16MiscEventFlags |= DRVUSB_EVENT_FLAG_SETUP;
        
        /* Clear setup event flag */
        _DRVUSB_SET_EVENT_FLAG(INTSTS_SETUP);
        
        /* Assign the toggle bit for data phase */
        psDevice->abData0[0] = 1;
    }
    
    u32INTSTS = inp32(&USBD->INTSTS); 
    outp32(&USBD->INTSTS, (u32INTSTS & 0x00FF0000UL));

    for (i=16;i<16+MAX_EP_ID;i++)
    {
        if (u32INTSTS & (1 << i))
        {
            u32EpId = i - 16;

            /* Get the address of EP */
            u32EpNum = (_DRVUSB_GET_CFG(u32EpId) & CFG_EPT);    
            
            u32PacketType = ((u32EPSTS >> (4 + u32EpId * 3)) & EPSTS_STS);
            
            if (u32PacketType == EPSTS_IN_ACK)
            {
                /* Set this EP event as non-handled. */
                psDevice->u16EPEventFlags |= (1 << (u32EpNum*2));
            }
            else if(u32PacketType == EPSTS_OUT0_ACK)
            {
                /* Set this EP event as non-handled. */
                psDevice->u16EPEventFlags |= (1 << (u32EpNum*2 + 1));
                
                /* Assign the toggle bit of data packet */
                psDevice->abData0[u32EpId] = 1;
            }
            else if(u32PacketType == EPSTS_OUT1_ACK)
            {
                /* Set this EP event as non-handled. */
                psDevice->u16EPEventFlags |= (1 << (u32EpNum*2 + 1));
                
                /* Assign the toggle bit of data packet */
                psDevice->abData0[u32EpId] = 0;
            }
            else if(u32PacketType == EPSTS_ISO)
            {
                u32ISOPacketType = (_DRVUSB_GET_CFG(u32EpId) & CFG_STATE);
                
                psDevice->abData0[u32EpId] = 0;
                
                if (u32ISOPacketType == 0x20)
                {
                    /* This is isochronous out translation end. Call relative handler */
                    if(g_sUsbOps[u32EpNum*2 + 1].apfnCallback != 0)
                    {
                        g_sUsbOps[u32EpNum*2 + 1].apfnCallback(g_sUsbOps[u32EpNum*2 + 1].apCallbackArgu);
                    }
                }
                else if(u32ISOPacketType == 0x40)
                {
                    /* This is isochronous in translation end. Call relative handler */
                    if (g_sUsbOps[u32EpNum*2].apfnCallback != 0)
                    {
                        g_sUsbOps[u32EpNum*2].apfnCallback(g_sUsbOps[u32EpNum*2].apCallbackArgu);
                    }
                }
            }       
        }
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_DispatchWakeupEvent                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      psDevice    -[in]  USB driver device pointer                                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      None                                                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_DispatchWakeupEvent(S_DRVUSB_DEVICE *psDevice)
{

}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_DispatchMiscEvent                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      psDevice    -[in]  USB driver device pointer                                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Dispatch Misc event, the event is set by pre dispatch                                              */
/*      Misc event include  attach/detach/bus reset/bus suspend/                                           */
/*      bus resume and setup ACK, Misc event'handler is g_sBusOps[]                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_DispatchMiscEvent(S_DRVUSB_DEVICE *psDevice)
{
    uint16_t u16MiscEventFlags;
    int32_t i;
    
    u16MiscEventFlags = psDevice->u16MiscEventFlags;
    for (i=0;i<6;i++)
    {
        if (u16MiscEventFlags & (1 << i))
        {           
            psDevice->u16MiscEventFlags &= ~(1 << i);
        
            if (g_sBusOps[i].apfnCallback != 0)
            {
                g_sBusOps[i].apfnCallback(g_sBusOps[i].apCallbackArgu);
            }
        }
    }   
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_DispatchEPEvent                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      psDevice    -[in]  USB driver device pointer                                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Dispatch EP event, the event is set by pre dispatch                                                */
/*      EP event include  IN ACK/IN NAK/OUT ACK/ISO end,                                                   */
/*      EP event'handler is g_sUsbOps[]                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_DispatchEPEvent(S_DRVUSB_DEVICE *psDevice)
{
    int32_t i;
    
    for (i=0;i<12;i++)
    {
        if ((psDevice->u16EPEventFlags >> i)&1)
        {
            psDevice->u16EPEventFlags &= ~(1 << i);
            /* Call the corresponding event handler. */
            if (g_sUsbOps[i].apfnCallback != 0)
            {
                g_sUsbOps[i].apfnCallback(g_sUsbOps[i].apCallbackArgu);
            }       
        }
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_Init                                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      0           Success                                                                                */
/*      Otherwise   error                                                                                  */
/*                                                                                                         */
/* Description:                                                                                            */
/*      USB init to enable USB clock and reset USB                                                         */
/*---------------------------------------------------------------------------------------------------------*/
static int32_t DrvUSB_Init(void)
{
    uint32_t temp;
    volatile int32_t delay;
 
    /* Enable USB Clock */
    SYSCLK->APBCLK.USBD_EN = 1;

    /* Reset USB */
    SYS->IPRSTC2.USBD_RST = 1;
    delay = 500;
    while(delay--);
    SYS->IPRSTC2.USBD_RST = 0;

    _DRVUSB_ENABLE_USB();
    _DRVUSB_ENABLE_SE0();   
    temp = 0x100;
    while(temp--);   
    _DRVUSB_DISABLE_SE0();
    
    return 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_UnInit                                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Disable USB and its clock                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
static void DrvUSB_UnInit(void)
{
    /* Disable USB */
    _DRVUSB_DISABLE_USB();
    
    /* Disable the USB clock */
    SYSCLK->APBCLK.USBD_EN = 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_CtrlSetupSetAddress                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in], parameter passed by DrvUSB_InstallCtrlHandler                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Setup ACK handler for set address command                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_CtrlSetupSetAddress(void * pVoid)
{
    E_DRVUSB_STATE eUsbState;
    S_DRVUSB_DEVICE *pInfraDevice = &gsUsbDevice;   

    eUsbState = DrvUSB_GetUsbState();
    if (eUsbState == eDRVUSB_DEFAULT)
    {
        pInfraDevice->u8UsbAddress = pInfraDevice->au8Setup[2];
        _DRVUSB_SET_EP_TOG_BIT(0, 0);
        _DRVUSB_TRIG_EP(0, 0);

        DrvUSB_SetUsbState(eDRVUSB_ADDRESS);

    }
    else
    {
        DrvUSB_ClrCtrlReadyAndTrigStall();
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_CtrlSetupClearSetFeature                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in], parameter passed by DrvUSB_InstallCtrlHandler                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Setup ACK handler for clear feature command                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_CtrlSetupClearSetFeature(void * pVoid)
{
    S_DRVUSB_DEVICE *pInfraDevice = &gsUsbDevice;
    uint32_t u32EpId;
    
    /* Device */
    if ((pInfraDevice->au8Setup[0] == 0x00) && (pInfraDevice->au8Setup[2] == FEATURE_DEVICE_REMOTE_WAKEUP))
    {
        if(pInfraDevice->au8Setup[1] == SET_FEATURE)
            DrvUSB_EnableRemoteWakeup();
        if(pInfraDevice->au8Setup[1] == CLEAR_FEATURE)
            DrvUSB_DisableRemoteWakeup();
    }
    /* Interface */
    else if (pInfraDevice->au8Setup[0] == 0x01);
    
    /* Endpoint */
    else if ((pInfraDevice->au8Setup[0] == 0x02) && (pInfraDevice->au8Setup[2] == FEATURE_ENDPOINT_HALT))
    {
        u32EpId = DrvUSB_GetEpId(pInfraDevice->au8Setup[4]);
        
        if (u32EpId < MAX_EP_ID)
        {
            _DRVUSB_CLEAR_EP_DSQ_SYNC(u32EpId);
            if (pInfraDevice->au8Setup[1] == CLEAR_FEATURE)
            {
                _DRVUSB_CLEAR_EP_STALL(u32EpId);
            }
            else
            {
                _DRVUSB_TRIG_EP_STALL(u32EpId);
            }
        }
        else
        {
            DrvUSB_ClrCtrlReadyAndTrigStall();
        }
    }
    else
    {
        DrvUSB_ClrCtrlReadyAndTrigStall();
    }

    DrvUSB_DataIn(0, NULL, 0);  
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_CtrlSetupGetConfiguration                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in], parameter passed by DrvUSB_InstallCtrlHandler                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Setup ACK handler for Get configuration command                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_CtrlSetupGetConfiguration(void * pVoid)
{
    DrvUSB_DataIn(0, &gsUsbDevice.u8UsbConfiguration, 1);   
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_CtrlSetupGetStatus                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in], parameter passed by DrvUSB_InstallCtrlHandler                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Setup ACK handler Get status command                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_CtrlSetupGetStatus(void * pVoid)
{
    uint8_t au8Buf[4];
    S_DRVUSB_DEVICE *pInfraDevice = &gsUsbDevice;
    uint32_t u32EpId;
    
    u32EpId = DrvUSB_GetEpId(pInfraDevice->au8Setup[4]);

    if (pInfraDevice->au8Setup[0] == 0x80)
    {
        au8Buf[0] = pInfraDevice->bRemoteWakeup << 1 | pInfraDevice->bSelfPowered;
    }
    else if (pInfraDevice->au8Setup[0] == 0x81)
    {
        au8Buf[0] = 0x00;
    }
    else if (pInfraDevice->au8Setup[0] == 0x82)
    {
        if (u32EpId < MAX_EP_ID)
        {
            /* Interrupt-In Endpoint */
            au8Buf[0] = (_DRVUSB_GET_CFGP(u32EpId) & CFGP_SSTALL) ? 1 : 0;
        }
        else
        {
            DrvUSB_ClrCtrlReadyAndTrigStall();
            return;
        }
    }
    else
    {
        DrvUSB_ClrCtrlReadyAndTrigStall();
        return;
    }
    au8Buf[1] = 0x00;

    DrvUSB_DataIn(0, au8Buf, 2);    
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_CtrlSetupGetInterface                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in], parameter passed by DrvUSB_InstallCtrlHandler                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Setup ACK handler for Get interface command                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_CtrlSetupGetInterface(void * pVoid)
{
    const uint8_t c = 0x00;

    DrvUSB_DataIn(0, &c, 1);    
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_CtrlSetupSetInterface                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in], parameter passed by DrvUSB_InstallCtrlHandler                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Setup ACK handler for Set interface command                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_CtrlSetupSetInterface(void * pVoid)
{
    S_DRVUSB_DEVICE *pDrvDevice = &gsUsbDevice;
    S_DRVUSB_CLASS *psUsbClass = pDrvDevice->psUsbClass;
    
    if(psUsbClass->pfnStart)
        psUsbClass->pfnStart(pVoid);

    DrvUSB_DataIn(0, NULL, 0);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_CtrlSetupSetConfiguration                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in], parameter passed by DrvUSB_InstallCtrlHandler                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Setup ACK handler for Set configuration command                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_CtrlSetupSetConfiguration(void * pVoid)
{
    S_DRVUSB_DEVICE *pDrvDevice = &gsUsbDevice;
    S_DRVUSB_CLASS *psUsbClass = pDrvDevice->psUsbClass;
    int32_t bIsDeviceConfigure;
    
    bIsDeviceConfigure = psUsbClass->pfnCompare ? psUsbClass->pfnCompare(pDrvDevice->au8Setup[2]) : 1;
    
    if (pDrvDevice->au8Setup[2] == 0)
    {
        /* USB address state */
        DrvUSB_SetUsbState(eDRVUSB_ADDRESS);
        pDrvDevice->u8UsbConfiguration = pDrvDevice->au8Setup[2];
        DrvUSB_DataIn(0, NULL, 0);
    }
    else if (bIsDeviceConfigure)
    {
        /* USB configured state */
        DrvUSB_SetUsbState(eDRVUSB_CONFIGURED);

        /* Call USB class's start function */
        if(psUsbClass->pfnStart)
            psUsbClass->pfnStart(pVoid);

        pDrvDevice->u8UsbConfiguration = pDrvDevice->au8Setup[2];

        DrvUSB_DataIn(0, NULL, 0);
    }
    else
    {
        /* Not support. Reply STALL */
        DrvUSB_ClrCtrlReadyAndTrigStall();
    }   
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_CtrlDataInSetAddress                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in], parameter passed by DrvUSB_InstallCtrlHandler                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      Setup ACK handler for Set address command                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_CtrlDataInSetAddress(void * pVoid)
{
    S_DRVUSB_DEVICE *pInfraDevice = &gsUsbDevice;

    if (pInfraDevice->u8UsbAddress == 0x00)
    {
        DrvUSB_SetUsbState(eDRVUSB_DEFAULT);
    }
    else
    {
        DrvUSB_SetUsbState(eDRVUSB_ADDRESS);
        _DRVUSB_SET_FADDR(pInfraDevice->u8UsbAddress);
    }   
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvUSB_memcpy                                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pi8Dest    -[in]  Destination pointer                                                              */
/*      pi8Src     -[in]  Source pointer                                                                   */
/*      u32Size    -[in]  Data size. The unit is byte.                                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      The USB buffer is recommended to be byte access                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUSB_memcpy(uint8_t *pi8Dest, uint8_t *pi8Src, uint32_t u32Size)
{
    int32_t i;
    
    for(i=0;i<u32Size;i++)
    {
        pi8Dest[i] = pi8Src[i];
    }

}
