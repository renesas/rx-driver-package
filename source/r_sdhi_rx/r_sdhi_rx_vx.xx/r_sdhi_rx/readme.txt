PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_sdhi_rx
=======================

Document Number 
---------------
R01AN3852EJ0210
R01AN3852JJ0210

Version
-------
v2.10

Overview
--------
This driver is a block type device driver to control the SD Host Interface (SDHI).

Features
--------
* Supports SDHI clock supply / stop setting
* Supports SD mode (1 bit/4 bits) setting
* Supports Default speed mode and high-speed mode
* Supports interrupt enable / disable setting used in SDHI
* Supports clearing interrupt flag used in SDHI
* Supports callback function call when an interrupt occurs
* Supports SDHI register setting / getting
* Supports operation in little endian / big endian

Supported MCUs
--------------
* RX231 Group
* RX23W Group
* RX64M Group
* RX65N Group
* RX66N Group
* RX671 Group
* RX71M Group
* RX72M Group
* RX72N Group

Boards Tested On
----------------
* RSK for RX231
* RSSK for RX23W
* RSK for RX64M
* RSK for RX65N
* RSK for RX671
* RSK for RX71M
* RSK for RX72M
* RSK for RX72N
 
Limitations
-----------
None

Peripherals Used Directly
-------------------------
* SD Host Interface (SDHI)

Required Packages
-----------------
* r_bsp    v7.20

How to add to your project
--------------------------
This module must be added to each project in which it is used. 
Renesas recommends the method using the Smart Configurator described in (1) or (3) or (5) below. 
However, the Smart Configurator only supports some RX devices. 
Please use the methods of (2) or (4) for RX devices that are not supported by the Smart Configurator.

(1) Adding the FIT module to your project using the Smart Configurator in e2 studio
By using the Smart Configurator in e2 studio, 
the FIT module is automatically added to your project. 
Refer to "RX Smart Configurator User's Guide: e2 studio (R20AN0451)" for details.

(2) Adding the FIT module to your project using the FIT Configurator in e2 studio
By using the FIT Configurator in e2 studio, 
the FIT module is automatically added to your project. 
Refer to "RX Family Adding Firmware Integration Technology Modules to Projects (R01AN1723)" for details.

(3) Adding the FIT module to your project using the Smart Configurator in CS+
By using the Smart Configurator Standalone version in CS+, 
the FIT module is automatically added to your project. 
Refer to "RX Smart Configurator User's Guide: CS+ (R20AN0470)" for details.

(4) Adding the FIT module to your project in CS+
In CS+, please manually add the FIT module to your project. 
Refer to "RX Family Adding Firmware Integration Technology Modules to CS+ Projects (R01AN1826)" for details.

(5) Adding the FIT module to your project using the Smart Configurator in IAREW
By using the Smart Configurator Standalone version, 
the FIT module is automatically added to your project. 
Refer to "RX Smart Configurator User's Guide: IAREW (R20AN0535)" for details.

Toolchain(s) Used
-----------------
* Renesas RX v3.04
* GCC for Renesas RX v8.3.0.202202
* IAR C/C++ Compiler for Renesas RX v4.20.3

File Structure
--------------
r_sdhi_rx
|   r_sdhi_rx_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |       r01an3852ej0210-rx-communication.pdf
|   +---ja
|           r01an3852jj0210-rx-communication.pdf
|
+---ref
|       r_sdhi_rx_config_reference.h
|
+---src
    |   r_sdhi_int.c
    |   r_sdhi_open.c
    |   r_sdhi_rx_private.h
    |   r_sdhi_util.c
    |
    +---targets
        |
        +---rx231
        |       r_sdhi_dev.c
        |       r_sdhi_register.c
        |       r_sdhi_rx_target.h
        |
        +---rx23w
        |       r_sdhi_dev.c
        |       r_sdhi_register.c
        |       r_sdhi_rx_target.h
        |
        +---rx64m
        |       r_sdhi_dev.c
        |       r_sdhi_register.c
        |       r_sdhi_rx_target.h
        |
        +---rx65n
        |       r_sdhi_dev.c
        |       r_sdhi_register.c
        |       r_sdhi_rx_target.h
        |
        +---rx66n
        |       r_sdhi_dev.c
        |       r_sdhi_register.c
        |       r_sdhi_rx_target.h
        |
        +---rx671
        |       r_sdhi_dev.c
        |       r_sdhi_register.c
        |       r_sdhi_rx_target.h
        |
        +---rx71m
        |       r_sdhi_dev.c
        |       r_sdhi_register.c
        |       r_sdhi_rx_target.h
        |
        +---rx72m
        |       r_sdhi_dev.c
        |       r_sdhi_register.c
        |       r_sdhi_rx_target.h
        |
        +---rx72n
                r_sdhi_dev.c
                r_sdhi_register.c
                r_sdhi_rx_target.h

r_config
    r_sdhi_rx_config.h

