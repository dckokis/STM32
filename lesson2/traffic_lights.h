#include "stm32f0xx.h"

//PC6..PC9
void init_leds(void);
//PA0
void init_button(void);

void Sys_Tick(void);

enum STATE {
	SG = 0,
	SR = 1,
	SY = 2,
	FLASHING = 3,
	BEGIN = -1,
	PAUSE = -2
};

void Green(void);
void Red(void);
void Yellow(void);

void GreenOff(void);
void RedOff(void);
void YellowOff(void);
