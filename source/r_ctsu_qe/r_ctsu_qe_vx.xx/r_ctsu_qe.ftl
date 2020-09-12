<#--
  Copyright(C) 2018 Renesas Electronics Corporation
  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
  This program must be used solely for the purpose for which it was furnished 
  by Renesas Electronics Corporation. No part of this program may be reproduced
  or disclosed to others, in any form, without the prior written permission of 
  Renesas Electronics Corporation.
-->
<#-- = DECLARE FUNCTION INFORMATION HERE =================== -->
<#assign Function_Description = "This function initializes pins for r_ctsu_qe module">
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
<#assign Function_Name = "R_CTSU_PinSetInit">
<#include "lib/functionheader.ftl">
void R_CTSU_PinSetInit()
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
    </#list>

<#list pins as pin>
    <#assign pinUsed= "${pin.assignedPinName}">
    <#if pinUsed != "UNUSED">
    <#if pin.pinName == "TSCAP">
<#assign Function_Name = "R_Set_CTSU_TSCAP_Discharge">
/***********************************************************************************************************************
* Function Name: R_Set_CTSU_TSCAP_Discharge
* Description  : This function discharges TSCAP pin
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_Set_CTSU_TSCAP_Discharge()
{
    /* Set ${pin.pinName} pin */
    PORT${pin.portNum}.PMR.BIT.B${pin.pinBitNum} = 0U;
    PORT${pin.portNum}.PODR.BIT.B${pin.pinBitNum} = 0U;
    PORT${pin.portNum}.PDR.BIT.B${pin.pinBitNum} = 1U;
}

<#assign Function_Name = "R_Set_CTSU_TSCAP_Charge">
/***********************************************************************************************************************
* Function Name: R_Set_CTSU_TSCAP_Charge
* Description  : This function charges TSCAP pin
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_Set_CTSU_TSCAP_Charge()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set ${pin.pinName} pin */
    MPC.${pin.assignedPinName}PFS.BYTE = 0x${pin.pinMPC}U;
    PORT${pin.portNum}.PMR.BIT.B${pin.pinBitNum} = 1U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}
    </#if>
    </#if>
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
void R_CTSU_PinSetInit();
            <#list pins as pin>
                <#assign pinUsed= "${pin.assignedPinName}">
                <#if pinUsed != "UNUSED">
                    <#if pin.pinName == "TSCAP">
void R_Set_CTSU_TSCAP_Discharge();
void R_Set_CTSU_TSCAP_Charge();
                    </#if>
                </#if>
            </#list>
        </#if>
    </#list>
</#macro>

<#-- = END OF FILE ========================================= -->