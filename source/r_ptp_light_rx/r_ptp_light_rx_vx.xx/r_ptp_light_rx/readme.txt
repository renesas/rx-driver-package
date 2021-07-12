PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_ptp_light_rx
=======================

Document Number 
---------------
R01AN3035EJ0114 (English version)
R01AN3035JJ0114 (Japanese version)

Version
-------
v1.14

Overview
--------
The PTP light FIT Module is reduced the PTP (Precision Time Protocol) function defined by the IEEE1588-2008 
specification and focus on the enhanced standard Ethernet function such as the simple switch and multicast 
frame filter functions.

Features
--------
* HW simple switch function
* HW multicast frame filter function.

Supported MCUs
--------------
* RX64M Group
* RX71M Group
* RX72M Group
* RX72N Group

Boards Tested On
----------------
* RSKRX64M
* RSKRX71M
* RSKRX72M
* RSKRX72N

Limitations
-----------
* Not support PTP time synchronization.
* Cannot use PTP FIT module (full version) simultaneously.
* Cannot receive and process the PTP message frames. (Relay control is possible)

Peripherals Used Directly
-------------------------
* PTP Module for the Ethernet Controller (EPTPC)

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
* Renesas RX v3.01.00
* GCC for Renesas RX 4.8.4
* IAR EWRX 4.12.1

File Structure
--------------
r_ptp_light_rx
|   readme.txt
|   r_ptp_light_rx_if.h
|
+---doc
|   |
|   +---en
|       r01an3035ej0113-rx-eptpc.pdf
|   |
|   +---ja
|       r01an3035jj0113-rx-eptpc.pdf
|
+---ref
|       r_ptp_light_rx_config_reference.h
|
\---src
|       r_ptp_light.c
|       r_ptp_light_rx_private.h
|
| \---targets
| |
| +---rx
| | r_ptp_light_dev.c

