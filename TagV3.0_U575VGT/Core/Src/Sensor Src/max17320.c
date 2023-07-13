/*
 *      @file   max17320.c
 *      @brief  Implementation of MAX17320G2 Driver
 *      @author Saksham Ahuja
 * 
 */

// Include files

#include "max17320.h"

static inline max17320_Reg_Status __statusRegister_from_raw(uint16_t raw) {
	return (max17320_Reg_Status) {
        .power_on_reset = _RSHIFT(raw, 1, 1),
	    .min_current_alert = _RSHIFT(raw, 2, 1),
	    .max_current_alert = _RSHIFT(raw, 6, 1),
	    .state_of_charge_change_alert = _RSHIFT(raw, 7, 1),
	    .min_voltage_alert = _RSHIFT(raw, 8, 1),
	    .min_temp_alert = _RSHIFT(raw, 9, 1),
	    .min_state_of_charge_alert = _RSHIFT(raw, 10, 1),
	    .max_voltage_alert = _RSHIFT(raw, 12, 1),
	    .max_temp_alert = _RSHIFT(raw, 13, 1),
	    .max_state_of_charge_alert = _RSHIFT(raw, 14, 1),
	    .protection_alert = _RSHIFT(raw, 15, 1),
	};
}

HAL_StatusTypeDef max17320_init(MAX17320_HandleTypeDef *dev, I2C_HandleTypeDef *hi2c_device) {
    HAL_StatusTypeDef ret = HAL_ERROR;
    dev->i2c_handler = hi2c_device;

   // Check status
   ret = max17320_get_status(dev);

   return ret;
}

HAL_StatusTypeDef max17320_get_status(MAX17320_HandleTypeDef *dev) {

    uint8_t data_buf[2] = {0};
    
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_STATUS, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, 1000);

    dev->status = __statusRegister_from_raw(TO_16_BIT(data_buf[0], data_buf[1]));

	return ret;
}
