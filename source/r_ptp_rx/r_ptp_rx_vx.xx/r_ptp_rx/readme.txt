PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_ptp_rx
=======================

Document Number 
---------------
R01AN1943EJ0116 (English version)
R01AN1943JJ0116 (Japanese version)

Version
-------
v1.16

Overview
--------
The EPTPC FIT module makes the time synchronization using the PTP function of the EPTPC peripheral module (EPTPC). 
This module also supports enhanced functions of the standard Ethernet such as the simple switch 
and multicast frame filter functions.

Features
--------
* Time synchronization based on the PTP (Precision Time Protocol) defined by the IEEE1588-2008 specification
* PTP frame operation
* HW simple switch function
* HW multicast frame filter function.

Supported MCUs
--------------
* RX64M Group
* RX71M Group
* RX72M Group

Boards Tested On
----------------
* RSKRX64M
* RSKRX71M
* RSKRX72M
 
Limitations
-----------
* None

Peripherals Used Directly
-------------------------
* PTP Module for the Ethernet Controller (EPTPC)
* PTP Ethernet Controller Direct Memory Access Controller (PTPEDMAC)

Required Packages
-----------------
* r_bsp
* r_ether_rx

How to add to your project
--------------------------
Please refer to the following documents;
* r01an1723eu0121_rx.pdf and r01an1826ej0110_rx.pdf (English version)
* r01an1723ju0121_rx.pdf and r01an1826jj0110_rx.pdf (Japanese version)

Toolchain(s) Used
-----------------
* Renesas RX Toolchain v3.01.00

File Structure
--------------
r_ptp_rx
|   readme.txt
|   r_ptp_rx_if.h
|   r_ptpif_rx_if.h
|
+---doc
|   |
|   +---en
|       r01an1943ej0116-rx-eptpc.pdf
|   |
|   +---ja
|       r01an1943jj0116-rx-eptpc.pdf
|
+---ref
|       r_ptp_rx_config_reference.h
|
\---src
|       r_ptp.c
|       r_ptp_rx_private.h
|       r_ptpif.c
|       r_ptpif_rx_private.h
| \---targets
| |
| +---rx
| | r_ptp_dev.c
| | r_ptpif_dev.h

