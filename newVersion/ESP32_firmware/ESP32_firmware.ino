#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define CS    -1  // Declaration of Chip Select
#define DC     4  // Declaration of Data / Command Pin
#define RST    2   // Declaration of RESET Pin

Adafruit_ST7789 display = Adafruit_ST7789(CS, DC, RST);

struct gpsData{
  String messg;
  unsigned long driveNumber;
  float speed;
  int sats;
  String lat;
  String lon;
};

struct gpsData gpsdata;

void setup(){
  Serial2.begin(115200);
  Serial.begin(115200);

  display.init(240, 240, SPI_MODE2);
  display.setTextColor(0xFFFF, 0x0000);
  display.setRotation(3);
  
  display.fillScreen(0x0000);

}

void loop(){
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.println(gpsdata.messg);

  smartDelay(500);
}

void smartDelay(unsigned long delayTime){
  unsigned long loopTime = millis() + delayTime;
  while (millis() < loopTime){
    if (Serial2.available() > 0) {

      char incomingChar = Serial2.read();

      if (incomingChar == '?') {
        String message = "";
        int whileOverflow = 0;

        while (incomingChar != '!') {
        if (whileOverflow > 50) {
          KERNEL_PANIC();
        }

        if (Serial2.available() > 0) {
          whileOverflow ++;

          incomingChar = Serial2.read();
          if (incomingChar != '!'){
            message += incomingChar;
          }
        }

        }
        gpsdata.messg = message;
      }
    }
  }
}

void KERNEL_PANIC(){
  ESP.restart();
}
