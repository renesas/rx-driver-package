PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_usb_basic_mini
=======================

Document Number 
---------------
R01AN2166EJ
R01AN2166JJ

Version
-------
v1.12

Overview
--------
USB Basic Mini Host and Peripheral Driver

Features
--------
* Can operate in either host or peripheral mode.
* Device connect/disconnect, suspend/resume, and USB bus reset processing.
* Control transfer on pipe 0.
* Data transfer on pipes 1 to 9. (bulk or interrupt transfer: CPU access/DTC or DMA access)

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


How to add to your project
--------------------------

Toolchain(s) Used
-----------------
* Renesas RX v.3.01.00
* GCC for Renesas RX 4.08.04.201902
* IAR C/C++ Compiler for Renesas version 4.12.01

File Structure
--------------
r_usb_basic_mini
|   readme.txt
|   r_usb_basic_mini_if.h
|
+---doc
|     \en
|     |   r01an2166ej0112_usb.pdf
|     \jp
|         r01an2166jj0112_usb.pdf
|
+---ref
|       r_usb_config_mini_reference.h
|
\---utilities
|       qe_usb_firm_setting.xml
|
\---src
     +---driver
     |      +---inc
     |            r_usb_basic_define.h
     |            r_usb_extern.h
     |            r_usb_typedef.h
     |
     |      r_usb_cdataio.c
     |      r_usb_clibusbip.c
     |      r_usb_hbc.c
     |      r_usb_hdriver.c
     |      r_usb_hdriverapi.c
     |      r_usb_hlibusbip.c
     |      r_usb_hscheduler.c
     |      r_usb_hsignal.c
     |      r_usb_hstdfunction.c
     |      r_usb_pbc.c
     |      r_usb_pcontrolrw.c
     |      r_usb_pdriver.c
     |      r_usb_pintfifo.c
     |      r_usb_pinthandler_usbip0.c
     |      r_usb_plibusbip.c
     |      r_usb_psignal.c
     |      r_usb_pstdfunction.c
     |      r_usb_pstdrequest.c
     |      r_usb_usbif_api.c
     |            
     \---hw
           +---inc
                  r_usb_bitdefine.h
                  r_usb_dmac.h
                  r_usb_reg_access.h
                  
           r_usb_creg_abs.c
           r_usb_creg_access.c
           r_usb_dma.c
           r_usb_hreg_abs.c
           r_usb_hreg_access.c
           r_usb_preg_abs.c
           r_usb_preg_access.c
           r_usb_rx_mcu.c
