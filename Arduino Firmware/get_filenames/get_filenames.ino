#include <SPI.h>
#include <SD.h>

File root;

#define cs_pin 4

void setup() {
  Serial.begin(115200);

  if (!SD.begin(cs_pin)) {
    Serial.println("initialization failed!");
    while (1);
  }

  root = SD.open("/");

  printFiles(root);

}

void loop(){}


void printFiles(File dir){
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }

    Serial.println(String(entry.name()));

  }
}