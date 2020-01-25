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

void write_page_to_flash_mem(uint32_t strona, uint8_t *buf) BOOTLOADER_SECTION;
void write_packet_to_flash_mem(RxMessage rxmessage);

#endif /* BOOTLOADER_H_ */
