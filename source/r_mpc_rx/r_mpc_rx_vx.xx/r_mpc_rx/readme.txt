PLEASE REFER TO THE APPLICATION NOTE FOR THIS MODULE FOR MORE INFORMATION

r_mpc_rx
========

Overview
--------
Implements a Multi-Function Pin Controller driver. Allows users to easily setup the MPC registers for their device using
r_gpio_rx pin definitions.

Features
--------
* Easily configure the MPC registers for your device.
* Can use API to setup pins for analog use, IRQ use, or peripheral use

File Structure
--------------
r_mpc_rx
|   readme.txt
|   r_mpc_rx_if.h
|
+---doc
|   |
|   +---en
|   |   r01an1724ej{VERSION_NUMBER}-rx-mpc.pdf
|   |
|   |
|   +---ja
|       r01an1724jj{VERSION_NUMBER}-rx-mpc.pdf
|
+---ref
|       r_mpc_rx_config_reference.h
|
\---src
        r_mpc_rx.c
        
r_config
    r_mpc_rx_config.h
        
