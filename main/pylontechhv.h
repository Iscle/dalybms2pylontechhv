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

#include <stdint.h>
#include <driver/gpio.h>

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

void pylontechhv_init(gpio_num_t tx, gpio_num_t rx);