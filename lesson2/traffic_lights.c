#include "traffic_lights.h"

void init_leds(void){
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  GPIOC -> MODER &=~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 |GPIO_MODER_MODER9);
	GPIOC -> MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER7_0|GPIO_MODER_MODER8_0;  
}

void init_button(void){
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  GPIOA -> MODER &=~ GPIO_MODER_MODER0;
}

void Sys_Tick(void) {
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
	SystemCoreClockUpdate();
	SysTick->LOAD = SystemCoreClock / 10 - 1;//100ms
}

void Green(void){
	GPIOC->BSRR = GPIO_BSRR_BS_9;
}
void Red(void){
	GPIOC->BSRR = GPIO_BSRR_BS_6;
}
void Yellow(void){
	GPIOC->BSRR = GPIO_BSRR_BS_8;
}
void GreenOff(void){
	GPIOC->BRR = GPIO_BRR_BR_9;
}
void RedOff(void){
	GPIOC->BRR = GPIO_BRR_BR_6;
}
void YellowOff(void){
	GPIOC->BRR = GPIO_BRR_BR_8;
}
