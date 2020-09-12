PLEASE REFER TO THE APPLICATION NOTE FOR MORE INFORMATION

r_wdt_rx
============

Overview
--------
The WDT module operates the Watchdog Timer. By default, the 
watchdog timer is not running. After starting the WDT counter via an Open() 
call or by changing the BSP_CFG_OFS0_REG_VALUE in r_bsp_config.h file, the 
user needs to refresh the timer periodically to avoid a reset or NMI interrupt.

Features
--------
* Functions to make it easier to control the WDT module
* Selection of auto start mode and register start mode
* Selection of MCU reset or NMI interrupt on abnormal operation


File Structure
--------------
r_wdt_rx
+-- readme.txt
+-- r_wdt_rx_if.h
|
+---doc
|    +--ja
|    |   +-- r01an3200jj{VERSION_NUMBER}-rx-wdt.pdf
|    +--en
|        +-- r01an3200ej{VERSION_NUMBER}-rx-wdt.pdf
|
+---ref
|    +-- r_wdt_rx_config_reference.h
|
+---src
     +-- r_wdt_rx.c
     
r_config
+-- r_wdt_rx_config.h

     
     
