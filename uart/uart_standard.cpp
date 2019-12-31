/*
 * uart_standard.cpp
 *
 *  Created on: Jan 22, 2017
 *      Author: rafal
 */

#include "uart_standard.h"
#include <avr/pgmspace.h>
#include "../bootloader.h"
#include <avr/interrupt.h>

volatile unsigned char value;

uint8_t buffer;

void uart1_init(uint32_t baud){
   // Set baud rate
   UBRR1L = BAUD_PRESCALE(baud);// Load lower 8-bits into the low byte of the UBRR register
   UBRR1H = (BAUD_PRESCALE(baud) >> 8);
	 /* Load upper 8-bits into the high byte of the UBRR register
    Default frame format is 8 data bits, no parity, 1 stop bit
  to change use UCSRC, see AVR datasheet*/

  // Enable receiver and transmitter and receive complete interrupt
  UCSR1B = ((1<<TXEN1) | (1<<RXEN1) | (1<<RXCIE1));
}

void uart1_puts(const char* s){
	while (*s) {
		uart1_putc(*s++);
	}
}

void uart1_puts_p(const char *progmem_s )
{
	register char c;

	while ( (c = pgm_read_byte(progmem_s++)) ) {
		uart1_putc(c);
	}

} /* uart0_puts_p */

void uart1_putc(uint8_t u8Data){

  // Wait until last byte has been transmitted
  while((UCSR1A &(1<<UDRE)) == 0);

  // Transmit data
  UDR1 = u8Data;
}


// not being used but here for completeness
      // Wait until a byte has been received and return received data
uint8_t uart1_getc(){
  //while((UCSR1A &(1<<RXC)) == 0);
  return UDR1;
}

uint8_t uart1_available(){
	return (UCSR1A &(1<<RXC1));
}

void uart1_flush(){
	while(uart1_available())
		uart1_getc();
}

ISR(USART1_RX_vect){
	PIN_HI(PORTD, LED_RED);
	//PIN_HI(PORTD, LED_BLUE);
	volatile uint8_t data;
	data = UDR1;
//	buffer = data;
	//UCSR1B &= ~(1<<RXCIE);


}




