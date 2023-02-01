#include <stdio.h>
#include <driver/spi_master.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "mcp2515.h"

static struct mcp2515_handle inverter_can;
static struct mcp2515_handle bms1_can;
static struct mcp2515_handle bms2_can;
static struct mcp2515_handle bms3_can;

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

void app_main(void) {
    int ret;

    ret = spi_init(SPI2_HOST, 13, 12, 14);
    if (ret) {
        printf("Failed to initialize SPI2 bus: %d\n", ret);
    }

    ret = spi_init(SPI3_HOST, 23, 19, 18);
    if (ret) {
        printf("Failed to initialize SPI3 bus: %d\n", ret);
    }

    printf("SPI buses initialized successfully\n");

    ret = mcp2515_init(&inverter_can, SPI3_HOST, 5);
    if (ret) {
        printf("Failed to initialize inverter CAN bus: %d\n", ret);
    }

    printf("Inverter CAN bus initialized successfully\n");

    // ret = mcp2515_init(&bms1_can, SPI2_HOST, -1);
    // if (ret) {
    //     printf("Failed to initialize BMS 1 can bus: %d\n", ret);
    // }
    // ret = mcp2515_init(&bms2_can, SPI2_HOST, -1);
    // if (ret) {
    //     printf("Failed to initialize BMS 2 can bus: %d\n", ret);
    // }
    // ret = mcp2515_init(&bms3_can, SPI2_HOST, -1);
    // if (ret) {
    //     printf("Failed to initialize BMS 3 can bus: %d\n", ret);
    // }

    // printf("BMS CAN buses initialized successfully\n");

    while (1) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
