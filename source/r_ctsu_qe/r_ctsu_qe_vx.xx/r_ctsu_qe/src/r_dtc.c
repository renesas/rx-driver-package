/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_dtc.c
* Version      : 1.00
* Device(s)    : R5F5113xAxFP,R5F5231xAxFP,R5F51305AxFN
* Description  : This file handles DTC operations specific to the QECTSU driver.
***********************************************************************************************************************/

/***********************************************************************************************************************
* History      : DD.MM.YYYY  Version  Description
*              : 08.06.2018  1.00     First Release
***********************************************************************************************************************/
/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "platform.h"
#include "r_dtc.h"
#include "r_ctsu_qe_config.h"

#if (CTSU_CFG_USE_DTC == 1)
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
External functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Global variables and functions
***********************************************************************************************************************/
#if (defined(BSP_MCU_RX23_ALL) || defined(BSP_MCU_RX113))
    #pragma address dtc_vector60=0x00007CF0U
    uint32_t dtc_vector60;
    dtc_register_data_t g_dtc_info_ctsu_wr;                    /* DTC transfer information for CTSU WR     */

    #pragma address dtc_vector61=0x00007CF4U
    uint32_t dtc_vector61;
    dtc_register_data_t g_dtc_info_ctsu_rd;                    /* DTC transfer information for CTSU RD     */
#endif

#ifdef BSP_MCU_RX130
    #pragma address dtc_vector60=0x000024F0U
    uint32_t dtc_vector60;
    dtc_register_data_t g_dtc_info_ctsu_wr;                    /* DTC transfer information for CTSU WR     */

    #pragma address dtc_vector61=0x000024F4U
    uint32_t dtc_vector61;
    dtc_register_data_t g_dtc_info_ctsu_rd;                    /* DTC transfer information for CTSU RD     */
#endif


/***********************************************************************************************************************
* Function Name: dtc_ctsu_init
* Description  : This function initializes the DTC for transferring data for the CTSU write and read interrupts.
*                WARNING! If DTC is used elsewhere in the application, conflicts may exist. Note the location of
*                the DTC vector locations.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void dtc_ctsu_init(void)
{
    /* based upon legacy DTC_Set_Initial_of_CTSU() */

    /* Get DTC peripheral out of stop condition */
    R_BSP_RegisterProtectDisable (BSP_REG_PROTECT_LPC_CGC_SWR);
    MSTP(DTC) = 0;
    R_BSP_RegisterProtectEnable (BSP_REG_PROTECT_LPC_CGC_SWR);

    DTC.DTCST.BIT.DTCST = _0_DTCST_STOP;            // do not accept DTC transfer requests
    DTC.DTCADMOD.BIT.SHORT = _0_SHORT_FULL;         // use full-address mode


    /* Set up write block for CTSUSSC/SO0/SO1 registers (ctsu write interrupt) */

    g_dtc_info_ctsu_wr.word          = 0x0000;              // Initialize MRA and MRB = 0
    g_dtc_info_ctsu_wr.MRA.bit.MD    = _10_MD_BLOCK;        // Block transfer mode setting
    g_dtc_info_ctsu_wr.MRA.bit.SZ    = _01_SZ_WORD;         // Word transfer setting
    g_dtc_info_ctsu_wr.MRA.bit.SM    = _10_SM_INC;          // SAR value is incremented after data transfer

    g_dtc_info_ctsu_wr.MRB.bit.CHNE  = _0_CHNE_DISABLE;     // Chain transfer disable setting
    g_dtc_info_ctsu_wr.MRB.bit.DISEL = _0_DISEL_COMPLETED;  // Interrupt when specified data transfer is completed
    g_dtc_info_ctsu_wr.MRB.bit.DTS   = _0_DTS_DESTINATION;  // Transfer destination side is repeat area or block area
    g_dtc_info_ctsu_wr.MRB.bit.DM    = _10_DM_INC;          // DAR value is incremented after data transfer
                                                            // Source Register set to initialized array in DTC_Set_Start_of_CTSU()
    g_dtc_info_ctsu_wr.DAR = (uint32_t)&CTSU.CTSUSSC.WORD;  // Destination Register is address of CTSUSSC register
    g_dtc_info_ctsu_wr.CRA           = CTSU_DTC_WR_CNT;     // Transfer count setting (3 words)
    g_dtc_info_ctsu_wr.CRB           = 0x0000;              // Not used in normal transfer


    /* Set up read block for CTSUSC/RX registers (ctsu read interrupt) */

    g_dtc_info_ctsu_rd.word          = 0x0000;              // Initialize MRA and MRB = 0
    g_dtc_info_ctsu_rd.MRA.bit.MD    = _10_MD_BLOCK;        // Block transfer mode setting
    g_dtc_info_ctsu_rd.MRA.bit.SZ    = _01_SZ_WORD;         // Word transfer setting
    g_dtc_info_ctsu_rd.MRA.bit.SM    = _10_SM_INC;          // SAR value is incremented after data transfer

    g_dtc_info_ctsu_rd.MRB.bit.CHNE  = _0_CHNE_DISABLE;     // Chain transfer disable setting
    g_dtc_info_ctsu_rd.MRB.bit.DISEL = _0_DISEL_COMPLETED;  // Interrupt when specified data transfer is completed
    g_dtc_info_ctsu_rd.MRB.bit.DTS   = _1_DTS_SOURCE;       // Transfer source side is repeat area or block area
    g_dtc_info_ctsu_rd.MRB.bit.DM    = _10_DM_INC;          // DAR value is incremented after data transfer

    g_dtc_info_ctsu_rd.SAR = (uint32_t)&CTSU.CTSUSC.WORD;   // Source Register is address of CTSUSC register
                                                            // Destination Reg is addr of scan buf set in DTC_Set_Start_of_CTSU()
    g_dtc_info_ctsu_rd.CRA           = CTSU_DTC_RD_CNT;     // Transfer count setting (2 words)
    g_dtc_info_ctsu_rd.CRB           = 0x0000;              //

    DTC.DTCCR.BIT.RRS = _1_RRS_SKIP;        // Skip setting


    /* Set addresses in DTC vector table */
    dtc_vector60 = (uint32_t)&g_dtc_info_ctsu_wr;
    dtc_vector61 = (uint32_t)&g_dtc_info_ctsu_rd;
#if (defined(BSP_MCU_RX23_ALL) || defined(BSP_MCU_RX113))
    DTC.DTCVBR = (void *)0x00007C00U;
#endif
#ifdef BSP_MCU_RX130
    DTC.DTCVBR = (void *)0x00002400U;
#endif

    /* Accept transfer requests */
    DTCE(CTSU,CTSUWR) = 1;                  // DTC activation by CTSU WR interrupt request is enabled
    DTCE(CTSU,CTSURD) = 1;                  // DTC activation by CTSU RD interrupt request is enabled
    DTC.DTCST.BIT.DTCST = _1_DTCST_START;   // DTC accept transfer requests

}


/***********************************************************************************************************************
* Function Name: dtc_ctsu_xfer_addrs
* Description  : This function sets the non-static addresses for DTC transfers for the CTSU write and read interrupts.
* Arguments    : wr_int_src_addr -
*                    Buffer address of values to transfer into CTSUSSC, SO0, and SO1 registers
*                rd_int_dst_addr -
*                    Scan buffer address to transfer CTSUSC and CTSURC register values into.
* Return Value : None
***********************************************************************************************************************/
void dtc_ctsu_xfer_addrs(uint32_t wr_int_src_addr, uint32_t rd_int_dst_addr)
{

    /* HW Manual Section 16.4.1: When updating the vector table and transfer information,
     * set the RRS bit to 0, update the vector table and transfer information,
     * and then set the RRS bit to 1.
     */
    DTC.DTCCR.BIT.RRS = 0;

    /* CRB holds the block transfer count. Typically, this would be reset here to the number of
     * elements to be scanned. By constantly resetting it to 0, the counter is effectively
     * reset to 65536, so transfers will simply always occur (not concerned with auto-stopping
     * of transfers).
     */

    /* Set source address for write interrupt/DTC transfer 1 (destination always same) */
    g_dtc_info_ctsu_wr.SAR = wr_int_src_addr;
    g_dtc_info_ctsu_wr.CRB = 0;

    /* Set destination address for read interrupt/DTC transfer 2 (source always same) */
    g_dtc_info_ctsu_rd.DAR = rd_int_dst_addr;
    g_dtc_info_ctsu_rd.CRB = 0;

    DTC.DTCCR.BIT.RRS = 1;
}

#endif // (CTSU_CFG_USE_DTC == 1)
