PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_usb_hmsc_mini
=======================

Document Number 
---------------
R01AN2169EJ
R01AN2169JJ

Version
-------
v1.12

Overview
--------
USB Host Mass Storage Class Driver (HMSC)

Features
--------
The USB host mass storage class driver comprises a USB mass storage class 
Bulk-Only Transport (BOT) protocol. When combined with a file system and 
storage device driver, it enables communication with a BOT-compatible USB 
storage device.


Supported MCUs
--------------
* RX111 Group
* RX113 Group
* RX231 Group
* RX23W Group

Boards Tested On
----------------
* RSKRX111
* RSKRX113
* RSKRX231
* RSSKRX231
 
Limitations
-----------

Peripherals Used Directly
-------------------------


Required Packages
-----------------
* r_bsp
* r_usb_basic_mini

How to add to your project
--------------------------

Toolchain(s) Used
-----------------
* Renesas RX v.3.01.00
* GCC for Renesas RX 4.08.04.201902
* IAR C/C++ Compiler for Renesas version 4.12.01

File Structure
--------------
r_usb_hmsc_mini
|   readme.txt
|   r_usb_hmsc_mini_if.h
|
+---doc
|     \en
|     |   r01an2169ej0112_usb.pdf
|     \jp
|         r01an2169jj0112_usb.pdf
|
+---ref
|       r_usb_hmsc_mini_config_reference.h
|
\---src
     |  r_usb_hmsc_api.c
     |  r_usb_hmsc_driver.c
     |  r_usb_hstorage_driver.c
     |  r_usb_hstorage_driver_api.c
     |
     \---inc
             r_usb_hmsc.h
