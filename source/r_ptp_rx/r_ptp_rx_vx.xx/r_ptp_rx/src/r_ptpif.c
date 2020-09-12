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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_ptpif.c
* Description  : PTP Interface (PTPEDMAC part) device driver
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 10.07.2014 1.00    FIT spec version
*         : 20.08.2014 1.01    State transition and worst10 setting added
*         : 31.12.2014 1.02    Supported RX71M device and added BMC function
*                              Changed module name (eliminate "_api")
*         : 31.03.2016 1.10    Data structures changed
*         : 11.11.2016 1.12    Corrected the internal operation of getting version functions
*                              Corrected wait operation to information retention of GETINFOR
*                              Corrected the array index deviation of ptp_tmr_isr.
*         : 31.03.2017 1.13    Corrected BC and P2P TC setting.
*                              Modified listening state operations of R_PTP_Init, R_PTP_Start and R_PTP_Stop functions.
*                              Added registering user function to MINT interrupt handler.
*                              Changed MINT interrupt handler operation.
*                              Changed TC&OC combined opeartion to TC only operation.
*                              Added my clockIdentity setting function.
*                              Added domainnumber field updating function.
*                              Added announce message fields updating function.
*                              Added and modified transmission interval setting function.
*                              Added offsetFromMaster updating wait function when device state is P2P slave.
*                              Added PTP port state setting function.
*         : 30.04.2017 1.14    Corrected underflow operation of R_PTP_GetSyncInfo function.
*                              Corrected logMessageInterval out of range judgment.
*		  : 31.07.2019 1.15    Added support for GNUC and ICCRX.
*                              Added "WAIT_LOOP" comments for every loop operation.
*                              Corrected RACT bit setting sequence in the R_PTPIF_Read_ZC2_BufRelease function.
*		  : 31.08.2019 1.16    Supported RX72M device.
*                              Added Bypass setting.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_ptpif_rx_if.h"
#include "r_ptpif_rx_private.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Transfer status flag */
typedef enum 
{
    PTPIF_FLAG_OFF = 0, /* Transfer disable */
	PTPIF_FLAG_ON = 1   /* Transfer enable */
} ptpif_trans_t;

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/**
 * Private global function prototyes
 */
static void _R_PTPIF_InitDescriptors(void);
static void _R_PTPIF_ConfigEthernet(void);

/**
 * Private global variables
 */ 

/* Frame transfer enable or disable flag */
static volatile ptpif_trans_t ptpif_trans_flag;

/* Pointer to the PTPIF read message handler */
static PTPIF_HNDLR ptpif_read_isr = NULL;

/* Pointer to the PTPIF write message handler */
static PTPIF_HNDLR ptpif_write_isr = NULL;

/* Pointer to the PTPIF error handler */
static PTPIF_HNDLR ptpif_err_isr = NULL;

/* Pointer to the receive descriptors */
static descriptor_p *app_ptp_rx_desc = NULL;

/* Pointer to the transmit descriptors */
static descriptor_p *app_ptp_tx_desc = NULL;

/* Receive descriptors */
R_BSP_ATTRIB_SECTION_CHANGE(B, _PTPIF_RX_DESC, 1)
static descriptor_p ptp_rx_descriptors[PTPIF_CFG_NUM_RX_DESCRIPTORS];

/* Transmit descriptors */
R_BSP_ATTRIB_SECTION_CHANGE(B, _PTPIF_TX_DESC, 1)
static descriptor_p ptp_tx_descriptors[PTPIF_CFG_NUM_TX_DESCRIPTORS];

/* Transmit and receive buffers */
R_BSP_ATTRIB_SECTION_CHANGE(B, _PTPIF_BUFFER, 1)
static etherbuffer_p ptpif_buffer;

R_BSP_ATTRIB_SECTION_CHANGE_END

/**
 * Renesas Ethernet API functions
 */
/***********************************************************************************************************************
* Function Name: R_PTPIF_GetVersion
* Description  : Get PTP Host interface driver part version number.
* Arguments    : None
* Return Value : PTP Host interface module version number
* Note         : This function is expanded as inline function.
***********************************************************************************************************************/
uint32_t R_PTPIF_GetVersion(void)
{
    uint32_t version;

    /* Major version number (upper 16bit) */
    version = ((uint16_t)RX_PTPIF_VERSION_MAJOR << 16u);
    
    /* Minor version number (lower 16bit) */
    version |= (uint16_t)RX_PTPIF_VERSION_MINOR;

    return version;
} /* End of function R_PTPIF_GetVersion() */

/***********************************************************************************************************************
* Function Name: R_PTPIF_Reset
* Description  : Reset PTPEDMAC.  
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PTPIF_Reset(void)
{
	/* Enable PTPEDMAC */
	ptpif_dev_enab();
	
	/* reset PTPEDMAC */
	PTPEDMAC.EDMR.BIT.SWR = 1;

    /* wait more than 64 PCLKA cycles */
	/* WAIT_LOOP */
	R_BSP_SoftwareDelay(1, BSP_DELAY_MICROSECS);
	
} /* End of function R_PTPIF_Reset() */

/***********************************************************************************************************************
* Function Name: R_PTPIF_Init
* Description  : Initialize resources of the PTP Host interface driver.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PTPIF_Init(void)
{
    /* Initialize the transmit and receive descriptors */
    memset(&ptp_rx_descriptors, 0x00, sizeof(ptp_rx_descriptors));
    memset(&ptp_tx_descriptors, 0x00, sizeof(ptp_tx_descriptors));
	
	/* Initialize the PTPEDMAC buffer */
    memset(&ptpif_buffer,  0x00, sizeof(ptpif_buffer));
	
} /* End of function R_PTPIF_Init() */

/***********************************************************************************************************************
* Function Name: R_PTPIF_Open_ZC2
* Description  : Initialize PTP Host interface and peripheral modules. 
* Arguments    : None
* Return Value : PTPIF_OK  - Processing completed successfully
*                PTPIF_ERR - Not implemented this version
* Note         : Execute this function only after the ETHERC and standard EDMAC were opened. 
***********************************************************************************************************************/
ptpif_return_t R_PTPIF_Open_ZC2(void)
{
    ptpif_return_t ret;

    /* Initialize the flags */
    ptpif_trans_flag = PTPIF_FLAG_OFF;
    
    /* Clear all PTPEDMAC status flags */
    PTPEDMAC.EESR.LONG  = 0x45BF019F;
	
    /* Set PTPEDMAC interrupt */
    ptpif_dev_start();
	
	ret = PTPIF_OK;

    return ret;
} /* End of function R_PTPIF_Open_ZC2() */

/***********************************************************************************************************************
* Function Name: R_PTPIF_LinkProcess
* Description  : Set PTP Host interface to transfer PTP message.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PTPIF_LinkProcess(void)
{
    /* Initialize the receive and transmit descriptors */
	_R_PTPIF_InitDescriptors();
	
	/* Initializes PTPEDMAC */
    _R_PTPIF_ConfigEthernet();
	
	/* Set PTPEDMAC receive operation to enable */ 
	PTPEDMAC.EDRRR.LONG = 0x00000001;
	
    /* Set PTP Host interface transfer flag to enable */
	ptpif_trans_flag = PTPIF_FLAG_ON;

} /* End of function R_PTPIF_LinkProcess() */

/***********************************************************************************************************************
* Function Name: R_PTPIF_CheckLink_ZC
* Description  : Check PTP Host interface communication status.
* Arguments    : None
* Return Value : PTPIF_OK  - Transfer enable
*                PTPIF_ERR - Transfer disable
***********************************************************************************************************************/
ptpif_return_t R_PTPIF_CheckLink_ZC(void)
{
    if (PTPIF_FLAG_ON == ptpif_trans_flag)
    { /* transfer enable */
        return PTPIF_OK;
    }
    else
    { /* transfer disable */
        return PTPIF_ERR;
    }
} /* End of function R_PTPIF_CheckLink_ZC() */

/***********************************************************************************************************************
* Function Name: R_PTPIF_Close_ZC2
* Description  : Disable PTP Host interface peripheral modules
* Arguments    : None
* Return Value : PTPIF_OK  - Processing completed successfully
*                PTPIF_ERR - Already closed
***********************************************************************************************************************/
ptpif_return_t R_PTPIF_Close_ZC2(void)
{
	if (PTPIF_FLAG_OFF == ptpif_trans_flag)
	{ /* Already closed */
		return PTPIF_ERR;
	}

	/* Clear PTPEDMAC interrupt */
    ptpif_dev_stop();

	/* Set PTP Host interface transfer flag to disable */
    ptpif_trans_flag = PTPIF_FLAG_OFF;

    return PTPIF_OK;
} /* End of function R_PTPIF_Close_ZC2() */

/***********************************************************************************************************************
* Function Name: R_PTPIF_Read
* Description  : Receive PTP message
* Arguments    : ch  - received port channel (0 or 1)
*                buf - read buffer pointer
* Return Value : More than 0       - Number of received data
*                PTPIF_ERR_NO_DATA - No data received
*                PTPIF_ERR         - Any error occurred
* Note         : Standard library <string.h> is used in this function.
*                User's data buffer is had to allocate more than buffer size of PTPEDMAC (=PTPIF_CFG_BUFSIZE). 
*                Otherwise, user's data buffer is overwritten by the PTP driver.
*                This function is prohibited to simultaneous use with R_PTPIF_Read_ZC2 and R_PTPIF_Read_ZC2_BufRelease.
***********************************************************************************************************************/
int32_t R_PTPIF_Read(uint32_t *ch, void *buf)
{
	int32_t ret;
	ptpif_return_t ptpif_ret;
	uint8_t* r_buf;

	if (NULL == buf) 
	{
		return PTPIF_ERR;
	}
	
    /* Set the allocated buffer pointer for received data */
	ret = R_PTPIF_Read_ZC2(ch, (void **)&r_buf);
	if (0 < ret)
	{
	    memcpy((uint8_t*)buf, r_buf, ret);
		
		/* Release the receive buffer */
	    ptpif_ret = R_PTPIF_Read_ZC2_BufRelease();
		if (PTPIF_OK != ptpif_ret)
        {
			ret = (int32_t)ptpif_ret;
		}
        
		return ret;
	}
	else
	{
		return PTPIF_ERR_NO_DATA; /* No data */
	}
} /* End of R_PTPIF_Read() */

/***********************************************************************************************************************
* Function Name: R_PTPIF_Write
* Description  : Transmit PTP message or standard Ethernet frame.
* Arguments    : buf  - write buffer pointer
*                size - write data size
* Return Value : PTPIF_OK      - Processing completed successfully
*                PTPIF_ERR_LEN - Data length error
*                PTPIF_ERR     - Any error occured but for "PTPIF_ERR_LEN"
* Note         : This function is used not only PTP message but also standard Ethernet frame transmission.
*                Standard library <string.h> is used in this function.
*                This function is prohibited to simultaneous use with R_PTPIF_Write_ZC2GetBuf 
*                and R_PTPIF_Write_ZC2_SetBuf. 
***********************************************************************************************************************/
ptpif_return_t R_PTPIF_Write(uint8_t* buf, uint32_t size)
{
	ptpif_return_t ret;
	uint8_t* w_buf;
	uint16_t w_size;

	if ((NULL == buf) || (0 == size)) 
	{
		return PTPIF_ERR;
	}

	if (PTPIF_CFG_BUFSIZE <= size)
	{
		return PTPIF_ERR_LEN;
	}
	
	ret = R_PTPIF_Write_ZC2_GetBuf((void**)&w_buf, &w_size);
	if (PTPIF_OK == ret)
	{
	    memcpy(w_buf, (uint8_t*)buf, size);
		ret = R_PTPIF_Write_ZC2_SetBuf(size);
	}
	else
	{
		ret = PTPIF_ERR;
	}
    
	return ret;
} /* End of R_PTPIF_Write() */

/***********************************************************************************************************************
* Function Name: R_PTPIF_Read_ZC2
* Description  : Receive one PTP message frame or fragment.
*                Set the allocated buffer pointer for received data.
* Arguments    : ch  - received port channel (0 or 1)
*                buf - received data store buffer
* Return Value : More than 0        - Number of received data
*                PTPIF_ERR_NO_DATA  - No data received
*                PTPIF_ERR_NOT_TRAN - Not transfer enabled
* Note         : This function is prohibited to simultaneous use with R_PTPIF_Read.
***********************************************************************************************************************/
int32_t R_PTPIF_Read_ZC2(uint32_t *ch, void **buf)
{
    int32_t num_recvd;
    int32_t ret;
	
    /* Check Link status */
    if (PTPIF_FLAG_OFF == ptpif_trans_flag)
    {
        ret = PTPIF_ERR_NOT_TRAN;
    }
    
    if (RACT != (app_ptp_rx_desc->status & RACT))
    { /* Received data exists */
		/* Get received port channel */
		*ch = ((app_ptp_rx_desc->status & PORT) >> 7u);
		
		/* Set the allocated buffer pointer for received data */
        *buf = (void *)app_ptp_rx_desc->buf_p;
		
        /* Get received data length */
        num_recvd = app_ptp_rx_desc->size;
        ret = num_recvd;
    }
    else
    { /* No received data */
    	ret = PTPIF_ERR_NO_DATA;
    }
    
    return ret;
} /* End of function R_PTPIF_Read_ZC2() */

/***********************************************************************************************************************
* Function Name: R_PTPIF_Read_ZC2_BufRelease
* Description  : Release the receive buffer.
* Arguments    : None
* Return Value : PTPIF_OK           - Processing completed successfully
*                PTPIF_ERR_NOT_TRAN - Not transfer enabled
* Note         : This function is prohibited to simultaneous use with R_PTPIF_Read.
***********************************************************************************************************************/
ptpif_return_t R_PTPIF_Read_ZC2_BufRelease(void)
{
    if (PTPIF_FLAG_OFF == ptpif_trans_flag)
    {
        return PTPIF_ERR_NOT_TRAN;
    }
	
    if (RACT != (app_ptp_rx_desc->status & RACT))
    { /* Received data exists */
        /* Move current descriptor to next one */
        app_ptp_rx_desc->status &= ~(RFP1 | RFP0 | RFE | RFOF | PORT | PVER | TYPE3 | TYPE2 | TYPE1 | TYPE0);
		app_ptp_rx_desc->status |= RACT;
        app_ptp_rx_desc = app_ptp_rx_desc->next;
    }

	if (0x00000000L == PTPEDMAC.EDRRR.LONG)
	{ /* If receive operation of the PTPEDMAC was disabled, make PTPEDMAC to receive enable */
    	PTPEDMAC.EDRRR.LONG = 0x00000001L;
	}
	
	return PTPIF_OK;
} /* End of function R_PTPIF_Read_ZC2_BufRelease() */

/***********************************************************************************************************************
* Function Name: R_PTPIF_Write_ZC2_GetBuf
* Description  : Set the buffer pointer and allocate the buffer by transmit data size.
* Arguments    : buf - write data store buffer
*                size - allocated buffer size
* Return Value : PTPIF_OK           - Processing completed successfully
*                PTPIF_ERR_TACT     - No remaining transmit descriptor
*                PTPIF_ERR_NOT_TRAN - Not transfer enabled
* Note         : This function is used not only PTP message but also standard Ethernet frame transmission.
*                This function is prohibited to simultaneous use with R_PTPIF_Write.
***********************************************************************************************************************/
ptpif_return_t R_PTPIF_Write_ZC2_GetBuf(void **buf, uint16_t *size)
{
    ptpif_return_t ret;

    if (PTPIF_FLAG_OFF == ptpif_trans_flag)
    {
        ret = PTPIF_ERR_NOT_TRAN;
    }
	
    if (TACT == (app_ptp_tx_desc->status & TACT))
    { /* No remaining transmit descriptor */
        ret = PTPIF_ERR_TACT;
    }
    else
    { /* Remaining transmit descriptor existed */
        /* Set the buffer pointer and allocate the buffer by transmit data size */
        *buf = app_ptp_tx_desc->buf_p;
        *size = PTPIF_CFG_BUFSIZE;
        ret = PTPIF_OK;
    }
	
    return ret;
} /* End of function R_PTPIF_Write_ZC2_GetBuf() */

/***********************************************************************************************************************
* Function Name: R_EPTPIF_Write_ZC2_SetBuf
* Description  : Transmit one PTP message or one Ethernet frame or fragment. 
*                Set the descriptor for the transmit data.
* Arguments    : len - transmit data length
* Return Value : PTPIF_OK           - Processing completed successfully
*                PTPIF_ERR_NOT_TRAN - Not transfer enabled
*                PTPIF_ERR_LEN      - Data length error
* Note         : This function is used not only PTP message but also standard Ethernet frame transmission.
*              : This function is prohibited to simultaneous use with R_PTPIF_Write.
***********************************************************************************************************************/
ptpif_return_t R_PTPIF_Write_ZC2_SetBuf(uint32_t len)
{
    ptpif_return_t ret = PTPIF_OK;

    if (PTPIF_FLAG_OFF == ptpif_trans_flag)
    {
        return PTPIF_ERR_NOT_TRAN;
    }

	if (PTPIF_CFG_BUFSIZE < len)
	{
		return PTPIF_ERR_LEN;
	}
	
    /* Set the descriptor for the transmit data */
    app_ptp_tx_desc->bufsize = len;
    app_ptp_tx_desc->status &= ~(TFP1 | TFP0);
    app_ptp_tx_desc->status |= (TFP1 | TFP0 | TACT);
    app_ptp_tx_desc = app_ptp_tx_desc->next;
    
    if (0x00000000L == PTPEDMAC.EDTRR.LONG)
    { /* Restart, if transmit operation was disabled */
        PTPEDMAC.EDTRR.LONG = 0x00000001L;
    }

	return ret;
} /* End of function R_PTPIF_Write_ZC2_SetBuf() */

#if (PTPIF_CFG_MODE & PTPIF_MODE_HWINT)
/***********************************************************************************************************************
* Function Name: R_PTPIF_RegMsgHndr
* Description  : Register a user function to the interrupt handler of PTPEDMAC.
* Arguments    : event - PTPEDMAC interrupt event
*                    PTPIF_FUNC_READ: Frame reception interrupt (FR) (EMACP_FR_INT = 0x0004000)
*                    PTPIF_FUNC_WRITE: Frame transmission interrupt (TC) (EMACP_TC_INT = 0x0020000)
*                    PTPIF_FUNC_ERR: Error interrupt (MACE, RFOF, RDE, TFUF, TDE, ADE and RFCOF)
*                func - register function
* Return Value : PTPIF_OK        - Processing completed successfully
*                PTPIF_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptpif_return_t R_PTPIF_RegMsgHndr(PTPIF_INTEVT event, PTPIF_HNDLR func)
{
    ptpif_return_t ret = PTPIF_OK;

	if (PTPIF_FUNC_READ == event)
    { /* register message read function handler */
	    ptpif_read_isr = func;
    }
    else if (PTPIF_FUNC_WRITE == event)
    { /* register message write function handler */
	    ptpif_write_isr = func;
    }
	else if (PTPIF_FUNC_ERR == event)
    { /* register errors function handler */
	    ptpif_err_isr = func;
    }
    else
    { /* add error interrupt */
		ret = PTPIF_ERR;
    }

	return ret;
} /* End of R_PTPIF_RegMsgHndr() */
#endif

/***********************************************************************************************************************
* Function Name: Ptpedmac_isr
* Description  : Interrupt handler of PTPEDMAC.
* Arguments    : None
* Return Value : None
* Note         : This function defines read message, write message and error handler.  
***********************************************************************************************************************/
void Ptpedmac_isr(void)
{
    uint32_t status_eesr = PTPEDMAC.EESR.LONG;

    /* Judge the interrupt element */
	if (status_eesr & (EMACP_MACE_INT | EMACP_RFOF_INT | EMACP_RDE_INT | EMACP_TFUF_INT | EMACP_TDE_INT 
		| EMACP_ADE_INT | EMACP_RFCOF_INT))
    { /* MAC address, Receive buffer overflow, Lack of receiving descriptor error, Transmit buffer overflow
		, Lack of transmiting descriptor, Address or The number of received frame overflow error */
		if (ptpif_err_isr != NULL) 
		{
		    ptpif_err_isr(0, status_eesr);
        }
	}
    if (status_eesr & EMACP_FR_INT)
    { /* Frame receive complete */
        if (ptpif_read_isr != NULL) 
		{
		    ptpif_read_isr((uint8_t)((status_eesr >> 7u) & 0x1), (status_eesr & 0xF));
        }
	}
    if (status_eesr & EMACP_TC_INT)
    { /* Frame transmit complete */
		if (ptpif_write_isr != NULL) 
		{
		    ptpif_write_isr(0, status_eesr);
        }
	}
	
    /* Clear PTPEDMAC status bits. 1'b write cleared */
    PTPEDMAC.EESR.LONG  = status_eesr;

} /* End of function Ptpedmac_isr() */

/***********************************************************************************************************************
* Function Name: _R_PTPIF_InitDescriptors
* Description  : Initialize PTPEDMAC descriptors and the driver buffers.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void _R_PTPIF_InitDescriptors(void)
{
    descriptor_p *descriptor;
    uint32_t i;

    /* Initialize the receive descriptors */
    for(i = 0; i < PTPIF_CFG_NUM_RX_DESCRIPTORS; i++)
    {
        descriptor = &(ptp_rx_descriptors[i]); 
        descriptor->buf_p = &(ptpif_buffer.buffer[i][0]);
        descriptor->bufsize = PTPIF_CFG_BUFSIZE;
        descriptor->size = 0;
        descriptor->status = RACT;
        descriptor->next = &(ptp_rx_descriptors[i + 1]);
    }

    /* Boundary condition for receive descriptors allocation */
    descriptor->status |= RDLE;
    descriptor->next = &(ptp_rx_descriptors[0]);
	
    /* Initialize receive descriptors pointer to which application allocated */
    app_ptp_rx_desc  = &(ptp_rx_descriptors[0]);

    /* Initialize the transmit descriptors */
    for (i = 0; i < PTPIF_CFG_NUM_TX_DESCRIPTORS; i++)
    {
        descriptor = &(ptp_tx_descriptors[i]);
        descriptor->buf_p = &(ptpif_buffer.buffer[PTPIF_CFG_NUM_RX_DESCRIPTORS + i][0]);
        descriptor->bufsize = PTPIF_CFG_BUFSIZE;
        descriptor->size = 0;
        descriptor->status = 0;
        descriptor->next = &(ptp_tx_descriptors[i + 1]);
    }

    /* Boundary condition for transmit descriptors allocation */
    descriptor->status |= TDLE;
    descriptor->next = &(ptp_tx_descriptors[0]);

    /* Initialize transmit descriptors pointer to which application allocated */
    app_ptp_tx_desc  = &(ptp_tx_descriptors[0]);
} /* End of function _R_PTPIF_InitDescriptors() */

/***********************************************************************************************************************
* Function Name: _R_PTPIF_ConfigEthernet
* Description  : Configure PTP Direct Memory Access controller (PTPEDMAC).
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void _R_PTPIF_ConfigEthernet(void)
{	
    /* enable error interrupt */
    /* MACE, RFOF, RDE, TFUF, TDE, ADE and RFCOF */
    PTPEDMAC.EESIPR.LONG = 0x019B0100;

	/* enable frame receved interrupt (FR) */
    PTPEDMAC.EESIPR.BIT.FRIP = 1;
    
	/* enable frame transmit complete interrupt (TC) */
    PTPEDMAC.EESIPR.BIT.TCIP = 1;

#ifdef __LIT
    /* Set little endian mode */
    PTPEDMAC.EDMR.BIT.DE = 1;
#endif

    /* Initialize receive descriptor list address */
    /* Casting the pointer to a uint32_t type is valid because the Renesas Compiler uses 4 bytes per pointer. */
    PTPEDMAC.RDLAR = (void*)app_ptp_rx_desc;
    
    /* Initialize transmit descriptor list address */
    /* Casting the pointer to a uint32_t type is valid because the Renesas Compiler uses 4 bytes per pointer. */
    PTPEDMAC.TDLAR = (void*)app_ptp_tx_desc;
    
    /* Threshold of transmit FIFO */
    /* To prevent a transmit underflow, setting the initial value (store and forward modes) is recommended. */
    PTPEDMAC.TFTR.LONG = 0x00000000;
    
    /* Transmit FIFO is 2048 byte and receive FIFO is 4096 byte */
    PTPEDMAC.FDR.LONG  = 0x0000070F;
    
    /*  Configure receiving method
    b0      RNR - Receive Request Bit Reset - Continuous reception of multiple frames is possible.
    b31:b1  Reserved set to 0
    */
    PTPEDMAC.RMCR.LONG  = 0x00000001;
	
} /* End of function _R_PTPIF_ConfigEthernet() */

/* End of File */
