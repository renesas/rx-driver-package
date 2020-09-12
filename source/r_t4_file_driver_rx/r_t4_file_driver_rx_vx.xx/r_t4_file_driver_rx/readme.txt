PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_t4_file_driver_rx
=========

Document Number
---------------
R20AN0333JJ0102-RX-T4
R20AN0333EJ0102-RX-T4

Version
-------
v1.02

Overview
--------
File driver for HTTP Server and FTP Server
Please refer to the
 http://japan.renesas.com/mw/t4 (Japanese)
 http://www.renesas.com/mw/t4 (English)
Features
--------
* File driver for HTTP Server and FTP Server

Supported MCUs
--------------
* RX family

Boards Tested On
----------------
* RSKRX64M
* RSKRX63N
* RSKRX62N
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
"r01an1723ju0111_rx.pdf/r01an1826jj0102_rx.pdf(R20AN0333JJ0102-RX-T4)"
"r01an1723eu0111_rx.pdf/r01an1826ej0102_rx.pdf(R20AN0333EJ0102-RX-T4)".

Toolchain(s) Used
-----------------
* Renesas RX V.2.05.00

File Structure
--------------
r_config
    r_t4_file_driver_rx_config.h

r_t4_file_driver_rx
|   readme.txt
|   r_t4_file_driver_rx_if.h
|
+---doc
|    |
|    +---en
|    |   r20an0333ej0102-rx-t4.pdf
|    +---ja
|        r20an0333jj0102-rx-t4.pdf
|
+---src
|    |   r_file_driver.c
|    |   r_file_driver.h
|    |
|    +---use_external_mem
|    |      r_file_driver_external.c
|    |
|    +---use_internal_mem
|           r_file_driver_internal.c
|
+---ref
          r_t4_file_driver_rx_config_reference.h

