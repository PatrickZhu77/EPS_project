
#include "custom_crc.h"

uint8_t crc8_calculate(uint8_t data)
{
    uint8_t i;
    uint8_t crc = 0;

    crc ^= data;

    for ( i = 0; i < 8; i++ )
    {
        if ( crc & 0x80 )
            crc = (crc << 1) ^ 0x07;
        else
            crc <<= 1;
    }
    return crc;
}

uint8_t crc8_check(uint8_t data, uint8_t crc)
{
    uint8_t temp_crc = 0;

    temp_crc = crc8_calculate(data);
    if(temp_crc == crc)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
