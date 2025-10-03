;;/*
;;* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
;;*
;;* SPDX-License-Identifier: BSD-3-Clause
;;*/
;;/***********************************************************************************************************************
;;* File Name    : iar_call_inits.s
;;* Description  : Defines _CALL_INIT function that is called from startup in C++ project using IAR Compiler.
;;***********************************************************************************************************************/
;;/***********************************************************************************************************************
;;* History : DD.MM.YYYY Version   Description
;;*         : 28.02.2023 1.00      First Release
;;*         : 26.02.2025 1.01      Changed the disclaimer.
;;***********************************************************************************************************************/

#ifndef __ROPI__
#error __ROPI__ not defined
#endif

;------------------------------------------------------------------------------
; Initialize global class objects.
;------------------------------------------------------------------------------
        section  .text:CODE:NOROOT
#if __ROPI__
        extern  __PID_TOP
#endif
        extern  SHT$$PREINIT_ARRAY$$Base
        extern  SHT$$INIT_ARRAY$$Limit
        extern  ___call_ctors
        public  __CALL_INIT
        code
__CALL_INIT:
#if __ROPI__
        add     #SHT$$PREINIT_ARRAY$$Base - __PID_TOP, r13, r1
        add     #SHT$$INIT_ARRAY$$Limit - __PID_TOP, r13, r2
#else
        mov.l   #SHT$$PREINIT_ARRAY$$Base, r1
        mov.l   #SHT$$INIT_ARRAY$$Limit, r2
#endif
        bsr.a   ___call_ctors
        rts
        end
