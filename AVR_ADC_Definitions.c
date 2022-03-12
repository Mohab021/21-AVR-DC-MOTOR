
#define F_CPU 4000000UL
#include <avr/io.h>

void ADC_Initialize()
{
	ADMUX = (1<<REFS0); //0b01000000 Voltage Reference is AVCC, Right justified , Using ADC0
	ADCSRA = 0x86; //0b10000110 = 0x86, Enable ADC, FOSC/64 (Prescaler);
}

uint16_t ADC_Read(uint8_t ch)
{
	//ADC_Read(0); // Read the ADC value of channel zero where (E.g. the sensor) is connected
	
	/* In case of using:
	ADMUX = ch (directly) = 0b00000XXX, then we put the last 5 bits to zero (Wrong)
	So we will won't change any of the last 5 bits in ADMUX as the following:

	ch is form 0-7, so we can represent it by ch = 0b00000XXX
	ch & 0b00000111 = 0b00000XXX, will always keep the ch value between 0-7
	ADMUX & 0b11111000 = 0bXXXXX000, will always keep the last 5 bits of ADMUX and clear the first 3 bits
	ADMUX | ch = 0bXXXXX000 | 0b00000XXX = 0bXXXXXXXX
	So finally, we changed MUX0-3 in ADMUX (according to input ch) without changing the last 5 bits
	= Last 5 bits of ADMUX (left) + First 3 bits of ch (right)
	*/
	
	ch &= 0x07;
	ADMUX &= 0xF8;
	ADMUX |= ch;
	
	ADCSRA |= (1<<ADSC); //start single conversion (put 1 in ADSC)
	while( (ADCSRA & (1<<ADSC)) ) ; //run loop continuously until conversion is complete (ADSC becomes 0)

	return ADC; //we use uint16_t, as it returns 10 bits (ADC register)
}

