PLEASE REFER TO THE APPLICATION NOTE FOR THIS MODULE FOR MORE INFORMATION

rm_touch_qe
=================

Document Number 
---------------
r01an4470jj0210

Version
-------
v2.10

Overview
--------
A simple Application Program Interface (API) has been created to allow users of
RX100 and RX200 and RX600 Series devices with the CTSU peripheral to easily integrate 
capacitive touch capabilities into their applications. Requires CTSU QE FIT driver
for full operation.

Features
--------
* Self capacitance measurement
* Mutual capacitance measurement
* Multiple configurations (methods) supported within a single application
* Uses initialization data from QE Tool for Capacitive Touch
* Supports monitoring capabilities by the QE Tool 
* Wrapper API for Touch API FIT V1.11 support
* Serial tuning support with standalone QE Tool for Capacitive Touch
* Touch threshold adjustment function support.


Supported MCUs
--------------
* RX113 Group
* RX130 Group
* RX140 Group
* RX230 Group
* RX231 Group
* RX23W Group
* RX671 Group

Boards Tested On
----------------
* RX130 Capacitive Touch Kit
* RSKRX231
* RX140 Capacitive Touch Kit

Limitations
-----------
-

Peripherals Used Directly
-------------------------
* CTSU


Required Packages
-----------------
* r_bsp      v6.20
* r_ctsu_qe  v2.10


Toolchain(s) Used
-----------------
* Renesas CC-RX 3.03


File Structure
--------------
rm_touch_qe (v2.10)
|   +-- readme.txt
|   +-- rm_touch_qe_if.h (v2.10)
|
+---inc
|   +-- rm_touch_qe.h (v2.10)
|   +-- rm_touch_qe_api.h (v2.10)
|
+---wrapper
|   +-- r_touch_wrapper_qe.c
|   +-- r_touch_wrapper_qe_h
|
+---src
    +-- rm_touch_qe.c (v2.10)

r_config
    rm_touch_qe_config.h (v2.10)


