/*
 *      @file   max17320.h
 *      @brief  Header file of MAX17320G2 Driver
 *      @author Saksham Ahuja
 *      Datasheet: https://www.analog.com/en/products/max17320.html
 * 
 */


#ifndef MAX17320_H
#define MAX17320_H


// Include Files
#include "stm32u5xx_hal.h"


// MAX17320G2 Device Address
#define MAX17320_DEV_ADDR               0x6C // For internal memory range 0x000 - 0x0FF


// MAX17320G2 Data Registers
#define MAX17320_REG_STATUS             0x000 
#define MAX17320_REG_DEV_NAME           0x021 


// Other MACROS
#define MAX17320_TIMEOUT                100
#define MAX17320_DATA_SIZE              2



// Device Struct
typedef struct __MAX17320_HandleTypeDef {

    I2C_HandleTypeDef *i2c_handler;

    uint8_t status[2];


} MAX17320_HandleTypeDef;



HAL_StatusTypeDef max17320_init(MAX17320_HandleTypeDef *dev, I2C_HandleTypeDef *hi2c_device);

#endif // MAX17320_H


