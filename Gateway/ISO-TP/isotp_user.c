/*
 * isotp_user.c
 *
 * Created: 1/15/2019 9:25:52 PM
 *  Author: michel
 */ 

#include <stdint.h>
#include "../clock.h"
#include "../uart_f.h"
#include "../can.h"

/* user implemented, print debug message */
void isotp_user_debug(const char* message){
	uart_puts(message);
}

/* user implemented, send can message */
int  isotp_user_send_can(const uint32_t arbitration_id, const uint8_t* data, const uint8_t size){
	can_msg_t message;
	can_id_t id;
	memcpy(id,&arbitration_id,2);
	can_send_message(0, id, data, size);
	return 0;
}

/* user implemented, get millisecond */
uint32_t isotp_user_get_ms(void){
	return millis();
}

