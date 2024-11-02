//
// Created by 81301 on 2024/11/2.
//

#include "BMI088.h"
#include "tim.h"

uint8_t id, cnt=0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim == &htim1) {
        id = BMI088_accel_read_reg(ACC_CONF);
        cnt++;
    }
}
