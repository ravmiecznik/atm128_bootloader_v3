/*
 * main.cpp
 *
 *  Created on: 18 gru 2019
 *      Author: rafal
 */

#include <avr/delay.h>
#include "bootloader.h"
#include "uart/uart_standard.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "setup.h"
#include "circbuffer_bootloader/cirbuffer_b.h"
#include "avr_message_sender_bootloader/avr_txmessage_sender.h"
#include <stdio.h>
#include <stdlib.h>


typedef void (*main_app)();

void print(const char* str){
	uart1_puts(str);
}


int main(){
	//LEDS configuration
	char bootloader_handshake[] = "bootloader3";
	DDRD |= _BV(LED_RED) | _BV(LED_BLUE);
	PIN_HI(PORTD, LED_BLUE);
#if NEW_BOARD_VER == 1
	//new board pull up to activate bootloader
	DDRE &= ~_BV(BOOTLOADER_PINE);
	PIN_LO(PORTE, BOOTLOADER_PINE);
#else
	//old board version: pull down to activate bootloader
	DDRB &= ~_BV(BOOTLOADER_PINB);
	PIN_HI(PORTB, BOOTLOADER_PINB);
#endif

	uart1_init(115200);
	CircBufferB cbuffer;
	TxMessage(tx_id::txt).sends(bootloader_handshake);
#if NEW_BOARD_VER == 1
		//new board
		if(PINE & _BV(BOOTLOADER_PINE))
#else
		//old board
		if((PINB & _BV(BOOTLOADER_PINB)))
#endif

		{
			uint32_t cnt = 0;
			PIN_HI(PORTD, LED_RED);
			_delay_ms(50);
			PIN_LO(PORTD, LED_RED);
			while(1){
				//MAIN LOOP HERE
				if(not(cnt%10000)){
					PORTD ^= _BV(LED_BLUE);
				}
				cnt++;

				//receive data from uart
				while(uart1_available()){
					cbuffer.put(uart1_getc());
				}

				//periodic check of cbuffer
				if((not (cnt%5000)) and cbuffer.available()){
					RxMessage rxmessage(cbuffer);
					rx_id::id msg_id = rxmessage.msg_id();

					//command handler
					switch (msg_id) {
						case rx_id::fail:
							TxMessage(tx_id::nak_feedback);
							break;
						case rx_id::write_at:
						{
							write_packet_to_flash_mem(rxmessage);
						}
							break;
						case rx_id::bootloader:
							TxMessage(tx_id::txt, rxmessage.header.context).sends(bootloader_handshake);
							cbuffer.flush();
							break;
						case rx_id::run_main_app:
						{
							TxMessage(tx_id::txt).sends("STARTING MAIN APP");
							main_app main_app_ptr = (main_app)0;
							main_app_ptr();
						}
							break;
						default:
							cbuffer.flush(sizeof(MessageHeader));
							TxMessage(tx_id::dtx, rxmessage.header.context);
							break;
					}
				}
			}
		}
		else
		{
			PIN_LO(PORTD, LED_RED);
		}



	return 0;
}
