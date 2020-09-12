<#--
  Copyright(C) 2015 Renesas Electronics Corporation
  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
  This program must be used solely for the purpose for which it was furnished 
  by Renesas Electronics Corporation. No part of this program may be reproduced
  or disclosed to others, in any form, without the prior written permission of 
  Renesas Electronics Corporation.
-->
<#-- = DECLARE FUNCTION INFORMATION HERE =================== -->
<#assign Function_Base_Name = "R_ICU_PinSet">
<#assign Function_Description = "This function initializes pins for r_irq_rx module">
<#assign Function_Arg = "none">
<#assign Function_Ret = "none">
<#assign Version = 1.00>

<#-- = DECLARE FUNCTION CONTENT HERE ======================= -->
<#macro mainmacro peripherals channels>
<#assign Function_Name = "R_ICU_PinSet">
<#include "lib/functionheader.ftl">

void ${Function_Name}()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);
    <#list peripherals as peripheral>
        <#assign pins=peripheral.pins.pin>
        <#list pins as pin>
            <#assign pinUsed= "${pin.assignedPinName}">
            <#if pinUsed != "UNUSED">

    /* Set ${pin.pinName} pin */
                <#if headerInfo.device?contains("R5F524T")>
                    <#if pin.portNum != "5" && pin.portNum != "6">
    PORT${pin.portNum}.PMR.BIT.B${pin.pinBitNum} = 0U;
                    </#if>
                <#elseif headerInfo.device?contains("R5F524U")>
                    <#if pin.portNum != "5" && pin.portNum != "6">
    PORT${pin.portNum}.PMR.BIT.B${pin.pinBitNum} = 0U;
                    </#if>
                <#else>
    PORT${pin.portNum}.PMR.BIT.B${pin.pinBitNum} = 0U;
                </#if>
                <#if headerInfo.device?contains("R5F513T")>
                    <#if pin.portNum != "E" && pin.portNum != "2">
    PORT${pin.portNum}.PDR.BIT.B${pin.pinBitNum} = 0U;
                    </#if>
                <#else>
    PORT${pin.portNum}.PDR.BIT.B${pin.pinBitNum} = 0U;
                </#if>
    MPC.${pin.assignedPinName}PFS.BYTE = 0x${pin.pinMPC}U;
            </#if>
        </#list>
    </#list>

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}
</#macro>

<#macro mainheadermacro peripherals channels>
    <#list peripherals as peripheral>
        <#assign pins=peripheral.pins.pin>
        <#list pins as pin>
            <#assign pinUsed= "${pin.assignedPinName}">
            <#if pinUsed != "UNUSED">
#define IRQ_PORT_${pin.pinName}_PORT     ('${pin.portNum}')
#define IRQ_PORT_${pin.pinName}_BIT      ('${pin.pinBitNum}')
            <#else>
#define IRQ_PORT_${pin.pinName}_PORT     ('m')
#define IRQ_PORT_${pin.pinName}_BIT      ('n')
            </#if>
        </#list>
    </#list>

void R_ICU_PinSet();
</#macro>

<#-- = END OF FILE ========================================= -->