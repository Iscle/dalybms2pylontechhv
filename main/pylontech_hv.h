#pragma once

#include <stdint.h>

// basic status
#define PYLONTECH_BASIC_STATUS_FLAG_SLEEP 0x00
#define PYLONTECH_BASIC_STATUS_FLAG_CHARGE 0x01
#define PYLONTECH_BASIC_STATUS_FLAG_DISCHARGE 0x02
#define PYLONTECH_BASIC_STATUS_FLAG_IDLE 0x03

#define PYLONTECH_BASIC_STATUS_FLAG_FORCE_CHARGE 0x08
#define PYLONTECH_BASIC_STATUS_FLAG_FORCE_BALANCE 0x10

// fault
// todo

// alarm
// todo

// extended fault
// todo

int pylontech_hv_0x4210(uint8_t *buf, float battery_voltage, float battery_current, float temp, uint8_t soc, uint8_t soh);

int pylontech_hv_0x4220(uint8_t *buf, float charge_cutoff_voltage, float discharge_cutoff_voltage, float max_charge_current, float max_discharge_current);

int pylontech_hv_0x4230(uint8_t *buf, float max_battery_cell_voltage, float min_battery_cell_voltage, uint16_t max_battery_cell_voltage_number, uint16_t min_battery_cell_voltage_number);

int pylontech_hv_0x4240(uint8_t *buf, float max_battery_cell_temp, float min_battery_cell_temp, uint16_t max_battery_cell_temp_number, uint16_t min_battery_cell_temp_number);

int pylontech_hv_0x4250(uint8_t *buf, uint8_t basic_status, uint8_t cycle_period, uint8_t fault, uint16_t alarm, uint16_t protection);

int pylontech_hv_0x4260(uint8_t *buf, float module_max_voltage, float module_min_voltage, uint16_t module_max_voltage_number, uint16_t module_min_voltage_number);

int pylontech_hv_0x4270(uint8_t *buf, float module_max_temp, float module_min_temp, uint16_t module_max_temp_number, uint16_t module_min_temp_number);

int pylontech_hv_0x4280(uint8_t *buf, uint8_t charge_forbidden, uint8_t discharge_forbidden);

int pylontech_hv_0x4290(uint8_t *buf, uint8_t fault_extension);

// todo: implement 0x42A0

int pylontech_hv_0x7310(uint8_t *buf);

int pylontech_hv_0x7320(uint8_t *buf, uint16_t battery_module_count, uint8_t battery_module_in_series, uint8_t battery_cell_count_in_module, uint16_t voltage_level, uint16_t capacity_level);

int pylontech_hv_0x7330(uint8_t *buf);

int pylontech_hv_0x7340(uint8_t *buf);