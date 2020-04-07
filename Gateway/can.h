/*
 * can.h
 *
 * Created: 10/15/2019 8:42:43 PM
 *  Author: XXXX
 */ 


#ifndef CAN_H_
#define CAN_H_

#define INTR_MASK 0b10000000
#define BXOK_MASK 0b00010000
#define RXOK_MASK 0b00100000
#define TXOK_MASK 0b01000000
#define PAGE_MASK 0b00001000

#include "types.h"

/* can_init() function.
Initialize the CAN controller.
*/
void can_init();

/* can_send_message() function.
Send a CAN message, and block until message is sent.
The mob is freed after the message is sent.

	Inputs:
		mobnr	- The MOB to use for the transmission.
		id		- The CAN ID of the message.
		message - The message payload.
		size	- The size of the message.
*/
int can_send_message( uint8_t mobnr , can_id_t id, can_msg_t message, uint8_t size);

/* can_enable_receive() function.
Designate a MOB and enable it for message reception.
Use can_wait_for_receive() to wait for an incoming message.
Use can_retrieve_message() to retrieve the message upon reception.

	Inputs:
		mobnr	- The MOB that is to be enabled.
		mask	- The CAN ID mask that is used for reception.
*/
void can_enable_receive(uint8_t mobnr, can_mask_t mask);

/* can_wait_for_receive() function.
Block until a message is received.*/
int can_wait_for_receive();

/* can_retrieve_message() function.
Retrieve a received message.

	Inputs:
		mobnr	- The MOB to retrieve the message from.
		
	Outputs:
		id		- The CAN ID of the incoming message.
		message - The payload of the incoming message.
		size	- the data length code (DLC) of the incoming message.
*/
int can_retrieve_message(uint8_t mobnr, can_id_t id, can_msg_t message, uint8_t *size);

/* can_receive_message() function.
Enable MOB, wait for a message, and retrieve the message.

	Inputs:
		mobnr	- The MOB to use of message reception.
		mask	- The CAN ID mask used to filter incoming messages.
		
	Outputs:
		id		- The CAN ID of the incoming message.
		message	- The payload of the incoming message.
*/
int can_receive_message( uint8_t mobnr, can_id_t id, can_mask_t mask, can_msg_t message, uint8_t *len);

/* can_print_message() function.
Send the ID and payload of an incoming message to the UART interface.

	Inputs:
		mobnr	- MOB to print message from.
*/
void can_print_message( uint8_t mobnr);

/* can_free_mob() function.
Disable a given MOB.

	Inputs:
		mobnr	- The MOB that is to be disabled.
*/
void can_free_mob(uint8_t mobnr);

//Send frame buffer.
int can_send_frame_buffer( uint8_t *message, uint8_t buff_len);

//Receive frame buffer.
int can_receive_frame_buffer( uint8_t *message, uint8_t buff_len);



#endif /* CAN_H_ */