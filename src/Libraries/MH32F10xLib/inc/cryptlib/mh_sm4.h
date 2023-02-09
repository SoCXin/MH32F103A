/* sm4.h - header file for sm4.c
 */

#ifndef __MH_SM4_H
#define __MH_SM4_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdint.h>
#include "mh_crypt.h"
    
#define MH_SM4_OPT_MODE_Mask        (BIT(0))         
#define MH_SM4_OPT_MODE_ENCRYPT     (0)
#define MH_SM4_OPT_MODE_DECRYPT     (BIT(0))

#define MH_SM4_OPT_BLK_Mask         (BIT(1)|BIT(2))
#define MH_SM4_OPT_BLK_CBC          (BIT(1))
#define MH_SM4_OPT_BLK_ECB          (BIT(2))
#define MH_SM4_OPT_BLK_CBC_MAC      (BIT(1)|BIT(2))





#define MH_RET_SM4_INIT                         (0x584E69AA)
#define MH_RET_SM4_PARAM_ERR                    (MH_RET_SM4_INIT + 0x01)
#define MH_RET_SM4_TRNG_ERR                     (MH_RET_SM4_INIT + 0x02)
#define MH_RET_SM4_PACK_MODE_ERROR              (MH_RET_SM4_INIT + 0x03)
#define MH_RET_SM4_OUTBUF_TOO_SMALL             (MH_RET_SM4_INIT + 0x04)
#define MH_RET_SM4_INPUT_SIZE_ERROR             (MH_RET_SM4_INIT + 0x05)
#define MH_RET_SM4_ATTACKED                     (MH_RET_SM4_INIT + 0x06)
#define MH_RET_SM4_MODE_ERROR                   (MH_RET_SM4_INIT + 0x07)
#define MH_RET_SM4_SUCCESS                      (MH_RET_SM4_INIT + 0x0100)

uint32_t MHSM4_EncDec(MH_SYM_CRYPT_CALL *pCall);


#endif


#ifdef __cplusplus
}
#endif

