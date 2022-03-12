
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include "avr/interrupt.h"
#include "AVR_Ports.h"
#include "AVR_LCD_Declarations.h"
#include "AVR_Counter_Timer.h"
#include "AVR_ADC_Declarations.h"

uint8_t Direction;

ISR(INT2_vect)
{
	//PB6 = 1, PB7 = 0 for motor clockwise
	//PB6 = 0, PB7 = 1 for motor anticlockwise
	
	PORTB ^= (1<<6) | (1<<7); //Toggle PB6 & PB7
	Direction = !Direction; //Toggle direction
	_delay_ms(50); //de-bouncing
}

int main(void)
{
	uint16_t ADC_V;
	uint8_t Duty_R;
	
	LCD_Initialize();
	ADC_Initialize(); //ADC for speed control (input from user)
	PWM_Initialize(); //PWM for speed control (output to motor driver)
	
	DDRA &= !(1<<0); //make PA0 as input [ADC0]
	DDRB &= !(1<<2); //make PB2 as input [INT2]
	DDRB |= (1<<3); //make PB3 as output [OC0, PWM out]
	DDRB |= (1<<6); //make PB6 as output [for direction control]
	DDRB |= (1<<7); //make PB7 as output [for direction control]
	
	GICR |= (1<<INT2); //Enable Interrupt 2
	MCUCSR |= (1<<ISC2); //Trigger INT2 on Rising Edge triggered
	sei(); //Enable Global Interrupt
	
	PORTB |= (1<<6); ////PB6 = 1, //PB7 = 0 [clockwise as a start]
	Direction = 0; //0 for clockwise direction [as a start]
	
    while(1) 
    {	
		if(Direction == 0) //0 for clockwise direction
		{
			LCD_Print_xy(0, 0, "+ve");
			_delay_ms(5);
		}
		else //1 for anticlockwise direction
		{
			LCD_Print_xy(0, 0, "-ve");
			_delay_ms(5);
		}
		
		ADC_V = ADC_Read(0); //a value from 0 to 1023
		Duty_R = ADC_V / 4; //rescale ADC output to a value from 0 to 255 [suitable for PWM - 8bit T0]
		OCR0 = Duty_R; //change duty ratio to change motor input voltage and motor speed as well
	}
}
