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
|    |    +---r01an2769ej0200_rx.pdf
|    |
|    +---ja
|         |
|         +---r01an2769jj0200_rx.pdf
|    
+---ref
|    r_lpc_rx_config_reference.h
|
+---src
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
          +---rx64m
          |    r_lpc_rx64m.c
          |    r_lpc_rx64m_if.h
          |    r_lpc_rx64m_private.h
          |
          +---rx71m
               r_lpc_rx71m.c
               r_lpc_rx71m_if.h
               r_lpc_rx71m_private.h

r_config
    r_lpc_rx_config.h
