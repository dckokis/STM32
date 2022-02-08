#include "traffic_lights.h"
#define TIMELIMIT 2000

static int CLOCK = 0;


void SysTick_Handler(void);
void SysTick_Handler(void){
		CLOCK += 100;
}

int main(void) 
{
	init_leds();
	init_button();
	Sys_Tick();
	int flash_count = 0;
	enum STATE state = BEGIN;
	while(1) {
		//if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
		//	CLOCK+= 100;
		//}
		if (!(GPIOA->IDR &= GPIO_IDR_0)) {
			switch(state){
				case BEGIN:
					if (CLOCK > 0){
						CLOCK = 0;
						Red();
						state = SR;
					}
					break;
				case SR:
					if (CLOCK >= TIMELIMIT & CLOCK < TIMELIMIT * 2){
						Yellow();					
					}
					else if (CLOCK >= TIMELIMIT * 2){
						CLOCK = 0;
						RedOff();
						YellowOff();
						Green();
						state = SG;
					}
					break;
				case SG:
					if (CLOCK >= TIMELIMIT){
						CLOCK = 0;
						GreenOff();
						state = PAUSE;
					}
					break;
				case PAUSE:
					if (CLOCK >= TIMELIMIT / 5){
						CLOCK = 0;
						state = FLASHING;
					}
					break;
				case FLASHING:
					if (CLOCK < TIMELIMIT / 5) {
						Green();
					}
					else if (CLOCK >= TIMELIMIT / 5 & flash_count < 2){
						CLOCK = 0;
						GreenOff();
						state = PAUSE;
						flash_count++;
					}
					else if (CLOCK >= TIMELIMIT / 5 & flash_count >= 2){
						CLOCK = 0;
						GreenOff();
						state = SY;
						Yellow();
						flash_count = 0;
					}
					break;
				case SY:
					if (CLOCK >= TIMELIMIT){
						CLOCK = 0;
						YellowOff();
						Red();
						state = SR;
					}
					break;
			}
		} else {
			switch(state){
				case SR:
					CLOCK = 0;
					RedOff();
					YellowOff();
					Green();
					state = SG;
					break;
				case SG:
				case SY:
				case FLASHING:
				case BEGIN:
				case PAUSE:
					break;
			}
		}
	}
}
