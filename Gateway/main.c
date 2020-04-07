/*
 * main.c
 *
 * Created: 3/28/2019 12:30:25 AM
 * Author : XXXX
 */ 
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>


#include "types.h"
#include "uart_f.h"
#include "hexconv.h"
#include "leds.h"
#include "can.h"
#include "ECC/uECC.h"
#include "permission_table.h"
#include "sha256.h"
#include "hmac-sha256.h"
#include <util/delay.h>
#include "isotp.h"
#include "isotp_defines.h"
#include "isotp_user.h"
#include "clock.h"
#include "isotp_interface.h"

#include "microvisor.h"

const uint8_t public_key1_hex[] = {0xf0, 0xc8, 0x63, 0xf8,0xe5, 0x55, 0x11, 0x4b, 0xf4, 0x88, 0x2c, 0xc7, 0x87, 
	0xb9, 0x5c, 0x27, 0x2a, 0x7f, 0xe4, 0xdc, 0xdd, 0xf1, 0x92, 0x2f, 0x4f, 0x18, 0xa4, 0x94, 0xe1, 0xc3, 0x57,
	0xa1, 0xa6, 0xc3, 0x2, 0xd0, 0x7b, 0xeb, 0x57, 0x6a, 0xb6, 0x01, 0x06, 0x80, 0x68, 0xf0, 0xa9, 0xe0, 0x16,
	0x0c, 0x3a, 0x14, 0x11, 0x9f, 0x5d, 0x42, 0x6a, 0x79, 0x55, 0xda, 0x3e, 0x6e, 0xd3, 0xe81};

const uint8_t public_key2_hex[] = {0x7b, 0xcc, 0x5d, 0xea,  0x40, 0xe5, 0x32, 0x5c, 0xc6, 0x2c, 0x1b, 0x64, 0x2b,
	0xdd, 0x25, 0x8d, 0xc4, 0x89, 0x7b, 0x22, 0xb2, 0x06, 0x6e, 0x99, 0x28, 0x62, 0x15, 0xc4, 0xd5, 0x60, 0x30, 0x27,
	0x24, 0x6a, 0x7f, 0xb3, 0x1f, 0x11, 0x81, 0xdb, 0x96, 0x1c, 0xa6, 0x36, 0x57, 0xc4, 0xea, 0xbe, 0xed, 0xbe, 0x67,
	0xbb, 0x02, 0x42, 0x9d, 0x69, 0x42, 0x96, 0x24, 0x52, 0xcf, 0xc6, 0xec, 0x4f};

const uint8_t public_key3_hex[] = {0xcc, 0xfd, 0x9d, 0x10, 0x18, 0x62, 0x14, 0x3f, 0x83, 0xb3, 0xde, 0x5c, 0xac, 0xcd,
	0xf4, 0xa3, 0xaa, 0x55, 0xc0, 0xcd, 0xd8, 0xcd, 0x84, 0x8b, 0x84, 0x16, 0xd2, 0xb7, 0x10, 0x94, 0x88, 0xb9, 0xda, 0x60,
	0xf3, 0xbe, 0x17, 0xdd, 0xb0, 0x04, 0x12, 0xeb, 0x4c, 0x64, 0xd4, 0x22, 0xb3, 0xdf, 0x3b, 0xe9, 0x78, 0xc4, 0x18, 0x44,
	0x19, 0x5a, 0x89, 0x0e, 0xc5, 0xe3, 0x24, 0x9e, 0xfe, 0x5e};

const uint8_t public_key4_hex[] = {0xbd, 0x42, 0x37, 0x1a, 0xac, 0x68, 0x0a, 0x66, 0x03, 0xeb, 0xa1, 0xcf, 0x0f, 0x5e, 0x49,
	0x5a, 0xce, 0x59, 0x29, 0x9a, 0x4e, 0x2f, 0x42, 0xaa, 0x94, 0x3b, 0x84, 0x06, 0xc4, 0x2b, 0x66, 0xc8, 0x18, 0x2c, 0x09, 
	0x36, 0x37, 0xeb, 0x11, 0x99, 0xec, 0x41, 0xde, 0xe8, 0xba, 0x7a, 0x2f, 0xaa, 0xe0, 0x7b, 0x04, 0x85, 0x7b, 0x56, 0x90, 
	0x33, 0xc7, 0xb7, 0x3e, 0x31, 0x8a, 0x1f, 0x63, 0x21};

const uint8_t public_key5_hex[] = {0x5d, 0x19, 0xb5, 0x5c, 0xc3, 0x52, 0x8a, 0xaf, 0x86, 0x64, 0xbb, 0x20, 0xc9, 0xa1, 0x99,
	0x56, 0x7a, 0x24, 0x44, 0xb5, 0x49, 0xeb, 0xfa, 0x11, 0xc7, 0x21, 0xfd, 0x7f, 0xdc, 0xe2, 0xd2, 0xb3, 0x15, 0x71, 0xb5, 
	0x03, 0x39, 0x32, 0xb1, 0xd1, 0x43, 0x73, 0xf6, 0x86, 0x0d, 0x5a, 0x97, 0xf6, 0xef, 0xe6, 0x54, 0x70, 0xe5, 0x47, 0xaa,
	0x1c, 0x66, 0x3b, 0xdb, 0xb5, 0x79, 0x77, 0x37, 0x8c};

static const can_id_t default_id = {0x05, 0x05};
static const can_mask_t zero_mask = {0x00, 0x00};
static const can_mask_t default_mask = {255, 255};
static const can_msg_t ack_pos = {ACK_POS};
static const can_msg_t ack_neg = {ACK_NEG};

volatile state_t state = IDLE_S;
volatile event_t event = NULL_E;

/*	RNG() function.
Random number generator function.
	
	Inputs:	
		size	- The size of the random data required.
		
	Outputs:
		dest	- The randomly generated data.

Note:	The RNG implemented here is only of demonstrative purposes.
		For use in cryptographic operations, this would have to be implemented
		using some inherently random input (e.g. noise). 
*/
static int RNG(uint8_t *dest, unsigned size) {
	while(size){
		uint8_t val = (uint8_t) rand() + rand();
		*dest = val;
		++dest;
		--size;
	}
	return 1;
}

// Retrieve the appropriate public key from memory
static void get_public_key(uint8_t role, uint8_t public[64]){
	switch (role)
	{
		case ADMIN_ROLE :
		hex_to_bytes(public_key1_hex, 128, public);
		break;
		case OEM_ROLE :
		hex_to_bytes(public_key2_hex, 128, public);
		break;
		case POLICEMAN_ROLE :
		hex_to_bytes(public_key3_hex, 128, public);
		break;
		case REPAIRMAN_ROLE :
		hex_to_bytes(public_key4_hex, 128, public);
		break;
		case OWNER_ROLE :
		hex_to_bytes(public_key5_hex, 128, public);
		break;
	}
}

int verify_signature(uint8_t challenge[64], uint8_t signature[64], role_t role){
	const struct uECC_Curve_t * curve = uECC_secp256r1();
	int result;
	
	//Hash first for added security.
	uint8_t hash[32];
	sha256(hash, challenge,512);
	uint8_t public[64];
	
	//Retrieve appropriate public key.
	get_public_key(role, public);
	
	//Verify ECDSA signature
	result = uECC_verify(public, hash, 32, signature, curve);
	return result;
}

/* authenticated_key_agreement() function.
Implements the authenticated key agreement procedure outlined in thesis.pdf.

	Inputs:
		role	- The role that the user wishes to authenticate as.
	Outputs:
		secret	- The ECDH shared secret that was calculated.
*/
int authenticated_key_agreement(role_t role, uint8_t *secret){
	
	//Start protocol by calculating new private/public key pair for shared secret establishment.
	volatile uint8_t result;
	uart_puts("authentication started");
	uECC_set_rng(RNG);
	volatile uint8_t private2[32];
	volatile uint8_t public2[64];
	const struct uECC_Curve_t * curve = uECC_secp256r1();
	result = uECC_make_key(public2, private2, curve);
	uint8_t secret_unhashed[32];
	uint8_t public[64];
	get_public_key(role, public);
	
	//Send new public key.
	isotpi_send_multi(default_id, 64, public2);
	uart_puts("public key sent");
	
	//Wait for Signature.
	uint8_t signature[64];
	can_id_t idtmp;
	isotpi_receive_multi(default_id, idtmp, 64, signature);
	uart_puts("signature received");
	
	//Verify Signature.
	result = verify_signature(public2, signature, role);
	
	//Handle result of signature verification
	can_msg_t ack;
	if(result==1) {
		uart_puts("signature is valid");
		
		//calculate shared secret.
		result = uECC_shared_secret(public, private2, secret_unhashed, curve);
		uint32_t len = 256;
		sha256(secret, secret_unhashed, len);
		
		isotpi_send(default_id, 7, ack_pos);
	}else{
		uart_puts("signature is false!");
		
		isotpi_send(default_id, 7, ack_neg);
		return 1;
	} 
	return 0;
}

/* message_authentication() function.
Implements the message authentication procedure outlined in thesis.pdf

	Inputs:
		role	- The role that the user wishes to authenticate as.
		secret	- The ECDH shared secret used to secure the session.
*/
int message_authentication(role_t role, uint8_t	secret[32]){
	//Every iteration of this loop equals 1 message being authenticated using the shared secret.
	while(1){
		can_id_t id;
		can_msg_t message;
		uint8_t len;
		// Receive the message that the tester wants to send to the network.
		isotpi_receive_multi(default_id, id, 8, message);
		
		//Check + acknwoledge the permission check.
		if(check_permission(id, role) == 0){
			uart_puts("Permission granted");
			isotpi_send(default_id, 7, ack_pos);
		}else{
			uart_puts("Permission denied");
			isotpi_send(default_id, 7, ack_neg);
			continue;
		}
		
		// Receive the MAC of the message
		can_id_t idtmp;
		uint8_t mac[32];
		isotpi_receive_multi(default_id, idtmp, 32, mac);
		
		// Calculate MAC
		uint8_t mac2[32];
		uint16_t klen = 256;
		uint32_t msglen = 64;
		hmac_sha256(mac2, secret, klen ,message , msglen);
		
		//Check the MAC.
		if(memcmp(mac, mac2, 32) == 0){
			uart_puts("Authentication Ok");
			isotpi_send(default_id, 7, ack_pos);
		}else{
			uart_puts("Authentication Failed");
			isotpi_send(default_id, 7, ack_neg);
			continue;
		}
		
		//Forward the message to the internal vehicle network.
		forward_message(message, id);
	}
	return 0;
	
}

// In a real gateway, this function would forward the message to the appropriate subnetwork.
void forward_message(can_msg_t msg, can_id_t id){
	char idl[2];
	char idh[2];
	bytes_to_hex(id, 1, idl);
	id++;
	bytes_to_hex(id, 1, idh);
	uart_puts("---MESSAGE FORWARDED ONTO NETWORK---");
	uart_puts("IDL:");
	uart_putd(idl,2);
	uart_puts("IDH:");
	uart_putd(idh,2);
	char msghex[16];
	bytes_to_hex(msg, 8, msghex);
	uart_puts("MESSAGE:");
	uart_putd(msghex, 16);
	uart_puts("------------------------------------");
}

 int main()
 {
	uart_init();
	can_init();
	init_permissions_table();
	uart_puts("idle");
	clock_Init();
   	
	can_msg_t init;
	uint8_t secret[32];
	while(1){
		isotpi_receive(default_id, 7, init);
		if(authenticated_key_agreement(init[0],secret)) return 0;
		if(message_authentication(init[0],secret)) return 0;
	}
	
 }


