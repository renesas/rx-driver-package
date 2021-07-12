PLEASE REFER TO THE APPLICATION NOTE FOR THIS MODULE FOR MORE INFORMATION

r_uid_rx
=================

Document Number 
---------------
r01an2191ej0114
r01an2191jj0114

Version
-------
v1.14

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
* r_bsp      V5.52

Toolchain(s) Used
-----------------
* Renesas RX v3.02.00
* GCC for Renesas RX 8.3.0.201904
* IAR C/C++ Compiler for Renesas RX version 4.14.1


File Structure
--------------
r_uid_rx
|   readme.txt
|   r_uid_rx_if.h
|
+---doc
|   +---en
|   |   r01an2191ej0114-rx-uid.pdf
|   |
|   +---ja
|       r01an2191jj0114-rx-uid.pdf
|
+---src
    +-- r_uid_rx.c
    +-- r_uid_rx.h
    +-- r_uid_ram.c
    +-- r_uid_ram.h
 

