/* mh_sm3.h - header file for sm3.c
 */

#ifndef __MH_SM3_H
#define __MH_SM3_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdint.h>

#define MH_RET_SM3_INIT                     (0x594F29AA)
#define MH_RET_SM3_PARAM_ERR                (MH_RET_SM3_INIT + 0x01)
#define MH_RET_SM3_BUSY                     (MH_RET_SM3_INIT + 0x02)
#define MH_RET_SM3_SUCCESS                  (MH_RET_SM3_INIT + 0x0100)  

typedef struct
{
    uint32_t    au32Total[2];
    uint8_t     au8Buffer[64];
    uint32_t    au32State[8];
} MHSM3_Context;

/**
  * @method	mh_sm3
  * @brief	SM3 Hash Check
  * @param	output		:output data buffer
  * @param	input		:input data buffer
  * @param	ibytes		:size of input data
  * @retval 			:SM3_SUCCESS or SM3_BUSY
  */
uint32_t MHSM3_Cal(uint8_t au8Output[32], uint8_t *pcu8Input, uint32_t u32Size);
uint32_t MHSM3_Starts(MHSM3_Context *pCtx);
uint32_t MHSM3_Update(MHSM3_Context *pCtx, uint8_t *pcu8Input, uint32_t u32Ilen);
uint32_t MHSM3_Finish(MHSM3_Context *pCtx, uint8_t au8Digest[32]);
    
#ifdef __cplusplus
}
#endif

#endif
