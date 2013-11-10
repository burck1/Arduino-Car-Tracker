#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <EasyTransferI2C.h>

#define I2C_ADDRESS_SLAVE 0x2

static const int TXPin = 3, RXPin = 4;
static const unsigned long GPSBaud = 9600;

TinyGPSPlus gps;
EasyTransferI2C ET;

SoftwareSerial ss(RXPin, TXPin);

struct SEND_DATA_STRUCTURE {
  double lat; // gps.location.lat() : Latitude in degrees
  double lng; // gps.location.lng() : Longitude in degrees
  double mph; // gps.speed.mph() : Speed in miles per hour
  unsigned int year; // gps.date.year() : Year (2000+)
  byte month; // gps.date.month() : Month (1-12)
  byte day; // gps.date.day() : Day (1-31)
  byte hour; // gps.time.hour() : Hour (0-23)
  byte minute; // gps.time.minute() : Minute (0-59)
  byte second; // gps.time.second() : Second (0-59)
  byte centisecond; // gps.time.centisecond() : 100ths of a second (0-99)
};

SEND_DATA_STRUCTURE mydata;

boolean initialized = false;

unsigned long Timer;
unsigned long send_interval = 2*1000; // ms

void setup() {
  Serial.begin(9600);
  
  // Used to communicate with GPS Module
  ss.begin(GPSBaud);

  Wire.begin();
  ET.begin(details(mydata), &Wire);
  
  // Initialize Timer
  Timer = millis();
}

void loop() {
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }
  if (millis() - Timer >= send_interval) {
    if (gps.location.isUpdated()) {
      initialized = true;
      if (gps.location.isValid()) {
        mydata.lat = gps.location.lat();
        mydata.lng = gps.location.lng();
      }
      if (gps.speed.isValid()) {
        mydata.mph = gps.speed.mph();
      }
      if (gps.date.isValid()) {
        mydata.year = gps.date.year();
        mydata.month = gps.date.month();
        mydata.day = gps.date.day();
      }
      if (gps.time.isValid()) {
        mydata.hour = gps.time.hour();
        mydata.minute = gps.time.minute();
        mydata.second = gps.time.second();
        mydata.centisecond = gps.time.centisecond();
      }
    }
    
    if (initialized) {
      Serial.print(mydata.lat,6);Serial.print(",");Serial.print(mydata.lng,6);Serial.print(" ");
      Serial.print(mydata.mph,2);Serial.print("mph ");
      Serial.print(mydata.month);Serial.print("/");Serial.print(mydata.day);Serial.print("/");Serial.print(mydata.year);Serial.print(" ");
      Serial.print(mydata.hour);Serial.print(":");Serial.print(mydata.minute);Serial.print(":");Serial.print(mydata.second);Serial.print(".");Serial.println(mydata.centisecond);
  
      ET.sendData(I2C_ADDRESS_SLAVE);
    }
    
    Timer = millis();
  }
}
