#pragma once
#include <stm32f0xx.h>

void initSPI(void);
uint32_t bitSet(uint32_t value, uint32_t position);

void initSPI() {	
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	GPIOB->AFR[1] = 0 << (15 - 8) * 4;
	GPIOB->AFR[1] = 0 << (13 - 8) * 4;
	SPI2->CR1 = 
		  SPI_CR1_SSM 
		| SPI_CR1_SSI 
		| SPI_CR1_BR 
		| SPI_CR1_MSTR 
		| SPI_CR1_CPOL 
		| SPI_CR1_CPHA;
	GPIOB->MODER |= GPIO_MODER_MODER13_1 | GPIO_MODER_MODER15_1;
	GPIOA->MODER |= GPIO_MODER_MODER8_0;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR13_1 | GPIO_PUPDR_PUPDR15_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR8_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1; 
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_1;
	GPIOB->AFR[1] |= (0 << (4 * (13 - 8))) | (0 << (4 * (15 - 8)));
	SPI2->CR2 = SPI_CR2_DS | SPI_CR2_RXNEIE;
	NVIC_EnableIRQ(SPI2_IRQn);
	SPI2->CR1 |= SPI_CR1_SPE;
	SPI2->DR = 0;
}

uint32_t bitSet(uint32_t value, uint32_t position)
{
	return (value | (1 << position));
}