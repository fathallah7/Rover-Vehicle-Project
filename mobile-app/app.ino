/*
  -- car WATAR --

  This source code of graphical user interface
  has been generated automatically by RemoteXY editor.
  To compile this code using RemoteXY library 3.1.13 or later version
  download by link http://remotexy.com/en/library/
  To connect using RemoteXY mobile app by link http://remotexy.com/en/download/
    - for ANDROID 4.15.01 or later version;
    - for iOS 1.12.1 or later version;

  This source code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
// #define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__HARDSERIAL

// RemoteXY connection settings
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 9600

#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] = // 144 bytes
    {255, 4, 0, 27, 0, 137, 0, 19, 0, 0, 0, 0, 31, 1, 106, 200, 1, 1, 12, 0,
    66, 53, 20, 50, 15, 129, 37, 26, 66, 3, 20, 50, 15, 193, 204, 26, 66, 3, 53, 102,
    14, 129, 134, 26, 129, 2, 7, 60, 11, 64, 1, 116, 101, 109, 112, 101, 114, 97, 116, 117,
    114, 101, 0, 129, 5, 40, 59, 11, 64, 135, 65, 105, 114, 32, 104, 117, 109, 105, 100, 105,
    116, 121, 0, 67, 65, 8, 40, 10, 68, 1, 26, 11, 67, 65, 41, 40, 10, 68, 149, 26,
    11, 69, 82, 101, 20, 20, 0, 165, 129, 61, 104, 20, 12, 64, 151, 103, 97, 115, 0, 4,
    2, 70, 106, 17, 128, 2, 26, 5, 11, 115, 79, 79, 32, 95, 24, 31, 4, 2, 89, 106,
    17, 128, 2, 26};

// this structure defines all the variables and events of your control interface
struct
{

  // input variables
  int8_t slider_01;     // from 0 to 100
  int8_t joystick_01_x; // from -100 to 100
  int8_t joystick_01_y; // from -100 to 100
  int8_t slider_02;     // from 0 to 100

  // output variables
  int8_t level_temp_up;   // from 0 to 100
  int8_t level_temp_down; // from 0 to 100
  int8_t level_hum;       // from 0 to 100
  char text_temp[11];     // string UTF8 end zero
  char text_hum[11];      // string UTF8 end zero
  int16_t sound_01;       // =0 no sound, else ID of sound, =1001 for example, look sound list in app

  // other variable
  uint8_t connect_flag; // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

void setup()
{
  RemoteXY_Init();

  // TODO you setup code
}

void loop()
{
  RemoteXY_Handler();

  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay()
}