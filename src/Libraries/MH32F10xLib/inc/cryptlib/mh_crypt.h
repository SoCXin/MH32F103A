#ifndef __MH_CRYPT_H
#define __MH_CRYPT_H

#ifdef __cplusplus
 extern "C" {
#endif
     
#include <stdint.h>
#include <string.h>
#include "mh32f10x.h"

void MHCRYPT_SYMCITClear(void);

void MHCRYPT_ASYMCITClear(void);

void MHCRYPT_SetBusKey(uint32_t pu32Key[2]);

void MHCRYPT_CramInit(void);

void MHCRYPT_CramKeySet(uint32_t pu32Key[2]);

void MHCRYPT_CramKeyReset(void);

void MHCRYPT_SecureEnable(void);

void MHCRYPT_SecureDisable(void);

typedef struct
{
    uint8_t *pu8In;
    uint8_t *pu8Out;
    uint8_t *pu8IV;
    uint8_t *pu8IVout;
    uint8_t *pu8Key;
    uint32_t u32InLen;
    uint32_t u32OutLen;
    uint16_t u16Opt;
    uint32_t u32Crc;
} MH_SYM_CRYPT_CALL;

#ifdef __cplusplus
}
#endif

#endif // __MH_CRYPT_H
