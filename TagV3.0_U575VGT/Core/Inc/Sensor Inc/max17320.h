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
#include <stdbool.h>
#include <util.h>


// MAX17320G2 Device Address
#define MAX17320_DEV_ADDR               0x6C // For internal memory range 0x000 - 0x0FF


// MAX17320G2 Data Registers
#define MAX17320_REG_STATUS             0x000 
#define MAX17320_REG_DEV_NAME           0x021 


// Other MACROS
#define MAX17320_TIMEOUT                100
#define MAX17320_DATA_SIZE              2

// 8-bit to 16-bit conversion
#define TO_16_BIT(b1, b2)				((uint16_t)(b2 << 8) | (uint16_t)b1)


// Register Structs

typedef struct {
	bool power_on_reset;
	bool min_current_alert;
	bool max_current_alert;
	bool state_of_charge_change_alert; // Alerts 1% change
	bool min_voltage_alert;
	bool min_temp_alert;
	bool min_state_of_charge_alert;
	bool max_voltage_alert;
	bool max_temp_alert;
	bool max_state_of_charge_alert;
	bool protection_alert;

} max17320_Reg_Status;



// Device Struct
typedef struct __MAX17320_HandleTypeDef {

    I2C_HandleTypeDef *i2c_handler;


    max17320_Reg_Status status;

} MAX17320_HandleTypeDef;



HAL_StatusTypeDef max17320_init(MAX17320_HandleTypeDef *dev, I2C_HandleTypeDef *hi2c_device);

HAL_StatusTypeDef max17320_get_status(MAX17320_HandleTypeDef *dev);



#endif // MAX17320_H


