#ifndef LED_H
#define LED_H

#include "main.h"
#include "stm32f0xx_hal.h"


//----- Временные интервалы СТРОБ импульса
#define START_STROBE_1		0
#define STOP_STROBE_1			START_STROBE_1 + 80
#define START_STROBE_2		STOP_STROBE_1 + 50
#define STOP_STROBE_2			START_STROBE_2 + 80
#define PERIOD_STROBE			1500
#define PAUSE_BEGIN				1500

//----- Вариант исполнения БАНО
#define MODE_BANO_LEFT		0
#define MODE_BANO_RIGHT		!MODE_BANO_LEFT

//----- Начальный процент ШИМ на светодиоды
#define PWM_GREEN_RIGHT		68		// 2.6*3+0.35 = 8.15 B == 68%
#define PWM_RED_LEFT			56		// 1.6*4+0.35 = 6.75 B == 56%
#define PWM_WHITE_TAIL		49		// 2.8*2+0.35 = 5.95 B == 49%

//----- Пороговое значение тока светодиода
#define ADC_GREEN_RIGHT		434		// Значение ADC для 350 мА * 1 Ом = 350 мВ (= 434)
#define ADC_RED_LEFT			434		// Значение ADC для 350 мА * 1 Ом = 350 мВ (= 434)
#define ADC_WHITE_TAIL		434		// Значение ADC для 350 мА * 1 Ом = 350 мВ (= 434)
#define ADC_DELTA					20

void SetLedPWM(uint16_t value);
void StrobeLed(void);
uint8_t GetModeBano(void);
uint8_t GetFirstPWM(void);
uint16_t GetThresholdADC(void);
void Synchronize(void);
#endif
