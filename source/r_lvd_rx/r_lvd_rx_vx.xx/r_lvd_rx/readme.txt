PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_lvd_rx
=========

Overview
--------
This module provides an interface to configure the Low Voltage Detection Circuit.

Features
--------
* Configure Low Voltage Detection circuit
* Use to generate NMI, Interrupt, Reset or Polling.
* Supports multiple channels

File Structure
--------------
r_lvd_rx
|   readme.txt
|   r_lvd_rx_if.h
|
+---doc
|   +---en
|   |   r01an3152ej{VERSION_NUMBER}-rx-lvd.pdf
|   |
|   \---ja
|       r01an3152jj{VERSION_NUMBER}-rx-lvd.pdf
|
+---src
    |   r_lvd_rx.c
    |   r_lvd_rx_hw.c
    |   r_lvd_rx_private.h
    |   r_lvd_rx_private_targets.h
    |   
    \---targets
        +---rx110
        |       r_lvd_rx110.h
        |
        +---rx111
        |       r_lvd_rx111.h
        |
        :

r_config
    r_lvd_rx_config.h

