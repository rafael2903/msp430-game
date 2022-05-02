#ifndef DMA_H_
#define DMA_H_

#include <stdint.h>
#include <msp430.h>

void config_dma(int src_inc_mode);
void dma_transfer(uint8_t* source, uint8_t* destination, int size);

#endif  // DMA_H_
