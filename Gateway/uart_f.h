/*
 * uart_f.h
 *
 * Created: 5/9/2019 1:59:52 AM
 *  Author: XXXX
 */ 


#ifndef UART_F_H_
#define UART_F_H_

#ifndef F_CPU
# define F_CPU 8000000L
#endif

#define USART_BAUDRATE 57600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

/* uart_init() function.
Initialize the UART controller by setting the baud rate and enabling the connection.
*/
void uart_init();

/* uart_puts() function.
Transmit a string over the UART interface.

	Inputs:
		s		- The string that is to be sent.
				  Carriage return (CR) and line feed (LF) characters are automatically transmitted at the end of the string.
*/
void uart_puts(char* s);

/* uart_putd() function.
Transmits a byte array over the UART interface.

	Inputs:
		s		- The byte array that is to be sent.
		len		- The length of the byte array.
				  Carriage return (CR) and line feed (LF) characters are automatically transmitted at the end of the string.	
*/
void uart_putd(char* s, uint8_t len);


#endif /* UART_F_H_ */