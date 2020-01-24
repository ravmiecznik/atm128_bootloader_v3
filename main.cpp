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

void print(const char* str){
	uart1_puts(str);
}


int main(){
	//LEDS configuration
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

#if NEW_BOARD_VER == 1
		//new board
		if(PINE & _BV(BOOTLOADER_PINE))
#else
		//old board
		if((PINB & _BV(BOOTLOADER_PINB)))
#endif

		{
			uint32_t cnt=0;
			PIN_HI(PORTD, LED_RED);
			_delay_ms(50);
			PIN_LO(PORTD, LED_RED);
			while(1){
				//MAIN LOOP HERE
				if(not(cnt%10000)){
					PORTD ^= _BV(LED_BLUE);
				}
				cnt++;

				if(uart1_available()){
					if(cbuffer.put(uart1_getc()) == false){
						TOGGLE(PORTD, LED_RED);
					}
				}

				//periodic check of cbuffer
				if((not (cnt%5000)) and cbuffer.available()){
					RxMessage rxmessage(cbuffer);
					rx_id::id msg_id = rxmessage.msg_id();
					switch (msg_id) {
						case rx_id::fail:
							print("nak\n");
							break;
						case rx_id::write_at:
							//write_page_to_flash_mem(0, (uint8_t*)"");
							print("ack\n");
							break;
						case rx_id::bootloader:
							TxMessage(tx_id::txt, rxmessage.header.context).sends("bootloader3\n");
							cbuffer.flush();
							break;
						default:
							cbuffer.flush(sizeof(MessageHeader));
							print("dtx\n");
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
