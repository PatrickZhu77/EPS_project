#include "error_message.h"

/***************************************************************************
 * @brief
 *   Initialize the error buffer
 *
 * @param[in] err_buffer
 *   Pointer to error buffer
 *
 ******************************************************************************/
void error_buffer_init(uint8_t *err_buffer)
{
    *err_buffer = 0;
    *(err_buffer+1) = 0;
}


/***************************************************************************
 * @brief
 *   Log the error message to the buffer (in RAM)
 *
 * @param[in] err_buffer
 *   Pointer to error buffer
 *
 * @param[in] err_type
 *   Type of the error
 *
 * @param[in] err_data
 *   Error data
 *
 ******************************************************************************/
void error_log_the_data(uint8_t *err_buffer, uint8_t err_type, uint8_t err_data)
{
    uint8_t write_ptr = *err_buffer;
    uint8_t err_logged = *(err_buffer+1);

    if(err_logged < ERROR_BUFFER_SIZE)                           //if the buffer is not full
    {
        *(err_buffer+2+2*write_ptr) = err_type;                  //even bit stores error type
        *(err_buffer+2+2*write_ptr+1) = err_data;                //odd bit stores error data

        *err_buffer = write_ptr+1;                               //write pointer plus one
        *(err_buffer+1) = err_logged+1;                          //number of logged error plus one

    }
    else                                                         //if the buffer is full
    {
        write_ptr = write_ptr%ERROR_BUFFER_SIZE;                 //if write pointer equals ERROR_BUFFER_SIZE, it will be set to 0. Otherwise it remains the same value.

        *(err_buffer+2+2*write_ptr) = err_type;                  //even bit stores error type
        *(err_buffer+2+2*write_ptr+1) = err_data;                //odd bit stores error data

        *err_buffer = write_ptr+1;                               //write pointer plus one
    }

}

/***************************************************************************
 * @brief
 *   Log the timestamp of error message to the buffer (in RAM)
 *
 * @param[in] err_buffer
 *   Pointer to error buffer
 *
 * @param[in] timestamp_s_buffer
 *   Pointer to error timestamp_sec buffer
 *
 * @param[in] timestamp_ms_buffer
 *   Pointer to error timestamp_ms buffer
 *
 * @param[in] timestamp_s
 *   Timestamp in second when the error occurs
 *
 *  @param[in] timestamp_ms
 *   Timestamp in ms when the error occurs
 *
 ******************************************************************************/
void error_log_the_timestamp(uint8_t *err_buffer, uint32_t *timestamp_s_buffer, uint16_t *timestamp_ms_buffer, uint32_t timestamp_s, uint16_t timestamp_ms)
{
    uint8_t write_ptr = (*err_buffer) - 1;

    *(timestamp_s_buffer+write_ptr) = timestamp_s;
    *(timestamp_ms_buffer+write_ptr) = timestamp_ms;
}
