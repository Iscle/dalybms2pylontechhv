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
#define MCP2515_REG_TXB0CTRL 0x30
#define MCP2515_REG_TXB1CTRL 0x40
#define MCP2515_REG_TXB2CTRL 0x50
#define MCP2515_REG_RXB0CTRL 0x60
#define MCP2515_REG_RXB1CTRL 0x70

// CAN registers
#define MCP2515_REG_RXF0SIDH 0x00
#define MCP2515_REG_RXF0SIDL 0x01
#define MCP2515_REG_RXF0EID8 0x02
#define MCP2515_REG_RXF0EID0 0x03
#define MCP2515_REG_RXF1SIDH 0x04
#define MCP2515_REG_RXF1SIDL 0x05
#define MCP2515_REG_RXF1EID8 0x06
#define MCP2515_REG_RXF1EID0 0x07
#define MCP2515_REG_RXF2SIDH 0x08
#define MCP2515_REG_RXF2SIDL 0x09
#define MCP2515_REG_RXF2EID8 0x0A
#define MCP2515_REG_RXF2EID0 0x0B
#define MCP2515_REG_BFPCTRL 0x0C
#define MCP2515_REG_TXRTSCTRL 0x0D
#define MCP2515_REG_CANSTAT 0x0E
#define MCP2515_REG_CANCTRL 0x0F
#define MCP2515_REG_RXF3SIDH 0x10
#define MCP2515_REG_RXF3SIDL 0x11
#define MCP2515_REG_RXF3EID8 0x12
#define MCP2515_REG_RXF3EID0 0x13
#define MCP2515_REG_RXF4SIDH 0x14
#define MCP2515_REG_RXF4SIDL 0x15
#define MCP2515_REG_RXF4EID8 0x16
#define MCP2515_REG_RXF4EID0 0x17
#define MCP2515_REG_RXF5SIDH 0x18
#define MCP2515_REG_RXF5SIDL 0x19
#define MCP2515_REG_RXF5EID8 0x1A
#define MCP2515_REG_RXF5EID0 0x1B
#define MCP2515_REG_TEC 0x1C
#define MCP2515_REG_REC 0x1D
#define MCP2515_REG_RXM0SIDH 0x20
#define MCP2515_REG_RXM0SIDL 0x21
#define MCP2515_REG_RXM0EID8 0x22
#define MCP2515_REG_RXM0EID0 0x23
#define MCP2515_REG_RXM1SIDH 0x24
#define MCP2515_REG_RXM1SIDL 0x25
#define MCP2515_REG_RXM1EID8 0x26
#define MCP2515_REG_RXM1EID0 0x27
#define MCP2515_REG_CNF3 0x28
#define MCP2515_REG_CNF2 0x29
#define MCP2515_REG_CNF1 0x2A
#define MCP2515_REG_CANINTE 0x2B
#define MCP2515_REG_CANINTF 0x2C
#define MCP2515_REG_EFLG 0x2D
#define MCP2515_REG_TXB0CTRL 0x30
#define MCP2515_REG_TXB0SIDH 0x31
#define MCP2515_REG_TXB0SIDL 0x32
#define MCP2515_REG_TXB0EID8 0x33
#define MCP2515_REG_TXB0EID0 0x34
#define MCP2515_REG_TXB0DLC 0x35
#define MCP2515_REG_TXB0D0 0x36
#define MCP2515_REG_TXB0D1 0x37
#define MCP2515_REG_TXB0D2 0x38
#define MCP2515_REG_TXB0D3 0x39
#define MCP2515_REG_TXB0D4 0x3A
#define MCP2515_REG_TXB0D5 0x3B
#define MCP2515_REG_TXB0D6 0x3C
#define MCP2515_REG_TXB0D7 0x3D
#define MCP2515_REG_TXB1CTRL 0x40
#define MCP2515_REG_TXB1SIDH 0x41
#define MCP2515_REG_TXB1SIDL 0x42
#define MCP2515_REG_TXB1EID8 0x43
#define MCP2515_REG_TXB1EID0 0x44
#define MCP2515_REG_TXB1DLC 0x45
#define MCP2515_REG_TXB1D0 0x46
#define MCP2515_REG_TXB1D1 0x47
#define MCP2515_REG_TXB1D2 0x48
#define MCP2515_REG_TXB1D3 0x49
#define MCP2515_REG_TXB1D4 0x4A
#define MCP2515_REG_TXB1D5 0x4B
#define MCP2515_REG_TXB1D6 0x4C
#define MCP2515_REG_TXB1D7 0x4D
#define MCP2515_REG_TXB2CTRL 0x50
#define MCP2515_REG_TXB2SIDH 0x51
#define MCP2515_REG_TXB2SIDL 0x52
#define MCP2515_REG_TXB2EID8 0x53
#define MCP2515_REG_TXB2EID0 0x54
#define MCP2515_REG_TXB2DLC 0x55
#define MCP2515_REG_TXB2D0 0x56
#define MCP2515_REG_TXB2D1 0x57
#define MCP2515_REG_TXB2D2 0x58
#define MCP2515_REG_TXB2D3 0x59
#define MCP2515_REG_TXB2D4 0x5A
#define MCP2515_REG_TXB2D5 0x5B
#define MCP2515_REG_TXB2D6 0x5C
#define MCP2515_REG_TXB2D7 0x5D
#define MCP2515_REG_RXB0CTRL 0x60
#define MCP2515_REG_RXB0SIDH 0x61
#define MCP2515_REG_RXB0SIDL 0x62
#define MCP2515_REG_RXB0EID8 0x63
#define MCP2515_REG_RXB0EID0 0x64
#define MCP2515_REG_RXB0DLC 0x65
#define MCP2515_REG_RXB0D0 0x66
#define MCP2515_REG_RXB0D1 0x67
#define MCP2515_REG_RXB0D2 0x68
#define MCP2515_REG_RXB0D3 0x69
#define MCP2515_REG_RXB0D4 0x6A
#define MCP2515_REG_RXB0D5 0x6B
#define MCP2515_REG_RXB0D6 0x6C
#define MCP2515_REG_RXB0D7 0x6D
#define MCP2515_REG_RXB1CTRL 0x70
#define MCP2515_REG_RXB1SIDH 0x71
#define MCP2515_REG_RXB1SIDL 0x72
#define MCP2515_REG_RXB1EID8 0x73
#define MCP2515_REG_RXB1EID0 0x74
#define MCP2515_REG_RXB1DLC 0x75
#define MCP2515_REG_RXB1D0 0x76
#define MCP2515_REG_RXB1D1 0x77
#define MCP2515_REG_RXB1D2 0x78
#define MCP2515_REG_RXB1D3 0x79
#define MCP2515_REG_RXB1D4 0x7A
#define MCP2515_REG_RXB1D5 0x7B
#define MCP2515_REG_RXB1D6 0x7C
#define MCP2515_REG_RXB1D7 0x7D

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