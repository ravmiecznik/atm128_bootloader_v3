/*
 * cirbuffer_b.h
 *
 *  Created on: 26 gru 2019
 *      Author: rafal
 */

#ifndef CIRCBUFFER_BOOTLOADER_CIRBUFFER_B_H_
#define CIRCBUFFER_BOOTLOADER_CIRBUFFER_B_H_

#include <stdint.h>
#include "../setup.h"

#define CBUFFER_SIZE (SINGLE_PACKET_SIZE + 30)

class CircBufferB {
public:
	uint8_t* buffer;
	uint16_t head;
	uint16_t tail;
	uint16_t _available;
public:
	CircBufferB();
	CircBufferB(CircBufferB* cbuffer_ptr);
	bool put(uint8_t byte);
	uint8_t get();
	uint16_t available();
	uint16_t free_space();
	void flush();
	void flush(uint32_t amount);
	void peek_sync(CircBufferB* peek_cbuffer);
	CircBufferB peek();
	uint16_t get_uint16t();
};


#endif /* CIRCBUFFER_BOOTLOADER_CIRBUFFER_B_H_ */
