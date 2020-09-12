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
+---ref
|       r_lvd_rx_config_reference.h
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
        +---rx113
        |       r_lvd_rx113.h
        |
        +---rx130
        |       r_lvd_rx130.h
        |
        +---rx13t
        |       r_lvd_rx13t.h
        |
        +---rx230
        |       r_lvd_rx230.h
        |
        +---rx231
        |       r_lvd_rx231.h
        |
        +---rx23t
        |       r_lvd_rx23t.h
        |
        +---rx23w
        |       r_lvd_rx23w.h
        |
        +---rx24t
        |       r_lvd_rx24t.h
        |
        +---rx24u
        |       r_lvd_rx24u.h
        |
        +---rx64m
        |       r_lvd_rx64m.h
        |
        +---rx65n
        |       r_lvd_rx65n.h
        |
        +---rx66n
        |       r_lvd_rx66n.h
        |
        +---rx66t
        |       r_lvd_rx66n.h
        |
        +---rx71m
        |       r_lvd_rx71m.h
        |
        +---rx72m
        |       r_lvd_rx72m.h
        |
        +---rx72n
        |       r_lvd_rx72n.h
        |
        +---rx72t
        |       r_lvd_rx72t.h
        |

r_config
    r_lvd_rx_config.h

