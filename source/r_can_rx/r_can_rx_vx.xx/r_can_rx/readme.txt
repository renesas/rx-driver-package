PLEASE REFER TO THE APPLICATION NOTE r01an2472eu0215_can FOR THIS DRIVER FOR MORE INFORMATION

r_can_rx
=========

Document Number 
---------------
R01AN2472EU0320
R01AN2472JU0320

Version
-------
v3.20.

Overview
--------
This module provides an interface to communicate over the CAN bus. 

Features
--------
* Runs in polled or interrupt modes.
* Supports:
        - Standard and extended CAN IDs.
        - Multiple CAN channels.
        - Loopback modes for easy testing.
        - Remote frames.
        - Bus error checking. 

Supported MCUs
--------------
* RX64M Group
* RX65N/RX651 Groups
* RX66T Group
* RX66N Group
* RX71M Group
* RX72M Group
* RX72N Group
* RX72T Group


Boards Tested On
----------------
* RSKRX64M+
* RSKRX65N+
* RSKRX65N+ 2MB
* RSKRX66N
* RSKRX66T
* RSKRX71M+
* RSKRX72M
* RSKRX72N
* RSKRX72T


Limitations
-----------
* CAN FIFO not implemented.

Peripherals Used Directly
-------------------------
* CAN

Required Packages
-----------------
* r_bsp_rx     v5.20 or greater (See platform.h)

How to add the driver to your project
----------------------------------------
* Use e2studio Smart Configurator or the following:
* Add the r_can_rx and r_config folders to your project.
* Add a project include path to the 'r_can_rx' directory. 
* Add a project include path to the 'r_can_rx\src' directory.
* Edit the configuration file 'r_can_rx_config.h' to configure the driver for your project.
* If you are using FIT, add the r_bsp FIT module to your project.
* Add '#include r_can_rx_if.h' to any source files that use the API.

Toolchain(s) Used
-----------------
* Renesas RX v3.01
* GNURX 4.08.04
* ICCRX 4.12.01

File Structure
--------------
r_can_rx
|   readme.txt
|   r_can_rx_if.h (copy to your application.c)
|
+---doc
|   +-- en
|       r01an2472eu0320-rx-can.pdf
|   +-- ja
|       r01an2472ju0320-rx-can.pdf
|
+---ref
|       r_can_rx_config_reference.h (copy to ../r_config)
|
+---src
        r_can_rx.c
        r_can_rx.h

