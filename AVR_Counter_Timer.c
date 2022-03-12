
#include <avr/io.h>
#define F_CPU 4000000UL
#include "AVR_Counter_Timer.h"

void PWM_Initialize()
{
	//Using Fast PWM
	TCCR0 = 0x69; //0b01101001, Fast PWM, no pre-scaling, Non-Inverted (Clear on compare match & Set at top)
}

