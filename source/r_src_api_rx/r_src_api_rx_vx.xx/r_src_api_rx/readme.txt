PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_src_api_rx
=======================

Document Number 
---------------
r01an2090jj0113
r01an2090ej0113


Version
-------
v1.13

Overview
--------
This is a FIT compliant module for RX's SRC peripheral.
It provides ways of sampling rate conversion using the SRC (Sampling Rate Converter) peripheral.

Features
--------
* PCM data sampling rate conversion with SRC peripheral.

Supported MCUs
--------------
* RX64M Group
* RX71M Group

Boards Tested On
----------------
* RSKRX64M
* RSKRX71M
 
Limitations
-----------
* None

Peripherals Used Directly
-------------------------
* SRC (Smapling Rate Converter)

Required Packages
-----------------
* r_bsp     v5.20

How to add to your project
--------------------------
* Add src\r_src_api_rx.c to your project.
* Add an include path to the 'r_src_api_rx' directory. 
* Add an include path to the 'r_src_api_rx\src' directory.
* Copy r_src_api_rx_config_reference.h from 'ref' directory to your desired location and rename to r_src_api_rx_config.h.
* Configure this API through r_src_api_rx_config.h.
* Add a #include for r_src_api_rx_if.h in any source file that needs to use this API.

Toolchain(s) Used
-----------------
* Renesas RX v3.01.00
* GCC for Renesas RX 4.8.4.201801
* IAR C/C++ Compiler for Renesas RX version 4.10.1

File Structure
--------------
r_src_api_rx
|  readme.txt
|  r_src_api_rx_if.h
|
+---doc
|   +---en
|   |   r01an2090ej0113-rx-src.pdf
|   |
|   +---ja
|       r01an2090jj0113-rx-src.pdf
|
+---ref
|  r_src_api_rx_config_reference.h
|
\---src
   r_src_api_rx.c
   r_src_api_rx_coef_table.c
   r_src_api_rx_private.h

r_config
    r_sci_iic_rx_config.h

