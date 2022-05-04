#include "timers.h"

void config_timer_A0() {
    TA0CTL = TASSEL__SMCLK | MC__UP | ID__8 | TACLR; // 125000 Hz
    TA0CCR0 = 6250; // 20hz
    TA0CCTL0 = CCIE;
}

void wait(unsigned int time) {
    TA1CCR0 = time;
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR; // 32768 Hz

    while (!(TA1CTL & TAIFG)); // Espera passar o tempo

    TA1CTL = MC__STOP;
}

void config_timer_A2() {
    P2SEL |= BIT5;  // From module
    P2DIR |= BIT5;  // Output
    P2OUT &= ~BIT5;

    TA2CTL = TASSEL__SMCLK | MC__STOP | ID__8 | TACLR; // 125000 Hz
}

void start_buzzer() {
    TA2CTL |= MC__UP | TACLR;
    TA2CCR0 = 496;
    TA2CCR2 = TA2CCR0 / 2;
    TA2CCTL2 = OUTMOD_3;
}

void stop_buzzer() {
    TA2CTL |= MC__STOP;
    TA2CCTL2 = OUTMOD_0;
}

void config_timers() {
    config_timer_A0();
    config_timer_A2();
}
