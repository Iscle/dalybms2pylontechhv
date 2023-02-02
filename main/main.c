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
