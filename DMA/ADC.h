#pragma once
#include "stm32f0xx.h"

void initADC(void);
void initADC()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER1;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->CR |= ADC_CR_ADCAL;
	while (ADC1->CR & ADC_CR_ADCAL) {}
	ADC1->CR |= ADC_CR_ADEN;
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0);
	ADC1->CHSELR |= ADC_CHSELR_CHSEL1;					
	ADC1->CFGR1 |= ADC_CFGR1_RES_0; 
	ADC1->CFGR1 |= ADC_CFGR1_CONT;
	ADC1->CR |= ADC_CR_ADSTART;
}
