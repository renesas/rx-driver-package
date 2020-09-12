PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_glcdc_rx
==========

Document Number
---------------
R01AN3609EJ0130 (English version)
R01AN3609JJ0130 (Japanese version)

Version
-------
v1.30

Overview
--------
The GLCDC FIT module uses the GLCDC to provide the method to output image data read from memory to the LCD panel. 
The GLCDC FIT module supports the following features.
  32- or 16-bit per pixel image data and 8-, 4-, or 1-bit CLUT (color lookup table) data format 
  Superimposition of three planes (and alpha blending of two planes)
  Correction of brightness, contrast, and RGB gamma for the LCD panel to output image data
  Parallel data output of RGB888, RGB666, and RGB565. Dithering for the output data format. 
All API calls and their supporting interface definitions are located in r_glcdc_rx_if.h.

Features
--------
* Output graphics data to the LCD panel.

Supported MCUs
--------------
* RX65N Group
* RX72M
* RX72N
* RX66N

Boards Tested On
----------------
* RSKRX65N_2MB
* RSKRX72N

Limitations
-----------
* Output of serial RGB data is not supported.
* Input of an external clock (LCD_EXTCLK) is not supported.

Peripherals Used Directly
-------------------------
* GLCDC

Required Packages
-----------------
* r_bsp (Rev.5.20 or higher)

How to add to your project
--------------------------
Please refer to the Adding Firmware Integration Technology Modules to Projects.
"r01an1723eu0121-rx.pdf/r01an1826ej0110-rx.pdf"(Only English version)
"r01an1723ju0121-rx.pdf/r01an1826jj0110-rx.pdf"(Only Japanese version)


Toolchain(s) Used
-----------------
* Renesas RX v3.01.00
* GCC for Renesas RX 4.8.4.201902
* IAR C/C++ Compiler for Renesas RX version 4.12.1

File Structure
--------------
r_glcdc_rx
|       readme.txt
|       r_glcdc_rx_if.h
|
+---doc
|   |
|   +---en
|   |   r01an3609ej0130-rx.pdf
|   |
|   \---ja
|       r01an3609jj0130-rx.pdf
|
+---ref
|       r_glcdc_rx_config_reference.h
|
\---src
    |   r_glcdc_rx.c
    |   r_glcdc_private.c
    |   r_glcdc_private.h
    |
    \---targets
        |
        +---rx65n
        |    r_glcdc_rx65n.c
        |
        +---rx72m
        |    r_glcdc_rx72m.c
        |
        +---rx72n
        |    r_glcdc_rx72n.c
        |
        \---rx66n
            r_glcdc_rx66n.c