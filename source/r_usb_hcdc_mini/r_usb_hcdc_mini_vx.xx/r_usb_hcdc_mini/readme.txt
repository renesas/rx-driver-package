PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_usb_hcdc_mini
=======================

Document Number 
---------------
R01AN2167EJ
R01AN2167JJ

Version
-------
v1.12

Overview
--------
USB Host Communication Device Class Driver (HCDC)

Features
--------
The USB host Communication Device Class (CDC) driver conforms to the Abstract Control Model
 of the Communications Device Class specification, in particular, the Subclass Specification
 for PSTN Devices. It and enables communication with a CDC peripheral device.

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
r_usb_hcdc_mini
|   readme.txt
|   r_usb_hcdc_mini_if.h
|
+---doc
|     \en
|     |   r01an2167ej0112_usb.pdf
|     \jp
|         r01an2167jj0112_usb.pdf
|
+---ref
|       r_usb_hcdc_mini_config_reference.h
|
\---src
     |  r_usb_hcdc_driver.c
     |
     \---inc
             r_usb_hcdc.h
