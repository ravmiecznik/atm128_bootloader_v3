/*
 * main.cpp
 *
 *  Created on: 18 gru 2019
 *      Author: rafal
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/delay.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include "bootloader.h"
#include "uart/uart_standard.h"
//#include <avr/interrupt.h>

#include "setup.h"
#include "circbuffer_bootloader/cirbuffer_b.h"
#include "avr_message_sender_bootloader/avr_txmessage_sender.h"



typedef void (*main_app)();

void print(const char* str){
	uart1_puts(str);
}

void print_P(const char* str){
	uart1_puts_p(str);
}

void handshake(){
	TxMessage(tx_id::txt).sends_P(bootloader3).send_tail();
	{
		_delay_ms(500);
		TxMessage welcome_txmsg;
		welcome_txmsg.fetch_str_P(welcome_msg);
		welcome_txmsg.fetch_str_P(compilation_date);
	}
}

//bootloader flag must be at address 0 in EEPROM
uint8_t EEMEM BOOTLOADER_FLAG = true;

int main(){
	//LEDS configuration
	main_app main_app_ptr = (main_app)0;
	uint8_t bootloader_flag = eeprom_read_byte(&BOOTLOADER_FLAG);
	DDRD |= _BV(LED_RED) | _BV(LED_BLUE);
	PIN_HI(PORTD, LED_BLUE);
#if NEW_BOARD_VER == 1
	PIN_LO(PINE, BOOTLOADER_PINE);
#else
	PIN_LO(PINB, BOOTLOADER_PINB);
#endif
#if NEW_BOARD_VER == 1
	//new board pull up to activate bootloader
	DDRE &= ~_BV(BOOTLOADER_PINE);
	PIN_LO(PORTE, BOOTLOADER_PINE);
#else
	//old board version: pull down to activate bootloader
	DDRB &= ~_BV(BOOTLOADER_PINB);
	PIN_HI(PORTB, BOOTLOADER_PINB);
#endif


#if NEW_BOARD_VER == 1
		//new board
		if(bootloader_flag or PINE & _BV(BOOTLOADER_PINE))
#else
		//old board
		//if((PINB & _BV(BOOTLOADER_PINB)) or bootloader_flag)
			if(bootloader_flag or (PINB & _BV(BOOTLOADER_PINB)))
#endif

		{

			uart1_init(115200);
			CircBufferB cbuffer;
			handshake();
			uint32_t cnt = 0;
			PIN_HI(PORTD, LED_RED);
			_delay_ms(50);
			PIN_LO(PORTD, LED_RED);
			while(1){
				//MAIN LOOP HERE
				if(not(cnt%20000)){
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

					if(cbuffer.available() >= rxmessage.header.msg_len and check_crc(rxmessage)){
						//command handler
						switch (msg_id) {
							case rx_id::fail:
								TxMessage(tx_id::nak_feedback);
								break;
							case rx_id::write_at:
							{
								write_packet_to_flash_mem(rxmessage);
								cbuffer.flush();
							}
								break;
							case rx_id::bootloader:
								handshake();
								cbuffer.flush();
								break;
							case rx_id::run_main_app:
							{
								TxMessage(tx_id::txt).sends_P(starting_main_app);
								eeprom_write_byte(&BOOTLOADER_FLAG, false);
								main_app_ptr();
							}
								break;
							default:
								cbuffer.flush();
								TxMessage(tx_id::dtx, rxmessage.header.context);
								break;
						}
					}
					//else check crc
					else{
						TxMessage(tx_id::nak_feedback, rxmessage.header.context).sends("mainloop");
						cbuffer.flush();
					}
				}
			}
		}
		else
		{
			PIN_LO(PORTD, LED_RED);
			PIN_LO(PORTD, LED_BLUE);
			main_app_ptr();
		}



	return 0;
}
