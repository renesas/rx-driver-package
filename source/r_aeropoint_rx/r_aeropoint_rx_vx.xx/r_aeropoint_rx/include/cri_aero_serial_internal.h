/***********************************************************************************************************************
 * File Name    : cri_aero_serial_internal.h
 * Version      : 0.01
 * Creation Date: 2020-06-10
 ***********************************************************************************************************************/

#ifndef CRI_AERO_SERIAL_INTERNAL_H
#define CRI_AERO_SERIAL_INTERNAL_H


#include "cri_aero_serial.h"




void criAeroSerial_CallLogCallback(CriUint32 Length, const CriChar8* pData);


void CriAeroSerial_Initialize_AIS(void);
void CriAeroSerial_Finalize_AIS(void);
void CriAeroSerial_Start_AIS(void);
void CriAeroSerial_Stop_AIS(void);

int criAeroSerial_SendData_AIS(CriUint32 Length, const CriUint8 *pData);
void criAeroSerial_UpDate_AIS(void);


void CriAeroSerial_LogNotifySlideNo(CriUint32 No);
void CriAeroSerial_LogNotifyButtonPress(CriUint32 Slide , CriUint32 Param);


void criAeroSerial_SendTestDemo(CriUint32 No);
void criAeroSerial_ParseAndRegist(CriUint32 Length, CriUint8 *pData);



#endif 
