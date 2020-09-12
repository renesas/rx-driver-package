PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_qspi_smstr_rx (QSPI clock synchronous single master driver)
=======================

Document Number 
---------------
R01AN1940EJ0114
R01AN1940JJ0114
     
Version
-------
v1.14

Overview
--------
The QSPI built into the RX Family microcontroller is used to implement clock synchronous control. QSPI mode single
master control can be enabled by adding slave device selection control by means of port control.

Features
--------
* Block type device driver for clock synchronous single master using the QSPI, with the RX Family microcontroller
  as the master device
* The QSPI operates in clock synchronous serial communication mode. It can control one or more channels specified
  by the user.
* Reentrancy from a different channel is possible.
* Slave device selection control is unsupported.
  Slave device selection control must be implemented separately by means of port control.
* Operation with both big-endian and little-endian data order is supported. 
* Data is transferred in MSB-first format.
* Only software transfers are supported.
  A separate DMAC/DTC transfer program is required to perform DMAC/DTC transfers.

Supported MCUs
--------------
* RX64M Group
* RX65N Group
* RX66N Group
* RX71M Group
* RX72M Group
* RX72N Group

Boards Tested On
----------------
* RSK for RX64M
* RSK for RX65N
* RSK for RX71M
* RSK for RX65N-2MB
* RSK for RX72M
* RSK for RX72N

Limitations
-----------
None

Peripherals Used Directly
-------------------------
* QSPI Serial Peripheral Interface (QSPI)

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
r_qspi_smstr_rx
|   r_qspi_smstr_rx_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |       r01an1940ej0114-rx-serial.pdf
|   +---ja
|           r01an1940jj0114-rx-serial.pdf
|
+---ref
|       r_qspi_smstr_rx_config_reference.h
|       r_qspi_smstr_rx_pin_config_reference.h
|
+---src
    |   r_qspi_smstr.c
    |   r_qspi_smstr_private.h
    |
    +---targets
        |
        +---rx64m
        |       r_qspi_smstr_target.c
        |       r_qspi_smstr_target.h
        |       r_qspi_smstr_target_dev_port.c
        |
        +---rx65n
        |       r_qspi_smstr_target.c
        |       r_qspi_smstr_target.h
        |       r_qspi_smstr_target_dev_port.c
        |
        +---rx66n
        |       r_qspi_smstr_target.c
        |       r_qspi_smstr_target.h
        |       r_qspi_smstr_target_dev_port.c
        |
        +---rx71m
        |       r_qspi_smstr_target.c
        |       r_qspi_smstr_target.h
        |       r_qspi_smstr_target_dev_port.c
        |
        +---rx72m
        |       r_qspi_smstr_target.c
        |       r_qspi_smstr_target.h
        |       r_qspi_smstr_target_dev_port.c
        |
        +---rx72n
                r_qspi_smstr_target.c
                r_qspi_smstr_target.h
                r_qspi_smstr_target_dev_port.c

r_config
    r_qspi_smstr_rx_config.h
    r_qspi_smstr_rx_pin_config.h
