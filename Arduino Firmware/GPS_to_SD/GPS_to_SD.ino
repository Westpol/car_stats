#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
/*
   This sample code demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 2, TXPin = 3;

#define chipSelect 4

static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

    if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

}

void loop(){

    String dataString = "";       //defining new, empty String to load GPS data onto

  unsigned long milli = millis() + 100;     //smart Delay (delay whie pulling possible GPS Data)
  unsigned long continous_read_time = 0;
  while(millis() < milli){
    while(millis() < continous_read_time){
      while(ss.available()){
        gps.encode(ss.read());
        continous_read_time = millis() + 10;
      }
    }
  }

  if(gps.satellites.value() > 5){
    printDate(&dataString);
    dataString += ",";
    printTime(&dataString);
    dataString += ",";
    dataString += String(gps.satellites.value());
    dataString += ",";
    printPosition(&dataString);
    dataString += ",";
    dataString += String(gps.speed.kmph());
    dataString += ",";
    dataString += String(gps.course.deg());
    dataString += ",";
    dataString += String(gps.altitude.meters());
    dataString += ",";
    dataString += String(gps.hdop.hdop());
    dataString += (";");

    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      Serial.println(dataString);
    }
  }
  else {
  Serial.print("too few Satellites!   Number of Satellites: ");
  Serial.println(gps.satellites.value());
  }

}


void printDate(String* dataAddress){
  *dataAddress += String(gps.date.day());
  *dataAddress += "$";
  *dataAddress += String(gps.date.month());
  *dataAddress += "$";
  *dataAddress += String(gps.date.year());
}


void printTime(String* dataAddress){
  *dataAddress += String(gps.time.hour());
  *dataAddress += "$";
  *dataAddress += String(gps.time.minute());
  *dataAddress += "$";
  *dataAddress += String(gps.time.second());
}


void printPosition(String* dataAddress){
  *dataAddress += String(gps.location.lat(), 10);
  *dataAddress += "$";
  *dataAddress += String(gps.location.lng(), 10);

}