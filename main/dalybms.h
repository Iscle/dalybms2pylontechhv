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

#define DALYBMS_MAGIC 0xA5

struct dalybms_message {
    uint8_t magic; // DALYBMS_MAGIC
    uint8_t address; // 1 - 16
    uint8_t id;
    uint8_t len; // 8
    uint8_t data[8];
    uint8_t checksum;
} __attribute__((packed)) ;

void dalybms_test(void);

void dalybms_init(uart_port_t uart_num, int ro, int re_de, int di);