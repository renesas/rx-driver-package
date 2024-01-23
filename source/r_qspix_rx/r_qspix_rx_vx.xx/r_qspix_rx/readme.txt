PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_qspix_rx (QSPIX driver)
=======================

Document Number 
---------------
R01AN5685ES0141
R01AN5685JS0141
     
Version
-------
v1.41

Overview
--------
The QSPIX built into the RX Family microcontroller is used to implement clock synchronous control.

Features
--------
* Memory mapped read access to the QSPIX flash
* Programming the QSPIX flash device
* Erasing the QSPI flash device
* Sending device specific commands and reading back responses
* Entering and exiting SPI mode
* Entering and exiting XIP mode
* 1, 2, 3 or 4 byte addressing

Supported MCUs
--------------
* RX671 Group

Boards Tested On
----------------
* RSK for RX671

Limitations
-----------
None

Peripherals Used Directly
-------------------------
* QSPI Serial Peripheral Interface (QSPIX)

Required Packages
-----------------
* r_bsp    v7.41

How to add to your project
--------------------------
This module must be added to each project in which it is used.
Renesas recommends using "Smart Configurator" described in (1) or (2).
However, "Smart Configurator" only supports some RX devices.
Please use the methods of (3) for unsupported RX devices.

(1) Adding the FIT module to your project using "Smart Configurator" in e2 studio
By using the "Smart Configurator" in e2 studio, 
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(2) Adding the FIT module to your project using "Smart Configurator" on CS+
By using the "Smart Configurator Standalone version" in CS+,
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(3) Adding the FIT module to your project in CS+
In CS+, please manually add the FIT module to your project.
Refer to "Adding Firmware Integration Technology Modules to CS+ Projects (R01AN1826)" for details.


File Structure
--------------
r_qspix_rx
|   r_qspix_rx_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |       r01an5685es{VERSION_NUMBER}-rx-qspix.pdf
|   +---ja
|           r01an5685js{VERSION_NUMBER}-rx-qspix.pdf
|
+---src
    |   r_qspix_rx_private.h
    |   r_qspix_rx.c
    |
    +---targets
        |
        +---rx671
        |       r_qspix_rx671_private.h
        |       r_qspix_rx671.c
r_config
    r_qspix_rx_config.h
