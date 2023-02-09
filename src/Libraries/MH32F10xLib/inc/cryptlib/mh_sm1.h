#ifndef __MH_SM1_H      /* ToDo: replace '<Device>' with your device name */
#define __MH_SM1_H

#ifdef __cplusplus
    extern "C" 
    {
#endif

#include "mh_crypt.h"
#include <stdint.h>

#define MH_SM1_OPT_MODE_Mask            (BIT(0))
#define MH_SM1_OPT_MODE_ENCRYPT         (0)
#define MH_SM1_OPT_MODE_DECRYPT         (BIT(0))

#define MH_SM1_OPT_BLK_Mask             (BIT(1)|BIT(2))
#define MH_SM1_OPT_BLK_ECB              (BIT(1))
#define MH_SM1_OPT_BLK_CBC              (BIT(2))
#define MH_SM1_OPT_BLK_CBC_MAC          (BIT(1)|BIT(2))

#define MH_SM1_OPT_KEY_LEN_Mask         (BIT(3)|BIT(4))
#define MH_SM1_OPT_KEY_128              (BIT(3))
#define MH_SM1_OPT_KEY_192              (BIT(4))
#define MH_SM1_OPT_KEY_256              (BIT(3)|BIT(4))

#define MH_SM1_OPT_ROUND_Mask           (BIT(5)|BIT(6))
#define MH_SM1_OPT_ROUND_8              (0)
#define MH_SM1_OPT_ROUND_10             (BIT(5))
#define MH_SM1_OPT_ROUND_12             (BIT(6))
#define MH_SM1_OPT_ROUND_14             (BIT(5)|BIT(6))

#define MH_RET_SM1_INIT                     (0x5B1689AA)
#define MH_RET_SM1_PARAM_ERR                (MH_RET_SM1_INIT + 0x01)
#define MH_RET_SM1_TRNG_ERR                 (MH_RET_SM1_INIT + 0x02)
#define MH_RET_SM1_KEY_LEN_ERR              (MH_RET_SM1_INIT + 0x03)
#define MH_RET_SM1_ROUND_ERR                (MH_RET_SM1_INIT + 0x04)
#define MH_RET_SM1_MODE_ERROR               (MH_RET_SM1_INIT + 0x05)
#define MH_RET_SM1_BLK_ERROR                (MH_RET_SM1_INIT + 0x06)
#define MH_RET_SM1_ATTACKED                 (MH_RET_SM1_INIT + 0x07)
#define MH_RET_SM1_INPUT_SIZE_ERROR         (MH_RET_SM1_INIT + 0x08)
#define MH_RET_SM1_SUCCESS                  (MH_RET_SM1_INIT + 0x0100)  

uint32_t MHSM1_EncDec(MH_SYM_CRYPT_CALL *pCall);



#ifdef __cplusplus
    }
#endif


#endif
