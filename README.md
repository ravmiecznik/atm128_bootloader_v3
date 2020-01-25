# Next version of bootloader.

```
Intention of this rework is to acquire a bootloader more robust for transmission errors.
Use idea of RxMessage implemented in EMUBT, the Reflasher application will keep 100% control of writing packets,
it will relay only on Ack/Nak/Dtx responses, if no Ack was received it will retransmit unitl success with 
maximum number of Retx.
```

```
TODO:   32.12.2019
        RxMessage must calculate CRC
        TxMessage must be implemented
        After removal of printf of stdout.h implement puts function in uart, removal of printf frees extra 2kbytes of program size
        25.01.2020
        Support bootloader_activate flag in eeprom
        Update write_to_flash_mem function pointer, it was changed after Bootloader rework
```
        
* *31.12.2019: -RxMessage and CircBuffer design for new bootloader works*
* *24.01.2020: -TxMessage added, replace all printf with simple print function*
* *25.01.2020: -old TODO tasks done, tests performed, stable version*
