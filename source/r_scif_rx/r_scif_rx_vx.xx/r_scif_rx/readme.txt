PLEASE REFER TO THE APPLICATION NOTE FOR THIS DRIVER FOR MORE INFORMATION

r_scif_rx
=========

Overview
--------
The r_scif_rx module is a multi-channel, multi-mode, interrupt-driven driver 
which supports Asynchronous and Master Synchronous operation for the SCIFA 
peripheral. The API includes standard functions to initialize a channel and to 
send and receive data, as well as a special control function for taking actions 
such as issuing a break signal or enabling noise cancellation. The driver 
supports all channels available on the mcu. The driver can be reduced in size 
by removing code used for parameter checking, unused channels, or unused modes.
These configuration options can be found in "r_config\r_scif_rx_config.h".
An original copy of the configuration file is stored in 
"r_scif_rx\ref\r_scif_rx_config_reference.h".

Features
--------
* Simultaneous operation of up to 4 channels.
* Simultaneous operation of Async and Sync modes on different channels.
* Queueing of up to 2 messages for continuous streaming of tx and rx data.
* Interrupt driven.

File Structure
--------------
r_scif_rx
|   +-- readme.txt
|   +-- r_scif_rx_if.h
|
+---doc
|   +-- en
|   |    +-- r01an2222ej{VERSION_NUMBER}-rx.pdf
|   +-- ja
|        +-- r01an2222jj{VERSION_NUMBER}-rx.pdf
|
+---ref
|   +-- r_scif_rx_config_reference.h
|
+---src
    +-- r_scif_rx.c
    +-- r_scif_rx_data.c
    +-- r_scif_rx_private.h

r_config
    r_scif_rx_config.h
