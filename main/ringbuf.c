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

#include "ringbuf.h"
#include <string.h>

void ringbuf_init(ringbuf_t *rb) {
    memset(rb->buf, 0x69, RINGBUF_SIZE);
    rb->read_pos = 0;
    rb->write_pos = 0;
}

void ringbuf_push(ringbuf_t *rb, const uint8_t *buf, size_t len) {
    if (rb->write_pos + len <= RINGBUF_SIZE) {
        memcpy(rb->buf + rb->write_pos, buf, len);
    } else {
        size_t leftover = rb->write_pos + len - RINGBUF_SIZE;
        memcpy(rb->buf + rb->write_pos, buf, len - leftover);
        memcpy(rb->buf, buf + len - leftover, leftover);
    }

    rb->write_pos = (rb->write_pos + len) % RINGBUF_SIZE;
}

void ringbuf_peek(ringbuf_t *rb, uint8_t *buf, size_t len) {
    if (rb->read_pos + len <= RINGBUF_SIZE) {
        memcpy(buf, rb->buf + rb->read_pos, len);
    } else {
        size_t leftover = rb->read_pos + len - RINGBUF_SIZE;
        memcpy(buf, rb->buf + rb->read_pos, len - leftover);
        memcpy(buf + len - leftover, rb->buf, leftover);
    }
}

void ringbuf_discard(ringbuf_t *rb, size_t len) {
    rb->read_pos = (rb->read_pos + len) % RINGBUF_SIZE;
}

size_t ringbuf_available(ringbuf_t *rb) {
    if (rb->read_pos <= rb->write_pos) {
        return rb->write_pos - rb->read_pos;
    } else {
        return RINGBUF_SIZE - rb->read_pos + rb->write_pos;
    }
}