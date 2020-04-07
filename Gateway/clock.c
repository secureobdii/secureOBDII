/*
 * clock.c
 *
 * Created: 11/16/2019 1:27:07 AM
 *  Author: XXXX
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>

volatile uint32_t ms = 0;

ISR(TIMER0_COMP_vect)
{
	ms = ms+1;
}

unsigned long int millis(void)
{
	return ms;
}

void clock_Init(void)
{
	OCR0A = 61;
	TIMSK0 = 1 << OCIE0A;
	TCCR0A = 1 << WGM00 | 1 << WGM01 | 1 << CS01 | 1 << CS00;
}