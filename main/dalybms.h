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

#pragma once

#include <driver/uart.h>
#include <driver/gpio.h>

#define DALYBMS_MAX_BMS 3
#define DALYBMS_MAX_CELLS 12

struct dalybms_data {
    //int32_t *pack_voltage, int32_t *samp_total_volt, int32_t *pack_current, uint16_t *pack_soc
    int32_t pack_voltage;
    int32_t samp_total_volt;
    int32_t pack_current;
    uint16_t pack_soc;
    //int32_t *max_cell_voltage, uint8_t *max_cell_id, int32_t *min_cell_voltage, uint8_t *min_cell_id
    int32_t max_cell_voltage;
    uint8_t max_cell_voltage_id;
    int32_t min_cell_voltage;
    uint8_t min_cell_voltage_id;
    //int16_t *max_temperature, uint8_t *max_temperature_id, int16_t *min_temperature, uint8_t *min_temperature_id
    int16_t max_temperature;
    uint8_t max_temperature_id;
    int16_t min_temperature;
    uint8_t min_temperature_id;
    //uint8_t *charge_discharge_status, uint8_t *charge_fet_status, uint8_t *discharge_fet_status, uint8_t *bms_life, int32_t *residual_capacity
    uint8_t charge_discharge_status;
    uint8_t charge_fet_status;
    uint8_t discharge_fet_status;
    uint8_t bms_life;
    int32_t residual_capacity;
    //uint8_t *cell_count, uint8_t *temp_sensor_count, uint8_t *charge_status, uint8_t *load_status, uint8_t *dio, uint16_t *cycle_count
    uint8_t cell_count;
    uint8_t temp_sensor_count;
    uint8_t charge_status;
    uint8_t load_status;
    uint8_t dio;
    uint16_t cycle_count;
    //int32_t *cell_voltages
    int32_t cell_voltages[DALYBMS_MAX_CELLS];
    //int16_t *temperatures
    int16_t temperatures[DALYBMS_MAX_CELLS];
    //uint8_t *balance_states
    uint8_t balance_states[DALYBMS_MAX_CELLS];
    //uint8_t *errors
    uint8_t errors[8];
};

struct dalybms_message {
    uint8_t magic; // DALYBMS_MAGIC
    uint8_t address; // 1 - 16
    uint8_t id;
    uint8_t len; // 8
    uint8_t data[8];
    uint8_t checksum;
} __attribute__((packed)) ;

int32_t dalybms_get_battery_voltage(void);
uint8_t dalybms_get_battery_soc(void);
int32_t dalybms_get_battery_soh(void);
int32_t dalybms_get_max_cell_voltage(void);
uint8_t dalybms_get_max_cell_voltage_id(void);
int32_t dalybms_get_min_cell_voltage(void);
uint8_t dalybms_get_min_cell_voltage_id(void);
int32_t dalybms_get_max_module_voltage(void);
uint8_t dalybms_get_max_module_voltage_id(void);
int32_t dalybms_get_min_module_voltage(void);
uint8_t dalybms_get_min_module_voltage_id(void);

void dalybms_init(uart_port_t uart_num, gpio_num_t ro, gpio_num_t re_de, gpio_num_t di);