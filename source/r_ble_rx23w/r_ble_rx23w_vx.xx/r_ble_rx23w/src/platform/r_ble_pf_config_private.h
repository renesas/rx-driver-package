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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_ble_pf_config_private.h
* Description  : Configures the BLE module.
* History      : DD.MM.YYYY Version Description           
*              : 23.08.2019 1.00    First Release
***********************************************************************************************************************/
#ifndef R_BLE_PF_CONFIG_PRIVATE_H
#define R_BLE_PF_CONFIG_PRIVATE_H

#include "r_ble_rx23w_config.h"

/****************************************************************************************
 Do not change the following values.
****************************************************************************************/

#if defined(__CCRX__)
#define ___BLE_CF_ADDR(block)        BLE_CF_ADDR_BLOCK ## block
#define __BLE_CF_ADDR(block)        ___BLE_CF_ADDR ## block
#elif defined(__ICCRX__)
#define ___BLE_CF_ADDR(block)       BLE_CF_ADDR_BLOCK(block)
#define __BLE_CF_ADDR(block)        ___BLE_CF_ADDR(block)
#endif /* defined(__CCRX__) */

#define _BLE_CF_ADDR(block)         __BLE_CF_ADDR(block) 
#define BLE_DEV_DATA_CF_ADDR        _BLE_CF_ADDR(BLE_CFG_DEV_DATA_CF_BLOCK)

#if defined(__CCRX__)
#define ___BLE_DF_ADDR(block)        BLE_DF_ADDR_BLOCK ## block
#define __BLE_DF_ADDR(block)        ___BLE_DF_ADDR ## block
#elif defined(__ICCRX__)
#define ___BLE_DF_ADDR(block)       BLE_DF_ADDR_BLOCK(block)
#define __BLE_DF_ADDR(block)        ___BLE_DF_ADDR(block)
#endif /* defined(__CCRX__) */

#define _BLE_DF_ADDR(block)         __BLE_DF_ADDR(block) 
#define BLE_DEV_DATA_DF_ADDR        _BLE_DF_ADDR(BLE_CFG_DEV_DATA_DF_BLOCK)
#define BLE_SECD_DATA_DF_ADDR       _BLE_DF_ADDR(BLE_CFG_SECD_DATA_DF_BLOCK)

#define BLE_CF_ADDR_BLOCK(x)        (0xFFF80000U + ((255-(x)) * 0x800))

#define BLE_CF_ADDR_BLOCK0          (0xFFFFF800U)
#define BLE_CF_ADDR_BLOCK1          (0xFFFFF000U)
#define BLE_CF_ADDR_BLOCK2          (0xFFFFE800U)
#define BLE_CF_ADDR_BLOCK3          (0xFFFFE000U)
#define BLE_CF_ADDR_BLOCK4          (0xFFFFD800U)
#define BLE_CF_ADDR_BLOCK5          (0xFFFFD000U)
#define BLE_CF_ADDR_BLOCK6          (0xFFFFC800U)
#define BLE_CF_ADDR_BLOCK7          (0xFFFFC000U)
#define BLE_CF_ADDR_BLOCK8          (0xFFFFB800U)
#define BLE_CF_ADDR_BLOCK9          (0xFFFFB000U)
#define BLE_CF_ADDR_BLOCK10         (0xFFFFA800U)
#define BLE_CF_ADDR_BLOCK11         (0xFFFFA000U)
#define BLE_CF_ADDR_BLOCK12         (0xFFFF9800U)
#define BLE_CF_ADDR_BLOCK13         (0xFFFF9000U)
#define BLE_CF_ADDR_BLOCK14         (0xFFFF8800U)
#define BLE_CF_ADDR_BLOCK15         (0xFFFF8000U)
#define BLE_CF_ADDR_BLOCK16         (0xFFFF7800U)
#define BLE_CF_ADDR_BLOCK17         (0xFFFF7000U)
#define BLE_CF_ADDR_BLOCK18         (0xFFFF6800U)
#define BLE_CF_ADDR_BLOCK19         (0xFFFF6000U)
#define BLE_CF_ADDR_BLOCK20         (0xFFFF5800U)
#define BLE_CF_ADDR_BLOCK21         (0xFFFF5000U)
#define BLE_CF_ADDR_BLOCK22         (0xFFFF4800U)
#define BLE_CF_ADDR_BLOCK23         (0xFFFF4000U)
#define BLE_CF_ADDR_BLOCK24         (0xFFFF3800U)
#define BLE_CF_ADDR_BLOCK25         (0xFFFF3000U)
#define BLE_CF_ADDR_BLOCK26         (0xFFFF2800U)
#define BLE_CF_ADDR_BLOCK27         (0xFFFF2000U)
#define BLE_CF_ADDR_BLOCK28         (0xFFFF1800U)
#define BLE_CF_ADDR_BLOCK29         (0xFFFF1000U)
#define BLE_CF_ADDR_BLOCK30         (0xFFFF0800U)
#define BLE_CF_ADDR_BLOCK31         (0xFFFF0000U)
#define BLE_CF_ADDR_BLOCK32         (0xFFFEF800U)
#define BLE_CF_ADDR_BLOCK33         (0xFFFEF000U)
#define BLE_CF_ADDR_BLOCK34         (0xFFFEE800U)
#define BLE_CF_ADDR_BLOCK35         (0xFFFEE000U)
#define BLE_CF_ADDR_BLOCK36         (0xFFFED800U)
#define BLE_CF_ADDR_BLOCK37         (0xFFFED000U)
#define BLE_CF_ADDR_BLOCK38         (0xFFFEC800U)
#define BLE_CF_ADDR_BLOCK39         (0xFFFEC000U)
#define BLE_CF_ADDR_BLOCK40         (0xFFFEB800U)
#define BLE_CF_ADDR_BLOCK41         (0xFFFEB000U)
#define BLE_CF_ADDR_BLOCK42         (0xFFFEA800U)
#define BLE_CF_ADDR_BLOCK43         (0xFFFEA000U)
#define BLE_CF_ADDR_BLOCK44         (0xFFFE9800U)
#define BLE_CF_ADDR_BLOCK45         (0xFFFE9000U)
#define BLE_CF_ADDR_BLOCK46         (0xFFFE8800U)
#define BLE_CF_ADDR_BLOCK47         (0xFFFE8000U)
#define BLE_CF_ADDR_BLOCK48         (0xFFFE7800U)
#define BLE_CF_ADDR_BLOCK49         (0xFFFE7000U)
#define BLE_CF_ADDR_BLOCK50         (0xFFFE6800U)
#define BLE_CF_ADDR_BLOCK51         (0xFFFE6000U)
#define BLE_CF_ADDR_BLOCK52         (0xFFFE5800U)
#define BLE_CF_ADDR_BLOCK53         (0xFFFE5000U)
#define BLE_CF_ADDR_BLOCK54         (0xFFFE4800U)
#define BLE_CF_ADDR_BLOCK55         (0xFFFE4000U)
#define BLE_CF_ADDR_BLOCK56         (0xFFFE3800U)
#define BLE_CF_ADDR_BLOCK57         (0xFFFE3000U)
#define BLE_CF_ADDR_BLOCK58         (0xFFFE2800U)
#define BLE_CF_ADDR_BLOCK59         (0xFFFE2000U)
#define BLE_CF_ADDR_BLOCK60         (0xFFFE1800U)
#define BLE_CF_ADDR_BLOCK61         (0xFFFE1000U)
#define BLE_CF_ADDR_BLOCK62         (0xFFFE0800U)
#define BLE_CF_ADDR_BLOCK63         (0xFFFE0000U)
#define BLE_CF_ADDR_BLOCK64         (0xFFFDF800U)
#define BLE_CF_ADDR_BLOCK65         (0xFFFDF000U)
#define BLE_CF_ADDR_BLOCK66         (0xFFFDE800U)
#define BLE_CF_ADDR_BLOCK67         (0xFFFDE000U)
#define BLE_CF_ADDR_BLOCK68         (0xFFFDD800U)
#define BLE_CF_ADDR_BLOCK69         (0xFFFDD000U)
#define BLE_CF_ADDR_BLOCK70         (0xFFFDC800U)
#define BLE_CF_ADDR_BLOCK71         (0xFFFDC000U)
#define BLE_CF_ADDR_BLOCK72         (0xFFFDB800U)
#define BLE_CF_ADDR_BLOCK73         (0xFFFDB000U)
#define BLE_CF_ADDR_BLOCK74         (0xFFFDA800U)
#define BLE_CF_ADDR_BLOCK75         (0xFFFDA000U)
#define BLE_CF_ADDR_BLOCK76         (0xFFFD9800U)
#define BLE_CF_ADDR_BLOCK77         (0xFFFD9000U)
#define BLE_CF_ADDR_BLOCK78         (0xFFFD8800U)
#define BLE_CF_ADDR_BLOCK79         (0xFFFD8000U)
#define BLE_CF_ADDR_BLOCK80         (0xFFFD7800U)
#define BLE_CF_ADDR_BLOCK81         (0xFFFD7000U)
#define BLE_CF_ADDR_BLOCK82         (0xFFFD6800U)
#define BLE_CF_ADDR_BLOCK83         (0xFFFD6000U)
#define BLE_CF_ADDR_BLOCK84         (0xFFFD5800U)
#define BLE_CF_ADDR_BLOCK85         (0xFFFD5000U)
#define BLE_CF_ADDR_BLOCK86         (0xFFFD4800U)
#define BLE_CF_ADDR_BLOCK87         (0xFFFD4000U)
#define BLE_CF_ADDR_BLOCK88         (0xFFFD3800U)
#define BLE_CF_ADDR_BLOCK89         (0xFFFD3000U)
#define BLE_CF_ADDR_BLOCK90         (0xFFFD2800U)
#define BLE_CF_ADDR_BLOCK91         (0xFFFD2000U)
#define BLE_CF_ADDR_BLOCK92         (0xFFFD1800U)
#define BLE_CF_ADDR_BLOCK93         (0xFFFD1000U)
#define BLE_CF_ADDR_BLOCK94         (0xFFFD0800U)
#define BLE_CF_ADDR_BLOCK95         (0xFFFD0000U)
#define BLE_CF_ADDR_BLOCK96         (0xFFFCF800U)
#define BLE_CF_ADDR_BLOCK97         (0xFFFCF000U)
#define BLE_CF_ADDR_BLOCK98         (0xFFFCE800U)
#define BLE_CF_ADDR_BLOCK99         (0xFFFCE000U)
#define BLE_CF_ADDR_BLOCK100        (0xFFFCD800U)
#define BLE_CF_ADDR_BLOCK101        (0xFFFCD000U)
#define BLE_CF_ADDR_BLOCK102        (0xFFFCC800U)
#define BLE_CF_ADDR_BLOCK103        (0xFFFCC000U)
#define BLE_CF_ADDR_BLOCK104        (0xFFFCB800U)
#define BLE_CF_ADDR_BLOCK105        (0xFFFCB000U)
#define BLE_CF_ADDR_BLOCK106        (0xFFFCA800U)
#define BLE_CF_ADDR_BLOCK107        (0xFFFCA000U)
#define BLE_CF_ADDR_BLOCK108        (0xFFFC9800U)
#define BLE_CF_ADDR_BLOCK109        (0xFFFC9000U)
#define BLE_CF_ADDR_BLOCK110        (0xFFFC8800U)
#define BLE_CF_ADDR_BLOCK111        (0xFFFC8000U)
#define BLE_CF_ADDR_BLOCK112        (0xFFFC7800U)
#define BLE_CF_ADDR_BLOCK113        (0xFFFC7000U)
#define BLE_CF_ADDR_BLOCK114        (0xFFFC6800U)
#define BLE_CF_ADDR_BLOCK115        (0xFFFC6000U)
#define BLE_CF_ADDR_BLOCK116        (0xFFFC5800U)
#define BLE_CF_ADDR_BLOCK117        (0xFFFC5000U)
#define BLE_CF_ADDR_BLOCK118        (0xFFFC4800U)
#define BLE_CF_ADDR_BLOCK119        (0xFFFC4000U)
#define BLE_CF_ADDR_BLOCK120        (0xFFFC3800U)
#define BLE_CF_ADDR_BLOCK121        (0xFFFC3000U)
#define BLE_CF_ADDR_BLOCK122        (0xFFFC2800U)
#define BLE_CF_ADDR_BLOCK123        (0xFFFC2000U)
#define BLE_CF_ADDR_BLOCK124        (0xFFFC1800U)
#define BLE_CF_ADDR_BLOCK125        (0xFFFC1000U)
#define BLE_CF_ADDR_BLOCK126        (0xFFFC0800U)
#define BLE_CF_ADDR_BLOCK127        (0xFFFC0000U)
#define BLE_CF_ADDR_BLOCK128        (0xFFFBF800U)
#define BLE_CF_ADDR_BLOCK129        (0xFFFBF000U)
#define BLE_CF_ADDR_BLOCK130        (0xFFFBE800U)
#define BLE_CF_ADDR_BLOCK131        (0xFFFBE000U)
#define BLE_CF_ADDR_BLOCK132        (0xFFFBD800U)
#define BLE_CF_ADDR_BLOCK133        (0xFFFBD000U)
#define BLE_CF_ADDR_BLOCK134        (0xFFFBC800U)
#define BLE_CF_ADDR_BLOCK135        (0xFFFBC000U)
#define BLE_CF_ADDR_BLOCK136        (0xFFFBB800U)
#define BLE_CF_ADDR_BLOCK137        (0xFFFBB000U)
#define BLE_CF_ADDR_BLOCK138        (0xFFFBA800U)
#define BLE_CF_ADDR_BLOCK139        (0xFFFBA000U)
#define BLE_CF_ADDR_BLOCK140        (0xFFFB9800U)
#define BLE_CF_ADDR_BLOCK141        (0xFFFB9000U)
#define BLE_CF_ADDR_BLOCK142        (0xFFFB8800U)
#define BLE_CF_ADDR_BLOCK143        (0xFFFB8000U)
#define BLE_CF_ADDR_BLOCK144        (0xFFFB7800U)
#define BLE_CF_ADDR_BLOCK145        (0xFFFB7000U)
#define BLE_CF_ADDR_BLOCK146        (0xFFFB6800U)
#define BLE_CF_ADDR_BLOCK147        (0xFFFB6000U)
#define BLE_CF_ADDR_BLOCK148        (0xFFFB5800U)
#define BLE_CF_ADDR_BLOCK149        (0xFFFB5000U)
#define BLE_CF_ADDR_BLOCK150        (0xFFFB4800U)
#define BLE_CF_ADDR_BLOCK151        (0xFFFB4000U)
#define BLE_CF_ADDR_BLOCK152        (0xFFFB3800U)
#define BLE_CF_ADDR_BLOCK153        (0xFFFB3000U)
#define BLE_CF_ADDR_BLOCK154        (0xFFFB2800U)
#define BLE_CF_ADDR_BLOCK155        (0xFFFB2000U)
#define BLE_CF_ADDR_BLOCK156        (0xFFFB1800U)
#define BLE_CF_ADDR_BLOCK157        (0xFFFB1000U)
#define BLE_CF_ADDR_BLOCK158        (0xFFFB0800U)
#define BLE_CF_ADDR_BLOCK159        (0xFFFB0000U)
#define BLE_CF_ADDR_BLOCK160        (0xFFFAF800U)
#define BLE_CF_ADDR_BLOCK161        (0xFFFAF000U)
#define BLE_CF_ADDR_BLOCK162        (0xFFFAE800U)
#define BLE_CF_ADDR_BLOCK163        (0xFFFAE000U)
#define BLE_CF_ADDR_BLOCK164        (0xFFFAD800U)
#define BLE_CF_ADDR_BLOCK165        (0xFFFAD000U)
#define BLE_CF_ADDR_BLOCK166        (0xFFFAC800U)
#define BLE_CF_ADDR_BLOCK167        (0xFFFAC000U)
#define BLE_CF_ADDR_BLOCK168        (0xFFFAB800U)
#define BLE_CF_ADDR_BLOCK169        (0xFFFAB000U)
#define BLE_CF_ADDR_BLOCK170        (0xFFFAA800U)
#define BLE_CF_ADDR_BLOCK171        (0xFFFAA000U)
#define BLE_CF_ADDR_BLOCK172        (0xFFFA9800U)
#define BLE_CF_ADDR_BLOCK173        (0xFFFA9000U)
#define BLE_CF_ADDR_BLOCK174        (0xFFFA8800U)
#define BLE_CF_ADDR_BLOCK175        (0xFFFA8000U)
#define BLE_CF_ADDR_BLOCK176        (0xFFFA7800U)
#define BLE_CF_ADDR_BLOCK177        (0xFFFA7000U)
#define BLE_CF_ADDR_BLOCK178        (0xFFFA6800U)
#define BLE_CF_ADDR_BLOCK179        (0xFFFA6000U)
#define BLE_CF_ADDR_BLOCK180        (0xFFFA5800U)
#define BLE_CF_ADDR_BLOCK181        (0xFFFA5000U)
#define BLE_CF_ADDR_BLOCK182        (0xFFFA4800U)
#define BLE_CF_ADDR_BLOCK183        (0xFFFA4000U)
#define BLE_CF_ADDR_BLOCK184        (0xFFFA3800U)
#define BLE_CF_ADDR_BLOCK185        (0xFFFA3000U)
#define BLE_CF_ADDR_BLOCK186        (0xFFFA2800U)
#define BLE_CF_ADDR_BLOCK187        (0xFFFA2000U)
#define BLE_CF_ADDR_BLOCK188        (0xFFFA1800U)
#define BLE_CF_ADDR_BLOCK189        (0xFFFA1000U)
#define BLE_CF_ADDR_BLOCK190        (0xFFFA0800U)
#define BLE_CF_ADDR_BLOCK191        (0xFFFA0000U)
#define BLE_CF_ADDR_BLOCK192        (0xFFF9F800U)
#define BLE_CF_ADDR_BLOCK193        (0xFFF9F000U)
#define BLE_CF_ADDR_BLOCK194        (0xFFF9E800U)
#define BLE_CF_ADDR_BLOCK195        (0xFFF9E000U)
#define BLE_CF_ADDR_BLOCK196        (0xFFF9D800U)
#define BLE_CF_ADDR_BLOCK197        (0xFFF9D000U)
#define BLE_CF_ADDR_BLOCK198        (0xFFF9C800U)
#define BLE_CF_ADDR_BLOCK199        (0xFFF9C000U)
#define BLE_CF_ADDR_BLOCK200        (0xFFF9B800U)
#define BLE_CF_ADDR_BLOCK201        (0xFFF9B000U)
#define BLE_CF_ADDR_BLOCK202        (0xFFF9A800U)
#define BLE_CF_ADDR_BLOCK203        (0xFFF9A000U)
#define BLE_CF_ADDR_BLOCK204        (0xFFF99800U)
#define BLE_CF_ADDR_BLOCK205        (0xFFF99000U)
#define BLE_CF_ADDR_BLOCK206        (0xFFF98800U)
#define BLE_CF_ADDR_BLOCK207        (0xFFF98000U)
#define BLE_CF_ADDR_BLOCK208        (0xFFF97800U)
#define BLE_CF_ADDR_BLOCK209        (0xFFF97000U)
#define BLE_CF_ADDR_BLOCK210        (0xFFF96800U)
#define BLE_CF_ADDR_BLOCK211        (0xFFF96000U)
#define BLE_CF_ADDR_BLOCK212        (0xFFF95800U)
#define BLE_CF_ADDR_BLOCK213        (0xFFF95000U)
#define BLE_CF_ADDR_BLOCK214        (0xFFF94800U)
#define BLE_CF_ADDR_BLOCK215        (0xFFF94000U)
#define BLE_CF_ADDR_BLOCK216        (0xFFF93800U)
#define BLE_CF_ADDR_BLOCK217        (0xFFF93000U)
#define BLE_CF_ADDR_BLOCK218        (0xFFF92800U)
#define BLE_CF_ADDR_BLOCK219        (0xFFF92000U)
#define BLE_CF_ADDR_BLOCK220        (0xFFF91800U)
#define BLE_CF_ADDR_BLOCK221        (0xFFF91000U)
#define BLE_CF_ADDR_BLOCK222        (0xFFF90800U)
#define BLE_CF_ADDR_BLOCK223        (0xFFF90000U)
#define BLE_CF_ADDR_BLOCK224        (0xFFF8F800U)
#define BLE_CF_ADDR_BLOCK225        (0xFFF8F000U)
#define BLE_CF_ADDR_BLOCK226        (0xFFF8E800U)
#define BLE_CF_ADDR_BLOCK227        (0xFFF8E000U)
#define BLE_CF_ADDR_BLOCK228        (0xFFF8D800U)
#define BLE_CF_ADDR_BLOCK229        (0xFFF8D000U)
#define BLE_CF_ADDR_BLOCK230        (0xFFF8C800U)
#define BLE_CF_ADDR_BLOCK231        (0xFFF8C000U)
#define BLE_CF_ADDR_BLOCK232        (0xFFF8B800U)
#define BLE_CF_ADDR_BLOCK233        (0xFFF8B000U)
#define BLE_CF_ADDR_BLOCK234        (0xFFF8A800U)
#define BLE_CF_ADDR_BLOCK235        (0xFFF8A000U)
#define BLE_CF_ADDR_BLOCK236        (0xFFF89800U)
#define BLE_CF_ADDR_BLOCK237        (0xFFF89000U)
#define BLE_CF_ADDR_BLOCK238        (0xFFF88800U)
#define BLE_CF_ADDR_BLOCK239        (0xFFF88000U)
#define BLE_CF_ADDR_BLOCK240        (0xFFF87800U)
#define BLE_CF_ADDR_BLOCK241        (0xFFF87000U)
#define BLE_CF_ADDR_BLOCK242        (0xFFF86800U)
#define BLE_CF_ADDR_BLOCK243        (0xFFF86000U)
#define BLE_CF_ADDR_BLOCK244        (0xFFF85800U)
#define BLE_CF_ADDR_BLOCK245        (0xFFF85000U)
#define BLE_CF_ADDR_BLOCK246        (0xFFF84800U)
#define BLE_CF_ADDR_BLOCK247        (0xFFF84000U)
#define BLE_CF_ADDR_BLOCK248        (0xFFF83800U)
#define BLE_CF_ADDR_BLOCK249        (0xFFF83000U)
#define BLE_CF_ADDR_BLOCK250        (0xFFF82800U)
#define BLE_CF_ADDR_BLOCK251        (0xFFF82000U)
#define BLE_CF_ADDR_BLOCK252        (0xFFF81800U)
#define BLE_CF_ADDR_BLOCK253        (0xFFF81000U)
#define BLE_CF_ADDR_BLOCK254        (0xFFF80800U)
#define BLE_CF_ADDR_BLOCK255        (0xFFF80000U)

#define BLE_DF_ADDR_BLOCK(x)        (0x00100000U + ((x) * 0x400))

#define BLE_DF_ADDR_BLOCK0          (0x00100000U)
#define BLE_DF_ADDR_BLOCK1          (0x00100400U)
#define BLE_DF_ADDR_BLOCK2          (0x00100800U)
#define BLE_DF_ADDR_BLOCK3          (0x00100C00U)
#define BLE_DF_ADDR_BLOCK4          (0x00101000U)
#define BLE_DF_ADDR_BLOCK5          (0x00101400U)
#define BLE_DF_ADDR_BLOCK6          (0x00101800U)
#define BLE_DF_ADDR_BLOCK7          (0x00101C00U)

/**** RF event notify function pointer ****/
#define BLE_EVENT_TYPE_CONN               (0x0000U)
#define BLE_EVENT_TYPE_ADV                (0x0001U)
#define BLE_EVENT_TYPE_SCAN               (0x0002U)
#define BLE_EVENT_TYPE_INITIATOR          (0x0003U)

#define BLE_EVENT_NOTIFY_CONN_START_POS   (0)
#define BLE_EVENT_NOTIFY_ADV_START_POS    (1)
#define BLE_EVENT_NOTIFY_SCAN_START_POS   (2)
#define BLE_EVENT_NOTIFY_INIT_START_POS   (3)
#define BLE_EVENT_NOTIFY_CONN_CLOSE_POS   (4)
#define BLE_EVENT_NOTIFY_ADV_CLOSE_POS    (5)
#define BLE_EVENT_NOTIFY_SCAN_CLOSE_POS   (6)
#define BLE_EVENT_NOTIFY_INIT_CLOSE_POS   (7)
#define BLE_EVENT_NOTIFY_DS_START_POS     (8)
#define BLE_EVENT_NOTIFY_DS_CLOSE_POS     (9)

#define BLE_EVENT_TYPE_RF_DS_START        (0x0U)
#define BLE_EVENT_TYPE_RF_DS_CLOSE        (0x1U)

#define BLE_EVENT_NOTIFY_CONN_START_BIT   (0x1U << BLE_EVENT_NOTIFY_CONN_START_POS)
#define BLE_EVENT_NOTIFY_ADV_START_BIT    (0x1U << BLE_EVENT_NOTIFY_ADV_START_POS)
#define BLE_EVENT_NOTIFY_SCAN_START_BIT   (0x1U << BLE_EVENT_NOTIFY_SCAN_START_POS)
#define BLE_EVENT_NOTIFY_INIT_START_BIT   (0x1U << BLE_EVENT_NOTIFY_INIT_START_POS)

#define BLE_EVENT_NOTIFY_CONN_CLOSE_BIT   (0x1U << BLE_EVENT_NOTIFY_CONN_CLOSE_POS)
#define BLE_EVENT_NOTIFY_ADV_CLOSE_BIT    (0x1U << BLE_EVENT_NOTIFY_ADV_CLOSE_POS)
#define BLE_EVENT_NOTIFY_SCAN_CLOSE_BIT   (0x1U << BLE_EVENT_NOTIFY_SCAN_CLOSE_POS)
#define BLE_EVENT_NOTIFY_INIT_CLOSE_BIT   (0x1U << BLE_EVENT_NOTIFY_INIT_CLOSE_POS)

#define BLE_EVENT_NOTIFY_DS_START_BIT     (0x1U << BLE_EVENT_NOTIFY_DS_START_POS)
#define BLE_EVENT_NOTIFY_DS_CLOSE_BIT     (0x1U << BLE_EVENT_NOTIFY_DS_CLOSE_POS)

#define BLE_EVENT_NOTIFY_START_MASK   (   \
    (BLE_EVENT_NOTIFY_CONN_START_BIT) |   \
    (BLE_EVENT_NOTIFY_ADV_START_BIT)  |   \
    (BLE_EVENT_NOTIFY_SCAN_START_BIT) |   \
    (BLE_EVENT_NOTIFY_INIT_START_BIT) |   \
    (0x0U) )

#define BLE_EVENT_NOTIFY_CLOSE_MASK   (   \
    (BLE_EVENT_NOTIFY_CONN_CLOSE_BIT) |   \
    (BLE_EVENT_NOTIFY_ADV_CLOSE_BIT)  |   \
    (BLE_EVENT_NOTIFY_SCAN_CLOSE_BIT) |   \
    (BLE_EVENT_NOTIFY_INIT_CLOSE_BIT) |   \
    (0x0U) )

#define BLE_EVENT_NOTIFY_DS_MASK      (   \
    (BLE_EVENT_NOTIFY_DS_START_BIT)   |   \
    (BLE_EVENT_NOTIFY_DS_CLOSE_BIT)   |   \
    (0x0U) )

#define BLE_EVENT_NOTIFY_ENABLE_VAL   (   \
    ((BLE_CFG_EVENT_NOTIFY_CONN_START & 0x1U) << BLE_EVENT_NOTIFY_CONN_START_POS)  | \
    ((BLE_CFG_EVENT_NOTIFY_CONN_CLOSE & 0x1U) << BLE_EVENT_NOTIFY_CONN_CLOSE_POS)  | \
    ((BLE_CFG_EVENT_NOTIFY_ADV_START  & 0x1U) << BLE_EVENT_NOTIFY_ADV_START_POS)   | \
    ((BLE_CFG_EVENT_NOTIFY_ADV_CLOSE  & 0x1U) << BLE_EVENT_NOTIFY_ADV_CLOSE_POS)   | \
    ((BLE_CFG_EVENT_NOTIFY_SCAN_START & 0x1U) << BLE_EVENT_NOTIFY_SCAN_START_POS)  | \
    ((BLE_CFG_EVENT_NOTIFY_SCAN_CLOSE & 0x1U) << BLE_EVENT_NOTIFY_SCAN_CLOSE_POS)  | \
    ((BLE_CFG_EVENT_NOTIFY_INIT_START & 0x1U) << BLE_EVENT_NOTIFY_INIT_START_POS)  | \
    ((BLE_CFG_EVENT_NOTIFY_INIT_CLOSE & 0x1U) << BLE_EVENT_NOTIFY_INIT_CLOSE_POS)  | \
    ((BLE_CFG_EVENT_NOTIFY_DS_START   & 0x1U) << BLE_EVENT_NOTIFY_DS_START_POS)    | \
    ((BLE_CFG_EVENT_NOTIFY_DS_WAKEUP  & 0x1U) << BLE_EVENT_NOTIFY_DS_CLOSE_POS)    | \
    (0x0) )

#define BLE_HOST_TBL_NUM              8

/******************************************************************************************************************//**
 * @typedef ble_mcu_clock_change_cb_t
 * @brief   ble_mcu_clock_change_cb_t is the callback function type to use CLKOUT_RF as the MCU main clock source.
 * @param none
 * @return none
 **********************************************************************************************************************/
typedef void (*ble_mcu_clock_change_cb_t)(void);

/******************************************************************************************************************//**
 * @typedef ble_rf_notify_cb_t
 * @brief   ble_rf_notify_cb_t is the RF event notify callback function type.
 * @param[in] uint32_t     The information of RF event notification.
 * @return none
 **********************************************************************************************************************/
typedef void (*ble_rf_notify_cb_t)(uint32_t);

/******************************************************************************************************************//**
 *  @struct st_ble_rf_notify_t
 *  @brief  This structure is RF event notify management.
 **********************************************************************************************************************/
typedef struct {
    /**
     *  @brief Set enable/disable of each RF event notification
     *  @details 
     *      Bit0  Notify Connection event start(0:Disable/1:Enable)\n
     *      Bit1  Notify Advertising event start(0:Disable/1:Enable)\n
     *      Bit2  Notify Scanning event start(0:Disable/1:Enable)\n
     *      Bit3  Notify Initiating event start(0:Disable/1:Enable)\n
     *      Bit4  Notify Connection event close(0:Disable/1:Enable)\n
     *      Bit5  Notify Advertising event close(0:Disable/1:Enable)\n
     *      Bit6  Notify Scanning event close(0:Disable/1:Enable)\n
     *      Bit7  Notify Initiating event close(0:Disable/1:Enable)\n
     *      Bit8  Notify RF_DEEP_SLEEP event start(0:Disable/1:Enable)\n
     *      Bit9  Notify RF_DEEP_SLEEP event close(0:Disable/1:Enable)\n
     *      Other Bit: Reserved for future use.\n
     */
    uint32_t            enable;
    /**
     *  @brief Set callback function pointer for RF event start
     */
    ble_rf_notify_cb_t  start_cb;
    /**
     *  @brief Set callback function pointer for RF event close
     */
    ble_rf_notify_cb_t  close_cb;
    /**
     *  @brief Set callback function pointer for RF_DEEP_SLEEP
     */
    ble_rf_notify_cb_t  dsleep_cb;
} st_ble_rf_notify_t;

/* prototype */
void r_ble_rf_notify_event_start(uint32_t param);
void r_ble_rf_notify_event_close(uint32_t param);
void r_ble_rf_notify_deep_sleep(uint32_t param);

#endif /* R_BLE_PF_CONFIG_PRIVATE_H */
