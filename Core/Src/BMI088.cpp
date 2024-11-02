//
// Created by 81301 on 2024/11/2.
//
#include "BMI088.h"

void BMI088_init() {
    // switch accelerometer to SPI mode
    // dummy read ACC_CHIP_ID
    BMI088_accel_read_reg(ACC_CHIP_ID);
    // enter normal mode
    //BMI088_accel_write_reg(ACC_PWR_CONF, 0x00);
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
    uint8_t rxdata = BMI088_exchange_byte(DUMMY_BYTE);
    BMI088_cs_accel_h();
    return rxdata;
}

uint8_t BMI088_gyro_read_reg(uint8_t reg) {
    BMI088_cs_gyro_l();
    // send reg address
    BMI088_exchange_byte(reg | 0x80);
    // read data
    uint8_t rxdata = BMI088_exchange_byte(DUMMY_BYTE);
    BMI088_cs_gyro_h();
    return rxdata;
}

// burst read
void BMI088_accel_burst_read(uint8_t reg, uint8_t rxdata[], uint8_t size) {
    BMI088_cs_accel_l();
    // send reg address
    BMI088_exchange_byte(reg | 0x80);
    // read dummy byte
    BMI088_exchange_byte(DUMMY_BYTE);
    // read data
    for (int i = 0; i < size; i++) {
        rxdata[i] = BMI088_exchange_byte(DUMMY_BYTE);
    }
    BMI088_cs_accel_h();
}

void BMI088_gyro_burst_read(uint8_t reg, uint8_t rxdata[], uint8_t size) {
    BMI088_cs_gyro_l();
    // send reg address
    BMI088_exchange_byte(reg | 0x80);
    // read data
    for (int i = 0; i < size; i++) {
        rxdata[i] = BMI088_exchange_byte(DUMMY_BYTE);
    }
    BMI088_cs_gyro_h();
}

// get data
void BMI088_get_accel_data(int16_t* accel_x, int16_t* accel_y, int16_t* accel_z) {
    uint8_t rx_buffer[6];
    BMI088_accel_burst_read(ACC_DATA, rx_buffer, 6);
    *accel_x = rx_buffer[1] << 8 | rx_buffer[0];
    *accel_y = rx_buffer[3] << 8 | rx_buffer[2];
    *accel_z = rx_buffer[5] << 8 | rx_buffer[4];
}

void BMI088_get_gyro_data(int16_t* rate_x, int16_t* rate_y, int16_t* rate_z) {
    uint8_t rx_buffer[6];
    BMI088_gyro_burst_read(RATE_DATA, rx_buffer, 6);
    *rate_x = rx_buffer[1] << 8 | rx_buffer[0];
    *rate_y = rx_buffer[3] << 8 | rx_buffer[2];
    *rate_z = rx_buffer[5] << 8 | rx_buffer[4];
}
