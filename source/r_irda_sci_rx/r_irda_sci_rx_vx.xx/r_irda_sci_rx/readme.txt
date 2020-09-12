PLEASE REFER TO THE APPLICATION NOTE FOR THIS DRIVER FOR MORE INFORMATION

r_irda_sci_rx
========

Document Number 
---------------
R01AN2175JJ0101

Version
-------
v1.01

Overview
--------------------------------------------------------------------------------
The r_irda_sci_rx module transmits and receives IrDA data communication waveforms 
using SCI5 based on the IrDA (Infrared Data Association) standard 1.0. This driver 
initializes the IrDA module, transmits and receives data, and disables the IrDA module. 
To change the communication bit rate, disable the IrDA module once, and initialize 
the IrDA module with the new communication bit rate. The code size can be reduced by 
removing code for parameter checking, unused channels, or unused modes. These 
configuration options can be found in "r_config\r_irda_sci_rx_config.h". An original 
copy of the configuration file is stored in "r_sci_rx\ref\r_irda_sci_rx_config_reference.h".


Features
--------
* Incoming and outgoing data are placed on the queue.
* Transmission and reception are performed in the interrupt handlings.


Supported MCUs
--------------
* RX113 Group


Boards Tested On
----------------
* RSKRX113


Limitations
-----------
N/A


Peripherals Used Directly
-------------------------
* IrDA


Required Packages
-----------------
* r_bsp     v2.70
* r_byteq   v1.10


How to add to your project
--------------------------
The module must be added to an existing e2Studio project.
It is best to use the e2Studio FIT plug-in to add the FIT module to your project as that will automatically update the
include file paths for you. To add the FIT module using the plug-in, refer to Åg2. Adding FIT Modules to e2 studio
Projects Using FIT Plug-InÅh in the application note ÅgAdding Firmware Integration Technology Modules to Projects
(R01AN1723EU)Åh.
Alternatively, the FIT module can be added manually. To add the FIT module manually, refer to Åg3. Adding FIT
Modules to e2 studio Projects ManuallyÅh in the application note ÅgAdding Firmware Integration Technology Modules to
Projects (R01AN1723EU)Åh.
When using the FIT module, the BSP FIT module also needs to be added to the project. For details on the BSP FIT
module, refer to the application note ÅgBoard Support Package Module Using Firmware Integration Technology
(R01AN1685EU)Åh.

Toolchain(s) Used
-----------------
* Renesas RX v2.02


File Structure
--------------
r_irda_sci_rx
|   readme.txt
|   r_irda_sci_rx_if.h
|
+---doc
|       r01an2175jj0101_rx.pdf
|
+---ref
|       r_irda_sci_rx_config_reference.h
|
+---src
    |
    +---targets
        |
        +---irda
            |  r_irda_sci_rx.c
            |  r_irda_sci_rx_private.h
            |  r_irda_sci_rx_platform.h
            |
            +---rx113
                   r_irda_sci_rx113.c
                   r_irda_sci_rx113_private.h

r_config
    r_irda_sci_rx_config.h

