/*
 * bootloader.h
 *
 *  Created on: 18 gru 2019
 *      Author: rafal
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include <avr/boot.h>
#include "circbuffer_bootloader/cirbuffer_b.h"
#include "avr_message_receiver_bootloader/avr_message_receiver_b.h"
#include "avr_message_sender_bootloader/avr_txmessage_sender.h"
#include <avr/pgmspace.h>

#define LED_RED PD6
#define LED_BLUE PD7
#define	BOOTLOADER_PINE PE4
#define	BOOTLOADER_PINB PB1
#define TOGGLE(PORT, PIN) PORT ^= _BV(PIN);
#define PIN_HI(PORT, PIN) PORT |= _BV(PIN);
#define PIN_LO(PORT, PIN) PORT &= ~_BV(PIN);
#define PIN_VAL(PORT, PIN) (PORT & _BV(PIN))
#define MOSI_PIN PE0

void write_page_to_flash_mem(uint32_t page, uint8_t *buf) BOOTLOADER_SECTION;
void write_packet_to_flash_mem(RxMessage rxmessage);

//this macro will create string in PGMSPACE with the same content as name like: rav = "rav"
#define P(_NAME) const char BOOTLOADER_SECTION PROGMEM _NAME [] = #_NAME

//this macro will create only PGM array name (PROGMEM STRING ARRAY)
#define PSA(_NAME) const char PROGMEM _NAME []

P(bootloader3);
PSA(starting_main_app) = "STARTING MAIN APP";
PSA(welcome_msg) = ""
		"\n-----BOOTLOADER3-----\n"
		"Compilation date:\n";
PSA(compilation_date) = __DATE__;

#endif /* BOOTLOADER_H_ */
