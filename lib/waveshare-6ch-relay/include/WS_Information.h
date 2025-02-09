#ifndef _WS_INFORMATION_H_
#define _WS_INFORMATION_H_

#define WIFI_Enable           1  // Whether WIFI is enabled  .1:true    0:false
#define Extension_Enable      0  // Whether to extend the connection to external devices   1:Expansion device Modbus RTU Relay    0:No extend
#define RTC_Enable            0


// TODO: Replace with your wifi info
// Name and password of the WiFi access point
#define STASSID       "Waveshare-TL-WR886N_2L"
#define STAPSK        "waveshare0755"

// !!! Timing switch relay, this operation must be connected to the Pico-RTC-DS3231 with the correct time setting,
// RTC_Enable must be set to 1. If no change is required, set RTC_Enable to 0
/* Set in WS_RTC.h instead
#define RTC_OPEN_Time_Hour    18              // Time when the timing switch is enabled
#define RTC_OPEN_Time_Min     33              // Time when the timing switch is enabled
#define RTC_Closs_Time_Hour   18              // Time when the timing switch is off
#define RTC_Closs_Time_Min    34              // Time when the timing switch is off
*/

#endif // _WS_INFORMATION_H_