#include "buttons.h"

void ask_buttons_1_3(void){
	GPIOA->BSRR = GPIO_BSRR_BS_15;
	if(GPIOA->IDR & GPIO_IDR_4){
		buttons_state[0] = ON;
	} else 
		buttons_state[0] = OFF;
	if(GPIOA->IDR & GPIO_IDR_5){
		buttons_state[2] = ON;
	} else
		buttons_state[2] = OFF;
	GPIOA->BRR = GPIO_BRR_BR_15;
}

void ask_buttons_2_4(void){
	GPIOC->BSRR = GPIO_BSRR_BS_12;
	if(GPIOA->IDR & GPIO_IDR_4){
		buttons_state[1] = ON;
	} else 
		buttons_state[1] = OFF;
	if(GPIOA->IDR & GPIO_IDR_5){
		buttons_state[3] = ON;
	} else 
		buttons_state[3] = OFF;
	GPIOC->BRR = GPIO_BRR_BR_12;
}
