/*
 * avr_message_receiver_b.cpp
 *
 *  Created on: 25 sty 2020
 *      Author: rafal
 */

#include "avr_message_receiver_b.h"
#include <avr/crc16.h>


RxMessage::RxMessage(CircBufferB& cbuffer):header((MessageHeader&)*_header), buffer(cbuffer){
	CircBufferB peek = cbuffer.peek();
	if(not check_header(peek)){
		cbuffer.flush();
		header.id = rx_id::fail;
	}
	else{
		buffer.flush(sizeof(MessageHeader));
	}
}

rx_id::id RxMessage::msg_id(){
	return (rx_id::id)header.id;
}

bool RxMessage::check_header(CircBufferB& peek){
	if(peek.available() >= sizeof(MessageHeader)){
		//find header start point
		_header[0] = 0;
		_header[sizeof(MessageHeader)-1] = 0;
		while(peek.available() >= sizeof(MessageHeader)){
			_header[0]=peek.get();
			//header start mark match
			if(_header[0] == HEADER_START){
				//collect header
				for(uint8_t i=1; i<sizeof(MessageHeader); i++){
					_header[i] = peek.get();
				}
				//check header integrity
				if( (_header[sizeof(MessageHeader)-1] == HEADER_END))
					return true;
			}
		}
		return false;
	}
	return false;
}

//rx_id::id message(CircBufferB& cbuffer){
//	CircBufferB peek(cbuffer);
//
//	return rx_id::fail;
//}

RxMessage::operator rx_id::id(){
	return (rx_id::id)header.id;
}

bool check_crc(RxMessage& message){
	/*
	 * Msg should be at relational pos 0 (not absolute) in cbuffer
	 */
	//CircBufferB& buffer = message.buffer;
	CircBufferB peek = message.buffer.peek();
	uint32_t cnt = message.header.msg_len;
	uint16_t calc_crc = 0;
	char c;

	//buffer.peek_sync(&peek);
	while(cnt--){
		c = peek.get();
		//print_buff_char(c);
		calc_crc = _crc_xmodem_update(calc_crc, c);
	}
	if(calc_crc != message.header.crc){
		message.header.id = rx_id::fail;
	}
	return calc_crc == message.header.crc;
}
