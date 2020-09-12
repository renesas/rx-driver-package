PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_lcdc_rx
=========

Document Number
---------------
R01AN1980JJ0101
R01AN1980EJ0101

Version
-------
v1.01

Overview
--------
This document describes the LCD module using firmware integration technology (FIT). This module uses the LCD controller/driver (LCDC) to display data on an LCD. Hereinafter this module is referred to as LCDC FIT module.

Features
--------
- The internal voltage boosting method, the capacitor split method, or the external resistance division method can be
selected for the LCD driver voltage generator.
- The display bias method can be selected from the 1/2 bias method, 1/3 bias method, or 1/4 bias method.
- The time slice of display can be selected from static, 2-time slice, 3-time slice, 4-time slice, or 8-time slice.
- The display waveform can be selected from waveform A or waveform B.
- The display data area can be selected from A-pattern, B-pattern, or the blinking. The display data area can be
switched.
- The blinking display with A-pattern and B-pattern can be achieved using the RTC periodic interrupt (PRD).
- The reference voltage, which is generated when operating the voltage boost circuit, can be selected from 16 steps
(contrast adjustment).

Supported MCUs
--------------
* RX113 Group

Boards Tested On
----------------
* RSKRX113

Limitations
-----------
None

Peripherals Used Directly
-------------------------
* LCDC

Required Packages
-----------------
* r_bsp     v5.20

How to add to your project
--------------------------
The driver must be added to an existing e2 Studio project.
It is best to use the e2Studio FIT plugin to add the driver to your project
as that will automatically update the include file paths for you. 
Alternatively,the driver can be imported from the archive that
accompanies this application note and manually added by following these steps:

1. This application note is distributed with a zip file package
   that includes the FIT Simple IIC support module in its own folder r_lcdc_rx.
2. Unzip the package into the location of your choice.
3. In a file browser window, browse to the directory where you unzipped the 
   distribution package and locate the r_lcdc_rx folder.
4. Open your e2 Studio workspace.
5. In the e2 Studio project explorer window, select the project 
   that you want to add the simple IIC module to.
6. Drag and drop the r_lcdc_rx folder from the browser window (orcopy/paste)
   into your e2 Studio project at the top level of the project.
7. Update the source search/include paths for your project by adding the paths 
   to the module files:
   a. Navigate to the "Add directory path" control:
      i. 'project name'->properties->C/C++ Build->Settings->Compiler->Source -Add (green + icon)
   b. Add the following paths:
      i. "${workspace_loc:/${ProjName}/r_lcdc_rx}"
     ii. "${workspace_loc:/${ProjName}/r_lcdc_rx/src}"

Whether you used the plug-in or manually added the package to your project,
it is necessary to configure the driver for yourapplication.

8. Locate the r_lcdc_rx_config_reference.h file in the rlcdc_rx/ref/targets/rx113 source folder in your project and copy it to your project's r_config folder.
9. Change the name of the copy in the r_config folder to r_lcdc_rx_config.h
10. Make the required configuration settings by editing the copied
    r_lcdc_rx_config.h file. See Configuration Overview.

The LCDC module uses the r_bsp package for certain MCU information
and support functions. The r_bsp package is easily configured through the platform.h header file 
which is located in the r_bsp folder. To configure the r_bsppackage, open up platform.h and uncomment the #include for the board you are using.
For example, to run the demo on a RSKRX111 board, the user would uncomment the #include for 
Åe./board/rskrx111/r_bsp.hÅf macro and make sure all other board #includes are commented out.


Toolchain(s) Used
-----------------
* Renesas RX v3.01.00
* GCC for Renesas RX 4.8.4.201801
* IAR C/C++ Compiler for Renesas RX version 4.10.1


File Structure
--------------
r_lcdc_rx
|   readme.txt
|   r_lcdc_rx_if.h
|
+---doc
|   +---en
|   |   r01an1980ej0101-rx113-lcdc.pdf
|   |
|   +---ja
|       r01an1980jj0101-rx113-lcdc.pdf
|
+---ref
|       r_lcdc_config_reference.h
|
+---src
    |
    +---targets
        |
        |
        +---rx113
              r_lcdc_rx.c
              r_lcdc_rx_private.h

r_config
    r_lcdc_rx_config.h
