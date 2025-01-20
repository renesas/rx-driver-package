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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_tfat_drv_if.c
* Description  : TFAT driver Interface.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.12.2014 1.00     First Release
*              : 21.01.2015 1.01     Added Support USB Mini Firmware.
*              : 22.06.2015 1.02     Added support MCU RX231.
*              : 01.04.2016 1.03     Updated the xml file.
*              : 29.06.2018 1.04     Modified get_fattime() and
*                                    SD memory device.
*              : 14.12.2018 1.05     Supporting USB dirver for RTOS.
*              : 08.08.2019 2.00     Added support for FreeRTOS and 
*                                    Renesas uITRON (RI600V4).
*                                    Added support for GNUC and ICCRX.
*              : 10.06.2020 2.10     Added Support MMC Firmware and 
*                                    FLASH Firmware.
*              : 31.08.2023 2.30     Updated FatFs ff15.
*              : 15.12.2023 2.40     Fixed to comply with GSCE Coding Standards
*                                    Rev.6.5.0.
*              : 08.05.2024 2.50     Added support SPI mode SD card.
*              : 30.09.2024 2.60     Changed the comment of API functions to
*                                    the Doxygen style.
*                                    Fixed to comply with GSCE Coding Standards
*                                    Rev.6.5.0.
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "platform.h"
#include "r_tfat_driver_rx_if.h"
#include "r_tfat_driver_rx_config.h"
#include "r_tfat_drv_if_dev.h"

#include "r_sys_time_rx_if.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* for USB */
#if (TFAT_USB_DRIVE_NUM > 0)
#if (TFAT_DRIVE_ALLOC_NUM_0 == TFAT_CTRL_USB)
#define USB_NUM_0    (1)
#define DRV0_DEV_CH  (0)
#else
#define USB_NUM_0    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_0 == TFAT_CTRL_USB)
#if (TFAT_DRIVE_ALLOC_NUM_1 == TFAT_CTRL_USB)
#define USB_NUM_1    (1<<1)
#define DRV1_DEV_CH  (0 + USB_NUM_0)
#else
#define USB_NUM_1    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_1 == TFAT_CTRL_USB)
#if (TFAT_DRIVE_ALLOC_NUM_2 == TFAT_CTRL_USB)
#define USB_NUM_2    (1<<2)
#define DRV2_DEV_CH  (0 + USB_NUM_0 + (USB_NUM_1>>1))
#else
#define USB_NUM_2    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_2 == TFAT_CTRL_USB)
#if (TFAT_DRIVE_ALLOC_NUM_3 == TFAT_CTRL_USB)
#define USB_NUM_3    (1<<3)
#define DRV3_DEV_CH  (0 + USB_NUM_0 + (USB_NUM_1>>1) + (USB_NUM_2>>2))
#else
#define USB_NUM_3    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_3 == TFAT_CTRL_USB)
#if (TFAT_DRIVE_ALLOC_NUM_4 == TFAT_CTRL_USB)
#define USB_NUM_4    (1<<4)
#define DRV4_DEV_CH  (0 + USB_NUM_0 + (USB_NUM_1>>1) + (USB_NUM_2>>2) \
                        + (USB_NUM_3>>3))
#else
#define USB_NUM_4    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_4 == TFAT_CTRL_USB)
#if (TFAT_DRIVE_ALLOC_NUM_5 == TFAT_CTRL_USB)
#define USB_NUM_5    (1<<5)
#define DRV5_DEV_CH  (0 + USB_NUM_0 + (USB_NUM_1>>1) + (USB_NUM_2>>2) \
                        + (USB_NUM_3>>3) + (USB_NUM_4>>4))
#else
#define USB_NUM_5    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_5 == TFAT_CTRL_USB)
#if (TFAT_DRIVE_ALLOC_NUM_6 == TFAT_CTRL_USB)
#define USB_NUM_6    (1<<6)
#define DRV6_DEV_CH  (0 + USB_NUM_0 + (USB_NUM_1>>1) + (USB_NUM_2>>2) \
                        + (USB_NUM_3>>3) + (USB_NUM_4>>4) + (USB_NUM_5>>5))
#else
#define USB_NUM_6    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_6 == TFAT_CTRL_USB)
#if (TFAT_DRIVE_ALLOC_NUM_7 == TFAT_CTRL_USB)
#define USB_NUM_7    (1<<7)
#define DRV7_DEV_CH  (0 + USB_NUM_0 + (USB_NUM_1>>1) + (USB_NUM_2>>2) \
                        + (USB_NUM_3>>3) + (USB_NUM_4>>4) + (USB_NUM_5>>5) \
                        + (USB_NUM_6>>6))
#else
#define USB_NUM_7    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_7 == TFAT_CTRL_USB)
#if (TFAT_DRIVE_ALLOC_NUM_8 == TFAT_CTRL_USB)
#define USB_NUM_8    (1<<8)
#define DRV8_DEV_CH  (0 + USB_NUM_0 + (USB_NUM_1>>1) + (USB_NUM_2>>2) \
                        + (USB_NUM_3>>3) + (USB_NUM_4>>4) + (USB_NUM_5>>5) \
                        + (USB_NUM_6>>6) + (USB_NUM_7>>7))
#else
#define USB_NUM_8    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_8 == TFAT_CTRL_USB)
#if (TFAT_DRIVE_ALLOC_NUM_9 == TFAT_CTRL_USB)
#define USB_NUM_9    (1<<9)
#define DRV9_DEV_CH  (0 + USB_NUM_0 + (USB_NUM_1>>1) + (USB_NUM_2>>2) \
                        + (USB_NUM_3>>3) + (USB_NUM_4>>4) + (USB_NUM_5>>5) \
                        + (USB_NUM_6>>6) + (USB_NUM_7>>7) + (USB_NUM_8>>8))
#else
#define USB_NUM_9    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_9 == TFAT_CTRL_USB)

static uint32_t TFAT_DRIVE_USB_ALLOC = (USB_NUM_9 | USB_NUM_8 | USB_NUM_7 | \
                                        USB_NUM_6 | USB_NUM_5 | USB_NUM_4 | \
                                        USB_NUM_3 | USB_NUM_2 | USB_NUM_1 | \
                                        USB_NUM_0);

#if TFAT_USB_DRIVE_NUM != ((USB_NUM_9>>9)+(USB_NUM_8>>8)+(USB_NUM_7>>7)+ \
                            (USB_NUM_6>>6)+(USB_NUM_5>>5)+ (USB_NUM_4>>4)+ \
                            (USB_NUM_3>>3)+(USB_NUM_2>>2)+(USB_NUM_1>>1)+ \
                            USB_NUM_0)
#error "Error! Invalid setting for TFAT_USB_DRIVE_NUM or TFAT_DRIVE_ALLOC_NUM_x in r_tfat_driver_rx_config.h"
#endif

#endif /* (TFAT_USB_DRIVE_NUM > 0) */


/* for SD memory card */
#if (TFAT_SDMEM_DRIVE_NUM > 0)
#if (TFAT_DRIVE_ALLOC_NUM_0 == TFAT_CTRL_SDMEM)
#define SDMEM_NUM_0   (1)
#define DRV0_DEV_CH   (0)
#else
#define SDMEM_NUM_0   (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_0 == TFAT_CTRL_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_1 == TFAT_CTRL_SDMEM)
#define SDMEM_NUM_1   (1<<1)
#define DRV1_DEV_CH   (0 + SDMEM_NUM_0)
#else
#define SDMEM_NUM_1   (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_1 == TFAT_CTRL_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_2 == TFAT_CTRL_SDMEM)
#define SDMEM_NUM_2   (1<<2)
#define DRV2_DEV_CH   (0 + SDMEM_NUM_0 + (SDMEM_NUM_1>>1))
#else
#define SDMEM_NUM_2   (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_2 == TFAT_CTRL_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_3 == TFAT_CTRL_SDMEM)
#define SDMEM_NUM_3   (1<<3)
#define DRV3_DEV_CH   (0 + SDMEM_NUM_0 + (SDMEM_NUM_1>>1) + (SDMEM_NUM_2>>2))
#else
#define SDMEM_NUM_3   (0)
#endif //  (TFAT_DRIVE_ALLOC_NUM_3 == TFAT_CTRL_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_4 == TFAT_CTRL_SDMEM)
#define SDMEM_NUM_4   (1<<4)
#define DRV4_DEV_CH   (0 + SDMEM_NUM_0 + (SDMEM_NUM_1>>1) + (SDMEM_NUM_2>>2) \
                        + (SDMEM_NUM_3>>3))
#else
#define SDMEM_NUM_4    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_4 == TFAT_CTRL_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_5 == TFAT_CTRL_SDMEM)
#define SDMEM_NUM_5   (1<<5)
#define DRV5_DEV_CH   (0 + SDMEM_NUM_0 + (SDMEM_NUM_1>>1) + (SDMEM_NUM_2>>2) \
                        + (SDMEM_NUM_3>>3) + (SDMEM_NUM_4>>4))
#else
#define SDMEM_NUM_5    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_5 == TFAT_CTRL_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_6 == TFAT_CTRL_SDMEM)
#define SDMEM_NUM_6   (1<<6)
#define DRV6_DEV_CH   (0 + SDMEM_NUM_0 + (SDMEM_NUM_1>>1) + (SDMEM_NUM_2>>2) \
                    + (SDMEM_NUM_3>>3) + (SDMEM_NUM_4>>4) + (SDMEM_NUM_5>>5))
#else
#define SDMEM_NUM_6    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_6 == TFAT_CTRL_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_7 == TFAT_CTRL_SDMEM)
#define SDMEM_NUM_7   (1<<7)
#define DRV7_DEV_CH   (0 + SDMEM_NUM_0 + (SDMEM_NUM_1>>1) + (SDMEM_NUM_2>>2) \
                    + (SDMEM_NUM_3>>3) + (SDMEM_NUM_4>>4) + (SDMEM_NUM_5>>5) \
                    + (SDMEM_NUM_6>>6))
#else
#define SDMEM_NUM_7    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_7 == TFAT_CTRL_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_8 == TFAT_CTRL_SDMEM)
#define SDMEM_NUM_8   (1<<8)
#define DRV8_DEV_CH   (0 + SDMEM_NUM_0 + (SDMEM_NUM_1>>1) + (SDMEM_NUM_2>>2) \
                    + (SDMEM_NUM_3>>3) + (SDMEM_NUM_4>>4) + (SDMEM_NUM_5>>5) \
                    + (SDMEM_NUM_6>>6) + (SDMEM_NUM_7>>7))
#else
#define SDMEM_NUM_8    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_8 == TFAT_CTRL_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_9 == TFAT_CTRL_SDMEM)
#define SDMEM_NUM_9   (1<<9)
#define DRV9_DEV_CH   (0 + SDMEM_NUM_0 + (SDMEM_NUM_1>>1) + (SDMEM_NUM_2>>2) \
                    + (SDMEM_NUM_3>>3) + (SDMEM_NUM_4>>4) + (SDMEM_NUM_5>>5) \
                    + (SDMEM_NUM_6>>6) + (SDMEM_NUM_7>>7) + (SDMEM_NUM_8>>8))
#else
#define SDMEM_NUM_9   (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_9 == TFAT_CTRL_SDMEM)

static uint32_t TFAT_DRIVE_SDMEM_ALLOC = (SDMEM_NUM_9 | SDMEM_NUM_8 | \
                                    SDMEM_NUM_7 | SDMEM_NUM_6 | SDMEM_NUM_5 | \
                                    SDMEM_NUM_4 | SDMEM_NUM_3 | SDMEM_NUM_2 | \
                                    SDMEM_NUM_1 | SDMEM_NUM_0);

#if TFAT_SDMEM_DRIVE_NUM != ( (SDMEM_NUM_9>>9)+(SDMEM_NUM_8>>8)+ \
                (SDMEM_NUM_7>>7)+(SDMEM_NUM_6>>6)+(SDMEM_NUM_5>>5)+ \
                (SDMEM_NUM_4>>4)+(SDMEM_NUM_3>>3)+(SDMEM_NUM_2>>2)+ \
                (SDMEM_NUM_1>>1)+SDMEM_NUM_0 )
#error "Error! Invalid setting for TFAT_SDMEM_DRIVE_NUM or TFAT_DRIVE_ALLOC_NUM_x in r_tfat_driver_rx_config.h"
#endif

#endif /* (TFAT_SDMEM_DRIVE_NUM > 0) */


/* for SD SPI memory card */
#if (TFAT_SPI_SDMEM_DRIVE_NUM > 0)
#if (TFAT_DRIVE_ALLOC_NUM_0 == TFAT_CTRL_SPI_SDMEM)
#define SPI_SDMEM_NUM_0   (1)
#define DRV0_DEV_CH       (0)
#else
#define SPI_SDMEM_NUM_0   (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_0 == TFAT_CTRL_SPI_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_1 == TFAT_CTRL_SPI_SDMEM)
#define SPI_SDMEM_NUM_1   (1<<1)
#define DRV1_DEV_CH       (0 + SPI_SDMEM_NUM_0)
#else
#define SPI_SDMEM_NUM_1   (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_1 == TFAT_CTRL_SPI_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_2 == TFAT_CTRL_SPI_SDMEM)
#define SPI_SDMEM_NUM_2   (1<<2)
#define DRV2_DEV_CH       (0 + SPI_SDMEM_NUM_0 + (SPI_SDMEM_NUM_1>>1))
#else
#define SPI_SDMEM_NUM_2   (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_2 == TFAT_CTRL_SPI_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_3 == TFAT_CTRL_SPI_SDMEM)
#define SPI_SDMEM_NUM_3   (1<<3)
#define DRV3_DEV_CH       (0 + SPI_SDMEM_NUM_0 + (SPI_SDMEM_NUM_1>>1) \
                            + (SPI_SDMEM_NUM_2>>2))
#else
#define SPI_SDMEM_NUM_3   (0)
#endif //  (TFAT_DRIVE_ALLOC_NUM_3 == TFAT_CTRL_SPI_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_4 == TFAT_CTRL_SPI_SDMEM)
#define SPI_SDMEM_NUM_4   (1<<4)
#define DRV4_DEV_CH       (0 + SPI_SDMEM_NUM_0 + (SPI_SDMEM_NUM_1>>1) \
                            + (SPI_SDMEM_NUM_2>>2) + (SPI_SDMEM_NUM_3>>3))
#else
#define SPI_SDMEM_NUM_4    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_4 == TFAT_CTRL_SPI_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_5 == TFAT_CTRL_SPI_SDMEM)
#define SPI_SDMEM_NUM_5   (1<<5)
#define DRV5_DEV_CH       (0 + SPI_SDMEM_NUM_0 + (SPI_SDMEM_NUM_1>>1) \
                            + (SPI_SDMEM_NUM_2>>2) + (SPI_SDMEM_NUM_3>>3) \
                            + (SPI_SDMEM_NUM_4>>4))
#else
#define SPI_SDMEM_NUM_5    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_5 == TFAT_CTRL_SPI_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_6 == TFAT_CTRL_SPI_SDMEM)
#define SPI_SDMEM_NUM_6   (1<<6)
#define DRV6_DEV_CH       (0 + SPI_SDMEM_NUM_0 + (SPI_SDMEM_NUM_1>>1) \
                            + (SPI_SDMEM_NUM_2>>2) + (SPI_SDMEM_NUM_3>>3) \
                            + (SPI_SDMEM_NUM_4>>4) + (SPI_SDMEM_NUM_5>>5))
#else
#define SPI_SDMEM_NUM_6    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_6 == TFAT_CTRL_SPI_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_7 == TFAT_CTRL_SPI_SDMEM)
#define SPI_SDMEM_NUM_7   (1<<7)
#define DRV7_DEV_CH       (0 + SPI_SDMEM_NUM_0 + (SPI_SDMEM_NUM_1>>1) \
                            + (SPI_SDMEM_NUM_2>>2) + (SPI_SDMEM_NUM_3>>3) \
                            + (SPI_SDMEM_NUM_4>>4) + (SPI_SDMEM_NUM_5>>5) \
                            + (SPI_SDMEM_NUM_6>>6))
#else
#define SPI_SDMEM_NUM_7    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_7 == TFAT_CTRL_SPI_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_8 == TFAT_CTRL_SPI_SDMEM)
#define SPI_SDMEM_NUM_8   (1<<8)
#define DRV8_DEV_CH       (0 + SPI_SDMEM_NUM_0 + (SPI_SDMEM_NUM_1>>1) \
                            + (SPI_SDMEM_NUM_2>>2) + (SPI_SDMEM_NUM_3>>3) \
                            + (SPI_SDMEM_NUM_4>>4) + (SPI_SDMEM_NUM_5>>5) \
                            + (SPI_SDMEM_NUM_6>>6) + (SPI_SDMEM_NUM_7>>7))
#else
#define SPI_SDMEM_NUM_8    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_8 == TFAT_CTRL_SPI_SDMEM)
#if (TFAT_DRIVE_ALLOC_NUM_9 == TFAT_CTRL_SPI_SDMEM)
#define SPI_SDMEM_NUM_9   (1<<9)
#define DRV9_DEV_CH       (0 + SPI_SDMEM_NUM_0 + (SPI_SDMEM_NUM_1>>1) \
                            + (SPI_SDMEM_NUM_2>>2) + (SPI_SDMEM_NUM_3>>3) \
                            + (SPI_SDMEM_NUM_4>>4) + (SPI_SDMEM_NUM_5>>5) \
                            + (SPI_SDMEM_NUM_6>>6) + (SPI_SDMEM_NUM_7>>7) \
                            + (SPI_SDMEM_NUM_8>>8))
#else
#define SPI_SDMEM_NUM_9   (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_9 == TFAT_CTRL_SPI_SDMEM)

static uint32_t TFAT_DRIVE_SPI_SDMEM_ALLOC = (SPI_SDMEM_NUM_9 | SPI_SDMEM_NUM_8 | \
                                        SPI_SDMEM_NUM_7 | SPI_SDMEM_NUM_6 | \
                                        SPI_SDMEM_NUM_5 | SPI_SDMEM_NUM_4 | \
                                        SPI_SDMEM_NUM_3 | SPI_SDMEM_NUM_2 | \
                                        SPI_SDMEM_NUM_1 | SPI_SDMEM_NUM_0);

#if TFAT_SPI_SDMEM_DRIVE_NUM != ((SPI_SDMEM_NUM_9>>9)+(SPI_SDMEM_NUM_8>>8) \
                                +(SPI_SDMEM_NUM_7>>7)+(SPI_SDMEM_NUM_6>>6) \
                                +(SPI_SDMEM_NUM_5>>5)+ (SPI_SDMEM_NUM_4>>4) \
                                +(SPI_SDMEM_NUM_3>>3)+(SPI_SDMEM_NUM_2>>2) \
                                +(SPI_SDMEM_NUM_1>>1)+SPI_SDMEM_NUM_0)
#error "Error! Invalid setting for TFAT_SPI_SDMEM_DRIVE_NUM or TFAT_DRIVE_ALLOC_NUM_x in r_tfat_driver_rx_config.h"
#endif

#endif /* (TFAT_SPI_SDMEM_DRIVE_NUM > 0) */


/* for MMC */
#if (TFAT_MMC_DRIVE_NUM > 0)
#if (TFAT_DRIVE_ALLOC_NUM_0 == TFAT_CTRL_MMC)
#define MMC_NUM_0    (1)
#define DRV0_DEV_CH  (0)
#else
#define MMC_NUM_0    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_0 == TFAT_CTRL_MMC)
#if (TFAT_DRIVE_ALLOC_NUM_1 == TFAT_CTRL_MMC)
#define MMC_NUM_1    (1<<1)
#define DRV1_DEV_CH  (0 + MMC_NUM_0)
#else
#define MMC_NUM_1    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_1 == TFAT_CTRL_MMC)
#if (TFAT_DRIVE_ALLOC_NUM_2 == TFAT_CTRL_MMC)
#define MMC_NUM_2    (1<<2)
#define DRV2_DEV_CH  (0 + MMC_NUM_0 + (MMC_NUM_1>>1))
#else
#define MMC_NUM_2    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_2 == TFAT_CTRL_MMC)
#if (TFAT_DRIVE_ALLOC_NUM_3 == TFAT_CTRL_MMC)
#define MMC_NUM_3    (1<<3)
#define DRV3_DEV_CH  (0 + MMC_NUM_0 + (MMC_NUM_1>>1) + (MMC_NUM_2>>2))
#else
#define MMC_NUM_3    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_3 == TFAT_CTRL_MMC)
#if (TFAT_DRIVE_ALLOC_NUM_4 == TFAT_CTRL_MMC)
#define MMC_NUM_4    (1<<4)
#define DRV4_DEV_CH  (0 + MMC_NUM_0 + (MMC_NUM_1>>1) + (MMC_NUM_2>>2) \
                        + (MMC_NUM_3>>3))
#else
#define MMC_NUM_4    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_4 == TFAT_CTRL_MMC)
#if (TFAT_DRIVE_ALLOC_NUM_5 == TFAT_CTRL_MMC)
#define MMC_NUM_5    (1<<5)
#define DRV5_DEV_CH  (0 + MMC_NUM_0 + (MMC_NUM_1>>1) + (MMC_NUM_2>>2) \
                        + (MMC_NUM_3>>3) + (MMC_NUM_4>>4))
#else
#define MMC_NUM_5    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_5 == TFAT_CTRL_MMC)
#if (TFAT_DRIVE_ALLOC_NUM_6 == TFAT_CTRL_MMC)
#define MMC_NUM_6    (1<<6)
#define DRV6_DEV_CH  (0 + MMC_NUM_0 + (MMC_NUM_1>>1) + (MMC_NUM_2>>2) \
                        + (MMC_NUM_3>>3) + (MMC_NUM_4>>4) + (MMC_NUM_5>>5))
#else
#define MMC_NUM_6    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_6 == TFAT_CTRL_MMC)
#if (TFAT_DRIVE_ALLOC_NUM_7 == TFAT_CTRL_MMC)
#define MMC_NUM_7    (1<<7)
#define DRV7_DEV_CH  (0 + MMC_NUM_0 + (MMC_NUM_1>>1) + (MMC_NUM_2>>2) \
                        + (MMC_NUM_3>>3) + (MMC_NUM_4>>4) \
                        + (MMC_NUM_5>>5) + (MMC_NUM_6>>6))
#else
#define MMC_NUM_7    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_7 == TFAT_CTRL_MMC)
#if (TFAT_DRIVE_ALLOC_NUM_8 == TFAT_CTRL_MMC)
#define MMC_NUM_8    (1<<8)
#define DRV8_DEV_CH  (0 + MMC_NUM_0 + (MMC_NUM_1>>1) + (MMC_NUM_2>>2) \
                        + (MMC_NUM_3>>3) + (MMC_NUM_4>>4) + (MMC_NUM_5>>5) \
                        + (MMC_NUM_6>>6) + (MMC_NUM_7>>7))
#else
#define MMC_NUM_8    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_8 == TFAT_CTRL_MMC)
#if (TFAT_DRIVE_ALLOC_NUM_9 == TFAT_CTRL_MMC)
#define MMC_NUM_9    (1<<9)
#define DRV9_DEV_CH  (0 + MMC_NUM_0 + (MMC_NUM_1>>1) + (MMC_NUM_2>>2) \
                        + (MMC_NUM_3>>3) + (MMC_NUM_4>>4) + (MMC_NUM_5>>5) \
                        + (MMC_NUM_6>>6) + (MMC_NUM_7>>7) + (MMC_NUM_8>>8))
#else
#define MMC_NUM_9    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_9 == TFAT_CTRL_MMC)

static uint32_t TFAT_DRIVE_MMC_ALLOC = (MMC_NUM_9 | MMC_NUM_8 | MMC_NUM_7 | \
                                        MMC_NUM_6 | MMC_NUM_5 | MMC_NUM_4 | \
                                        MMC_NUM_3 | MMC_NUM_2 | MMC_NUM_1 | \
                                        MMC_NUM_0);

#if TFAT_MMC_DRIVE_NUM != ((MMC_NUM_9>>9)+(MMC_NUM_8>>8)+(MMC_NUM_7>>7) \
                            +(MMC_NUM_6>>6)+(MMC_NUM_5>>5)+ (MMC_NUM_4>>4) \
                            +(MMC_NUM_3>>3)+(MMC_NUM_2>>2)+(MMC_NUM_1>>1) \
                            +MMC_NUM_0)
#error "Error! Invalid setting for TFAT_MMC_DRIVE_NUM or TFAT_DRIVE_ALLOC_NUM_x in r_tfat_driver_rx_config.h"
#endif

#endif /* (TFAT_MMC_DRIVE_NUM > 0) */

/* for USB mini */
#if (TFAT_USB_MINI_DRIVE_NUM > 0)
#if (TFAT_DRIVE_ALLOC_NUM_0 == TFAT_CTRL_USB_MINI)
#define USB_MINI_NUM_0    (1)
#define DRV0_DEV_CH       (0)
#else
#define USB_MINI_NUM_0    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_0 == TFAT_CTRL_USB_MINI)
#if (TFAT_DRIVE_ALLOC_NUM_1 == TFAT_CTRL_USB_MINI)
#define USB_MINI_NUM_1    (1<<1)
#define DRV1_DEV_CH       (0 + USB_MINI_NUM_0)
#else
#define USB_MINI_NUM_1    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_1 == TFAT_CTRL_USB_MINI)
#if (TFAT_DRIVE_ALLOC_NUM_2 == TFAT_CTRL_USB_MINI)
#define USB_MINI_NUM_2    (1<<2)
#define DRV2_DEV_CH       (0 + USB_MINI_NUM_0 + (USB_MINI_NUM_1>>1))
#else
#define USB_MINI_NUM_2    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_2 == TFAT_CTRL_USB_MINI)
#if (TFAT_DRIVE_ALLOC_NUM_3 == TFAT_CTRL_USB_MINI)
#define USB_MINI_NUM_3    (1<<3)
#define DRV3_DEV_CH       (0 + USB_MINI_NUM_0 + (USB_MINI_NUM_1>>1) \
                            + (USB_MINI_NUM_2>>2))
#else
#define USB_MINI_NUM_3    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_3 == TFAT_CTRL_USB_MINI)
#if (TFAT_DRIVE_ALLOC_NUM_4 == TFAT_CTRL_USB_MINI)
#define USB_MINI_NUM_4    (1<<4)
#define DRV4_DEV_CH       (0 + USB_MINI_NUM_0 + (USB_MINI_NUM_1>>1) \
                            + (USB_MINI_NUM_2>>2) + (USB_MINI_NUM_3>>3))
#else
#define USB_MINI_NUM_4    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_4 == TFAT_CTRL_USB_MINI)
#if (TFAT_DRIVE_ALLOC_NUM_5 == TFAT_CTRL_USB_MINI)
#define USB_MINI_NUM_5    (1<<5)
#define DRV5_DEV_CH       (0 + USB_MINI_NUM_0 + (USB_MINI_NUM_1>>1) \
                            + (USB_MINI_NUM_2>>2) + (USB_MINI_NUM_3>>3) \
                            + (USB_MINI_NUM_4>>4))
#else
#define USB_MINI_NUM_5    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_5 == TFAT_CTRL_USB_MINI)
#if (TFAT_DRIVE_ALLOC_NUM_6 == TFAT_CTRL_USB_MINI)
#define USB_MINI_NUM_6    (1<<6)
#define DRV6_DEV_CH       (0 + USB_MINI_NUM_0 + (USB_MINI_NUM_1>>1) \
                            + (USB_MINI_NUM_2>>2) + (USB_MINI_NUM_3>>3) \
                            + (USB_MINI_NUM_4>>4) + (USB_MINI_NUM_5>>5))
#else
#define USB_MINI_NUM_6    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_6 == TFAT_CTRL_USB_MINI)
#if (TFAT_DRIVE_ALLOC_NUM_7 == TFAT_CTRL_USB_MINI)
#define USB_MINI_NUM_7    (1<<7)
#define DRV7_DEV_CH       (0 + USB_MINI_NUM_0 + (USB_MINI_NUM_1>>1) \
                            + (USB_MINI_NUM_2>>2) + (USB_MINI_NUM_3>>3) \
                            + (USB_MINI_NUM_4>>4) + (USB_MINI_NUM_5>>5) \
                            + (USB_MINI_NUM_6>>6))
#else
#define USB_MINI_NUM_7    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_7 == TFAT_CTRL_USB_MINI)
#if (TFAT_DRIVE_ALLOC_NUM_8 == TFAT_CTRL_USB_MINI)
#define USB_MINI_NUM_8    (1<<8)
#define DRV8_DEV_CH       (0 + USB_MINI_NUM_0 + (USB_MINI_NUM_1>>1) \
                            + (USB_MINI_NUM_2>>2) + (USB_MINI_NUM_3>>3) \
                            + (USB_MINI_NUM_4>>4) + (USB_MINI_NUM_5>>5) \
                            + (USB_MINI_NUM_6>>6) + (USB_MINI_NUM_7>>7))
#else
#define USB_MINI_NUM_8    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_8 == TFAT_CTRL_USB_MINI)
#if (TFAT_DRIVE_ALLOC_NUM_9 == TFAT_CTRL_USB_MINI)
#define USB_MINI_NUM_9    (1<<9)
#define DRV9_DEV_CH       (0 + USB_MINI_NUM_0 + (USB_MINI_NUM_1>>1) \
                            + (USB_MINI_NUM_2>>2) + (USB_MINI_NUM_3>>3) \
                            + (USB_MINI_NUM_4>>4) + (USB_MINI_NUM_5>>5) \
                            + (USB_MINI_NUM_6>>6) + (USB_MINI_NUM_7>>7) \
                            + (USB_MINI_NUM_8>>8))
#else
#define USB_MINI_NUM_9    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_9 == TFAT_CTRL_USB_MINI)

static uint32_t TFAT_DRIVE_USB_MINI_ALLOC = (USB_MINI_NUM_9 | USB_MINI_NUM_8 | \
                                            USB_MINI_NUM_7 | USB_MINI_NUM_6 | \
                                            USB_MINI_NUM_5 | USB_MINI_NUM_4 | \
                                            USB_MINI_NUM_3 | USB_MINI_NUM_2 | \
                                            USB_MINI_NUM_1 | USB_MINI_NUM_0);

#if TFAT_USB_MINI_DRIVE_NUM != ((USB_MINI_NUM_9>>9)+(USB_MINI_NUM_8>>8) \
                                +(USB_MINI_NUM_7>>7)+(USB_MINI_NUM_6>>6) \
                                +(USB_MINI_NUM_5>>5)+(USB_MINI_NUM_4>>4) \
                                +(USB_MINI_NUM_3>>3)+(USB_MINI_NUM_2>>2)\
                                +(USB_MINI_NUM_1>>1)+USB_MINI_NUM_0)
#error "Error! Invalid setting for TFAT_USB_MINI_DRIVE_NUM or TFAT_DRIVE_ALLOC_NUM_x in r_tfat_driver_rx_config.h"
#endif

#endif /* (TFAT_USB_MINI_DRIVE_NUM > 0) */

/* for FLASH */
#if (TFAT_SERIAL_FLASH_DRIVE_NUM > 0)
#if (TFAT_DRIVE_ALLOC_NUM_0 == TFAT_CTRL_SERIAL_FLASH)
#define SERIAL_FLASH_NUM_0    (1)
#define DRV0_DEV_CH           (0)
#else
#define SERIAL_FLASH_NUM_0    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_0 == TFAT_CTRL_SERIAL_FLASH)
#if (TFAT_DRIVE_ALLOC_NUM_1 == TFAT_CTRL_SERIAL_FLASH)
#define SERIAL_FLASH_NUM_1    (1<<1)
#define DRV1_DEV_CH           (0 + SERIAL_FLASH_NUM_0)
#else
#define SERIAL_FLASH_NUM_1    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_1 == TFAT_CTRL_SERIAL_FLASH)
#if (TFAT_DRIVE_ALLOC_NUM_2 == TFAT_CTRL_SERIAL_FLASH)
#define SERIAL_FLASH_NUM_2    (1<<2)
#define DRV2_DEV_CH           (0 + SERIAL_FLASH_NUM_0 + (SERIAL_FLASH_NUM_1>>1))
#else
#define SERIAL_FLASH_NUM_2    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_2 == TFAT_CTRL_SERIAL_FLASH)
#if (TFAT_DRIVE_ALLOC_NUM_3 == TFAT_CTRL_SERIAL_FLASH)
#define SERIAL_FLASH_NUM_3    (1<<3)
#define DRV3_DEV_CH    (0 + SERIAL_FLASH_NUM_0 + (SERIAL_FLASH_NUM_1>>1) \
                        + (SERIAL_FLASH_NUM_2>>2))
#else
#define SERIAL_FLASH_NUM_3    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_3 == TFAT_CTRL_SERIAL_FLASH)
#if (TFAT_DRIVE_ALLOC_NUM_4 == TFAT_CTRL_SERIAL_FLASH)
#define SERIAL_FLASH_NUM_4    (1<<4)
#define DRV4_DEV_CH    (0 + SERIAL_FLASH_NUM_0 + (SERIAL_FLASH_NUM_1>>1) \
                            + (SERIAL_FLASH_NUM_2>>2) + (SERIAL_FLASH_NUM_3>>3))
#else
#define SERIAL_FLASH_NUM_4    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_4 == TFAT_CTRL_SERIAL_FLASH)
#if (TFAT_DRIVE_ALLOC_NUM_5 == TFAT_CTRL_SERIAL_FLASH)
#define SERIAL_FLASH_NUM_5    (1<<5)
#define DRV5_DEV_CH    (0 + SERIAL_FLASH_NUM_0 + (SERIAL_FLASH_NUM_1>>1) \
                        + (SERIAL_FLASH_NUM_2>>2) + (SERIAL_FLASH_NUM_3>>3) \
                        + (SERIAL_FLASH_NUM_4>>4))
#else
#define SERIAL_FLASH_NUM_5    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_5 == TFAT_CTRL_SERIAL_FLASH)
#if (TFAT_DRIVE_ALLOC_NUM_6 == TFAT_CTRL_SERIAL_FLASH)
#define SERIAL_FLASH_NUM_6    (1<<6)
#define DRV6_DEV_CH    (0 + SERIAL_FLASH_NUM_0 + (SERIAL_FLASH_NUM_1>>1) \
                        + (SERIAL_FLASH_NUM_2>>2) + (SERIAL_FLASH_NUM_3>>3) \
                        + (SERIAL_FLASH_NUM_4>>4) + (SERIAL_FLASH_NUM_5>>5))
#else
#define SERIAL_FLASH_NUM_6    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_6 == TFAT_CTRL_SERIAL_FLASH)
#if (TFAT_DRIVE_ALLOC_NUM_7 == TFAT_CTRL_SERIAL_FLASH)
#define SERIAL_FLASH_NUM_7    (1<<7)
#define DRV7_DEV_CH    (0 + SERIAL_FLASH_NUM_0 + (SERIAL_FLASH_NUM_1>>1) \
                        + (SERIAL_FLASH_NUM_2>>2) + (SERIAL_FLASH_NUM_3>>3) \
                        + (SERIAL_FLASH_NUM_4>>4) + (SERIAL_FLASH_NUM_5>>5) \
                        + (SERIAL_FLASH_NUM_6>>6))
#else
#define SERIAL_FLASH_NUM_7    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_7 == TFAT_CTRL_SERIAL_FLASH)
#if (TFAT_DRIVE_ALLOC_NUM_8 == TFAT_CTRL_SERIAL_FLASH)
#define SERIAL_FLASH_NUM_8    (1<<8)
#define DRV8_DEV_CH (0 + SERIAL_FLASH_NUM_0 + (SERIAL_FLASH_NUM_1>>1) \
                    + (SERIAL_FLASH_NUM_2>>2) + (SERIAL_FLASH_NUM_3>>3) \
                    + (SERIAL_FLASH_NUM_4>>4) + (SERIAL_FLASH_NUM_5>>5) \
                    + (SERIAL_FLASH_NUM_6>>6) + (SERIAL_FLASH_NUM_7>>7))
#else
#define SERIAL_FLASH_NUM_8    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_8 == TFAT_CTRL_SERIAL_FLASH)
#if (TFAT_DRIVE_ALLOC_NUM_9 == TFAT_CTRL_SERIAL_FLASH)
#define SERIAL_FLASH_NUM_9    (1<<9)
#define DRV9_DEV_CH    (0 + SERIAL_FLASH_NUM_0 + (SERIAL_FLASH_NUM_1>>1) \
                        + (SERIAL_FLASH_NUM_2>>2) + (SERIAL_FLASH_NUM_3>>3) \
                        + (SERIAL_FLASH_NUM_4>>4) + (SERIAL_FLASH_NUM_5>>5) \
                        + (SERIAL_FLASH_NUM_6>>6) + (SERIAL_FLASH_NUM_7>>7) \
                        + (SERIAL_FLASH_NUM_8>>8))
#else
#define SERIAL_FLASH_NUM_9    (0)
#endif // (TFAT_DRIVE_ALLOC_NUM_9 == TFAT_CTRL_SERIAL_FLASH)

static uint32_t TFAT_DRIVE_FLASH_ALLOC = (SERIAL_FLASH_NUM_9 | SERIAL_FLASH_NUM_8 | \
                                    SERIAL_FLASH_NUM_7 | SERIAL_FLASH_NUM_6 | \
                                    SERIAL_FLASH_NUM_5 | SERIAL_FLASH_NUM_4 | \
                                    SERIAL_FLASH_NUM_3 | SERIAL_FLASH_NUM_2 | \
                                    SERIAL_FLASH_NUM_1 | SERIAL_FLASH_NUM_0);

#if TFAT_SERIAL_FLASH_DRIVE_NUM != ((SERIAL_FLASH_NUM_9>>9)+(SERIAL_FLASH_NUM_8>>8) \
                            +(SERIAL_FLASH_NUM_7>>7)+(SERIAL_FLASH_NUM_6>>6) \
                            +(SERIAL_FLASH_NUM_5>>5)+ (SERIAL_FLASH_NUM_4>>4) \
                            +(SERIAL_FLASH_NUM_3>>3)+(SERIAL_FLASH_NUM_2>>2) \
                            +(SERIAL_FLASH_NUM_1>>1)+SERIAL_FLASH_NUM_0)
#error "Error! Invalid setting for TFAT_SERIAL_FLASH_DRIVE_NUM or TFAT_DRIVE_ALLOC_NUM_x in r_tfat_driver_rx_config.h"
#endif

#endif /* (TFAT_SERIAL_FLASH_DRIVE_NUM > 0) */

#ifndef DRV0_DEV_CH
#define DRV0_DEV_CH  (0)
#endif
#ifndef DRV1_DEV_CH
#define DRV1_DEV_CH  (0)
#endif
#ifndef DRV2_DEV_CH
#define DRV2_DEV_CH  (0)
#endif
#ifndef DRV3_DEV_CH
#define DRV3_DEV_CH  (0)
#endif
#ifndef DRV4_DEV_CH
#define DRV4_DEV_CH  (0)
#endif
#ifndef DRV5_DEV_CH
#define DRV5_DEV_CH  (0)
#endif
#ifndef DRV6_DEV_CH
#define DRV6_DEV_CH  (0)
#endif
#ifndef DRV7_DEV_CH
#define DRV7_DEV_CH  (0)
#endif
#ifndef DRV8_DEV_CH
#define DRV8_DEV_CH  (0)
#endif
#ifndef DRV9_DEV_CH
#define DRV9_DEV_CH  (0)
#endif

#define TFAT_DRIVE_ALLOC_NUM_MAX    (TFAT_DRIVE_NUM_MAX)

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
*******************************************************************************/
static uint8_t drive_alloc_tbl[TFAT_DRIVE_ALLOC_NUM_MAX][2] =
{
    { TFAT_DRIVE_ALLOC_NUM_0, DRV0_DEV_CH },
    { TFAT_DRIVE_ALLOC_NUM_1, DRV1_DEV_CH },
    { TFAT_DRIVE_ALLOC_NUM_2, DRV2_DEV_CH },
    { TFAT_DRIVE_ALLOC_NUM_3, DRV3_DEV_CH },
    { TFAT_DRIVE_ALLOC_NUM_4, DRV4_DEV_CH },
    { TFAT_DRIVE_ALLOC_NUM_5, DRV5_DEV_CH },
    { TFAT_DRIVE_ALLOC_NUM_6, DRV6_DEV_CH },
    { TFAT_DRIVE_ALLOC_NUM_7, DRV7_DEV_CH },
    { TFAT_DRIVE_ALLOC_NUM_8, DRV8_DEV_CH },
    { TFAT_DRIVE_ALLOC_NUM_9, DRV9_DEV_CH },
};

/******************************************************************************
* Function Name : chk_use_usb
* Description   : allocation check
* Arguments     : uint8_t drive : Physical drive number for TFAT module
* Return value  : true : 0x01
******************************************************************************/
#if (TFAT_USB_DRIVE_NUM > 0)
static uint8_t chk_use_usb(uint8_t drive)
{
    return (((TFAT_DRIVE_USB_ALLOC & (0x01 << drive)) != 0) ?  0x01 : 0x00);
}
#endif /* USE_USB */

/******************************************************************************
* Function Name : chk_use_sdmem
* Description   : allocation check
* Arguments     : uint8_t drive : Physical drive number for TFAT module
* Return value  : true : 0x01
******************************************************************************/
#if (TFAT_SDMEM_DRIVE_NUM > 0)
static uint8_t chk_use_sdmem(uint8_t drive)
{
    return (((TFAT_DRIVE_SDMEM_ALLOC & (0x01 << drive)) != 0) ?  0x01 : 0x00);
}
#endif /* USE_SDMEM */

/******************************************************************************
* Function Name : chk_use_spi_sdmem
* Description   : allocation check
* Arguments     : uint8_t drive : Physical drive number for TFAT module
* Return value  : true : 0x01
******************************************************************************/
#if (TFAT_SPI_SDMEM_DRIVE_NUM > 0)
static uint8_t chk_use_spi_sdmem(uint8_t drive)
{
    return (((TFAT_DRIVE_SPI_SDMEM_ALLOC & (0x01 << drive)) != 0) ?  0x01 : 0x00);
}
#endif /* USE_SPI_SDMEM */

/******************************************************************************
* Function Name : chk_use_mmc
* Description   : allocation check
* Arguments     : uint8_t drive : Physical drive number for TFAT module
* Return value  : true : 0x01
******************************************************************************/
#if TFAT_MMC_DRIVE_NUM > 0
static uint8_t chk_use_mmc(uint8_t drive)
{
    return (((TFAT_DRIVE_MMC_ALLOC & (0x01 << drive)) != 0) ?  0x01 : 0x00);
}
#endif /* USE_MMC */

/******************************************************************************
* Function Name : chk_use_usb_mini
* Description   : allocation check
* Arguments     : uint8_t drive : Physical drive number for TFAT module
* Return value  : true : 0x01
******************************************************************************/
#if (TFAT_USB_MINI_DRIVE_NUM > 0)
static uint8_t chk_use_usb_mini(uint8_t drive)
{
    return (((TFAT_DRIVE_USB_MINI_ALLOC & (0x01 << drive)) != 0) ?  0x01 : 0x00);
}
#endif /* USE_USB */

/******************************************************************************
* Function Name : chk_use_flash
* Description   : allocation check
* Arguments     : uint8_t drive : Physical drive number for TFAT module
* Return value  : true : 0x01
******************************************************************************/
#if TFAT_SERIAL_FLASH_DRIVE_NUM > 0
static uint8_t chk_use_flash(uint8_t drive)
{
    return (((TFAT_DRIVE_FLASH_ALLOC & (0x01 << drive)) != 0) ?  0x01 : 0x00);
}
#endif /* USE_FLASH */


/**********************************************************************************************************************
* Function Name: disk_initialize
*******************************************************************************************************************//**
* @brief The disk_initialize() function is called to initialize the storage device.
*
* @param[in]    pdrv  Physical drive number to identify the target device. Always zero at single drive system.
*
* @return This function returns the current drive status flags as the result. For details of the drive status,
* refer to the disk_status() function.
*
* @details This function initializes the storage device and puts it ready to generic read/write. When the function
* succeeds, STA_NOINIT flag in the return value is cleared.\n
* Remarks: This function needs to be under the control of FatFs module. Application program MUST NOT call this function,
* or FAT structure on the volume can be broken. To re-initialize the filesystem, use f_mount function instead.
* @note None
*/
DSTATUS disk_initialize (
    BYTE pdrv    /* Physical drive number to identify the drive */
)
{
    DSTATUS ret = RES_PARERR;

#if (TFAT_USB_DRIVE_NUM > 0)
    if (chk_use_usb(pdrv))
    {
        /* function for USB */
        ret = usb_disk_initialize(drive_alloc_tbl[pdrv][1]);
    }
#endif

#if (TFAT_SDMEM_DRIVE_NUM > 0)
    if (chk_use_sdmem(pdrv))
    {
        /* function for SDMEM */
        ret = sdmem_disk_initialize(drive_alloc_tbl[pdrv][1]);
    }
#endif

#if (TFAT_SPI_SDMEM_DRIVE_NUM > 0)
    if (chk_use_spi_sdmem(pdrv))
    {
        /* function for SPI SDMEM */
        ret = spi_sdmem_disk_initialize(drive_alloc_tbl[pdrv][1]);
    }
#endif

#if (TFAT_MMC_DRIVE_NUM > 0)
    if (chk_use_mmc(pdrv))
    {
        /* function for mmc */
        ret = mmcif_disk_initialize(drive_alloc_tbl[pdrv][1]);
    }
#endif

#if (TFAT_USB_MINI_DRIVE_NUM > 0)
    if (chk_use_usb_mini(pdrv))
    {
        /* function for USB Mini */
        ret = usb_mini_disk_initialize(drive_alloc_tbl[pdrv][1]);
    }
#endif

#if (TFAT_SERIAL_FLASH_DRIVE_NUM > 0)
    if (chk_use_flash(pdrv))
    {
        /* function for FLASH */
        ret = flash_spi_disk_initialize(drive_alloc_tbl[pdrv][1]);
    }
#endif

    return  ret;
}
/******************************************************************************
 End of function disk_initialize
 *****************************************************************************/


/***********************************************************************************************************************
* Function Name: disk_read
********************************************************************************************************************//**
* @brief This function is called to read data from the sector(s) of storage device.
*
* @param[in]    pdrv   Physical drive number to identify the target device.
*
* @param[out]   buff   Pointer to the first item of the byte array to store read data. Size of read data will
* be the sector size * count bytes.
*
* @param[in]    sector Start sector number in 32-bit logical block address (LBA).
*
* @param[in]    count  Number of sectors to read.
*
* @retval RES_OK     The function succeeded.
*
* @retval RES_ERROR  An unrecoverable hard error occurred during the read operation.
*
* @retval RES_PARERR Invalid parameter.
*
* @retval RES_NOTRDY The device has not been initialized.
*
* @details Read/write operation to the generic storage devices, such as memory card, hard disk and optical disk, is
* done in unit of block of data bytes called sector. FatFs supports the sector size in the range of 512 to 4096 bytes.
* When FatFs is configured for fixed sector size (FF_MIN_SS == FF_MAX_SS, this is the most case), the read/write
* function must work at that sector size. When FatFs is configured for variable sector size (FF_MIN_SS < FF_MAX_SS),
* the sector size of medium is inquired with disk_ioctl() function immediately following disk_initialize() function
* succeeded.\n
* There are some considerations about the memory address passed via buff. It is not that always aligned to word
* boundary because the argument is defined as BYTE*. The unaligned transfer request can occur at direct transfer.
* If the bus architecture, especially DMA controller, does not allow unaligned memory access, it should be solved
* in this function. If it is the case, there are some workarounds described below to avoid this issue.\n
* Convert word transfer to byte transfer with some method in this function. - Recommended.\n
* On the f_read() calls, avoid long read request that includes a whole sector. - Any direct transfer never
* occurs.\n
* On the f_read(fp, dat, btw, bw) calls, make sure that (((UINT)dat & 3) == (f_tell(fp) & 3)) is true. - Word
* alignment of buff is guaranteed.\n
* Also, the memory area may be out of reach in DMA. This is the case if it is in tightly coupled memory which is
* usually used for stack. Use double buffered transfer or avoid to define any file I/O buffer includes FatFs and
* FIL structure as local variables where on the stack.\n
* Generally, a multiple sector read request must not be split into single sector transactions to the storage
* device, or read throughput gets worse.
* @note None
*/
DRESULT disk_read (
    BYTE pdrv,        /* Physical drive number to identify the drive */
    BYTE *buff,       /* Data buffer to store read data */
    LBA_t sector,     /* Start sector in LBA */
    UINT count        /* Number of sectors to read */
)
{
    DRESULT ret = RES_PARERR;

#if (TFAT_USB_DRIVE_NUM > 0)
    if (chk_use_usb(pdrv))
    {
        /* function for USB */
        ret = usb_disk_read(drive_alloc_tbl[pdrv][1], buff, sector, count);
    }
#endif

#if (TFAT_SDMEM_DRIVE_NUM > 0)
    if (chk_use_sdmem(pdrv))
    {
        /* function for SDMEM */
        ret = sdmem_disk_read(drive_alloc_tbl[pdrv][1], buff, sector, count);
    }
#endif

#if (TFAT_SPI_SDMEM_DRIVE_NUM > 0)
    if (chk_use_spi_sdmem(pdrv))
    {
        /* function for SPI SDMEM */
        ret = spi_sdmem_disk_read(drive_alloc_tbl[pdrv][1], buff, sector, count);
    }
#endif

#if (TFAT_MMC_DRIVE_NUM > 0)
    if (chk_use_mmc(pdrv))
    {
        /* function for MMC */
        ret = mmcif_disk_read(drive_alloc_tbl[pdrv][1], buff, sector, count);
    }
#endif

#if (TFAT_USB_MINI_DRIVE_NUM > 0)
    if (chk_use_usb_mini(pdrv))
    {
        /* function for USB Mini */
        ret = usb_mini_disk_read(drive_alloc_tbl[pdrv][1], buff,
                                    sector, count);
    }
#endif

#if (TFAT_SERIAL_FLASH_DRIVE_NUM > 0)
    if ( chk_use_flash(pdrv) )
    {
        /* function for FLASH */
        ret = flash_spi_disk_read(drive_alloc_tbl[pdrv][1], buff,
                                    sector, count);
    }
#endif

    return ret;
}
/******************************************************************************
 End of function disk_read
 *****************************************************************************/


/***********************************************************************************************************************
* Function Name: disk_write
********************************************************************************************************************//**
* @brief This function is called to write data to the sector(s) of storage device.
*
* @param[in]   pdrv   Physical drive number to identify the target device.
*
* @param[in]   buff   Pointer to the first item of the byte array to be written. The size of data to be written
* is sector size * count bytes.
*
* @param[in]   sector Start sector number in 32-bit logical block address (LBA).
*
* @param[in]   count  Number of sectors to write.
*
* @retval RES_OK     The function succeeded.
*
* @retval RES_ERROR  An unrecoverable hard error occurred during the write operation.
*
* @retval RES_WRPRT  The device is write-protected.
*
* @retval RES_PARERR Invalid parameter.
*
* @retval RES_NOTRDY The device has not been initialized.
*
* @details The specified memory address is not that always aligned to word boundary because the argument is
* defined as BYTE*. For more information, refer to the description of disk_read() function.\n
* Generally, a multiple sector write request (count > 1) must not be split into single sector transactions
* to the storage device, or the file write throughput will be drastically decreased.\n
* FatFs expects delayed write function of the disk control layer. The write operation to the media does not
* need to be completed at return from this function by what write operation is in progress or data is only
* stored into the write-back cache. But writting data on the buff is invalid after return from this function.
* The write completion request is done by CTRL_SYNC command of disk_ioctl() function. Therefore, if a delayed
* write function is implemented, the write throughput of the filesystem will be improved.\n
* Remarks: Application program MUST NOT call this function, or FAT structure on the volume can be collapsed.
* @note This function is not needed when FF_FS_READONLY = 1.
*/
DRESULT disk_write (
    BYTE pdrv,            /* Physical drive number to identify the drive */
    const BYTE *buff,     /* Data to be written */
    LBA_t sector,         /* Start sector in LBA */
    UINT count            /* Number of sectors to write */
)
{
    DRESULT ret = RES_PARERR;

#if (TFAT_USB_DRIVE_NUM > 0)
    if (chk_use_usb(pdrv))
    {
        /* function for USB */
        ret = usb_disk_write(drive_alloc_tbl[pdrv][1], buff, sector, count);
    }
#endif

#if (TFAT_SDMEM_DRIVE_NUM > 0)
    if (chk_use_sdmem(pdrv))
    {
        /* function for SDMEM */
        ret = sdmem_disk_write(drive_alloc_tbl[pdrv][1], buff, sector, count);
    }
#endif

#if (TFAT_SPI_SDMEM_DRIVE_NUM > 0)
    if (chk_use_spi_sdmem(pdrv))
    {
        /* function for SPI SDMEM */
        ret = spi_sdmem_disk_write(drive_alloc_tbl[pdrv][1], buff, sector, count);
    }
#endif

#if (TFAT_MMC_DRIVE_NUM > 0)
    if (chk_use_mmc(pdrv))
    {
        /* function for MMC */
        ret = mmcif_disk_write(drive_alloc_tbl[pdrv][1], buff, sector, count);
    }
#endif

#if (TFAT_USB_MINI_DRIVE_NUM > 0)
    if (chk_use_usb_mini(pdrv))
    {
        /* function for USB Mini */
        ret = usb_mini_disk_write(drive_alloc_tbl[pdrv][1], buff,
                                    sector, count);
    }
#endif

#if (TFAT_SERIAL_FLASH_DRIVE_NUM > 0)
    if (chk_use_flash(pdrv))
    {
        /* function for FLASH */
        ret = flash_spi_disk_write(drive_alloc_tbl[pdrv][1], buff,
                                    sector, count);
    }
#endif

    return ret;
}
/******************************************************************************
 End of function disk_write
 *****************************************************************************/


/***********************************************************************************************************************
* Function Name: disk_ioctl
********************************************************************************************************************//**
* @brief This function is called to control device specific features and miscellaneous functions other
* than generic read/write.
*
* @param[in]      pdrv  Physical drive number to identify the target device.
*
* @param[in]      cmd   Command code.
*
* @param[in, out] buff  Pointer to the parameter depends on the command code. Do not care if the command has no
* parameter to be passed.
*
* @retval RES_OK     The function succeeded.
*
* @retval RES_ERROR  An error occurred.
*
* @retval RES_PARERR The command code or parameter is invalid.
*
* @retval RES_NOTRDY The device has not been initialized.
*
* @details The FatFs module requires only five device general-purpose commands. See Section disk_ioctl() in the
* application note for details.
* @note The disk_ioctl() function is not needed when FF_FS_READONLY = 1 and FF_MAX_SS = FF_MIN_SS.
*/
DRESULT disk_ioctl (
    BYTE pdrv,        /* Physical drive nmuber (0..) */
    BYTE cmd,         /* Control code */
    void *buff        /* Buffer to send/receive control data */
)
{
    DRESULT ret = RES_PARERR;

#if (TFAT_USB_DRIVE_NUM > 0)
    if (chk_use_usb(pdrv))
    {
        /* function for USB */
        ret = usb_disk_ioctl(drive_alloc_tbl[pdrv][1], cmd, buff);
    }
#endif

#if (TFAT_SDMEM_DRIVE_NUM > 0)
    if (chk_use_sdmem(pdrv))
    {
        /* function for SDMEM */
        ret = sdmem_disk_ioctl (drive_alloc_tbl[pdrv][1], cmd, buff);
    }
#endif

#if (TFAT_SPI_SDMEM_DRIVE_NUM > 0)
    if (chk_use_spi_sdmem(pdrv))
    {
        /* function for SPI SDMEM */
        ret = spi_sdmem_disk_ioctl (drive_alloc_tbl[pdrv][1], cmd, buff);
    }
#endif

#if (TFAT_MMC_DRIVE_NUM > 0)
    if (chk_use_mmc(pdrv))
    {
        /* function for MMC */
        ret = mmcif_disk_ioctl (drive_alloc_tbl[pdrv][1], cmd, buff);
    }
#endif

#if (TFAT_USB_MINI_DRIVE_NUM > 0)
    if (chk_use_usb_mini(pdrv))
    {
        /* function for USB Mini */
        ret = usb_mini_disk_ioctl(drive_alloc_tbl[pdrv][1], cmd, buff);
    }
#endif

#if (TFAT_SERIAL_FLASH_DRIVE_NUM > 0)
    if (chk_use_flash(pdrv))
    {
        /* function for FLASH */
        ret = flash_spi_disk_ioctl (drive_alloc_tbl[pdrv][1], cmd, buff);
    }
#endif

    return ret;
}
/******************************************************************************
 End of function disk_ioctl
 *****************************************************************************/


/***********************************************************************************************************************
* Function Name: disk_status
********************************************************************************************************************//**
* @brief This function is called to inquire the current drive status.
*
* @param[in] pdrv Physical drive number to identify the target device. Always zero at single drive system.
*
* @return The current drive status is returned in combination of status flags described below. FatFs refers only
* to STA_NOINIT and STA_PROTECT.
* \li STA_NOINIT: Indicates that the device has not been initialized and not ready to work. This flag is set
* on system reset, media removal or failure of disk_initialize() function. It is cleared on disk_initialize function
* succeeded. Any media change that occurs asynchronously must be captured and reflected to the status flags, or
* auto-mount function will not work correctly. If the system does not support media change detection, the
* application program needs to explicitly re-mount the volume with f_mount function after each media change.
* \li STA_NODISK: Indicates that there is no medium in the drive. This is always cleared at fixed disk drive. Note that
* FatFs does not refer to this flag.
* \li STA_PROTECT: Indicates that the medium is write-protected. This is always cleared at the drives without
* the write protect function. Not valid if STA_NODISK is set.
*
* @details None
* @note None
*/
DSTATUS disk_status (
    BYTE pdrv        /* Physical drive number to identify the drive */
)
{
    DSTATUS ret = RES_PARERR;

#if (TFAT_USB_DRIVE_NUM > 0)
    if (chk_use_usb(pdrv))
    {
        /* function for USB */
        ret = usb_disk_status (drive_alloc_tbl[pdrv][1]);
    }
#endif

#if (TFAT_SDMEM_DRIVE_NUM > 0)
    if (chk_use_sdmem(pdrv))
    {
        /* function for SDMEM */
        ret = sdmem_disk_status (drive_alloc_tbl[pdrv][1]);
    }
#endif

#if (TFAT_SPI_SDMEM_DRIVE_NUM > 0)
    if (chk_use_spi_sdmem(pdrv))
    {
        /* function for SPI SDMEM */
        ret = spi_sdmem_disk_status (drive_alloc_tbl[pdrv][1]);
    }
#endif

#if (TFAT_MMC_DRIVE_NUM > 0)
    if (chk_use_mmc(pdrv))
    {
        /* function for MMC */
        ret = mmcif_disk_status (drive_alloc_tbl[pdrv][1]);
    }
#endif

#if (TFAT_USB_MINI_DRIVE_NUM > 0)
    if (chk_use_usb_mini(pdrv))
    {
        /* function for USB Mini */
        ret = usb_mini_disk_status (drive_alloc_tbl[pdrv][1]);
    }
#endif

#if (TFAT_SERIAL_FLASH_DRIVE_NUM > 0)
    if (chk_use_flash(pdrv))
    {
        /* function for FLASH */
        ret = flash_spi_disk_status (drive_alloc_tbl[pdrv][1]);
    }
#endif

    return ret;
}
/******************************************************************************
 End of function disk_status
 *****************************************************************************/


/******************************************************************************
* Function Name : disk_1ms_interval
* Description   : This function is used to intialize the 1ms timer
* Arguments     : None
* Return value  : None
******************************************************************************/
void disk_1ms_interval(void)
{
#if (TFAT_SERIAL_FLASH_DRIVE_NUM > 0)
    flash_spi_1ms_interval (); /* function for FLASH */
#endif
}
/******************************************************************************
 End of function disk_1ms_interval
 *****************************************************************************/


#if !FF_FS_READONLY && !FF_FS_NORTC
/***********************************************************************************************************************
* Function Name: get_fattime
********************************************************************************************************************//**
* @brief This function is called to get the current time.
*
* @return Current local time shall be returned as bit-fields packed into a DWORD value. The bit fields are
* as follows:
* \li bit31:25 Year origin from the 1980 (0..127, e.g. 37 for 2017)
* \li bit24:21 Month (1..12)
* \li bit20:16 Day of the month (1..31)
* \li bit15:11 Hour (0..23)
* \li bit10:5  Minute (0..59)
* \li bit4:0   Second / 2 (0..29, e.g. 25 for 50)
*
* @details The get_fattime() function shall return any valid time even if the system does not support a real time
* clock. If a zero is returned, the file will not have a valid timestamp.
* @note This function is not needed when FF_FS_READONLY = 1 or FF_FS_NORTC = 1.
*/
DWORD get_fattime(void)
{
    uint32_t tmr;
    SYS_TIME time;

    /* Disable interrupts */
    R_BSP_InterruptsDisable();

    R_SYS_TIME_GetCurrentTime(&time);

    /* Convert the time to store in FAT entry */
    tmr  =  (((uint32_t)time.year - 60) << 25);

    /* Cast to 'uint32_t' type */
    tmr |=  ((uint32_t)time.month << 21);

    /* Cast to 'uint32_t' type */
    tmr |=  ((uint32_t)time.day << 16);

    /* Cast to 'uint32_t' type */
    tmr |=  ((uint32_t)time.hour << 11);

    /* Cast to 'uint32_t' type */
    tmr |=  ((uint32_t)time.min << 5);

    /* Cast to 'uint32_t' type */
    tmr |=  ((uint32_t)time.sec >> 1);

    /* Enable interrupts    */
    R_BSP_InterruptsEnable();

    return (DWORD)tmr;
}
/******************************************************************************
 End of function get_fattime
 *****************************************************************************/

#endif /* #if !FF_FS_READONLY && !FF_FS_NORTC */

/***********************************************************************************************************************
* Function Name: drv_change_alloc
********************************************************************************************************************//**
* @brief This function changes a drive's allocation. This function has nothing to do with FatFs
* and is a unique API of Renesas.
*
* @param[in] tfat_drv    The physical drive number for TFAT FIT.
*
* @param[in] dev_type    The device defines type (TFAT_USB_DRIVE_NUM, TFAT_SDMEM_DRIVE_NUM, or TFAT_USB_MINI_DRIVE_NUM).
*
* @param[in] dev_drv_num The drive number/device channel for device driver.
*
* @retval RES_OK     The function succeeded.
*
* @retval RES_PARERR The specified value of tfat_drv is invalid.
*
* @details The drive used for TFAT FIT is specified by the TFAT_DRIVE_ALLOC_NUM_i definition in
* r_tfat_driver_rx_config.h, and the drive number for TFAT FIT is associated with the drive number of
* the memory driver.\n
* Drive numbers for memory drivers are automatically assigned in ascending order.\n
* Use this function if you want to change the association dynamically.
* @note None
*/
DRESULT drv_change_alloc(TFAT_DRV_NUM tfat_drv, uint8_t dev_type,
                                            uint8_t dev_drv_num)
{
    if (TFAT_DRIVE_ALLOC_NUM_MAX <= tfat_drv)
    {
        return RES_ERROR;
    }

#if (TFAT_USB_DRIVE_NUM > 0)
    if (TFAT_CTRL_USB == dev_type)
    {
        TFAT_DRIVE_USB_ALLOC |= (0x01 << tfat_drv);
    }
    else
    {
        TFAT_DRIVE_USB_ALLOC &= ~(0x01 << tfat_drv);
    }
#endif
#if (TFAT_SDMEM_DRIVE_NUM > 0)
    if (TFAT_CTRL_SDMEM == dev_type)
    {
        TFAT_DRIVE_SDMEM_ALLOC |= (0x01 << tfat_drv);
    }
    else
    {
        TFAT_DRIVE_SDMEM_ALLOC &= ~(0x01 << tfat_drv);
    }
#endif
#if (TFAT_SPI_SDMEM_DRIVE_NUM > 0)
    if (TFAT_CTRL_SPI_SDMEM == dev_type)
    {
        TFAT_DRIVE_SPI_SDMEM_ALLOC |= (0x01 << tfat_drv);
    }
    else
    {
        TFAT_DRIVE_SPI_SDMEM_ALLOC &= ~(0x01 << tfat_drv);
    }
#endif
#if (TFAT_MMC_DRIVE_NUM > 0)
    if (TFAT_CTRL_MMC == dev_type)
    {
        TFAT_DRIVE_MMC_ALLOC |= (0x01 << tfat_drv);
    }
    else
    {
        TFAT_DRIVE_MMC_ALLOC &= ~(0x01 << tfat_drv);
    }
#endif
#if (TFAT_USB_MINI_DRIVE_NUM > 0)
    if (TFAT_CTRL_USB_MINI == dev_type)
    {
        TFAT_DRIVE_USB_MINI_ALLOC |= (0x01 << tfat_drv);
    }
    else
    {
        TFAT_DRIVE_USB_MINI_ALLOC &= ~(0x01 << tfat_drv);
    }
#endif
#if (TFAT_SERIAL_FLASH_DRIVE_NUM > 0)
    if (TFAT_CTRL_SERIAL_FLASH == dev_type)
    {
        TFAT_DRIVE_FLASH_ALLOC |= (0x01 << tfat_drv);
    }
    else
    {
        TFAT_DRIVE_FLASH_ALLOC &= ~(0x01 << tfat_drv);
    }
#endif

    drive_alloc_tbl[tfat_drv][0] = dev_type;
    drive_alloc_tbl[tfat_drv][1] = dev_drv_num;

    return RES_OK;
}
/******************************************************************************
 End of function drv_change_alloc
 *****************************************************************************/


/*******************************************************************************
End  of file
*******************************************************************************/
