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

#define DALYBMS_MAGIC 0xA5

static uart_port_t _uart_num;
static struct dalybms_data _data[DALYBMS_MAX_BMS];

static void dalybms_parse_0x90(const struct dalybms_message *msg, int32_t *pack_voltage, int32_t *samp_total_volt, int32_t *pack_current, uint16_t *pack_soc) {
    *pack_voltage = ((msg->data[0]) << 8 | (msg->data[1])) * 100;
    *samp_total_volt = ((msg->data[2]) << 8 | (msg->data[3])) * 100;
    *pack_current = (((msg->data[4]) << 8 | (msg->data[5])) - 30000) * 100;
    *pack_soc = ((msg->data[6]) << 8 | (msg->data[7]));
}

static void dalybms_parse_0x91(const struct dalybms_message *msg, int32_t *max_cell_voltage, uint8_t *max_cell_voltage_id, int32_t *min_cell_voltage, uint8_t *min_cell_voltage_id) {
    *max_cell_voltage = (msg->data[0]) << 8 | (msg->data[1]);
    *max_cell_voltage_id = msg->data[2];
    *min_cell_voltage = (msg->data[3]) << 8 | (msg->data[4]);
    *min_cell_voltage_id = msg->data[5];
}

static void dalybms_parse_0x92(const struct dalybms_message *msg, int16_t *max_temperature, uint8_t *max_temperature_id, int16_t *min_temperature, uint8_t *min_temperature_id) {
    *max_temperature = (msg->data[0] - 40) * 10;
    *max_temperature_id = msg->data[1];
    *min_temperature = (msg->data[2] - 40) * 10;
    *min_temperature_id = msg->data[3];
}

static void dalybms_parse_0x93(const struct dalybms_message *msg, uint8_t *charge_discharge_status, uint8_t *charge_fet_status, uint8_t *discharge_fet_status, uint8_t *bms_life, int32_t *residual_capacity) {
    *charge_discharge_status = msg->data[0];
    *charge_fet_status = msg->data[1];
    *discharge_fet_status = msg->data[2];
    *bms_life = msg->data[3];
    *residual_capacity = (msg->data[4]) << 24 | (msg->data[5]) << 16 | (msg->data[6]) << 8 | (msg->data[7]);
}

static void dalybms_parse_0x94(const struct dalybms_message *msg, uint8_t *cell_count, uint8_t *temp_sensor_count, uint8_t *charge_status, uint8_t *load_status, uint8_t *dio, uint16_t *cycle_count) {
    *cell_count = msg->data[0];
    *temp_sensor_count = msg->data[1];
    *charge_status = msg->data[2];
    *load_status = msg->data[3];
    *dio = msg->data[4];
    *cycle_count = (msg->data[5]) << 8 | (msg->data[6]);
}

static uint8_t dalybms_parse_0x95(const struct dalybms_message *msg, int32_t *cell_voltages) {
    cell_voltages[msg->data[0] * 3 + 0] = (msg->data[1]) << 8 | (msg->data[2]);
    cell_voltages[msg->data[0] * 3 + 1] = (msg->data[3]) << 8 | (msg->data[4]);
    cell_voltages[msg->data[0] * 3 + 2] = (msg->data[5]) << 8 | (msg->data[6]);
    return msg->data[0];
}

static uint8_t dalybms_parse_0x96(const struct dalybms_message *msg, int16_t *temperatures) {
    temperatures[msg->data[0] * 3 + 0] = (msg->data[1] - 40) * 10;
    temperatures[msg->data[0] * 3 + 1] = (msg->data[2] - 40) * 10;
    temperatures[msg->data[0] * 3 + 2] = (msg->data[3] - 40) * 10;
    temperatures[msg->data[0] * 3 + 3] = (msg->data[4] - 40) * 10;
    temperatures[msg->data[0] * 3 + 4] = (msg->data[5] - 40) * 10;
    temperatures[msg->data[0] * 3 + 5] = (msg->data[6] - 40) * 10;
    temperatures[msg->data[0] * 3 + 6] = (msg->data[7] - 40) * 10;
    return msg->data[0];
}

static void dalybms_parse_0x97(const struct dalybms_message *msg, uint8_t *balance_states) {
    balance_states[0] = msg->data[0];
    balance_states[1] = msg->data[1];
    balance_states[2] = msg->data[2];
    balance_states[3] = msg->data[3];
    balance_states[4] = msg->data[4];
    balance_states[5] = msg->data[5];
    balance_states[6] = msg->data[6];
    balance_states[7] = msg->data[7];
}

static void dalybms_parse_0x98(const struct dalybms_message *msg, uint8_t *errors) {
    errors[0] = msg->data[0];
    errors[1] = msg->data[1];
    errors[2] = msg->data[2];
    errors[3] = msg->data[3];
    errors[4] = msg->data[4];
    errors[5] = msg->data[5];
    errors[6] = msg->data[6];
    errors[7] = msg->data[7];
}

static void dalybms_handle_message(struct dalybms_message *msg) {
    struct dalybms_data *data = &_data[msg->address - 1];

    switch (msg->id) {
        case 0x90: {
            dalybms_parse_0x90(msg, &data->pack_voltage, &data->samp_total_volt, &data->pack_current, &data->pack_soc);
            LOGI("dalybms: 0x90: pack_voltage=%ld, samp_total_volt=%ld, pack_current=%ld, pack_soc=%d\n", data->pack_voltage, data->samp_total_volt, data->pack_current, data->pack_soc);
            break;
        }
        case 0x91: {
            dalybms_parse_0x91(msg, &data->max_cell_voltage, &data->max_cell_voltage_id, &data->min_cell_voltage, &data->min_cell_voltage_id);
            LOGI("dalybms: 0x91: max_cell_voltage=%ld, max_cell_voltage_id=%d, min_cell_voltage=%ld, min_cell_voltage_id=%d\n", data->max_cell_voltage, data->max_cell_voltage_id, data->min_cell_voltage, data->min_cell_voltage_id);
            break;
        }
        case 0x92: {
            dalybms_parse_0x92(msg, &data->max_temperature, &data->max_temperature_id, &data->min_temperature, &data->min_temperature_id);
            LOGI("dalybms: 0x92: max_temperature=%d, max_temperature_id=%d, min_temperature=%d, min_temperature_id=%d\n", data->max_temperature, data->max_temperature_id, data->min_temperature, data->min_temperature_id);
            break;
        }
        case 0x93: {
            dalybms_parse_0x93(msg, &data->charge_discharge_status, &data->charge_fet_status, &data->discharge_fet_status, &data->bms_life, &data->residual_capacity);
            LOGI("dalybms: 0x93: charge_discharge_status=%d, charge_fet_status=%d, discharge_fet_status=%d, bms_life=%d, residual_capacity=%ld\n", data->charge_discharge_status, data->charge_fet_status, data->discharge_fet_status, data->bms_life, data->residual_capacity);
            break;
        }
        case 0x94: {
            dalybms_parse_0x94(msg, &data->cell_count, &data->temp_sensor_count, &data->charge_status, &data->load_status, &data->dio, &data->cycle_count);
            LOGI("dalybms: 0x94: cell_count=%d, temp_sensor_count=%d, charge_status=%d, load_status=%d, dio=%d, cycle_count=%d\n", data->cell_count, data->temp_sensor_count, data->charge_status, data->load_status, data->dio, data->cycle_count);
            break;
        }
        case 0x95: {
            uint8_t iteration = dalybms_parse_0x95(msg, data->cell_voltages);
            LOGI("dalybms: 0x95: iteration=%d\n", iteration);
            break;
        }
        case 0x96: {
            uint8_t iteration = dalybms_parse_0x96(msg, data->temperatures);
            LOGI("dalybms: 0x96: iteration=%d\n", iteration);
            break;
        }
        case 0x97: {
            dalybms_parse_0x97(msg, data->balance_states);
            break;
        }
        case 0x98: {
            dalybms_parse_0x98(msg, data->errors);
            break;
        }
    }

    LOGI("dalybms: Handled message 0x%02X from %d successfully\n", msg->id, msg->address);
}

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

static int dalybms_send(struct dalybms_message *msg) {
    msg->magic = DALYBMS_MAGIC;
    if (msg->address < 0x40) msg->address += 0x40 - 1; // Address 1 is 0x40
    msg->len = 8;
    msg->checksum = dalybms_checksum((uint8_t *) msg);
    return uart_write_bytes(_uart_num, msg, sizeof(struct dalybms_message)) == sizeof(struct dalybms_message);
}

static void dalybms_task_function(void *arg) {
    LOGI("dalybms: Starting receive task...\n");

    ringbuf_t rbuf;
    ringbuf_init(&rbuf);

    uint8_t next_id = 0x90;
    uint8_t next_address = 1;
    while (1) {
        if (next_id > 0x98) {
            next_id = 0x90;
            next_address++;
        }

        if (next_address > DALYBMS_MAX_BMS) {
            next_address = 1;
        }

        // Send a message to the BMS
        struct dalybms_message out_msg = {
                .address = next_address,
                .id = next_id,
        };
        dalybms_send(&out_msg);

        // Increment the id for the next message
        next_id++;

        // Read the response
        uint8_t buf[32];
        int len = uart_read_bytes(_uart_num, buf, sizeof(buf), 500 / portTICK_PERIOD_MS);
        if (len < 0) {
            LOGE("dalybms: Something went wrong while retrieving data!\n");
            continue;
        }

        // Push the bytes to the ring buffer
        ringbuf_push(&rbuf, buf, len);

        // Check if we have enough bytes to read a message
        while (ringbuf_available(&rbuf) >= sizeof(struct dalybms_message)) {
            struct dalybms_message in_msg;
            ringbuf_peek(&rbuf, (uint8_t *) &in_msg, sizeof(struct dalybms_message));

            if (in_msg.magic != DALYBMS_MAGIC) {
                // Not the magic value we're expecting, discard it
                LOGW("dalybms: Discarding byte because of wrong magic!\n");
                ringbuf_discard(&rbuf, 1);
                continue;
            }

            if (in_msg.checksum != dalybms_checksum((uint8_t *) &in_msg)) {
                // Wrong checksum, discard first byte
                LOGW("dalybms: Discarding byte because of wrong checksum!\n");
                ringbuf_discard(&rbuf, 1);
                continue;
            }

            // Discard the bytes of this message
            ringbuf_discard(&rbuf, sizeof(struct dalybms_message));
            
            LOGI("dalybms: Address: %d, ID: 0x%02X, Length: %d, Data: ", in_msg.address, in_msg.id, in_msg.len);
            printbuf(in_msg.data, in_msg.len);
            printf("\n");

            dalybms_handle_message(&in_msg);
        }
    }
}

struct dalybms_data *dalybms_get_data(void) {
    return _data;
}

void dalybms_init(uart_port_t uart_num, gpio_num_t ro, gpio_num_t re_de, gpio_num_t di) {
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

    xTaskCreate(dalybms_task_function, "dalybms_task_function", 2048, NULL, 5, NULL);
}