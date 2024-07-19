PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_tsip_rx
=========

Document Number
---------------
R20AN0371JJ0121-RX-TSIP-SECURITY
R20AN0371EJ0121-RX-TSIP-SECURITY

Version
-------
v1.21

Overview
--------
The TSIP(Trusted Secure IP) functions for Renesas MCUs.
TSIP supports AES, GCM, CCM, CMAC, SHA, MD5, Triple-DES, ARC4, RSA, ECC, Random number generate, Key management.
Support secure boot and secure firmware update sequence APIs.

These configuration options can be found in "r_config\r_tsip_rx_config.h". 


Features
--------
* Strongly Key Management.
* Hardware acceralator for AES, GCM, CCM, CMAC, SHA, MD5, Triple-DES, ARC4, RSA and ECC.
* Cooperate with SSL/TLS.
* Supports True Random Number Generating.
* Supports secure boot and secure firmware update.

Supported MCUs
--------------
* RX231 Group
* RX23W Group
* RX26T Group
* RX65N, RX651 Group
* RX66N Group
* RX66T Group
* RX671 Group
* RX72M Group
* RX72N Group
* RX72T Group

Boards Tested On
----------------
* RSKRX231
* RSSKRX23W
* MCB-RX26T
* RSKRX65N_2MB
* RSKRX66T
* RSKRX671
* RSKRX72M
* RSKRX72N
* EnvisionKitRX72N
* RSKRX72T

Limitations
-----------
None

Peripherals Used Directly
-------------------------
* Trusted Secure IP (TSIP)

Required Packages
-----------------
* r_bsp      V7.30

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
* Renesas RXC 3.06.00
* GNU GNURX 8.03.00.202311
* IAR ICCRX 5.10.01

File Structure
--------------
r_tsip_rx
|   readme.txt
|   r_tsip_rx_if.h
|
+---doc
|   +---ja
|   |    r20an0371jj0121-rx-tsip-security.pdf
|   +---en
|        r20an0371ej0121-rx-tsip-security.pdf
|
+---src
    |
    +---targets
        |   
        +---rx231_rx23w
        |   +   r_tsip_aes_rx.c
        |   |   r_tsip_rx.c
        |   |   r_tsip_rx_private.c
        |   |   r_tsip_rx_private.h
        |   |
        |   +---iodefine
        |   |      r_tsip_rx231_rx23w_iodefine.h
        |   |
        |   +---ip
        |          r_tsip_rx_function001.c
        |          r_tsip_rx_function002.c
        |          r_tsip_rx_function003.c
        |          :
        |
        +---rx26t
        |   +   r_tsip_aes_rx.c
        |   |   r_tsip_rx.c
        |   |   r_tsip_rx_private.c
        |   |   r_tsip_rx_private.h
        |   |
        |   +---iodefine
        |   |      r_tsip_rx26t_iodefine.h
        |   |
        |   +---ip
        |          r_tsip_rx_function001.c
        |          r_tsip_rx_function002.c
        |          r_tsip_rx_function003.c
        |          :
        |
        +---rx66t_rx72t
        |   +   r_tsip_aes_rx.c
        |   |   r_tsip_rx.c
        |   |   r_tsip_rx_private.c
        |   |   r_tsip_rx_private.h
        |   |
        |   +---iodefine
        |   |      r_tsip_rx66t_rx72t_iodefine.h
        |   |
        |   +---ip
        |          r_tsip_rx_function001.c
        |          r_tsip_rx_function002.c
        |          r_tsip_rx_function003.c
        |          :
        |
        +---rx65n
        |   +   r_tsip_aes_rx.c
        |   |   r_tsip_arc4_rx.c
        |   |   r_tsip_ecc_rx.c
        |   |   r_tsip_hash_rx.c
        |   |   r_tsip_rsa_rx.c
        |   |   r_tsip_rx.c
        |   |   r_tsip_rx_private.c
        |   |   r_tsip_rx_private.h
        |   |   r_tsip_tdes_rx.c
        |   |   r_tsip_tls_rx.c
        |   |
        |   +---iodefine
        |   |      r_tsip_rx65n_iodefine.h
        |   |
        |   +---ip
        |          r_tsip_rx_function001.c
        |          r_tsip_rx_function002.c
        |          r_tsip_rx_function003.c
        |          :
        |
        +---rx671
        |   +   r_tsip_aes_rx.c
        |   |   r_tsip_arc4_rx.c
        |   |   r_tsip_ecc_rx.c
        |   |   r_tsip_hash_rx.c
        |   |   r_tsip_rsa_rx.c
        |   |   r_tsip_rx.c
        |   |   r_tsip_rx_private.c
        |   |   r_tsip_rx_private.h
        |   |   r_tsip_tdes_rx.c
        |   |   r_tsip_tls_rx.c
        |   |
        |   +---iodefine
        |   |      r_tsip_rx671_iodefine.h
        |   |
        |   +---ip
        |          r_tsip_rx_function001.c
        |          r_tsip_rx_function002.c
        |          r_tsip_rx_function003.c
        |           :
        |
        +---rx72m_rx72n_rx66n
            +   r_tsip_aes_rx.c
            |   r_tsip_arc4_rx.c
            |   r_tsip_ecc_rx.c
            |   r_tsip_hash_rx.c
            |   r_tsip_rsa_rx.c
            |   r_tsip_rx.c
            |   r_tsip_rx_private.c
            |   r_tsip_rx_private.h
            |   r_tsip_tdes_rx.c
            |   r_tsip_tls_rx.c
            |
            +---iodefine
            |      r_tsip_rx72m_rx72n_rx66n_iodefine.h
            |
            +---ip
                   r_tsip_rx_function001.c
                   r_tsip_rx_function002.c
                   r_tsip_rx_function003.c
                   :
        

r_config
    r_tsip_rx_config.h


