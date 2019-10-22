/*
 * isotp_interface.c
 *
 * Created: 1/16/2019 8:54:12 PM
 *  Author: XXXX
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "can.h"
#include "ISO-TP/isotp.h"
#include "ISO-TP/isotp_defines.h"
#include "ISO-TP/isotp_user.h"
#include "clock.h"
#include <util/delay.h>

const can_mask_t zero_mask = {0x00, 0x00};
const can_mask_t default_mask = {255, 255};

static IsoTpLink g_link;
static uint8_t g_isotpRecvBuf[128];
static uint8_t g_isotpSendBuf[128];

int isotpi_send(can_id_t id, uint8_t payload_size , uint8_t payload[payload_size]){
	uint32_t arbitration_id = 0;
	memcpy(&arbitration_id,id,2);
	isotp_init_link(&g_link,arbitration_id, g_isotpSendBuf, sizeof(g_isotpSendBuf),g_isotpRecvBuf, sizeof(g_isotpRecvBuf));
	return isotp_send(&g_link, payload, payload_size);
}

int isotpi_receive(can_id_t id, uint8_t payload_size , uint8_t payload[payload_size]){
	uint32_t arbitration_id = 0;
	memcpy(&arbitration_id,id,2);
	isotp_init_link(&g_link,arbitration_id, g_isotpSendBuf, sizeof(g_isotpSendBuf),g_isotpRecvBuf, sizeof(g_isotpRecvBuf));
	can_msg_t message;
	can_receive_message(0,id,zero_mask,message,8);
	isotp_on_can_message(&g_link, message, 8);
	uint16_t out_size;
	return isotp_receive(&g_link, payload, payload_size, &out_size);
}

int isotpi_send_multi(can_id_t id, uint8_t payload_size ,uint8_t payload[payload_size]){
	uint32_t arbitration_id = 0;
	memcpy(&arbitration_id,id,2);
	isotp_init_link(&g_link,arbitration_id, g_isotpSendBuf, sizeof(g_isotpSendBuf),g_isotpRecvBuf, sizeof(g_isotpRecvBuf));

	can_enable_receive(1,zero_mask);
	isotp_send(&g_link, payload, payload_size);
	
	can_msg_t message;
	can_id_t message_id;
	uint8_t len;
	uint8_t mobnr;
	
	mobnr = can_wait_for_receive();
	
	can_retrieve_message(mobnr, message_id, message, &len);
	isotp_on_can_message(&g_link,message,len);
	
	while(1){
		
		isotp_poll(&g_link);
		
		if(g_link.send_status == ISOTP_SEND_STATUS_IDLE){
			can_free_mob(1);
			return 0;
		}
		
		if(g_link.send_status == ISOTP_SEND_STATUS_ERROR){
			can_free_mob(1);
			return 1;
		}
	}
}

int isotpi_receive_multi(can_id_t id_send, can_id_t id_rec, uint8_t payload_size, uint8_t payload[payload_size]){
	uint32_t arbitration_id = 0;
	memcpy(&arbitration_id,id_send,2);
	isotp_init_link(&g_link, arbitration_id, g_isotpSendBuf, sizeof(g_isotpSendBuf), g_isotpRecvBuf, sizeof(g_isotpRecvBuf));
    
	while(1){
		
		uint8_t j;
		for(j=0; j<14; j++){
			can_enable_receive(j,zero_mask);
		}
		
		uint8_t mobnr;
		can_msg_t message;
		uint8_t size;
		
		while(1){
			mobnr = can_wait_for_receive();
			can_retrieve_message(mobnr, id_rec, message, &size);
			isotp_on_can_message(&g_link, message, size);
			switch(g_link.receive_status){
				case ISOTP_RECEIVE_STATUS_INPROGRESS :
				break;
				case ISOTP_RECEIVE_STATUS_FULL :
				for(j=0; j<14; j++){
					can_free_mob(j);
				}
				uint16_t out_size;
				uint8_t ret;
				return isotp_receive(&g_link, payload, payload_size, &out_size);
			}
		} 
	}
	
}


