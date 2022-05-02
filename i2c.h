#ifndef I2C_H_
#define I2C_H_

#include <msp430.h>
#include <stdint.h>
#include "dma.h"

void config_i2c(uint8_t address);
void i2c_transmit_bytes(uint8_t* data, int bytesQty);

#endif  // I2C_H_
