PLEASE REFER TO THE APPLICATION NOTE FOR THIS MODULE FOR MORE INFORMATION

r_ctsu_qe
=================

Document Number 
---------------
r01an4469jj0300

Version
-------
v3.00

Overview
--------
A simple Application Program Interface (API) has been created to allow users of
RX100 and RX200 and RX600 Series devices with the CTSU peripheral to easily integrate 
capacitive touch capabilities into their applications. Requires Touch QE FIT driver
for full operation.

Features
--------
* Self capacitance measurement
* Mutual capacitance measurement
* Multiple configurations (methods) supported within a single application
* Uses initialization data from QE Tool for Capacitive Touch
* diagnosis function support.
* Temperature correcttion function support.
* MEC function support.
* Automatic correction support.
* Automatic judgement support.
* Judgement Majoriry Mode(JMM) support.

Supported MCUs
--------------
* RX113 Group
* RX130 Group
* RX140 Group
* RX230 Group
* RX231 Group
* RX260 Group
* RX261 Group
* RX23W Group
* RX671 Group


Boards Tested On
----------------
* RX130 Capacitive Touch Kit
* RSKRX231
* RX140 Capacitive Touch Kit
* RX671 Capacitive Touch Kit
* RX261 Capacitive Touch Kit


Limitations
-----------
-


Peripherals Used Directly
-------------------------
RX113, RX130, RX230, RX231, RX23W, RX671
* CTSU
RX140 (64KB)
* CTSU2L
RX140 (128KB, 256KB)
* CTSU2SL
RX260, RX261
* CTSU2SLa


Required Packages
-----------------
* r_bsp       v7.50
* r_touch_qe  v3.00


Toolchain(s) Used
-----------------
* Renesas CC-RX 3.06


File Structure
--------------
r_ctsu_qe
|   +-- readme.txt
|   +-- r_ctsu_qe_if.h

+---src
|   +-- r_ctsu_qe.h
|   +-- r_ctsu_qe_api.h
|
+---src
    +-- r_ctsu_qe.c

r_config
    r_ctsu_qe_config.h

