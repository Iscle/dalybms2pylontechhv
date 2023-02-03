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

#include <stdio.h>
#include <driver/spi_master.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "mcp2515.h"

static struct mcp2515_handle inverter_can;
static struct mcp2515_handle bms_can[3];

static int spi_init(spi_host_device_t spi_host, int mosi, int miso, int sclk) {
    int ret;
    
    spi_bus_config_t spi_bus_cfg = {
        .mosi_io_num = mosi,
        .miso_io_num = miso,
        .sclk_io_num = sclk,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

    ret = spi_bus_initialize(spi_host, &spi_bus_cfg, SPI_DMA_DISABLED);
    if (ret != ESP_OK) {
        printf("spi_bus_initialize failed: %d\n", ret);
        return ret;
    }

    return 0;
}

static void inverter_can_bus_init(void) {
    int ret;

    ret = spi_init(SPI3_HOST, 23, 19, 18);
    if (ret) {
        printf("Failed to initialize inverter SPI bus: %d\n", ret);
        assert(0);
    }

    ret = mcp2515_init(&inverter_can, SPI3_HOST, 5);
    if (ret) {
        printf("Failed to initialize inverter CAN bus: %d\n", ret);
        assert(0);
    }
}

static void bms_can_bus_init(void) {
    int ret;

    ret = spi_init(SPI2_HOST, 13, 12, 14);
    if (ret) {
        printf("Failed to initialize BMS SPI bus: %d\n", ret);
        assert(0);
    }

    // ret = mcp2515_init(&bms_can[0], SPI2_HOST, -1);
    // if (ret) {
    //     printf("Failed to initialize BMS 1 can bus: %d\n", ret);
    //     assert(0);
    // }
    // ret = mcp2515_init(&bms_can[1], SPI2_HOST, -1);
    // if (ret) {
    //     printf("Failed to initialize BMS 2 can bus: %d\n", ret);
    //     assert(0);
    // }
    // ret = mcp2515_init(&bms_can[2], SPI2_HOST, -1);
    // if (ret) {
    //     printf("Failed to initialize BMS 3 can bus: %d\n", ret);
    //     assert(0);
    // }
}

void app_main(void) {
    inverter_can_bus_init();
    printf("Inverter CAN bus initialized successfully\n");

    bms_can_bus_init();
    printf("BMS CAN buses initialized successfully\n");

    while (1) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
