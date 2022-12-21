

#include "fee_function.h"
#include "ti_fee.h"
#include "sys_common.h"


static Std_ReturnType oResult=E_OK;
static uint16 Status;

void delay(void)
{
    unsigned int dummycnt=0x0000FFU;
    do
    {
        dummycnt--;
    }
    while(dummycnt>0);
}

/***************************************************************************
 * @brief
 *   Internal functions used during data conversion process
 *
 ******************************************************************************/
uint8_t pick_high8bit_of_uint16(uint16_t data)
{
    return (uint8_t)(data>>8);
}

uint8_t pick_low8bit_of_uint16(uint16_t data)
{
    return (uint8_t)data;
}

uint8_t pick_highest8bit_of_uint32(uint32_t data)
{
    return (uint8_t)(data>>24);
}

uint8_t pick_high8bit_of_uint32(uint32_t data)
{
    return (uint8_t)(data>>16);
}

uint8_t pick_low8bit_of_uint32(uint32_t data)
{
    return (uint8_t)(data>>8);
}

uint8_t pick_lowest8bit_of_uint32(uint32_t data)
{
    return (uint8_t)data;
}

uint16_t combine_8bit_to_uint16(uint8_t *data)
{
    uint16_t data_temp = 0;
    data_temp = *(data)<<8 | *(data+1);
    return data_temp;
}

uint32_t combine_8bit_to_uint32(uint8_t *data)
{
    uint32_t data_temp = 0;
    data_temp = *(data)<<24 | *(data+1)<<16 | *(data+2)<<8 | *(data+3);
    return data_temp;
}
/*****************************************************************************/



/***************************************************************************
 * @brief
 *   Initialize the TI FEE module (To be called only once at the beginning)
 *
 ******************************************************************************/
void fee_initial(void)
{
    TI_Fee_Init();
    do
    {
        TI_Fee_MainFunction();
        delay();
        Status=TI_Fee_GetStatus(0 );
    }
    while(Status!= IDLE);

}

/***************************************************************************
 * @brief
 *   Package config data in 8bit format with CRC checksum
 *
 * @param[in] source_data
 *   Pointer to the source config data.
 *
 * @param[in] packed_data
 *   Pointer to the destination packaged data .
 ******************************************************************************/
void fee_package_data_with_crc(system_config_t *source_data, uint8_t *packed_data)
{
    uint8_t i = 0;
    uint16_t pointer_counter = 0;
    uint32_t crc_checksum = 0;

    /*Start packaging data*/

    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->configuration_version);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->configuration_version);
    pointer_counter++;

    for(i=0; i<NUM_OF_INA226_OVERCURRENT_PROTECTION; i++)
    {
        *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->overcurrent_protection_alert_mA[i]);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->overcurrent_protection_alert_mA[i]);
        pointer_counter++;
    }

    for(i=0; i<NUM_OF_INA226_MONITOR; i++)
    {
        *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->current_monitor_alert_mA[i]);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->current_monitor_alert_mA[i]);
        pointer_counter++;
    }

    for(i=0; i<NUM_OF_INA226_OVERCURRENT_PROTECTION; i++)
    {
        *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->overcurrent_protection_Rshunt[i]);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->overcurrent_protection_Rshunt[i]);
        pointer_counter++;
    }

    for(i=0; i<NUM_OF_INA226_MONITOR; i++)
    {
        *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->current_monitor_Rshunt[i]);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->current_monitor_Rshunt[i]);
        pointer_counter++;
    }

    for(i=0; i<NUM_OF_INA226_BATTERY; i++)
    {
        *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->battery_protection_Rshunt[i]);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->battery_protection_Rshunt[i]);
        pointer_counter++;
    }

    for(i=0; i<NUM_OF_INA226_CHANNEL; i++)
    {
        *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->channel_monitor_Rshunt[i]);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->channel_monitor_Rshunt[i]);
        pointer_counter++;
    }

    for(i=0; i<NUM_OF_INA3221; i++)
    {
        *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->power_conversion_Rshunt[i]);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->power_conversion_Rshunt[i]);
        pointer_counter++;
    }

    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->batt_charging_current_limit_mA);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->batt_charging_current_limit_mA);
    pointer_counter++;

    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->batt_discharging_current_limit_mA);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->batt_discharging_current_limit_mA);
    pointer_counter++;

    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->heater_tumble_threshold_time_s);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->heater_tumble_threshold_time_s);
    pointer_counter++;

    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->heater_solar_panel_threshold_power_mW);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->heater_solar_panel_threshold_power_mW);
    pointer_counter++;

    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->heater_orbit_period_s);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->heater_orbit_period_s);
    pointer_counter++;

    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->heater_battery_heat_up_time_s);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->heater_battery_heat_up_time_s);
    pointer_counter++;

    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->dac_init);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->dac_init);
    pointer_counter++;

    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->dac_stepsize_init);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->dac_stepsize_init);
    pointer_counter++;


    *(packed_data + pointer_counter) = pick_highest8bit_of_uint32((uint32_t)source_data->batt_charging_temp_min_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_high8bit_of_uint32((uint32_t)source_data->batt_charging_temp_min_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint32((uint32_t)source_data->batt_charging_temp_min_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_lowest8bit_of_uint32((uint32_t)source_data->batt_charging_temp_min_c);
    pointer_counter++;

    *(packed_data + pointer_counter) = pick_highest8bit_of_uint32((uint32_t)source_data->batt_charging_temp_max_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_high8bit_of_uint32((uint32_t)source_data->batt_charging_temp_max_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint32((uint32_t)source_data->batt_charging_temp_max_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_lowest8bit_of_uint32((uint32_t)source_data->batt_charging_temp_max_c);
    pointer_counter++;

    *(packed_data + pointer_counter) = pick_highest8bit_of_uint32((uint32_t)source_data->batt_discharging_temp_min_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_high8bit_of_uint32((uint32_t)source_data->batt_discharging_temp_min_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint32((uint32_t)source_data->batt_discharging_temp_min_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_lowest8bit_of_uint32((uint32_t)source_data->batt_discharging_temp_min_c);
    pointer_counter++;

    *(packed_data + pointer_counter) = pick_highest8bit_of_uint32((uint32_t)source_data->batt_discharging_temp_max_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_high8bit_of_uint32((uint32_t)source_data->batt_discharging_temp_max_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint32((uint32_t)source_data->batt_discharging_temp_max_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_lowest8bit_of_uint32((uint32_t)source_data->batt_discharging_temp_max_c);
    pointer_counter++;

    *(packed_data + pointer_counter) = pick_highest8bit_of_uint32((uint32_t)source_data->heater_sunshine_temp_on_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_high8bit_of_uint32((uint32_t)source_data->heater_sunshine_temp_on_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint32((uint32_t)source_data->heater_sunshine_temp_on_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_lowest8bit_of_uint32((uint32_t)source_data->heater_sunshine_temp_on_c);
    pointer_counter++;

    *(packed_data + pointer_counter) = pick_highest8bit_of_uint32((uint32_t)source_data->heater_sunshine_temp_off_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_high8bit_of_uint32((uint32_t)source_data->heater_sunshine_temp_off_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint32((uint32_t)source_data->heater_sunshine_temp_off_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_lowest8bit_of_uint32((uint32_t)source_data->heater_sunshine_temp_off_c);
    pointer_counter++;

    *(packed_data + pointer_counter) = pick_highest8bit_of_uint32((uint32_t)source_data->heater_eclipse_temp_on_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_high8bit_of_uint32((uint32_t)source_data->heater_eclipse_temp_on_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint32((uint32_t)source_data->heater_eclipse_temp_on_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_lowest8bit_of_uint32((uint32_t)source_data->heater_eclipse_temp_on_c);
    pointer_counter++;

    *(packed_data + pointer_counter) = pick_highest8bit_of_uint32((uint32_t)source_data->heater_eclipse_temp_off_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_high8bit_of_uint32((uint32_t)source_data->heater_eclipse_temp_off_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint32((uint32_t)source_data->heater_eclipse_temp_off_c);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_lowest8bit_of_uint32((uint32_t)source_data->heater_eclipse_temp_off_c);
    pointer_counter++;

    /*Sensor config struct*/
    *(packed_data + pointer_counter) = source_data->sensor_config_data.software_version;
    pointer_counter++;
    *(packed_data + pointer_counter) = source_data->sensor_config_data.max6698_cfg1_setting;
    pointer_counter++;
    *(packed_data + pointer_counter) = source_data->sensor_config_data.max6698_cfg2_setting;
    pointer_counter++;
    *(packed_data + pointer_counter) = source_data->sensor_config_data.max6698_cfg3_setting;
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->sensor_config_data.ina226_cfg_setting);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->sensor_config_data.ina226_cfg_setting);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->sensor_config_data.ina226_overcurrent_mask);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->sensor_config_data.ina226_overcurrent_mask);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->sensor_config_data.ina226_monitor_mask);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->sensor_config_data.ina226_monitor_mask);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->sensor_config_data.ina226_battery_mask);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->sensor_config_data.ina226_battery_mask);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->sensor_config_data.ina226_channel_mask);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->sensor_config_data.ina226_channel_mask);
    pointer_counter++;
//    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->sensor_config_data.ina3221_cfg_setting);
//    pointer_counter++;
//    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->sensor_config_data.ina3221_cfg_setting);
//    pointer_counter++;
//    *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->sensor_config_data.ina3221_mask_setting);
//    pointer_counter++;
//    *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->sensor_config_data.ina3221_mask_setting);
//    pointer_counter++;

    /*Channel config struct*/
    for(i=0; i<NUM_OF_CHANNELS; i++)
    {
        *(packed_data + pointer_counter) = source_data->chan_config_data[i].priority;
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->chan_config_data[i].onlevel_mV);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->chan_config_data[i].onlevel_mV);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->chan_config_data[i].offlevel_mV);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->chan_config_data[i].offlevel_mV);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->chan_config_data[i].maxI_mA);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->chan_config_data[i].maxI_mA);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->chan_config_data[i].maxV_mV);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->chan_config_data[i].maxV_mV);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->chan_config_data[i].minV_mV);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->chan_config_data[i].minV_mV);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_high8bit_of_uint16(source_data->chan_config_data[i].maxI_increment_mA);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint16(source_data->chan_config_data[i].maxI_increment_mA);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_highest8bit_of_uint32(source_data->chan_config_data[i].group_mask);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_high8bit_of_uint32(source_data->chan_config_data[i].group_mask);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint32(source_data->chan_config_data[i].group_mask);
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_lowest8bit_of_uint32(source_data->chan_config_data[i].group_mask);
        pointer_counter++;
    }

    /*Calculate CRC checksum and add it to the end of the array*/
    crc_checksum = crc32_calculate(packed_data, pointer_counter);

    *(packed_data + pointer_counter) = pick_highest8bit_of_uint32(crc_checksum);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_high8bit_of_uint32(crc_checksum);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_low8bit_of_uint32(crc_checksum);
    pointer_counter++;
    *(packed_data + pointer_counter) = pick_lowest8bit_of_uint32(crc_checksum);

}


/***************************************************************************
 * @brief
 *   Check CRC checksum of packaged data and then unpackage it if CRC is good
 *
 * @param[in] packed_data
 *   Pointer to the data to be unpackaged.
 *
 * @param[in] dest_data
 *   Pointer to the destination data structure to store the config data.
 *
 * @return
 *   Return the result of this conversion. 1:CRC is bad. 0:CRC is good, data unpackaged.
 ******************************************************************************/
uint8_t fee_check_crc_then_unpackage_data(uint8_t *packed_data, system_config_t *dest_data)
{
    uint8_t i = 0;
    uint16_t pointer_counter = 0;
    uint32_t crc_checksum = 0, stored_crc_checksum = 0;

    /*Calculate the CRC checksum of the packaged data*/
    crc_checksum = crc32_calculate(packed_data, (FLASHED_DATA_LENGTH-4));   //minus 4 bytes of crc checksum
    /*Read previous CRC checksum*/
    stored_crc_checksum = combine_8bit_to_uint32(packed_data+(FLASHED_DATA_LENGTH-4));

    if(crc_checksum != stored_crc_checksum)         //if crc is bad, return 1
    {
        return 1;
    }

    /*if crc is good, unpackage the data*/
    dest_data->configuration_version = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;

    for(i=0; i<NUM_OF_INA226_OVERCURRENT_PROTECTION; i++)
    {
        dest_data->overcurrent_protection_alert_mA[i] = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

    for(i=0; i<NUM_OF_INA226_MONITOR; i++)
    {
        dest_data->current_monitor_alert_mA[i] = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

    for(i=0; i<NUM_OF_INA226_OVERCURRENT_PROTECTION; i++)
    {
        dest_data->overcurrent_protection_Rshunt[i] = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

    for(i=0; i<NUM_OF_INA226_MONITOR; i++)
    {
        dest_data->current_monitor_Rshunt[i] = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

    for(i=0; i<NUM_OF_INA226_BATTERY; i++)
    {
        dest_data->battery_protection_Rshunt[i] = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

    for(i=0; i<NUM_OF_INA226_CHANNEL; i++)
    {
        dest_data->channel_monitor_Rshunt[i] = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

    for(i=0; i<NUM_OF_INA3221; i++)
    {
        dest_data->power_conversion_Rshunt[i] = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

    dest_data->batt_charging_current_limit_mA = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->batt_discharging_current_limit_mA = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->heater_tumble_threshold_time_s = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->heater_solar_panel_threshold_power_mW = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->heater_orbit_period_s = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->heater_battery_heat_up_time_s = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->dac_init = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->dac_stepsize_init = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;

    dest_data->batt_charging_temp_min_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;
    dest_data->batt_charging_temp_max_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;
    dest_data->batt_discharging_temp_min_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;
    dest_data->batt_discharging_temp_max_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;
    dest_data->heater_sunshine_temp_on_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;
    dest_data->heater_sunshine_temp_off_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;
    dest_data->heater_eclipse_temp_on_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;
    dest_data->heater_eclipse_temp_off_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;

    /*Sensor config struct*/
    dest_data->sensor_config_data.software_version = *(packed_data + pointer_counter);
    pointer_counter++;
    dest_data->sensor_config_data.max6698_cfg1_setting = *(packed_data + pointer_counter);
    pointer_counter++;
    dest_data->sensor_config_data.max6698_cfg1_setting = *(packed_data + pointer_counter);
    pointer_counter++;
    dest_data->sensor_config_data.max6698_cfg1_setting = *(packed_data + pointer_counter);
    pointer_counter++;
    dest_data->sensor_config_data.ina226_cfg_setting = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->sensor_config_data.ina226_overcurrent_mask = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->sensor_config_data.ina226_monitor_mask = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->sensor_config_data.ina226_battery_mask = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->sensor_config_data.ina226_channel_mask = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
//    dest_data->sensor_config_data.ina3221_cfg_setting = combine_8bit_to_uint16(packed_data + pointer_counter);
//    pointer_counter = pointer_counter+2;
//    dest_data->sensor_config_data.ina3221_mask_setting = combine_8bit_to_uint16(packed_data + pointer_counter);
//    pointer_counter = pointer_counter+2;


    for(i=0; i<NUM_OF_CHANNELS; i++)
    {
        dest_data->chan_config_data[i].priority = *(packed_data + pointer_counter);
        pointer_counter++;
        dest_data->chan_config_data[i].onlevel_mV = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
        dest_data->chan_config_data[i].offlevel_mV = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
        dest_data->chan_config_data[i].maxI_mA = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
        dest_data->chan_config_data[i].maxV_mV = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
        dest_data->chan_config_data[i].minV_mV = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
        dest_data->chan_config_data[i].maxI_increment_mA = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
        dest_data->chan_config_data[i].group_mask = combine_8bit_to_uint32(packed_data + pointer_counter);
        pointer_counter = pointer_counter+4;
    }

    return 0;
}


/***************************************************************************
 * @brief
 *   Unpackage data directly (ignore the CRC)
 *
 * @param[in] packed_data
 *   Pointer to the data to be unpackaged.
 *
 * @param[in] dest_data
 *   Pointer to the destination data structure to store the config data.
 *
 ******************************************************************************/
void fee_unpackage_data_ignore_crc(uint8_t *packed_data, system_config_t *dest_data)
{
    uint8_t i = 0;
    uint16_t pointer_counter = 0;

    /*Directly unpackages the data*/
    dest_data->configuration_version = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;

    for(i=0; i<NUM_OF_INA226_OVERCURRENT_PROTECTION; i++)
    {
        dest_data->overcurrent_protection_alert_mA[i] = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

    for(i=0; i<NUM_OF_INA226_MONITOR; i++)
    {
        dest_data->current_monitor_alert_mA[i] = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

    for(i=0; i<NUM_OF_INA226_OVERCURRENT_PROTECTION; i++)
    {
        dest_data->overcurrent_protection_Rshunt[i] = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

    for(i=0; i<NUM_OF_INA226_MONITOR; i++)
    {
        dest_data->current_monitor_Rshunt[i] = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

    for(i=0; i<NUM_OF_INA226_BATTERY; i++)
    {
        dest_data->battery_protection_Rshunt[i] = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

    for(i=0; i<NUM_OF_INA226_CHANNEL; i++)
    {
        dest_data->channel_monitor_Rshunt[i] = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

    for(i=0; i<NUM_OF_INA3221; i++)
    {
        dest_data->power_conversion_Rshunt[i] = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

    dest_data->batt_charging_current_limit_mA = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->batt_discharging_current_limit_mA = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->heater_tumble_threshold_time_s = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->heater_solar_panel_threshold_power_mW = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->heater_orbit_period_s = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->heater_battery_heat_up_time_s = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->dac_init = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->dac_stepsize_init = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;

    dest_data->batt_charging_temp_min_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;
    dest_data->batt_charging_temp_max_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;
    dest_data->batt_discharging_temp_min_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;
    dest_data->batt_discharging_temp_max_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;
    dest_data->heater_sunshine_temp_on_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;
    dest_data->heater_sunshine_temp_off_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;
    dest_data->heater_eclipse_temp_on_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;
    dest_data->heater_eclipse_temp_off_c = (int32_t)combine_8bit_to_uint32(packed_data + pointer_counter);
    pointer_counter = pointer_counter+4;

    /*Sensor config struct*/
    dest_data->sensor_config_data.software_version = *(packed_data + pointer_counter);
    pointer_counter++;
    dest_data->sensor_config_data.max6698_cfg1_setting = *(packed_data + pointer_counter);
    pointer_counter++;
    dest_data->sensor_config_data.max6698_cfg1_setting = *(packed_data + pointer_counter);
    pointer_counter++;
    dest_data->sensor_config_data.max6698_cfg1_setting = *(packed_data + pointer_counter);
    pointer_counter++;
    dest_data->sensor_config_data.ina226_cfg_setting = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->sensor_config_data.ina226_overcurrent_mask = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->sensor_config_data.ina226_monitor_mask = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->sensor_config_data.ina226_battery_mask = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
    dest_data->sensor_config_data.ina226_channel_mask = combine_8bit_to_uint16(packed_data + pointer_counter);
    pointer_counter = pointer_counter+2;
//    dest_data->sensor_config_data.ina3221_cfg_setting = combine_8bit_to_uint16(packed_data + pointer_counter);
//    pointer_counter = pointer_counter+2;
//    dest_data->sensor_config_data.ina3221_mask_setting = combine_8bit_to_uint16(packed_data + pointer_counter);
//    pointer_counter = pointer_counter+2;


    for(i=0; i<NUM_OF_CHANNELS; i++)
    {
        dest_data->chan_config_data[i].priority = *(packed_data + pointer_counter);
        pointer_counter++;
        dest_data->chan_config_data[i].onlevel_mV = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
        dest_data->chan_config_data[i].offlevel_mV = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
        dest_data->chan_config_data[i].maxI_mA = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
        dest_data->chan_config_data[i].maxV_mV = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
        dest_data->chan_config_data[i].minV_mV = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
        dest_data->chan_config_data[i].maxI_increment_mA = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
        dest_data->chan_config_data[i].group_mask = combine_8bit_to_uint32(packed_data + pointer_counter);
        pointer_counter = pointer_counter+4;
    }

}

/***************************************************************************
 * @brief
 *   Read data from FLASH
 *
 * @param[in] BlockNumber
 *   Block number of the data to be read
 *
 * @param[in] pData
 *   Pointer to the destination data structure to store the packaged data.
 *
 * @param[in] length
 *   Length of the data to be read (in byte)
 ******************************************************************************/
void fee_read_flashed_data(uint16_t BlockNumber, uint8_t *pData, uint16_t length)
{
    oResult=TI_Fee_Read(BlockNumber, 0, pData, length);
    do
    {
        TI_Fee_MainFunction();
        delay();
        Status=TI_Fee_GetStatus(0);
    }
    while(Status!=IDLE);
}

/***************************************************************************
 * @brief
 *   Package error message timestamp_sec in 8bit array format
 *
 * @param[in] source_data
 *   Pointer to the source data
 *
 * @param[in] packed_data
 *   Pointer to the destination packaged array .
 ******************************************************************************/
void fee_package_err_msg_timestamp_s(uint32_t *source_data, uint8_t *packed_data)
{
    uint8_t i = 0;
    uint16_t pointer_counter = 0;

    /*Start packaging data*/
    for(i=0; i<ERROR_BUFFER_SIZE; i++)
    {
        *(packed_data + pointer_counter) = pick_highest8bit_of_uint32(*(source_data+i));
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_high8bit_of_uint32(*(source_data+i));
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint32(*(source_data+i));
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_lowest8bit_of_uint32(*(source_data+i));
        pointer_counter++;
    }

}

/***************************************************************************
 * @brief
 *   Unpackage error message timestamp_sec data
 *
 * @param[in] packed_data
 *   Pointer to the data to be unpackaged.
 *
 * @param[in] dest_data
 *   Pointer to the destination array.
 *
 ******************************************************************************/
void fee_unpackage_err_msg_timestamp_s(uint8_t *packed_data, uint32_t *dest_data)
{
    uint8_t i = 0;
    uint16_t pointer_counter = 0;

    /*Start unpackaging data*/
    for(i=0; i<ERROR_BUFFER_SIZE; i++)
    {
        *(dest_data+i) = combine_8bit_to_uint32(packed_data + pointer_counter);
        pointer_counter = pointer_counter+4;
    }

}

/***************************************************************************
 * @brief
 *   Package error message timestamp_ms in 8bit array format
 *
 * @param[in] source_data
 *   Pointer to the source data
 *
 * @param[in] packed_data
 *   Pointer to the destination packaged array .
 ******************************************************************************/
void fee_package_err_msg_timestamp_ms(uint16_t *source_data, uint8_t *packed_data)
{
    uint8_t i = 0;
    uint16_t pointer_counter = 0;

    /*Start packaging data*/
    for(i=0; i<ERROR_BUFFER_SIZE; i++)
    {
        *(packed_data + pointer_counter) = pick_high8bit_of_uint16(*(source_data+i));
        pointer_counter++;
        *(packed_data + pointer_counter) = pick_low8bit_of_uint16(*(source_data+i));
        pointer_counter++;
    }

}

/***************************************************************************
 * @brief
 *   Unpackage error message timestamp_ms data
 *
 * @param[in] packed_data
 *   Pointer to the data to be unpackaged.
 *
 * @param[in] dest_data
 *   Pointer to the destination array.
 *
 ******************************************************************************/
void fee_unpackage_err_msg_timestamp_ms(uint8_t *packed_data, uint16_t *dest_data)
{
    uint8_t i = 0;
    uint16_t pointer_counter = 0;

    /*Start unpackaging data*/
    for(i=0; i<ERROR_BUFFER_SIZE; i++)
    {
        *(dest_data+i) = combine_8bit_to_uint16(packed_data + pointer_counter);
        pointer_counter = pointer_counter+2;
    }

}

