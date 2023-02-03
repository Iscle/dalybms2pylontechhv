/*
 * JK-BMS to Pylontech HV (High Voltage) CAN Bus converter
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

#include <stdint.h>

// basic status
#define PYLONTECH_BASIC_STATUS_FLAG_SLEEP 0x00
#define PYLONTECH_BASIC_STATUS_FLAG_CHARGE 0x01
#define PYLONTECH_BASIC_STATUS_FLAG_DISCHARGE 0x02
#define PYLONTECH_BASIC_STATUS_FLAG_IDLE 0x03

#define PYLONTECH_BASIC_STATUS_FLAG_FORCE_CHARGE 0x08
#define PYLONTECH_BASIC_STATUS_FLAG_FORCE_BALANCE 0x10

// fault
// todo

// alarm
// todo

// extended fault
// todo

void pylontech_hv_0x4210(uint8_t *buf, uint32_t battery_voltage, int32_t battery_current, int16_t temp, uint8_t soc, uint8_t soh);

void pylontech_hv_0x4220(uint8_t *buf, uint32_t charge_cutoff_voltage, uint32_t discharge_cutoff_voltage, int32_t max_charge_current, int32_t max_discharge_current);

void pylontech_hv_0x4230(uint8_t *buf, uint32_t max_cell_voltage, uint32_t min_cell_voltage, uint16_t max_cell_voltage_id, uint16_t min_cell_voltage_id);

void pylontech_hv_0x4240(uint8_t *buf, int16_t max_cell_temp, int16_t min_cell_temp, uint16_t max_cell_temp_id, uint16_t min_cell_temp_id);

void pylontech_hv_0x4250(uint8_t *buf, uint8_t basic_status, uint8_t cycle_period, uint8_t fault, uint16_t alarm, uint16_t protection);

void pylontech_hv_0x4260(uint8_t *buf, uint32_t max_module_voltage, uint32_t min_module_voltage, uint16_t max_module_voltage_id, uint16_t min_module_voltage_id);

void pylontech_hv_0x4270(uint8_t *buf, int16_t module_max_temp, int16_t module_min_temp, uint16_t module_max_temp_id, uint16_t module_min_temp_id);

void pylontech_hv_0x4280(uint8_t *buf, uint8_t charge_forbidden, uint8_t discharge_forbidden);

void pylontech_hv_0x4290(uint8_t *buf, uint8_t fault_extension);

// todo: implement 0x42A0

void pylontech_hv_0x7310(uint8_t *buf);

void pylontech_hv_0x7320(uint8_t *buf, uint16_t battery_module_count, uint8_t battery_module_in_series, uint8_t battery_cell_count_in_module, uint32_t voltage_level, int32_t capacity_level);

void pylontech_hv_0x7330(uint8_t *buf);

void pylontech_hv_0x7340(uint8_t *buf);