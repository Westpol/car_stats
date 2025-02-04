#include <TinyGPSPlus.h>
// Search for tinygpsplus and install the version from Mikal Hart
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

// TODO: Add check if file has correct format

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
long driveNum = 0;
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

  while(!SD.begin(chipSelect)) {
    delay(1000);
  }

  

}

void loop(){
  digitalWrite(LED_BUILTIN, LOW);

  if(digitalRead(ignitionKey)){   // one time while turning ignition on
    while(gps.satellites.value() < 5){smartDelay(250);}
    filename = "";
    File root;              //get drive Number
    root = SD.open("/");
    while(driveNum == 0){
      driveNum = highestNumber(root, &filename);
      delay(500);
    }
    root.close();
    File dataFile = SD.open(filename, FILE_WRITE);

    while(digitalRead(ignitionKey)){    // looping while ignition is on

      smartDelay(500);
      String dataString = "";   // defining new, empty String to load GPS data onto

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
      char letter = gpsSerial.read();
      gps.encode(letter);
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
  if(!dir){
    return 0;
  }

  long highestNum = 0;
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }

    String filename = String(entry.name());
    entry.close();

    char extractedNum[6];
    extractedNum[0] = filename[0];
    extractedNum[1] = filename[1];
    extractedNum[2] = filename[2];
    extractedNum[3] = filename[3];
    extractedNum[4] = filename[4];
    extractedNum[5] = '\0';

    int number = atoi(extractedNum);
    if(number > highestNum){
      highestNum = number;
    }

  }

  highestNum += 1;

  *filenameaddress = "/";

  char numBuffer[6];
  snprintf(numBuffer, sizeof(numBuffer), "%05ld", highestNum); // Ensures a 5-digit number
  *filenameaddress += numBuffer;
  *filenameaddress += ".txt";

  return highestNum;
}