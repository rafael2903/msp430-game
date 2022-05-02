#include <msp430.h>

void config_uart() {
    P3DIR |= BIT3;
    P3SEL |= BIT3;

    P3DIR &= ~BIT4;
    P3SEL |= BIT4;
    P3REN &= ~BIT3;

    UCA0CTL1 |= UCSWRST; // Desliga o modulo

    UCA0CTL0 = UCMODE_0 | // UART
                // UCPEN |    Sem paridade
                // UCMSB |    LSB First
                // UC7BIT |   8bit
                //UCSPB |     1 Stop Bit
                //UCSYNC      Assynchronous Mode
                0;

    UCA0CTL1 = UCSSEL__SMCLK | UCSWRST;

    // BaudRate: 9600
    // BRCLK  = 1 MHz
    // UCBRx  = 104
    // UCBRSx = 1
    // UCBRFx = 0
    // UCOS16 = 0
    UCA0MCTL = UCBRS_1;
    UCA0BRW = 104;

    UCA0CTL1 &= ~UCSWRST; // Liga o modulo

    UCA0IE = UCRXIE; // Habilita interrução de recepção
}