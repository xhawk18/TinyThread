/****************************************************************
 *                                                              *
 * Copyright (c) Nuvoton Technology Corp. All rights reserved.  *
 *                                                              *
 ****************************************************************/

#ifndef __VENDORUSB_H__
#define __VENDORUSB_H__

#ifdef  __cplusplus
extern "C"
{
#endif


extern const uint8_t gau8DeviceDescriptor[];
extern const uint8_t gau8ConfigDescriptor[];
extern const uint8_t gau8VendorStringDescriptor[];
extern const uint8_t gau8ProductStringDescriptor[];

extern const uint8_t gau8StringLang[];
extern const uint8_t gau8StringSerial[];
extern const uint8_t gau8StringAudio[];

int32_t UVC_MainProcess(void);

#ifdef  __cplusplus
}
#endif

#endif // #ifndef __VENDORUSB_H__



