#include "pylontech_hv.h"

int pylontech_hv_0x4210(uint8_t *buf, float battery_voltage, float battery_current, float temp, uint8_t soc, uint8_t soh) {
    // battery voltage
    uint16_t pylontech_battery_voltage = (uint16_t) (battery_voltage * 10);
    buf[0] = (uint8_t) (pylontech_battery_voltage & 0xFF);
    buf[1] = (uint8_t) ((pylontech_battery_voltage >> 8) & 0xFF);

    // battery current
    uint16_t pylontech_battery_current = (uint16_t) ((battery_current + 3000) * 10);
    buf[2] = (uint8_t) (pylontech_battery_current & 0xFF);
    buf[3] = (uint8_t) ((pylontech_battery_current >> 8) & 0xFF);

    // temp
    uint16_t pylontech_temp = (uint16_t) ((temp + 100) * 10);
    buf[4] = (uint8_t) (pylontech_temp & 0xFF);
    buf[5] = (uint8_t) ((pylontech_temp >> 8) & 0xFF);

    // soc
    buf[6] = soc;

    // soh
    buf[7] = soh;

    return 0;
}

int pylontech_hv_0x4220(uint8_t *buf, float charge_cutoff_voltage, float discharge_cutoff_voltage, float max_charge_current, float max_discharge_current) {
    // charge cutoff voltage
    uint16_t pylontech_charge_cutoff_voltage = (uint16_t) (charge_cutoff_voltage * 10);
    buf[0] = (uint8_t) (pylontech_charge_cutoff_voltage & 0xFF);
    buf[1] = (uint8_t) ((pylontech_charge_cutoff_voltage >> 8) & 0xFF);

    // discharge cutoff voltage
    uint16_t pylontech_discharge_cutoff_voltage = (uint16_t) (discharge_cutoff_voltage * 10);
    buf[2] = (uint8_t) (pylontech_discharge_cutoff_voltage & 0xFF);
    buf[3] = (uint8_t) ((pylontech_discharge_cutoff_voltage >> 8) & 0xFF);

    // max charge current
    uint16_t pylontech_max_charge_current = (uint16_t) ((max_charge_current + 3000) * 10);
    buf[4] = (uint8_t) (pylontech_max_charge_current & 0xFF);
    buf[5] = (uint8_t) ((pylontech_max_charge_current >> 8) & 0xFF);

    // max discharge current
    uint16_t pylontech_max_discharge_current = (uint16_t) ((max_discharge_current + 3000) * 10);
    buf[6] = (uint8_t) (pylontech_max_discharge_current & 0xFF);
    buf[7] = (uint8_t) ((pylontech_max_discharge_current >> 8) & 0xFF);

    return 0;
}

int pylontech_hv_0x4230(uint8_t *buf, float max_battery_cell_voltage, float min_battery_cell_voltage, uint16_t max_battery_cell_voltage_number, uint16_t min_battery_cell_voltage_number) {
    // max battery cell voltage
    uint16_t pylontech_max_battery_cell_voltage = (uint16_t) (max_battery_cell_voltage * 1000);
    buf[0] = (uint8_t) (pylontech_max_battery_cell_voltage & 0xFF);
    buf[1] = (uint8_t) ((pylontech_max_battery_cell_voltage >> 8) & 0xFF);

    // min battery cell voltage
    uint16_t pylontech_min_battery_cell_voltage = (uint16_t) (min_battery_cell_voltage * 1000);
    buf[2] = (uint8_t) (pylontech_min_battery_cell_voltage & 0xFF);
    buf[3] = (uint8_t) ((pylontech_min_battery_cell_voltage >> 8) & 0xFF);

    // max battery cell voltage number
    buf[4] = (uint8_t) (max_battery_cell_voltage_number & 0xFF);
    buf[5] = (uint8_t) ((max_battery_cell_voltage_number >> 8) & 0xFF);

    // min battery cell voltage number
    buf[6] = (uint8_t) (min_battery_cell_voltage_number & 0xFF);
    buf[7] = (uint8_t) ((min_battery_cell_voltage_number >> 8) & 0xFF);

    return 0;
}

int pylontech_hv_0x4240(uint8_t *buf, float max_battery_cell_temp, float min_battery_cell_temp, uint16_t max_battery_cell_temp_number, uint16_t min_battery_cell_temp_number) {
    // max battery cell temp
    uint16_t pylontech_max_battery_cell_temp = (uint16_t) ((max_battery_cell_temp + 100) * 10);
    buf[0] = (uint8_t) (pylontech_max_battery_cell_temp & 0xFF);
    buf[1] = (uint8_t) ((pylontech_max_battery_cell_temp >> 8) & 0xFF);

    // min battery cell temp
    uint16_t pylontech_min_battery_cell_temp = (uint16_t) ((min_battery_cell_temp + 100) * 10);
    buf[2] = (uint8_t) (pylontech_min_battery_cell_temp & 0xFF);
    buf[3] = (uint8_t) ((pylontech_min_battery_cell_temp >> 8) & 0xFF);

    // max battery cell temp number
    buf[4] = (uint8_t) (max_battery_cell_temp_number & 0xFF);
    buf[5] = (uint8_t) ((max_battery_cell_temp_number >> 8) & 0xFF);

    // min battery cell temp number
    buf[6] = (uint8_t) (min_battery_cell_temp_number & 0xFF);
    buf[7] = (uint8_t) ((min_battery_cell_temp_number >> 8) & 0xFF);

    return 0;
}

int pylontech_hv_0x4250(uint8_t *buf, uint8_t basic_status, uint8_t cycle_period, uint8_t fault, uint16_t alarm, uint16_t protection) {
    // basic status
    buf[0] = basic_status;

    // cycle period
    buf[1] = cycle_period;

    // fault
    buf[2] = fault;

    // alarm
    buf[3] = (uint8_t) (alarm & 0xFF);
    buf[4] = (uint8_t) ((alarm >> 8) & 0xFF);

    // protection
    buf[5] = (uint8_t) (protection & 0xFF);
    buf[6] = (uint8_t) ((protection >> 8) & 0xFF);

    return 0;
}

int pylontech_hv_0x4260(uint8_t *buf, float module_max_voltage, float module_min_voltage, uint16_t module_max_voltage_number, uint16_t module_min_voltage_number) {
    // module max voltage
    uint16_t pylontech_module_max_voltage = (uint16_t) (module_max_voltage * 1000);
    buf[0] = (uint8_t) (pylontech_module_max_voltage & 0xFF);
    buf[1] = (uint8_t) ((pylontech_module_max_voltage >> 8) & 0xFF);

    // module min voltage
    uint16_t pylontech_module_min_voltage = (uint16_t) (module_min_voltage * 1000);
    buf[2] = (uint8_t) (pylontech_module_min_voltage & 0xFF);
    buf[3] = (uint8_t) ((pylontech_module_min_voltage >> 8) & 0xFF);

    // module max voltage number
    buf[4] = (uint8_t) (module_max_voltage_number & 0xFF);
    buf[5] = (uint8_t) ((module_max_voltage_number >> 8) & 0xFF);

    // module min voltage number
    buf[6] = (uint8_t) (module_min_voltage_number & 0xFF);
    buf[7] = (uint8_t) ((module_min_voltage_number >> 8) & 0xFF);

    return 0;
}

int pylontech_hv_0x4270(uint8_t *buf, float module_max_temp, float module_min_temp, uint16_t module_max_temp_number, uint16_t module_min_temp_number) {
    // module max temp
    uint16_t pylontech_module_max_temp = (uint16_t) ((module_max_temp + 100) * 10);
    buf[0] = (uint8_t) (pylontech_module_max_temp & 0xFF);
    buf[1] = (uint8_t) ((pylontech_module_max_temp >> 8) & 0xFF);

    // module min temp
    uint16_t pylontech_module_min_temp = (uint16_t) ((module_min_temp + 100) * 10);
    buf[2] = (uint8_t) (pylontech_module_min_temp & 0xFF);
    buf[3] = (uint8_t) ((pylontech_module_min_temp >> 8) & 0xFF);

    // module max temp number
    buf[4] = (uint8_t) (module_max_temp_number & 0xFF);
    buf[5] = (uint8_t) ((module_max_temp_number >> 8) & 0xFF);

    // module min temp number
    buf[6] = (uint8_t) (module_min_temp_number & 0xFF);
    buf[7] = (uint8_t) ((module_min_temp_number >> 8) & 0xFF);

    return 0;
}

int pylontech_hv_0x4280(uint8_t *buf, uint8_t charge_forbidden, uint8_t discharge_forbidden) {
    // charge forbidden
    buf[0] = charge_forbidden ? 0xAA : 0x00;

    // discharge forbidden
    buf[1] = discharge_forbidden ? 0xAA : 0x00;

    // reserved
    buf[2] = 0x00;
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x00;
    buf[7] = 0x00;

    return 0;
}

int pylontech_hv_0x4290(uint8_t *buf, uint8_t fault_extension) {
    // fault extension
    buf[0] = fault_extension;

    // reserved
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x00;
    buf[7] = 0x00;

    return 0;
}

// todo: implement 0x42A0

int pylontech_hv_0x7310(uint8_t *buf) {
    // hardware version
    buf[0] = 0x01;

    // reserved
    buf[1] = 0x00;

    // hardware version v
    buf[2] = 0x10;

    // hardware version r
    buf[3] = 0x02;

    // software version v major
    buf[4] = 0x04;

    // software version v minor
    buf[5] = 0x05;

    // software version
    buf[6] = 0x34;

    // software version
    buf[7] = 0x0C;

    return 0;
}

int pylontech_hv_0x7320(uint8_t *buf, uint16_t battery_module_count, uint8_t battery_module_in_series, uint8_t battery_cell_count_in_module, uint16_t voltage_level, uint16_t capacity_level) {
    // battery module count
    buf[0] = (uint8_t) (battery_module_count & 0xFF);
    buf[1] = (uint8_t) ((battery_module_count >> 8) & 0xFF);

    // battery module in series
    buf[2] = battery_module_in_series;

    // battery cell count in module
    buf[3] = battery_cell_count_in_module;

    // voltage level
    buf[4] = (uint8_t) (voltage_level & 0xFF);
    buf[5] = (uint8_t) ((voltage_level >> 8) & 0xFF);

    // capacity level
    buf[6] = (uint8_t) (capacity_level & 0xFF);
    buf[7] = (uint8_t) ((capacity_level >> 8) & 0xFF);

    return 0;
}

int pylontech_hv_0x7330(uint8_t *buf) {
    buf[0] = 'P';
    buf[1] = 'Y';
    buf[2] = 'L';
    buf[3] = 'O';
    buf[4] = 'N';
    buf[5] = 'T';
    buf[6] = 'E';
    buf[7] = 'C';

    return 0;
}

int pylontech_hv_0x7340(uint8_t *buf) {
    buf[0] = 'H';
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x00;
    buf[7] = 0x00;

    return 0;
}