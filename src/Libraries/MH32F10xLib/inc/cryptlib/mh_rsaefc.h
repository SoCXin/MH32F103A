#ifndef __MH_RSAEFC_H
#define __MH_RSAEFC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "mh_rsa.h"
    
typedef struct
{
    //Key info
    uint16_t u16NBits;      //Public key N bits
    uint16_t u16EBits;      //Public key E bits
    //Pub key
    uint8_t *pu8N;
    uint8_t *pu8E;
    uint8_t *pu8ND;
    uint32_t *pu32NC;
    uint32_t u32Crc;
} MH_RSA_PUBKEY;    
    
uint32_t MHRSA_SetPubEfc(MH_RSA_PUBKEY *pSrcPub, MH_RSA_KEY_SPACE *pKeyBase, uint8_t *pu8Base);    

uint32_t MHRSA_CrtEfc(MH_RSA_CALL *pCall);

uint32_t MHRSA_PubEfc(MH_RSA_CALL *pCall);

uint32_t MHRSA_PriEfc(MH_RSA_CALL *pCall);


#ifdef __cplusplus
}
#endif

#endif
