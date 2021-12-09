
#include "sys_common.h"
#include "system.h"
#include "spi.h"

#include "ad5324.h"
#include "gio.h"
#include "reg_het.h"
#include "mppt.h"



void dac_write(spiBASE_t *spi, uint16 * srcbuff, mppt_data *data)
{
    gioSetBit(hetPORT2,6,1);

    *srcbuff = (*srcbuff) & AD5324_Mask;
    *srcbuff = (*srcbuff) | (dac_OUT_A << 14) | (LDAC_high << 12);

    spiDAT1_t dataconfig1_t;

    dataconfig1_t.CS_HOLD = FALSE;
    dataconfig1_t.WDEL    = TRUE;
    dataconfig1_t.DFSEL   = SPI_FMT_0;
    dataconfig1_t.CSNR    = 0xFE;

    gioSetBit(hetPORT2,6,0);

    spiSendData(spiREG2, &dataconfig1_t, 1, srcbuff);

    gioSetBit(hetPORT2,6,1);
}

//void dac_setup(
//                dac_struct *dac,
//                dac_t       type,
//                uint32_t    spi_device,
//                uint8_t     pin_nCS,
//                uint8_t     pin_SCK,
//                uint8_t     pin_MOSI
//                )
//{
//    // apply parameters
//    dac->dac_type   = type;
//    dac->spi_device = spi_device;
//    dac->spi_state  = IDLE;
//    dac->pin_nCS    = pin_nCS;
//    dac->pin_SCK    = pin_SCK;
//    dac->pin_MOSI   = pin_MOSI;
//
//    /*
//     * Initialize SPI device
//     */
//
//    // make sure, the selected SPI device is disabled
//    spi_disable(spi_device);
//
//    // setup GPIO pins
//    nrf_gpio_pin_dir_set(pin_nCS,   NRF_GPIO_PIN_DIR_OUTPUT);
//    nrf_gpio_pin_dir_set(pin_SCK,   NRF_GPIO_PIN_DIR_OUTPUT);
//    nrf_gpio_pin_dir_set(pin_MOSI,  NRF_GPIO_PIN_DIR_OUTPUT);
///*
//    GPIO_PIN_CNF[pin_nCS]   = GPIO_OUTPUT
//                            | GPIO_INPUTBUFFER_CONNECT
//                            | GPIO_NOPULL
//                            | GPIO_DRIVE_S0S1
//                            | GPIO_SENSE_DISABLED;
//    GPIO_PIN_CNF[pin_SCK]   = 1;
//    GPIO_PIN_CNF[pin_MOSI]  = 1;
//*/
//    // Chip Select = HIGH: dac not selected
//    nrf_gpio_pin_set(pin_nCS);
//
//    // GPIO pull settings on SPI pins have no effect
//
//    /*
//    TODO: use gpio.h instead of nrf_gpio.h
//    gpio_setup(pin_nCS, OUTPUT);
//    gpio_set(pin_nCS, HIGH);
//    */
//
//    // select GPIO pins for SPI device
//    spi_pin_select(
//                    spi_device,
//                    pin_SCK,            // SCK
//                    pin_MOSI,           // MOSI
//                    SPI_PIN_DISABLED    // MISO
//                  );
//
//    // configure SPI parameters
//    SPI_CONFIG(spi_device)      = SPI_BITORDER_MSBFIRST
//                                | SPI_CLOCKPOLARITY_ACTIVELOW
//                                | SPI_CLOCKPHASE_LEADING;
//    SPI_FREQUENCY(spi_device)   = SPI_FREQUENCY_4M;
//
//    // enable interrupt, so that we know, when a transmission is complete
//    //spi_interrupt_upon_READY_enable(spi_device);
//    //TODO: use own NVIC routines from cortex_m0.h
//    //interrupt_enable(INTERRUPT_SPI);
//    //NVIC_EnableIRQ(INTERRUPT_SPI);
//
//    // configure interrupt handler to clear TRANSMITTING flag
//    if (spi_device == SPI0)
//        dac_at_spi0 = dac;
//    else if (spi_device == SPI1)
//        dac_at_spi1 = dac;
//
//    // we are ready to go
//    spi_enable(spi_device);
//}
//
//
//void dac_conv(mppt_data *data)
//{
//    // nLDAC
//    if (channel == dac_OUT_D)
//        // clear nLDAC: update all outputs after transmission completion
//        value &= ~(1 << 12);
//    else
//        // set nLDAC: only update input register, don't change output
//        value |= (1 << 12);
//
//    // no power-down (nPD)
//    value |= (1 << 13);
//
//    // clear event flag
//    SPI_EVENT_READY(dac->spi_device) = 0;
//    dac->spi_state = TRANSMITTING;
//}
//
//
//
//void dac_write(
//                dac_struct     *dac,
//                dac_channel_t   channel,
//                uint16_t        value
//                )
//{
//    // trim value according to chip resolution
//    value  &= AD53X4_RESOLUTION[dac->dac_type];
//
//    //TODO: The following line causes the SPI MOSI to stay low
//    //value <<= AD53X4_LEFTSHIFT [dac->dac_type];
//
//    // select channel
//    value  |= (channel << 14);
//
//    // nLDAC
//    if (channel == dac_OUT_D)
//        // clear nLDAC: update all outputs after transmission completion
//        value &= ~(1 << 12);
//    else
//        // set nLDAC: only update input register, don't change output
//        value |= (1 << 12);
//
//    // no power-down (nPD)
//    value |= (1 << 13);
//
//    // clear event flag
//    SPI_EVENT_READY(dac->spi_device) = 0;
//    dac->spi_state = TRANSMITTING;
//
//    // SPI: select slave
//    nrf_gpio_pin_clear(dac->pin_nCS);
//    //TODO:
//    //gpio_set(dac->pin_nCS, LOW);
//
//    // SPI_TX is double buffered, so two bytes can be pushed in one go
//    spi_write(dac->spi_device, (value >> 8) & 0xFF);
//    // transmission starts about 0-5us after write
//    spi_write(dac->spi_device, (value & 0xFF));
//
//    // wait for output to complete
//    /* TODO: somehow it doesn't work with interrupts and flags
//    while (dac->spi_state == TRANSMITTING)
//        asm("wfi");
//    */
//
//    // for 1 MHz: 14us
//    // for 2 MHz: 5us
//    // for 4 MHz: 1us
//    // 8 MHz: too fast for the DAC (or DAC test setup)
//    delay_us(1);
//
//    // this has no effect:
//    //nrf_gpio_pin_clear(dac->pin_MOSI);
//
//    // SPI: unselect slave
//    nrf_gpio_pin_set(dac->pin_nCS);
