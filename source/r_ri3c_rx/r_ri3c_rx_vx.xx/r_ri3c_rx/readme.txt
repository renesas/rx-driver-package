PLEASE REFER TO THE APPLICATION NOTE OF THIS MIDDLEWARE FOR MORE INFORMATION

r_ri3c_rx
=========

Overview
--------
The RI3C FIT Module provides a method to transmit and receive data between
the controller and slave device using the I3C-bus interface (RI3C).

The driver supports one channels available on the MCU.
The driver can be reduced in size by removing code used by unset parameter checking or controller/target supports.
These configuration options can be found in "r_config\r_ri3c_rx_config.h".

Features
--------

* Controller transmission, controller reception, target transmission, and target reception.
* Multi-controller configuration which is that conmmunicatioon is arbitrated between multiple controllers and a single target.
* In-band interrupt (IBI).
* Communication mode can be standard mode, fast mode, fast mode plus or high-speed mode. And the conmmunication rate is 3 Mbps at maximum in RX26T.

File Structure
--------------
r_ri3c_rx
|   readme.txt
|   r_ri3c_rx_api.h
|   r_ri3c_rx_data.h
|   r_ri3c_rx_if.h
|   r_ri3c_rx_reg.h
|
+---doc
|   |
|   +---en
|   |   r01an{MODULE_CODE}es{VERSION_NUMBER}-rx-ri3c.pdf
|   |
|   +---ja
|       r01an{MODULE_CODE}js{VERSION_NUMBER}-rx-ri3c.pdf
|
+---src
    |   r_ri3c_rx.c
    |   r_ri3c_rx_data.h

r_config
    r_ri3c_rx_config.h