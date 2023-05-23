PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_lpc_rx
========

Overview
--------
This module provides an interface to configure the low power consumption mode.

Features
--------
* Configure CPU-active power modes including High Speed, Middle Speed(s) and Low Speed(s)
* Configure CPU-Inactive modes including sleep, All Module stop, Standby and Deep Software Standby

File Structure
--------------
r_lpc_rx
|    readme.txt
|    r_lpc_rx_if.h
|    
+---doc
|    |
|    +---en
|    |    |
|    |    +---r01an2769ej0220-rx-lpc.pdf
|    |
|    +---ja
|         |
|         +---r01an2769jj0220-rx-lpc.pdf
|
+---src
     |    r_lpc_rx.c
     |    r_lpc_rx_platform.h
     |    r_lpc_rx_private.h
     |
     +---targets
          |
          +---rx110
          |    r_lpc_rx110.c
          |    r_lpc_rx110_if.h
          |    r_lpc_rx110_private.h
          |
          +---rx111
          |    r_lpc_rx111.c
          |    r_lpc_rx111_if.h
          |    r_lpc_rx111_private.h
          |
          +---rx113
          |    r_lpc_rx113.c
          |    r_lpc_rx113_if.h
          |    r_lpc_rx113_private.h
          |
          +---rx130
          |    r_lpc_rx130.c
          |    r_lpc_rx130_if.h
          |    r_lpc_rx130_private.h
          |
          +---rx140
          |    r_lpc_rx140.c
          |    r_lpc_rx140_if.h
          |    r_lpc_rx140_private.h
          |
          +---rx230
          |    r_lpc_rx230.c
          |    r_lpc_rx230_if.h
          |    r_lpc_rx230_private.h
          |
          +---rx231
          |    r_lpc_rx231.c
          |    r_lpc_rx231_if.h
          |    r_lpc_rx231_private.h
          |
          +---rx23w
          |    r_lpc_rx23w.c
          |    r_lpc_rx23w_if.h
          |    r_lpc_rx23w_private.h
          |
          +---rx26t
          |    r_lpc_rx26t.c
          |    r_lpc_rx26t_if.h
          |    r_lpc_rx26t_private.h
          |
          +---rx64m
          |    r_lpc_rx64m.c
          |    r_lpc_rx64m_if.h
          |    r_lpc_rx64m_private.h
          |
          +---rx65n
          |    r_lpc_rx65n.c
          |    r_lpc_rx65n_if.h
          |    r_lpc_rx65n_private.h
          |
          +---rx660
          |    r_lpc_rx660.c
          |    r_lpc_rx660_if.h
          |    r_lpc_rx660_private.h
          |
          +---rx66n
          |    r_lpc_rx66n.c
          |    r_lpc_rx66n_if.h
          |    r_lpc_rx66n_private.h
          |
          +---rx671
          |    r_lpc_rx671.c
          |    r_lpc_rx671_if.h
          |    r_lpc_rx671_private.h
          |
          +---rx71m
          |    r_lpc_rx71m.c
          |    r_lpc_rx71m_if.h
          |    r_lpc_rx71m_private.h
          |
          +---rx72m
          |    r_lpc_rx72m.c
          |    r_lpc_rx72m_if.h
          |    r_lpc_rx72m_private.h
          |
          +---rx72n
               r_lpc_rx72n.c
               r_lpc_rx72n_if.h
               r_lpc_rx72n_private.h

r_config
    r_lpc_rx_config.h
