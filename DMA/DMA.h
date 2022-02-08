#pragma once
#include "stm32f0xx.h"

const uint32_t DMA_buf_size = 10;
uint16_t DMA_buf[DMA_buf_size];
enum PART {
	FIRST,
	SECOND
};

uint8_t part_for_analysis = FIRST;

void initDMA(void);
void initDMA(){
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	ADC1->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG | ADC_CFGR1_OVRMOD;
	DMA1_Channel1->CPAR = (uint32_t) (&(ADC1->DR)); 
	DMA1_Channel1->CMAR = (uint32_t)(DMA_buf); 
	DMA1_Channel1->CNDTR = DMA_buf_size; 
	DMA1_Channel1->CCR |= DMA_CCR_MINC | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0| DMA_CCR_TCIE | DMA_CCR_CIRC | DMA_CCR_HTIE; 
	DMA1_Channel1->CCR |= DMA_CCR_EN;
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	NVIC_SetPriority(DMA1_Channel1_IRQn, 0);
}
