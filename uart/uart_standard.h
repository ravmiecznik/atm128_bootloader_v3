/*
 * uart_standard.h
 *
 *  Created on: Jan 22, 2017
 *      Author: rafal
 */

#ifndef UART_STANDARD_H_
#define UART_STANDARD_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD_PRESCALE(USART_BAUDRATE) (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void uart1_init(uint32_t baud);


void uart1_putc(uint8_t u8Data);

void uart1_puts(const char*);
void uart1_puts_p(const char *progmem_s );

// not being used but here for completeness
      // Wait until a byte has been received and return received data
uint8_t uart1_getc();

uint8_t uart1_available();

void uart1_flush();

#endif /* UART_STANDARD_H_ */
