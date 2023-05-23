/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : cellular_private_api.h
 * Description  : Configures the driver.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "r_cellular_if.h"

#ifndef CELLULAR_PRIVATE_API_H
#define CELLULAR_PRIVATE_API_H

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#ifndef CELLULAR_CFG_UART_SCI_CH
#error "Error! Need to define CELLULAR_CFG_UART_SCI_CH in cellular_config.h"
#elif CELLULAR_CFG_UART_SCI_CH == (0)
#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
/* Store R_SCI_PinSet_SCI0() in R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()      (R_SCI_PinSet_SCI0())
#elif defined(__CCRL__) || defined(__ICCRL78__) || defined(__RL)
/*  */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()
#endif
#define R_SCI_CFG_CELLULAR_SERIAL_CH            (SCI_CH0)
#define R_SCI_CFG_TX_BUFSIZE                    (SCI_CFG_CH0_TX_BUFSIZ)
#define R_SCI_CFG_RX_BUFSIZE                    (SCI_CFG_CH0_RX_BUFSIZ)
#elif CELLULAR_CFG_UART_SCI_CH == (1)
#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
/* Store R_SCI_PinSet_SCI1() in R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()      (R_SCI_PinSet_SCI1())
#elif defined(__CCRL__) || defined(__ICCRL78__) || defined(__RL)
/* Emptying R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()
#endif
#define R_SCI_CFG_CELLULAR_SERIAL_CH            (SCI_CH1)
#define R_SCI_CFG_TX_BUFSIZE                    (SCI_CFG_CH1_TX_BUFSIZ)
#define R_SCI_CFG_RX_BUFSIZE                    (SCI_CFG_CH1_RX_BUFSIZ)
#elif CELLULAR_CFG_UART_SCI_CH == (2)
#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
/* Store R_SCI_PinSet_SCI2() in R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()      (R_SCI_PinSet_SCI2())
#elif defined(__CCRL__) || defined(__ICCRL78__) || defined(__RL)
/* Emptying R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()
#endif
#define R_SCI_CFG_CELLULAR_SERIAL_CH            (SCI_CH2)
#define R_SCI_CFG_TX_BUFSIZE                    (SCI_CFG_CH2_TX_BUFSIZ)
#define R_SCI_CFG_RX_BUFSIZE                    (SCI_CFG_CH2_RX_BUFSIZ)
#elif CELLULAR_CFG_UART_SCI_CH == (3)
#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
/* Store R_SCI_PinSet_SCI3() in R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()      (R_SCI_PinSet_SCI3())
#elif defined(__CCRL__) || defined(__ICCRL78__) || defined(__RL)
/* Emptying R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()
#endif
#define R_SCI_CFG_CELLULAR_SERIAL_CH            (SCI_CH3)
#define R_SCI_CFG_TX_BUFSIZE                    (SCI_CFG_CH3_TX_BUFSIZ)
#define R_SCI_CFG_RX_BUFSIZE                    (SCI_CFG_CH3_RX_BUFSIZ)
#elif CELLULAR_CFG_UART_SCI_CH == (4)
/* Store R_SCI_PinSet_SCI4() in R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()      (R_SCI_PinSet_SCI4())
#define R_SCI_CFG_CELLULAR_SERIAL_CH            (SCI_CH4)
#define R_SCI_CFG_TX_BUFSIZE                    (SCI_CFG_CH4_TX_BUFSIZ)
#define R_SCI_CFG_RX_BUFSIZE                    (SCI_CFG_CH4_RX_BUFSIZ)
#elif CELLULAR_CFG_UART_SCI_CH == (5)
/* Store R_SCI_PinSet_SCI5() in R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()      (R_SCI_PinSet_SCI5())
#define R_SCI_CFG_CELLULAR_SERIAL_CH            (SCI_CH5)
#define R_SCI_CFG_TX_BUFSIZE                    (SCI_CFG_CH5_TX_BUFSIZ)
#define R_SCI_CFG_RX_BUFSIZE                    (SCI_CFG_CH5_RX_BUFSIZ)
#elif CELLULAR_CFG_UART_SCI_CH == (6)
/* Store R_SCI_PinSet_SCI6() in R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()      (R_SCI_PinSet_SCI6())
#define R_SCI_CFG_CELLULAR_SERIAL_CH            (SCI_CH6)
#define R_SCI_CFG_TX_BUFSIZE                    (SCI_CFG_CH6_TX_BUFSIZ)
#define R_SCI_CFG_RX_BUFSIZE                    (SCI_CFG_CH6_RX_BUFSIZ)
#elif CELLULAR_CFG_UART_SCI_CH == (7)
/* Store R_SCI_PinSet_SCI7() in R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()      (R_SCI_PinSet_SCI7())
#define R_SCI_CFG_CELLULAR_SERIAL_CH            (SCI_CH7)
#define R_SCI_CFG_TX_BUFSIZE                    (SCI_CFG_CH7_TX_BUFSIZ)
#define R_SCI_CFG_RX_BUFSIZE                    (SCI_CFG_CH7_RX_BUFSIZ)
#elif CELLULAR_CFG_UART_SCI_CH == (8)
/* Store R_SCI_PinSet_SCI8() in R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()      (R_SCI_PinSet_SCI8())
#define R_SCI_CFG_CELLULAR_SERIAL_CH            (SCI_CH8)
#define R_SCI_CFG_TX_BUFSIZE                    (SCI_CFG_CH8_TX_BUFSIZ)
#define R_SCI_CFG_RX_BUFSIZE                    (SCI_CFG_CH8_RX_BUFSIZ)
#elif CELLULAR_CFG_UART_SCI_CH == (9)
/* Store R_SCI_PinSet_SCI9() in R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()      (R_SCI_PinSet_SCI9())
#define R_SCI_CFG_CELLULAR_SERIAL_CH            (SCI_CH9)
#define R_SCI_CFG_TX_BUFSIZE                    (SCI_CFG_CH9_TX_BUFSIZ)
#define R_SCI_CFG_RX_BUFSIZE                    (SCI_CFG_CH9_RX_BUFSIZ)
#elif CELLULAR_CFG_UART_SCI_CH == (10)
/* Store R_SCI_PinSet_SCI10() in R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()      (R_SCI_PinSet_SCI10())
#define R_SCI_CFG_CELLULAR_SERIAL_CH            (SCI_CH10)
#define R_SCI_CFG_TX_BUFSIZE                    (SCI_CFG_CH10_TX_BUFSIZ)
#define R_SCI_CFG_RX_BUFSIZE                    (SCI_CFG_CH10_RX_BUFSIZ)
#elif CELLULAR_CFG_UART_SCI_CH == (11)
/* Store R_SCI_PinSet_SCI11() in R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()      (R_SCI_PinSet_SCI11())
#define R_SCI_CFG_CELLULAR_SERIAL_CH            (SCI_CH11)
#define R_SCI_CFG_TX_BUFSIZE                    (SCI_CFG_CH11_TX_BUFSIZ)
#define R_SCI_CFG_RX_BUFSIZE                    (SCI_CFG_CH11_RX_BUFSIZ)
#elif CELLULAR_CFG_UART_SCI_CH == (12)
/* Store R_SCI_PinSet_SCI12() in R_SCI_CFG_PINSET_CELLULAR_SERIAL() */
#define R_SCI_CFG_PINSET_CELLULAR_SERIAL()      (R_SCI_PinSet_SCI12())
#define R_SCI_CFG_CELLULAR_SERIAL_CH            (SCI_CH12)
#define R_SCI_CFG_TX_BUFSIZE                    (SCI_CFG_CH12_TX_BUFSIZ)
#define R_SCI_CFG_RX_BUFSIZE                    (SCI_CFG_CH12_RX_BUFSIZ)
#else
#error "Error! Invalid setting for CELLULAR_UART_SCI_CH in cellular_config.h"
#endif /* CELLULAR_CFG_UART_SCI_CH */

#if R_SCI_CFG_TX_BUFSIZE < (2048)
#error "The SCI send buffer size is too small."
#endif
#if R_SCI_CFG_RX_BUFSIZE < (2048)
#error "The SCI receive buffer size is too small."
#endif
#if BSP_CFG_RTOS_USED == (1)
#if CELLULAR_CFG_SCI_PRIORITY > configMAX_SYSCALL_INTERRUPT_PRIORITY
#error "SCI interrupt priority is outside the control of the FreeRTOS."
#endif
#endif

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/
extern st_cellular_ctrl_t * gp_cellular_ctrl;

/*****************************************************************************
 * Private Functions
 *****************************************************************************/
/****************************************************************************
 * Function Name  @fn            cellular_power_down
 * Description    @details       Stop the power supply to the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully initialized the serial communication function.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***************************************************************************/
e_cellular_err_t cellular_power_down (st_cellular_ctrl_t * const p_ctrl);

/**********************************************************************************************
 * Function Name  @fn            cellular_serial_open
 * Description    @details       Initializing the serial communication function.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully initialized the serial communication function.
 *                @retval        CELLULAR_ERR_SERIAL_OPEN -
 *                                  Serial initialization failed.
 *********************************************************************************************/
e_cellular_err_t cellular_serial_open (st_cellular_ctrl_t * const p_ctrl);

/**********************************************************************************************
 * Function Name  @fn            cellular_serial_close
 * Description    @details       Terminate the serial communication function.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *********************************************************************************************/
void cellular_serial_close (st_cellular_ctrl_t * const p_ctrl);

/****************************************************************************
 * Function Name  @fn            cellular_semaphore_init
 * Description    @details       Initialize the semaphore function.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully semaphore initialization.
 *                @retval        CELLULAR_ERR_SEMAPHORE_INIT -
 *                                  Failed to initialize the semaphore.
 ***************************************************************************/
e_cellular_err_t cellular_semaphore_init (st_cellular_ctrl_t * const p_ctrl);

/****************************************************************************
 * Function Name  @fn            cellular_socket_init
 * Description    @details       Send AT command to the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully Socket initialization.
 *                @retval        CELLULAR_ERR_BYTEQ_OPEN -
 *                                  Failed to initialize the byte queue.
 ***************************************************************************/
e_cellular_err_t cellular_socket_init (st_cellular_ctrl_t * const p_ctrl);

/*******************************************************************************
 * Function Name  @fn            cellular_timeout_init
 * Description    @details       Set the communication timeout with the module.
 * Arguments      @param[in/out] p_timeout_ctrl -
 *                                  Pointer to the timeout management structure.
 *                @param[in]     timeout_ms -
 *                                  Timeout millisecond.
 ******************************************************************************/
void cellular_timeout_init (st_cellular_time_ctrl_t * const p_timeout_ctrl, const uint32_t timeout_ms);

/*******************************************************************************
 * Function Name  @fn            cellular_check_timeout
 * Description    @details       Check the timeout.
 * Arguments      @param[in/out] p_timeout_ctrl -
 *                                  Pointer to the timeout management structure.
 * Return Value   @retval        CELLULAR_NOT_TIMEOUT -
 *                                  No timeout occurred.
 *                @retval        CELLULAR_TIMEOUT; -
 *                                  Timeout occurs.
 ******************************************************************************/
e_cellular_timeout_check_t cellular_check_timeout (st_cellular_time_ctrl_t * const p_timeout_ctrl);

/************************************************************************
 * Function Name  @fn            cellular_shutdownsocket
 * Description    @details       Disconnect the socket connection.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Number to close socket.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully disconnected the socket connection.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t cellular_shutdownsocket (st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no);

/************************************************************************
 * Function Name  @fn            cellular_closesocket
 * Description    @details       Close the socket.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Number to close socket.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully closed the socket.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t cellular_closesocket (st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no);

/*******************************************************************************
 * Function Name  @fn            cellular_disconnect
 * Description    @details       Disconnect from the access point.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully disconnected from access point.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ******************************************************************************/
e_cellular_err_t cellular_disconnect (st_cellular_ctrl_t * const p_ctrl);

/*******************************************************************************
 * Function Name  @fn            cellular_module_reset
 * Description    @details       Reset the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully disconnected from access point.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 ******************************************************************************/
e_cellular_err_t cellular_module_reset (st_cellular_ctrl_t * const p_ctrl);

/**********************************************************************************************
 * Function Name  @fn            cellular_irq_open
 * Description    @details       Initializing the external terminal interrupt function.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully initialized the serial communication function.
 *                @retval        CELLULAR_ERR_IRQ_OPEN -
 *                                  IRQ initialization failed.
 *********************************************************************************************/
e_cellular_err_t cellular_irq_open (st_cellular_ctrl_t * const p_ctrl);

/**********************************************************************************************
 * Function Name  @fn            cellular_psm_config
 * Description    @details       Initializing the external terminal interrupt function.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     mode -
 *                                  PSM mode setting.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully initialized the serial communication function.
 *                @retval        CELLULAR_ERR_IRQ_OPEN -
 *                                  IRQ initialization failed.
 *********************************************************************************************/
e_cellular_err_t cellular_psm_config (st_cellular_ctrl_t * const p_ctrl, const e_cellular_psm_mode_t mode);

/**********************************************************************************************
 * Function Name  @fn            cellular_irq_close
 * Description    @details       Terminate the external terminal interrupt function.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *********************************************************************************************/
void cellular_irq_close (st_cellular_ctrl_t * const p_ctrl);

/*******************************************************************************
 * Function Name  @fn            cellular_rts_ctrl
 * Description    @details       Controls the RTS Pin.
 * Arguments      @param[in]     lowhigh -
 *                                  Control direction.
 ******************************************************************************/
void cellular_rts_ctrl (const uint8_t lowhigh);

/*******************************************************************************
 * Function Name  @fn            cellular_rts_hw_flow_enable
 * Description    @details       Enable hardware flow.
 ******************************************************************************/
void cellular_rts_hw_flow_enable (void);

/*******************************************************************************
 * Function Name  @fn            cellular_rts_hw_flow_disable
 * Description    @details       Disable hardware flow.
 ******************************************************************************/
void cellular_rts_hw_flow_disable (void);

/***********************************************************************************************
 * Function Name  @fn            cellular_recv_task
 * Description    @details       Process incoming data from the module.
 * Arguments      @param[in/out] p_pvParameters -
 *                                  Pointer to the parameter given at the time of task creation.
 **********************************************************************************************/
#if BSP_CFG_RTOS_USED == (1)
void cellular_recv_task (void * p_pvParameters);
#elif BSP_CFG_RTOS_USED == (5)
void cellular_recv_task (ULONG p_pvParameters);
#endif

/**************************************************************************************************************
 * Function Name  @fn            cellular_ring_task
 * Description    @details       Control of the RTS terminal in accordance with the operation of the RING line.
 * Arguments      @param[in/out] p_pvParameters -
 *                                  Pointer to the parameter given at the time of task creation.
 *************************************************************************************************************/
#if BSP_CFG_RTOS_USED == (1)
void cellular_ring_task (void * p_pvParameters);
#elif BSP_CFG_RTOS_USED == (5)
void cellular_ring_task (ULONG p_pvParameters);
#endif

/****************************************************************************
 * Function Name  @fn            cellular_start_recv_task
 * Description    @details       Start receiving task.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully executed AT command.
 *                @retval        CELLULAR_ERR_EVENT_GROUP_INIT -
 *                                  Failed to initialize the event group.
 *                @retval        CELLULAR_ERR_CREATE_TASK -
 *                                  Failed to create task.
 ***************************************************************************/
e_cellular_err_t cellular_start_recv_task (st_cellular_ctrl_t * const p_ctrl);

/****************************************************************************
 * Function Name  @fn            cellular_start_ring_task
 * Description    @details       Start RING Line task.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully executed AT command.
 *                @retval        CELLULAR_ERR_EVENT_GROUP_INIT -
 *                                  Failed to initialize the event group.
 *                @retval        CELLULAR_ERR_CREATE_TASK -
 *                                  Failed to create task.
 ***************************************************************************/
e_cellular_err_t cellular_start_ring_task (st_cellular_ctrl_t * const p_ctrl);

/****************************************************************************
 * Function Name  @fn            cellular_start_monitor_task
 * Description    @details       Start monitor task.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully executed AT command.
 *                @retval        CELLULAR_ERR_EVENT_GROUP_INIT -
 *                                  Failed to initialize the event group.
 *                @retval        CELLULAR_ERR_CREATE_TASK -
 *                                  Failed to create task.
 ***************************************************************************/
e_cellular_err_t cellular_start_monitor_task (st_cellular_ctrl_t * const p_ctrl);

/****************************************************************************
 * Function Name  @fn            cellular_set_atc_number
 * Description    @details       Stores the AT command number being executed.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     command -
 *                                  Running AT command number.
 ***************************************************************************/
void cellular_set_atc_number (st_cellular_ctrl_t * const p_ctrl, const e_atc_list_t command);

/****************************************************************************
 * Function Name  @fn            cellular_get_atc_response
 * Description    @details       Get the response from the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Arguments      @param[in]     command -
 *                                  Running AT command number.
 * Return Value   @retval        ATC_RETURN_NONE -
 *                                  No response from the module.
 *                @retval        ATC_RETURN_OK -
 *                                  Module response is "OK".
 *                @retval        ATC_RETURN_ERROR -
 *                                  Module response is "ERROR".
 *                @retval        ATC_RETURN_OK_GO_SEND -
 *                                  Module response is ">".
 *                @retval        ATC_RETURN_SEND_NO_CARRIER -
 *                                  Module response is "NO CARRIER".
 *                @retval        ATC_RETURN_AP_CONNECTING -
 *                                  Module response is "CONNECT".
 *************************************************************************/
e_cellular_atc_return_t cellular_get_atc_response (st_cellular_ctrl_t * const p_ctrl);

/*************************************************************************************
 * Function Name  @fn            cellular_execute_at_command
 * Description    @details       Send AT command to the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     timeout_ms -
 *                                  Timeout millisecond.
 *                @param[in]     expect_code -
 *                                  Response expectancy.
 *                @param[in]     command -
 *                                  Running AT command number.
 * Return Value   @retval        CELLULAR_ATC_OK -
 *                                  Successfully executed AT command.
 *                @retval        CELLULAR_ATC_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ATC_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ATC_ERR_COMPARE -
 *                                  The return value from the module is disappointing.
 *                @retval        CELLULAR_ATC_ERR_TIMEOUT -
 *                                  Time out.
 ************************************************************************************/
e_cellular_err_t cellular_execute_at_command (st_cellular_ctrl_t * const p_ctrl, const uint32_t timeout_ms,
                                                        const e_cellular_atc_return_t expect_code,
                                                                const e_atc_list_t command);

/****************************************************************************************
 * Function Name  @fn            cellular_smcwrx
 * Description    @details       Starts a downlink continuous wave service.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     earfcn -
 *                                  An E-UTRA Absolute Radio Frequency Channel Number.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully executed AT command.
 *                @retval        CELLULAR_ATC_ERR_MODULE_COM -
 *                                  Communication with module failed.
 ***************************************************************************************/
e_cellular_err_t cellular_smcwrx (st_cellular_ctrl_t * const p_ctrl, const uint16_t earfcn);

/**************************************************************************************************************
 * Function Name  @fn            cellular_smcwtx
 * Description    @details       Starts an uplink continuous wave service.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     enable -
 *                                  1 to start; 0 to stop.
 *                @param[in]     earfcn -
 *                                  An E-UTRA Absolute Radio Frequency Channel Number.
 *                @param[in]     level -
 *                                  RF output power level of the continuous wave signal, in hundredths of dBm.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully executed AT command.
 *                @retval        CELLULAR_ATC_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *************************************************************************************************************/
e_cellular_err_t cellular_smcwtx (st_cellular_ctrl_t * const p_ctrl, const uint8_t enable,
                                    const uint16_t earfcn, const int32_t level);

/**************************************************************************************************************
 * Function Name  @fn            cellular_getpdpaddr
 * Description    @details       Stores the obtained PDP address in the structure.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] p_addr -
 *                                  Pointer to structure to store address.
 *************************************************************************************************************/
void cellular_getpdpaddr (st_cellular_ctrl_t * const p_ctrl, st_cellular_ipaddr_t * const p_addr);

#endif /* CELLULAR_PRIVATE_API_H */
