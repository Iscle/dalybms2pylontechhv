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

#include "jkbms.h"

static void jkbms_parse_0xff_0x01(struct jkbms_0xff_response *data, const uint8_t *buf) {
    data->temperature = buf[1] << 8 | buf[2];
    data->voltage_mv = buf[3] << 8 | buf[4];
    data->average_cell_voltage_mv = buf[5] << 8 | buf[6];
    data->calculated_cell_count = buf[7];
}

static void jkbms_parse_0xff_0x02(struct jkbms_0xff_response *data, const uint8_t *buf) {
    data->highest_voltage_cell = buf[1];
    data->lowest_voltage_cell = buf[2];
    data->balance_and_alarm = buf[3];
    data->max_voltage_difference_mv = buf[4] << 8 | buf[5];
    data->balance_current_ma = buf[6] << 8 | buf[7];
}

static void jkbms_parse_0xff_0x03(struct jkbms_0xff_response *data, const uint8_t *buf) {
    data->balance_trigger_voltage_difference_mv = buf[1] << 8 | buf[2];
    data->max_balance_current_ma = buf[3] << 8 | buf[4];
    data->balance_status = buf[5];
    data->configured_cell_count = buf[6];
}

static void jkbms_parse_0xff_0x04(struct jkbms_0xff_response *data, const uint8_t *buf) {
    data->cell_voltage_mv[buf[1]] = buf[2] << 8 | buf[3];
    data->cell_voltage_mv[buf[1] + 1] = buf[4] << 8 | buf[5];
    data->cell_voltage_mv[buf[1] + 2] = buf[6] << 8 | buf[7];
}

void jkbms_parse_0xff(struct jkbms_0xff_response *data, const uint8_t *buf) {
    jkbms_parse_0xff_0x01(data, buf);
    jkbms_parse_0xff_0x02(data, buf + 8);
    jkbms_parse_0xff_0x03(data, buf + 16);
    jkbms_parse_0xff_0x04(data, buf + 24);
    jkbms_parse_0xff_0x04(data, buf + 32);
    jkbms_parse_0xff_0x04(data, buf + 40);
    jkbms_parse_0xff_0x04(data, buf + 48);
    jkbms_parse_0xff_0x04(data, buf + 56);
    jkbms_parse_0xff_0x04(data, buf + 64);
    jkbms_parse_0xff_0x04(data, buf + 72);
    jkbms_parse_0xff_0x04(data, buf + 80);
}