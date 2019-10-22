/*
 * uart_f.c
 *
 * Created: 5/9/2018 1:59:11 AM
 *  Author: XXXX
 */ 

#include <avr/io.h>
#include <string.h>
#include "uart_f.h"
#include "types.h"


void uart_init(){
	UBRR0H = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRR0L = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	UCSR0C = (0<<UMSEL0) | (0<<UPM0) | (1<<USBS0) | (3<<UCSZ0);
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
}

void uart_puts(char* s){
	volatile uint8_t i;
	int len = strlen(s);
	for (i = 0; i < len; i++){
		while(!( UCSR0A & 0X20));
		UDR0=s[i];
	}
	while(!( UCSR0A & 0X20));
	UDR0 = 13;
	while(!( UCSR0A & 0X20));
	UDR0 = 10;
}

void uart_putd(char* s, uint8_t len){
	int i;
	for (i = 0; i < len; i++){
		while(!( UCSR0A & 0X20));
		UDR0=s[i];
	}
	while(!( UCSR0A & 0X20));
	UDR0 = 13;
	while(!( UCSR0A & 0X20));
	UDR0 = 10;
}


