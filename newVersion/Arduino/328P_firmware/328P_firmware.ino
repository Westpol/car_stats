#include <TinyGPSPlus.h>
// Search for tinygpsplus and install the version from Mikal Hart
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

//GPS
//-------------------------------------
#define rxPin 6
#define txPin 7
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

//Serial comm
//-------------------------------------
#define intercomSpeed 115200
//-------------------------------------

//power off detection
//-------------------------------------
#define ignitionKey 2
//-------------------------------------

void setup(){
  delay(1000);
  gpsSerial.begin(GPSBaud);

  pinMode(ignitionKey, INPUT);    // digital pin that detects ignition key position
  pinMode(LED_BUILTIN, OUTPUT);   // debug LED to show enough satellites, etc.
  digitalWrite(LED_BUILTIN, LOW);

  SPI.begin();

  if (!SD.begin(chipSelect)) {
    // don't do anything more:
    while (1);
  }

  

}

void loop(){
  digitalWrite(LED_BUILTIN, LOW);

  if(digitalRead(ignitionKey)){   // one time while turning ignition on
    while(gps.satellites.value() < 5){smartDelay(100);}
    File root;              //get drive Number
    root = SD.open("/");
    driveNum = highestNumber(root, &filename);
    File dataFile = SD.open(filename, FILE_WRITE);
    String dataString = "";       //defining new, empty String to load GPS data onto

    while(digitalRead(ignitionKey)){    // looping while ignition is on

      smartDelay(500);

      digitalWrite(LED_BUILTIN, LOW);
      if(gps.satellites.value() > 5){
        digitalWrite(LED_BUILTIN, HIGH);
        createString(&dataString);

        if (dataFile) {
          dataFile.println(dataString);
        }
      }
    }
    dataFile.close();
  }
  smartDelay(250);
}

void smartDelay(long milliseconds){
  unsigned long milli = millis() + milliseconds;     //smart Delay (delay while pulling possible GPS Data)
  unsigned long continous_read_time = 0;
  while(millis() < milli){
    while(gpsSerial.available()){   // get GPS chars
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

  *dataAddress += ";";

  *dataAddress += String(gps.time.hour());
  *dataAddress += "$";
  *dataAddress += String(gps.time.minute());
  *dataAddress += "$";
  *dataAddress += String(gps.time.second());

  *dataAddress += ";";

  *dataAddress += String(gps.satellites.value());
  *dataAddress += ";";
  
  *dataAddress += String(gps.location.lat(), 10);
  *dataAddress += "$";
  *dataAddress += String(gps.location.lng(), 10);

  *dataAddress += ";";
  *dataAddress += String(gps.speed.kmph());
  *dataAddress += ";";
  *dataAddress += String(gps.course.deg());
  *dataAddress += ";";
  *dataAddress += String(gps.altitude.meters());
  *dataAddress += ";";
  *dataAddress += String(gps.hdop.hdop());
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