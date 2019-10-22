/*
 * leds.c
 *
 * Created: 5/9/2018 1:35:55 AM
 *  Author: XXXX
 */ 

#include <avr/io.h>

void light_led( uint8_t nr){
	switch (nr){
		case 0:
		PORTA |= 1 << PORTA0;
		DDRA |= 1 << DDA0;
		break;
		case 1:
		PORTA |= 1 << PORTA1;
		DDRA |= 1 << DDA1;
		break;
		case 2:
		PORTA |= 1 << PORTA2;
		DDRA |= 1 << DDA2;
		break;
		case 3:
		PORTA |= 1 << PORTA3;
		DDRA |= 1 << DDA3;
		break;
		case 4:
		PORTA |= 1 << PORTA4;
		DDRA |= 1 << DDA4;
		break;
		case 5:
		PORTA |= 1 << PORTA5;
		DDRA |= 1 << DDA5;
		break;
		case 6:
		PORTA |= 1 << PORTA6;
		DDRA |= 1 << DDA6;
		break;
		case 7:
		PORTA |= 1 << PORTA7;
		DDRA |= 1 << DDA7;
		break;
		default:
		break;
	}
}

void clear_led( uint8_t nr){
	switch (nr){
		case 0:
		PORTA |= 0 << PORTA0;
		DDRA |= 0 << DDA0;
		break;
		case 1:
		PORTA |= 0 << PORTA1;
		DDRA |= 0 << DDA1;
		break;
		case 2:
		PORTA |= 0 << PORTA2;
		DDRA |= 1 << DDA2;
		break;
		case 3:
		PORTA |= 0 << PORTA3;
		DDRA |= 0 << DDA3;
		break;
		case 4:
		PORTA |= 0 << PORTA4;
		DDRA |= 0 << DDA4;
		break;
		case 5:
		PORTA |= 0 << PORTA5;
		DDRA |= 0 << DDA5;
		break;
		case 6:
		PORTA |= 0 << PORTA6;
		DDRA |= 0 << DDA6;
		break;
		case 7:
		PORTA |= 0 << PORTA7;
		DDRA |= 0 << DDA7;
		break;
		default:
		break;
	}
}


void clear_leds(){
	PORTA = 0x00;
}


