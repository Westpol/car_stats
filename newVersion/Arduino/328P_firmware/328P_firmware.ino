#include <TinyGPSPlus.h>  // GPS Library
#include <SPI.h>
#include <SdFat.h>        // SdFat Library for SD Card
#include <SoftwareSerial.h>

// GPS
//-------------------------------------
#define rxPin 6
#define txPin 7
#define GPSBaud 9600
TinyGPSPlus gps;
SoftwareSerial gpsSerial(rxPin, txPin);
//-------------------------------------

// SD Card
//-------------------------------------
#define chipSelect 10  // CS for SD Card
SdFat SD;              // Use SdFat for better performance
SdFile dataFile;
long driveNum = 0;
char filename[13];  // File name buffer ("/00000.txt")
//-------------------------------------

// Power Off Detection
//-------------------------------------
#define ignitionKey 2
//-------------------------------------

void setup() {
  delay(1000);
  gpsSerial.begin(GPSBaud);

  pinMode(ignitionKey, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  SPI.begin();

  // Initialize SdFat in SPI_HALF_SPEED mode for Arduino Uno compatibility
  if (!SD.begin(chipSelect, SD_SCK_MHZ(4))) {  // Lower speed for stability
    while (1) { digitalWrite(LED_BUILTIN, HIGH); delay(500); digitalWrite(LED_BUILTIN, LOW); delay(500); }
  }
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);

  if (digitalRead(ignitionKey)) {  // One-time execution when ignition is turned on
    while (gps.satellites.value() < 5) {
      smartDelay(250);
    }

    SdFile root;
    if (!root.open("/")) {
      return;
    }

    while (driveNum == 0) {
      driveNum = highestNumber(root, filename);  // Pass `root` by reference
      delay(500);
    }
    root.close();

    if (!dataFile.open(filename, O_WRITE | O_CREAT)) {
      return;
    }

    while (digitalRead(ignitionKey)) {  // Loop while ignition is on
      smartDelay(500);
      char dataString[64];  // Buffer for GPS data string
      createString(dataString, sizeof(dataString));

      digitalWrite(LED_BUILTIN, LOW);
      if (gps.satellites.value() > 5) {
        digitalWrite(LED_BUILTIN, HIGH);
        if (dataFile.isOpen()) {
          dataFile.println(dataString);
        }
      }
    }
    dataFile.close();
  }
  smartDelay(250);
}

void smartDelay(long milliseconds) {
  unsigned long milli = millis() + milliseconds;
  while (millis() < milli) {
    while (gpsSerial.available()) {  // Get GPS chars
      char letter = gpsSerial.read();
      gps.encode(letter);
    }
  }
}

void createString(char* dataBuffer, size_t bufferSize) {
  snprintf(dataBuffer, bufferSize, "%02d$%02d$%04d;%02d$%02d$%02d;%d;%.6f$%.6f;%.2f;%.2f;%.1f;%.1f",
           gps.date.day(), gps.date.month(), gps.date.year(),
           gps.time.hour(), gps.time.minute(), gps.time.second(),
           gps.satellites.value(),
           gps.location.lat(), gps.location.lng(),
           gps.speed.kmph(), gps.course.deg(), gps.altitude.meters(), gps.hdop.hdop());
}

long highestNumber(SdFile& dir, char* filenameBuffer) {
  if (!dir.isOpen()) {
    return 0;
  }

  long highestNum = 0;
  SdFile entry;

  while (entry.openNext(&dir, O_READ)) {
    char entryName[13];  // Buffer for filename
    entry.getName(entryName, sizeof(entryName));
    entry.close();

    if (strlen(entryName) == 9 && strcmp(entryName + 5, ".txt") == 0) {
      char extractedNum[6] = {0};  // Extract numeric part
      strncpy(extractedNum, entryName, 5);

      int number = atoi(extractedNum);
      if (number > highestNum) {
        highestNum = number;
      }
    }
  }

  highestNum += 1;
  snprintf(filenameBuffer, 13, "/%05ld.txt", highestNum);  // Generate filename

  return highestNum;
}
