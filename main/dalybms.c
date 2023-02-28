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

#include "dalybms.h"
#include "ringbuf.h"
#include "log.h"
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static uart_port_t _uart_num;

static uint8_t dalybms_checksum(const uint8_t *buf) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < 4 + buf[3]; i++) {
        checksum += buf[i];
    }
    return checksum;
}

static void printbuf(uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (i != 0) {
            printf(", 0x%02X", buf[i]);
        } else {
            printf("{0x%02X", buf[i]);
        }
    }
    if (len > 0) printf("}");
}

void dalybms_receive(void *arg) {
    LOGI("dalybms: Starting receive task...\n");

    ringbuf_t rbuf;
    ringbuf_init(&rbuf);

    while (1) {
        uint8_t buf[32];
        int len = uart_read_bytes(_uart_num, buf, sizeof(buf), 100 / portTICK_PERIOD_MS);
        if (len < 0) {
            LOGE("dalybms: Something went wrong while retrieving data!\n");
            continue;
        }

        ringbuf_push(&rbuf, buf, len);

        while (ringbuf_available(&rbuf) >= sizeof(struct dalybms_message)) {
            struct dalybms_message msg;
            ringbuf_peek(&rbuf, (uint8_t *) &msg, sizeof(struct dalybms_message));

            if (msg.magic != DALYBMS_MAGIC) {
                // Not the magic value we're expecting, discard it
                LOGW("dalybms: Discarding byte because of wrong magic!\n");
                ringbuf_discard(&rbuf, 1);
                continue;
            }

            if (msg.checksum != dalybms_checksum((uint8_t *) &msg)) {
                // Wrong checksum, discard first byte
                LOGW("dalybms: Discarding byte because of wrong checksum!\n");
                ringbuf_discard(&rbuf, 1);
                continue;
            }

            // Discard the bytes of this message
            ringbuf_discard(&rbuf, sizeof(struct dalybms_message));
            
            LOGI("dalybms: Address: %d, ID: 0x%02X, Length: %d, Data: ", msg.address, msg.id, msg.len);
            printbuf(msg.data, msg.len);
            printf("\n");
        }
    }
}

static int dalybms_send(struct dalybms_message *msg) {
    msg->magic = DALYBMS_MAGIC;
    if (msg->address < 0x40) msg->address += 0x40 - 1; // Address 1 is 0x40
    msg->len = 8;
    msg->checksum = dalybms_checksum((uint8_t *) msg);
    return uart_write_bytes(_uart_num, msg, sizeof(struct dalybms_message)) == sizeof(struct dalybms_message);
}

void dalybms_test(void) {    
    for (uint8_t i = 0x90; i <= 0x98; i++) {
        struct dalybms_message msg = {
            .address = 1,
            .id = i,
        };
        dalybms_send(&msg);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void dalybms_init(uart_port_t uart_num, int ro, int re_de, int di) {
    _uart_num = uart_num;

    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_DEFAULT
    };

    assert(uart_driver_install(_uart_num, UART_FIFO_LEN * 2, 0, 0, NULL, 0) == ESP_OK);
    assert(uart_param_config(_uart_num, &uart_config) == ESP_OK);
    assert(uart_set_pin(_uart_num, di, ro, re_de, UART_PIN_NO_CHANGE) == ESP_OK);
    assert(uart_set_mode(_uart_num, UART_MODE_RS485_HALF_DUPLEX) == ESP_OK);

    xTaskCreate(dalybms_receive, NULL, 2048, NULL, 5, NULL);
}