<#--
  Copyright(C) 2015 Renesas Electronics Corporation
  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
  This program must be used solely for the purpose for which it was furnished 
  by Renesas Electronics Corporation. No part of this program may be reproduced
  or disclosed to others, in any form, without the prior written permission of 
  Renesas Electronics Corporation.
-->
<#-- = DECLARE FUNCTION INFORMATION HERE =================== -->
<#assign Function_Description = "This function initializes pins for r_mmcif_rx module">
<#assign Function_Arg = "none">
<#assign Function_Ret = "none">
<#assign Version = 1.00>

<#-- = DECLARE FUNCTION BODY CONTENT HERE ======================= -->

<#--
  The mainmacro is called by FIT Configurator plugin 
  Do not edit the macro header and argument 
  Arguments detail:
    peripherals: peripherals info from FIT module database
    channels: channels info from FIT module database
-->
<#macro mainmacro peripherals channels>
    <#list peripherals as peripheral>
        <#assign pins=peripheral.pins.pin>
<#assign Function_Name = "R_MMCIF_PinSetInit">
<#include "lib/functionheader.ftl">
void R_MMCIF_PinSetInit()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);
    <#list pins as pin>
        <#assign pinUsed= "${pin.assignedPinName}">
        <#if pinUsed != "UNUSED">

    /* Set ${pin.pinName} pin */
    PORT${pin.portNum}.PMR.BIT.B${pin.pinBitNum} = 0U;
    <#if pin.pinName == "MMC_CLK" || pin.pinName == "MMC_CMD" || pin.pinName == "MMC_D0" || pin.pinName == "MMC_D1" || pin.pinName == "MMC_D2" || pin.pinName == "MMC_D3" || pin.pinName == "MMC_D4" || pin.pinName == "MMC_D5" || pin.pinName == "MMC_D6" || pin.pinName == "MMC_D7">
        <#if headerInfo.device?contains("R5F565N") || headerInfo.device?contains("R5F5651") || headerInfo.device?contains("R5F566N") || headerInfo.device?contains("R5F572M") || headerInfo.device?contains("R5F572N")>
    PORT${pin.portNum}.DSCR.BIT.B${pin.pinBitNum} = 1U;
        <#elseif headerInfo.device?contains("R5F564M") || headerInfo.device?contains("R5F571M")>
            <#if pin.portNum == "C" || pin.portNum == "D" || pin.portNum == "E" >
    PORT${pin.portNum}.DSCR.BIT.B${pin.pinBitNum} = 1U;
            </#if>
        </#if>
    </#if>
    PORT${pin.portNum}.PCR.BIT.B${pin.pinBitNum} = 0U;
    <#if pin.pinName == "MMC_CD" >
    PORT${pin.portNum}.PDR.BIT.B${pin.pinBitNum} = 0U;
    MPC.${pin.assignedPinName}PFS.BYTE = 0x${pin.pinMPC}U;
    PORT${pin.portNum}.PMR.BIT.B${pin.pinBitNum} = 1U;
    <#elseif pin.pinName == "MMC_CLK" || pin.pinName == "MMC_CMD" || pin.pinName == "MMC_D0" || pin.pinName == "MMC_D1" || pin.pinName == "MMC_D2" || pin.pinName == "MMC_D3" || pin.pinName == "MMC_D4" || pin.pinName == "MMC_D5" || pin.pinName == "MMC_D6" || pin.pinName == "MMC_D7">
    PORT${pin.portNum}.PODR.BIT.B${pin.pinBitNum} = 0U;
    PORT${pin.portNum}.PDR.BIT.B${pin.pinBitNum} = 1U;
    MPC.${pin.assignedPinName}PFS.BYTE = 0x00U;
    </#if>
        </#if>
    </#list>

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}


<#assign Function_Name = "R_MMCIF_PinSetTransfer">
/***********************************************************************************************************************
* Function Name: R_MMCIF_PinSetTransfer
* Description  : This function enables MMCIF communication
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_MMCIF_PinSetTransfer()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);
    <#list pins as pin>
        <#assign pinUsed= "${pin.assignedPinName}">
        <#if pinUsed != "UNUSED">

    /* Set ${pin.pinName} pin */
    MPC.${pin.assignedPinName}PFS.BYTE = 0x${pin.pinMPC}U;
    PORT${pin.portNum}.PMR.BIT.B${pin.pinBitNum} = 1U;
        </#if>
    </#list>

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

<#assign Function_Name = "R_MMCIF_PinSetDetection">
/***********************************************************************************************************************
* Function Name: R_MMCIF_PinSetDetection
* Description  : This function disables MMCIF communication and enables card detection
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_MMCIF_PinSetDetection()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);
    <#list pins as pin>
        <#assign pinUsed= "${pin.assignedPinName}">
        <#if pinUsed != "UNUSED">

    /* Set ${pin.pinName} pin */
    <#if pin.pinName == "MMC_CD" > 
    MPC.${pin.assignedPinName}PFS.BYTE = 0x${pin.pinMPC}U;
    PORT${pin.portNum}.PMR.BIT.B${pin.pinBitNum} = 1U;
    <#else>
    PORT${pin.portNum}.PMR.BIT.B${pin.pinBitNum} = 0U;
    MPC.${pin.assignedPinName}PFS.BYTE = 0x00U;
    </#if>
        </#if>
    </#list>

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

<#assign Function_Name = "R_MMCIF_PinSetEnd">
/***********************************************************************************************************************
* Function Name: R_MMCIF_PinSetEnd
* Description  : This function ends pins for r_mmcif_rx module
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_MMCIF_PinSetEnd()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);
    <#list pins as pin>
        <#assign pinUsed= "${pin.assignedPinName}">
        <#if pinUsed != "UNUSED">

    /* Set ${pin.pinName} pin */
    PORT${pin.portNum}.PMR.BIT.B${pin.pinBitNum} = 0U;
    MPC.${pin.assignedPinName}PFS.BYTE = 0x00U;
        </#if>
    </#list>

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}
    </#list>
</#macro>

<#--
  The mainheadermacro is called by FIT Configurator plugin 
  Do not edit the macro header and argument 
  Arguments detail:
    peripherals: peripherals info from FIT module database
    channels: channels info from FIT module database
-->
<#macro mainheadermacro peripherals channels>
    <#list peripherals as peripheral>
        <#assign pins=peripheral.pins.pin>
        <#if pins?has_content>
void R_MMCIF_PinSetInit();
void R_MMCIF_PinSetTransfer();
void R_MMCIF_PinSetDetection();
void R_MMCIF_PinSetEnd();
        </#if>
    </#list>
</#macro>

<#-- = END OF FILE ========================================= -->