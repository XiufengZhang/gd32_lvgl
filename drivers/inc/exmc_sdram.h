/*!
    \file  exmc_sdram.h
    \brief the header file of SDRAM driver
*/

#ifndef EXMC_SDRAM_H
#define EXMC_SDRAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f4xx.h"

#define SDRAM_DEVICE0_ADDR ((uint32_t)0xC0000000)
#define SDRAM_DEVICE1_ADDR ((uint32_t)0xD0000000)
#define SDRAM_DEVICE0_SIZE ((uint32_t)0x1000000)// multiple of 2 bytes
#define SDRAM_DEVICE1_SIZE ((uint32_t)0x1000000)// multiple of 2 bytes

/* sdram peripheral initialize */
ErrStatus exmc_synchronous_dynamic_ram_init(uint32_t sdram_device);

/* fill the buffer with specified value */
void fill_buffer(uint8_t *pbuffer, uint16_t buffer_lengh, uint16_t offset);

/* write a byte buffer(data is 8 bits) to the EXMC SDRAM memory */
void sdram_writebuffer_8(uint32_t sdram_device, uint8_t *pbuffer, uint32_t writeaddr, uint32_t numtowrite);

/* read a block of 8-bit data from the EXMC SDRAM memory */
void sdram_readbuffer_8(uint32_t sdram_device, uint8_t *pbuffer, uint32_t readaddr, uint32_t numtoread);

/* write a half-word buffer(data is 16 bits) to the EXMC SDRAM memory */
void sdram_writebuffer_16(uint32_t sdram_device, uint16_t *pbuffer, uint32_t writeaddr, uint32_t numtowrite);

/* read a block of 16-bit data from the EXMC SDRAM memory */
void sdram_readbuffer_16(uint32_t sdram_device, uint16_t *pbuffer, uint32_t readaddr, uint32_t numtoread);

/* set a byte data(data is 8 bits) to the EXMC SDRAM memory */
void sdram_set_8(uint32_t sdram_device, uint8_t data, uint32_t writeaddr, uint32_t numtowrite);

/* set a half-word data(data is 16 bits) to the EXMC SDRAM memory */
void sdram_set_16(uint32_t sdram_device, uint16_t data, uint32_t writeaddr, uint32_t numtowrite);

/* set 3 byte data(data is 24 bits) to the EXMC SDRAM memory */
void sdram_set_24(uint32_t sdram_device, uint32_t data, uint32_t writeaddr, uint32_t numtowrite);

/* set a word data(data is 32 bits) to the EXMC SDRAM memory */
void sdram_set_32(uint32_t sdram_device, uint32_t data, uint32_t writeaddr, uint32_t numtowrite);

/* check the EXMC SDRAM memory */
void sdram_memcheck(uint32_t sdram_device);

#ifdef __cplusplus
}
#endif

#endif /* EXMC_SDRAM_H */
