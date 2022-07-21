PLEASE REFER TO THE APPLICATION NOTE FOR THIS DRIVER FOR MORE INFORMATION

r_rsci_rx
========

Overview
--------------------------------------------------------------------------------
The r_rsci_rx module is a multi-channel, multi-mode, interrupt-driven driver which
supports Asynchronous, Master Synchronous, Single Master Simple SPI (SSPI), and
Manchester operation for the RSCI peripherals. The API includes standard functions 
to initialize a channel and to send and receive data, as well as a special control 
function for taking actions such as issuing a break signal or enabling noise 
cancellation. The driver supports all channels available on the mcu. The driver 
can be reduced in size by removing code used for parameter checking, unused 
channels, or unused modes. These configuration options can be found in 
"r_config\r_rsci_rx_config.h".


Features
--------
* (RX671) Simultaneous operation of up to 2 channels
* (RX660) Simultaneous operation of up to 2 channels
* Simultaneous operation of Async, Sync, SSPI or Manc modes on different channels.
* Queueing of incoming and outgoing data for Asynchronous or Manchester channels.
* Interrupt driven.


File Structure
--------------
r_rsci_rx
|   readme.txt
|   r_rsci_rx_if.h
|
+---doc
|   +---ja
|   |    r01an5759js{VERSION_NUMBER}-rx-rsci.pdf
|   +---en
|        r01an5759es{VERSION_NUMBER}-rx-rsci.pdf
|
+---src
    |   r_rsci_rx.c
    |   r_rsci_rx_platform.h
    |   r_rsci_rx_private.h
    |
    +---targets
        |   
        +---rx671
        |       r_rsci_rx671.c
        |       r_rsci_rx671_data.c
        |       r_rsci_rx671_private.h
        +---rx660
        |       r_rsci_rx660.c
        |       r_rsci_rx660_data.c
        |       r_rsci_rx660_private.h
        :

r_config
    r_rsci_rx_config.h

r_rsci_rx.ftl

