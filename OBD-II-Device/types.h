/*
 * types.h
 *
 * Created: 6/8/2018 5:26:44 PM
 *  Author: XXXX
 */ 


#ifndef TYPES_H_
#define TYPES_H_

#define ADMIN_ROLE 0b00000000
#define OEM_ROLE 0b00000001
#define POLICEMAN_ROLE 0b00000010
#define REPAIRMAN_ROLE 0b00000011
#define OWNER_ROLE 0b00000100

#define ACK_POS 0x00
#define ACK_NEG 0x01

typedef uint8_t can_id_t[2];
typedef uint8_t can_mask_t[2];
typedef uint8_t can_msg_t[8];
typedef uint8_t can_buff_512_t[64];
typedef uint8_t can_buff_128_t[64];

#endif /* TYPES_H_ */