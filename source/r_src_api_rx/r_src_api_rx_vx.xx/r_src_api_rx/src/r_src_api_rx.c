/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name    : r_src_api_rx.c
* Version      : 1.14
* Device       : RX64M, RX71M
* Tool-Chain   : RX Family C Compiler v3.02.00
*                GCC for Renesas RX 8.3.0.201904
*                IAR C/C++ Compiler for Renesas RX version 4.13.1
* Description  : SRC API for RX
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 11.07.2014 1.00    First Release
*         : 05.09.2014 1.10    Second Release
*                              'R_SRC_Write() modification to apply lower PCLKB
*                               clock.
*         : 11.12.2014 1.11    RX71M support added.
*         : 20.05.2019 1.13    Supported the following compilers:
*                              - GCC for Renesas RX
*                              - IAR C/C++ Compiler for Renesas RX
*                              Added comments "WAIT_LOOP"
*         : 10.06.2020 1.14    Added support for atomic control.
*                              Modified comment of API function to Doxygen style.
*
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Configuration options for the SRC API. This is also included in 
   r_src_api_rx.h and would normally not need to be included separately.
   It is included separately here so that the decision can be made to use
   the r_bsp package or not. */
#include "r_src_api_rx_config.h"
/* Function prototypes and device specific info needed for SRC API */
#include "r_src_api_rx_if.h"
/* Information needed for SRC API. */
#include "r_src_api_rx_private.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/* Inhibit parameters for IFS */
#define  SRC_IFS_INHIBIT_3   (3)
#define  SRC_IFS_INHIBIT_7   (7)
/* Inhibit parameters for OFS */
#define  SRC_OFS_INHIBIT_3   (3)

/* input FIFO size */
#define  SRC_INFIFO_SIZE     (8)

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables 
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SRC_Open
 *****************************************************************************************************************/ /**
 * @brief Locks to keep SRC peripheral, initializes and configures it according to r_src_api_rx_config.h
 * @retval    SRC_SUCCESS    Successful, SRC peripheral is configured.
 * @retval    SRC_ERR_LOCKED Not successful, because SRC peripheral is already locked.
 * @details   Certainly call this function once before starting to use SRC peripheral. \n
 *            It does following items for SRC peripheral to use.\n
 *            \li Locks to keep SRC peripheral.
 *            \li Cancels SRC peripheral's module stop state.
 *            \li Initializes SRC peripheral registers.
 *            \li Configures SRC peripheral registers according to file r_src_api_rx_config.h.
 *            \li Downloads filter coefficients. (contained in file r_src_api_rx_coef.h)
 *
 * @note      None.
 */
src_ret_t R_SRC_Open( void )
{
/* Note: The #include file below is removed for GSCE rule 6.6. From the "SCOPE"
            point of view this should be placed here.
*/
/* Filter coefficient table. */
#include "r_src_api_rx_coef_table.h"

    src_ret_t ret = SRC_SUCCESS;
    uint32_t  count = 0;
    
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
    bsp_int_ctrl_t int_ctrl;
#endif


    /* Check hardware lock */
    if ( false == R_BSP_HardwareLock(BSP_LOCK_SRC) )
    {
        /* case : hardware unlocked. */
        ret = SRC_ERR_LOCKED;
    }
    else
    {
        /* case : hardware locked. */
        /* Start feeding clock to SRC peripheral to activate it. */
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
        MSTP(SRC) = 0;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

        /* Configure SRC peripheral according to user configuration. */
        SRC.SRCCTRL.WORD    = 0;                /* Clear SRCCTRL register */
        SRC.SRCCTRL.BIT.CL  = SRC_CL_ON;        /* Clear internal work memory */

        SRC.SRCIDCTRL.BIT.IFTRG = SRC_IFTG;     /* Set user configured IFTG value */
        SRC.SRCIDCTRL.BIT.IED   = SRC_IED_OFF;  /* Set user configured IED value */
        SRC.SRCIDCTRL.BIT.IEN   = SRC_IEN_OFF;  /* Disable input data empty interrupt */

        SRC.SRCODCTRL.BIT.OFTRG = SRC_OFTG;     /* Set user configured OFTG value */
        SRC.SRCODCTRL.BIT.OED   = SRC_OED_OFF;  /* Set user configured OED value */
        SRC.SRCODCTRL.BIT.OCH   = SRC_OCH;      /* Set user configured OCH value */
        SRC.SRCODCTRL.BIT.OEN   = SRC_OEN_OFF;  /* Disable output data full interrupt */

        SRC.SRCCTRL.BIT.FICRAE  = SRC_FICRAE_WR_ENABLE; /* Enable to write filter coefficient */

        /* Write filter coefficient to SRC peripheral. */
        /* WAIT_LOOP */
        for ( count = 0; count < SRC_COEF_TABLE_SIZE; count++ )
        {
            SRC.SRCFCTR[count].LONG = src_filter_coef[count];
        }
        SRC.SRCCTRL.BIT.FICRAE  = SRC_FICRAE_WR_DISABLE; /* Disable to write filter coefficient */

        /* clear SRCSTS (status register) */
        if ( 1 == SRC.SRCSTAT.BIT.CEF )
        {
            SRC.SRCSTAT.BIT.CEF = 0;
        }
        else
        {
            /* Do nothing */
        }

        if ( 1 == SRC.SRCSTAT.BIT.UDF )
        {
            SRC.SRCSTAT.BIT.UDF = 0;
        }
        else
        {
            /* Do nothing */
        }

        if ( 1 == SRC.SRCSTAT.BIT.OVF )
        {
            SRC.SRCSTAT.BIT.OVF = 0;
        }
        else
        {
            /* Do nothing */
        }

        if ( 1 == SRC.SRCSTAT.BIT.IINT )
        {
            SRC.SRCSTAT.BIT.IINT = 0;
        }
        else
        {
            /* Do nothing */
        }

        if ( 1 == SRC.SRCSTAT.BIT.OINT )
        {
            SRC.SRCSTAT.BIT.OINT = 0;
        }
        else
        {
            /* Do nothing */
        }
    }

    return ret;
}

/**********************************************************************************************************************
 * Function Name: R_SRC_Close
 *****************************************************************************************************************/ /**
 * @brief Unlocks to release SRC peripheral.
 * @retval    SRC_SUCCESS    Successful, unlocked to release SRC peripheral.
 * @retval    SRC_ERR_UNLOCK Not successful, because the SRC peripheral is not locked yet.
 * @details   Call this function when finish to use SRC peripheral. \n
 *            It does following items for SRC peripheral to use.\n
 *            \li Checks if SRC peripheral is locked and if not, it returns SRC_ERR_UNLOCK.
 *            \li Unlocks to release SRC peripheral.
 *            \li Sets SRC peripheral to module stop state.
 *
 * @note      None.
 */
src_ret_t R_SRC_Close( void )
{
    src_ret_t ret = SRC_SUCCESS;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
    bsp_int_ctrl_t int_ctrl;
#endif

    /* Check hardware lock */
    if ( true == R_BSP_HardwareLock(BSP_LOCK_SRC) )
    {
        /* case : hardware unlocked. */
        R_BSP_HardwareUnlock(BSP_LOCK_SRC);           /* Release hardware lock */
        ret = SRC_ERR_UNLOCK;
    }
    else
    {
        /* case : hardware locked. */
        SRC.SRCCTRL.WORD    = 0;                /* Clear SRCCTRL register */
        SRC.SRCCTRL.BIT.CL  = SRC_CL_ON;        /* Clear internal work memory */
        SRC.SRCIDCTRL.WORD  = 0;                /* Clear SRCIDCTRL register */
        SRC.SRCODCTRL.WORD  = 0;                /* Clear SRCODCTRL register */

        if ( false == R_BSP_HardwareUnlock(BSP_LOCK_SRC) )
        {
            ret = SRC_ERR_UNLOCK;
        }
        else
        {
            /* Do nothing */
        }

        /* Stop feeding clock to SRC peripheral to inactivate it for power efficiency. */
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
        MSTP(SRC) = 1;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
    }

    return ret;
}

/**********************************************************************************************************************
 * Function Name: R_SRC_Start
 *****************************************************************************************************************/ /**
 * @brief Starts sampling rate conversion corresponding to the arguments.
 * @param[in] fsi
 *             It is the sampling frequency of the PCM data before sampling rate conversion.
 *             Choose one enumerator member from the enumerator typedef src_ifs_t described in file r_src_api_rx_if.h.
 * @param[in] fso
 *             It is the sampling frequency of the PCM data after sampling rate conversion.
 *             Choose one enumerator member from the enumerator typedef src_ofs_t described in file r_src_api_rx_if.h.
 * @param[in] ied
 *             It specifies the endian format of input PCM data.
 *             Choose one enumerator from the enumerator typedef src_ied_t described in file r_src_api_rx_if.h.
 * @param[in] oed
 *             It specifies the endian format of output PCM data.
 *             Choose one enumerator from the enumerator typedef src_oed_t described in file r_src_api_rx_if.h.
 * @retval    SRC_SUCCESS    Successful, SRC started sampling rate conversion.
 * @retval    SRC_PARAM      Parameter is Illegal.
 * @retval    SRC_ERR_UNLOCK SRC peripheral is not locked yet.
 * @retval    SRC_NOT_END    Flush processing is not completed.
 * @details   Call this function when starting sampling rate conversion. \n
 *            It does following processes to start sampling rate conversion.\n
 *            \li Checks if SRC peripheral is locked and if not, it returns SRC_ERR_UNLOCK.
 *            \li Checks the legality of parameters, and if they are illegal it returns SRC_ERR_PARAM.
 *            \li Checks if no flush process on-going. If the process is on-going, it returns SRC_NOT_END.
 *            \li Clears SRC peripheral's internal data.
 *            \li Sets the sampling frequency of the PCM data before sampling rate conversion corresponding to \e ifs.
 *            \li Sets the sampling frequency of the PCM data after sampling rate conversion corresponding to \e ofs.
 *            \li Sets the endian format of input PCM data corresponding to \e ied.
 *            \li Sets the endian format of output PCM data corresponding to \e oed.
 *            \li Enables or disables input Data FIFO Empty interrupt and output Data FIFO Full interrupt according to
 *                r_src_api_rx_config.h.
 *            \li Start sampling rate conversion.
 *
 * @note      None.
 */
src_ret_t R_SRC_Start( src_ifs_t fsi, src_ofs_t fso, src_ied_t ied, src_oed_t oed )
{
    /* Check hardware lock */
    if ( true == R_BSP_HardwareLock(BSP_LOCK_SRC) )
    {
        /* case : hardware unlocked. */
        R_BSP_HardwareUnlock(BSP_LOCK_SRC);           /* Release hardware lock */
        return ( SRC_ERR_UNLOCK );
    }
    else
    {
        /* Do nothing */
    }

    /* case : hardware locked. */
    /* Check arguments if the value are adequate or not. */
    if ( (SRC_IFS_48 < fsi) || (SRC_IFS_INHIBIT_7 == fsi) || (SRC_IFS_INHIBIT_3 == fsi) )
    {
        return ( SRC_ERR_PARAM );
    }
    else
    {
        /* Do nothing */
    }

    if ( (SRC_OFS_16 < fso) || (SRC_OFS_INHIBIT_3 == fso) )
    {
        return ( SRC_ERR_PARAM );
    }
    else
    {
        /* Do nothing */
    }

    if ( SRC_IFS_44 != fsi )
    {
        if ( (SRC_OFS_8 == fso) || (SRC_OFS_16 == fso) )
        {
            return ( SRC_ERR_PARAM );
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    /* Although IAR generates a warning, it does not affect the operation. */
    if ( (SRC_IED_ON < ied) || (SRC_IED_OFF > ied) )
    {
        return ( SRC_ERR_PARAM );
    }
    else
    {
        /* Do nothing */
    }

    /* Although IAR generates a warning, it does not affect the operation. */
    if ( (SRC_OED_ON < oed) || (SRC_OED_OFF > oed) )
    {
        return ( SRC_ERR_PARAM );
    }
    else
    {
        /* Do nothing */
    }

    /* Check SRCSTAT if flush operation is on-going or not. */
    if ( 1 == SRC.SRCSTAT.BIT.FLF )
    {
        return ( SRC_NOT_END );
    }
    else
    {
        /* Do nothing */
    }

    /* Configure SRC to start sampling rate conversion. */
    SRC.SRCCTRL.BIT.SRCEN = SRC_SRCEN_OFF;  /* Disable conversion */

    SRC.SRCCTRL.BIT.CL    = SRC_CL_ON;      /* Clear internal work memory */
    SRC.SRCCTRL.BIT.IFS   = fsi;            /* Set user configured IFS value */
    SRC.SRCCTRL.BIT.OFS   = fso;            /* Set user configured OED value */
    SRC.SRCIDCTRL.BIT.IED = ied;            /* Set user configured IED value */
    SRC.SRCODCTRL.BIT.OED = oed;            /* Set user configured OED value */
    SRC.SRCIDCTRL.BIT.IEN = SRC_IEN;        /* Set user configured IEN value */
    SRC.SRCODCTRL.BIT.OEN = SRC_OEN;        /* Set user configured OEN value */

    SRC.SRCCTRL.BIT.SRCEN = SRC_SRCEN_ON;   /* Enable sample rate conversion */

    return ( SRC_SUCCESS );

}

/**********************************************************************************************************************
 * Function Name: R_SRC_Stop
 *****************************************************************************************************************/ /**
 * @brief Triggers flush processing to finish sampling rate conversion.
 * @retval    SRC_SUCCESS    Successful, request to stop is accepted.
 * @retval    SRC_ERR_UNLOCK SRC peripheral is not locked yet.
 * @retval    SRC_NOT_END    Flush processing is not completed.
 * @retval    SRC_END        Flush processing is completed.
 * @details   Call this function when trigger flush processing to finish sampling rate conversion. \n
 *            It does following processes.\n
 *            \li Checks if SRC peripheral is locked. If not, it returns SRC_ERR_UNLOCK.
 *            \li Checks if no flush process on-going. If the process is on-going, it returns SRC_NOT_END.
 *                Or already completed, it returns SRC_END.
 *            \li Disables input Data FIFO Empty interrupt.
 *            \li Triggers flush processing.
 *
 *            Note that after R_SRC_Stop() call, R_SRC_Read() should be continuously called until all data to be read
 *            from SRC peripheral to complete flush processing.
 * @note      None.
 */
src_ret_t R_SRC_Stop( void )
{
    /* Check hardware lock */
    if ( true == R_BSP_HardwareLock(BSP_LOCK_SRC) )
    {
        /* case : hardware unlocked. */
        R_BSP_HardwareUnlock(BSP_LOCK_SRC);           /* Release hardware lock */
        return ( SRC_ERR_UNLOCK );
    }
    else
    {
        /* Do nothing */
    }

    /* case : hardware locked. */
    /* Check SRCSTAT if flush operation is already on-going or not. */
    if ( 1 == SRC.SRCSTAT.BIT.FLF )
    {
        return ( SRC_NOT_END );
    }
    else
    {
        /* Do nothing */
    }

    if ( 1 == SRC.SRCSTAT.BIT.CEF )
    {
        return ( SRC_END );
    }
    else
    {
        /* Do nothing */
    }

    /* Disable Input FIFO empty interrupt and execute flash operation*/
    SRC.SRCIDCTRL.BIT.IEN = SRC_IEN_OFF;
    SRC.SRCCTRL.BIT.FL = SRC_FL_ON;

    return ( SRC_SUCCESS );

}

/**********************************************************************************************************************
 * Function Name: R_SRC_Write
 *****************************************************************************************************************/ /**
 * @brief Write the PCM data before sampling rate conversion to SRC peripheral.
 * @param[in] buf
 *             It specifies the starting address of PCM data buffer to write to Input Data Register of SRC peripheral.
 *             See Section 3 in the application note, the PCM data in the buffer must be aligned as the figure
 *             in API function "R_SRC_Write". The one sample is defined as a pair of 16 bit PCM data.
 * @param[in] samples
 *             It specifies the number of PCM data samples to write to Input Data Register of SRC peripheral.
 * @return    <b>The number of written samples:</b> Shows the number of PCM data samples written to
 *               Input Data Register.\n
 *            <b>SRC_PARAM:</b> Parameter is Illegal.\n
 *            <b>SRC_ERR_UNLOCK:</b> SRC peripheral is not locked yet.\n
 *            <b>SRC_NOT_END:</b> Flush processing is not completed.\n
 *            <b>SRC_END:</b> Flush processing is completed.
 * @details   Call this function to write the PCM data before sampling rate conversion to SRC peripheral's Input Data
 *            Register. The number of PCM data samples to write and the buffer memory address are specified by
 *            the arguments.\n
 *            It does following processes.\n
 *            \li Checks if SRC peripheral is locked. If not, it returns SRC_ERR_UNLOCK.
 *            \li Checks if no flush process on-going. If the process is on-going, it returns SRC_NOT_END.
 *            Or already completed, it returns SRC_END.
 *            \li Checks the legality of parameters, and if they are illegal it returns SRC_ERR_PARAM.
 *            \li Writes the PCM data on Input Data Register corresponding to parameters \e buf and \e samples.\n
 *                Note that there is a limit to writing, so when the input data FIFO is full, it stops writing and
 *                returns the number of data written successfully.
 *
 *            Note that R_SRC_Write() and R_SRC_Read() should be called repeatedly during the sampling rate conversion.
 *            And the repeat times to call the functions are different between them.
 * @note      None.
 */
int8_t R_SRC_Write( uint16_t *buf, uint32_t samples )
{
    int8_t   ret_write = 0;
    uint32_t *buffer;

   /* Casting the pointer to a uint32_t type is valid because even
     though the Renesas Compiler uses 4 bytes per pointer, this
     pointer will never refer memory over 0xFFFFFFFF. */
    buffer = (uint32_t *)buf;

    /* Check hardware lock */
    if ( true == R_BSP_HardwareLock(BSP_LOCK_SRC) ) 
    {
        /* case : hardware unlocked. */
        R_BSP_HardwareUnlock(BSP_LOCK_SRC);  /* Release hardware lock */
        ret_write = (int8_t)SRC_ERR_UNLOCK;  /* return */
    }
    /* Check arguments if the value are adequate or not. */
        /* Casting FIT_NO_PTR to (uint_32 *) is valid, 
           because "buffer" is capable (uint_32 *) type. */
    else if ( (uint32_t *)FIT_NO_PTR == buffer )
    {
        /* Casting SRC_ERR_PARAM to (int_8 *) is valid, 
           because enumerator "src_ret_t" ranges 0..-5. */
        ret_write = (int8_t)SRC_ERR_PARAM;
    }
    /* Check SRCSTAT if flush operation is on-going or not. */
    else if (1 == SRC.SRCSTAT.BIT.FLF)
    {
        /* Casting SRC_ERR_PARAM to (int_8 *) is valid, 
           because enumerator "src_ret_t" ranges 0..-5. */
        ret_write = (int8_t)SRC_NOT_END;
    }
    /* Check SRCSTAT if flush operation is finished. */
    else if (1 == SRC.SRCSTAT.BIT.CEF )
    {
        /* Casting SRC_ERR_PARAM to (int_8 *) is valid, 
           because enumerator "src_ret_t" ranges 0..-5. */
        ret_write = (int8_t)SRC_END;
    }
    /* Check SRCSTAT if IINT shows FIFO empty or not. */
    else if (1 == SRC.SRCSTAT.BIT.IINT)
    {
        /* PCM data write operation */
        samples = R_BSP_MIN ((SRC_INFIFO_SIZE - SRC.SRCSTAT.BIT.IFDN), samples);     
        /* WAIT_LOOP */
        for ( ret_write = 0; ret_write < samples; ret_write++ )
        {
            SRC.SRCID.LONG = *buffer++;
        }

        SRC.SRCSTAT.BIT.IINT = 0;
    }
    else
    {
        /* Do nothing */
    }

    return ( ret_write );
}

/**********************************************************************************************************************
 * Function Name: R_SRC_Read
 *****************************************************************************************************************/ /**
 * @brief Read the PCM data after sampling rate conversion from SRC peripheral.
 * @param[in,out] buf
 *             It specifies the starting address of PCM data buffer to store PCM data read from Output Data
 *             Register of SRC peripheral. See Section 3 in the application note, The PCM data is aligned
 *             in the buffer as the figure in API function "R_SRC_Read".
 *             And the one sample is defined as a pair of 16 bit PCM data.
 * @param[in] samples
 *             It specifies the number of the PCM data samples to read from Output Data Register of SRC peripheral.
 * @return    <b>The number of read samples:</b> Shows the number of PCM data samples read from Output Data Register.\n
 *            <b>SRC_PARAM:</b> Parameter is Illegal.\n
 *            <b>SRC_ERR_UNLOCK:</b> SRC peripheral is not locked yet.\n
 *            <b>SRC_END:</b> Flush processing is completed.
 * @details   Call this function to read the PCM data after sampling rate conversion from SRC peripheral's
 *            Output Data Register. The number of PCM data samples to read and the buffer memory address are
 *            specified by the arguments.\n
 *            It does following processes.\n
 *            \li Checks if SRC peripheral is locked. If not, it returns SRC_ERR_UNLOCK.
 *            \li Checks the legality of parameters, and if they are illegal it returns SRC_ERR_PARAM.
 *            \li Checks if flush process on-going. If the process is already completed, it returns SRC_END.
 *            \li Reads the PCM data from Output Data Register corresponding to parameters \e buf and \e samples.\n
 *                Note that there is a limit to reading, so when the output data FIFO is empty,
 *                it stops reading and returns the number of data successfully read.
 *
 *            Note that R_SRC_Read() and R_SRC_Write() should be called repeatedly during the sampling rate conversion.
 *            And the repeat times to call the functions are different between them.
 * @note      None.
 */
int32_t R_SRC_Read( uint16_t * buf, uint32_t samples)
{
    int32_t num_read = 0;
    uint32_t *buffer;

/* Casting the pointer to a uint32_t type is valid because even
   though the Renesas Compiler uses 4 bytes per pointer, this
   pointer will never reference memory over 0xFFFFFFFF. */
    buffer = (uint32_t *)buf;

    /* Check hardware lock */
    if ( true == R_BSP_HardwareLock(BSP_LOCK_SRC) )
    {
        /* case : hardware unlocked. */
        R_BSP_HardwareUnlock(BSP_LOCK_SRC);  /* Release hardware lock */
        return ( SRC_ERR_UNLOCK );
    }
    else
    {
        /* Do nothing */
    }

    /* case : hardware locked. */
      /* Check arguments if the value are adequate or not. */
      /* Casting FIT_NO_PTR to (uint_32 *) is valid, 
          because "buffer" is capable (uint_32 *) type. */
    if ( (uint32_t *)FIT_NO_PTR == buffer )
    {
        return ( SRC_ERR_PARAM );
    }
    else
    {
        /* Do nothing */
    }

    /* Check SRCSTAT if flush operation is on-going or not. */
    if ( 1 == SRC.SRCSTAT.BIT.CEF )
    {
        return ( SRC_END );
    }
    else
    {
        /* Do nothing */
    }

    /* Output PCM data read operation */
    /* WAIT_LOOP */
    while(1)
    {
        if ( 0 == SRC.SRCSTAT.BIT.OFDN )
        {
            return ( num_read );
        }
        else
        {
            /* Do nothing */
        }

        if ( 0 == samples )
        {
            return ( num_read );
        }
        else
        {
            /* Do nothing */
        }

        *buffer = SRC.SRCOD.LONG;
        buffer++;
        samples--;
        num_read++;
    }
}

/**********************************************************************************************************************
 * Function Name: R_SRC_CheckFlush
 *****************************************************************************************************************/ /**
 * @brief Checks if flush processing is completed or not.
 * @retval    SRC_ERR_UNLOCK SRC peripheral is not locked yet.
 * @retval    SRC_NOT_END    Flush processing is not completed.
 * @retval    SRC_END        Flush processing is completed.
 * @details   Call this function to check if flush processing is completed. This function is used in case of that
 *            DMACs are used instead of R_SRC_Write() and R_SRC_Read() for PCM data transfer to/from SRC peripheral.\n
 *            It does following processes.\n
 *            \li Checks if SRC peripheral is locked. If not, it returns SRC_ERR_UNLOCK.
 *            \li Checks if flush process on-going. If the process is on-going, it returns SRC_NOT_END.
 *                Or already completed, it returns SRC_END.
 *
 * @note      None.
 */
src_ret_t R_SRC_CheckFlush( void )
{
    src_ret_t ret = SRC_SUCCESS;

    /* Check hardware lock */
    if ( true == R_BSP_HardwareLock(BSP_LOCK_SRC) )
    {
        /* case : hardware unlocked. */
        R_BSP_HardwareUnlock(BSP_LOCK_SRC);  /* Release hardware lock */
        ret = SRC_ERR_UNLOCK;
    } 
    else
    { 
        /* case : hardware locked. */
        /* Check SRCSTAT if flush operation is on-going or not. */
        if ( 1 == SRC.SRCSTAT.BIT.FLF )
        {
            ret = SRC_NOT_END;
        }
        else
        {
            /* Do nothing */
        }

        if ( 1 == SRC.SRCSTAT.BIT.CEF )
        {
            ret = SRC_END;
        }
        else
        {
            /* Do nothing */
        }
    }

    return ret; 
}

/**********************************************************************************************************************
 * Function Name: R_SRC_GetVersion
 *****************************************************************************************************************/ /**
 * @brief Returns the module version.
 * @return    Version number with major and minor version digits packed into a single 32-bit value.
 * @details   The function returns the version of this module. The version number is encoded such that
 *            the top two bytes are the major version number and the bottom two bytes are the minor version number.
 * @note      None.
 */
uint32_t R_SRC_GetVersion( void )
{
    uint32_t version_number = 0;
    /* Bring in major version number. */
    version_number = ((uint16_t)SRC_API_VERSION_MAJOR) << 16;
    /* Bring in minor version number. */
    version_number |= (uint16_t)SRC_API_VERSION_MINOR;

    return version_number;
}

