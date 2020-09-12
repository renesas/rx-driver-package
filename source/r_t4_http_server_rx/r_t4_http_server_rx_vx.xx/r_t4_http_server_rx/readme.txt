PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_t4_http_server_rx
==================

Document Number
---------------
R20AN0075JJ0106-RX-T4
R20AN0075EJ0106-RX-T4

Version
-------
v1.06

Overview
--------
T4 HTTP server provides simple HTTP server function.

 http://japan.renesas.com/mw/t4 (Japanese)
 http://www.renesas.com/mw/t4 (English)

These configuration options can be found in "r_config\r_t4_http_server_rx_config.h". 
An original copy of the configuration file is stored in 
"r_t4_http_server_rx\ref\r_t4_http_server_rx_config_reference.h".

Features
--------
* Simple HTTP server function

Supported MCUs
--------------
* RX family

Boards Tested On
----------------
* RSKRX62N
* RSKRX63N
* RSKRX64M
* RSKRX71M
* RSKRX65N

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
Please refer to the Adding Firmware Integration Technology Modules to Projects.
"r01an1723ju0111_rx.pdf/r01an1826jj0102_rx.pdf(R20AN0075JJ0106-RX-T4)"
"r01an1723eu0111_rx.pdf/r01an1826ej0102_rx.pdf(R20AN0075EJ0106-RX-T4)"

Toolchain(s) Used
-----------------
* Renesas RXC Toolchain v2.05.00

File Structure
--------------
r_t4_http_server_rx
|   readme.txt
|   r_t4_http_server_rx_if.h
|
+---doc
|   |
|   +---ja
|   |        r20an0075jj0106-rx-t4.pdf
|   +---en
|            r20an0075ej0106-rx-t4.pdf
|
+---ref
|           r_t4_http_server_rx_config_reference.h
|
+---src
|        r_http_server.c
|        r_http_server_config.c
|        r_http_server_config.h
|
r_config
    r_t4_http_server_rx_config.h
