# Security in Automobiles: Vulnerability and Protection of the On-board Diagnostics Port (OBD-II)

*** This file describes the various source code components used to provide a proof of concept implementation of a secure OBD-II protocol.

## Software dependencies 
We used the following open source libraries (which are included in this repo.):

- **AVR Crypto Lib** for SHA2 and HMAC-SHA2 : https://github.com/cantora/avr-crypto-lib
- **ECC** for the various components of ECC (e.g. ECDSA, ECDH, etc.) : https://github.com/kmackay/micro-ecc
- **ISO-TP** for the network and transport layers of CAN protocol: https://github.com/lishen2/isotp-c 

****************************************

## Hardware dependencies

This software has been developed and tested on DVK90CAN1 platform, which has an 8-bit AT90CAN128 microcontroller running at 8MHz, with 4KB of SRAM and 128 KB of Flash memory, as well as CAN interface. 

For more details:
http://ww1.microchip.com/downloads/en/devicedoc/doc4381.pdf
http://ww1.microchip.com/downloads/en/devicedoc/doc7679.pdf

****************************************

## Contents

- **core:** source code of MicroVisor along with crypto modules used.
- **Gateway** the source code of the automotive gateway side including ISO-TP implementation.
- **OBD-II-Device:** Represents the attached OBD-II device along with logical OEM server.
- **snapshot of some results:** some screenshots of some results. 



****************************************
 
## How to Run the code

- You should have two DVK90CAN1 platforms where their CAN interfaces connected to each other, and the platform that reperesnts the gateway connected to a PC through its serial interface to demonstrate results.
- Depending on which party you want to doploy in either platform, you have to update first the Makefile.include file in core Folder, where you have to specify the details of your programmer and add all source .c files.
- From the location where main.c resides, run the following commands:
	 - make mainTCA.hex
	 - make flash

- If you have different AVR platform, you have to edit the implementation of can.c and can.h to comply with the specifications listed in the datasheet of your platform.

- One you are done with both sides (Gateway and OBD-II device), you can start playing with buttons (keyboard) of the OBD-II device and see the results of the different scenarios implemented on the secreen.  
***************************************

** Please note that this implementation is only applicable for AVR-based MCUs. Supporting other MCUs will be added soon. 

** For further info. Please contact: secureobdii@gmail.com








