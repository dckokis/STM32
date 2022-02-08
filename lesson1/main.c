#include "stm32f0xx.h"
#define PAUSE 100000

// PC6..PC9 - leds
// PA0 - button
void init_led(void);
void rotate_right(void);
void init_button(void);
void rotate_left(void);

void init_led(void)
{
	RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;
	
	GPIOC -> MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 | GPIO_MODER_MODER9); //bit-clear
	GPIOC -> MODER |= (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0); //bit-set
	
}

void init_button(void) 
{
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA -> MODER &= ~GPIO_MODER_MODER0;
}

void rotate_right(void)
{
	int time = 0;
	
	GPIOC -> ODR |= GPIO_ODR_6;
	for (time = 0; time < PAUSE; time++){}
	GPIOC -> ODR &= ~GPIO_ODR_6;
		
	GPIOC -> ODR |= GPIO_ODR_9;
	for (time = 0; time < PAUSE; time++){}
	GPIOC -> ODR &= ~GPIO_ODR_9;
		
	GPIOC -> ODR |= GPIO_ODR_7;
	for (time = 0; time < PAUSE; time++){}
	GPIOC -> ODR &= ~GPIO_ODR_7;
		
	GPIOC -> ODR |= GPIO_ODR_8;
	for (time = 0; time < PAUSE; time++){}
	GPIOC -> ODR &= ~GPIO_ODR_8;
}	

void rotate_left(void)
{
	int time = 0;
	
	GPIOC -> ODR |= GPIO_ODR_6;
	for (time = 0; time < PAUSE; time++){}
	GPIOC -> ODR &= ~GPIO_ODR_6;
		
	GPIOC -> ODR |= GPIO_ODR_8;
	for (time = 0; time < PAUSE; time++){}
	GPIOC -> ODR &= ~GPIO_ODR_8;
		
	GPIOC -> ODR |= GPIO_ODR_7;
	for (time = 0; time < PAUSE; time++){}
	GPIOC -> ODR &= ~GPIO_ODR_7;
		
	GPIOC -> ODR |= GPIO_ODR_9;
	for (time = 0; time < PAUSE; time++){}
	GPIOC -> ODR &= ~GPIO_ODR_9;
}

int main(void)
{
	//init
	init_led();
	init_button();
	while(1)
	{
		//TODO
		if ((GPIOA -> IDR & GPIO_IDR_0) == 0) 
		{
			rotate_right();
		}
		else 
		{
			rotate_left();
		}
	}
}
