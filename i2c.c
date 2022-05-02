#include "i2c.h"

// P3.0 - SDA
// P3.1 - SCL
void config_i2c(uint8_t address) {
    P3SEL |= BIT1 | BIT0;   // From module
    P3REN |= BIT1 | BIT0;   // Habilita resistores
    P3OUT |= BIT1 | BIT0;   // Pull-up
    
    UCB0CTL1 |= UCSWRST; //Desliga o modulo
    
    UCB0I2CSA = address;

    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;  // Master / I2C / Synchronous Mode

    UCB0CTL1 |= UCSSEL__SMCLK | UCTR; // 1 MHz / Transmitter

    UCB0BRW = 2; // 1 MHz / 2 ~ 500.00 KHz (baund rate)

    UCB0CTL1 &= ~UCSWRST; // Liga o modulo

    UCB0IE = 0;
}

void i2c_transmit_bytes(uint8_t* data, int bytesQty) {
    unsigned int i;
    UCB0IFG = 0;

    while (UCB0CTL1 & UCTXSTP);

    UCB0CTL1 |= UCTR | UCTXSTT; // transmitter / start condition

    while (!(UCB0IFG & UCTXIFG));

    for (i = 0; i < bytesQty; i++) {
        UCB0TXBUF = data[i];
        while (!(UCB0IFG & UCTXIFG)); // Espera o buffer estar disponivel
    }

    UCB0CTL1 |= UCTXSTP; // Pede stop

    while (UCB0CTL1 & UCTXSTP); // Aguarda o stop ser recebido

    UCB0IFG = 0;
}
