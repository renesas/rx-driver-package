PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_dsmif_rx
========

Overview
--------
This module configures filters on the Delta-Sigma Modulator Interface (DSMIF) and receives 16-bit digital data.

Features
--------
* The user can select 15 types of filter settings.
* The user is alerted through the callback function when a current error occurs.


File Structure
--------------
r_dsmif_rx
|   readme.txt
|   r_dsmif_rx_if.h
|
+---doc
|   +---ja
|   |    r01an4707jj{VERSION_NUMBER}-rx-dsmif.pdf
|   +---en
|        r01an4707ej{VERSION_NUMBER}-rx-dsmif.pdf
|
\---src
        r_dsmif_rx.c
        r_dsmif_rx_private.c
        r_dsmif_rx_private.h

r_config
    r_dsmif_rx_config.h

