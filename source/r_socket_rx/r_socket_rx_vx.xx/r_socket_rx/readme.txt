PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_socket_rx
=========

Document Number 
---------------
R20AN0296JJ0131-RX-T4
R20AN0296EJ0131-RX-T4

Version
-------
v1.31

Overview
--------
The Socket API for Renesas TCP/IP stack M3S-T4-Tiny(T4).

These configuration options can be found in "r_config\r_socket_rx_config.h". 
An original copy of the configuration file is stored in 
"r_socket_rx\ref\r_socket_rx_config_reference.h".


Features
--------
* Add Socket API to T4.

Supported MCUs
--------------
* RX family

Boards Tested On
----------------
* RSKRX64M
* RSKRX63N
* RSKRX62N
* RSKRX71M
* RSKRX65N

Limitations
-----------
None

Peripherals Used Directly
-------------------------
None

Required Packages
-----------------
r_t4_rx
r_t4_driver_rx

How to add to your project
--------------------------
Please refer to the Adding Firmware Integration Technology Modules to Projects.
"r01an1723ju0111_rx.pdf/r01an1826jj0102_rx.pdf(R20AN0296JJ0131-RX-T4)"
"r01an1723eu0111_rx.pdf/r01an1826ej0102_rx.pdf(R20AN0296EJ0131-RX-T4)".

Toolchain(s) Used
-----------------
* Renesas RX Compiler V.2.05.00

File Structure
--------------
r_socket_rx
|   readme.txt
|	r_socket_rx_if.h
|
+---doc
|   |
|   +---ja
|   |     r20an0296jj0131-rx-t4.pdf
|   +---en
|         r20an0296ej0131-rx-t4.pdf
|
+---src
|       r_errno.h
|       r_socket.c
|       r_socket_par_check.c
|       r_socket_par_check.h
|
+---ref
        r_socket_rx_config_reference.h

r_config
        r_socket_rx_config.h
