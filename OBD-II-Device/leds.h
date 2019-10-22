/*
 * leds.h
 *
 * Created: 5/9/2018 1:48:15 AM
 *  Author: XXXX
 */ 


#ifndef LEDS_H_
#define LEDS_H_

/* light_led() function.
Lights the LED with the given number.

	Inputs:
		nr		- The number of the LED that is lit (0 to 7).
				  If the given number does not fall within the accepted range
				  no LED is lit.
*/
void light_led( uint8_t nr);

/* clear_led() function.
Clears the LED with the given number.

	Inputs:
		nr		- The number of the LED that is to be cleared (0 to 7).
				  If the given number does not fall within the accepted range
				  no LED is cleared.
*/
void clear_led( uint8_t nr);

/* clear_leds() function.
Clears all LEDs.
*/
void clear_leds();

#endif /* LEDS_H_ */