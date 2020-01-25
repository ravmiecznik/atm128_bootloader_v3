/*
 * bootloader.cpp
 *
 *  Created on: 18 gru 2019
 *      Author: rafal
 */
#include "bootloader.h"
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/boot.h>



void write_page_to_flash_mem(uint32_t page, uint8_t *buf){
	uint16_t i;
	uint8_t sreg;

	sreg = SREG;	//save global interrupt flags
	cli();

	for (i=0; i<SPM_PAGESIZE; i+=2)
	{
		uint16_t word=*buf++;
		word+=(*buf++)<<8;
		boot_page_fill_safe(page+i, word);	//put data to buffer
	}
	boot_page_erase_safe(page);
	//boot_page_erase (page);

	boot_page_write_safe(page);     //write buffer to page

	boot_rww_enable_safe();			  //unlock RWW
	SREG = sreg;					  //restore SREG
}

void write_packet_to_flash_mem(RxMessage rxmessage){
	CircBufferB& cbuffer = rxmessage.buffer;
	if(cbuffer.available() >= rxmessage.header.msg_len){
		uint8_t* buffer = &cbuffer.buffer[sizeof(MessageHeader)];
		uint16_t packet_num = cbuffer.get_uint16t();
		for(uint16_t i=0; i<SINGLE_PACKET_SIZE; i+=SPM_PAGESIZE){
			write_page_to_flash_mem(packet_num*SINGLE_PACKET_SIZE + i, &cbuffer.buffer[sizeof(MessageHeader) + i + sizeof(uint16_t)]);
			TOGGLE(PORTD, LED_RED);
		}
		cbuffer.flush();
		PIN_LO(PORTD, LED_RED);
		TxMessage(tx_id::ack_feedback, rxmessage.header.context);
	}
	else{
		TxMessage(tx_id::dtx, rxmessage.header.context).sends("dtx");
	}
}




