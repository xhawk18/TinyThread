/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __VCOMSYS_H__
#define __VCOMSYS_H__


#ifdef  __cplusplus
extern "C"
{
#endif


#define VCOM_MAJOR_NUM	1
#define VCOM_MINOR_NUM  0
#define VCOM_BUILD_NUM	1


// E_VIRTUALCOM_UNDEFINE	              Undefined error.
// E_VIRTUALCOM_NULL_POINTER	          NULL pointer.
// E_VIRTUALCOM_BUFFER_OVERRUN	          Allocated buffer is not enough.
// E_VIRTUALCOM_INVALID_EP_NUM	          Invalid EP number.
// E_VIRTUALCOM_SCSI_PDT_NOT_SUPPORT	  Non-support SCSI PDT.
// E_VIRTUALCOM_INVALID_LUN	              Invalid LUN.
// E_VIRTUALCOM_CTRL_REG_TAB_FULL	      Control register table is full.
// E_VIRTUALCOM_SCSI_REG_TAB_FULL	      SCSI register table is full.

#define E_VCOM_ERR_UNDEFINE         		_SYSINFRA_ERRCODE(TRUE, MODULE_ID_VCOM, 0)
#define E_VCOM_ERR_NULL_POINTER     		_SYSINFRA_ERRCODE(TRUE, MODULE_ID_VCOM, 1)
#define E_VCOM_ERR_BUFFER_OVERRUN   		_SYSINFRA_ERRCODE(TRUE, MODULE_ID_VCOM, 2)
#define E_VCOM_ERR_INVALID_EP_NUM   		_SYSINFRA_ERRCODE(TRUE, MODULE_ID_VCOM, 3)
#define E_VCOM_ERR_SCSI_PDT_NOT_SUPPORT     _SYSINFRA_ERRCODE(TRUE, MODULE_ID_VCOM, 4)
#define E_VCOM_ERR_INVALID_LUN              _SYSINFRA_ERRCODE(TRUE, MODULE_ID_VCOM, 5)
#define E_VCOM_ERR_CTRL_REG_TAB_FULL        _SYSINFRA_ERRCODE(TRUE, MODULE_ID_VCOM, 6)
#define E_VCOM_ERR_SCSI_REG_TAB_FULL        _SYSINFRA_ERRCODE(TRUE, MODULE_ID_VCOM, 7)

// Define module version number.
_SYSINFRA_VERSION_DEF(VCOM, VCOM_MAJOR_NUM, VCOM_MINOR_NUM, VCOM_BUILD_NUM)

#define	VCOM_CTRL_MXPLD  64  /* Maximum packet size of control */ 
#define	BULK_EP_MXPLD    64  /* Maximum packet size of BULK endpoint */
#define INT_EP_MXPLD     8   /* Maximum packet size of INT endpoint  */

#define SEND_ENCAPSULATED_COMMAND   0x00
#define GET_ENCAPSULATED_RESPONSE   0x01
#define SET_COMM_FEATURE            0x02
#define GET_COMM_FEATURE            0x03
#define CLEAR_COMM_FEATURE          0x04
#define SET_LINE_CODE               0x20
#define GET_LINE_CODE               0x21
#define SET_CONTROL_LINE_STATE      0x22
#define SEND_BREAK                  0x23



typedef struct
{
	const uint8_t *au8DeviceDescriptor;
	const uint8_t *au8ConfigDescriptor;
	const uint8_t *au8ProductStringDescriptor;
	const uint8_t *sVendorStringDescriptor;
	const uint8_t *sProductStringDescriptor;
	void* *device;
	
} S_VCOM_DEVICE;

extern S_VCOM_DEVICE gsVcomDevice;


uint32_t VCOM_GetVersion(void);
int32_t  VCOM_Open(void);
void VCOM_Close(void);
void VCOM_CtrlLineState(void* pVoid);
void VCOM_CtrlSetupSetConfiguration(void* pVoid);
void VCOM_VendorCmd(void* pVoid);
void VCOM_GetLineCoding(void* pVoid);
void VCOM_SetLineCodingDataOut(void* pVoid);
void VCOM_SetLineCoding(void* pVoid);
void VCOM_CtrlSetupGetDescriptor(void* pVoid);
void VCOM_BulkOutAckCallback(void* pVoid);
void VCOM_BulkInAckCallback(void* pVoid);
void VCOM_IntInAckCallback(void* pVoid);
void VCOM_CtrlDataInGetDescriptor(void* pVoid);

#ifdef  __cplusplus
}
#endif

#endif // __VCOMSYS_H__


