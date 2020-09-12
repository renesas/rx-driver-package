PLEASE REFER TO THE APPLICATION NOTE FOR THIS MODULE FOR MORE INFORMATION

r_uid_rx
=================

Document Number 
---------------
r01an2191ej0113
r01an2191jj0113

Version
-------
v1.13

Overview
--------
This module implements the UniqueID Read API for the RX MCU's

Features
--------
* UniqueID Read

Supported MCUs
--------------
* RX110 Group
* RX111 Group
* RX113 Group
* RX130 Group
* RX13T Group

Boards Tested On
----------------
* RSKRX110
* RSKRX111
* RSKRX113
* RSKRX130
* RX13T CPU Card


Limitations
-----------
* None

Peripherals Used Directly
-------------------------
* None

Required Packages
-----------------
* r_bsp      V5.30

Toolchain(s) Used
-----------------
* Renesas RX v3.01.00
* GCC for Renesas RX 4.8.4.201801
* IAR C/C++ Compiler for Renesas RX version 4.10.1


File Structure
--------------
r_uid_rx
|   readme.txt
|   r_uid_rx_if.h
|
+---doc
|   +---en
|   |   r01an2191ej0113-rx-uid.pdf
|   |
|   +---ja
|       r01an2191jj0113-rx-uid.pdf
|
+---src
    +-- r_uid_rx.c
    +-- r_uid_rx.h
    +-- r_uid_ram.c
    +-- r_uid_ram.h
 

