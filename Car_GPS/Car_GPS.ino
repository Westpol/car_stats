#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
/*
   This sample code demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
}

void loop()
{
  unsigned long milli = millis() + 100;
  while(millis() < milli){
    while(ss.available()){
      gps.encode(ss.read());
    }
  }
  printDate();
  Serial.print(",");
  printTime();
  Serial.print(",");
  Serial.print(gps.satellites.value());
  Serial.print(",");
  printPosition();
  Serial.print(",");
  Serial.print(gps.speed.kmph());
  Serial.print(",");
  Serial.print(gps.course.deg());
  Serial.print(",");
  Serial.print(gps.altitude.meters());
  Serial.print(",");
  Serial.print(gps.hdop.hdop());
  Serial.print(";\n");
}


void printDate(){
  Serial.print(gps.date.day());
  Serial.print("$");
  Serial.print(gps.date.month());
  Serial.print("$");
  Serial.print(gps.date.year());
}


void printTime(){
  Serial.print(gps.time.hour());
  Serial.print("$");
  Serial.print(gps.time.minute());
  Serial.print("$");
  Serial.print(gps.time.second());
}


void printPosition(){
  Serial.print(gps.location.lat(), 10);
  Serial.print("$");
  Serial.print(gps.location.lng(), 10);

}