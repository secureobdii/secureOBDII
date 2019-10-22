/*
 * hexconv.h
 *
 * Created: 5/9/2018 2:20:07 AM
 *  Author: XXXX
 *
 * Based on existing procedures:
 * https://stackoverflow.com/questions/3408706/hexadecimal-string-to-byte-array-in-c
 * https://stackoverflow.com/questions/6357031/how-do-you-convert-a-byte-array-to-a-hexadecimal-string-in-c/34701004 
 */ 

#include <stdlib.h>
#include "types.h"

#ifndef HEXCONV_H_
#define HEXCONV_H_

/* bytes_to_hex() function.
Converts a byte array to its corresponding hexadecimal representation.

	Inputs:
		src		- The byte array that is to be converted.
		intlen	- The length of the byte array (in bytes).
		
	Outputs:
		dest	- Will be filled in with the corresponding hexadecimal representation (char array).
				  The calling function is expected to supply a pointer to a char array of the correct size,
				  which is two times the size of the given byte array.
*/
void bytes_to_hex(const uint8_t *src, uint8_t len, char *dest);

/* hex_to_bytes() function.
Converts a char array of hexadecimal numbers to a byte array.

	Inputs:
		src		- The hexadecimal array that is to be converted (in chars).
		strlen	- The length of the hexadecimal char array (in bytes).
		
	Outputs:
		dest	- Will be filled in with the corresponding byte array.
				  The calling function is expected to supply a pointer to a byte array of the correct size,
				  which is half the size of the given char array.	
*/
void hex_to_bytes(char* src, uint8_t slength, uint8_t* dest);

#endif /* HEXCONV_H_ */