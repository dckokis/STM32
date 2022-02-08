#include "stm32f0xx.h"
#include "SPI.h"

const uint8_t update_time = 10; // in millisecs
const uint16_t scanTime = 100; //in millisecs time of scan

uint8_t READ = 0;
uint32_t TIME = 0;

uint32_t max_voltage = 1100;
uint32_t cur_voltage = 0;
uint32_t voltage_arr[8];
uint32_t prev_col = 4;

void SysTickInit(void);
void SysTick_Handler(void);
void SPI2_IRQHandler(void);
void initADC(void);
void updateVoltage(void);
uint32_t getPattern(void);

void SysTickInit()
{	
 	SystemCoreClockUpdate();
	SysTick->LOAD = SystemCoreClock/100 - 1;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk; // the last bit means make on interrupt system
}

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

void SysTick_Handler()
{
	TIME += 1;
	if(TIME == (scanTime/update_time))
	{
		TIME = 0;
		READ = 1;
	}
}

uint32_t getPattern()
{
	uint32_t cur_col = (prev_col + 1) == 8 ? 0 : (prev_col + 1);
	uint32_t pattern = 0;
	pattern = bitSet(pattern,8 + cur_col);
	uint32_t line_to_show = voltage_arr[cur_col] * 8 / max_voltage;
	line_to_show = 7 - line_to_show;
	pattern = bitSet(pattern,line_to_show);
	return pattern;
}

void updateVoltage()
{
	for(int i = 0; i < 7; ++i) {
		voltage_arr[7-i] = voltage_arr[6-i];
	}
	voltage_arr[0] = cur_voltage;
}

void SPI2_IRQHandler(void)
{
	if((SPI2->SR && SPI_SR_RXNE) == SPI_SR_RXNE) 
	{
		GPIOA->BSRR = GPIO_BSRR_BS_8;
		SPI2->DR;
		prev_col = ((prev_col + 1) == 8) ? 0 : (prev_col + 1);
		SPI2->DR = getPattern();
		GPIOA->BSRR = GPIO_BSRR_BR_8;
	}
}

int main(void)
{
	initSPI();
	SysTickInit();
	initADC();
	
	while(1)
	{
		if(READ)
		{
			while ((ADC1->ISR & ADC_ISR_EOC) == 0);
			cur_voltage = ADC1->DR;
			updateVoltage();
			READ = 0;
		}
	}
}
