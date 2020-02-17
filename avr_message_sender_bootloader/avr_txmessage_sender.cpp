/*
 * avr_message_sender.cpp
 *
 *  Created on: 11 wrz 2019
 *      Author: rafal
 */


#include "avr_txmessage_sender.h"
#include <avr/crc16.h>
#include <string.h>
#include "../uart/uart_standard.h"

using namespace tx_id;


//***CONSTRUCTORS***
TxMessage::TxMessage(uint16_t msg_id, uint16_t context){
	tail.id = msg_id;
	tail.context = context;
	//_stdout.put	= &PUTCHAR;
}


TxMessage::TxMessage(){
	tail.id = tx_id::txt;
	tail.context = 0;
	//_stdout.put	= &PUTCHAR;
}

TxMessage::~TxMessage(){
	send_tail();
}

//***METHODS***
void TxMessage::_putchar(char c){
	uart1_putc(c);
}

void TxMessage::send(char* buffer, uint16_t amount){
	for(uint16_t i=0; i<amount; i++){
		tail.msg_len++;
		tail.msg_crc = _crc_xmodem_update(tail.msg_crc, *buffer);
		uart1_putc(*buffer);
		buffer++;				//pointer increment
	}
	send_tail();
}

void TxMessage::send(char c){
	tail.msg_len++;
	tail.msg_crc = _crc_xmodem_update(tail.msg_crc, c);
	uart1_putc(c);
	send_tail();
}


void TxMessage::fetch_str(char* buffer){
	while(*buffer){
		tail.msg_len++;
		tail.msg_crc = _crc_xmodem_update(tail.msg_crc, *buffer);
		uart1_putc(*buffer);
		buffer++;				//pointer increment
	}
}

void TxMessage::fetch_str_P(const char* buffer){
	register char c;
	while(c = pgm_read_byte_far(buffer++)){
		tail.msg_len++;
		tail.msg_crc = _crc_xmodem_update(tail.msg_crc, c);
		uart1_putc(c);
	}
}

void TxMessage::sends(char* buffer){
	fetch_str(buffer);
	send_tail();
}

TxMessage& TxMessage::sends_P(const char* buffer){
	fetch_str_P(buffer);
	send_tail();
	return *this;
}

void TxMessage::fetch_byte(uint8_t c){
	uart1_putc(c);
	tail.msg_len++;
	tail.msg_crc = _crc_xmodem_update(tail.msg_crc, c);
}

void TxMessage::send_tail(){
	/*
	 * Method to close the message by sending its tail
	 */
	uint16_t tail_crc = 0;
	uint8_t* tail_crc_p = (uint8_t*)&tail_crc;
	char c;
	if(not tail_was_sent){
		tail_was_sent = true;
		uint8_t* p_tail = (uint8_t*)tail;
		for(uint8_t i=0; i<tail_size; i++){
			c = *p_tail++;
			_putchar(c);
			tail_crc = _crc_xmodem_update(tail_crc, c);
		}
		for(uint8_t i=0; i<2; i++){
			_putchar(tail_crc_p[i]);
		}
	}
}

uint8_t TxMessage::tail_size = sizeof(Tail);

