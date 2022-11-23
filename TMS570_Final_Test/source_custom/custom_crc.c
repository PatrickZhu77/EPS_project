
#include "custom_crc.h"


/***************************************************************************
 * @brief
 *   Calculate the CRC checksum with CRC-32 standard
 *
 * @param[in] data
 *   Pointer to the head of the data to be checked.
 *
 * @param[in] length
 *   Length of the data (in byte).
 *
 * @return
 *   Return the calculated checksum.
 *
 ****************************************************************************/
uint32_t crc32_calculate(uint8_t *data, uint16_t length)
{
    uint8_t i;
    uint32_t crc = 0xffffffff;                      //Initial value

    while(length--)
    {
        crc ^= *data;
        data++;

        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;      //0xEDB88320 = reverse 0x04C11DB7
            else
                crc = (crc >> 1);
        }
    }
    return ~crc;
}
