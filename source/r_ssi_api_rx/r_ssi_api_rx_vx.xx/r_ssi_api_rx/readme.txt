PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_ssi_api_rx
=======================

Overview
--------
This is a FIT compliant module for RX's SSI peripheral.
This software module provides ways of PCM data transmit and receive operations using SSI peripheral.

Features
--------
* PCM data transmitting and receiving with SSI peripheral.

Supported MCUs
--------------
* RX64M Group
* RX71M Group
* RX113 Group
* RX231 Group
* RX230 Group
* RX23W Group
* RX72M Group
* RX72N Group
* RX66N Group

Boards Tested On
----------------
* RSKRX64M
* RSKRX71M
* RSKRX113
* RSKRX231
* RSSKRX23W
* RSKRX72M
* RSKRX72N
 
Limitations
-----------
* None

Peripherals Used Directly
-------------------------
* SSI (Serial Sound Interface)

Required Packages
-----------------
* None

How to add to your project
--------------------------
* Add src\r_ssi_api_rx.c and r_ssi_api_rx_private.h to your project.
* Add src\r_ssie_api_rx.c and r_ssie_api_rx_private.h to your project.
* Add an include path to the 'r_ssi_api_rx' directory. 
* Add an include path to the 'r_ssi_api_rx\src directory.
* Copy r_ssi_api_rx_config_reference.h from 'ref' directory to
  your desired location and rename to r_ssi_api_rx_config.h.
* Configure this API through r_ssi_api_rx_config.h.
* Add a #include for r_ssi_api_rx_if.h in any source file that
  needs to use this API.

Toolchain(s) Used
-----------------
* RX Family C Compiler
* GCC for Renesas RX
* IAR C/C++ Compiler for Renesas RX

File Structure
--------------
r_ssi_api_rx
|   readme.txt
|   r_ssi_api_rx_if.h
|
+---doc
|   |
|   +---en
|   |      r01an2150ej{VERSION_NUMBER}-rx-ssi.pdf
|   +---ja
|          r01an2150jj{VERSION_NUMBER}-rx-ssi.pdf
|
+---ref
|       r_ssi_api_rx_config_reference.h
|
\---src
        r_ssi_api_rx.c
        r_ssi_api_rx_private.h
        r_ssie_api_rx.c
        r_ssie_api_rx_private.h

r_config
    r_ssi_api_rx_config.h

