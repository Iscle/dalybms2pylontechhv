/*
 * Daly BMS to Pylontech HV (High Voltage) CAN Bus converter
 * Copyright (C) 2023  Iscle
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pylontechhv.h"
#include <driver/twai.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "log.h"

static void pylontechhv_0x4210(twai_message_t *msg, int32_t battery_voltage, int32_t battery_current, int16_t temp, uint8_t soc, uint8_t soh) {
    msg->data_length_code = 8;

    // battery voltage
    int16_t pylontech_battery_voltage = (int16_t) (battery_voltage / 100);
    msg->data[0] = (uint8_t) (pylontech_battery_voltage & 0xFF);
    msg->data[1] = (uint8_t) ((pylontech_battery_voltage >> 8) & 0xFF);

    // battery current
    int16_t pylontech_battery_current = (int16_t) ((battery_current / 100) + 300);
    msg->data[2] = (uint8_t) (pylontech_battery_current & 0xFF);
    msg->data[3] = (uint8_t) ((pylontech_battery_current >> 8) & 0xFF);

    // temp
    int16_t pylontech_temp = (int16_t) (temp + 1000);
    msg->data[4] = (uint8_t) (pylontech_temp & 0xFF);
    msg->data[5] = (uint8_t) ((pylontech_temp >> 8) & 0xFF);

    // soc
    msg->data[6] = soc;

    // soh
    msg->data[7] = soh;
}

static void pylontechhv_0x4220(twai_message_t *msg, int32_t charge_cutoff_voltage, int32_t discharge_cutoff_voltage, int32_t max_charge_current, int32_t max_discharge_current) {
    msg->data_length_code = 8;

    // charge cutoff voltage
    int16_t pylontech_charge_cutoff_voltage = (int16_t) (charge_cutoff_voltage / 100);
    msg->data[0] = (uint8_t) (pylontech_charge_cutoff_voltage & 0xFF);
    msg->data[1] = (uint8_t) ((pylontech_charge_cutoff_voltage >> 8) & 0xFF);

    // discharge cutoff voltage
    int16_t pylontech_discharge_cutoff_voltage = (int16_t) (discharge_cutoff_voltage / 100);
    msg->data[2] = (uint8_t) (pylontech_discharge_cutoff_voltage & 0xFF);
    msg->data[3] = (uint8_t) ((pylontech_discharge_cutoff_voltage >> 8) & 0xFF);

    // max charge current
    int16_t pylontech_max_charge_current = (int16_t) ((max_charge_current / 100) + 300);
    msg->data[4] = (uint8_t) (pylontech_max_charge_current & 0xFF);
    msg->data[5] = (uint8_t) ((pylontech_max_charge_current >> 8) & 0xFF);

    // max discharge current
    int16_t pylontech_max_discharge_current = (int16_t) ((max_discharge_current / 100) + 300);
    msg->data[6] = (uint8_t) (pylontech_max_discharge_current & 0xFF);
    msg->data[7] = (uint8_t) ((pylontech_max_discharge_current >> 8) & 0xFF);
}

static void pylontechhv_0x4230(twai_message_t *msg, int32_t max_cell_voltage, int32_t min_cell_voltage, uint16_t max_cell_voltage_id, uint16_t min_cell_voltage_id) {
    msg->data_length_code = 8;

    // max cell voltage
    msg->data[0] = (uint8_t) (max_cell_voltage & 0xFF);
    msg->data[1] = (uint8_t) ((max_cell_voltage >> 8) & 0xFF);

    // min cell voltage
    msg->data[2] = (uint8_t) (min_cell_voltage & 0xFF);
    msg->data[3] = (uint8_t) ((min_cell_voltage >> 8) & 0xFF);

    // max cell voltage id
    msg->data[4] = (uint8_t) (max_cell_voltage_id & 0xFF);
    msg->data[5] = (uint8_t) ((max_cell_voltage_id >> 8) & 0xFF);

    // min cell voltage id
    msg->data[6] = (uint8_t) (min_cell_voltage_id & 0xFF);
    msg->data[7] = (uint8_t) ((min_cell_voltage_id >> 8) & 0xFF);
}

static void pylontechhv_0x4240(twai_message_t *msg, int16_t max_cell_temp, int16_t min_cell_temp, uint16_t max_cell_temp_id, uint16_t min_cell_temp_id) {
    msg->data_length_code = 8;

    // max cell temp
    int16_t pylontech_max_cell_temp = (int16_t) (max_cell_temp + 1000);
    msg->data[0] = (uint8_t) (pylontech_max_cell_temp & 0xFF);
    msg->data[1] = (uint8_t) ((pylontech_max_cell_temp >> 8) & 0xFF);

    // min cell temp
    int16_t pylontech_min_cell_temp = (int16_t) (min_cell_temp + 1000);
    msg->data[2] = (uint8_t) (pylontech_min_cell_temp & 0xFF);
    msg->data[3] = (uint8_t) ((pylontech_min_cell_temp >> 8) & 0xFF);

    // max cell temp id
    msg->data[4] = (uint8_t) (max_cell_temp_id & 0xFF);
    msg->data[5] = (uint8_t) ((max_cell_temp_id >> 8) & 0xFF);

    // min cell temp id
    msg->data[6] = (uint8_t) (min_cell_temp_id & 0xFF);
    msg->data[7] = (uint8_t) ((min_cell_temp_id >> 8) & 0xFF);
}

static void pylontechhv_0x4250(twai_message_t *msg, uint8_t basic_status, uint16_t cycle_period, uint8_t fault, uint16_t alarm, uint16_t protection) {
    msg->data_length_code = 8;

    // basic status
    msg->data[0] = basic_status;

    // cycle period
    msg->data[1] = (uint8_t) (cycle_period & 0xFF);
    msg->data[2] = (uint8_t) ((cycle_period >> 8) & 0xFF);

    // fault
    msg->data[3] = fault;

    // alarm
    msg->data[4] = (uint8_t) (alarm & 0xFF);
    msg->data[5] = (uint8_t) ((alarm >> 8) & 0xFF);

    // protection
    msg->data[6] = (uint8_t) (protection & 0xFF);
    msg->data[7] = (uint8_t) ((protection >> 8) & 0xFF);
}

static void pylontechhv_0x4260(twai_message_t *msg, int32_t max_module_voltage, int32_t min_module_voltage, uint16_t max_module_voltage_id, uint16_t min_module_voltage_id) {
    msg->data_length_code = 8;
    
    // max module voltage
    msg->data[0] = (uint8_t) (max_module_voltage & 0xFF);
    msg->data[1] = (uint8_t) ((max_module_voltage >> 8) & 0xFF);

    // min module voltage
    msg->data[2] = (uint8_t) (min_module_voltage & 0xFF);
    msg->data[3] = (uint8_t) ((min_module_voltage >> 8) & 0xFF);

    // max module voltage id
    msg->data[4] = (uint8_t) (max_module_voltage_id & 0xFF);
    msg->data[5] = (uint8_t) ((max_module_voltage_id >> 8) & 0xFF);

    // min module voltage id
    msg->data[6] = (uint8_t) (min_module_voltage_id & 0xFF);
    msg->data[7] = (uint8_t) ((min_module_voltage_id >> 8) & 0xFF);
}

static void pylontechhv_0x4270(twai_message_t *msg, int16_t module_max_temp, int16_t module_min_temp, uint16_t module_max_temp_id, uint16_t module_min_temp_id) {
    msg->data_length_code = 8;
    
    // module max temp
    int16_t pylontech_module_max_temp = (int16_t) ((module_max_temp + 100) * 10);
    msg->data[0] = (uint8_t) (pylontech_module_max_temp & 0xFF);
    msg->data[1] = (uint8_t) ((pylontech_module_max_temp >> 8) & 0xFF);

    // module min temp
    int16_t pylontech_module_min_temp = (int16_t) ((module_min_temp + 100) * 10);
    msg->data[2] = (uint8_t) (pylontech_module_min_temp & 0xFF);
    msg->data[3] = (uint8_t) ((pylontech_module_min_temp >> 8) & 0xFF);

    // module max temp id
    msg->data[4] = (uint8_t) (module_max_temp_id & 0xFF);
    msg->data[5] = (uint8_t) ((module_max_temp_id >> 8) & 0xFF);

    // module min temp id
    msg->data[6] = (uint8_t) (module_min_temp_id & 0xFF);
    msg->data[7] = (uint8_t) ((module_min_temp_id >> 8) & 0xFF);
}

static void pylontechhv_0x4280(twai_message_t *msg, uint8_t charge_forbidden, uint8_t discharge_forbidden) {
    msg->data_length_code = 8;

    // charge forbidden
    msg->data[0] = charge_forbidden ? 0xAA : 0x00;

    // discharge forbidden
    msg->data[1] = discharge_forbidden ? 0xAA : 0x00;

    // reserved
    msg->data[2] = 0x00;
    msg->data[3] = 0x00;
    msg->data[4] = 0x00;
    msg->data[5] = 0x00;
    msg->data[6] = 0x00;
    msg->data[7] = 0x00;
}

static void pylontechhv_0x4290(twai_message_t *msg, uint8_t fault_extension) {
    msg->data_length_code = 8;

    // fault extension
    msg->data[0] = fault_extension;

    // reserved
    msg->data[1] = 0x00;
    msg->data[2] = 0x00;
    msg->data[3] = 0x00;
    msg->data[4] = 0x00;
    msg->data[5] = 0x00;
    msg->data[6] = 0x00;
    msg->data[7] = 0x00;
}

// todo: implement 0x42A0

static void pylontechhv_0x7310(twai_message_t *msg) {
    msg->data_length_code = 8;

    // hardware version
    msg->data[0] = 0x01;

    // reserved
    msg->data[1] = 0x00;

    // hardware version v
    msg->data[2] = 0x10;

    // hardware version r
    msg->data[3] = 0x02;

    // software version v major
    msg->data[4] = 0x04;

    // software version v minor
    msg->data[5] = 0x05;

    // software version
    msg->data[6] = 0x34;

    // software version
    msg->data[7] = 0x0C;
}

static void pylontechhv_0x7320(twai_message_t *msg, uint16_t battery_module_count, uint8_t battery_module_in_series, uint8_t battery_cell_count_in_module, int32_t voltage_level, int32_t capacity_level) {
    msg->data_length_code = 8;

    // battery module count
    msg->data[0] = (uint8_t) (battery_module_count & 0xFF);
    msg->data[1] = (uint8_t) ((battery_module_count >> 8) & 0xFF);

    // battery module in series
    msg->data[2] = battery_module_in_series;

    // battery cell count in module
    msg->data[3] = battery_cell_count_in_module;

    // voltage level
    int16_t pylontech_voltage_level = (int16_t) (voltage_level / 1000);
    msg->data[4] = (uint8_t) (pylontech_voltage_level & 0xFF);
    msg->data[5] = (uint8_t) ((pylontech_voltage_level >> 8) & 0xFF);

    // capacity level
    int16_t pylontech_capacity_level = (int16_t) (capacity_level / 1000);
    msg->data[6] = (uint8_t) (pylontech_capacity_level & 0xFF);
    msg->data[7] = (uint8_t) ((pylontech_capacity_level >> 8) & 0xFF);
}

static void pylontechhv_0x7330(twai_message_t *msg) {
    msg->data_length_code = 8;
    msg->data[0] = 'P';
    msg->data[1] = 'Y';
    msg->data[2] = 'L';
    msg->data[3] = 'O';
    msg->data[4] = 'N';
    msg->data[5] = 'T';
    msg->data[6] = 'E';
    msg->data[7] = 'C';
}

static void pylontechhv_0x7340(twai_message_t *msg) {
    msg->data_length_code = 8;
    msg->data[0] = 'H';
    msg->data[1] = 0x00;
    msg->data[2] = 0x00;
    msg->data[3] = 0x00;
    msg->data[4] = 0x00;
    msg->data[5] = 0x00;
    msg->data[6] = 0x00;
    msg->data[7] = 0x00;
}

static uint8_t device_address = 1;

static void pylontechhv_send_0x4200_0_response(void) {
    twai_message_t msg = {
        .extd = 1,
    };

    msg.identifier = 0x4210 + device_address;
    pylontechhv_0x4210(&msg, 0, 0, 0, 0, 0);
    assert(twai_transmit(&msg, portMAX_DELAY) == ESP_OK);

    msg.identifier = 0x4220 + device_address;
    pylontechhv_0x4220(&msg, 0, 0, 0, 0);
    assert(twai_transmit(&msg, portMAX_DELAY) == ESP_OK);

    msg.identifier = 0x4230 + device_address;
    pylontechhv_0x4230(&msg, 0, 0, 0, 0);
    assert(twai_transmit(&msg, portMAX_DELAY) == ESP_OK);

    msg.identifier = 0x4240 + device_address;
    pylontechhv_0x4240(&msg, 0, 0, 0, 0);
    assert(twai_transmit(&msg, portMAX_DELAY) == ESP_OK);

    msg.identifier = 0x4250 + device_address;
    pylontechhv_0x4250(&msg, 0, 0, 0, 0, 0);
    assert(twai_transmit(&msg, portMAX_DELAY) == ESP_OK);

    msg.identifier = 0x4260 + device_address;
    pylontechhv_0x4260(&msg, 0, 0, 0, 0);
    assert(twai_transmit(&msg, portMAX_DELAY) == ESP_OK);

    msg.identifier = 0x4270 + device_address;
    pylontechhv_0x4270(&msg, 0, 0, 0, 0);
    assert(twai_transmit(&msg, portMAX_DELAY) == ESP_OK);

    msg.identifier = 0x4280 + device_address;
    pylontechhv_0x4280(&msg, 0, 0);
    assert(twai_transmit(&msg, portMAX_DELAY) == ESP_OK);

    msg.identifier = 0x4290 + device_address;
    pylontechhv_0x4290(&msg, 0);
    assert(twai_transmit(&msg, portMAX_DELAY) == ESP_OK);
}

static void pylontechhv_send_0x4200_2_response(void) {
    twai_message_t msg = {
        .extd = 1,
    };

    msg.identifier = 0x7310 + device_address;
    pylontechhv_0x7310(&msg);
    assert(twai_transmit(&msg, portMAX_DELAY) == ESP_OK);

    msg.identifier = 0x7320 + device_address;
    pylontechhv_0x7320(&msg, 0, 0, 0, 0, 0);
    assert(twai_transmit(&msg, portMAX_DELAY) == ESP_OK);

    msg.identifier = 0x7330 + device_address;
    pylontechhv_0x7330(&msg);
    assert(twai_transmit(&msg, portMAX_DELAY) == ESP_OK);

    msg.identifier = 0x7340 + device_address;
    pylontechhv_0x7340(&msg);
    assert(twai_transmit(&msg, portMAX_DELAY) == ESP_OK);
}

#define PYLONTECHHV_CAN_REQUEST_ID 0x4200
#define PYLONTECHHV_CAN_SLEEP_AWAKE_ID 0x8200
#define PYLONTECHHV_CAN_CHARGE_DISCHARGE_ID 0x8210
#define PYLONTECHHV_CAN_ERROR_MASK_ID 0x8240

static void pylontechhv_recv_can(void *arg) {
    while (1) {
        twai_message_t msg;
        if (twai_receive(&msg, portMAX_DELAY) == ESP_OK) {
            LOGI("pylontechhv: Received CAN message with ID 0x%04lX", msg.identifier);

            if (msg.identifier == PYLONTECHHV_CAN_REQUEST_ID) {
                if (msg.data[0] == 0) {
                    pylontechhv_send_0x4200_0_response();
                } else if (msg.data[0] == 2) {
                    pylontechhv_send_0x4200_2_response();
                }
            } else if ((msg.identifier & 0xFFF0) == PYLONTECHHV_CAN_SLEEP_AWAKE_ID) {
                
            } else if ((msg.identifier & 0xFFF0) == PYLONTECHHV_CAN_CHARGE_DISCHARGE_ID) {

            } else if ((msg.identifier & 0xFFF0) == PYLONTECHHV_CAN_ERROR_MASK_ID) {

            } else {
                LOGW("pylontechhv: Received unknown CAN message with ID 0x%04lX", msg.identifier);
            }
        }
    }
}

void pylontechhv_init(gpio_num_t tx, gpio_num_t rx) {
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21, GPIO_NUM_22, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    assert(twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK);
    assert(twai_start() == ESP_OK);

    xTaskCreate(pylontechhv_recv_can, "pylontechhv_recv_can", 2048, NULL, 5, NULL);
}