#include "oled.h"

enum {
    SET_DISPLAY_OFF = 0xAE,
    SET_DISPLAY_ON = 0xAF,
    SET_CONTRAST = 0x81,
    NORMAL_DISPLAY = 0xA6,
    OUTPUT_RAM_CONTENT = 0xA4,
    SET_ADDRESSING_MODE = 0X20,
    HORIZONTAL_ADDRESSING_MODE = 0x00,
    SET_COLUMN_ADDRESS = 0x21,
    COLUMN_START_ADDRESS = 0x00, // Column 0
    COLUMN_END_ADDRESS = 0x7F, // Column 127
    SET_PAGE_ADDRESS = 0x22,
    PAGE_START_ADDRESS = 0x00, // Page 0
    PAGE_END_ADDRESS = 0x07, // Page 7
    SET_START_LINE = 0x40,
    SET_SEGMENT_RE_MAP = 0xA1,
    SET_MULTIPLEX_RATIO = 0xA8,
    SET_64_ROWS = 0x3F,
    REMAPPED_MODE = 0xC8,
    SET_OFFSET = 0xD3,
    NO_OFFSET = 0x00,
    SET_COM_PINS = 0xDA,
    COM_PINS_CONFIGURATION = 0x12,
    SET_DISPLAY_CLOCK = 0xD5,
    CLOCK_CONFIGURATION = 0x80,
    SET_CHARGE_PUMP_SETTING = 0x8D,
    ENABLE_CHARGE_PUMP = 0x14
} commands;

const uint8_t ZERO = 0x00;
uint8_t display_mode = NORMAL_DISPLAY;
uint8_t brightness = MAX_BRIGHTNESS;

struct {
    uint8_t command;
    uint8_t content[1024];
} buffer = { .command = DATA_IDENTIFIER };

void oled_send_command(uint8_t command) {
    uint8_t tmp[2] = { COMMAND_IDENTIFIER, command };
    i2c_transmit_bytes(tmp, 2);
}

void config_oled() {
    config_i2c(OLED_ADDRESS);

    const uint8_t setup_commands[52] = {
        COMMAND_IDENTIFIER,
        SET_DISPLAY_OFF,

        COMMAND_IDENTIFIER,
        SET_CONTRAST,
        COMMAND_IDENTIFIER,
        brightness,

        COMMAND_IDENTIFIER,
        display_mode,

        COMMAND_IDENTIFIER,
        OUTPUT_RAM_CONTENT,

        COMMAND_IDENTIFIER,
        SET_ADDRESSING_MODE,
        COMMAND_IDENTIFIER,
        HORIZONTAL_ADDRESSING_MODE,

        COMMAND_IDENTIFIER,
        SET_COLUMN_ADDRESS,
        COMMAND_IDENTIFIER,
        COLUMN_START_ADDRESS,
        COMMAND_IDENTIFIER,
        COLUMN_END_ADDRESS,

        COMMAND_IDENTIFIER,
        SET_PAGE_ADDRESS,
        COMMAND_IDENTIFIER,
        PAGE_START_ADDRESS,
        COMMAND_IDENTIFIER,
        PAGE_END_ADDRESS,

        COMMAND_IDENTIFIER,
        SET_START_LINE,

        COMMAND_IDENTIFIER,
        SET_SEGMENT_RE_MAP,

        COMMAND_IDENTIFIER,
        SET_MULTIPLEX_RATIO,
        COMMAND_IDENTIFIER,
        SET_64_ROWS,

        COMMAND_IDENTIFIER,
        REMAPPED_MODE,

        COMMAND_IDENTIFIER,
        SET_OFFSET,
        COMMAND_IDENTIFIER,
        NO_OFFSET,

        COMMAND_IDENTIFIER,
        SET_COM_PINS,
        COMMAND_IDENTIFIER,
        COM_PINS_CONFIGURATION,

        COMMAND_IDENTIFIER,
        SET_DISPLAY_CLOCK,
        COMMAND_IDENTIFIER,
        CLOCK_CONFIGURATION,

        COMMAND_IDENTIFIER,
        SET_CHARGE_PUMP_SETTING,
        COMMAND_IDENTIFIER,
        ENABLE_CHARGE_PUMP,
    };

    i2c_transmit_bytes(setup_commands, 52);
    oled_clear_screen();
    oled_send_command(SET_DISPLAY_ON);
}

void oled_print_area(uint8_t start_column, uint8_t start_page, uint8_t n_columns, uint8_t n_pages, uint8_t* data) {
    int i = n_pages;
    uint8_t* cursor = buffer.content + (start_page << 7) + start_column;
    config_dma(DMASRCINCR_3); // Ponteiro fonte, incrementar

    while (i--) {
        dma_transfer(data, cursor, n_columns);
        cursor += 128;
        data += n_columns;
    }
}

void oled_clear_area(uint8_t start_column, uint8_t start_page, uint8_t n_columns, uint8_t n_pages) {
    int i = n_pages;
    uint8_t* cursor = buffer.content + (start_page << 7) + start_column;
    config_dma(DMASRCINCR_0); // Ponteiro fonte, fixo

    while (i--) {
        dma_transfer(&ZERO, cursor, n_columns);
        cursor += 128;
    }
}

void oled_print_screen(uint8_t* screen) {
    config_dma(DMASRCINCR_3); // Ponteiro fonte, incrementar
    dma_transfer(screen, buffer.content, 1024);
}

void oled_clear_screen() {
    config_dma(DMASRCINCR_0); // Ponteiro fonte, fixo
    dma_transfer(&ZERO, buffer.content, 1024);
}

void oled_update_screen() {
    i2c_transmit_bytes(&buffer, 1025);
}

void oled_invert() {
    display_mode ^= 0x01;
    oled_send_command(display_mode);
}

void oled_increase_brightness() {
    if (brightness == MAX_BRIGHTNESS) return;
    brightness += BRIGHTNESS_STEP;
    oled_send_command(0x81);
    oled_send_command(brightness);
}

void oled_reduce_brightness() {
    if (brightness == MIN_BRIGHTNESS) return;
    brightness -= BRIGHTNESS_STEP;
    oled_send_command(0x81);
    oled_send_command(brightness);
}