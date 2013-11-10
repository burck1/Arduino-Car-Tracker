#include <Wire.h>
#include <EasyTransferI2C.h>

#define I2C_ADDRESS_MASTER 0x1
#define I2C_ADDRESS_SLAVE 0x2

EasyTransferI2C ET;

struct RECEIVE_DATA_STRUCTURE {
  float my_lat;
  float my_lng;
  unsigned int my_date;
  unsigned int my_time;
};

RECEIVE_DATA_STRUCTURE mydata;

//volatile float lat;
//volatile float lng;
//volatile unsigned int date;
//volatile unsigned int time;

void setup()
{
  Serial.begin(9600);
  
  Wire.begin(I2C_ADDRESS_MASTER);
  ET.begin(details(mydata), &Wire);
}

void loop()
{
  Wire.requestFrom(I2C_ADDRESS_SLAVE, 1);
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
//  if(ET.receiveData()) {
//    Serial.print(mydata.my_lat,6);Serial.print(",");Serial.println(mydata.my_lng,6);
//  }
//  char in[30];
//  int i = 0;
//  byte x[4];
//  while(Wire.available())
//  {
//    in[i++] = Wire.read();
//    x[i++] = Wire.read();
//  }
//  in[i] = '\0';
//  Serial.println((unsigned int) x);
  
  delay(500);
}
