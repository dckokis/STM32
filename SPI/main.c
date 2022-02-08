#include <stm32f0xx.h>
#include <stdbool.h>
#include "buttons.h"
#include "SPI.h"

static volatile int x = 1;
static volatile int y = 1;
volatile static uint32_t matrix[8] = {0};

void Init(void);
void Init() {
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN|RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER0;
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;
	GPIOC->MODER &= ~GPIO_MODER_MODER12;
	GPIOC->MODER |=	GPIO_MODER_MODER12_0;
	GPIOA->MODER &= ~GPIO_MODER_MODER4;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_1;
	GPIOA->MODER &= ~GPIO_MODER_MODER5;
	GPIOA->MODER &= ~GPIO_MODER_MODER15;
	GPIOA->MODER |= GPIO_MODER_MODER15_0;
	
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
	SystemCoreClockUpdate();
	SysTick->LOAD = SystemCoreClock / 1000 - 1;//1ms
}

void SysTick_Handler(void);
void SysTick_Handler(void) {	
	ask_buttons_1_3();
	ask_buttons_2_4();
}

void SPI2_IRQHandler(void);
void SPI2_IRQHandler(void) {
	static int i = 0;
	if (SPI2->SR & SPI_SR_RXNE) {
		GPIOA->BSRR = GPIO_BSRR_BS_8;
		SPI2->DR;
		GPIOA->BSRR = GPIO_BSRR_BR_8;
		SPI2->DR = ((matrix[i])) | ((0x1U << i) << 8);
		if(i < 7) { i++; } 
		else { i = 0; }
	}
}

int main(void) { 
	Init();
	initSPI();
	matrix[y - 1] = 1 << x;
	matrix[y] = 1 << x | 1 << (x - 1) | 1 << (x + 1);
	matrix[y + 1] = 1 << x;
	while(1){
		if (keys[UP].state) {
			if(y > 1) {
				matrix[x + 1] = matrix[x + 1] >> 1;
				matrix[x] = matrix[x] >> 1;
				matrix[x - 1] = matrix[x - 1] >> 1;
				--y;
			}
			keys[UP].state = false;
		}
		if (keys[DOWN].state) {
			if(y < 6) {
				matrix[x + 1] = matrix[x + 1] << 1;
				matrix[x] = matrix[x] << 1;
				matrix[x - 1] = matrix[x - 1] << 1;
				++y;
			}
			keys[DOWN].state = false;
		}
		if (keys[LEFT].state) {
			if(x > 1) {
				matrix[x + 1] = 0;
				matrix[x] = 1 << y;
				matrix[x - 1] = 1 << y | 1 << (y-1) | 1 << (y + 1);
				matrix[x - 2] = 1 << y;
				--x;
			}
			keys[LEFT].state = false;
	  }
		if (keys[RIGHT].state) {
			if(x < 6){
				matrix[x + 1] = 1 << y | 1 << (y-1) | 1 << (y + 1);
				matrix[x] = 1 << y;
				matrix[x - 1] = 0;
				matrix[x + 2] = 1 << y;
				++x;
			}
			keys[RIGHT].state = false;
		}
	}
}
