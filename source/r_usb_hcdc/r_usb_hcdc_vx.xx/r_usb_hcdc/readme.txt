PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_usb_hcdc
=======================

Document Number 
---------------
R01AN2027EJ
R01AN2027JJ

Version
-------
v1.30

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
* RX64M Group
* RX71M Group
* RX65N Group
* RX66T Group
* RX72T Group
* RX72M Group
* RX72N Group
* RX66N Group


Boards Tested On
----------------
* RSKRX64M
* RSKRX71M
* RSKRX65N
* RSKRX65N_2MB
* RSKRX72T
* RSKRX72M
* RSKRX72N


Limitations
-----------

Peripherals Used Directly
-------------------------


Required Packages
-----------------
* r_bsp
* r_usb_basic

How to add to your project
--------------------------

Toolchain(s) Used
-----------------
* Renesas RX v.3.01.00
* GCC for Renesas RX 4.08.04.201902
* IAR C/C++ Compiler for Renesas version 4.12.01

File Structure
--------------
r_usb_hcdc
|   readme.txt
|   r_usb_hcdc_if.h
|
+---doc
|     \en
|     |   r01an2027ej0130_usb.pdf
|     \jp
|         r01an2027jj0130_usb.pdf
|
+---ref
|       r_usb_hcdc_config_reference.h
|
\---src
     |  r_usb_hcdc_api.c
     |  r_usb_hcdc_driver.c
     |
     \---inc
             r_usb_hcdc.h
