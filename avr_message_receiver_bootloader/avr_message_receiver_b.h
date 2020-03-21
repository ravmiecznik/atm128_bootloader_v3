/*
 * avr_message_receiver_b.h
 *
 *  Created on: 25 sty 2020
 *      Author: rafal
 */

#ifndef AVR_MESSAGE_RECEIVER_BOOTLOADER_AVR_MESSAGE_RECEIVER_B_H_
#define AVR_MESSAGE_RECEIVER_BOOTLOADER_AVR_MESSAGE_RECEIVER_B_H_

#include <stdint.h>
#include "../circbuffer_bootloader/cirbuffer_b.h"

namespace rx_id{
	enum id{
		txt_command,
		write_at,
		rxflush,
		bootloader_old=11,
		run_main_app=254,
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
	CircBufferB& buffer;
	RxMessage(CircBufferB& cbuffer);
	bool check_header(CircBufferB& peek);
	rx_id::id msg_id();
	operator rx_id::id();
};

rx_id::id message(CircBufferB& cbuffer);

bool check_crc(RxMessage& message);


#endif /* AVR_MESSAGE_RECEIVER_BOOTLOADER_AVR_MESSAGE_RECEIVER_B_H_ */
