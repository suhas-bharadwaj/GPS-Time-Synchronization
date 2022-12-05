#include <TinyGPS++.h>
#include <RTClib.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <time.h>

#define IST_OFFSET 19800

RTC_DS3231 rtc;
TinyGPSPlus gps;

SoftwareSerial ss(4, 3);

bool isUpdated = false;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  ss.begin(9600);

  if (!rtc.begin())
  {
    Serial.println("RTC not found");
  }

  

  if(rtc.lostPower())
  {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //Set gps time here instead. // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  rtc.adjust(DateTime(2000, 01, 01, 0, 0, 0));

  rtc.disable32K(); //Probably not needed.
  Serial.println("Start");
  

}

void loop()
{
  DateTime rtcTime = rtc.now();

  if(rtc.lostPower())
  {
    rtc.adjust(DateTime(2000, 01, 01, 0, 0, 0));
  }
  
  if(!isUpdated)
  {
    while(ss.available() > 0)
    {
      if(gps.encode(ss.read())) //If there is data
      {
        Serial.println("GPS sentence received");
        if(gps.time.isValid() && gps.date.isValid() && ((rtcTime.second() <= gps.time.second())))
        {
            rtc.adjust(DateTime(gps.date.year(), gps.date.month(), gps.date.day(), gps.time.hour(), gps.time.minute() ,gps.time.second()));
            Serial.println("Updated");

        }
      }
    }
  }

  

  Serial.println(String(rtcTime.day()) + "/" + String(rtcTime.month()) + "/" + String(rtcTime.year()) + "\t\t" + String(rtcTime.hour()) + ":" + String(rtcTime.minute()) + ":" + String(rtcTime.second()) + "\tGps: " +String(gps.time.value()));

  delay(1000);
}
