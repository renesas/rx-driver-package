PLEASE REFER TO THE APPLICATION NOTE FOR THIS DRIVER FOR MORE INFORMATION

r_rscan_rx
==========

Document Number 
---------------
R01AN2805EU0200
R01AN2805JU0200


Version
-------
v2.00


Overview
--------------------------------------------------------------------------------
The r_rscan_rx module is a CAN driver for the RSCAN peripheral.

All mailboxes are one-message deep. There are 4 transmit mailboxes and 4 receive 
mailboxes. The transmit mailboxes can optionally be configured for interrupt 
operation, whereas the receive mailboxes cannot. The transmit mailboxes do not 
accept a message for transmit until the previous message has been sent. The 
receive mailboxes always contain the most recent message received, overwriting 
the previous contents without an error condition being generated. There is no 
hardware interrupt option available. 

The transmit and receive FIFOs are 4-messages deep. FIFOs are used for the sending 
and receiving of messages just like a mailbox. These can optionally be configured 
to be interrupt driven. Setting a receive FIFO to interrupt on every message 
received would behave similar to a receive mailbox with interrupt support.

There is a special FIFO called the Transmit History FIFO, and this FIFO is 8 entries 
deep. The History FIFO logs all messages tagged in an R_CAN_SendMsg() call in the 
order they are sent. Note that any FIFO usage is optional and are not required for 
normal operation.

The driver can be reduced in size by removing code used for parameter checking. 
This configuration option can be found in "r_config\r_rscan_rx_config.h". An 
original copy of the configuration file is stored in 
"r_rscan_rx\ref\r_rscan_rx_config_reference.h".


Features
--------
* Transmit or receive messages using mailboxes or FIFOs
* Built-in logging capability of messages sent
* Operate in interrupt or polling mode


Supported MCUs
--------------
* RX230, RX231 Group
* RX24T-512 Group
* RX24U Group


Boards Tested On
----------------
* RSKRX231
* RSKRX24T
* RSKRX24U


Limitations
-----------
N/A


Peripherals Used Directly
-------------------------
* RSCAN


Required Packages
-----------------
* r_bsp    


How to add to your project
--------------------------
* Add the r_rscan_rx and r_config folders to your project.
* Add a project include path for the 'r_rscan_rx' directory. 
* Add a project include path for the 'r_rscan_rx\src' directory.
* Add a project include path for the 'r_config' directory.
* Open "r_config\r_rscan_rx_config.h" file and configure the driver for your 
  project.
* Add a #include for r_rscan_rx_if.h to any source files that need to use 
  the API functions.


Toolchain(s) Used
-----------------
* Renesas RX v3.01
* GCC for Renesas RX 4.8.4
* IAR C/C++ Compiler for Renesas RX 4.11.1


File Structure
--------------
r_rscan_rx
  |   readme.txt
  |   r_rscan_rx_if.h
  |
  +---doc
  |   +-- en 
  |   |   r01an2805eu0200-rx-rscan.pdf
  |   +-- ja
  |       r01an2805ju0200-rx-rscan.pdf
  |
  +---ref
  |     r_rscan_rx_config_reference.h
  |
  +---src
        r_rscan_rx.c
        r_rscan_rx_private.h
   
r_config
    r_rscan_rx_config.h

