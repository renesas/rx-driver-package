PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_datfrx_rx
=========

Document Number
---------------
R20AN0507JJ0201-RX-DATFRX
R20AN0507EJ0201-RX-DATFRX

Version
-------
v2.01

Overview
--------
Manage the data of the DATA FLASH .
Assign the logical number to writing and reading data and manage it.
It is possible to set the number and size of the data number individually.

These configuration options can be found in "r_config\r_datfrx_rx_config.h". 
An original copy of the configuration file is stored in 
"r_datfrx_rx\ref\r_datfrx_rx_config_reference.h".


Features
--------
* You can update or read data with the data number specified by the user.
* User data is managed with logical data of DATFRX. The number of data and the data size can be set by the user.
* DATFRX selects an empty block and updates the data.
* DATFRX erases blocks that store unnecessary data.

Supported MCUs
--------------
* RX Family

Boards Tested On
----------------
None

Limitations
-----------
None

Peripherals Used Directly
-------------------------
* Flash

Required Packages
-----------------
* r_bsp      V3.91
* r_flash_rx V3.41

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
* Renesas RX Compiler V.3.00.00

File Structure
--------------
r_datfrx_rx
|   readme.txt
|   r_flash_dm_rx_if.h
|
+---doc
|   +---en
|   |       r20an0507ej0201-rx-datfrx.pdf
|   \---ja
|           r20an0507jj0201-rx-datfrx.pdf
|
+---ref
|       r_datfrx_rx_config_reference.h
|
\---src
    |   r_flash_dm_private.h
    |   r_flash_dm_rx_if.c
    |
    +---flash_type_1
    |       r_dispatch_1.c
    |       r_dispatch_1.h
    |       r_dispatch_1_cf_1k.h
    |       r_dispatch_1_cf_2k.h
    |       r_dispatch_1_df_1k.h
    |       r_dm_1.c
    |       r_dm_1.h
    |
    +---flash_type_2
    |       r_datf_crc.c
    |       r_datf_crc.h
    |       r_dispatch_2.c
    |       r_dispatch_2.h
    |       r_dispatch_2_df.h
    |       r_dm_2.c
    |       r_dm_2.h
    |
    +---flash_type_3
    |       r_dm_3.c
    |       r_dm_3.h
    |
    \---flash_type_4
            r_dm_4.c
            r_dm_4.h

r_config
    r_datfrx_rx_config.h


