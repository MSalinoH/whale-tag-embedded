/*
 *      @file   max17320.c
 *      @brief  Implementation of MAX17320G2 Driver
 *      @author Saksham Ahuja
 * 
 */

// Include files
#include "max17320.h"

HAL_StatusTypeDef max17320_init(MAX17320_HandleTypeDef *dev, I2C_HandleTypeDef *hi2c_device) {
    HAL_StatusTypeDef ret = HAL_ERROR;
    dev->i2c_handler = hi2c_device;

    uint8_t raw_data[2] = {0};

    // Read the status register and check for correct value of 0x0002
    ret = HAL_I2C_Mem_Read(dev->i2c_handler, 0x6C, 0x34, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&raw_data, 2, 1000);

    return ret;
}
