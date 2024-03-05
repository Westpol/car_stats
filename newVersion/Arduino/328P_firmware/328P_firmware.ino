#include <TinyGPSPlus.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

//GPS
//-------------------------------------
#define rxPin 2
#define txPin 3
#define GPSAvailPin 4
#define GPSBaud 9600
TinyGPSPlus gps;

SoftwareSerial gpsSerial (rxPin, txPin);
//-------------------------------------

//SD Card
//-------------------------------------
#define chipSelect 9    //CS for SD Card
long driveNum;
String filename;
//-------------------------------------

#define intercomSpeed 115200

void setup(){
  delay(3000);
  Serial.begin(intercomSpeed);
  gpsSerial.begin(GPSBaud);

  pinMode(GPSAvailPin, OUTPUT);

  SPI.begin();

  if (!SD.begin(chipSelect)) {
    // don't do anything more:
    while (1);
  }

  File root;              //get drive Number
  root = SD.open("/");
  driveNum = highestNumber(root, &filename);

}

void loop(){
  String dataString = "";       //defining new, empty String to load GPS data onto

  smartDelay(200);
  espCommunication();

  if(gps.satellites.value() > 5 && gps.location.lat() != 0){
    digitalWrite(GPSAvailPin, HIGH);
    createString(&dataString);

    File dataFile = SD.open(filename, FILE_WRITE);

    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    }
  }
  else {
    digitalWrite(GPSAvailPin, LOW);
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

void espCommunication(){
  String gpsData = String(gps.speed.kmph());
  gpsData += ",";
  gpsData += String(gps.satellites.value());
  gpsData += ",";
  gpsData += String(gps.location.lat(), 10);
  gpsData += "$";
  gpsData += String(gps.location.lng(), 10);
  gpsData += ";";
  Serial.println(gpsData);
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
  // ifs use 24384 program storage and 1352 dynamic memory
  // Stringlength uses 24376 program storage and 1350 dynamic memory
  // division by ten uses 24360 program storage and 1350 dynamic memory

  float tenner = highestNum;
  int i = 0;
  while(tenner > 1){
    i++;
    tenner = tenner / 10.0;
  }
  for(int k = 0; k < 4 - i; k++){
    *filenameaddress += "0";
  }

  *filenameaddress += String(highestNum);
  *filenameaddress += ".txt";

  return highestNum;
}