Next version of bootloader.

Intention of this rework is to acquire a bootloader more robust for transmission errors.
Use idea of RxMessage implemented in EMUBT, the Reflasher application will keep 100% control of writing packets,
it will relay only on Ack/Nak/Dtx responses, if no Ack was received it will retransmit unitl success with 
maximum number of Retx.


31.12.2019: -RxMessage and CircBuffer design for new bootloader works

TODO:   RxMessage must calculate CRC
        TxMessage must be implemented
