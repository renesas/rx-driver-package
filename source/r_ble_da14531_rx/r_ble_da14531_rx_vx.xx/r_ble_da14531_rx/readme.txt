PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_ble_da14531_rx
=======

Document Number
---------------
R01AN7174EU (English version)

Version
-------
v1.20

Overview
--------------------------------------------------------------------------------
The r_ble_da14531_rx module provides BLE GAP functionality for DA14531 BLE module
which are compliant with Bluetooth version 5.0.
This driver is tested and works with the following toolchain:
  Renesas RX Toolchain v3.05.00
All API calls and their supporting interface definitions are located in r_ble_da14531_if.h.

Features
--------
* Peripheral support only.
* Legacy Advertising, connection (No Security)
* UART support
* Dynamic loading of custom profile.
* Integration with QE BLE plugin
* GAP, GATT Server
* FreeRTOS Support
* AzureRTOS Support
* Bare metal support
* L2CAP, GATT Client support
* Support SDK download from host MCU.

Supported MCUs
--------------
* RX65N Group

Boards Tested On
----------------
* CK-RX65N

Limitations
-----------
* r_ble_da14531_rx only supports QE BLE projects. To generate sample code using QE for BLE,
  please refer to the Application Note (5. Sample Code Generation Using QE for BLE).
* The boot from host feature currently only supports 1-wire UART operation.

Peripherals Used Directly
-------------------------
* BLE (Bluetooth Low Energy)

Required Packages
-----------------
* r_bsp
* r_sci_rx
* r_byteq

How to add to your project
--------------------------
Please refer to the Adding Firmware Integration Technology Modules to Projects.
"r01an1723eu0121_rx.pdf/r01an1826ej0110_rx.pdf/r20an0451es0100-e2studio-sc.pdf"(Only English version)
"r01an1723ju0121_rx.pdf/r01an1826jj0110_rx.pdf/r20an0451js0100-e2studio-sc.pdf"(Only Japanese version)

Toolchain(s) Used
-----------------
* Renesas RX v3.05.00

File Structure
--------------
r_ble_da14531_rx
|   readme.txt
|   r_ble_da14531_if.h
|
+---doc
|    \---en
|           r01an7174eu{VERSION_NUMBER}-rx-da14531-ble.pdf
|
\---src
    |   hal_data.c
    |   hal_data.h
    |   r_ble_api.h
    |   r_ble_gtl_api.c
    |
    |
    +---rm_abs
    |       rm_ble_abs.h
    |       rm_ble_abs_api.h
    |       rm_ble_abs_gtl.c
    |
    |
    \---r_ble_gtl
            r_ble_gtl.c
            r_ble_gtl.h
            r_ble_gtl_image.c
            r_ble_gtl_image.h
   
r_config
    r_ble_da14531_config.h
