#include <main.h>
//#include <Arduino.h>
#include <ESP32Time.h>

ESP32Time rtc;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  Wifi_Init();

  while (!WifiConnected());
  struct tm timeinfo;
  GetLocalTime(&timeinfo);
  rtc.setTimeStruct(timeinfo);

}

void loop() {
  
  // printLocalTime(epochTime);
  // if(WifiConnected())
  // {
  // struct tm timeinfo;
  //   if(GetLocalTime(&timeinfo))
  //   Serial.printf("%02d:%02d:%02d %02d/%02d/%04d\n",
  //               timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec,
  //               timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
  //   else Serial.println("Fail to obtain");
  // }
  struct tm timeinfo = rtc.getTimeStruct();
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  delay(1000); // Update time every minute
}
