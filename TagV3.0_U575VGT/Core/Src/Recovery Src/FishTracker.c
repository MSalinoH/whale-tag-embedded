/*
 * FishTracker.c
 *
 *  Created on: Aug 16, 2023
 *      Author: Kaveet
 */

#include "Recovery Inc/FishTracker.h"
#include "Recovery Inc/VHF.h"
#include <stdint.h>
#include "constants.h"
#include "main.h"
#include <stdbool.h>

//For our DAC values
static void calcFishtrackerDacValues();
uint32_t fishtracker_dac_input[FISHTRACKER_NUM_DAC_SAMPLES] = {0};

//Extern variables
extern DAC_HandleTypeDef hdac1;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart4;

void fishtracker_thread_entry(ULONG thread_input){

	//Initialization - find out our DAC values and the timer period
	calcFishtrackerDacValues();

	uint8_t timerPeriod = (TIM2_SCALED_FREQ / FISHTRACKER_NUM_DAC_SAMPLES /FISHTRACKER_INPUT_FREQ_HZ);

	//Reinitialize timer
	//MX_TIM2_Fake_Init(timerPeriod);

	//Initialize VHF module for transmission. Turn transmission off so we don't hog the frequency
	initialize_vhf(huart4, false, FISHTRACKER_CARRIER_FREQ_MHZ, FISHTRACKER_CARRIER_FREQ_MHZ);
	set_ptt(false);

	set_ptt(true);
	while (1) {

		//Start our DAC and our timer to trigger the conversion edges

		HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, fishtracker_dac_input, FISHTRACKER_NUM_DAC_SAMPLES, DAC_ALIGN_8B_R);
		HAL_TIM_Base_Start(&htim2);
		//HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_8B_R, 255);

		tx_thread_sleep(FISHTRACKER_ON_TIME_TICKS);

		//Stop DAC and timer
		HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
		HAL_TIM_Base_Stop(&htim2);
		//HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_8B_R, 0);

		tx_thread_sleep(FISHTRACKER_OFF_TIME_TICKS);
	}
}


static void calcFishtrackerDacValues(){

	for (uint8_t i = 0; i < FISHTRACKER_NUM_DAC_SAMPLES; i++){

		//Formula taken from STM32 documentation online on sine wave generation.
		//Generates a sine wave with a min of 0V and a max of the reference voltage.
		fishtracker_dac_input[i] = ((sin(i * 2 * PI/FISHTRACKER_NUM_DAC_SAMPLES) + 1) * (256/2));
		//fishtracker_dac_input[i] = 255;
	}

}
