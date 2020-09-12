PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_flash_spi (Serial Flash memory control software)
=======================

Document Number 
---------------
R01AN2662EJ0301
R01AN2662JJ0301

Version
-------
v3.01

Overview
--------
The Serial Flash memory control software controls the Serial Flash memory devices using a Renesas Electronics' microcomputer.
A clock synchronous single-master control program that is specific to the individual MCU is separately required.

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
* r_bsp    v5.20

How to add to your project
--------------------------
This module must be added to each project in which it is used.
Renesas recommends using "Smart Configurator" described in (1) or (3).
However, "Smart Configurator" only supports some RX devices.
Please use the methods of (2) or (4) for unsupported RX devices.

(1)	Adding the FIT module to your project using "Smart Configurator" in e2 studio
By using the "Smart Configurator" in e2 studio, 
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(2)	Adding the FIT module to your project using "FIT Configurator" in e2 studio
By using the "FIT Configurator" in e2 studio,
the FIT module is automatically added to your project.
Refer to "Adding Firmware Integration Technology Modules to Projects (R01AN1723)" for details.

(3)	Adding the FIT module to your project using "Smart Configurator" on CS+
By using the "Smart Configurator Standalone version" in CS+,
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(4)	Adding the FIT module to your project in CS+
In CS+, please manually add the FIT module to your project.
Refer to "Adding Firmware Integration Technology Modules to CS+ Projects (R01AN1826)" for details.

Toolchain(s) Used
-----------------
* Renesas RX v3.01 release 00

File Structure
--------------
r_flash_spi
|   r_flash_spi_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |       r01an2662ej0301-rx.pdf
|   +---ja
|           r01an2662jj0301-rx.pdf
|
+---ref
|       r_flash_spi_config_reference.h
|       r_flash_spi_pin_config_reference.h
|
+---src
    |   r_flash_spi.c
    |   r_flash_spi_private.h
    |
    +---driver_interfaces
    |       r_flash_spi_drvif.c
    |
    +---flash_types
    |   |   r_flash_spi_type.c
    |   |
    |   +---flash_m25p
    |   |
    |   +---flash_m45pe
    |   |
    |   +---flash_mx
    |   |       r_flash_spi_type_sub.c
    |   |       r_flash_spi_type_sub.h
    |   |
    |   +---flash_n25q
    |   |
    |   +---flash_s25fl
    |
    +---dev_port
        |   r_flash_spi_dev_port.h
        |
        +---using_gpio_fit_module
        |       r_flash_spi_dev_port.c
        |
        +---using_iodefine
                r_flash_spi_dev_port.c

r_config
    r_flash_spi_config.h
    r_flash_spi_pin_config.h
