PLEASE REFER TO THE APPLICATION NOTE FOR MORE INFORMATION

r_dac_rx
=========

Overview
--------
This driver supports both channels of the Digital-to-Analog Converter (DAC). 
Precision is 8-, 10-, or 12-bit depending upon the MCU. All available hardware
features of the MCUs are supported. 

Features
--------
* 2-channel DAC support.
* Left or right justified data alignment.
* Synchronization with ADC (MCU dependent)
* Selection of reference voltage (MCU dependent)
* Disabling of converter when output is disabled (MCU dependent)
* Enabling internal amplifier (MCU dependent)

File Structure
--------------
r_dac_rx
+-- readme.txt
+-- r_dac_rx_if.h
|
+-- doc
|    +[ja]
|    | +-- r01an1818jj{VERSION_NUMBER}-rx-dac.pdf
|    |
|    +[en]
|      +-- r01an1818ej{VERSION_NUMBER}-rx-dac.pdf
|
+-- ref
|    +-- r_dac_rx_config_reference.h
|
+-- src
     +-- r_dac_rx.c

r_config
+-- r_dac_rx_config.h
