PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_jpege_rx
=========

Document Number
---------------
R20AN0263JJ
R20AN0263EJ

Version
-------
v1.01

Overview
--------
JPEG Encoder consists of two, JPEG File Compress Library which compress bitmap images into
JPEG files and JPEG Encode Library which provides routines for performing the calculations
required when compressing to a JPEG file.

Features
--------
* - JPEG File Compress Library -
* compress bitmap images into JPEG files (using JPEG Encode Library)
*
* - JPEG Encode Library -
* Registers Quantization table
* Executes DCT and quantization
* Executes Huffman encoding
* Writes DRI
* Writes RSTm
* Writes EOI
* Forcibly writes Huffman encoded data

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
* - JPEG File Compress Library -
* Interface header file is "r_jpege_rx\lib\r_compress_jpege.h".
* Add sections
*    P_jpeg_cmp_F
*    P_jpeg_cmp_S
*    C_jpeg_cmp_F
*    C_jpeg_cmp_F_2
*    C_jpeg_cmp_S
*    B_jpeg_cmp_F
*    B_jpeg_cmp_F_2
*    B_jpeg_cmp_F_1
*
* - JPEG Encode Library -
* Interface header file is "r_jpege_rx\lib\r_jpeg.h".
* Add sections
*    P_jpeg_enc_F
*    P_jpeg_enc_F_8
*    P_jpeg_enc_S
*    C_jpeg_enc_F
*    C_jpeg_enc_F_1
*    C_jpeg_enc_S

Toolchain(s) Used
-----------------
* Renesas RXC V.1.02.01

File Structure
--------------
r_jpege_rx
|   readme.txt
|
+---doc
|   +---en
|   |       r20an0263ej0101_rx_jpege.pdf
|   |       r20uw0122ej0100_jpege.pdf
|   |
|   \---ja
|           r20an0263jj0101_rx_jpege.pdf
|           r20uw0122jj0100_jpege.pdf
|
+---lib
|       compress_jpege_rx200_big.lib
|       compress_jpege_rx200_little.lib
|       compress_jpege_rx600_big.lib
|       compress_jpege_rx600_little.lib
|       jpege_rx200_big.lib
|       jpege_rx200_little.lib
|       jpege_rx600_big.lib
|       jpege_rx600_little.lib
|       r_compress_jpege.h
|       r_jpeg.h
|       r_mw_version.h
|       r_stdint.h
|
+---make_lib
|       make_lib.zip
|
\---ref
        r_jpege_rx_config_reference.h
