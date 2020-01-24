/*
 * bootloader.h
 *
 *  Created on: 18 gru 2019
 *      Author: rafal
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include <avr/boot.h>
#include "circbuffer_bootloader/cirbuffer_b.h"
#include <avr/pgmspace.h>

#define LED_RED PD6
#define LED_BLUE PD7
#define	BOOTLOADER_PINE PE4
#define	BOOTLOADER_PINB PB1
#define TOGGLE(PORT, PIN) PORT ^= _BV(PIN);
#define PIN_HI(PORT, PIN) PORT |= _BV(PIN);
#define PIN_LO(PORT, PIN) PORT &= ~_BV(PIN);
#define PIN_VAL(PORT, PIN) (PORT & _BV(PIN))
#define MOSI_PIN PE0

//this macro will create string in PGMSPACE with the same content as name like: rav = "rav"
#define P(_NAME) const char PROGMEM _NAME [] = #_NAME

//this macro will create only PGM array name (PROGMEM STRING ARRAY)
#define PSA(_NAME) const char PROGMEM _NAME []


void write_page_to_flash_mem(uint32_t strona, uint8_t *buf) BOOTLOADER_SECTION;

namespace rx_id{
	enum id{
		txt_command,
		write_at,
		rxflush,
		bootloader=11,
		fail=255,
	};
}

#define HEADER_START '>'
#define HEADER_END '<'

struct MessageHeader{
	char 		head_start;	//'>'
	uint16_t	id;
	uint16_t	context;
	uint32_t 	msg_len;
	uint16_t	crc;
	char		head_end;	//'<'
};

class RxMessage{
public:
	uint8_t _header[sizeof(MessageHeader)];
	MessageHeader& header;
public:
	RxMessage(CircBufferB& cbuffer);
	bool check_header(CircBufferB& peek);
	rx_id::id msg_id();
	operator rx_id::id();
};

rx_id::id message(CircBufferB& cbuffer);


#endif /* BOOTLOADER_H_ */
