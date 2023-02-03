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

#include <driver/spi_master.h>

#define MCP2515_CANSTAT_OPMOD_SHIFT (5)
#define MCP2515_CANSTAT_OPMOD_MASK (0xE0)
#define MCP2515_CANSTAT_OPMOD_NORMAL (0x00 << MCP2515_CANSTAT_OPMOD_SHIFT)
#define MCP2515_CANSTAT_OPMOD_SLEEP (0x01 << MCP2515_CANSTAT_OPMOD_SHIFT)
#define MCP2515_CANSTAT_OPMOD_LOOPBACK (0x02 << MCP2515_CANSTAT_OPMOD_SHIFT)
#define MCP2515_CANSTAT_OPMOD_LISTENONLY (0x03 << MCP2515_CANSTAT_OPMOD_SHIFT)
#define MCP2515_CANSTAT_OPMOD_CONFIGURATION (0x04 << MCP2515_CANSTAT_OPMOD_SHIFT)

struct mcp2515_handle {
    spi_device_handle_t spi;
};

int mcp2515_init(struct mcp2515_handle *handle, spi_host_device_t spi_host, int cs);