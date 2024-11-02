//
// Created by 81301 on 2024/11/2.
//

#ifndef BMI088_H
#define BMI088_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio.h"
#include "spi.h"
#include "stm32f4xx.h"

#define BMI088_TIMEOUT 1000
#define DUMMY_BYTE 0xFF

// register addresses
// accel
#define ACC_CHIP_ID 0x00
#define ACC_DATA 0x12
#define ACC_CONF 0x40
#define ACC_RANGE 0x41
#define ACC_PWR_CONF 0x7C
#define ACC_PWR_CTRL 0x7D

// gyro
#define GYRO_CHIP_ID 0x00
#define RATE_DATA 0x02
#define GYRO_BANDWIDTH 0x10
#define GYRO_RANGE 0x0F

// configure IMU
void BMI088_init();

// Chip Select
// LOW to select
void BMI088_cs_accel_h();
void BMI088_cs_accel_l();

void BMI088_cs_gyro_h();
void BMI088_cs_gyro_l();

// exchange one byte
uint8_t BMI088_exchange_byte(uint8_t txdata);

// write
void BMI088_accel_write_reg(uint8_t reg, uint8_t data);
void BMI088_gyro_write_reg(uint8_t reg, uint8_t data);

// read
uint8_t BMI088_accel_read_reg(uint8_t reg);
uint8_t BMI088_gyro_read_reg(uint8_t reg);

// read several bytes (burst read)
void BMI088_accel_burst_read(uint8_t reg, uint8_t rxdata[], uint8_t size);
void BMI088_gyro_burst_read(uint8_t reg, uint8_t rxdata[], uint8_t size);

// get data
void BMI088_get_accel_data(int16_t* accel_x, int16_t* accel_y, int16_t* accel_z);
void BMI088_get_gyro_data(int16_t* rate_x, int16_t* rate_y, int16_t* rate_z);

#ifdef __cplusplus
}
#endif

#endif //BMI088_H
