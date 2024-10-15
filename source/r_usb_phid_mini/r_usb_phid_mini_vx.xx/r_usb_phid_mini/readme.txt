PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_usb_phid_mini
=======================

Document Number 
---------------
R01AN2171EJ
R01AN2171JJ

Version
-------
v1.30

Overview
--------
USB Peripheral Human Interface Device Class Driver (PHID)

Features
--------
USB peripheral HID class driver conforms to the USB HID class specification 
and implements communication with HID devices.


Supported MCUs
--------------
* RX111 Group
* RX113 Group
* RX231 Group
* RX23W Group
* RX261 Group

Boards Tested On
----------------
* RSKRX111
* RSKRX113
* RSKRX231
* RSSKRX23W
* EK-RX261
 
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
* Renesas RX v.3.06.00
* GCC for Renesas RX 8.3.0.202311
* IAR C/C++ Compiler for Renesas version 4.20.3

File Structure
--------------
r_usb_phid_mini
|   readme.txt
|   r_usb_phid_mini_if.h
|
+---doc
|     \en
|     |   r01an2171ej0130_usb.pdf
|     \jp
|         r01an2171jj0130_usb.pdf
|
+---ref
|       r_usb_phid_mini_config_reference.h
|
\---src
     |  r_usb_phid_driver.c
     |
     \---inc
             r_usb_phid.h

