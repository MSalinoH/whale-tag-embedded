/*
 * sys_monitor.c
 *
 *  Created on: Sept. 20, 2023
 *     Authors: Michael Salino-Hugg (msalinohugg@seas.harvard.edu)
 * Description: 
 *      Project CETI whale tag 
 */

#include "sys_monitor.h"
#include "main.h"
#include "util.h"

extern ADC_HandleTypeDef hadc1;

uint16_t power_voltages[2];

static HAL_StatusTypeDef priv_update_adc_values(void){
	HAL_RESULT_PROPAGATE(HAL_ADC_Start(&hadc1));
	HAL_RESULT_PROPAGATE(HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY));
	power_voltages[0] = HAL_ADC_GetValue(&hadc1);
	HAL_RESULT_PROPAGATE(HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY));
	power_voltages[1] = HAL_ADC_GetValue(&hadc1);
	return HAL_OK;
}

uint16_t systemMonitor_pos5v_read_raw(void){
	priv_update_adc_values();
	return power_voltages[0];
}

float systemMonitor_pos5v_read(void){
    uint16_t raw = systemMonitor_pos5v_read_raw();
    float adc_voltage = ADC_TO_VOLTAGE(raw);
    float line_voltage = POS5_UNSCALE_VOLTAGE(adc_voltage);
    return line_voltage;
}

uint16_t systemMonitor_neg5v_read_raw(void){
	priv_update_adc_values();
	return power_voltages[1];
}

float systemMonitor_neg5v_read(void){
    uint16_t raw = systemMonitor_neg5v_read_raw();
    float adc_voltage = ADC_TO_VOLTAGE(raw);
    float line_voltage = NEG5_UNSCALE_VOLTAGE(adc_voltage);
    return line_voltage;
}
