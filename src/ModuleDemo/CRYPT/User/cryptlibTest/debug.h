#ifndef __DEBUG_H
#define __DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
    

//#define USE_DBG_PRINTF              1
//#define USE_RESULT_PRINTF           1

//#if USE_DBG_PRINTF
//    #define DBG_PRINT(format, args...)     printf(format, ##args)
//    #define mh_printf(format, args...)      printf(format, ##args)
//#else
//    #define DBG_PRINT(format, args...)
//    #define mh_printf(format, args...)
//#endif


void r_printf(uint32_t b, char *s);
void ouputRes(char *pcFmt, void *pvbuff, uint32_t u32Len);
void ouputArrayHex(char *pcFmt, void *pvbuff, uint32_t u32Len);
void timer_Verify(void);


typedef struct
{
    uint32_t u32Ms;
    uint32_t u32Us;
    
} TIME_MS_US;


void timer_init_ms(uint32_t ms);
uint32_t get_time(TIME_MS_US *pTime);
void time_avg(TIME_MS_US *pTimeTotal, uint32_t u32Count, TIME_MS_US *pTimeAvg);
void time_add(TIME_MS_US *pTime1, TIME_MS_US *pTime2, TIME_MS_US *pTimeSum);

#ifdef __cplusplus
}
#endif  

#endif
