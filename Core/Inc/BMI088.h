//
// Created by 81301 on 2024/11/2.
//

#ifndef BMI088_H
#define BMI088_H

#include "gpio.h"
#include "spi.h"
#include "stm32f4xx.h"

#define BMI088_TIMEOUT 1000
#define DUMMY_BYTE 0xFF

// register addresses
// accel
#define ACC_CHIP_ID 0x00
#define ACC_CONF 0x40
#define ACC_RANGE 0x41
#define ACC_PWR_CTRL 0x7D

// gyro
#define GYRO_CHIP_ID 0x00
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

#endif //BMI088_H
