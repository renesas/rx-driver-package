/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
 * File Name    : r_fwupcomm_private.h
 * Description  : Private header file for the fwupcomm module.
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           20.05.2025 1.00    Initial Release
 *           11.11.2025 1.10    Added SPI communication support.
 ***********************************************************************************************************************/

#ifndef FWUPCOMM_SRC_R_FWUPCOMM_PRIVATE_H_
#define FWUPCOMM_SRC_R_FWUPCOMM_PRIVATE_H_

#include <string.h>
#include "r_smc_entry.h"
#include "r_fwupcomm_config.h"


/* Error directive */
#if ((defined (__CCRL__) || defined(__llvm__)) && (FWUPCOMM_CFG_DEVICE_PRIMARY == 1))
#error "The RL78 MCU can only be used as a secondary MCU."
#endif

#if ((FWUPCOMM_CFG_DEVICE_PRIMARY == 0) && (FWUPCOMM_CFG_CH_INTERFACE == 10))
#error "SCI SPI is only available when used as the primary MCU."
#endif

#if ((FWUPCOMM_CFG_SEND_PACKET_BUFFER_SIZE % 4 != 0) || (FWUPCOMM_CFG_RECV_PACKET_BUFFER_SIZE % 4 != 0))
#error "The size of send and recv buffers must be multiples of 4."
#endif

#define FWUPCOMM_COMM_IF_UART           (1)
#define FWUPCOMM_COMM_IF_SPI            (2)

#if   (FWUPCOMM_CFG_CH_INTERFACE == 1)  /* RX SCI UART */
#define FWUPCOMM_CH_RX_SCI_UART             (FWUPCOMM_CFG_CH_INTERFACE)
#define FWUPCOMM_COMM_IF                    (FWUPCOMM_COMM_IF_UART)
#elif (FWUPCOMM_CFG_CH_INTERFACE == 10) /* RX SCI SPI */
#define FWUPCOMM_CH_RX_SCI_SPI              (FWUPCOMM_CFG_CH_INTERFACE)
#define FWUPCOMM_COMM_IF                    (FWUPCOMM_COMM_IF_SPI)
#elif (FWUPCOMM_CFG_CH_INTERFACE == 11) /* RX RSPI SPI */
#define FWUPCOMM_CH_RX_RSPI_SPI             (FWUPCOMM_CFG_CH_INTERFACE)
#define FWUPCOMM_COMM_IF                    (FWUPCOMM_COMM_IF_SPI)
#elif (FWUPCOMM_CFG_CH_INTERFACE == 20) /* RL78/G23 CSI21 */
#define FWUPCOMM_CH_RL78G23_CSI21           (FWUPCOMM_CFG_CH_INTERFACE)
#define FWUPCOMM_COMM_IF                    (FWUPCOMM_COMM_IF_SPI)
#elif (FWUPCOMM_CFG_CH_INTERFACE == 21) /* RL78/G22 CSI21 */
#define FWUPCOMM_CH_RL78G22_CSI21           (FWUPCOMM_CFG_CH_INTERFACE)
#define FWUPCOMM_COMM_IF                    (FWUPCOMM_COMM_IF_SPI)
#elif (FWUPCOMM_CFG_CH_INTERFACE == 30) /* RL78/G23 SAU UART3 */
#define FWUPCOMM_CH_RL78G23_UART3           (FWUPCOMM_CFG_CH_INTERFACE)
#define FWUPCOMM_COMM_IF                    (FWUPCOMM_COMM_IF_UART)
#elif (FWUPCOMM_CFG_CH_INTERFACE == 31) /* RL78/G23 UARTA1 */
#define FWUPCOMM_CH_RL78G23_UARTA1          (FWUPCOMM_CFG_CH_INTERFACE)
#define FWUPCOMM_COMM_IF                    (FWUPCOMM_COMM_IF_UART)
#elif (FWUPCOMM_CFG_CH_INTERFACE == 32) /* RL78/G22 UART1 */
#define FWUPCOMM_CH_RL78G22_UART1           (FWUPCOMM_CFG_CH_INTERFACE)
#define FWUPCOMM_COMM_IF                    (FWUPCOMM_COMM_IF_UART)
#endif

#define FWUPCOMM_USE_CH                     (FWUPCOMM_CFG_CH_INTERFACE)

#if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_SCI_UART)
    #include "r_fwupcomm_rx_sci_uart.h"
#elif (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_SCI_SPI)
    #include "r_fwupcomm_rx_spi.h"
#elif (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)
    #include "r_fwupcomm_rx_spi.h"
#elif (FWUPCOMM_USE_CH == FWUPCOMM_CH_RL78G23_CSI21)
    #include "r_fwupcomm_rl78g23_csi21.h"
#elif (FWUPCOMM_USE_CH == FWUPCOMM_CH_RL78G22_CSI21)
    #include "r_fwupcomm_rl78g22_csi21.h"
#elif (FWUPCOMM_USE_CH == FWUPCOMM_CH_RL78G23_UART3)
    #include "r_fwupcomm_rl78g23_uart3.h"
#elif (FWUPCOMM_USE_CH == FWUPCOMM_CH_RL78G23_UARTA1)
    #include "r_fwupcomm_rl78g23_uarta1.h"
#elif (FWUPCOMM_USE_CH == FWUPCOMM_CH_RL78G22_UART1)
    #include "r_fwupcomm_rl78g22_uart1.h"
#endif


#if defined(__RX)
    /* memcpy wrap macro */
    #define MEMCPY(s1, s2, n)                 ( memcpy((s1), (s2), (n)) )
#else
    #if defined(__CCRL__)
        /* memcpy wrap macro */
        #define MEMCPY(s1, s2, n)             ( _COM_memcpy_ff((s1), (s2), (n)) )
    #elif defined(__llvm__)
        /* memcpy wrap macro */
        #define MEMCPY(s1, s2, n)             ( _COM_memcpy_ff((s1), (s2), (n)) )
    #endif /* defined(__CCRL__) */
#endif /* defined(__RX) */

#if BSP_CFG_RTOS_USED == 1  /* FreeRTOS is used. */
    #define FWUPCOMM_CMD_QUEUE_SIZE                 (5)
    #define FWUPCOMM_PROCESS_CMD_TASK_STACK_SIZE    (configMINIMAL_STACK_SIZE)
    #define FWUPCOMM_PROCESS_CMD_TASK_PRIORITY      (tskIDLE_PRIORITY + 1)
#endif /* BSP_CFG_RTOS_USED == 1 */

#define FWUPCOMM_CMD_HEADER_SIZE            (8)

#define FWUPCOMM_CMD_HEADER_DEVICE_ADDR     (0)
#define FWUPCOMM_CMD_HEADER_CMD_VER         (1)
#define FWUPCOMM_CMD_HEADER_CMD_INFO        (2)
#define FWUPCOMM_CMD_HEADER_CMD_OPTION      (3)
#define FWUPCOMM_CMD_HEADER_CMD             (4)
#define FWUPCOMM_CMD_HEADER_CMD_ARG         (5)
#define FWUPCOMM_CMD_HEADER_CMD_DATA_LEN    (6)
#define FWUPCOMM_CMD_HEADER_CMD_DATA        (8)

#define FWUPCOMM_BROADCAST_DEVICE_ADDRESS   (254)


/* Get the lower 8 bits of a variable of type uint16_t */
#define UINT16_LSB(X)                       ((uint8_t)((X) & 0x00ffU))
/* Get the upper 8 bits of a variable of type uint16_t */
#define UINT16_MSB(X)                       ((uint8_t)((X) >> 8))

typedef enum
{
    FWUPCOMM_PACKET_TYPE_CMD  = 0,
    FWUPCOMM_PACKET_TYPE_RESP = 1
} r_fwupcomm_packet_type_t;


#endif /* FWUPCOMM_SRC_R_FWUPCOMM_PRIVATE_H_ */
