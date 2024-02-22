#include <TinyGPSPlus.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

//GPS
//-------------------------------------
#define rxPin 2
#define txPin 3
#define GPSBaud 9600
TinyGPSPlus gps;

SoftwareSerial gpsSerial (rxPin, txPin);
//-------------------------------------

//SD Card
//-------------------------------------
#define chipSelect 10    //CS for SD Card
long driveNum;
String filename;
//-------------------------------------

void setup(){
  Serial.begin(115200);
  gpsSerial.begin(GPSBaud);

  SPI.begin();

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  File root;              //get drive Number
  root = SD.open("/");
  driveNum = highestNumber(root, &filename);

}

void loop(){
  String dataString = "";       //defining new, empty String to load GPS data onto

  smartDelay(100);

  if(gps.satellites.value() > 5 && gps.location.lat() != 0){
    digitalWrite(LED_BUILTIN, HIGH);
    createString(&dataString);

    File dataFile = SD.open(filename, O_APPEND);

    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      Serial.println(dataString);
    }
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("too few Satellites!   Number of Satellites: ");
    Serial.println(gps.satellites.value());
  }
}

void smartDelay(long milliseconds){
  unsigned long milli = millis() + milliseconds;     //smart Delay (delay while pulling possible GPS Data)
  unsigned long continous_read_time = 0;
  while(millis() < milli){
    while(gpsSerial.available()){
      gps.encode(gpsSerial.read());
    }
  }
}

void createString(String* dataAddress){
  
  *dataAddress += String(gps.date.day());
  *dataAddress += "$";
  *dataAddress += String(gps.date.month());
  *dataAddress += "$";
  *dataAddress += String(gps.date.year());

  *dataAddress += ",";

  *dataAddress += String(gps.time.hour());
  *dataAddress += "$";
  *dataAddress += String(gps.time.minute());
  *dataAddress += "$";
  *dataAddress += String(gps.time.second());

  *dataAddress += ",";

  *dataAddress += String(gps.satellites.value());
  *dataAddress += ",";
  
  *dataAddress += String(gps.location.lat(), 10);
  *dataAddress += "$";
  *dataAddress += String(gps.location.lng(), 10);

  *dataAddress += ",";
  *dataAddress += String(gps.speed.kmph());
  *dataAddress += ",";
  *dataAddress += String(gps.course.deg());
  *dataAddress += ",";
  *dataAddress += String(gps.altitude.meters());
  *dataAddress += ",";
  *dataAddress += String(gps.hdop.hdop());
  *dataAddress += (";");
}

long highestNumber(File dir, String* filenameaddress){
  long highestNum = 0;
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }

    String filename = String(entry.name());

    String extractedNum = String(filename[0]);
    extractedNum += String(filename[1]);
    extractedNum += String(filename[2]);
    extractedNum += String(filename[3]);
    extractedNum += String(filename[4]);

    int number = extractedNum.toInt();
    if(number > highestNum){
      highestNum = number;
    }

  }

  highestNum += 1;

  *filenameaddress = "/";

  if (highestNum < 10) {
    *filenameaddress += "0000";
    *filenameaddress += String(highestNum);
    *filenameaddress += ".txt";
  }
  else if (highestNum < 100) {
    *filenameaddress += "000";
    *filenameaddress += String(highestNum);
    *filenameaddress += ".txt";
  }
  else if (highestNum < 1000) {
    *filenameaddress += "00";
    *filenameaddress += String(highestNum);
    *filenameaddress += ".txt";
  }
  else if (highestNum < 10000) {
    *filenameaddress += "0";
    *filenameaddress += String(highestNum);
    *filenameaddress += ".txt";
  }
  else if (highestNum < 100000) {
    *filenameaddress += String(highestNum);
    *filenameaddress += ".txt";
  }
  return highestNum;
}