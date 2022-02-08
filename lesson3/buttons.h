#include "stm32f0xx.h"
#include <stdbool.h>
#define RED GPIO_BSRR_BS_6
#define BLUE GPIO_BSRR_BS_7
#define GREEN GPIO_BSRR_BS_9
#define ORANGE GPIO_BSRR_BS_8

typedef struct {
	bool state;
	bool pressed;
	int last_changed;
} KEY;

volatile KEY keys[4];
void reset(void);
void reset(void) {
	int i = 0;
	for(i = 0; i < 4; i++){
		keys[i].state = false;
		keys[i].pressed = true;
		keys[i].last_changed = 0;
	}
}

void ask_buttons_1_3(int cur_time);
void ask_buttons_2_4(int cur_time);

void ask_buttons_1_3(int cur_time){
	GPIOA->BSRR = GPIO_BSRR_BS_15;
	if(GPIOA->IDR & GPIO_IDR_4){
		if(cur_time > keys[0].last_changed + 100){
			if(keys[0].pressed){
				keys[0].state = true;
				keys[0].pressed = false;			
			} 
			else {
				keys[0].state = false;
				keys[0].pressed = true;
			}
			keys[0].last_changed = cur_time;
		}
	}
	else if(GPIOA->IDR & GPIO_IDR_5){
		if(cur_time > keys[2].last_changed + 100){
			if(keys[2].pressed){
				keys[2].state = true;
				keys[2].pressed = false;			
			} 
			else {
				keys[2].state = false;
				keys[2].pressed = true;
			}
			keys[2].last_changed = cur_time;
		}
	}
	GPIOA->BRR = GPIO_BRR_BR_15;
}
void ask_buttons_2_4(int cur_time){
	GPIOC->BSRR = GPIO_BSRR_BS_12;
	if(GPIOA->IDR & GPIO_IDR_4){
		if(cur_time > keys[1].last_changed + 100){
			if(keys[1].pressed){
				keys[1].state = true;
				keys[1].pressed = false;			
			} 
			else {
				keys[1].state = false;
				keys[1].pressed = true;
			}
			keys[1].last_changed = cur_time;
		}
	}
	else if(GPIOA->IDR & GPIO_IDR_5){
		if(cur_time > keys[3].last_changed + 100){
			if(keys[3].pressed){
				keys[3].state = true;
				keys[3].pressed = false;			
			} 
			else {
				keys[3].state = false;
				keys[3].pressed = true;
			}
			keys[3].last_changed = cur_time;
		}
	}
	GPIOC->BRR = GPIO_BRR_BR_12;
}
