#include "stm32f0xx.h"
#include "SPI.h"
#include "ADC.h"
#include "DMA.h"

const uint8_t update_time = 10; // in millisecs
const uint16_t scanTime = 100; //in millisecs time of scan

uint8_t READ = 0;
uint8_t DMA_READY = 0;
uint32_t TIME = 0;

uint32_t max_voltage = 1100;
uint32_t cur_voltage = 0;
uint32_t voltage_arr[8];
uint32_t prev_col = 4;


void SysTickInit(void);
void SysTick_Handler(void);
void SPI2_IRQHandler(void);
void DMA1_Channel1_IRQHandler(void);
void updateVoltage(void);
uint32_t getPattern(void);

void SysTickInit()
{	
 	SystemCoreClockUpdate();
	SysTick->LOAD = SystemCoreClock/100 - 1;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk; // the last bit means make on interrupt system
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

void DMA1_Channel1_IRQHandler()
{
	if(DMA1->ISR & DMA_ISR_HTIF1)
	{
		part_for_analysis = SECOND;
		DMA_READY = 1;
	}
	else if(DMA1->ISR & DMA_ISR_TCIF1)
	{
		part_for_analysis = FIRST;
		DMA_READY = 1;
	}
	DMA1->IFCR |= DMA_IFCR_CGIF1;
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

int main(void)
{
	initSPI();
	SysTickInit();
	initADC();
	initDMA();
	while(1)
	{
		if(READ)
		{
			updateVoltage();
			READ = 0;
		}
		if(DMA_READY)
		{
			uint32_t sum = 0;
			switch (part_for_analysis) {
				case FIRST:
					for (uint16_t i = (DMA_buf_size/2); i < DMA_buf_size; ++i){
						sum = (sum + DMA_buf[i]);
					}
					break;
				case SECOND:
					for (uint16_t i = 0; i < DMA_buf_size/2; ++i){
						sum = (sum + DMA_buf[i]);
					}
					break;
			}
			cur_voltage = (sum/(DMA_buf_size/2));
			DMA_READY = 0;
		}
	}
}
