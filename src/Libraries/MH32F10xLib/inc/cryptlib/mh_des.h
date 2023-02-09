/* des.h - header file for des.c
 */

#ifndef __MH_DES_H
#define __MH_DES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mh_crypt.h"


#define MH_RET_DES_INIT                         (0x55D659AA)
#define MH_RET_DES_PARAM_ERR                    (MH_RET_DES_INIT + 0x1)
#define MH_RET_DES_TRNG_ERR                     (MH_RET_DES_INIT + 0x2)
#define MH_RET_DES_PACK_MODE_ERROR              (MH_RET_DES_INIT + 0x3)
#define MH_RET_DES_INPUT_SIZE_ERROR             (MH_RET_DES_INIT + 0x4)
#define MH_RET_DES_MODE_ERROR                   (MH_RET_DES_INIT + 0x5)
#define MH_RET_DES_ATTACKED                     (MH_RET_DES_INIT + 0x6)
#define MH_RET_DES_OUTBUF_TOO_SMALL             (MH_RET_DES_INIT + 0x7)
#define MH_RET_DES_DFA_LOCK                     (MH_RET_DES_INIT + 0x8)
#define MH_RET_DES_KEY_SIZE_ERROR               (MH_RET_DES_INIT + 0x9)
#define MH_RET_DES_SUCCESS                      (MH_RET_DES_INIT + 0x0100)



#define MH_DES_OPT_MODE_Mask        (BIT(0))         
#define MH_DES_OPT_MODE_ENCRYPT     (0)
#define MH_DES_OPT_MODE_DECRYPT     (BIT(0))
#define MH_DES_OPT_BLK_Mask         (BIT(1)|BIT(2))
#define MH_DES_OPT_BLK_CBC          (BIT(1))
#define MH_DES_OPT_BLK_ECB          (BIT(2))
#define MH_DES_OPT_BLK_CBC_MAC      (BIT(1)|BIT(2))

#define MH_TDES_OPT_KEY_LEN_Mask    (BIT(3))
#define MH_TDES_OPT_KEY_2           (0)
#define MH_TDES_OPT_KEY_3           (BIT(3))

uint32_t MHDES_EncDec(MH_SYM_CRYPT_CALL *pCall);

uint32_t MHTDES_EncDec(MH_SYM_CRYPT_CALL *pCall);

#ifdef __cplusplus
}
#endif

#endif
