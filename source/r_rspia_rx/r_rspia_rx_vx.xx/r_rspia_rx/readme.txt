PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_rspia_rx
==========

Document Number 
---------------
R01AN5684ES0140
R01AN5684JS0140

Version
-------
v1.40

Overview
--------
This software provides an applications programing interface (API) to prepare the RSPIA peripheral for operation and for
performing data transfers over the SPI bus.
The RSPIA Driver module fits between the user application and the physical hardware to take care of the low-level
hardware control tasks that manage the RSPIA peripheral.
It is recommended to review the RSPIA peripheral chapter in the RX MCU hardware user's manual before using this
software.

Features
--------
This driver supports the following subset of the features available with the RSPIA peripheral.
RSPIA transfer functions:
  * Use of MOSI (master out/slave in), MISO (master in/slave out), SSL (slave select), and RSPCK (RSPIA
    clock) signals allows serial communications through SPI operation (four-wire method) or clock
    synchronous operation (three-wire method).
  * Capable of serial communications in master/slave mode
  * Switching of the polarity of the serial transfer clock
  * Switching of the phase of the serial transfer clock
Data format:
  * MSB-first/LSB-first selectable
  * Transfer bit length is selectable as 4 to 32 bits.
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
  * In slave mode: SSLn0 signal for input, selects the RSPIA slave. SSLn1 to SSLn3 signals are unused.
  * Controllable delay from SSL output assertion to RSPCK operation (RSPCK delay)
Range: 1 to 8 RSPCK cycles (set in RSPCK-cycle units)
  * Controllable delay from RSPCK stop to SSL output negation (SSL negation delay)
Range: 1 to 8 RSPCK cycles (set in RSPCK-cycle units)
  * Controllable wait for next-access SSL output assertion (next-access delay)
Range: 1 to 8 RSPCK cycles (set in RSPCK-cycle units)
  * Able to change SSL polarity
Control in master transfer:
  * For each transfer operation, the following can be set:
    Slave select number, further division of base bit rate, SPIA clock polarity/phase, transfer data bit-length,
MSB/LSB-first, burst (holding SSL), SPIA clock delay, slave select negation delay, and next-access delay
Interrupt sources:
  * RSPIA receive interrupt (receive buffer full)
  * RSPIA transmit interrupt (transmit buffer empty)
  * RSPIA error interrupt (mode fault, overrun, parity error).

Supported/Tested MCUs
--------------
* RX671
* RX26T

Boards Tested On
----------------
* RSKRX671
* MCB-RX26T

Limitations
-----------
* Maximum bit-rate performance depends on MCU speed

Peripherals Used Directly
-------------------------
* RSPIA

Required Packages
-----------------
* r_bsp     v7.30

How to add to your project
--------------------------
This module must be added to each project in which it is used.
Renesas recommends using "Smart Configurator" described in (1) or (2).
However, "Smart Configurator" only supports some RX devices.
Please use the methods of (3) for unsupported RX devices.

(1) Adding the FIT module to your project using "Smart Configurator" in e2 studio
By using the "Smart Configurator" in e2 studio, 
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(2) Adding the FIT module to your project using "Smart Configurator" on CS+
By using the "Smart Configurator Standalone version" in CS+,
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(3) Adding the FIT module to your project in CS+
In CS+, please manually add the FIT module to your project.
Refer to "Adding Firmware Integration Technology Modules to CS+ Projects (R01AN1826)" for details.

Toolchain(s) Used
-----------------
* Renesas RX
* GCC for Renesas RX
* IAR C/C++ Compiler for Renesas RX

File Structure
--------------
r_rspia_rx
|   r_rspia_rx_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |       r01an5684es0140-rx-rspia.pdf
|   +---ja
|           r01an5684js0140-rx-rspia.pdf
|
+---src
        r_rspia_defaults.h
        r_rspia_rx.c
        r_rspia_rx_private.h

r_config
    r_rspia_rx_config.h

