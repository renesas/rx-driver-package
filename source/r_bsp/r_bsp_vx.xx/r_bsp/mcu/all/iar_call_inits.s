;;/***********************************************************************************************************************
;;* DISCLAIMER
;;* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
;;* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
;;* applicable laws, including copyright laws. 
;;* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
;;* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
;;* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
;;* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
;;* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
;;* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
;;* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
;;* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
;;* following link:
;;* http://www.renesas.com/disclaimer 
;;*
;;* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
;;***********************************************************************************************************************/
;;/***********************************************************************************************************************
;;* File Name    : iar_call_inits.s
;;* Description  : Defines _CALL_INIT function that is called from startup in C++ project using IAR Compiler.
;;***********************************************************************************************************************/
;;/***********************************************************************************************************************
;;* History : DD.MM.YYYY Version   Description
;;*         : 28.02.2023 1.00      First Release
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
