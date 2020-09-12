/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.

----------------------------------------------------------------------
File        : PIDConf.c
Purpose     : Touch panel configuration
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"

#include "platform.h"
#include "r_sci_iic_rx_if.h"
#include "r_sys_time_rx_if.h"
#include "r_gpio_rx_if.h"
#include "r_emwin_rx_config.h"


/*********************************************************************
*
*       Config
*
**********************************************************************
*/
#ifndef USE_MULTITOUCH
  #define USE_MULTITOUCH  EMWIN_USE_MULTITOUCH
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define SLAVE_ADDRESS        EMWIN_SLAVE_ADDRESS
#define MAX_NUM_TOUCHPOINTS  EMWIN_MAX_NUM_TOUCHPOINTS
#define MAX_NUM_IDS          EMWIN_MAX_NUM_IDS
#define NUM_CALIB_POINTS     EMWIN_NUM_CALIB_POINTS

#define XSIZE  EMWIN_XSIZE_PHYS
#define YSIZE  EMWIN_YSIZE_PHYS

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
/*********************************************************************
*
*       TOUCH_DATA
*
*  Holds information about coordinates and ID.
*/
typedef struct {
  U8 xHigh;       // Bit 6..7 - EventFlag, Bit 0..3 xHigh
  U8 xLow;
  U8 yHigh;       // Bit 4..7 - TouchID, Bit 0..3 yHigh
  U8 yLow;
  U8 ID;
} TOUCH_DATA;

/*********************************************************************
*
*       REPORT_DATA
*
*  Holds information about different touch points, mode, gesture and
*  number of points.
*/
typedef struct {
  U8         DeviceMode;
  U8         GestureID;
  U8         NumPoints;
} REPORT_DATA;

/*********************************************************************
*
*       POINT_DATA
*
*  Used by this module to calculate the different MT flags and events.
*/
typedef struct {
  U16 xPos;
  U16 yPos;
  U8  Id;
  U8  Flags;
} POINT_DATA;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static int _LayerIndex;

static sci_iic_info_t _Info;

static U8 _aBuffer[63] = {0};
static U8 _Busy;

#if (USE_MULTITOUCH == 1)
  static U8  _aActiveIds[MAX_NUM_TOUCHPOINTS];
#endif

/*********************************************************************
*
*       Local code
*
**********************************************************************
*/
/*********************************************************************
*
*       _cb_SCI_IIC_ch6
*/
#if (USE_MULTITOUCH == 0)
static void _cb_SCI_IIC(void) {
  sci_iic_mcu_status_t      iic_status;
  volatile sci_iic_return_t ret;
  int Temp;

  ret = R_SCI_IIC_GetStatus(&_Info, &iic_status);
  if ((ret == SCI_IIC_SUCCESS) && (iic_status.BIT.NACK == 1)) {
    static GUI_PID_STATE StatePID;
    static int           IsTouched;
    REPORT_DATA          Report;
    TOUCH_DATA           TouchPoint;
    U8                 * pBuffer;

    StatePID.Layer    = _LayerIndex;          // Set layer who should handle touch
    pBuffer           =  _aBuffer;
    Report.DeviceMode = *pBuffer++;           // Get device mode, 000b - Work Mode, 001b - Factory Mode
    Report. GestureID = *pBuffer++;           // GestureID:  0x10 Move UP
                                              //             0x14 Move Left
                                              //             0x18 Move Down
                                              //             0x1C Move Right
                                              //             0x48 Zoom In
                                              //             0x49 Zoom Out
                                              //             0x00 No Gesture
    Report.NumPoints  = *pBuffer++;           // Number of points
    TouchPoint.xHigh  = (*pBuffer++) & 0x0F;  // Get the upper 4 bits of the x position
    TouchPoint.xLow   = *pBuffer++;           // and the lower 8 bits
    TouchPoint.yHigh  = (*pBuffer++) & 0x0F;  // Get the upper 4 bits of the y position
    TouchPoint.yLow   = *pBuffer++;           // and the lower 8 bits
    //
    // We just handle one touch point, so we stop here.
    //
    // Check if we have a touch detected
    //
    if (Report.NumPoints) {
      IsTouched        = 1;  // Remember that we have a touch, needed for generating up events
      StatePID.Pressed = 1;  // State is pressed
      //
      // Shift bits for x- and y- coordinate to the correct position
      //
      StatePID.x       = ((TouchPoint.xHigh & 0x0F) << 8 | TouchPoint.xLow);
      StatePID.y       = ((TouchPoint.yHigh & 0x0F) << 8 | TouchPoint.yLow);

      if (LCD_GetMirrorX()) {
        StatePID.x = XSIZE - 1 - StatePID.x;
      }
      if (LCD_GetMirrorY()) {
        StatePID.y = YSIZE - 1 - StatePID.y;
      }
      if (LCD_GetSwapXY()) {
        Temp = StatePID.x;
        StatePID.x = StatePID.y;
        StatePID.y = Temp;
      }

      //
      // Pass touch data to emWin
      //
      GUI_TOUCH_StoreStateEx(&StatePID);
    } else {
      if (IsTouched) {         // If we had a touch,
        IsTouched        = 0;  // now we don't.
        StatePID.Pressed = 0;  // So, state is not pressed.
        //
        // Tell emWin
        //
        GUI_TOUCH_StoreStateEx(&StatePID);
      }
    }
    _Busy = 0;
  }
}

#else

/*********************************************************************
*
*       _ActiveIdFound
*
* Purpose:
*   Checks if the given Id is part of the currently active Ids
*/
static int _ActiveIdFound(U8 Id) {
  int i;
  U8 * pId;

  pId = _aActiveIds;
  i   = GUI_COUNTOF(_aActiveIds);
  do {
    if (*pId++ == Id) {
      return 1;
    }
  } while (--i);
  return 0;
}

/*********************************************************************
*
*       _StoreId
*
* Purpose:
*   Finds a free 'slot' for the given Id and puts it to the active Ids
*/
static int _StoreId(U8 Id) {
  int i;
  U8 * pId;

  pId = _aActiveIds;
  i   = GUI_COUNTOF(_aActiveIds);
  do {
    if (*pId == 0) {
      *pId = Id;
      return 0;
    }
    pId++;
  } while (--i);
  return 1;
}

/*********************************************************************
*
*       _CurrentIdFound
*
* Purpose:
*   Checks if the given Id is part of the given POINT_DATA array
*/
static int _CurrentIdFound(U8 Id, POINT_DATA * pPointData, int NumPoints) {
  POINT_DATA * pi;
  int i;

  for (pi = pPointData, i = 0; i < NumPoints; i++, pi++) {
    if (pi->Id == Id) {
      return 1;
    }
  }
  return 0;
}

/*********************************************************************
*
*       _CreateInput
*
* Purpose:
*   Fills the given GUI_MTOUCH_INPUT structure with the given coordinates
*   of the POINT_DATA data structure. In case of UP events there is no
*   data and only the given Id is used.
*/
static void _CreateInput(U16 Flags, U8 Id, POINT_DATA * pPointData, GUI_MTOUCH_INPUT * pInput) {
  if (pPointData) {
    pInput->x  = pPointData->xPos;
    pInput->y  = pPointData->yPos;
    pInput->Id = pPointData->Id;
  } else {
    pInput->Id = Id;
  }
  pInput->Flags = Flags;
}

/*********************************************************************
*
*       _CreateUpInputs
*
* Purpose:
*   Checks if the already existing active touch points exist in current
*   data. For each non existing active Id in the current data an UP event
*   is created.
*/
static void _CreateUpInputs(POINT_DATA * pPointData, int NumPoints, GUI_MTOUCH_INPUT ** ppInput, int * pNumInputs) {
  GUI_MTOUCH_INPUT * pInput;
  int i;
  U8 * pId;
  U8 Id;

  pInput = *ppInput;
  pId = _aActiveIds;
  i   = GUI_COUNTOF(_aActiveIds);
  do {
    Id = *pId;
    if (Id) {
      if (_CurrentIdFound(Id, pPointData, NumPoints) == 0) {
        _CreateInput(GUI_MTOUCH_FLAG_UP, Id, NULL, pInput);
        (*pNumInputs)++;
        pInput++;
        *pId = 0;
      }
    }
    pId++;
  } while (--i);
  *ppInput = pInput;
}

/*********************************************************************
*
*       _CreateMoveAndDownInputs
*
* Purpose:
*   Fills the given array of GUI_MTOUCH_INPUT structures with data of current
*   POINT_DATA array. If an item already exist a MOVE event is created,
*   otherwise a DOWM event.
*/
static void _CreateMoveAndDownInputs(POINT_DATA * pPointData, int NumPoints, GUI_MTOUCH_INPUT * pInput) {
  int i, Found;

  for (i = 0; i < NumPoints; i++, pPointData++, pInput++) {
    Found = _ActiveIdFound(pPointData->Id);
    pInput->x     = pPointData->xPos;
    pInput->y     = pPointData->yPos;
    pInput->Id    = pPointData->Id;
    if (Found) {
      pInput->Flags = GUI_MTOUCH_FLAG_MOVE;
    } else {
      pInput->Flags = GUI_MTOUCH_FLAG_DOWN;
      _StoreId(pPointData->Id);
    }
  }
}

/*********************************************************************
*
*       _cb_SCI_IIC_ch6
*/
static void _cb_SCI_IIC(void) {
  sci_iic_mcu_status_t      iic_status;
  volatile sci_iic_return_t ret;
  GUI_MTOUCH_INPUT * pInput;
  GUI_MTOUCH_EVENT   Event;
  GUI_MTOUCH_INPUT   aInput[MAX_NUM_TOUCHPOINTS];
  POINT_DATA         aPointData[MAX_NUM_TOUCHPOINTS];
  REPORT_DATA        Report;
  TOUCH_DATA         TouchPoint;
  int                NumInputs;
  int                i;
  U8                 NumPoints;
  int                xCoord;
  int                yCoord;
  U8               * pBuffer;

  ret = R_SCI_IIC_GetStatus(&_Info, &iic_status);
  if ((ret == SCI_IIC_SUCCESS) && (iic_status.BIT.NACK == 1)) {
    pBuffer           =  _aBuffer;
    Report.DeviceMode = *pBuffer++;           // Get device mode, 000b - Work Mode, 001b - Factory Mode
    Report. GestureID = *pBuffer++;           // GestureID:  0x10 Move UP
                                            //             0x14 Move Left
                                            //             0x18 Move Down
                                            //             0x1C Move Right
                                            //             0x48 Zoom In
                                            //             0x49 Zoom Out
                                            //             0x00 No Gesture
    Report.NumPoints  = *pBuffer++;           // Number of points
    NumPoints         =  Report.NumPoints;
    //
    // Reading point data is only required if there is a touch point
    //
    if (NumPoints) {
      //
      // Get coordinates and Ids from buffer
      //
      for (i = 0; i < NumPoints; i++) {
        TouchPoint.xHigh  = (*pBuffer++) & 0x0F;  // Get the upper 4 bits of the x position
        TouchPoint.xLow   =  *pBuffer++;          // and the lower 8 bits
        TouchPoint.ID     = (*pBuffer)   & 0xF0;  // Extract the touch point ID
        TouchPoint.yHigh  = (*pBuffer++) & 0x0F;  // Get the upper 4 bits of the y position
        TouchPoint.yLow   =  *pBuffer++;          // and the lower 8 bits
        //
        // Increment buffer twice since we have two dummy bytes
        //
        pBuffer++;
        pBuffer++;
        //
        // Calculate coordinate values
        //
        xCoord = ((TouchPoint.xHigh & 0x0F) << 8 | TouchPoint.xLow);
        yCoord = ((TouchPoint.yHigh & 0x0F) << 8 | TouchPoint.yLow);
        //
        // Add 1 to ID because TC counts from 0 and emWin can't handle an ID with 0
        //
        aPointData[i].Id   = TouchPoint.ID + 1;
        aPointData[i].xPos = xCoord;
        aPointData[i].yPos = yCoord;
      }
    }
    // Independent of NumPoints check if UP-inputs need to be generated
    //
    //
    pInput    = aInput;
    NumInputs = 0;
    _CreateUpInputs(aPointData, NumPoints, &pInput, &NumInputs);
    //
    // Create MOVE- and DOWN-inputs only for current points
    //
    if (NumPoints) {
      _CreateMoveAndDownInputs(aPointData, NumPoints, pInput);
      NumInputs += NumPoints;
    }
    //
    // If any input exists, store an event into emWin buffer
    //
    if (NumInputs) {
      Event.LayerIndex = _LayerIndex;
      Event.NumPoints  = NumInputs;
      GUI_MTOUCH_StoreEvent(&Event, aInput);
    }
    _Busy = 0;
  }
}

#endif

/*********************************************************************
*
*       _Exec
*/
static void _Exec(void) {
  //
  // Read all touch points to clear the buffer on TC side
  //
  static uint8_t slave_addr_eeprom[] = { SLAVE_ADDRESS };  // Slave address
  static uint8_t access_addr1[]      = { 0x00 };           // 1st data field
  volatile sci_iic_return_t ret;

  if (_Busy) {
    return;
  }
  //
  // Sets IIC Information
  //
  _Info.p_slv_adr    = slave_addr_eeprom;
  _Info.p_data1st    = access_addr1;
  _Info.p_data2nd    = _aBuffer;
  _Info.dev_sts      = SCI_IIC_NO_INIT;
  _Info.cnt1st       = sizeof(access_addr1);
  _Info.cnt2nd       = sizeof(_aBuffer);
  _Info.callbackfunc = &_cb_SCI_IIC;
  //
  // Start Master Receive
  //
  ret = R_SCI_IIC_MasterReceive(&_Info);
  if (ret == SCI_IIC_SUCCESS) {
    _Busy = 1;
  }
}

/*********************************************************************
*
*       _cbTimer
*/
static void _cbTimer(void * pData) {
  _Exec();
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       PID_X_SetLayerIndex
*/
void PID_X_SetLayerIndex(int LayerIndex) {
  _LayerIndex = LayerIndex;
}

/*********************************************************************
*
*       PID_X_Init
*/
void PID_X_Init(void) {
  U32 Channel;

  //
  // Reset touch ic
  //
  R_GPIO_PinDirectionSet(EMWIN_TOUCH_IC_RESET_PIN, GPIO_DIRECTION_OUTPUT);
  R_GPIO_PinWrite(EMWIN_TOUCH_IC_RESET_PIN, GPIO_LEVEL_LOW);
  R_BSP_SoftwareDelay(10, BSP_DELAY_MILLISECS);
  R_GPIO_PinWrite(EMWIN_TOUCH_IC_RESET_PIN, GPIO_LEVEL_HIGH);
  R_BSP_SoftwareDelay(300, BSP_DELAY_MILLISECS);
  //
  // Sets IIC Information (Ch6)
  //
  _Info.ch_no        = EMWIN_SCI_IIC_NUMBER;
  //
  // SCI open
  //
  if (R_SCI_IIC_Open(&_Info) != SCI_IIC_SUCCESS) {
    return;  // Error
  }
  //
  // Create timer for executing touch
  //
  R_SYS_TIME_Open();
  R_SYS_TIME_RegisterPeriodicCallback(_cbTimer, 2);
#if (USE_MULTITOUCH == 1)
  GUI_MTOUCH_Enable(1);
#endif
}

/*********************************************************************
*
*       Dummies, not used
*/
void GUI_TOUCH_X_ActivateX(void) {}
void GUI_TOUCH_X_ActivateY(void) {}
int  GUI_TOUCH_X_MeasureX(void) { return 0;}
int  GUI_TOUCH_X_MeasureY(void) { return 0;}

/*************************** End of file ****************************/

