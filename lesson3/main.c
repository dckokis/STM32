#include "buttons.h"

static int CLOCK = 0;

void Sys_Tick(void);
void SysTick_Handler(void);
void init(void);

void Sys_Tick(void){
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
	SystemCoreClockUpdate();
	SysTick->LOAD = SystemCoreClock / 10 - 1;//100ms
}

void init(void){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN;

	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5);
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1);

	GPIOA->MODER &= ~(GPIO_MODER_MODER4 | GPIO_MODER_MODER5);

	GPIOA->MODER &= ~(GPIO_MODER_MODER15);
	GPIOA->MODER |= (GPIO_MODER_MODER15_0);

	GPIOC->MODER &= ~(GPIO_MODER_MODER12);
	GPIOC->MODER |= (GPIO_MODER_MODER12_0);

	GPIOC -> MODER &=~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 |GPIO_MODER_MODER9);
	GPIOC -> MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER7_0|GPIO_MODER_MODER8_0; 
}

void SysTick_Handler(void){
	CLOCK += 100;
	ask_buttons_1_3(CLOCK);
	ask_buttons_2_4(CLOCK);

}

void OffAll(void);
void OffAll(void){
	GPIOC->BRR = GPIO_BRR_BR_6;
	GPIOC->BRR = GPIO_BRR_BR_7;
	GPIOC->BRR = GPIO_BRR_BR_8;
	GPIOC->BRR = GPIO_BRR_BR_9;
}

int main(void) {
	init();
	Sys_Tick();
	while(1){
		OffAll();
		if(keys[0].state){
			GPIOC->BSRR = RED;
		}
		if(keys[2].state){
			GPIOC->BSRR = BLUE;
		}
		if(keys[1].state){
			GPIOC->BSRR = GREEN;
		}
		if(keys[3].state){
			GPIOC->BSRR = ORANGE;
		}
	}
}
