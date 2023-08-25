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

HAL_StatusTypeDef max17320_clear_write_protection(MAX17320_HandleTypeDef *dev) {

	uint8_t data_buf[2] = {0};

	HAL_StatusTypeDef ret = HAL_I2C_Mem_Write(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_COMM_STAT, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	ret = HAL_I2C_Mem_Write(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_COMM_STAT, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	return ret;
}

HAL_StatusTypeDef max17320_get_status(MAX17320_HandleTypeDef *dev) {

    uint8_t data_buf[2] = {0};
    
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_STATUS, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

    dev->status = __statusRegister_from_raw(TO_16_BIT(data_buf[0], data_buf[1]));

	return ret;
}

HAL_StatusTypeDef max17320_get_remaining_capacity(MAX17320_HandleTypeDef *dev) {

	uint8_t data_buf[2] = {0};

	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_REP_CAPACITY, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	dev->remaining_capacity = TO_16_BIT(data_buf[0], data_buf[1]) * CAPACITY_LSB/R_SENSE_VAL;

	return ret;
}

HAL_StatusTypeDef max17320_get_state_of_charge(MAX17320_HandleTypeDef *dev) {

	uint8_t data_buf[2] = {0};

	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_REP_SOC, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	dev->state_of_charge = TO_16_BIT(data_buf[0], data_buf[1]) * PERCENTAGE_LSB;

	return ret;
}

HAL_StatusTypeDef max17320_get_voltages(MAX17320_HandleTypeDef *dev) {

	uint8_t data_buf[2] = {0};

	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_CELL1_VOLTAGE, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	dev->cell_1_voltage = TO_16_BIT(data_buf[0], data_buf[1]) * CELL_VOLTAGE_LSB;

	ret = HAL_I2C_Mem_Read(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_CELL2_VOLTAGE, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	dev->cell_2_voltage = TO_16_BIT(data_buf[0], data_buf[1]) * CELL_VOLTAGE_LSB;

	ret = HAL_I2C_Mem_Read(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_TOTAL_BAT_VOLTAGE, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	dev->total_battery_voltage = TO_16_BIT(data_buf[0], data_buf[1]) * PACK_VOLTAGE_LSB;

	ret = HAL_I2C_Mem_Read(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_PACK_SIDE_VOLTAGE, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	dev->pack_side_voltage = TO_16_BIT(data_buf[0], data_buf[1]) * PACK_VOLTAGE_LSB;

 	return ret;
}

HAL_StatusTypeDef max17320_get_temperature(MAX17320_HandleTypeDef *dev) {

	uint8_t data_buf[2] = {0};

	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_TEMPERATURE, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	dev->temperature = TO_16_BIT(data_buf[0], data_buf[1]) * TEMPERATURE_LSB;

	return ret;
}

// Reads the instantaneous current of the battery
HAL_StatusTypeDef max17320_get_battery_current(MAX17320_HandleTypeDef *dev) {

	uint8_t data_buf[2] = {0};

	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_BATT_CURRENT, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	int16_t x = TO_16_BIT(data_buf[0], data_buf[1]);

	dev->battery_current = x * CURRENT_LSB/R_SENSE_VAL;

	return ret;
}

HAL_StatusTypeDef max17320_get_average_battery_current(MAX17320_HandleTypeDef *dev) {

	uint8_t data_buf[2] = {0};

	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_AVG_BATT_CURRENT, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	int16_t x = TO_16_BIT(data_buf[0], data_buf[1]);

	dev->average_current = x * CURRENT_LSB/R_SENSE_VAL;

	return ret;
}

HAL_StatusTypeDef max17320_get_time_to_empty(MAX17320_HandleTypeDef *dev) {

	uint8_t data_buf[2] = {0};

	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_TIME_TO_EMPTY, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	dev->time_to_empty = TO_16_BIT(data_buf[0], data_buf[1]) * TIME_LSB/SECOND_TO_HOUR;

	return ret;
}

HAL_StatusTypeDef max17320_get_time_to_full(MAX17320_HandleTypeDef *dev) {

	uint8_t data_buf[2] = {0};

	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_TIME_TO_FULL, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	dev->time_to_full = TO_16_BIT(data_buf[0], data_buf[1]) * TIME_LSB/SECOND_TO_HOUR;

	return ret;
}

HAL_StatusTypeDef max17320_set_alert_thresholds(MAX17320_HandleTypeDef *dev) {

	uint8_t data_buf[2] = {0};

	data_buf[0] = MAX17320_MIN_VOLTAGE_THR / VOLTAGE_ALT_LSB;
	data_buf[1] = MAX17320_MAX_VOLTAGE_THR / VOLTAGE_ALT_LSB;

	HAL_StatusTypeDef ret = HAL_I2C_Mem_Write(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_VOLTAGE_ALT_THR, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	ret = HAL_I2C_Mem_Read(dev->i2c_handler, MAX17320_DEV_ADDR, MAX17320_REG_VOLTAGE_ALT_THR, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&data_buf, 2, MAX17320_TIMEOUT);

	return ret;
}

HAL_StatusTypeDef max17320_configure_cell_balancing(MAX17320_HandleTypeDef *dev) {

	return 0;
}
