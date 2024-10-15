PLEASE REFER TO THE APPLICATION NOTE FOR THIS DRIVER FOR MORE INFORMATION

r_canfd_rx
=========

Overview
--------
This module provides an interface to communicate over the CAN bus. 

Features
--------
* Runs in polled or interrupt modes.
* Supports:
        - Standard and extended CANFD IDs.
        - Loopback modes for easy testing.
        - Remote frames.
        - Bus error checking. 

Supported MCUs
--------------
* RX660 Group
* RX26T Group
* RX261 Group


Boards Tested On
----------------
* RSKRX660
* MCK-RX26T
* EK-RX261

Peripherals Used Directly
-------------------------
* CANFD

Required Packages
-----------------
* r_bsp_rx     v7.51 or greater (See platform.h)

File Structure
--------------
r_canfd_rx
|   readme.txt
|   r_canfd_rx_if.h (copy to your application.c)
|
+---doc
|   +-- en
|   |   r01an6130es{VERSION_NUMBER}-rx-canfd.pdf
|   +-- ja
|       r01an6130js{VERSION_NUMBER}-rx-canfd.pdf
+---src
|       r_canfd_rx.c
|       r_canfd_data.c
|       r_canfd_rx_private.h
+---inc
        r_can_api.c
        r_canfd.h

r_config
    r_canfd_rx_config.h

r_canfd_rx.ftl
