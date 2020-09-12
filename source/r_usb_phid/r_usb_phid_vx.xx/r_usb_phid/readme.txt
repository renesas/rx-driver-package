PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_usb_phid
=======================

Document Number 
---------------
R01AN2663EJ
R01AN2663JJ

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
r_usb_pcdc
|   readme.txt
|   r_usb_phid_if.h
|
+---doc
|     \en
|     |   r01an2663ej0130_usb.pdf
|     \jp
|         r01an2663jj0130_usb.pdf
|
+---ref
|       r_usb_phid_config_reference.h
|
\---src
     |  r_usb_phid_driver.c
     |
     \---inc
             r_usb_phid.h

