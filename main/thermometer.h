#pragma once

#include <stdint.h>

int16_t thermometer_get_temp(void);
int16_t thermometer_get_max_cell_temp(void);
uint8_t thermometer_get_max_cell_temp_id(void);
int16_t thermometer_get_min_cell_temp(void);
uint8_t thermometer_get_min_cell_temp_id(void);
