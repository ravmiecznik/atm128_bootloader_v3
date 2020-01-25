# AVR MESSAGE SENDER
### Simplified message sender for bootloader purposes 

	/*
	 * Class to create and send message.
	 * |MESSAGE BODY|<TAIL>crc_tail
	 *
	 * When the object is destroyed the tail is sent
	 * which closes the message integrity
	 */

### How it works:
#### Message is sent when code exits the block, destructor attaches tail of message to close it.
```
{
	TxMessage(tx_id::txt)
}
```

#### If message must be a reply to a request message:
```
RxMessage rxmessage;
{
	TxMessage(tx_id::ack_feedback, rxmessage.header.context);
}
```

#### Attaching extra information to reply:
```
RxMessage rxmessage;
{
	TxMessage(tx_id::ack_feedback, rxmessage.header.context).sends("Extra info");
}

or
---------------------------------------------------------------------------------
RxMessage rxmessage;
{
	TxMessage txmessage(tx_id::ack_feedback, rxmessage.header.context);
	txmessage.fetch_str("Extra info 2")
	txmessage.fetch_str("Extra info 3")
}

```

#### Explicit message closure
```
int main(){
	while(true){
		RxMessage rxmessage(cbuffer);
		rx_id::id msg_id = rxmessage.msg_id();
		if(msg_id != rx_id::fail){
			TxMessage txmessage(tx_id::ack_feedback, rxmessage.header.context);
			txmessage.send_tail();
		}
	}

}
```
