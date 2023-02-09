#ifndef __DEFINE_H
#define __DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#define SPI_BUF_SIZE    0x3ff

typedef struct 
{
	uint16_t head;
	uint16_t tail;
	uint16_t err_stat;
	uint8_t  buf[SPI_BUF_SIZE + 1];
} SPI_BUF, *SPI_BUF_ID;

#define NEXT_HEAD(rng_buf)    ((rng_buf.head + 1) & SPI_BUF_SIZE)
#define NEXT_TAIL(rng_buf)    ((rng_buf.tail + 1) & SPI_BUF_SIZE)
#define BUF_LEN(rng_buf)	  ((rng_buf.tail - rng_buf.head) & SPI_BUF_SIZE)

#define IDLE_CHAR_1           (0x00)
#define IDLE_CHAR_2           (0xFF)
#define ES_CHAR               (0xDB)


#ifdef __cplusplus
}
#endif

#endif 
