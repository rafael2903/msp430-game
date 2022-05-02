#include "dma.h"

void config_dma(int src_inc_mode) {
    DMA1CTL &= ~DMAEN;

    DMACTL0 |= DMA1TSEL_0; // Disparo manual

    DMA1CTL = DMADT_1 | // Block transfer
        src_inc_mode |  // Modo do ponteiro fonte
        DMADSTINCR_3 |  // Incrementar ponteiro destino
        DMASRCBYTE |    // Fonte opera com bytes
        DMADSTBYTE;     // Destino opera com bytes
}

void dma_transfer(uint8_t* source, uint8_t* destination, int size) {
    DMA1SA = source;
    DMA1DA = destination;
    DMA1SZ = size;

    DMA1CTL |= DMAEN;

    DMA1CTL |= DMAREQ; // Trigger manual

    while (!(DMA1CTL & DMAIFG));
    DMA1CTL &= ~DMAIFG;
}
