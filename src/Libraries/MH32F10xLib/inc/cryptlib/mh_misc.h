#ifndef __MH_MISC_H
#define __MH_MISC_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "mh_crypt.h"


#define MH_CMP_INIT                 (0x00AA)
#define MH_CMP_EQUAL                (0x05AA)
#define MH_CMP_BIGGER               (0xA5AA)
#define MH_CMP_SMALLER              (0x25AA)
#define MH_CMP_ERR                  (0xAAAA)
#define MH_CMP_UNEQUAL              (0x85AA)
#define MH_RET_MISC_PARAM_ERR        (0x51B649AB)


uint32_t MHMEM_SCopy(void *pvDst, void *pvSrc, uint32_t u32ByteLen, uint8_t *pu8Index);

uint32_t MHMEM_SEqual(void *pvDst, void *pvSrc, uint32_t u32ByteLen, uint8_t *pu8Index);

uint32_t MHMEM_SCopyBig(void *pvDst, void *pvSrc, uint32_t u32DstLen, uint32_t u32SrcLen, uint8_t *pu8Index);

uint32_t MHMEM_CopyBig(void  *pvDst, void  *pvSrc, uint32_t u32DstLen, uint32_t u32SrcLen);

uint32_t MHMEM_Equal(void *pvDst, void *pvSrc, uint32_t u32ByteLen);

    
#ifdef __cplusplus
}
#endif

#endif
