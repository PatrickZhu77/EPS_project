
#ifndef INCLUDE_AD5324_H_
#define INCLUDE_AD5324_H_

#include <stdint.h>

//TODO: use my own libraries for NVIC stuff and GPIO control
//#include "cortex_m0.h"
//#include "nrf_gpio.h"
#include "gio.h"
//#include "spi_master.h"
#include "delay.h"
#include "clock.h"

// Chip variants
typedef enum
{
    AD5304 = 0,
    AD5314 = 1,
    AD5324 = 2
} dac_t;

// The dac chip has four output channels
typedef enum
{
    dac_OUT_A = 0,
    dac_OUT_B = 1,
    dac_OUT_C = 2,
    dac_OUT_D = 3
} dac_channel_t;

// remember the state of the selected SPI device
typedef enum
{
    IDLE,
    TRANSMITTING
} spi_activity_t;

// a struct holding the dac session parameters
typedef struct
{
             dac_t          dac_type;
             uint32_t       spi_device;
    volatile spi_activity_t spi_state;
             uint8_t        pin_nCS;
             uint8_t        pin_SCK;
             uint8_t        pin_MOSI;
} dac_struct;

// Configure
void dac_setup(
                dac_struct *dac,
                dac_t       type,
                uint32_t    spi_device,
                uint8_t     pin_nCS,
                uint8_t     pin_SCK,
                uint8_t     pin_MOSI
                );

// Use
void dac_write(
                dac_struct     *dac,
                dac_channel_t   channel,
                uint16_t        value
                );



#endif /* INCLUDE_AD5324_H_ */
