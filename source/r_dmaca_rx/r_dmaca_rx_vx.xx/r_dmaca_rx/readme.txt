PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_dmaca_rx
=========

Version
-------
v2.70

Overview
--------
The DMACA driver provides a method to transmit the data using the Direct Memory Access Controller (DMAC).
The driver includes API functions to initialize DMAC, create Transfer data, Control and get status of DMAC.
The driver can be reduced in size by removing code used for parameter checking. 
All configuration options can be found in "r_config\r_dmaca_rx_config.h". 
An original copy of the configuration file is stored in "r_dmaca_rx\ref\r_dmaca_rx_config_reference.h".

Features
--------
* Support Normal mode, Repeat mode and Block mode
* Support chain transfer

Supported MCUs
--------------
* RX230 Group
* RX231 Group
* RX23E-A Group
* RX23W Group
* RX64M Group
* RX65N Group
* RX651 Group
* RX66N Group
* RX66T Group
* RX671 Group
* RX71M Group
* RX72T Group
* RX72M Group
* RX72N Group

Limitations
-----------
* None

Peripherals Used Directly
-------------------------
* Direct Memory Access Controller (DMAC)

Required Packages
-----------------
* r_bsp 

How to add to your project
--------------------------
This module must be added to each project in which it is used.
Renesas recommends using "Smart Configurator" described in (1) or (3).
However, "Smart Configurator" only supports some RX devices.
Please use the methods of (2) or (4) for unsupported RX devices.

(1)    Adding the FIT module to your project using "Smart Configurator" in e2 studio
By using the "Smart Configurator" in e2 studio, 
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(2)    Adding the FIT module to your project using "FIT Configurator" in e2 studio
By using the "FIT Configurator" in e2 studio,
the FIT module is automatically added to your project.
Refer to "Adding Firmware Integration Technology Modules to Projects (R01AN1723)" for details.

(3)    Adding the FIT module to your project using "Smart Configurator" on CS+
By using the "Smart Configurator Standalone version" in CS+,
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(4)    Adding the FIT module to your project in CS+
In CS+, please manually add the FIT module to your project.
Refer to "Adding Firmware Integration Technology Modules to CS+ Projects (R01AN1826)" for details.

File Structure
--------------
r_dmaca_rx
|   r_dmaca_rx_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |       r01an2063ej{VERSION_NUMBER}-rx-dmac.pdf
|   +---ja
|           r01an2063jj{VERSION_NUMBER}-rx-dmac.pdf
|
+---src
    |   r_dmaca_rx.c
    |   r_dmaca_rx_private.h
    |
    +---targets
        |
        +---rx64m
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx65n
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx66n
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx66t
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx671
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx71m
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx72t
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx72m
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx72n
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx230
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx231
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx23e-a
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx23w
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h

r_config
    r_dmaca_rx_config.h
