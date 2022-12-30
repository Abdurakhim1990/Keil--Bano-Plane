#include "led.h"
#include "tim.h"

uint16_t my_pwm = 0;//
uint16_t my_adc = 0;//

uint16_t period_strobe = PERIOD_STROBE;

void SetLedPWM(uint16_t adc_value)
{
	static uint8_t last_pwm = 0;
	static uint8_t first_pass = 0;
	
	if(!first_pass)
	{
		last_pwm = GetFirstPWM();
		++first_pass;
	}
	
	if(adc_value > GetThresholdADC() + ADC_DELTA)
	{
		if(last_pwm > 0)
		{
			--last_pwm;
		}
	} else if(adc_value < GetThresholdADC() - ADC_DELTA)
	{
		if(last_pwm < 100)
		{
			++last_pwm;
		}
	}
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, last_pwm);
	
	my_pwm = last_pwm; //
	my_adc = adc_value; //
}


void StrobeLed(void)
{
	static uint8_t count_signal = 0;
	
	switch(count_signal)
	{
		case 0: __HAL_TIM_SET_AUTORELOAD(&htim1, PERIOD_STROBE - 200);
						++count_signal;
						break;
		case 1: HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
						__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
						HAL_TIM_Base_Start_IT(&htim16);
						__HAL_TIM_ENABLE_IT(&htim16, TIM_IT_UPDATE);
#ifndef BANO_TALL
						if(GetModeBano() == MODE_BANO_LEFT)
						{
							__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC2);
							__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, PERIOD_STROBE/2);
						}
#endif
						++count_signal;
						break;
		case 2: HAL_GPIO_WritePin(Strobe_GPIO_Port, Strobe_Pin, GPIO_PIN_SET);
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, STOP_STROBE_1);
						__HAL_TIM_SET_AUTORELOAD(&htim1, period_strobe);
						++count_signal;
						break;
		case 3: HAL_GPIO_WritePin(Strobe_GPIO_Port, Strobe_Pin, GPIO_PIN_RESET);
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, START_STROBE_2);
						++count_signal;
						break;
		case 4: HAL_GPIO_WritePin(Strobe_GPIO_Port, Strobe_Pin, GPIO_PIN_SET);
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, STOP_STROBE_2);
						++count_signal;
						break;
		case 5: HAL_GPIO_WritePin(Strobe_GPIO_Port, Strobe_Pin, GPIO_PIN_RESET);
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, START_STROBE_1);
						count_signal = 2;
						break;
	}
}

uint8_t GetModeBano(void)
{
	return HAL_GPIO_ReadPin(Mode_GPIO_Port, Mode_Pin) == GPIO_PIN_RESET;
}

uint8_t GetFirstPWM(void)				// Получить начальный ШИМ
{
#ifdef BANO_TALL
	return PWM_WHITE_TAIL;
#else
	return (GetModeBano() == MODE_BANO_LEFT ? PWM_RED_LEFT : PWM_GREEN_RIGHT);
#endif
}

uint16_t GetThresholdADC(void)	 // Получить пороговое напряжение
{
#ifdef BANO_TALL
	return ADC_WHITE_TAIL;
#else
	return (GetModeBano() == MODE_BANO_LEFT ? ADC_RED_LEFT : ADC_GREEN_RIGHT);
#endif
}

void Synchronize(void)
{
	if(HAL_GPIO_ReadPin(InSynch_GPIO_Port, InSynch_Pin) == GPIO_PIN_RESET)
	{
		__HAL_TIM_SET_COUNTER(&htim1, 0);
	}
}
