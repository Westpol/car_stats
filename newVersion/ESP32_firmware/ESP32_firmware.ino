#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define CS    -1  // Declaration of Chip Select
#define DC     4  // Declaration of Data / Command Pin
#define RST    2   // Declaration of RESET Pin

Adafruit_ST7789 display = Adafruit_ST7789(CS, DC, RST);


void setup(){
  Serial2.begin(115200);
  Serial.begin(115200);

  display.init(240, 240, SPI_MODE2);

}

void loop(){
  if (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
  display.fillScreen(ST77XX_BLACK);
  display.setTextSize(2);
  display.println("Hello World!");
}
