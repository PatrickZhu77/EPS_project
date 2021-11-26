/*
 * based on max6697.c
 * Copyright (c) 2012 Guenter Roeck <linux@roeck-us.net>
 *
 */


#ifndef MAX6697_H
#define MAX6697_H


/*
 * For all bit masks:
 * bit 0:    local temperature
 * bit 1..7: remote temperatures
 */
struct max6697_platform_data {
    bool smbus_timeout_disable; /* set to disable SMBus timeouts */
    bool extended_range_enable; /* set to enable extended temp range */
    bool beta_compensation;     /* set to enable beta compensation */
    u8 alert_mask;          /* set bit to 1 to disable alert */
    u8 over_temperature_mask;   /* set bit to 1 to disable */
    u8 resistance_cancellation; /* set bit to 0 to disable
                     * bit mask for MAX6581,
                     * boolean for other chips
                     */
    u8 ideality_mask;       /* set bit to 0 to disable */
    u8 ideality_value;      /* transistor ideality as per
                     * MAX6581 datasheet
                     */
};

#endif /* MAX6697_H */
