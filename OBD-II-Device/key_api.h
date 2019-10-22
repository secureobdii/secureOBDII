/*
 * key_api.h
 *
 * Created: 6/21/2018 4:35:15 AM
 *  Author: XXXX
 */ 


#ifndef KEY_API_H_
#define KEY_API_H_

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ECC/uECC.h"
#include "hexconv.h"
#include "sha256.h"
#include "types.h"

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
int RNG(uint8_t *dest, unsigned size);

/*	sign_challange() function.
This functions signs a challenge, using the appropriate private key.
	
	Inputs:
		challenge	- The challenge that is signed.
		role		- The role that is used to authenticate.
					  The role determines the private key that is used for signing.
					  
	Outputs:
		signature	- The calculated signature. 
	
*/
int sign_challenge(uint8_t challenge[64], uint8_t signature[64], uint8_t role);

/*	sign_challenge_dummy() function.
Used to generate a false signature by just returning the challenge.
	
	Inputs:
		challenge	- The challenge that is signed.
		role		- The role that is used to authenticate.
					  Is not used for the dummy operation.
	
	Outputs:
		signature	- The calculated signature (equals the given challenge).
	
*/
int sign_challenge_dummy(uint8_t challenge[64], uint8_t signature[64], uint8_t role);

/*	calculate_shared_secret() function.
Generate a shared secret using Elliptic Curve Diffie Hellman.

	Inputs:
		public		- The public key that is used for ECDH.
		role		- The role that is used to authenticate.
					  The role determines the private key that is used for ECDH.			  
	Outputs:
		secret		- The calculated secret.
*/
int calculate_shared_secret(uint8_t public[64], uint8_t role, uint8_t secret[32]);

/*	calculate_shared_secret_dummy()
Generate a dummy shared secret.

	Inputs:
		public		- The public key that is used for ECDH.
		role		- The role that is used to authenticate (not used in dummy).
		
	Outputs:
		secret		- The calculated secret (the public key is returned).
*/
int calculate_shared_secret_dummy(uint8_t public[64], uint8_t role, uint8_t secret[32]);


#endif /* KEY_API_H_ */