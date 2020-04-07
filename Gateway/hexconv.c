/*
 * hexconv.c
 *
 * Created: 5/9/2019 2:19:41 AM
 *  Author: XXXX
 */ 

#include <avr/io.h>
#include "hexconv.h"

void bytes_to_hex(const uint8_t *src, uint8_t intlen, char dest[intlen*2])
{
	static const unsigned char table[] = "0123456789abcdef";

	for (; intlen > 0; intlen--)
	{
		unsigned char c = *src++;
		*dest++ = table[c >> 4];
		*dest++ = table[c & 0x0f];
	}
}

void hex_to_bytes(char *src, uint8_t strlen, uint8_t dest[strlen/2]) {
	memset(dest, 0, strlen/2);
	uint8_t index = 0;
	while (index < strlen) {
		char c = src[index];
		int value = 0;
		if(c >= '0' && c <= '9')
		value = (c - '0');
		else if (c >= 'A' && c <= 'F')
		value = (10 + (c - 'A'));
		else if (c >= 'a' && c <= 'f')
		value = (10 + (c - 'a'));

		dest[(index/2)] += value << (((index + 1) % 2) * 4);

		index++;
	}
}

