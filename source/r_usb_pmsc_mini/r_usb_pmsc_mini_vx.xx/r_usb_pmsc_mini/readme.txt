PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_usb_pmsc_mini
=======================

Document Number 
---------------
R01AN2172EJ
R01AN2172JJ

Version
-------
v1.12

Overview
--------
USB Peripheral Mass Storage Class Driver (PMSC)

Features
--------
The USB Peripheral Mass Storage lass driver comprises a USB Mass Storage class 
bulk-only transport (BOT) protocol. When combined with a USB peripheral control
driver and storage device driver, it enables communication with a USB host as 
a BOT-compatible storage device.


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
* RSSKRX23W
 
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
r_usb_pmsc_mini
|   readme.txt
|   r_usb_pmsc_mini_if.h
|   r_usb_media_driver_mini_if.h
|
+---doc
|     \en
|     |   r01an2172ej0112_usb.pdf
|     \jp
|         r01an2172jj0112_usb.pdf
|
+---ref
|       r_usb_pmsc_mini_config_reference.h
|
\---src
     |  r_media_driver_api.c
     |  r_usb_atapi_driver.c
     |  r_usb_pmsc_driver.c
     |
     \---inc
             r_usb_patapi.h
             r_usb_pmsc.h
