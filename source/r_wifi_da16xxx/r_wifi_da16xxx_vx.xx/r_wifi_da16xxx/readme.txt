PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_wifi_da16xxx
=======

Document Number
---------------
R01AN7173EU (English version)

Version
-------
v1.20

Overview
--------------------------------------------------------------------------------
The r_wifi_da16xxx module is a wifi driver.

This driver is tested and works with the following toolchain:
  Renesas Electronics C/C++ Compiler for RX Family V3.06.00
  GCC for Renesas RX 8.3.0.202311
All API calls and their supporting interface definitions are located in r_wifi_da16xxx_if.h.

Features
--------
* UART baud rate set
* DNS query
* Ping IP
* DHCP Client
* SNTP Client
* AP Scan
* AP Connect (open, WEP, WPA, WPA2)
* AP Disconnect
* TCP Client Sockets (up to 4 sockets)
* TLS Client Sockets (up to 2 sockets)
* On-chip MQTT (w/TLS) Support
* On-chip HTTP client
* FreeRTOS Support based user application

Supported MCUs
--------------
* RX65N Group

Boards Tested On
----------------
* CK-RX65N

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
* Renesas Electronics C/C++ Compiler for RX Family V3.06.00
* GCC for Renesas RX 8.3.0.202311

File Structure
--------------
r_wifi_da16xxx
|   readme.txt
|   r_wifi_da16xxx_if.h
|
+---doc
|    \---en
|           r01an7173eu{VERSION_NUMBER}-rx-da16xxx-wifi.pdf
|
\---src
        r_wifi_da16xxx_atcmd.c
        r_wifi_da16xxx_common.c
        r_wifi_da16xxx_http.c
        r_wifi_da16xxx_mqtt.c
        r_wifi_da16xxx_os_wrap.c
        r_wifi_da16xxx_os_wrap.h
        r_wifi_da16xxx_private.h
        r_wifi_da16xxx_tcp.c
        r_wifi_da16xxx_tcp_tls.c
   
r_config
    r_wifi_da16xxx_config.h
