#include <TinyGPS++.h>
#include <RTClib.h>
#include <SoftwareSerial.h>

RTC_DS3231 rtc; TinyGPSPlus gps; 
bool isUpdated = false;

SoftwareSerial ss(4, 3);

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  ss.begin(9600);

  if (!rtc.begin()){
    Serial.println("RTC not found");
  }

  isUpdated = false;

  if(rtc.lostPower()){
    isUpdated = false;
  }
  rtc.adjust(DateTime(2000, 01, 01, 0, 0, 0));
  rtc.disable32K(); 
  Serial.println("Start");
}

void loop(){
  DateTime rtcTime = rtc.now();

  if(rtc.lostPower()){
    isUpdated = false;
  }
  
  while(ss.available() > 0 && isUpdated == false)
  {
    if(gps.encode(ss.read()))
    {
      Serial.println("GPS sentence received");
      if(gps.time.isValid() && gps.date.isValid() && gps.date.day() != 0)
      {
          rtc.adjust(DateTime(gps.date.year(), gps.date.month(), gps.date.day(), gps.time.hour(), gps.time.minute() ,gps.time.second()));
          Serial.println("Updated");
          isUpdated = true;
      }
         
    }
  }
  
  Serial.println(String(rtcTime.day()) + "/" + String(rtcTime.month()) + "/" + String(rtcTime.year()) + "\t\t" + String(rtcTime.hour()) + ":" + String(rtcTime.minute()) + ":" + String(rtcTime.second()) + "\tGps: " +String(gps.time.value()));
  delay(1000);
}
