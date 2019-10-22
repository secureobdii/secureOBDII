/*
 * GccApplication2.c
 *
 * Created: 3/28/2018 12:30:25 AM
 * Author : XXXX
 */ 
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <interrupt.h>
#include "uECC.h"
#include "leds.h"
#include "uart_f.h"
#include "hexconv.h"
#include "can.h"
#include "sha256.h"
#include "hmac-sha256.h"
#include "types.h"
#include "key_api.h"
#include "isotp_interface.h"

#include "microvisor.h"

#define F_CPU 8000000UL
#include <util/delay.h>

// Default id for ISO-TP transmissions.
const can_id_t default_id = {0x05, 0x05};

/* This implementation provides three testing scenarios:

SCENARIO1: Uses wrong signature, so th initial authentication fails.
SCENARIO2: Authenticate as OWNER, but send 2 messages that are not permitted for this role.
SCENARIO3: Authenticate as ADMIN, all messages are accepted.
*/
typedef enum {
	NOTHING,
	SCENARIO1,
	SCENARIO2,
	SCENARIO3
} run_t;

volatile run_t run_scenario = NOTHING;

// Initialize buttons, which are used to initiate authentication sequence.		
void buttons_init(){
	DDRE = 0x00;
	PORTE = 1 << PE4 | 1 << PE5 | 1 << PE6 | 1 << PE7;
	EICRB = 0x00;
	EIMSK = 1 << INT4 | 1 << INT5 | 1 << INT6 | 1 << INT7;
	
}

//INITIATE SCENARIO1	
ISR(INT4_vect){
	EIMSK &= ~(1 << INT4);
	uart_puts("Running scenario 1");
	run_scenario = SCENARIO1;
}

//INITIATE SCENARIO2
ISR(INT5_vect){
	EIMSK &= ~(1 << INT5);
	uart_puts("Running scenario 2");
	run_scenario = SCENARIO2;
}

//INITIATE SCENARIO3
ISR(INT6_vect){
	EIMSK &= ~(1 << INT6);
	uart_puts("Running scenario 3");
	run_scenario = SCENARIO3;
}

//Not used.
ISR(INT7_vect){
	EIMSK &= ~(1 << INT7);
	EIMSK |= 1 << INT7;
}

/* authenticated_key_agreement() function.
Corresponds with the authenticated key agreement procedure defined in thesis.pdf.

	Inputs:
		role	- The role the tester wishes to authenticate as.
		
	Outputs:
		secret	- The calculated shared secret (ECDH).	

*/
int authenticated_key_agreement(uint8_t role, uint8_t secret[32]){
	uart_puts("starting authentication");
	
	//init authentication by sending role.
	can_msg_t init; 
	init[0]= role;
	isotpi_send(default_id, 7, init);
	uint8_t public[64];
	
	can_id_t id_tmp;
	//Receive generated public key.
	isotpi_receive_multi(default_id, id_tmp ,64, public);
	uart_puts("public key received");
	
	//Calculate shared secret.
	if(run_scenario == SCENARIO1){
		calculate_shared_secret_dummy(public, role, secret);
		_delay_ms(1000); // Introduce delay to mimic calculation time.
		}else{
		calculate_shared_secret(public, role, secret);
	}
	
	//Sign the public key.
	uint8_t signature[64];
	if(run_scenario == SCENARIO1){
		sign_challenge_dummy(public, signature, role);
		_delay_ms(1000); // Introduce delay to mimic calculation time.
		}else{
		sign_challenge(public, signature, role);
	}
	
	//Send signature.
	isotpi_send_multi(default_id,64,signature);
	uart_puts("signature sent");
	uint8_t ack[8];
	
	//wait for acknowledgment.
	uint8_t len;
	isotpi_receive(default_id,7,ack);
	if(ack[0] == ACK_POS){
		uart_puts("Successfully authenticated!");
		}else{
		uart_puts("Authentication failed!");
		return 1;
	}
	return 0;
}

/* message_authentication() function.
Corresponds with the message authentication procedure defined in thesis.pdf

	Inputs:
		message	- The message that is to be forwarded by the gateway.
		id		- The identifier of the message.
		secret	- The shared secret (ECDH) that was established before.
*/
int message_authentication(can_msg_t message, can_id_t id, uint8_t secret[32]){	
		
	//Send message you want to send to the vehicle network.
	isotpi_send_multi(id, 8, message);
	
	//Wait for acknowledgment.
	can_msg_t ack;
	isotpi_receive(default_id,7,ack);
	
	if(ack[0] == ACK_POS){
		uart_puts("permission granted!");
	}else{
		uart_puts("permission denied!");
		return 1;
	}
	
	//Calculate and send Hmac of message.
	uint8_t mac[32];
	uint16_t klen = 256;
	uint32_t msglen = 64;
	hmac_sha256(mac, secret, klen ,message , msglen);
	isotpi_send_multi(default_id, 32, mac);
	
	//wait for acknowledgment.
	isotpi_receive(default_id, 7, ack);
	if(ack[0] == ACK_POS){
		uart_puts("message accepted!");
	}else{
		uart_puts("message denied!");
		return 1;
	}
	_delay_ms(500); // Wait some time for gateway to be ready to receive.
	
	return 0;
}


int run(uint8_t role){
	uint8_t msgs[3][8] = { {1,1,1,1,1,1,1,1} , {2,2,2,2,2,2,2,2} , {3,3,3,3,3,3,3,3} };
	uint8_t ids[3][2] = {{2,1},{4,2},{0,0}};
	uint8_t secret[32];
	
	uint8_t ret = 0;
	ret = authenticated_key_agreement(role, secret);
	
	if(ret != 0) return 1;
	
	for (uint8_t c=0;c<3;c++)
	{
		ret += message_authentication(msgs[c], ids[c], secret);
	}
	return ret;
}


 int main()
 {	

	 uart_init();
	 buttons_init();
	 can_init();
	 clock_Init();
	
	 while(1){
		run_t runlcl = run_scenario;
		switch(runlcl){
			case NOTHING :
				break;
				
			//SCENARIO1: Use wrong key.
			case SCENARIO1 :
				run(OWNER_ROLE);
				run_scenario = NOTHING;
				break;
				
			//SCENARIO2: Use owner private key (Some messages will be denied).
			case SCENARIO2 :
				run(OWNER_ROLE);
				run_scenario = NOTHING;
				break;
				
			//SCENARIO3: Use tester private key (all messages accepted).
			case SCENARIO3 :
				run(ADMIN_ROLE);
				run_scenario = NOTHING;
				break;
		}
	 }
	 return 0;
 }

