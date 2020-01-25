/*
 * circbuffer_bootloader.cpp
 *
 *  Created on: 26 gru 2019
 *      Author: rafal
 */
#include <stdlib.h>
#include "cirbuffer_b.h"


CircBufferB::CircBufferB(): head(0), tail(0), _available(0){
	buffer = (uint8_t*)malloc(CBUFFER_SIZE);
}

CircBufferB::CircBufferB(CircBufferB* cbuffer_ptr){
	head = cbuffer_ptr->head;
	tail = cbuffer_ptr->tail;
	_available = cbuffer_ptr->_available;
	buffer = cbuffer_ptr->buffer;
}

bool CircBufferB::put(uint8_t byte){
	/*
	 * Put single byte
	 */
	if(free_space() == 0){
		return false;
	}
	else{
		buffer[tail] = byte;
		tail++;
		tail = tail % CBUFFER_SIZE;
		_available++;
		return true;
	}

}

uint8_t CircBufferB::get(){
	/*
	 * Get single byte
	 */
	uint8_t byte;
	if(available()){
		byte = buffer[head];
		head++;
		head = head % CBUFFER_SIZE;
		_available--;
		return byte;
	}
	else{
		return 0;
	}
}

uint16_t CircBufferB::available(){
	return _available;
}

uint16_t CircBufferB::free_space(){
	/*
	 * Check free space
	 */
	return CBUFFER_SIZE - available();
}

void CircBufferB::flush(){
	head = 0;
	tail = 0;
	_available = 0;
}

void CircBufferB::flush(uint32_t amount){
	/*
	 * Flush cubuffer of 'amount' bytes
	 */
	while(amount--)
		get();
}

CircBufferB CircBufferB::peek(){
	/*
	 * Return CircBuffer created with pointer constructor
	 * It makes its own head, tail, available but points to common buffer
	 * In practice this is just freeze state of real CircBuffer can be used to peek
	 */
	return CircBufferB(this);
}

uint16_t CircBufferB::get_uint16t(){
	/*
	 * Get two bytes from cbuffer as uint16_t
	 */
	uint16_t value=0;
	for(uint8_t i=0; i<sizeof(uint16_t); i++){
		value += get()<<(8*i);
	}
	return value;
}
