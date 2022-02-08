#include "stm32f0xx.h"
#include <stdbool.h>
#define TIME 80
#define RED GPIO_BSRR_BS_6
#define BLUE GPIO_BSRR_BS_7
#define GREEN GPIO_BSRR_BS_9
#define ORANGE GPIO_BSRR_BS_8

typedef struct {
	bool state;
	uint8_t last_changed;
} KEY;

enum BUTTON {
	UP,
	LEFT,
	DOWN,
	RIGHT
};

volatile static KEY keys[4];

void ask_buttons_1_3(void);
void ask_buttons_2_4(void);

void ask_buttons_1_3(void){
	GPIOA->BSRR = GPIO_BSRR_BS_15;
	if(GPIOA->IDR & GPIO_IDR_4){
		if(keys[UP].last_changed < TIME){
			keys[UP].last_changed++;
		} else {
			keys[UP].state = true;
			keys[UP].last_changed = 0;
		}
	}
	else if(GPIOA->IDR & GPIO_IDR_5){
		if(keys[DOWN].last_changed < TIME){
			keys[DOWN].last_changed++;
		} else {
			keys[DOWN].state = true;
			keys[DOWN].last_changed = 0;
		}
	}
	GPIOA->BRR = GPIO_BRR_BR_15;
}

void ask_buttons_2_4(void){
	GPIOC->BSRR = GPIO_BSRR_BS_12;
	if(GPIOA->IDR & GPIO_IDR_4){
		if(keys[RIGHT].last_changed < TIME) {
			keys[RIGHT].last_changed++;
		} else {
			keys[RIGHT].state = true;
			keys[RIGHT].last_changed = 0;
		}
	}
	else if(GPIOA->IDR & GPIO_IDR_5){
		if(keys[LEFT].last_changed < TIME){
			keys[LEFT].last_changed++;
		} else {
			keys[LEFT].state = true;
			keys[LEFT].last_changed = 0;
		}
	}
	GPIOC->BRR = GPIO_BRR_BR_12;
}
