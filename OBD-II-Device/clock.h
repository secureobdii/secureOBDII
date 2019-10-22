/*
 * clock.h
 *
 * Created: 1/16/2019 1:27:26 AM
 *  Author: XXXX
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_

ISR(TIMER0_COMP_vect);

unsigned long int millis(void);

void clock_Init(void);


#endif /* CLOCK_H_ */