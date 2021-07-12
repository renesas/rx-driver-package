PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_eeprom_spi (SPI serial EEPROM control software)
=======================

Document Number 
---------------
R01AN2325EJ0302
R01AN2325JJ0302

Version
-------
v3.02

Overview
--------
The SPI Serial EEPROM control software controls the Renesas Electronics R1EX25xxx/HN58X25xxx series SPI Serial
EEPROM devices using a Renesas Electronics' microcomputer.
A clock synchronous single-master control program that is specific to the individual MCU is separately required.

Features
--------
* This softrare ia s Block type device driver, with a Renesas Electronics' MCU operating as the master device and
the Renesas Electronics R1EX25xxx/HN58X25xxx series SPI Serial EEPROM operating as the slave device.
* It controls the devices in the SPI mode using the MCU's internal serial communication function (clock synchronous
mode). It is possible to use the following FIT module for serial communication.
* It is possible to control up to two serial EEPROM.
* It is possible to set the differnt type name od serial EEPROM each device.
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
* r_bsp    v5.52

How to add to your project
--------------------------
This module must be added to each project in which it is used. Renesas recommends the method using the
Smart Configurator described in (1) or (3) or (5) below. However, the Smart Configurator only supports some
RX devices. Please use the methods of (2) or (4) for RX devices that are not supported by the Smart
Configurator.
(1) Adding the FIT module to your project using the Smart Configurator in e2 studio
By using the Smart Configurator in e2 studio, the FIT module is automatically added to your project.
Refer to ÅgRX Smart Configurator UserÅfs Guide: e2 studio (R20AN0451)Åh for details.
(2) Adding the FIT module to your project using the FIT Configurator in e2 studio
By using the FIT Configurator in e2 studio, the FIT module is automatically added to your project.
Refer to ÅgRX Family Adding Firmware Integration Technology Modules to Projects (R01AN1723)Åh
for details.
(3) Adding the FIT module to your project using the Smart Configurator in CS+
By using the Smart Configurator Standalone version in CS+, the FIT module is automatically added
to your project. Refer to ÅgRX Smart Configurator UserÅfs Guide: CS+ (R20AN0470)Åh for details.
(4) Adding the FIT module to your project in CS+
In CS+, please manually add the FIT module to your project. Refer to ÅgRX Family Adding Firmware
Integration Technology Modules to CS+ Projects (R01AN1826)Åh for details.
(5) Adding the FIT module to your project using the Smart Configurator in IAREW
By using the Smart Configurator Standalone version, the FIT module is automatically added to your
project. Refer to ÅgRX Smart Configurator UserÅfs Guide: IAREW (R20AN0535)Åh for details.

Toolchain(s) Used
-----------------
* Renesas RX v3.02 release 00
* GCC for Renesas RX 8.3.0
* IAR C/C++ Compiler for Renesas RX 4.14.1

File Structure
--------------
r_eeprom_spi
|   r_eeprom_spi_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |       r01an2325ej0302-rx-serial.pdf
|   +---ja
|           r01an2325jj0302-rx-serial.pdf
|
+---ref
|       r_eeprom_spi_config_reference.h
|       r_eeprom_spi_pin_config_reference.h
|
+---src
    |   r_eeprom_spi.c
    |   r_eeprom_spi_private.h
    |   r_eeprom_spi_sub.c
    |
    +---driver_interfaces
    |       r_eeprom_spi_drvif.c
    |   
    |
    +---dev_port
        |   r_eeprom_spi_dev_port.h
        |
        +---using_gpio_fit_module
        |      r_eeprom_spi_dev_port_gpio.c
        |
        +---using_iodefine
               r_eeprom_spi_dev_port_iodefine.c

r_config
    r_eeprom_spi_config.h
    r_eeprom_spi_pin_config.h
