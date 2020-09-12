PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_mmcif_rx (MMC Mode MMCIF Driver)
=======================

Document Number 
---------------
R01AN4234JJ0107
R01AN4234EJ0107

Version
-------
v1.07

Overview
--------
This driver is a block type device driver to control the MMC mode eMMC and MMC card using MultiMediaCard Interface (MMCIF).

Features
--------
* JEDEC Standard JESD84-A441.
* Block type device driver which takes 1 block to be 512 bytes
* MMC mode (1 bit/4 bits/8 bits) is supported.
* Backward compatible mode and high-speed mode are supported.
* eMMC and MMC cards are supported.
* Only user areas are supported.
* Only CD pin detection is supported in the case of MMC cards.

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
* RSK for RX65N_2MB
* RSK for RX71M
* RSK for RX72M
* RSK for RX72N
 
Limitations
-----------
None

Peripherals Used Directly
-------------------------
* MMC Host Interface (MMCIF)

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
* Renesas RX v3.01
* GCC for Renesas RX 4.8.4
* IAR C/C++ Compiler for Renesas RX 4.12.1

File Structure
--------------
r_mmcif_rx
|   r_mmcif_rx_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |       r01an4234ej0107-rx-communication.pdf
|   |
|   +---ja
|           r01an4234jj0107-rx-communication.pdf
|
+---ref
|       r_mmcif_rx_config_reference.h
|
+---src
    |   r_mmcif_cd.c
    |   r_mmcif_cmd.c
    |   r_mmcif_init.c
    |   r_mmcif_int.c
    |   r_mmcif_mount.c
    |   r_mmcif_read.c
    |   r_mmcif_trans.c
    |   r_mmcif_util.c
    |   r_mmcif_write.c
    |   r_mmcif_rx_private.h
    |
    +---targets
        |
        +---rx64m
        |       r_mmcif_dev.c
        |       r_mmcif_register.c
        |       r_mmcif_rx_target.h
        |
        +---rx65n
        |       r_mmcif_dev.c
        |       r_mmcif_register.c
        |       r_mmcif_rx_target.h
        |
        +---rx66n
        |       r_mmcif_dev.c
        |       r_mmcif_register.c
        |       r_mmcif_rx_target.h
        |
        +---rx71m
        |       r_mmcif_dev.c
        |       r_mmcif_register.c
        |       r_mmcif_rx_target.h
        |
        +---rx72m
        |       r_mmcif_dev.c
        |       r_mmcif_register.c
        |       r_mmcif_rx_target.h
        |
        +---rx72n
                r_mmcif_dev.c
                r_mmcif_register.c
                r_mmcif_rx_target.h

r_config
    r_mmcif_rx_config.h

