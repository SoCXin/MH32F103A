
#ifndef __MH_SMRAND_H
#define __MH_SMRAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mh_crypt.h"

#define MH_RET_SMRAND_INIT                       (0x5D6c39AA)
#define MH_RET_SMRAND_PARAM_ERR                  (MH_RET_SMRAND_INIT + 1)
#define MH_RET_SMRAND_CHECK_FAILURE              (MH_RET_SMRAND_INIT + 2)
#define MH_RET_SMRAND_CHECK_SUCCESS              (MH_RET_SMRAND_INIT + 0X0100)

uint32_t Rand_Check(uint8_t *pu8Src, uint32_t u32Len, uint32_t u32Mask);

#ifdef __cplusplus
}
#endif

#endif /*  __MH_SMRAND_H  */
