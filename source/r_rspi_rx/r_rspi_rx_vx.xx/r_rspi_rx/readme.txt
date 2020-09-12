PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_rspi_rx
=========

Document Number 
---------------
R01AN1827EJ0204
R01AN1827JJ0204

Version
-------
v2.04

Overview
--------
This software provides an applications programing interface (API) to prepare the RSPI peripheral for operation and for
performing data transfers over the SPI bus.
The RSPI Driver module fits between the user application and the physical hardware to take care of the low-level
hardware control tasks that manage the RSPI peripheral.
It is recommended to review the RSPI peripheral chapter in the RX MCU hardware user's manual before using this
software.

Features
--------
This driver supports the following subset of the features available with the RSPI peripheral.
RSPI transfer functions:
  * Use of MOSI (master out/slave in), MISO (master in/slave out), SSL (slave select), and RSPCK (RSPI
    clock) signals allows serial communications through SPI operation (four-wire method) or clock
    synchronous operation (three-wire method).
  * Capable of serial communications in master/slave mode
  * Switching of the polarity of the serial transfer clock
  * Switching of the phase of the serial transfer clock
Data format:
  * MSB-first/LSB-first selectable
  * Transfer bit length is selectable as 8, 9, 10, 11, 12, 13, 14, 15, 16, 20, 24, or 32 bits.
Bit rate:
  * In master mode, the on-chip baud rate generator generates RSPCK by frequency-dividing PCLK
    (Division ratio: 2 to 4096).
  * In slave mode, the externally input clock is used as the serial clock (the maximum frequency is that of
    PCLK divided by 8).
Error detection:
  * Mode fault error detection
  * Overrun error detection
  * Parity error detection
SSL control function:
  * Four SSL signals (SSLn0 to SSLn3) for each channel
  * In single-master mode: SSLn0 to SSLn3 signals are output.
  * In slave mode: SSLn0 signal for input, selects the RSPI slave. SSLn1 to SSLn3 signals are unused.
  * Controllable delay from SSL output assertion to RSPCK operation (RSPCK delay)
Range: 1 to 8 RSPCK cycles (set in RSPCK-cycle units)
  * Controllable delay from RSPCK stop to SSL output negation (SSL negation delay)
Range: 1 to 8 RSPCK cycles (set in RSPCK-cycle units)
  * Controllable wait for next-access SSL output assertion (next-access delay)
Range: 1 to 8 RSPCK cycles (set in RSPCK-cycle units)
  * Able to change SSL polarity
Control in master transfer:
  * For each transfer operation, the following can be set:
    Slave select number, further division of base bit rate, SPI clock polarity/phase, transfer data bit-length,
MSB/LSB-first, burst (holding SSL), SPI clock delay, slave select negation delay, and next-access delay
Interrupt sources:
  * RSPI receive interrupt (receive buffer full)
  * RSPI transmit interrupt (transmit buffer empty)
  * RSPI error interrupt (mode fault, overrun, parity error).

Supported/Tested MCUs
--------------
* RX71M
* RX66T
* RX65N
* RX64M
* RX24T
* RX24U
* RX23T
* RX231
* RX230
* RX23W
* RX130
* RX113
* RX111
* RX110
* RX72T
* RX72M
* RX72N
* RX66N

Boards Tested On
----------------
* RSKRX71M
* RSKRX66T
* RSKRX65N
* RSKRX65N_2MB
* RSKRX64M
* RSKRX24T
* RSKRX24U
* RSKRX23T
* RSKRX231
* RSKRX230
* RSSKRX23W
* RSKRX130
* RSKRX113
* RSKRX111
* RSKRX110
* RSKRX130_512KB
* RSKRX72T
* RSKRX72M
* RSKRX72N

Limitations
-----------
* Maximum bit-rate performance depends on MCU speed

Peripherals Used Directly
-------------------------
* RSPI

Required Packages
-----------------
* r_bsp     v5.50

How to add to your project
--------------------------
This module must be added to each project in which it is used.
Renesas recommends using "Smart Configurator" described in (1) or (3).
However, "Smart Configurator" only supports some RX devices.
Please use the methods of (2) or (4) for unsupported RX devices.

(1) Adding the FIT module to your project using "Smart Configurator" in e2 studio
By using the "Smart Configurator" in e2 studio, 
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(2) Adding the FIT module to your project using "FIT Configurator" in e2 studio
By using the "FIT Configurator" in e2 studio,
the FIT module is automatically added to your project.
Refer to "Adding Firmware Integration Technology Modules to Projects (R01AN1723)" for details.

(3) Adding the FIT module to your project using "Smart Configurator" on CS+
By using the "Smart Configurator Standalone version" in CS+,
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(4) Adding the FIT module to your project in CS+
In CS+, please manually add the FIT module to your project.
Refer to "Adding Firmware Integration Technology Modules to CS+ Projects (R01AN1826)" for details.

Toolchain(s) Used
-----------------
* Renesas RX v3.01.00

File Structure
--------------
r_rspi_rx
|   r_rspi_rx_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |       r01an1827ej0204-rx-serial.pdf
|   +---ja
|           r01an1827jj0204-rx-serial.pdf
|
+---ref
|       r_rspi_rx_config_reference.h
|
+---src
        r_rspi_defaults.h
        r_rspi_rx.c
        r_rspi_rx_private.h

r_config
    r_rspi_rx_config.h

