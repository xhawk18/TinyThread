/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef DRVPDMA_H
#define DRVPDMA_H

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include "NUC1xx.h"


#ifdef  __cplusplus
extern "C"
{
#endif


#define CHANNEL_OFFSET 0x100
/*---------------------------------------------------------------------------------------------------------*/
/* Version number                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVPDMA_MAJOR_NUM 1
#define DRVPDMA_MINOR_NUM 02
#define DRVPDMA_BUILD_NUM 002

							    
#define DRVPDMA_VERSION_NUM     _SYSINFRA_VERSION(DRVPDMA_MAJOR_NUM, DRVPDMA_MINOR_NUM, DRVPDMA_BUILD_NUM)

/*---------------------------------------------------------------------------------------------------------*/
/* Error Code define                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
// E_DRVPDMA_FALSE_INPUT		    Non-support specified parameter
// E_DRVPDMA_ERR_PORT_INVALID		Invaild port parameter
#define E_DRVPDMA_FALSE_INPUT   	_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVPDMA, 1)
#define E_DRVPDMA_ERR_PORT_INVALID	_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVPDMA, 2)

/*---------------------------------------------------------------------------------------------------------*/
/* For interrupt CallBack Function                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
typedef void (*PFN_DRVPDMA_CALLBACK)(uint32_t);

/*---------------------------------------------------------------------------------------------------------*/
/* Enumerate Type                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/

typedef enum										/* PDMA Operation Channel Seletion */
{
	eDRVPDMA_CHANNEL_0	=	0,
	eDRVPDMA_CHANNEL_1 	=	1,
	eDRVPDMA_CHANNEL_2	=	2,
	eDRVPDMA_CHANNEL_3	=	3,
	eDRVPDMA_CHANNEL_4	=	4,
	eDRVPDMA_CHANNEL_5 	=	5,
	eDRVPDMA_CHANNEL_6	=	6,
	eDRVPDMA_CHANNEL_7	=	7,
	eDRVPDMA_CHANNEL_8	=   8
	
}E_DRVPDMA_CHANNEL_INDEX;


typedef enum										/* PDMA Interrupt Flag */
{
	eDRVPDMA_TABORT_FLAG 			=1,
	eDRVPDMA_BLKD_FLAG         
}E_DRVPDMA_INT_FLAG;

typedef enum
{													/* PDMA Source/ Destination Transfer Selection structure */
	eDRVPDMA_DIRECTION_INCREMENTED 	=	0,
	eDRVPDMA_DIRECTION_FIXED      	=	2

}E_DRVPDMA_DIRECTION_SELECT;

typedef enum										/* PDMA Transfer width selection */
{
	eDRVPDMA_WIDTH_32BITS	=0,    
	eDRVPDMA_WIDTH_8BITS 	=1,
	eDRVPDMA_WIDTH_16BITS	=2
}E_DRVPDMA_TRANSFER_WIDTH;

typedef enum										/* PDMA Interrupt */
{
	eDRVPDMA_TABORT 	=1,
	eDRVPDMA_BLKD		=2
}E_DRVPDMA_INT_ENABLE;


typedef enum										/* PDMA APB Device selection */
{
	eDRVPDMA_SPI0	=	0,    
	eDRVPDMA_SPI1	=	1,  
	eDRVPDMA_SPI2	=	2,  
	eDRVPDMA_SPI3	=	3,  
	eDRVPDMA_UART0	=	4,	  
	eDRVPDMA_UART1	=	5,
	eDRVPDMA_ADC	=	7,						  
	eDRVPDMA_I2S	=	8						  
}E_DRVPDMA_APB_DEVICE;



typedef enum										/* PDMA APB Read/Write */
{
	eDRVPDMA_READ_APB	=0,    
	eDRVPDMA_WRITE_APB  =1
}E_DRVPDMA_APB_RW;


typedef enum										/* PDMA Operation Mode */
{
	eDRVPDMA_MODE_MEM2MEM	=	0,    
	eDRVPDMA_MODE_APB2MEM	=	1,    
	eDRVPDMA_MODE_MEM2APB	=	2
}E_DRVPDMA_MODE;									


typedef struct 								    	/* PDMA Source/ Destination Transfer Selection structure */
{
    uint32_t u32Addr;							   	/* Start Address */
    E_DRVPDMA_DIRECTION_SELECT eAddrDirection;		/* Transfer direction */
}STR_DRVPDMA_TRANSFER_SEL_T;


typedef struct DRVPDMA_STRUCT						/* PDMA structure */
{
    STR_DRVPDMA_TRANSFER_SEL_T 	sSrcCtrl;    	    /* Source Control */
	STR_DRVPDMA_TRANSFER_SEL_T 	sDestCtrl;		    /* Destination Control */
    E_DRVPDMA_TRANSFER_WIDTH  	u8TransWidth;		/* Transfer Width */
    E_DRVPDMA_MODE 			  	u8Mode;				/* Operation Mode */
	int32_t              	  	i32ByteCnt;			/* Byte Count */
    
}STR_PDMA_T;



/*---------------------------------------------------------------------------------------------------------*/
/* APIs declaration                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

void DrvPDMA_Init(void);
void DrvPDMA_Close(void);
void DrvPDMA_ClearIntFlag(E_DRVPDMA_CHANNEL_INDEX eChannel, E_DRVPDMA_INT_FLAG eIntFlag);

int32_t DrvPDMA_IsCHBusy(E_DRVPDMA_CHANNEL_INDEX eChannel);
int32_t DrvPDMA_Open(E_DRVPDMA_CHANNEL_INDEX sChannel, STR_PDMA_T *sParam);
int32_t DrvPDMA_IsEnabledCH(E_DRVPDMA_CHANNEL_INDEX eChannel);
int32_t DrvPDMA_GetTransferLength(E_DRVPDMA_CHANNEL_INDEX eChannel,uint32_t* pu32TransferLength);
int32_t DrvPDMA_SetAPBTransferWidth(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_TRANSFER_WIDTH eTransferWidth);
int32_t DrvPDMA_GetAPBTransferWidth(E_DRVPDMA_CHANNEL_INDEX eChannel);
int32_t DrvPDMA_GetCHForAPBDevice(E_DRVPDMA_APB_DEVICE eDevice,E_DRVPDMA_APB_RW eRWAPB);
int32_t DrvPDMA_GetWrapIntType(E_DRVPDMA_CHANNEL_INDEX eChannel);
int32_t DrvPDMA_CHSoftwareReset(E_DRVPDMA_CHANNEL_INDEX eChannel);
int32_t DrvPDMA_CHEnableTransfer(E_DRVPDMA_CHANNEL_INDEX eChannel);
int32_t DrvPDMA_EnableInt(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_INT_ENABLE eIntSource);
int32_t DrvPDMA_DisableInt(E_DRVPDMA_CHANNEL_INDEX eChannel, E_DRVPDMA_INT_ENABLE eIntSource);
int32_t DrvPDMA_IsIntEnabled(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_INT_ENABLE eIntSource);
int32_t DrvPDMA_PollInt(E_DRVPDMA_CHANNEL_INDEX eChannel,  E_DRVPDMA_INT_FLAG eIntFlag);

uint32_t DrvPDMA_GetCurrentSourceAddr(E_DRVPDMA_CHANNEL_INDEX eChannel);
uint32_t DrvPDMA_GetCurrentDestAddr(E_DRVPDMA_CHANNEL_INDEX eChannel);
uint32_t DrvPDMA_GetRemainTransferCount(E_DRVPDMA_CHANNEL_INDEX eChannel);
uint32_t DrvPDMA_GetInternalBufPointer(E_DRVPDMA_CHANNEL_INDEX eChannel);
uint32_t DrvPDMA_GetSharedBufData(E_DRVPDMA_CHANNEL_INDEX eChannel);

int32_t DrvPDMA_SetTransferSetting(
	E_DRVPDMA_CHANNEL_INDEX eChannel, 
	STR_DRVPDMA_TRANSFER_SEL_T* psSrcAddr, 
	STR_DRVPDMA_TRANSFER_SEL_T* psDestAddr, 
	uint32_t u32TransferLength
);

int32_t DrvPDMA_SetCHForAPBDevice(
    E_DRVPDMA_CHANNEL_INDEX eChannel, 
    E_DRVPDMA_APB_DEVICE eDevice,
    E_DRVPDMA_APB_RW eRWAPB    
);

int32_t DrvPDMA_InstallCallBack(
    E_DRVPDMA_CHANNEL_INDEX eChannel, 
    E_DRVPDMA_INT_ENABLE eIntSource,
	PFN_DRVPDMA_CALLBACK pfncallback
);


int32_t DrvPDMA_SetCHForAPBDevice(
    E_DRVPDMA_CHANNEL_INDEX eChannel, 
    E_DRVPDMA_APB_DEVICE 	eDevice,
    E_DRVPDMA_APB_RW 		eRWAPB    
);

int32_t	DrvPDMA_SetSourceAddress(E_DRVPDMA_CHANNEL_INDEX eChannel,uint32_t u32SourceAddr);
uint32_t DrvPDMA_GetSourceAddress(E_DRVPDMA_CHANNEL_INDEX eChannel);
int32_t	DrvPDMA_SetDestAddress(E_DRVPDMA_CHANNEL_INDEX eChannel,uint32_t u32DestAddr);
uint32_t DrvPDMA_GetDestAddress(E_DRVPDMA_CHANNEL_INDEX eChannel);

int32_t DrvPDMA_GetVersion(void);


#ifdef  __cplusplus
}
#endif

#endif	// DRVPDMA_H














