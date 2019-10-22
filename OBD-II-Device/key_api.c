/*
 * key_api.c
 *
 * Created: 6/21/2018 4:24:52 AM
 *  Author: XXXX
 */

#include "key_api.h"

/*	The key_api mimics the interface of the central server.
	The secret keys are stored here.
	Users should be able to authenticate by providing the appropriate credentials,
	thereby gaining access to the appropriate ECC functions.
*/


const uint8_t private_key1_hex[] = {0x92, 0x99, 0x07, 0x88, 0xd6, 0x6b, 0xf5, 0x58, 0x05, 0x2d, 0x11, 0x2f, 0x54, 0x98, 
	0x11, 0x17, 0x47, 0xb3, 0xe2, 0x8c, 0x55, 0x98, 0x4d, 0x43, 0xfe, 0xd8, 0xc8, 0x82, 0x2a, 0xd9, 0xf1, 0xa7};

const uint8_t private_key2_hex[] = {0x48, 0xe5, 0xca, 0x2a, 0x67, 0x5a, 0xb4, 0x9c, 0xa2, 0x14, 0xb8, 0x84, 0x81, 0x39,
	0x35, 0x02, 0x4b, 0x0c, 0x61, 0xed, 0xc8, 0xd1, 0x30, 0x5f, 0xe5, 0x23, 0x0d, 0xf3, 0x41, 0x62, 0x33, 0x48};

const uint8_t private_key3_hex[] = {0x8e, 0x29, 0x58, 0xa1, 0x47, 0x5e, 0xd7, 0x07, 0x62, 0x34, 0x0e, 0x97, 0x97, 0x78, 
	0x8e, 0x00, 0x61, 0xf2, 0x1f, 0xce, 0xbd, 0x67, 0x88, 0x9f, 0xdd, 0x4f, 0x4c, 0xe2, 0xb5, 0xf6, 0xb2, 0xde};

const uint8_t private_key4_hex[] = {0xb2, 0xc9, 0x50, 0xab, 0xc8, 0x7a, 0x55, 0x44, 0x2c, 0xc0, 0x0f, 0x1e, 0x3a, 0xc3,
	0x8f, 0x81, 0xb7, 0xe9, 0x50, 0x36, 0xfd, 0x19, 0x1e, 0xa1, 0x34, 0xff, 0x61, 0x6d, 0x98, 0x06, 0xe1, 0x0c};

const uint8_t private_key5_hex[] = {0xb0, 0x80, 0x39, 0xa1, 0x90, 0x79, 0xd5, 0x21, 0x84, 0x65, 0xf6, 0xd9, 0x75, 0x52,
	0xbd, 0x70, 0xb8, 0x86, 0x74, 0x23, 0xd6, 0x73, 0x65, 0xb8, 0x43, 0x1b, 0x6f, 0x21, 0x3a, 0x19, 0x74, 0x71};


int RNG(uint8_t *dest, unsigned size){
	while(size){
		uint8_t val = (uint8_t) rand();
		*dest = val;
		++dest;
		--size;
	}
	return 1;
}

static void get_private_key(uint8_t role, uint8_t private[32]){
	switch (role)
	{
		case ADMIN_ROLE :
			hex_to_bytes(private_key1_hex, 64, private);
			break;
		case OEM_ROLE :
			hex_to_bytes(private_key2_hex, 64, private);
			break;
		case POLICEMAN_ROLE :
			hex_to_bytes(private_key3_hex, 64, private);
			break;
		case REPAIRMAN_ROLE :
			hex_to_bytes(private_key4_hex, 64, private);
			break;
		case OWNER_ROLE :
			hex_to_bytes(private_key5_hex, 64, private);
			break;
	}
} 

int sign_challenge(uint8_t challenge[64], uint8_t signature[64], uint8_t role){
	
	// Set RNG.
	uECC_set_rng(&RNG);
	const struct uECC_Curve_t *curve = uECC_secp256r1();
	uint8_t hash[32];
	
	// Hash the challenge first for extra security.
	sha256(hash, challenge, 512);
	uint8_t private[32];
	
	// get the appropriate private key.
	get_private_key(role, private);

	if (!uECC_sign(private, hash, 32, signature, curve)) {
		uart_puts("sign failed");
		return 1;
	}
	
	return 0;
}


int sign_challenge_dummy(uint8_t challenge[64], uint8_t signature[64], uint8_t role){
	// Send back the challange as dummy signature.
	memcpy(signature,challenge,64);
	return 0;
}

int calculate_shared_secret(uint8_t public[64], uint8_t role, uint8_t secret[32]){
	const struct uECC_Curve_t * curve = uECC_secp256r1();
	uint8_t secret_unhashed[32];
	
	//Generate Shared Secret.
	uint8_t private[32];
	
	//Use the appropriate private key
	get_private_key(role, private);
	if(!uECC_shared_secret(public, private, secret_unhashed, curve)){
		uart_puts("shared secret creation failed");
		return 1;
	}
	uint32_t len = 256;
	
	//Hash the secret for extra security.
	sha256(secret, secret_unhashed, len);
	return 0;
}

int calculate_shared_secret_dummy(uint8_t public[64], uint8_t role, uint8_t secret[32]){
	//Send back the first part of the public key as dummy secret.
	memcpy(secret, public, 32);
	return 0;
}

