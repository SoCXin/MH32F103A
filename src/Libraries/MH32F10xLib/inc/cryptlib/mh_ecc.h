#ifndef __MH_ECC_H
#define __MH_ECC_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mh_bignum.h"
#include "mh_crypt.h"
#include "mh_ecc_define.h"

typedef struct
{
    uint32_t au32InBase[2];
}MH_ECC_INNER_BASE;

typedef struct
{
    //Call parameters
    uint8_t *pu8Input;
    uint8_t *pu8Output;
    uint8_t *pu8SharedMsg1;
    uint8_t *pu8SharedMsg2;
    uint16_t u16InLen;
    uint16_t u16OutLen;
    uint16_t u16M1Len;
    uint16_t u16M2Len;
    uint16_t u16MackeyLen;
    MH_ECC_INNER_BASE *pInBase;
    MHECC_PARAM *pParam;
    uint32_t u32Crc;
} MHECC_ENC_DEC_CALL;

typedef struct
{
    uint8_t *pu8SignR;
    uint8_t *pu8SignS;
}MHECC_SIGN;


typedef struct
{
    //Call parameters
    uint8_t *pu8Msg;
    uint8_t *pu8E;
    uint32_t u32MLen;
    uint16_t u16ELen;
    MHECC_SIGN *pu8Sign;
    MH_ECC_INNER_BASE *pInBase;
    MHECC_PARAM *pParam;
    uint32_t u32Crc;
} MHECC_SIG_VER_CALL;

typedef struct
{
    //Call parameters
    uint8_t *pu8KeyOut;
    MH_ECC_INNER_BASE *pInBaseU;
    MH_ECC_INNER_BASE *pInBaseV;
    MHECC_PARAM *pParam;
    uint32_t u32Crc;
} MHECC_ECDH_CALL;


uint32_t MHECC_ParamInit(MHECC_PARAM *pEccPara);

uint32_t MHECC_SetKey(MHECC_KEY *pKeySrc, MH_ECC_INNER_BASE *pInBase, uint8_t *pu8Base, MHECC_PARAM *pEccPara);

uint32_t MHECC_HashE(MHECC_SIG_VER_CALL *pCall);

uint32_t MHECC_EcdsaSign(MHECC_SIG_VER_CALL *pCall);

uint32_t MHECC_EcdsaVerify(MHECC_SIG_VER_CALL *pCall);

uint32_t MHECC_ECDH(MHECC_ECDH_CALL *pCall);

uint32_t MHECC_EciesEnc(MHECC_ENC_DEC_CALL *pCall);

uint32_t MHECC_EciesDec(MHECC_ENC_DEC_CALL *pCall);

uint32_t MHECC_GenKey(MHECC_KEY *pKey, MHECC_PARAM *pEccPara);

uint32_t MHECC_CompleteKey(MHECC_KEY *pKey,  MHECC_PARAM *pEccPara);

uint32_t MHECC_PointMul(MHECC_POINT *pPointR, MHECC_POINT *pPointA, MHBN_BIGNUM *pK, MHECC_PARAM *pParam);

uint32_t MHECC_PointAdd(MHECC_POINT *pPointR, MHECC_POINT *pPointA, MHECC_POINT *pPointB, MHECC_PARAM *pParam);

#ifdef __cplusplus
}
#endif

#endif

