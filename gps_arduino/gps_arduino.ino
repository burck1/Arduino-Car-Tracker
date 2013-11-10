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
  float my_lat;
  float my_lng;
  unsigned int my_date;
  unsigned int my_time;
};

SEND_DATA_STRUCTURE mydata;

void setup() {
  Serial.begin(9600);
  
  // Used to communicate with GPS Module
  ss.begin(GPSBaud);

  mydata.my_lat = 40.114119;
  mydata.my_lng = -88.225134;
  mydata.my_date = 11111;
  mydata.my_time = 11111;

  Wire.begin();
  ET.begin(details(mydata), &Wire);
  
  // Use LED for debugging
  pinMode(13, OUTPUT);
}

void loop() {
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }
  
  if (gps.location.isUpdated()) {
    if (gps.location.isValid()) {
      mydata.my_lat = gps.location.lat();
      mydata.my_lng = gps.location.lng();
    }
    if (gps.date.isValid()) {
      mydata.my_date = gps.date.value();
    }
    if (gps.time.isValid()) {
      mydata.my_time = gps.time.value();
    }
  }
  
  Serial.print(mydata.my_lat,6);
  Serial.print(",");
  Serial.print(mydata.my_lng,6);
  Serial.print(",");
  Serial.print(mydata.my_date);
  Serial.print(",");
  Serial.println(mydata.my_time);
  
  ET.sendData(I2C_ADDRESS_SLAVE);
  
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);

  // Send every 2 seconds
  delay(2 * 1000);
}
