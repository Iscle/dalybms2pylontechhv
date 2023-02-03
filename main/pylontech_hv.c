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

#include "pylontech_hv.h"

void pylontech_hv_0x4210(uint8_t *buf, int32_t battery_voltage, int32_t battery_current, int16_t temp, uint8_t soc, uint8_t soh) {
    // battery voltage
    int16_t pylontech_battery_voltage = (int16_t) (battery_voltage / 100);
    buf[0] = (uint8_t) (pylontech_battery_voltage & 0xFF);
    buf[1] = (uint8_t) ((pylontech_battery_voltage >> 8) & 0xFF);

    // battery current
    int16_t pylontech_battery_current = (int16_t) ((battery_current / 100) + 300);
    buf[2] = (uint8_t) (pylontech_battery_current & 0xFF);
    buf[3] = (uint8_t) ((pylontech_battery_current >> 8) & 0xFF);

    // temp
    int16_t pylontech_temp = (int16_t) (temp + 1000);
    buf[4] = (uint8_t) (pylontech_temp & 0xFF);
    buf[5] = (uint8_t) ((pylontech_temp >> 8) & 0xFF);

    // soc
    buf[6] = soc;

    // soh
    buf[7] = soh;
}

void pylontech_hv_0x4220(uint8_t *buf, int32_t charge_cutoff_voltage, int32_t discharge_cutoff_voltage, int32_t max_charge_current, int32_t max_discharge_current) {
    // charge cutoff voltage
    int16_t pylontech_charge_cutoff_voltage = (int16_t) (charge_cutoff_voltage / 100);
    buf[0] = (uint8_t) (pylontech_charge_cutoff_voltage & 0xFF);
    buf[1] = (uint8_t) ((pylontech_charge_cutoff_voltage >> 8) & 0xFF);

    // discharge cutoff voltage
    int16_t pylontech_discharge_cutoff_voltage = (int16_t) (discharge_cutoff_voltage / 100);
    buf[2] = (uint8_t) (pylontech_discharge_cutoff_voltage & 0xFF);
    buf[3] = (uint8_t) ((pylontech_discharge_cutoff_voltage >> 8) & 0xFF);

    // max charge current
    int16_t pylontech_max_charge_current = (int16_t) ((max_charge_current / 100) + 300);
    buf[4] = (uint8_t) (pylontech_max_charge_current & 0xFF);
    buf[5] = (uint8_t) ((pylontech_max_charge_current >> 8) & 0xFF);

    // max discharge current
    int16_t pylontech_max_discharge_current = (int16_t) ((max_discharge_current / 100) + 300);
    buf[6] = (uint8_t) (pylontech_max_discharge_current & 0xFF);
    buf[7] = (uint8_t) ((pylontech_max_discharge_current >> 8) & 0xFF);
}

void pylontech_hv_0x4230(uint8_t *buf, int32_t max_cell_voltage, int32_t min_cell_voltage, uint16_t max_cell_voltage_id, uint16_t min_cell_voltage_id) {
    // max cell voltage
    buf[0] = (uint8_t) (max_cell_voltage & 0xFF);
    buf[1] = (uint8_t) ((max_cell_voltage >> 8) & 0xFF);

    // min cell voltage
    buf[2] = (uint8_t) (min_cell_voltage & 0xFF);
    buf[3] = (uint8_t) ((min_cell_voltage >> 8) & 0xFF);

    // max cell voltage id
    buf[4] = (uint8_t) (max_cell_voltage_id & 0xFF);
    buf[5] = (uint8_t) ((max_cell_voltage_id >> 8) & 0xFF);

    // min cell voltage id
    buf[6] = (uint8_t) (min_cell_voltage_id & 0xFF);
    buf[7] = (uint8_t) ((min_cell_voltage_id >> 8) & 0xFF);
}

void pylontech_hv_0x4240(uint8_t *buf, int16_t max_cell_temp, int16_t min_cell_temp, uint16_t max_cell_temp_id, uint16_t min_cell_temp_id) {
    // max cell temp
    int16_t pylontech_max_cell_temp = (int16_t) (max_cell_temp + 1000);
    buf[0] = (uint8_t) (pylontech_max_cell_temp & 0xFF);
    buf[1] = (uint8_t) ((pylontech_max_cell_temp >> 8) & 0xFF);

    // min cell temp
    int16_t pylontech_min_cell_temp = (int16_t) (min_cell_temp + 1000);
    buf[2] = (uint8_t) (pylontech_min_cell_temp & 0xFF);
    buf[3] = (uint8_t) ((pylontech_min_cell_temp >> 8) & 0xFF);

    // max cell temp id
    buf[4] = (uint8_t) (max_cell_temp_id & 0xFF);
    buf[5] = (uint8_t) ((max_cell_temp_id >> 8) & 0xFF);

    // min cell temp id
    buf[6] = (uint8_t) (min_cell_temp_id & 0xFF);
    buf[7] = (uint8_t) ((min_cell_temp_id >> 8) & 0xFF);
}

void pylontech_hv_0x4250(uint8_t *buf, uint8_t basic_status, uint8_t cycle_period, uint8_t fault, uint16_t alarm, uint16_t protection) {
    // basic status
    buf[0] = basic_status;

    // cycle period
    buf[1] = cycle_period;

    // fault
    buf[2] = fault;

    // alarm
    buf[3] = (uint8_t) (alarm & 0xFF);
    buf[4] = (uint8_t) ((alarm >> 8) & 0xFF);

    // protection
    buf[5] = (uint8_t) (protection & 0xFF);
    buf[6] = (uint8_t) ((protection >> 8) & 0xFF);
}

void pylontech_hv_0x4260(uint8_t *buf, int32_t max_module_voltage, int32_t min_module_voltage, uint16_t max_module_voltage_id, uint16_t min_module_voltage_id) {
    // max module voltage
    buf[0] = (uint8_t) (max_module_voltage & 0xFF);
    buf[1] = (uint8_t) ((max_module_voltage >> 8) & 0xFF);

    // min module voltage
    buf[2] = (uint8_t) (min_module_voltage & 0xFF);
    buf[3] = (uint8_t) ((min_module_voltage >> 8) & 0xFF);

    // max module voltage id
    buf[4] = (uint8_t) (max_module_voltage_id & 0xFF);
    buf[5] = (uint8_t) ((max_module_voltage_id >> 8) & 0xFF);

    // min module voltage id
    buf[6] = (uint8_t) (min_module_voltage_id & 0xFF);
    buf[7] = (uint8_t) ((min_module_voltage_id >> 8) & 0xFF);
}

void pylontech_hv_0x4270(uint8_t *buf, int16_t module_max_temp, int16_t module_min_temp, uint16_t module_max_temp_id, uint16_t module_min_temp_id) {
    // module max temp
    int16_t pylontech_module_max_temp = (int16_t) ((module_max_temp + 100) * 10);
    buf[0] = (uint8_t) (pylontech_module_max_temp & 0xFF);
    buf[1] = (uint8_t) ((pylontech_module_max_temp >> 8) & 0xFF);

    // module min temp
    int16_t pylontech_module_min_temp = (int16_t) ((module_min_temp + 100) * 10);
    buf[2] = (uint8_t) (pylontech_module_min_temp & 0xFF);
    buf[3] = (uint8_t) ((pylontech_module_min_temp >> 8) & 0xFF);

    // module max temp number
    buf[4] = (uint8_t) (module_max_temp_id & 0xFF);
    buf[5] = (uint8_t) ((module_max_temp_id >> 8) & 0xFF);

    // module min temp number
    buf[6] = (uint8_t) (module_min_temp_id & 0xFF);
    buf[7] = (uint8_t) ((module_min_temp_id >> 8) & 0xFF);
}

void pylontech_hv_0x4280(uint8_t *buf, uint8_t charge_forbidden, uint8_t discharge_forbidden) {
    // charge forbidden
    buf[0] = charge_forbidden ? 0xAA : 0x00;

    // discharge forbidden
    buf[1] = discharge_forbidden ? 0xAA : 0x00;

    // reserved
    buf[2] = 0x00;
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x00;
    buf[7] = 0x00;
}

void pylontech_hv_0x4290(uint8_t *buf, uint8_t fault_extension) {
    // fault extension
    buf[0] = fault_extension;

    // reserved
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x00;
    buf[7] = 0x00;
}

// todo: implement 0x42A0

void pylontech_hv_0x7310(uint8_t *buf) {
    // hardware version
    buf[0] = 0x01;

    // reserved
    buf[1] = 0x00;

    // hardware version v
    buf[2] = 0x10;

    // hardware version r
    buf[3] = 0x02;

    // software version v major
    buf[4] = 0x04;

    // software version v minor
    buf[5] = 0x05;

    // software version
    buf[6] = 0x34;

    // software version
    buf[7] = 0x0C;
}

void pylontech_hv_0x7320(uint8_t *buf, uint16_t battery_module_count, uint8_t battery_module_in_series, uint8_t battery_cell_count_in_module, int32_t voltage_level, int32_t capacity_level) {
    // battery module count
    buf[0] = (uint8_t) (battery_module_count & 0xFF);
    buf[1] = (uint8_t) ((battery_module_count >> 8) & 0xFF);

    // battery module in series
    buf[2] = battery_module_in_series;

    // battery cell count in module
    buf[3] = battery_cell_count_in_module;

    // voltage level
    int16_t pylontech_voltage_level = (int16_t) (voltage_level / 1000);
    buf[4] = (uint8_t) (pylontech_voltage_level & 0xFF);
    buf[5] = (uint8_t) ((pylontech_voltage_level >> 8) & 0xFF);

    // capacity level
    int16_t pylontech_capacity_level = (int16_t) (capacity_level / 1000);
    buf[6] = (uint8_t) (pylontech_capacity_level & 0xFF);
    buf[7] = (uint8_t) ((pylontech_capacity_level >> 8) & 0xFF);
}

void pylontech_hv_0x7330(uint8_t *buf) {
    buf[0] = 'P';
    buf[1] = 'Y';
    buf[2] = 'L';
    buf[3] = 'O';
    buf[4] = 'N';
    buf[5] = 'T';
    buf[6] = 'E';
    buf[7] = 'C';
}

void pylontech_hv_0x7340(uint8_t *buf) {
    buf[0] = 'H';
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x00;
    buf[7] = 0x00;
}