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
#include <stdio.h>


/****************************************************************/
/*
 * Configuration of standard output for functions printf, printf_p and so on
 */
static int put(char c, FILE *stream){
	uart1_putc(c);
	return 0;
}
static FILE uartout = {0};
void setup_stdout_for_printf(){
	fdev_setup_stream(&uartout, put, NULL, _FDEV_SETUP_WRITE);
	stdout = &uartout;
	stderr = &uartout;
}
/****************************************************************/


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
	setup_stdout_for_printf();
	uart1_init(115200);
	//_delay_ms(500);
	CircBufferB cbuffer;

	//sei();

#if NEW_BOARD_VER == 1
		//new board
		if(PINE & _BV(BOOTLOADER_PINE))
#else
		//old board
		if((PINB & _BV(BOOTLOADER_PINB)))
#endif

		{
			uint16_t cnt=0;
			PIN_HI(PORTD, LED_RED);
			_delay_ms(50);
			PIN_LO(PORTD, LED_RED);
			while(1){
				//MAIN HERE
				if(not(cnt%10000))
					PORTD ^= _BV(LED_BLUE);
				cnt++;

				if(uart1_available()){
					if(cbuffer.put(uart1_getc()) == false){
						TOGGLE(PORTD, LED_RED);
					}
				}

				//periodic check of cbuffer
				if((not (cnt%50000)) and cbuffer.available()){
					RxMessage rxmessage(cbuffer);
					rx_id::id msg_id = rxmessage.msg_id();
					switch (msg_id) {
//						case rx_id::fail:
//							printf("fail\n");
//							break;
//						case rx_id::write_at:
//							printf("write at\n");
//							break;
						default:
							cbuffer.flush(sizeof(MessageHeader));
							printf("\n");
							for(uint8_t i=0; i< rxmessage.header.msg_len; i++){
								printf("%c", cbuffer.get());
							}
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
