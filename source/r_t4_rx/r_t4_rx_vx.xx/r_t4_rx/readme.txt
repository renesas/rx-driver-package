PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_t4_rx
=========

Document Number
---------------
R20AN0051JJ0209-RX-T4
R20AN0051EJ0209-RX-T4

Version
-------
v2.09

Overview
--------
The TCP/IP functions for Renesas MCUs.

These configuration options can be found in "r_config\r_t4_rx_config.h". 
An original copy of the configuration file is stored in 
"r_t4_rx\ref\r_t4_rx_config_reference.h".


Features
--------
* T4 has small footprint for Renesas MCUs.
* T4 can add TCP/IP functions for Renesas MCUs easily.
* T4 supports Renesas MCUs. Especially Ethernet Controller Model.

Supported MCUs
--------------
* RX Family

Boards Tested On
----------------
* RSKRX71M
* RSKRX64M
* RSKRX65N
* RSKRX63N
* RSKRX62N

Limitations
-----------
None

Peripherals Used Directly
-------------------------
None

Required Packages
-----------------
None

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
* Renesas RX Compiler V.3.01.00
* GCC for Renesas RX 4.8.4.201801
* IAR C/C++ Compiler for Renesas RX version 4.11.1

File Structure
--------------
r_t4_rx
|   readme.txt
|   
+---doc
|   +---en
|   |       r20an0051ej0209-rx-t4.pdf
|   |       r20uw0031ej0111-t4tiny.pdf
|   |       r20uw0032ej0108-t4tiny.pdf
|   |       
|   \---ja
|           r20an0051jj0209-rx-t4.pdf
|           r20uw0031jj0111-t4tiny.pdf
|           r20uw0032jj0108-t4tiny.pdf
|           
+---lib
|   |   r_mw_version.h
|   |   r_stdint.h
|   |   r_t4_itcpip.h
|   +---ccrx
|   |   T4_Library_ether_ccrx_rxv1_big.lib
|   |   T4_Library_ether_ccrx_rxv1_big_debug.lib
|   |   T4_Library_ether_ccrx_rxv1_little.lib
|   |   T4_Library_ether_ccrx_rxv1_little_debug.lib
|   +---gcc
|   |   libT4_Library_ether_gcc_rxv1_big.a
|   |   libT4_Library_ether_gcc_rxv1_little.a
|   \---iar
|       T4_Library_ether_iar_rxv1_big.a
|       T4_Library_ether_iar_rxv1_little.a
|       
+---make_lib
|       make_lib.zip
|       
+---ref
|       config_tcpudp_reference.tpl
|       r_t4_rx_config_reference.h
|       
\---src
        config_tcpudp.c
        
