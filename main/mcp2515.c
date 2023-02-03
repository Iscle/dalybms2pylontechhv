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

#include "mcp2515.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>

// Instructions
#define MCP2515_CMD_RESET 0xC0
#define MCP2515_CMD_READ 0x03
#define MCP2515_CMD_READ_RX_BUFFER 0x90
#define MCP2515_CMD_WRITE 0x02
#define MCP2515_CMD_LOAD_TX_BUFFER 0x40
#define MCP2515_CMD_RTS 0x80
#define MCP2515_CMD_READ_STATUS 0xA0
#define MCP2515_CMD_RX_STATUS 0xB0
#define MCP2515_CMD_BIT_MODIFY 0x05

// Control registers
#define MCP2515_REG_BFPCTRL 0x0C
#define MCP2515_REG_TXRTSCTRL 0x0D
#define MCP2515_REG_CANSTAT 0x0E
#define MCP2515_REG_CANCTRL 0x0F
#define MCP2515_REG_TEC 0x1C
#define MCP2515_REG_REC 0x1D
#define MCP2515_REG_CNF3 0x28
#define MCP2515_REG_CNF2 0x29
#define MCP2515_REG_CNF1 0x2A
#define MCP2515_REG_CANINTE 0x2B
#define MCP2515_REG_CANINTF 0x2C
#define MCP2515_REG_EFLG 0x2D
#define MCP2515_REG_RXB0CTRL 0x60
#define MCP2515_REG_RXB1CTRL 0x70

// CAN registers
#define MCP2515_REG_TXBnCTRL(x) (0x30 + (x) * 0x10)
#define MCP2515_REG_TXBnSIDH(x) (0x31 + (x) * 0x10)
#define MCP2515_REG_TXBnSIDL(x) (0x32 + (x) * 0x10)
#define MCP2515_REG_TXBnEID8(x) (0x33 + (x) * 0x10)
#define MCP2515_REG_TXBnEID0(x) (0x34 + (x) * 0x10)
#define MCP2515_REG_TXBnDLC(x) (0x35 + (x) * 0x10)
#define MCP2515_REG_TXBnDm(x, y) (0x36 + (x) * 0x10 + (y))
#define MCP2515_REG_RXBnSIDH(x) (0x61 + (x) * 0x10)
#define MCP2515_REG_RXBnSIDL(x) (0x62 + (x) * 0x10)
#define MCP2515_REG_RXBnEID8(x) (0x63 + (x) * 0x10)
#define MCP2515_REG_RXBnEID0(x) (0x64 + (x) * 0x10)
#define MCP2515_REG_RXBnDLC(x) (0x65 + (x) * 0x10)
#define MCP2515_REG_RXBnDm(x, y) (0x66 + (x) * 0x10 + (y))
#define MCP2515_REG_RXFnSIDH(x) (0x00 + (x) * 0x04)
#define MCP2515_REG_RXFnSIDL(x) (0x01 + (x) * 0x04)
#define MCP2515_REG_RXFnEID8(x) (0x02 + (x) * 0x04)
#define MCP2515_REG_RXFnEID0(x) (0x03 + (x) * 0x04)
#define MCP2515_REG_RXMnSIDH(x) (0x20 + (x) * 0x04)
#define MCP2515_REG_RXMnSIDL(x) (0x21 + (x) * 0x04)
#define MCP2515_REG_RXMnEID8(x) (0x22 + (x) * 0x04)
#define MCP2515_REG_RXMnEID0(x) (0x23 + (x) * 0x04)

#define MCP2515_OST_DELAY_MS 5

static void mcp2515_reset(struct mcp2515_handle *handle) {
    spi_transaction_t trans = {
        .flags = SPI_TRANS_USE_TXDATA,
        .tx_data = { MCP2515_CMD_RESET },
        .length = 8 * 1
    };
    
    vTaskDelay(MCP2515_OST_DELAY_MS / portTICK_PERIOD_MS);
    assert(spi_device_polling_transmit(handle->spi, &trans) == ESP_OK);
    vTaskDelay(MCP2515_OST_DELAY_MS / portTICK_PERIOD_MS);
}

static void mcp2515_read(struct mcp2515_handle *handle, uint8_t address, uint8_t *data) {
    spi_transaction_t trans = {
        .flags = SPI_TRANS_USE_TXDATA,
        .tx_data = { MCP2515_CMD_READ, address },
        .length = 8 * 2,
        .rx_buffer = data,
        .rxlength = 8 * 1
    };

    assert(spi_device_polling_transmit(handle->spi, &trans) == ESP_OK);
}

static void mcp2515_write(struct mcp2515_handle *handle, uint8_t address, uint8_t data) {
    spi_transaction_t trans = {
        .flags = SPI_TRANS_USE_TXDATA,
        .tx_data = { MCP2515_CMD_WRITE, address, data },
        .length = 8 * 3
    };

    assert(spi_device_polling_transmit(handle->spi, &trans) == ESP_OK);
}

int mcp2515_init(struct mcp2515_handle *handle, spi_host_device_t spi_host, int cs) {   
    memset(handle, 0, sizeof(struct mcp2515_handle));

    spi_device_interface_config_t spi_dev_iface_cfg = {
        .mode = 0,
        .clock_speed_hz = 1000, // TODO: Change to higher speed when possible
        .spics_io_num = cs,
        .queue_size = 1,
        .flags = SPI_DEVICE_HALFDUPLEX
    };

    assert(spi_bus_add_device(spi_host, &spi_dev_iface_cfg, &handle->spi) == ESP_OK);

    mcp2515_reset(handle);

    while (1) {
        uint8_t canstat;
        mcp2515_read(handle, MCP2515_REG_CANSTAT, &canstat);
        if ((canstat & MCP2515_CANSTAT_OPMOD_MASK) == MCP2515_CANSTAT_OPMOD_CONFIGURATION)
            break;
        printf("Waiting for MCP2515 to enter configuration mode (0x%02x)\n", canstat);
        vTaskDelay(MCP2515_OST_DELAY_MS / portTICK_PERIOD_MS);
    }

    return 0;
}