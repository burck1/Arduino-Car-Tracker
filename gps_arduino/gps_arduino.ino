//#include <TinyGPS++.h>
#include <Wire.h>
#include <EasyTransferI2C.h>

// stty -F /dev/ttyACM0 speed 1200 cs8 -cstopb -parenb

#define I2C_ADDRESS_MASTER 0x1
#define I2C_ADDRESS_SLAVE 0x2

//TinyGPSPlus gps;
EasyTransferI2C ET;

struct SEND_DATA_STRUCTURE {
  float my_lat;
  float my_lng;
  unsigned int my_date;
  unsigned int my_time;
};

SEND_DATA_STRUCTURE mydata;

void setup() {
  // Used to communicate with GPS Module
//  Serial1.begin(9600);
  Serial.begin(9600);

  mydata.my_lat = 0.0;
  mydata.my_lng = 0.0;
  mydata.my_date = 0;
  mydata.my_time = 0;

  // Used to communicate with other Arduino as slave sender
  Wire.begin(I2C_ADDRESS_SLAVE);
  Wire.onRequest(requestEvent);
  
  ET.begin(details(mydata), &Wire);
}

void loop() {
//  while (Serial1.available() > 0) {
//    gps.encode(Serial1.read());
//  }
  
//  if (gps.location.isUpdated()) {
//    mydata.my_lat = gps.location.lat();
//    mydata.my_lng = gps.location.lng();
//    mydata.my_date = gps.date.value();
//    mydata.my_time = gps.time.value();
//  }
  mydata.my_lat = 40.114119;
  mydata.my_lng = -88.225134;
  mydata.my_date = 110913;
  mydata.my_time = 03142731;
  
}

// function that executes whenever data is requested by master
void requestEvent() {
  Wire.write('A');
//  ET.sendData(I2C_ADDRESS_MASTER);
}

//void requestEvent() {
//  Wire.beginTransmission(I2C_ADDRESS_MASTER);
//  if (gps.location.isUpdated()) {
//    I2C_writeAnything(gps.location.lat()); // double float
//    I2C_writeAnything(gps.location.lng()); // double float
//    I2C_writeAnything(gps.date.value()); // u32
//    I2C_writeAnything(gps.time.value()); // u32
//  }
//  else {
//    float x = 0.0;
//    unsigned int y = 0;
//    I2C_writeAnything(x); // double float
//    I2C_writeAnything(x); // double float
//    I2C_writeAnything(y); // u32
//    I2C_writeAnything(y); // u32
//  }
//  if (Wire.endTransmission () != 0) {
//      Serial.println("I2C Transmission Failed");
//  }
  
//  if (gps.location.isUpdated()) {
//    char *lat = float_to_string(gps.location.lat());
//    char *lng = float_to_string(gps.location.lng());
//    Wire.write(lat);
//    Wire.write(lng);
//    Wire.write(gps.date.value()); // u32 = 4bytes
//    Wire.write(gps.time.value()); // u32 = 4bytes
//  }
//  else {
//    Wire.write(0);
//  }
//}

//char* float_to_string(double x) {
//  char fin[12];
//  
//  if (x >= 0) { fin[0] = '+'; }
//  else { fin[0] = '-'; x = -1*x; }
//  
//  unsigned int i = floor(x);
//  fin[3] = '0' + (i % 10); i /= 10;
//  fin[2] = '0' + (i % 10); i /= 10;
//  fin[1] = '0' + (i % 10); i /= 10;
//  
//  fin[4] = '.';
//  
//  for (i=5; i < 11; i++) {
//    x *= 10;
//    unsigned int j = floor(x);
//    fin[i] = '0' + (j%10);
//  }
//  
//  fin[11] = 0;
//  return fin;
//}
