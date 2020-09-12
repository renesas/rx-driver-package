PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_t4_driver_rx
=========

Document Number
---------------
R20AN0311JJ0108-RX-T4
R20AN0311EJ0108-RX-T4

Version
-------
v1.08

Overview
--------
Convert the TCP/IP(T4) - RX Ether Driver Interface

Features
--------
* Convert the TCP/IP(T4) - RX Ether Driver Interface

Supported MCUs
--------------
* RX Family

Boards Tested On
----------------
* RSKRX64M
* RSKRX65N

Limitations
-----------
None

Peripherals Used Directly
-------------------------
None

Required Packages
-----------------
None

How to add to your project
--------------------------
This module must be added to each project in which it is used.
Renesas recommends using "Smart Configurator" described in (1) or (3).
However, "Smart Configurator" only supports some RX devices.
Please use the methods of (2) or (4) for unsupported RX devices.

(1) Adding the FIT module to your project using "Smart Configurator" in e2 studio
By using the "Smart Configurator" in e2 studio, 
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(2) Adding the FIT module to your project using "FIT Configurator" in e2 studio
By using the "FIT Configurator" in e2 studio,
the FIT module is automatically added to your project.
Refer to "Adding Firmware Integration Technology Modules to Projects (R01AN1723)" for details.

(3) Adding the FIT module to your project using "Smart Configurator" on CS+
By using the "Smart Configurator Standalone version" in CS+,
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(4) Adding the FIT module to your project in CS+
In CS+, please manually add the FIT module to your project.
Refer to "Adding Firmware Integration Technology Modules to CS+ Projects (R01AN1826)" for details.

Toolchain(s) Used
-----------------
* Renesas RX Compiler V.3.01.00
* GCC for Renesas RX 4.8.4.201801
* IAR C/C++ Compiler for Renesas RX version 4.11.1

File Structure
--------------
r_t4_driver_rx
|   readme.txt
|
+---doc
|   |
|   +---ja
|   |     r20an0311jj0108-rx-t4.pdf
|   +---en
|         r20an0311ej0108-rx-t4.pdf
|
+---ref
|       r_t4_driver_rx_config_reference.h
|
+---src
|       ether_callback.c
|       t4_driver.c
|       timer.c
|       timer.h
|

r_config
        r_t4_driver_rx_config.h
