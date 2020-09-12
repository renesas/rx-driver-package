PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_memdrv_rx (Serial memory driver interface software)
=======================

Document Number 
---------------
R01AN4548EJ0102
R01AN4548JJ0102

Version
-------
v1.02

Overview
--------
The Serial memory driver interface software controls the Serial memory devices using a Renesas Electronics' microcomputer.
A clock synchronous program that is specific to the individual MCU is separately required.

Features
--------
* This softrare ia s Block type device driver, with a Renesas Electronics' MCU operating as the master device and
the Serial Flash memory operating as the slave device.
* It controls the devices in the SPI mode using the MCU's internal serial communication function (clock synchronous
mode). It is possible to use the following FIT module for serial communication.
* It is possible to control up to two Serial Flash memory.
* It is possible to set the differnt type name od Serial Flash memory each device.
* Both big-endian and little-endian operation are supported. (The choice depends on the MCU used.)

Supported MCUs
--------------
* RX Family MCU

Boards Tested On
----------------
* RSK for RX
 
Limitations
-----------
None

Peripherals Used Directly
-------------------------
* I/O Ports (GPIO)

Required Packages
-----------------
* r_bsp    v5.50

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
* Renesas RX v3.01 release 00
* GCC for Renesas RX 4.8.4
* IAR C/C++ Compiler for Renesas RX 4.12.1

File Structure
--------------
r_memdrv_rx
|   r_memdrv_rx_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |       r01an4548ej0102-rx-serial.pdf
|   +---ja
|           r01an4548jj0102-rx-serial.pdf
|
+---ref
|       r_memdrv_rx_config_reference.h
|
+---src
    |   r_memdrv_rx.c
    |   r_memdrv_rspi.c
    |   r_memdrv_qspi.c
    |   r_memdrv_sci.c
    |   r_memdrv_dmac.c
    |   r_memdrv_dtc.c
    |   r_memdrv_rx_private.h
    |
r_config
    r_memdrv_config.h
