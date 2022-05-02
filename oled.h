#ifndef OLED_H_
#define OLED_H_

#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include "i2c.h"
#include "elements.h"
#include "game.h"
#include "dma.h"

#define OLED_ADDRESS        0x3C
#define TOTAL_PAGES         8
#define TOTAL_COLUMNS       128
#define MAX_BRIGHTNESS      0xFF
#define MIN_BRIGHTNESS      0x33
#define BRIGHTNESS_STEP     0x33
#define COMMAND_IDENTIFIER  0x80
#define DATA_IDENTIFIER     0x40

void config_oled();
void oled_send_command(uint8_t command);
void oled_print_area(uint8_t start_column, uint8_t start_page, uint8_t n_columns, uint8_t n_pages, uint8_t* data);
void oled_clear_area(uint8_t start_column, uint8_t start_page, uint8_t n_columns, uint8_t n_pages);
void oled_print_screen(uint8_t* screen);
void oled_clear_screen();
void oled_update_screen();
void oled_invert();
void oled_increase_brightness();
void oled_reduce_brightness();

#endif  // OLED_H_
