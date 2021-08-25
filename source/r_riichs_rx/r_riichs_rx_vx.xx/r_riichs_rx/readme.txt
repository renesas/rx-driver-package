PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_riichs_rx
=========

Overview
--------
The RIICHS fit module provides a method to transmit and receive data between 
the master and slave devices using the I2C-bus interface (RIICHS).
The RIICHS is in compliance with single master mode of the XP I2C-bus (Inter-IC-Bus) interface.

The driver supports one channels available on the mcu.
The driver can be reducedin size by removing code used for parameter checking orfor unused channels.
These configuration options can be found in "r_config\r_riichs_rx_config.h".
An original copy of the configuration file is stored in "r_riichs_rx\ref\r_riichs_rx_config_reference.h".

Features
--------
* Master transmission, master reception, slave transmission, and slave reception.
* Multi-master configuration which is that communication is arbitrated between multiple masters and a single slave.
* Communication mode can be standard or fast mode and the maximum communication rate is 400 kbps. 
* Channel 0 of RX671 supports fast mode plus(maximum communication rate is 1 Mbps) and high speed mode(maximum communication rate is 3.4 Mbps).


File Structure
--------------
r_riichs_rx
|   readme.txt
|   r_riichs_rx_if.h
|
+---doc
|   |
|   +---en
|   |   r01an5552ej{VERSION_NUMBER}-rx-riichs.pdf
|   |
|   +---ja
|       r01an5552jj{VERSION_NUMBER}-rx-riichs.pdf
|
+---ref
|       r_riichs_rx_config_reference.h
|       r_riichs_rx_pin_config_reference.h
|
+---src
    |   r_riichs_rx.c
    |   r_riichs_rx_private.h
    |
    +---targets
        |
        +---rx671
               r_riichs_rx671.c
               r_riichs_rx671_private.h

r_config
    r_riichs_rx_config.h
    r_riichs_rx_pin_config.h
