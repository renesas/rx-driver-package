PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_vbatt_rx
=========

Document Number
---------------
R01AN2796JJ0104
R01AN2796EJ0104


Version
-------
v1.04


Overview
--------
This module detects drop for the battery backup power and voltage of VBATT pin.
So, the user can detect whether RTC count value is valid or not by this module.

The driver can be reducedin size by removing code used for parameter checking or for unused channels. 
These configuration options can be found in "r_config\r_vbatt_rx_config.h". 
An original copy of the configuration file is stored in 
"r_vbatt_rx\ref\r_vbatt_rx_config_reference.h".


Features
--------
This module can read the state of the setting of the VBATT pin voltage drop detection function 
and the battery backup function.
And, Users can judge four condition with the argument of the callback function.
(1) There is no detection of the battery backup power supply voltage drop
(2) There is the detection of the battery backup power supply voltage drop
(3) Under the non-maskable interrupt by the detection of the VBATT pin voltage drop
(4) Under the maskable interrupt by the detection of the VBATT pin voltage drop


Supported MCUs
--------------
* RX230 Group
* RX231 Group
* RX23W Group


Boards Tested On
----------------
* RSKRX230
* RSKRX231
* RSSKRX23W


Technical Update of applying
-----------
None


Limitations
-----------
* Please connect the VBATT pin to a power supply different from VCC.
* To use the battery backup function, enable the voltage monitoring 0 reset 
  and select the voltage detection 0 level.(in the case of RX231 group 2.51V)


Peripherals Used Directly
-------------------------
* Battery Backup Function


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
   that includes the FIT Battery Backup Function support module in its own folder r_vbatt_rx.
2. Unzip the package into the location of your choice.
3. In a file browser window, browse to the directory where you unzipped the 
   distribution package and locate the r_vbatt_rx folder.
4. Open your e2 Studio workspace.
5. In the e2 Studio project explorer window, select the project 
   that you want to add the Battery Backup Function module to.
6. Drag and drop the r_vbatt_rx folder from the browser window (orcopy/paste)
   into your e2 Studio project at the top level of the project.
7. Update the source search/include paths for your project by adding the paths 
   to the module files:
   a. Navigate to the "Add directory path" control:
      i. 'project name'->properties->C/C++ Build->Settings->Compiler->Source -Add (green + icon)
   b. Add the following paths:
      i. "${workspace_loc:/${ProjName}/r_vbatt_rx}"
     ii. "${workspace_loc:/${ProjName}/r_vbatt_rx/src}"

Whether you used the plug-in or manually added the package to your project,
it is necessary to configure the driver for yourapplication.

8. Locate the r_vbatt_rx_config_reference.h file in the r_vbatt_rx/ref/targets/vbatt source folder
   in your project and copy it to your project's r_config folder.
9. Change the name of the copy in the r_config folder to r_vbatt_rx_config.h
10. Make the required configuration settings by editing the copied
    r_vbatt_rx_config.h file. See Configuration Overview.

The Battery Backup Function module uses the r_bsp and r_cgc packages for certain MCU information
and support functions. The r_bsp package is easily configured through the platform.h header file 
which is located in the r_bsp folder. To configure the r_bsppackage, open up platform.h 
and uncomment the #include for the board you are using.
For example, to run the demo on a RSKRX231 board, the user would uncomment the #include for 
Åe./board/rskrx231/r_bsp.hÅf macro and make sure all other board #includes are commented out.


Toolchain(s) Used
-----------------
* Renesas RX v3.01.00
* GCC for Renesas RX 4.8.4.201801
* IAR C/C++ Compiler for Renesas RX version 4.10.1


File Structure
--------------
r_vbatt_rx
|   readme.txt
|   r_vbatt_rx_if.h
|
+---doc
|   +---en
|   |   r01an2796ej0104-rx-vbatt.pdf
|   |
|   +---ja
|       r01an2796jj0104-rx-vbatt.pdf
|
+---ref
|       r_vbatt_config_reference.h
|
+---src
    |
    +---targets
        |
        |
        +---vbatt
            |   r_vbatt_rx.c
            |   r_vbatt_rx_platform.h
            |   r_vbatt_rx_private.h
            |
            +---rx23w
            |       r_vbatt_rx23w.c
            |       r_vbatt_rx23w_private.h
            |
            +---rx230
            |       r_vbatt_rx230.c
            |       r_vbatt_rx230_private.h
            |
            +---rx231
                    r_vbatt_rx231.c
                    r_vbatt_rx231_private.h

r_config
    r_vbatt_rx_config.h

