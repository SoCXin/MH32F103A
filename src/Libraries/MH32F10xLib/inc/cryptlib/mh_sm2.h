#ifndef __MH_SM2_H
#define __MH_SM2_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "mh_misc.h"
#include "mh_rand.h"

#define MH_SM2_KEY_BITS 		256
#define MH_SM2_KEY_BYTES		((MH_SM2_KEY_BITS + 7) / 8)
#define MH_SM2_KEY_WORDS		((MH_SM2_KEY_BYTES + 3) / 4)

typedef struct
{
    uint8_t *pu8PubX;
    uint8_t *pu8PubY;
    uint8_t *pu8D;
    uint32_t u32Crc;
} MH_SM2_KEY;

typedef struct
{
    uint32_t au32Space[2];
}MH_SM2_INNER_BASE;

typedef struct
{
    //Call parameters
    uint8_t *pu8Input;
    uint8_t *pu8Output;
    uint32_t u32InLen;
    uint32_t u32OutLen;
    MH_SM2_INNER_BASE *pInBase;
    uint32_t u32Crc;
} MH_SM2_ENC_DEC_CALL;


typedef struct
{
    //Call parameters
    uint8_t *pu8Msg;
    uint8_t *pu8IDa;
    uint8_t *pu8E;
    uint32_t u32MLen;
    uint32_t u32IDaLen;
    uint8_t *pu8Sign;
    MH_SM2_INNER_BASE *pInBase;
    MH_SM2_KEY  *pSm2Key;
    uint32_t u32Crc;
} MH_SM2_SIG_VER_CALL;

uint32_t MHSM2_GenKey(MH_SM2_KEY *pKey);
uint32_t MHSM2_CompleteKey(MH_SM2_KEY *pKey);
uint32_t MHSM2_SetKey(MH_SM2_KEY *pKeySrc, MH_SM2_INNER_BASE *pInBase, uint8_t *pu8Base);
uint32_t MHSM2_Enc(MH_SM2_ENC_DEC_CALL *pCall);
uint32_t MHSM2_Dec(MH_SM2_ENC_DEC_CALL *pCall);
uint32_t MHSM2_HashE(MH_SM2_SIG_VER_CALL *pCall);
uint32_t MHSM2_SignE(MH_SM2_SIG_VER_CALL *pCall);
uint32_t MHSM2_VeriE(MH_SM2_SIG_VER_CALL *pCall);
uint32_t MHSM2_Sign(MH_SM2_SIG_VER_CALL *pCall);
uint32_t MHSM2_Veri(MH_SM2_SIG_VER_CALL *pCall);
uint32_t MHSM2_VerifyKeyPair(MH_SM2_KEY *pKey);

#define MH_RET_SM2_INIT                     (0x5A8379AA)
#define MH_RET_SM2_PARAM_ERR                (MH_RET_SM2_INIT + 0x01)
#define MH_RET_SM2_TRNG_ERR                 (MH_RET_SM2_INIT + 0x02)
#define MH_RET_SM2_FAILURE                  (MH_RET_SM2_INIT + 0x03)
#define MH_RET_SM2_VERIFY_SIGN_FAILURE      (MH_RET_SM2_INIT + 0x04)
#define MH_RET_SM2_VERIFY_KEY_FAILURE       (MH_RET_SM2_INIT + 0x05)
#define MH_RET_SM2_VERIFY_POINT_FAILURE     (MH_RET_SM2_INIT + 0x06)
#define MH_RET_SM2_DFA_LOCK                 (MH_RET_SM2_INIT + 0x07)
#define MH_RET_SM2_ERR                      (MH_RET_SM2_INIT + 0x08)
#define MH_RET_SM2_KDF_FAILURE              (MH_RET_SM2_INIT + 0x09)
#define MH_RET_SM2_SUCCESS                  (MH_RET_SM2_INIT + 0x0100)

#ifdef __cplusplus
}
#endif

#endif 

