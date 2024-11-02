//
// Created by 81301 on 2024/11/2.
//
#include "BMI088.h"

void BMI088_init() {
    // switch accelerometer to SPI mode
    // dummy read ACC_CHIP_ID
    BMI088_accel_read_reg(ACC_CHIP_ID);
    // enter normal mode
    BMI088_accel_write_reg(ACC_PWR_CTRL, 0x04);

    // configure output data rate
    BMI088_accel_write_reg(ACC_CONF, 0xAB); // 800Hz
    BMI088_gyro_write_reg(GYRO_BANDWIDTH, 0x02); // 1000Hz

    // configure measurement range
    BMI088_accel_write_reg(ACC_RANGE, 0x02); // -12g to 12g
    BMI088_gyro_write_reg(GYRO_RANGE, 0x00); // -2000 deg/s to 2000 deg/s
}

// Chip Select
// LOW to select
void BMI088_cs_accel_h() {
    HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_SET);
}
void BMI088_cs_accel_l() {
    HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_RESET);
}

void BMI088_cs_gyro_h() {
    HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_SET);
}
void BMI088_cs_gyro_l() {
    HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_RESET);
}

// exchange byte
uint8_t BMI088_exchange_byte(uint8_t txdata) {
    uint8_t rxdata;
    HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, BMI088_TIMEOUT);
    return rxdata;
}

// write
void BMI088_accel_write_reg(uint8_t reg, uint8_t data) {
    BMI088_cs_accel_l();
    // send reg address
    BMI088_exchange_byte(reg & 0x7F);
    // send data
    BMI088_exchange_byte(data);
    BMI088_cs_accel_h();
}

void BMI088_gyro_write_reg(uint8_t reg, uint8_t data) {
    BMI088_cs_gyro_l();
    // send reg address
    BMI088_exchange_byte(reg & 0x7F);
    // send data
    BMI088_exchange_byte(data);
    BMI088_cs_gyro_h();
}

// read
uint8_t BMI088_accel_read_reg(uint8_t reg) {
    BMI088_cs_accel_l();
    // send reg address
    BMI088_exchange_byte(reg | 0x80);
    // read dummy byte
    BMI088_exchange_byte(DUMMY_BYTE);
    // read data
    uint8_t rxdata = BMI088_exchange_byte(0xFF);
    BMI088_cs_accel_h();
    return rxdata;
}

uint8_t BMI088_gyro_read_reg(uint8_t reg) {
    BMI088_cs_gyro_l();
    // send reg address
    BMI088_exchange_byte(reg | 0x80);
    // read data
    uint8_t rxdata = BMI088_exchange_byte(0xFF);
    BMI088_cs_gyro_h();
    return rxdata;
}
