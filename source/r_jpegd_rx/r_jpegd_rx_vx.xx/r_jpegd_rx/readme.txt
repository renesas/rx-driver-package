PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_jpegd_rx
=========

Document Number
---------------
R20AN0104JJ
R20AN0104EJ

Version
-------
v2.06

Overview
--------
JPEG Decoder consists of two, JPEG File Expand Library which elongates a JPEG
file and JPEG Decode Library which performs basic operation.

Features
--------
* - JPEG File Expand Library -
* Initialization of JPEG File Expand Library
* Expand JPEG file (using JPEG Decode Library)
* Get information of JPEG file
*
* - JPEG Decode Library -
* Registration of Huffman table
* Registration of quantization table
* Huffman decoding
* Inverse DCT and Inverse quantization
* Detection of a restart marker

Supported MCUs
--------------
* RX family

Boards Tested On
----------------
* RSKRX71M
* RSKRX64M
* RSKRX63N
* RSKRX62N
* RSKRX210

Limitations
-----------
* This library uses DSP instructions. Please push/pop accumulator register (ACC) in user interrupts function using
* accumulator, because DSP instructions uses accumulator.
*
* This library can be used with Microcontroller Options fint_register=0 (Fast interrupt vectorregister [None]). The
* default for this option is fint_register=0.

Peripherals Used Directly
-------------------------
* None

Required Packages
-----------------
* None

How to add to your project
--------------------------
* - JPEG File Expand Library -
* Interface header file is "r_jpegd_rx\lib\r_expand_jpegd.h".
* Add sections
*    P_jpeg_exp_F
*    P_jpeg_exp_S
*    C_jpeg_exp_F
*    C_jpeg_exp_S
*    B_jpeg_exp_F
*    B_jpeg_exp_F_2
*    B_jpeg_exp_S
*    B_jpeg_exp_S_2
*
* - JPEG Decode Library -
* Interface header file is "r_jpegd_rx\lib\r_jpegd.h".
* Add sections
*    P_jpeg_dec_F8
*    P_jpeg_dec_S
*    C_jpeg_dec_F
*    C_jpeg_dec_F_2
*    C_jpeg_dec_S

Toolchain(s) Used
-----------------
Renesas RX Compiler V.1.02.01

File Structure
--------------
r_jpegd_rx
|   readme.txt
|
+---doc
|   +---en
|   |       r20an0104ej0206_rx_jpegd.pdf
|   |       r20uw0075ej0103_jpegd.pdf
|   |
|   \---ja
|           r20an0104jj0206_rx_jpegd.pdf
|           r20uw0075jj0103_jpegd.pdf
|
+---lib
|       expand_jpegd_rx200_big.lib
|       expand_jpegd_rx200_little.lib
|       expand_jpegd_rx600_big.lib
|       expand_jpegd_rx600_little.lib
|       jpegd_rx200_big.lib
|       jpegd_rx200_little.lib
|       jpegd_rx600_big.lib
|       jpegd_rx600_little.lib
|       r_expand_jpegd.h
|       r_jpegd.h
|       r_mw_version.h
|       r_stdint.h
|
+---make_lib
|       make_lib.zip
|
\---ref
        r_jpegd_rx_config_reference.h
