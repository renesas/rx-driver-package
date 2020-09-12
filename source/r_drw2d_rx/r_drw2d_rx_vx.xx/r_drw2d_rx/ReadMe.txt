drw_2d
=========

Overview
--------
DRW2D Driver for RX.

Release history
----------------
DD.MM.YYYY Version Description
26.03.2018 1.02    First release.
28.06.2019 1.03    Folder name was changed.
28.02.2020 1.10    Supported Smart Configurator.

File Structure
--------------
drw_2d
+---ReadMe.txt
|
+---doc 
|    +-- d2_driver(TES's DRW2D documents)
|    +-- en (APN English)
|    +-- ja (APN Japanese)
|    +-- License.txt
|
+---inc
|    +-- tes (TES's driver version 3.17)
+---ref 
|    +-- r_drw2d_rx_config_reference.h
+---src
     +-- rx (Source code depending on RX)
     |
     +-- tes (TES's driver source code)

Limitation
----------
This driver does not support big endian.

Usage Note
----------
 At initial setting Excep_ICU_GROUPAL1 function, which is the interrupt function of DRW2D, (around line 300 of dave_base_rx.c) 
 has been set to invalidate by using the macro of ICU_GROUPAL1_ENABLE considering using BSP at the same time.

 #define ICU_GROUPAL1_ENABLE    (0) /*ICU_GROUPAL1 Enable/Disable*/

 Two options described below allow user set DRW2D interrupt. 
 1)Change ICU_GROUPAL1_ENABLE from 0 to 1, 
   and comment out the function of group_al1_handler_isr in mcu_interrupt.c in BSP.
   This way also makes the other peripherals interrupt in the same group interrupt as DRW2D invalidate.

   When the same group interrupt is used by DRW2D and another peripheral device, 
   follow the next description.

 2)
 a) For functions created by the user, use the BSP API to register the interrupt handler
    for DRW2D. 
    /* Add an include statement for platform.h. */
    #include "platform.h"

    /* Add an extern declaration for the DRW2D interrupt handler to the registered function. */
    extern void drw_int_isr(void);

    user_function()
    {
        bsp_int_ctrl_t grpal1;

        /* Specify the priority of the group interrupt. */
        grpal1.ipl = 7;

        /* Use the BSP API to register the interrupt handler for DRW2D. */
        R_BSP_InterruptWrite(BSP_INT_SRC_AL1_DRW2D_DRW_IRQ, (bsp_int_cb_t)drw_int_isr);

        /* Use the BSP API to enable the group interrupt. */
        R_BSP_InterruptControl(BSP_INT_SRC_AL1_DRW2D_DRW_IRQ, BSP_INT_CMD_GROUP_INTERRUPT_ENABLE, (void *)&group_priority);
        ¥
        ¥
        ¥
    }


 b) In the DRW2D program, set the group interrupt priority based on the define macro 
    below (near line 26 of dave_irq_rx.c).

    If it is necessary to adjust the priority of the group interrupt, change the value of
    the define macro below. (The setting range is 0 to 15, with 0 specifying interrupt 
    disabled.)

    Note that the group interrupt has the same priority for the other peripheral device 
    using the same group interrupt as DRW2D.

    #define DAVE_IRQ_IPR     (7)

 Note: For details of BSP APIs, refer to the BSP application note (R01AN1685).

END OF DOCUMENT