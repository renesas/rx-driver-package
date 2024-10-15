PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_qe_display_rx
==========

Document Number
---------------
R01AN7283EJ0110 (English version)
R01AN7283JJ0110 (Japanese version)

Version
-------
v1.10

Overview
--------
* This module is used to adjust GLCDC using the standalone version QE for Display [RX].
* This module is not required when using e2 studio and the plug-in version of QE for Display [RX].
* Additionally, since this module is only for adjusting GLCDC, we recommend that you uninstall this module after making adjustments, or prepare and use a project specifically for making adjustments.
* Please refer to our website for details on QE for Display [RX].

Features
--------
* Serial communication function with QE
* GLCDC adjustment function
* Display the demo screen

Supported MCUs
--------------
* RX65N Group
* RX72M Group
* RX72N Group
* RX66N Group

Boards Tested On
----------------
* Renesas Envision Kit RPBRX72N
* Renesas Envision Kit RPBRX65N
* Renesas Starter Kit+ for RX72N

Limitations
-----------
* Please use Rev1.60 or higher for the GLCDC FIT module and Rev5.20 or higher for the SCI FIT module.
* If the color depth setting is 1 to 8 bits, the CLUT table is used by this module, emWin, and Aeropoint GUI FIT module, so it will be overwritten even if it is set by GLCDC FIT.
* When using in combination with the emWin FIT module or Aeropoint GUI FIT module, make adjustments while the display screen is stationary. It cannot be adjusted correctly when double buffering is in operation, such as during animation.

Peripherals Used Directly
-------------------------
* GLCDC
* SCI
* CMT
* GPIO

Required Packages
-----------------
* r_bsp      (Rev.5.20 or higher)
* r_glcdc_rx (Rev.1.60 or higher)
* r_sci_rx   (Rev.5.20 or higher)
* r_cmt_rx
* r_gpio_rx


Toolchain(s) Used
-----------------
* Renesas RX v3.06.00
* GCC for Renesas RX 8.3.0 202405
* IAR C/C++ Compiler for Renesas RX version 5.10.1

File Structure
--------------
r_config
    r_qe_display_rx_config.h

r_qe_display_rx
|       readme.txt
|       r_qe_display_rx_if.h
|
+---doc
|   |
|   +---en
|   |   r01an7283ej{VERSION_NUMBER}-rx-display.pdf
|   |
|   \---ja
|       r01an7283jj{VERSION_NUMBER}-rx-display.pdf
|
|
\---src
        r_qe_display_rx.c
        r_qe_display_lcdc.c
        r_qe_display_uart.c
        r_qe_display_private.h
