#ifndef _RS485_H
#define _RS485_H

// Note that the Waveshare ESP32-Relay-6CH board derives the DE/RE signals from the TX signal (see 74HC04D section of schematic)

// Attached sensor is https://www.amazon.com/HiLetgo-Temperature-Precision-Transmitter-Compatible/dp/B082F8WXD8
// From the Amazon.com review written by "Agrimétrie Inc" on 22-Mar-2024:
//     *Works fine but poorly documented*
//     Use function address 0x04 (read input register)
//     register 0x01 (2 bytes) is temperature
//     register 0x02 (2 bytes) is humidity
//     both registers need to be divided by 10 (ex. 215 means 21.5C)
//
// Seems to be better documented here:
// https://github.com/TronixLab/ModBus-RTU-Temperature-Humidity-SHT20-Sensor/tree/main?tab=readme-ov-file#industrial-grade-temperature-and-humidity-sht20-sensor
//
// The doc/XY-MD01-Temperature-and-Humidity-Transmitter-SHT20-High-Precision-With-Shell-Cable-1m-RS485-1.pdf was downloaded from:
// https://robu.in/wp-content/uploads/2023/11/XY-MD01-Temperature-and-Humidity-Transmitter-SHT20-High-Precision-With-Shell-Cable-1m-RS485-1.pdf

void rs485_init();
void rs485_task();
int get_relative_humidity_percent_x10();
int get_temperature_degC_x10();

#endif //_RS485_H
