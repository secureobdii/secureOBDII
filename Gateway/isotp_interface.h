/*
 * isotp_interface.h
 *
 * Created: 1/16/2019 8:54:32 PM
 *  Author: XXXX
 */ 

#ifndef ISOTP_INTERFACE_H_
#define ISOTP_INTERFACE_H_

/* isotpi_send() function.
Send a ISO-TP message with a payload that is smaller than 8 bytes.

	Inputs:
		id				- CAN identifier of the message.
		payload_size	- The size of the data of the message.
		payload			- The data of the message.
		
	Returns: 
		0	- message sent.
*/
int isotpi_send(can_id_t id, uint8_t payload_size , can_msg_t payload[payload_size]);

/* isotpi_receive() function.
Receive a ISO-TP message with a payload that is smaller than 8 bytes.
This function will block until a message is received.

	Outputs:
		id				- CAN identifier of the message.
		payload_size	- The size of the data of the message.
		payload			- The data of the message.
*/
int isotpi_receive(can_id_t id,uint8_t payload_size , can_msg_t payload[payload_size]);

/*	isotpi_send_multi() function.
Send a multi frame ISO-TP message (i.e. payload_size > 7).

	Inputs:
		id				- CAN identifier of the message.
		payload_size	- The size of the data of the message.
		payload			- The data of the message.
	
	Returns:
		0	- message sent.	
*/
int isotpi_send_multi(can_id_t id, uint8_t *payload_size, uint8_t *payload);

/* isotpi_receive_multi() function.
Receive a multi frame ISO-TP message (i.e. payload_size > 7).
This function will block until message is received, or an error occurs.

	Inputs:
		id_send			- CAN identifier of the flow control message.

	Outputs:
		id_rec			- CAN identifier of the received message.
		payload_size	- The size of the data of the message.
		payload			- The data of the message.
*/	
int isotpi_receive_multi(can_id_t id_send, can_id_t id_rec, uint8_t payload_size, uint8_t payload[payload_size]);

#endif /* ISOTP_INTERFACE_H_ */