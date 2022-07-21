PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_dtc_rx
=========

Version
-------
v4.10

Overview
--------
The DTC driver provides a method to transmit the data using Data Transfer Controller (DTC).
The driver includes API functions to initialize DTC, create Transfer data, Control and get status of DTC.
The driver can be reduced in size by removing code used for parameter checking. 
All configuration options can be found in "r_config\r_dtc_rx_config.h". 
An original copy of the configuration file is stored in "r_dtc_rx\ref\r_dtc_rx_config_reference.h".

Features
--------
* Support Normal trasnsfer mode, Repeat trasnsfer mode and Block trasnsfer mode.
* Support chain transfer

Supported MCUs
--------------
* RX110 MCU
* RX111 MCU
* RX113 MCU
* RX130 MCU
* RX140 MCU
* RX230 MCU
* RX231 MCU
* RX23E-A MCU
* RX64M MCU
* RX71M MCU
* RX23T MCU
* RX24T MCU
* RX24U MCU
* RX65N MCU
* RX651 MCU
* RX66T MCU
* RX660 MCU
* RX671 MCU
* RX66N MCU
* RX72T MCU
* RX23W MCU
* RX72M MCU
* RX13T MCU
* RX72N MCU

Limitations
-----------
* None

Peripherals Used Directly
-------------------------
* Data Transfer Controller (DTC)

Required Packages
-----------------
* r_bsp

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

File Structure
--------------
r_dtc_rx
|   r_dtc_rx_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |       r01an1819ej{VERSION_NUMBER}-rx-dtc-dmac2.pdf
|   +---ja
|           r01an1819jj{VERSION_NUMBER}-rx-dtc-dmac2.pdf
|
+---src
    |   r_dtc_rx.c
    |   r_dtc_rx_private.h
    |
    +---targets
        |
        +---rx23t
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx23w
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx24t
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx24u
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx64m
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx65n
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx66t
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx660
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx671
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx66n
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx71m
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx72m
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx72n
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx72t
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx110
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx111
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx113
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx130
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx13t
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx140
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx230
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx231
        |       r_dtc_rx_target.c
        |       r_dtc_rx_target.h
        |       r_dtc_rx_target_if.h
        |
        +---rx23E-A
                r_dtc_rx_target.c
                r_dtc_rx_target.h
                r_dtc_rx_target_if.h

r_config
    r_dtc_rx_config.h


