/******************************************************************************
 * @file     nano1xx_smartcard.h
 * @brief    NANO1xx SMARTCARD driver header file
 * @version  1.0.2
 * @date     02, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __NANO1XX_SC_H
#define __NANO1XX_SC_H


#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "nano1xx.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_SMARTCARD_Driver SMARTCARD Driver
  @{
*/


/** @addtogroup NANO1xx_SMARTCARD_EXPORTED_CONSTANTS SMARTCARD Exported Constants
  @{
*/

/* Do activation & de-activation & warm-rest with manual sequence */
#define SC_SEQ_MANUAL

/********************************************/


/** Define TIMER of SC */
typedef enum {
        SC_TIMER0  = 8,
        SC_TIMER1  = 16,
        SC_TIMER2  = 32
} E_SC_TIMER;

/** Clock Stop Level */
typedef enum {
        CLOCKSTOP_NOT_SUPPORT   = 0,
        CLOCKSTOP_LOW           = 1,
        CLOCKSTOP_HIGH          = 2,
        CLOCKSTOP_NO_PREFER     = 3
} E_CLOCK_STOP;


/** This union is used for data type conversion */
typedef union _LENGTH {

    struct {

        unsigned long   l0;

    } l;

    struct {

        uint8_t   b0;
        uint8_t   b1;
        uint8_t   b2;
        uint8_t   b3;
    } b;

} LENGTH, *PLENGTH;

#define MAXIMUM_ATR_CODES                   4
#define MAXIMUM_ATR_LENGTH                  33


/*@}*/ /* end of group NANO1xx_SMARTCARD_EXPORTED_CONSTANTS */


/** @addtogroup NANO1xx_SMARTCARD_EXPORTED_STRUCTS SMARTCARD Exported Structs
  @{
*/

typedef struct _T0_DATA {
    int32_t   Lc;	  ///< Number of data bytes in this request
    int32_t   Le;	  ///< Number of expected bytes from the card

} T0_DATA, *PT0_DATA;


typedef struct _T1_DATA {

    uint8_t   IFSC;	  ///< Current information field size that can be transmitted
    uint8_t   IFSD;	  ///< Current information field size we can receive
    uint8_t   IBLOCK_REC; ///< Record if received I-block was sent correctly from ICC
    uint8_t   RSN;	  ///< The 'number' of received I-Blocks
    uint8_t   SSN;	  ///< The 'number' of sent I-Blocks as defined in ISO 7816-3
    uint8_t   Wtx;   ///< Waiting time extension requested by the smart card, This value should be used by the driver to extend block waiting time

} T1_DATA, *PT1_DATA;


/** This struct is used by the lib for T1 I/O */
typedef struct _T1_BLOCK_FRAME {

    uint8_t   Nad;
    uint8_t   Pcb;
    uint8_t   Len;
    uint8_t *  Inf;

} T1_BLOCK_FRAME, *PT1_BLOCK_FRAME;

/** Clock rate conversion table according to ISO */
typedef struct _CLOCK_RATE_CONVERSION {

    const unsigned long F;
    const unsigned long fs;

} CLOCK_RATE_CONVERSION, *PCLOCK_RATE_CONVERSION;


/**
 * Bit rate adjustment factor
 * The layout of this table has been slightly modified due to
 * the unavailibility of floating point math support in the kernel.
 * The value D has beed devided into a numerator and a divisor.
 */
typedef struct _BIT_RATE_ADJUSTMENT {

    const unsigned long DNumerator;
    const unsigned long DDivisor;

} BIT_RATE_ADJUSTMENT, *PBIT_RATE_ADJUSTMENT;



/// @cond

/**
 * The clock rate conversion table itself.
 * All R(eserved)F(or Future)U(se) fields MUST be 0
 */
static CLOCK_RATE_CONVERSION ClockRateConversion[] = {

        { 372,  4000000     },
        { 372,  5000000     },
        { 558,  6000000     },
        { 744,  8000000     },
        { 1116, 12000000    },
        { 1488, 16000000    },
        { 1860, 20000000    },
        { 0,    0            },
        { 0,    0            },
        { 512,  5000000     },
        { 768,  7500000     },
        { 1024, 10000000    },
        { 1536, 15000000    },
        { 2048, 20000000    },
        { 0,    0            },
        { 0,    0            }
};

/**
 * The bit rate adjustment table itself.
 * All R(eserved)F(or)U(se) fields MUST be 0
 */
static BIT_RATE_ADJUSTMENT BitRateAdjustment[] = {

    { 0,    0   },
    { 1,    1   },
    { 2,    1   },
    { 4,    1   },
    { 8,    1   },
    { 16,   1   },
    { 32,   1   },
    { 64,   1   },
    { 12,   1   },
    { 20,   1   },
    { 0,    0   },
    { 0,    0   },
    { 0,    0   },
    { 0,    0   },
    { 0,    0   },
    { 0,    0   }
};

/// @endcond


/**
 * This struct holds information for the card currently in use
 * The driver must store a received ATR into the ATR struct which is
 * part of this struct. The lib will get all other information
 * out of the ATR.
 */
typedef struct _SCARD_CARD_CAPABILITIES{


    bool InversConvention;	///< Flag that indicates that the current card uses invers convention
    unsigned long   etu;        ///< Calculated etu in micro-second unit ; etu = F/D * 1/f

    /**
     * Answer To Reset string returned by card.
     * Use OsData->SpinLock to access this member
     */
    struct {

        uint8_t Buffer[64];
        uint8_t Length;

    } ATR;

    struct {

        uint8_t Buffer[16];
        uint8_t Length;

    } HistoricalChars;

    // !!! DO NOT MODIFY ANY OF THE BELOW VALUES
    // OTHERWISE THE LIBRARY WON'T WORK PROPERLY


    uint8_t Fl;	       ///< Clock rate conversion
    uint8_t Dl;	       ///< Bit rate adjustment
    uint8_t II;	       ///< Maximum programming current
    uint8_t P;	       ///< Programming voltage in .1 Volts
    uint8_t N;	       ///< Extra guard time in etu

    unsigned long GT;  ///< Calculated guard time in micro seconds

    struct {
                unsigned long Supported;	///< This is a bit mask of the supported protocols
                unsigned long Selected;		///< The currently selected protocol

    } Protocol;

    /** T=0 specific data */
    struct {
        uint8_t WI;	   ///< Waiting integer
        unsigned long WT;  ///< Waiting time in micro seconds

    } T0;

    /** T=1 specific data */
    struct {

        uint8_t IFSC;	 ///< Information field size of card

        uint8_t CWI;	 ///< Character waiting integer
        uint8_t BWI;	 ///< Block waiting integer

        uint8_t EDC;	 ///< Error detection code

        unsigned long CWT;   ///< Character and block waiting time in micro seconds
        unsigned long BWT;   ///< Character and block waiting time in micro seconds


        unsigned long BGT;   ///< Block guarding time in micro seconds

    } T1;

} SCARD_CARD_CAPABILITIES, *PSCARD_CARD_CAPABILITIES;


/**
  * @brief  SmartCard Data Structure definition
  */
typedef struct
{
        SC_TypeDef *base;
        volatile uint32_t op_state;             /* 0 - everything goes well, 1 - some error occured */
        /*volatile*/ int32_t errno;
        /*volatile*/ uint32_t openflag;
        /*volatile*/ uint32_t clock;                        /* frequence in KHz */
        E_CLOCK_STOP clock_stop_level;
        /*volatile*/ uint32_t etu;                  /* in millisecond */
        /*volatile*/ uint32_t voltage;
        /*volatile*/ int8_t ActivePowerPin;         /* 1: active High 0: active Low */

    /** Capabilities of the current inserted card */
    SCARD_CARD_CAPABILITIES CardCapabilities;

    /**
     * Current state of reader (card present/removed/activated)
     * Use OsData->SpinLock to access this member
     * (mandatory)
     * use for PPS
     */
    unsigned long   CurrentState;

    uint8_t *snd_buf, *rcv_buf;
    volatile int32_t snd_pos, snd_len;
    volatile int32_t rcv_len, rcv_pos;
    volatile int32_t rcv_cnt;  // cnt is current received data number in write stage read index in read stage
    volatile uint32_t bCardRemoved /* , bCardChanged */;
    int8_t pps_complete;

    T0_DATA T0;	   ///< Data for T=0
    T1_DATA T1;	   ///< Data for T=1

    uint8_t uart;
}S_SC_DEV_T;
/*@}*/ /* end of group NANO1xx_SMARTCARD_EXPORTED_STRUCTS */


/** @addtogroup NANO1xx_SMARTCARD_EXPORTED_CONSTANTS SMARTCARD Exported Constants
  @{
*/


/** @addtogroup SMARTCARD_Minimum_Buffer_Size_for_Request_and_Reply
  * @{
  */
#define MIN_BUFFER_SIZE             271// 288
/**
  * @}
  */


/** @addtogroup SMARTCARD_Return_Code_for_Parse_ATR
  * @{
  */
#define ATR_ERR_UNRECOGNIZED_MEDIA              -150
#define ATR_ERR_INVALID_PARAMETER               -149
#define ATR_ERR_INVALID_TCK                             -148
/**
  * @}
  */

#define PROTOCOL_T0_OK                                  0                       /* Command OK  */
#define PROTOCOL_T0_ICC_ERROR                   -1000           /* ICC comunication error */
#define PROTOCOL_T0_ERROR                               -1001           /* T=0 Protocol Error */

#define PROTOCOL_PPS_INVALID                    -147


/** @addtogroup SMARTCARD_Protocol_Flag_definitions
  * @{
  */
#define SCARD_PROTOCOL_UNDEFINED                0x00000000  ///< There is no active protocol.
#define SCARD_PROTOCOL_T0                       0x00000001  ///< T=0 is the active protocol.
#define SCARD_PROTOCOL_T1                       0x00000002  ///< T=1 is the active protocol.
#define SCARD_PROTOCOL_T15                      0x00008000  ///< T=15 is the active protocol.
#define SCARD_PROTOCOL_RAW                      0x00010000  ///< Raw is the active protocol.

#define SCARD_UNKNOWN                   0x0001  /*!< Unknown state */
#define SCARD_ABSENT                    0x0002  /*!< Card is absent */
#define SCARD_PRESENT                   0x0004  /*!< Card is present */
#define SCARD_SWALLOWED                 0x0008  /*!< Card not powered */
#define SCARD_POWERED                   0x0010  /*!< Card is powered */
#define SCARD_NEGOTIABLE                0x0020  /*!< Ready for PTS */
#define SCARD_SPECIFIC                  0x0040  /*!< PTS has been set */

/*****************************************************************************
*
*****************************************************************************/
#define T1_BLOCK_MAX_SIZE         259
#define T1_BLOCK_INF_MAX_SIZE     254

/* PCBs */
#define T1_BLOCK_I                0x00
#define T1_BLOCK_R                0x80
#define T1_BLOCK_S                0xC0
//
// constants for the T=1 i/o function
//
#define T1_INIT                         0
#define T1_START                        1
#define T1_I_BLOCK                      2
#define T1_R_BLOCK                      3
#define T1_RESTART                      4

#define T1_BLOCK_S_RESYNCH_REQ          0xC0
#define T1_BLOCK_S_RESYNCH_RES          0xE0
#define T1_BLOCK_S_IFS_REQ              0xC1
#define T1_BLOCK_S_IFS_RES              0xE1
#define T1_BLOCK_S_ABORT_REQ            0xC2
#define T1_BLOCK_S_ABORT_RES            0xE2
#define T1_BLOCK_S_WTX_REQ              0xC3
#define T1_BLOCK_S_WTX_RES              0xE3
#define T1_BLOCK_S_VPP_ERR              0xE4
/*****************************************************************************
*
*****************************************************************************/
#define PROTOCOL_T1_OK                          0               ///< Command OK
#define PROROCOL_T1_P_ERR_NOTICE                2000            ///< T=1 Parity Error Notice
#define PROTOCOL_T1_ICC_ERROR                   -2000           ///< ICC comunication error
#define PROTOCOL_T1_ERROR                       -2001           ///< T=1 Protocol Error
#define T1_ABORT_RECEIVED                       -2002
#define T1_RESYNCH_RECEIVED                     -2003
#define T1_VPP_ERROR_RECEIVED                   -2004
#define T1_WTXRES_RECEIVED                      -2005
#define T1_IFSRES_RECEIVED                      -2006
#define T1_ABORTRES_RECEIVED                    -2007


#define T1_IFSD             254	    ///< Information field size the lib uses
#define T1_MAX_RETRIES      2	    ///< Maximum attempts to resend a block in T1
#define T1_MORE_DATA        0x20    ///< Bit that indenticates if there are more data to send


// T1 Error values
#define T1_ERROR_CHKSUM         -1002
#define T1_ERROR_OTHER          -1003
#define T1_ERROR_LENGTH         -1004

#define T1_CRC_CHECK        1	  ///< Error detection bit as defined by ISO
#define T1_CWI_DEFAULT      13	  ///< Character waiting integer default value as definded by ISO

#define T1_BWI_DEFAULT      4	  ///< Block waiting integer default value as definded by ISO

#define ATR_TOTAL_TIME          20050	   ///< Receiving an ATR shall have a duration of less than or equal to initial etus

/**
  * @}
  */


#define SMARTCARD_ERR_ID        0xffff2000

#define MAX_ATR_LEN                                     (33)
#define MAX_BUF_LEN                                     (500)
#define MAX_CMD_LEN                                     (262)   /* header : 5, data : 256(max), le : 1, plus all 262 */

#define SMARTCARD_NUM                                       3      // we have 3 interfaces


/* iso7816 operation class */
#define SC_ISO_OPERATIONCLASS_AUTO                      (0x00)
#define SC_ISO_OPERATIONCLASS_A                         (0x01)
#define SC_ISO_OPERATIONCLASS_B                         (0x02)
#define SC_ISO_OPERATIONCLASS_C                         (0x03)

// Current card operation in ISR
#define SC_OP_NOP                                       (0x00)
#define SC_OP_ATR_READ                  (0x01)
#define SC_OP_READ                                      (0x02)
#define SC_OP_WRITE                             (0x03)

// status successful
#define SC_STATUS_SUCCESS               0

// error code
#define SC_ERR_CARD_REMOVED                     -120
#define SC_ERR_OVER_RUN                                 -119
#define SC_ERR_PARITY_ERROR                     -118
#define SC_ERR_NO_STOP                                  -117
#define SC_ERR_SILENT_BYTE                              -116
#define SC_ERR_CMD                                                  -115
#define SC_ERR_UNSUPPORTEDCARD          -114
#define SC_ERR_READ                                             -113
#define SC_ERR_WRITE                                        -112
#define SC_ERR_TIME0OUT                         -111
#define SC_ERR_TIME1OUT                         -110
#define SC_ERR_TIME2OUT                         -109
#define SC_ERR_AUTOCONVENTION                   -108
#define SC_ERR_BGTIMEOUT                                -107


// These are the error code actually returns to user application
#define SC_EIO                                                  (1| SMARTCARD_ERR_ID)
#define SC_ENODEV                                               (2| SMARTCARD_ERR_ID)
#define SC_ENOMEM                                               (3| SMARTCARD_ERR_ID)
#define SC_EBUSY                                                (4| SMARTCARD_ERR_ID)
#define SC_ENOTTY                                               (5| SMARTCARD_ERR_ID)
#define SC_EPROTOCOL                                    (6| SMARTCARD_ERR_ID)
#define SC_EPARAMETER                                   (7| SMARTCARD_ERR_ID)



/* ATRIB = A(nswer) T(o) R(eset) I(nterface) B(yte) */
#define ATR_INTERFACE_BYTE_TA           0       /* Interface byte TAi */
#define ATR_INTERFACE_BYTE_TB           1       /* Interface byte TBi */
#define ATR_INTERFACE_BYTE_TC           2       /* Interface byte TCi */
#define ATR_INTERFACE_BYTE_TD           3       /* Interface byte TDi */

// Reset type
#define SC_COLDRESET                (0x1)
#define SC_WARMRESET                (0x2)


#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))


#define SC_GET_ONE_BYTE()                               ((SC->RBR & 0xff))
#define SC_PUT_ONE_BYTE(data)                       (SC->THR = data)


/*---------------------------------------------------------------------------------------------------------*/
/*  Define parameter checking                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define CHECK_SC_PORT(port)                             ((port==SC0) || (port==SC1) || (port==SC2))

/*@}*/ /* end of group NANO1xx_SMARTCARD_EXPORTED_CONSTANTS */


/** @addtogroup NANO1xx_SMARTCARD_EXPORTED_STRUCTS SMARTCARD Exported Structs
  @{
*/

typedef struct
{
    uint32_t    u32BaudRate;
    uint32_t    u32CharLen;
    uint32_t    u32StopBitLen;
    uint32_t    u32Parity;
    uint32_t    u32RxTriggerLevel;
    uint16_t    u16TimeOut ;
}S_SC_UART;

/*@}*/ /* end of group NANO1xx_SMARTCARD_EXPORTED_STRUCTS */


/** @addtogroup NANO1xx_SMARTCARD_EXPORTED_CONSTANTS SMARTCARD Exported Constants
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/*  Define EMV checking                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
//#define EMV_ATR_CHECK         1               /* 1: Enable to check integrity of ATR for EMV */

//#define SC_DEBUG_ENABLE_ENTER_LEAVE
#define SC_IST_DEBUG                            /* IST information check in DrvSMARTCARD.c */
//#define SC_ATR_DEBUG                          /* ATR information check in DrvSMARTCARD.c */
#define SC_RD_DEBUG                             /* Send/Receive check in SMARTCARD_Reader.c */
#define SC_T1_DEBUG                             /* T1 protocol check in SMARTCARD_Protocol.c */

#ifdef SC_DEBUG_ENABLE_ENTER_LEAVE
#define ENTER()                                 printf("[%-20s] : Enter...\n", __func__)
#define LEAVE()                                 printf("[%-20s] : Leave...\n", __func__)
#else
#define ENTER()
#define LEAVE()
#endif


#ifdef SC_ATR_DEBUG
#define SCATRDEBUG     printf
#else
#define SCATRDEBUG(...)
#endif


#ifdef SC_IST_DEBUG
#define SCISTDEBUG     printf
#else
#define SCISTDEBUG(...)
#endif


#ifdef SC_RD_DEBUG
#define SCRDDEBUG     printf
#else
#define SCRDDEBUG(...)
#endif


#ifdef SC_T1_DEBUG
#define SCT1DEBUG     printf
#else
#define SCT1DEBUG(...)
#endif

/*@}*/ /* end of group NANO1xx_SMARTCARD_EXPORTED_CONSTANTS */


/** @addtogroup NANO1xx_SMARTCARD_EXPORTED_FUNCTIONS SMARTCARD Exported Functions
  @{
*/
//  function prototypes: SMARTCARD.c
void SMARTCARD_TimerINTEnable(S_SC_DEV_T *dev, E_SC_TIMER no);
void SMARTCARD_TimerINTDisable(S_SC_DEV_T *dev, E_SC_TIMER no);
void SMARTCARD_TimerCountSet(S_SC_DEV_T *dev, E_SC_TIMER no, uint32_t op_mode, uint32_t cnt);
void SMARTCARD_WaitFirstReceivedData(S_SC_DEV_T *dev, E_SC_TIMER no, uint32_t op_mode, uint32_t cnt);
void SMARTCARD_TimerStop(S_SC_DEV_T *dev, E_SC_TIMER no);
void SMARTCARD_TimerAllStop(S_SC_DEV_T *dev);
void SMARTCARD_TimerStart(S_SC_DEV_T *dev, E_SC_TIMER no);
void SMARTCARD_TimerSelect(S_SC_DEV_T *dev, uint32_t type);
int32_t SMARTCARD_WarmReset(S_SC_DEV_T *dev);
int32_t SMARTCARD_DoPPS(S_SC_DEV_T *dev, int32_t f, int32_t d, int32_t t);
void SMARTCARD_DeactivationCmd(S_SC_DEV_T *dev);
void SMARTCARD_ActivationCmd(S_SC_DEV_T *dev);
void SMARTCARD_WarmResetCmd(S_SC_DEV_T *dev);
void SMARTCARD_StartBlockGuardTime( S_SC_DEV_T *dev );
void SMARTCARD_StopBlockGuardTime( S_SC_DEV_T *dev );
void SMARTCARD_ClearFIFO(S_SC_DEV_T *dev);
uint8_t * SMARTCARD_ERRMSG(long errno);
int32_t SMARTCARD_CheckCardPresent(SC_TypeDef *portno);
int32_t SMARTCARD_ColdReset(S_SC_DEV_T *dev);
void SMARTCARD_SetClockState(S_SC_DEV_T *dev, int8_t onoff);
void SMARTCARD_VoltageConfig(S_SC_DEV_T *dev);
void SMARTCARD_SetReaderParameter(S_SC_DEV_T *dev);
void SMARTCARD_PowerActiveLevel(SC_TypeDef *portno, int8_t active);
void SMARTCARD_IgnoreCardAbsent(SC_TypeDef *portno);

//  SMARTCARD_Util.c
int32_t SmartcardUpdateCardCapabilities(S_SC_DEV_T *dev);

//  SMARTCARD_Reader.c
int32_t CBT1Transmit( S_SC_DEV_T *dev, uint8_t *buf, uint32_t len, uint8_t *outbuf, uint32_t *outlen );
int32_t CBT0Transmit( S_SC_DEV_T *dev, uint8_t *buf, uint32_t len, uint8_t *outbuf, uint32_t *outlen );
int32_t CBRawTransmit( S_SC_DEV_T *dev, uint8_t *buf, uint32_t txlen, uint32_t rxlen, uint8_t *outbuf, uint32_t *outlen );

void SMARTCARD_Init(SC_TypeDef *portno, int8_t edge);
void SMARTCARD_DeInit(SC_TypeDef *portno);
int32_t SMARTCARD_Open(SC_TypeDef *portno);
int32_t SMARTCARD_Close(SC_TypeDef *portno);
int32_t SMARTCARD_Write(SC_TypeDef *portno, uint8_t *buf, uint32_t len, uint8_t *outbuf, uint32_t *outlen );
int32_t SMARTCARD_GetATRBuffer(SC_TypeDef *portno, uint8_t *buf, int16_t length);


void SMARTCARD_UartOpen(SC_TypeDef *portno, S_SC_UART *sParam);
void SMARTCARD_UartClose(SC_TypeDef *portno);
int32_t SMARTCARD_UartRead(SC_TypeDef *portno, uint8_t *pu8RxBuf, uint32_t u32ReadBytes);
int32_t SMARTCARD_UartWrite(SC_TypeDef *portno, uint8_t *pu8TxBuf, uint32_t u32WriteBytes);
void SMARTCARD_UartEnableInt(SC_TypeDef *portno);
void SMARTCARD_UartDisableInt(SC_TypeDef *portno);


/*@}*/ /* end of group NANO1xx_SMARTCARD_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_SMARTCARD_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif          /* __NANO1XX_SC_H */


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/



