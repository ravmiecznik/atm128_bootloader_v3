/*
 * avr_message_sender.h
 *
 *  Created on: 11 wrz 2019
 *      Author: rafal
 */

#ifndef AVR_MESSAGE_SENDER_AVR_TXMESSAGE_SENDER_H_
#define AVR_MESSAGE_SENDER_AVR_TXMESSAGE_SENDER_H_

#include <stdint.h>
#include "../setup.h"
#include <avr/pgmspace.h>

#define TAIL_START_MARK	'<'
#define TAIL_END_MARK	'>'




namespace tx_id {
	enum id{
		ack_feedback,
		nak_feedback,
		dtx,
		txt,
		dbg,
		digidiag_frame,
		pin_change_pending,
	};
}

struct Tail{
	uint8_t 	tail_start=	TAIL_START_MARK;	//'>'
	uint16_t	id=			0;
	uint16_t	context=	0;
	uint16_t 	msg_len=	0;
	uint16_t	msg_crc=	0;
	uint8_t		tail_end=	TAIL_END_MARK;	//'<'

	operator uint8_t* (){
		return &tail_start;
	}
};

class TxMessage{
	/*
	 * Class to create and send message.
	 * |MESSAGE BODY|<TAIL>crc_tail
	 *
	 * When the object is destroyed the tail is sent
	 * which closes the message integrity
	 *
	 */
private:
	void 			_putchar(char);
	static uint8_t 	tail_size;
	Tail 			tail;
	bool			tail_was_sent = false;
public:

	TxMessage(uint16_t msg_id, uint16_t context=0);
	TxMessage();
	~TxMessage();
	void 	send(char*, uint16_t);
	void 	send(char c);
	void 	sends(char*);
	TxMessage& 	sends_P(const char*);
	void	fetch_str(char*);
	void 	fetch_str_P(const char* pgmaddr);
	void 	fetch_byte(uint8_t c);
	void	send_tail();
};




#endif /* AVR_MESSAGE_SENDER_AVR_TXMESSAGE_SENDER_H_ */
