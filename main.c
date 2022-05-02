#include <msp430.h>
#include <stdint.h>
#include "oled.h"
#include "dma.h"
#include "timers.h"
#include "game.h"
#include "uart.h"

#define TRUE        1
#define FALSE       0

void check_oled_actions();

enum {
    LEFT = '1',
    DOWN_BRIGHTNESS,
    START,
    INVERT,
    UP_BRIGHTNESS,
    RIGHT
} actions;

uint8_t action;
int action_received = FALSE;
int update_screen = FALSE;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    config_uart();
    config_oled();
    config_timers();

    __enable_interrupt();

    while (1) {
        switch (state) {
        case INITIAL:
            show_start_game_screen();
            if (action_received) {
                if (action == START) {
                    action_received = FALSE;
                    start_game();
                    state = PLAYING;
                }
                else check_oled_actions();
            }
            break;
        case PLAYING:
            if (update_screen) {
                update_screen = FALSE;
                move_cars_forward();
                oled_update_screen();
            }

            if (action_received) {
                if (action == LEFT) {
                    action_received = FALSE;
                    move_player_car_left();
                }
                else if (action == RIGHT) {
                    action_received = FALSE;
                    move_player_car_right();
                }
                else check_oled_actions();
            }
            break;
        case LOST:
            show_game_over_scene();
            state = INITIAL;
            break;
        default:
            break;
        }
    }

    return 0;
}


void check_oled_actions() {
    switch (action) {
    case INVERT:
        oled_invert();
        break;
    case UP_BRIGHTNESS:
        oled_increase_brightness();
        break;
    case DOWN_BRIGHTNESS:
        oled_reduce_brightness();
        break;
    default:
        break;
    }

    action_received = FALSE;
}

#pragma vector = USCI_A0_VECTOR
__interrupt void UART0_INTERRUPT(void) {
    switch (UCA0IV) {
    case 2:  // Recepção
        if (UCA0RXBUF != 0x0D && UCA0RXBUF != 0x0A) {
            action = UCA0RXBUF;
            action_received = TRUE;
        }
        break;
    default: break;
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer_A0_isr(void) {
    update_screen = TRUE;
}
