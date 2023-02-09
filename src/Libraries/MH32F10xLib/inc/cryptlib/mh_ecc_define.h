#ifndef __MH_ECC_DEFINE_H
#define __MH_ECC_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mh_bignum.h"

#define     MH_ECC192_SPACE_LEN     (156)
#define     MH_ECC224_SPACE_LEN     (168)
#define     MH_ECC256_SPACE_LEN     (180)
#define     MH_ECC384_SPACE_LEN     (228)
#define     MH_ECC521_SPACE_LEN     (282)

typedef struct
{
    uint16_t u16BitLen;			  
    uint8_t *pu8A;                
    uint8_t *pu8B;                                           
    uint8_t *pu8Gx;               
    uint8_t *pu8Gy;                                               
    uint8_t *pu8N;                                              
    uint8_t *pu8P;               
    uint8_t *pu8Space;            //for inner space
    uint32_t u32SpaceLen;         //inner space length
    uint32_t u32Crc;                            
} MHECC_PARAM;   
    
  
typedef struct
{
    uint16_t u16Bits;
    uint8_t *pu8PubX;
    uint8_t *pu8PubY;   
    uint8_t *pu8D;
    uint32_t u32Crc;
} MHECC_KEY;

typedef struct
{
    uint8_t *pu8X;
    uint8_t *pu8Y;
}MHECC_POINT;

    
#define MH_RET_ECC_INIT                      (0x53c2d9AA)      
#define MH_RET_ECC_PARAM_ERR                 (MH_RET_ECC_INIT + 0x01)
#define MH_RET_ECC_TRNG_ERR                  (MH_RET_ECC_INIT + 0x02)
#define MH_RET_ECC_VERIFY_KEY_FAILURE        (MH_RET_ECC_INIT + 0x03)
#define MH_RET_ECC_VERIFY_CURVE_FAILURE      (MH_RET_ECC_INIT + 0x04)
#define MH_RET_ECC_VERIFY_POINT_FAILURE      (MH_RET_ECC_INIT + 0x05)
#define MH_RET_ECC_INFI_FAR_VERIFY_FAILURE   (MH_RET_ECC_INIT + 0x06)
#define MH_RET_ECC_VERIFY_SIGN_FAILURE       (MH_RET_ECC_INIT + 0x07)
#define MH_RET_ECC_FAILURE                   (MH_RET_ECC_INIT + 0x08)
#define MH_RET_ECC_ERR                       (MH_RET_ECC_INIT + 0x09)
#define MH_RET_ECC_DFA_LOCK                  (MH_RET_ECC_INIT + 0x0A)
#define MH_RET_ECC_SHA_PARAM_ERR             (MH_RET_ECC_INIT + 0x0C)
#define MH_RET_ECC_SHA_BUSY                  (MH_RET_ECC_INIT + 0x0D)
#define MH_RET_ECC_POINT_INIT                (MH_RET_ECC_INIT + 0x10)
#define MH_RET_ECC_PONIT_RANG_CHECK_FAILURE  (MH_RET_ECC_INIT + 0x11) 
#define MH_RET_ECC_POINT_SUCCESS             (MH_RET_ECC_INIT + 0x200)  
#define MH_RET_ECC_VERIFY_SUCCESS            (MH_RET_ECC_INIT + 0x300)
#define MH_RET_ECC_SUCCESS                   (MH_RET_ECC_INIT + 0x0100)

#ifdef __cplusplus
}
#endif

#endif
