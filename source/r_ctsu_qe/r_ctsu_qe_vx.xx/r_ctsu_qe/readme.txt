PLEASE REFER TO THE APPLICATION NOTE FOR THIS MODULE FOR MORE INFORMATION

r_ctsu_qe
=================

Document Number 
---------------
r01an4469eu0111
r01an4469ju0111

Version
-------
v1.11

Overview
--------
A simple Application Program Interface (API) has been created to allow users of
RX100 and RX200 Series devices with the CTSU peripheral to easily integrate 
capacitive touch capabilities into their applications. Requires Touch QE FIT driver
for full operation.

Features
--------
* Self capacitance measurement
* Mutual capacitance measurement
* Multiple configurations (methods) supported within a single application
* Uses initialization data from QE Tool for Capacitive Touch
 

Supported MCUs
--------------
* RX113 Group
* RX130 Group
* RX231 Group
* RX23W Group


Boards Tested On
----------------
* RX113 Capacitive Touch Kit
* RX130 Capacitive Touch Kit
* RSKRX130
* RSKRX231
* RSSKRX23W


Limitations
-----------
* This code is not re-entrant and protects against multiple concurrent
    function calls.


Peripherals Used Directly
-------------------------
* CTSU


Required Packages
-----------------
* r_bsp       v4.24
* r_touch_qe  v1.11


Toolchain(s) Used
-----------------
* Renesas CC-RX 3.01


File Structure
--------------
r_ctsu_qe (v1.11)
|   +-- readme.txt
|   +-- r_ctsu_qe_if.h (v1.11)
|   +-- r_typedefs_qe.h (v1.10)
|
+---doc
|   +-- en
|   |   +-- r01an4469eu0111-rx.pdf
|   +-- ja
|       +-- r01an4469ju0111-rx.pdf
|
+---ref
|   +-- r_ctsu_qe_config_reference.h (v1.10)
|
+---src
    +-- r_correction.c (v1.11)
    +-- r_ctsu_qe_private.h (v1.10)
    +-- r_ctsu_qe.c (v1.11)
    +-- r_dtc.c (v1.10)
    +-- r_dtc.h (v1.10)

r_config
    r_ctsu_qe_rx_config.h (v1.10)

