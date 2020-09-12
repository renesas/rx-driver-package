PLEASE REFER TO THE APPLICATION NOTE FOR MORE INFORMATION

r_iwdt_rx
============

Overview
--------
The IWDT module operates the Independent Watchdog Timer. By default, the 
watchdog timer is not running. After starting the IWDT counter via an Open() 
call or by changing the BSP_CFG_OFS0_REG_VALUE in r_bsp_config.h file, the 
user needs to refresh the timer periodically to avoid a reset or NMI interrupt.

Features
--------
* Functions to make it easier to control the IWDT module
* Selection of auto start mode and register start mode
* Selection of MCU reset or NMI interrupt on abnormal operation

File Structure
--------------
r_iwdt_rx
+-- readme.txt
+-- r_iwdt_rx_if.h
|
+---doc
|    +--ja
|    |    +-- r01an1820jj{VERSION_NUMBER}-rx-iwdt.pdf
|    +--en
|         +-- r01an1820ej{VERSION_NUMBER}-rx-iwdt.pdf
|
+---ref
|    +-- r_iwdt_rx_config_reference.h
|
+---src
     +-- r_iwdt_rx.c

r_config
+-- r_iwdt_rx_config.h

