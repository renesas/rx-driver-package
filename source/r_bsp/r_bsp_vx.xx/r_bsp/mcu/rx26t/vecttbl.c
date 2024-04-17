/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : vecttbl.c
* Device(s)    : RX26T
* Description  : Definition of the exception vector table, reset vector, and user boot options.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version   Description
*         : 28.02.2023 1.00      First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* BSP configuration. */
#include "platform.h"

/* When using the user startup program, disable the following code. */
#if BSP_CFG_STARTUP_DISABLE == 0

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
R_BSP_POR_FUNCTION(R_BSP_POWER_ON_RESET_FUNCTION);

/***********************************************************************************************************************
* The following array fills in the option function select registers and the ID code protection bytes.
***********************************************************************************************************************/
#ifdef __BIG
    #define BSP_PRV_MDE_VALUE (0xfffffff8)    /* big */
#else
    #define BSP_PRV_MDE_VALUE (0xffffffff)    /* little */
#endif

#if BSP_CFG_ID_CODE_ENABLE == 1
    #define BSP_PRV_SPCC_IDE   (0xfeffffff)  /* ID code protection is enabled after a reset. */
#else
    #define BSP_PRV_SPCC_IDE   (0xffffffff)  /* ID code protection is disabled after a reset. */
#endif

#if (BSP_CFG_BLOCK_ERASE_CMD_PROTECT_ENABLE == 1) || (BSP_CFG_ID_CODE_ENABLE == 1)
    #define BSP_PRV_SPCC_SEPR  (0xdfffffff)  /* Block erasure command protection after a reset is enabled. */
#else
    #define BSP_PRV_SPCC_SEPR  (0xffffffff)  /* Block erasure command protection after a reset is disabled. */
#endif

#if (BSP_CFG_PROGRAM_CMD_PROTECT_ENABLE == 1) || (BSP_CFG_ID_CODE_ENABLE == 1)
    #define BSP_PRV_SPCC_WRPR  (0xbfffffff)  /* Programming command protection after a reset is enabled. */
#else
    #define BSP_PRV_SPCC_WRPR  (0xffffffff)  /* Programming command protection after a reset is disabled. */
#endif

#if (BSP_CFG_READ_CMD_PROTECT_ENABLE == 1) || (BSP_CFG_ID_CODE_ENABLE == 1)
    #define BSP_PRV_SPCC_RDPR  (0x7fffffff)  /* Read command protection after a reset is enabled. */
#else
    #define BSP_PRV_SPCC_RDPR  (0xffffffff)  /* Read command protection after a reset is disabled. */
#endif

#if BSP_CFG_SERIAL_PROGRAMMER_CONECT_ENABLE == 0
    #define BSP_PRV_SPCC_SPE   (0xf7ffffff)  /* Connection of a serial programmer after a reset is prohibited. */
#else
    #define BSP_PRV_SPCC_SPE   (0xffffffff)  /* Connection of a serial programmer after a reset is permitted. */
#endif

#define BSP_PRV_SPCC_VALUE ((((BSP_PRV_SPCC_IDE & BSP_PRV_SPCC_SEPR) & BSP_PRV_SPCC_WRPR) & BSP_PRV_SPCC_RDPR) & BSP_PRV_SPCC_SPE)

#if BSP_CFG_MCU_PART_MEMORY_SIZE == 0xF
  #if BSP_CFG_CODE_FLASH_BANK_MODE == 0 /* In the case of 512Kbyte ROM capacity. */
    #define BSP_PRV_BANK_MODE_VALUE (0xffffff8f)    /* dual */
  #else
    #define BSP_PRV_BANK_MODE_VALUE (0xffffffff)    /* linear */
  #endif
#else
    #define BSP_PRV_BANK_MODE_VALUE (0xffffffff)    /* linear */
#endif

#if BSP_CFG_MCU_PART_MEMORY_SIZE == 0xF
  #if BSP_CFG_CODE_FLASH_START_BANK == 0
    /* The address range of bank 1 from FFF80000h to FFFBFFFFh and bank 0 from FFFC0000h to FFFFFFFFh. */
    #define BSP_PRV_START_BANK_VALUE (0xffffffff)
  #else
    /* The address range of bank 1 from FFFC0000h to FFFFFFFFh and bank 0 from FFF80000h to FFFBFFFFh. */
    #define BSP_PRV_START_BANK_VALUE (0xfffffff8)
  #endif
#endif

#if defined(__CCRX__)

#pragma address __SPCCreg    = 0x00120040
#pragma address __TMEFreg    = 0x00120048
#pragma address __OSIS1reg   = 0x00120050
#pragma address __OSIS2reg   = 0x00120054
#pragma address __OSIS3reg   = 0x00120058
#pragma address __OSIS4reg   = 0x0012005c
#pragma address __TMINFreg   = 0x00120060
#pragma address __MDEreg     = 0x00120064
#pragma address __OFS0reg    = 0x00120068
#pragma address __OFS1reg    = 0x0012006c
#if BSP_CFG_MCU_PART_MEMORY_SIZE == 0xF
#pragma address __BANKSELreg = 0x00120090
#endif
#pragma address __FAWreg     = 0x001200A0

const uint32_t __SPCCreg    = BSP_PRV_SPCC_VALUE;
const uint32_t __TMEFreg    = BSP_CFG_TRUSTED_MODE_FUNCTION;
const uint32_t __OSIS1reg   = BSP_CFG_ID_CODE_LONG_1;
const uint32_t __OSIS2reg   = BSP_CFG_ID_CODE_LONG_2;
const uint32_t __OSIS3reg   = BSP_CFG_ID_CODE_LONG_3;
const uint32_t __OSIS4reg   = BSP_CFG_ID_CODE_LONG_4;
const uint32_t __TMINFreg   = 0xffffffff;
const uint32_t __MDEreg     = (BSP_PRV_MDE_VALUE & BSP_PRV_BANK_MODE_VALUE);
const uint32_t __OFS0reg    = BSP_CFG_OFS0_REG_VALUE;
const uint32_t __OFS1reg    = BSP_CFG_OFS1_REG_VALUE;
#if BSP_CFG_MCU_PART_MEMORY_SIZE == 0xF
const uint32_t __BANKSELreg = BSP_PRV_START_BANK_VALUE;
#endif
const uint32_t __FAWreg     = BSP_CFG_FAW_REG_VALUE;

#elif defined(__GNUC__)

const uint32_t __SPCCreg    __attribute__ ((section(".ofs1"))) = BSP_PRV_SPCC_VALUE;
const uint32_t __TMEFreg    __attribute__ ((section(".ofs2"))) = BSP_CFG_TRUSTED_MODE_FUNCTION;
const st_ofsm_sec_ofs3_t __ofsm_sec_ofs3   __attribute__ ((section(".ofs3"))) = {
    BSP_CFG_ID_CODE_LONG_1, /* __OSIS1reg */
    BSP_CFG_ID_CODE_LONG_2, /* __OSIS2reg */
    BSP_CFG_ID_CODE_LONG_3, /* __OSIS3reg */
    BSP_CFG_ID_CODE_LONG_4  /* __OSIS4reg */
};
const st_ofsm_sec_ofs4_t __ofsm_sec_ofs4   __attribute__ ((section(".ofs4"))) = {
    0xffffffff, /* __TMINFreg */
    (BSP_PRV_MDE_VALUE & BSP_PRV_BANK_MODE_VALUE), /* __MDEreg */
    BSP_CFG_OFS0_REG_VALUE, /* __OFS0reg */
    BSP_CFG_OFS1_REG_VALUE  /* __OFS1reg */
};
#if BSP_CFG_MCU_PART_MEMORY_SIZE == 0xF
const uint32_t __BANKSELreg __attribute__ ((section(".ofs5"))) = BSP_PRV_START_BANK_VALUE;
#endif
const uint32_t __FAWreg     __attribute__ ((section(".ofs6"))) = BSP_CFG_FAW_REG_VALUE;

#elif defined(__ICCRX__)

#pragma public_equ = "__SPCC", BSP_PRV_SPCC_VALUE
#pragma public_equ = "__TMINF", 0xffffffff
#pragma public_equ = "__OSIS_1", BSP_CFG_ID_CODE_LONG_1
#pragma public_equ = "__OSIS_2", BSP_CFG_ID_CODE_LONG_2
#pragma public_equ = "__OSIS_3", BSP_CFG_ID_CODE_LONG_3
#pragma public_equ = "__OSIS_4", BSP_CFG_ID_CODE_LONG_4
#pragma public_equ = "__TMEF", BSP_CFG_TRUSTED_MODE_FUNCTION
#pragma public_equ = "__MDE", (BSP_PRV_MDE_VALUE & BSP_PRV_BANK_MODE_VALUE)
#pragma public_equ = "__OFS0", BSP_CFG_OFS0_REG_VALUE
#pragma public_equ = "__OFS1", BSP_CFG_OFS1_REG_VALUE
#if BSP_CFG_MCU_PART_MEMORY_SIZE == 0xF
#pragma public_equ = "__BANKSEL", BSP_PRV_START_BANK_VALUE
#endif
#pragma public_equ = "__FAW", BSP_CFG_FAW_REG_VALUE

#endif /* defined(__CCRX__), defined(__GNUC__), defined(__ICCRX__) */

/***********************************************************************************************************************
* The following array fills in the exception vector table.
***********************************************************************************************************************/
#if BSP_CFG_RTOS_USED == 4  /* Renesas RI600V4 & RI600PX */
     /* System configurator generates the ritble.src as interrupt & exception vector tables. */
#else /* BSP_CFG_RTOS_USED!=4 */

#if defined(__CCRX__) || defined(__GNUC__)
R_BSP_ATTRIB_SECTION_CHANGE_EXCEPTVECT void (* const Except_Vectors[])(void) =
{
    /* Offset from EXTB: Reserved area - must be all 0xFF */
    (void (*)(void))0xFFFFFFFF,  /* 0x00 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x04 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x08 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x0c - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x10 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x14 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x18 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x1c - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x20 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x24 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x28 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x2c - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x30 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x34 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x38 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x3c - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x40 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x44 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x48 - Reserved */
    (void (*)(void))0xFFFFFFFF,  /* 0x4c - Reserved */

    /* Exception vector table */
    excep_supervisor_inst_isr,         /* 0x50  Exception(Supervisor Instruction) */
    excep_access_isr,                  /* 0x54  Exception(Access exception) */
    undefined_interrupt_source_isr,    /* 0x58  Reserved */
    excep_undefined_inst_isr,          /* 0x5c  Exception(Undefined Instruction) */
    undefined_interrupt_source_isr,    /* 0x60  Reserved */
    excep_floating_point_isr,          /* 0x64  Exception(Floating Point) */
    undefined_interrupt_source_isr,    /* 0x68  Reserved */
    undefined_interrupt_source_isr,    /* 0x6c  Reserved */
    undefined_interrupt_source_isr,    /* 0x70  Reserved */
    undefined_interrupt_source_isr,    /* 0x74  Reserved */
    non_maskable_isr,                  /* 0x78  NMI */
};
R_BSP_ATTRIB_SECTION_CHANGE_END
#endif /* defined(__CCRX__), defined(__GNUC__) */

/***********************************************************************************************************************
* The following array fills in the reset vector.
***********************************************************************************************************************/
#if defined(__CCRX__) || defined(__GNUC__)
R_BSP_ATTRIB_SECTION_CHANGE_RESETVECT void (* const Reset_Vector[])(void) =
{
    R_BSP_POWER_ON_RESET_FUNCTION                   /* 0xfffffffc  RESET */
};
R_BSP_ATTRIB_SECTION_CHANGE_END
#endif /* defined(__CCRX__), defined(__GNUC__) */

#endif/* BSP_CFG_RTOS_USED */

#endif /* BSP_CFG_STARTUP_DISABLE == 0 */

