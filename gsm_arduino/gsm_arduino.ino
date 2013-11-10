#include <Wire.h>
#include <EasyTransferI2C.h>
#include <GSM.h>

/* ---------- Xively Setup ---------- */
#define APIKEY     "9DGhzPeqvXxae8repC5PD2159hZZrn6IlNqpIYpsdt8ZS7jQ"
#define FEEDID     565709094
#define USERAGENT  "Car Tracker" // project name

char server[] = "api.xively.com";
char path[] = "/v2/feeds";

unsigned long lastConnectionTime = 0; // milliseconds
boolean lastConnected = false; // state of the connection last time through the main loop
const unsigned long postingInterval = 20*1000; //delay between updates to Xively.com


/* ---------- GSM Setup ---------- */
#define PINNUMBER      ""
#define GPRS_APN       "bluevia.movistar.es"
#define GPRS_LOGIN     ""
#define GPRS_PASSWORD  ""

GSMClient client;
GPRS gprs;
GSM gsmAccess;

boolean sent_get_request = false;
boolean recieved_all_data = false;


/* ---------- I2C Setup ---------- */
#define I2C_ADDRESS_SLAVE 0x2

EasyTransferI2C ET;

struct RECEIVE_DATA_STRUCTURE {
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
RECEIVE_DATA_STRUCTURE mydata;


void setup()
{
  Serial.begin(9600);
  
  Wire.begin(I2C_ADDRESS_SLAVE);
  ET.begin(details(mydata), &Wire);
  
  //define handler function on receiving data
  Wire.onReceive(receive);
  
  boolean notConnected = true;
  while(notConnected) {
    if((gsmAccess.begin(PINNUMBER)==GSM_READY) &
       (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY))
      notConnected = false;
    else {
      Serial.println("Not connected");
      delay(1000);
    }
  }
}

void loop()
{
  if(get_gps_data()) {
  }
  
  // for debugging
  while (client.available()) {
     char c = client.read();
     Serial.print(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && ((millis() - lastConnectionTime) > postingInterval)) {
    sendData();
  }

  // store the state of the connection for next time through the loop:
  lastConnected = client.connected();
}

void receive(int numBytes) { }

boolean get_gps_data() {
  if(ET.receiveData()) {
      Serial.print(mydata.lat,6);Serial.print(",");Serial.print(mydata.lng,6);Serial.print(" ");
      Serial.print(mydata.mph,2);Serial.print("mph ");
      Serial.print(mydata.month);Serial.print("/");Serial.print(mydata.day);Serial.print("/");Serial.print(mydata.year);Serial.print(" ");
      Serial.print(mydata.hour);Serial.print(":");Serial.print(mydata.minute);Serial.print(":");Serial.print(mydata.second);Serial.print(".");Serial.println(mydata.centisecond);
    return true;
  }
  else {
    return false;
  }
}

void sendData() {
  if (client.connect(server, 80)) {
    Serial.println("connecting...");

    // HEADER
    client.print("PUT ");
    client.print(path);
    client.print("/");
    client.print(FEEDID);
    client.println(".csv HTTP/1.1");
    client.println("Host: api.xively.com");
    client.print("X-ApiKey: ");
    client.println(APIKEY);
    client.print("User-Agent: ");
    client.println(USERAGENT);
    client.print("Content-Length: ");

    // 4 for "lat," + length of non-decimal part + decimal pt + 6 decimal places
    int thisLength = 4 + getLength(floor(mydata.lat)) + 1 + 6;
    thisLength += 4 + getLength(floor(mydata.lng)) + 1 + 6;
    thisLength += 4;
    
    unsigned int n;
    
    char date_buffer[16];
    n = sprintf(date_buffer, "%u/%u/%u", mydata.month, mydata.day, mydata.year);
    thisLength += n;
    
    char time_buffer[16];
    n = sprintf(time_buffer, "%u:%u:%u.%03u", mydata.hour, mydata.minute, mydata.second, mydata.centisecond);
    thisLength += n;

    // Extra to add on: found by testing
    thisLength += 14;

    client.println(thisLength);

    client.println("Content-Type: text/csv");
    client.println("Connection: close");
    client.println();

    // BODY
    client.print("lat,"); client.println(mydata.lat, 6);
    client.print("lng,"); client.println(mydata.lng, 6);
    client.print("date,"); client.println(date_buffer);
    client.print("time,"); client.println(time_buffer);
  }
  else {
    Serial.println("connection failed");
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  lastConnectionTime = millis();
}

/*
 * Calculates the number of digits in someValue
 */
int getLength(int someValue)
{
  int digits = 1;
  int dividend = someValue /10;
  while (dividend > 0)
  {
    dividend = dividend /10;
    digits++;
  }
  return digits;
}

/*
 * Future Use : Not yet implemented
 *
boolean get() {
  Serial.println("connecting...");

  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.print("GET ");
    client.print(path);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
    return true;
  } 
  else {
    Serial.println("connection failed");
    return false;
  }
}

boolean receive_response() {
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  if (!client.available() && !client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    return true;
  }
  else {
    return false;
  }
}
*/
