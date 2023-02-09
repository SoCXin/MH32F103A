#ifndef __MH_BIGNUM_H
#define __MH_BIGNUM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mh_crypt.h"
 
#define MH_RET_CRYPT_INIT                       (0x508B39AA)    
#define MH_RET_CRYPT_PARAM_ERR                  (MH_RET_CRYPT_INIT+1)   
#define MH_RET_CRYPT_FUNCTION_ID_ERR            (MH_RET_CRYPT_INIT+2)
#define MH_RET_CRYPT_LENGTH_ERR                 (MH_RET_CRYPT_INIT+3)
#define MH_RET_CRYPT_GCD_ISNOT_ONE              (MH_RET_CRYPT_INIT+4)
#define MH_RET_CRYPT_SUCCESS_WITH_OVERFLOW      (MH_RET_CRYPT_INIT+5)
#define MH_RET_CRYPT_SUCCESS                    (MH_RET_CRYPT_INIT + 0x0100) 

typedef struct
{
    uint8_t *pu8Num;
    uint32_t u32Len;
}MHBN_BIGNUM;


uint32_t MHBN_BnAdd(MHBN_BIGNUM *pBigR, MHBN_BIGNUM *pBigA, MHBN_BIGNUM *pBigB);

uint32_t MHBN_BnSub(MHBN_BIGNUM *pBigR, MHBN_BIGNUM *pBigA, MHBN_BIGNUM *pBigB);

uint32_t MHBN_BnModAdd(MHBN_BIGNUM *pBigR, MHBN_BIGNUM *pBigA, MHBN_BIGNUM *pBigB, MHBN_BIGNUM *pBigP);

uint32_t MHBN_BnModSub(MHBN_BIGNUM *pBigR, MHBN_BIGNUM *pBigA, MHBN_BIGNUM *pBigB, MHBN_BIGNUM *pBigP);

uint32_t MHBN_BnMul(MHBN_BIGNUM *pBigR, MHBN_BIGNUM *pBigA, MHBN_BIGNUM *pBigB);

uint32_t MHBN_BnModMul(MHBN_BIGNUM *pBigR, MHBN_BIGNUM *pBigA, MHBN_BIGNUM *pBigB, MHBN_BIGNUM *pBigP);

uint32_t MHBN_BnDiv(MHBN_BIGNUM *pBigQ, MHBN_BIGNUM *pBigR, MHBN_BIGNUM *pBigA, MHBN_BIGNUM *pBigB);

uint32_t MHBN_BnModInv(MHBN_BIGNUM *pBigR, MHBN_BIGNUM *pBigA, MHBN_BIGNUM *pBigB);

uint32_t MHBN_BnModExp(MHBN_BIGNUM *pBigR, MHBN_BIGNUM *pBigA, MHBN_BIGNUM *pBigB, MHBN_BIGNUM *pBigP);

uint32_t MHBN_BnCmp(MHBN_BIGNUM *pBigA, MHBN_BIGNUM *pBigB);

uint32_t MHBN_GetVersion(void);

#ifdef __cplusplus
}
#endif

#endif

