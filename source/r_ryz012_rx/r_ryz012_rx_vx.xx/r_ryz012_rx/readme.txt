
r_ryz012_rx
=======

Overview
--------------------------------------------------------------------------------
Use UART to control RYZ012 and connect to the Bluetooth Low Energy.

A configuration option for this exists in "r_config\r_ryz012_rx_config.h".

Features
--------
* RTOS is not supported.

File Structure
--------------
r_ryz012_rx 
|   readme.txt
|   r_ble_api.h
|   rm_ble_abs.h
|
+---doc
|    +---en
|    |      r01an6290ej{VERSION_NUMBER}-rx-ryz012-ble.pdf
|    |
|    +---ja
|           r01an6290jj{VERSION_NUMBER}-rx-ryz012-ble.pdf
|
+---src
        hal_data.c
        hal_data.h
        r_ble_spp.c
        r_ble_spp.h
        r_ble_spp_api.c
        rm_ble_abs_api.h
        rm_ble_abs_spp.c
        wrap_sci.c
        wrap_sci.h

r_config
    r_ryz012_rx_config.h

