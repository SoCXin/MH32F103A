#ifndef _MH_SM7_H_
#define _MH_SM7_H_

#include "mh_crypt.h"

#define MH_SM7_OPT_MODE_Mask        (BIT(0))         
#define MH_SM7_OPT_MODE_ENCRYPT     (0)
#define MH_SM7_OPT_MODE_DECRYPT     (BIT(0))          
#define MH_SM7_OPT_BLK_Mask         (BIT(1)|BIT(2))
#define MH_SM7_OPT_BLK_CBC          (BIT(1))
#define MH_SM7_OPT_BLK_ECB          (BIT(2))
#define MH_SM7_OPT_BLK_CBC_MAC      (BIT(1)|BIT(2))

#define MH_RET_SM7_INIT                         (0x5C3479AA)
#define MH_RET_SM7_PARAM_ERR                    (MH_RET_SM7_INIT + 1)
#define MH_RET_SM7_MODE_ERROR                   (MH_RET_SM7_INIT + 2)
#define MH_RET_SM7_PACK_MODE_ERROR              (MH_RET_SM7_INIT + 3)
#define MH_RET_SM7_OUTBUF_TOO_SMALL             (MH_RET_SM7_INIT + 4)
#define MH_RET_SM7_INPUT_SIZE_ERROR             (MH_RET_SM7_INIT + 5)
#define MH_RET_SM7_ATTACKED                     (MH_RET_SM7_INIT + 6)
#define MH_RET_SM7_SUCCESS                      (MH_RET_SM7_INIT + 0x0100)


uint32_t MHSM7_EncDec(MH_SYM_CRYPT_CALL *pCall);

#endif // _MH_SM7_H_
