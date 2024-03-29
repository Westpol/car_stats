#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0 
#define WHITE 0xFFFF

#define CS    -1  // Declaration of Chip Select
#define DC     4  // Declaration of Data / Command Pin
#define RST    2   // Declaration of RESET Pin

Adafruit_ST7789 display = Adafruit_ST7789(CS, DC, RST);

struct gpsData{
  String messg;
  unsigned long driveNumber;
  float speed = 0.0;
  int sats = 0;
  String lat;
  String lon;
};

struct gpsData gpsdata;

const unsigned char gps_bitmap [] PROGMEM = {
	// 'output-onlinepngtools, 30x30px
	0x00, 0x80, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 
	0x00, 0xf8, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x3e, 0x10, 0x00, 0x00, 0x1f, 0x78, 0x00, 
	0x00, 0x0e, 0xfc, 0x00, 0x00, 0x05, 0xfe, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x07, 0xfe, 0x00, 
	0x00, 0x03, 0xff, 0x00, 0x00, 0xfd, 0xfe, 0x00, 0x01, 0x1f, 0xfc, 0x00, 0x01, 0x07, 0x7b, 0x00, 
	0x01, 0x03, 0xb7, 0x80, 0x99, 0x11, 0x87, 0xc0, 0xd9, 0x91, 0x83, 0xe0, 0xc9, 0x80, 0x81, 0xf0, 
	0xcc, 0xc0, 0x80, 0xf8, 0x4c, 0x70, 0x80, 0x7c, 0x66, 0x37, 0x00, 0x38, 0x23, 0x00, 0x00, 0x10, 
	0x31, 0xc0, 0x00, 0x00, 0x18, 0xf0, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 
	0x01, 0xf0, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00
};

void setup(){
  Serial2.begin(115200);
  Serial.begin(115200);

  display.init(240, 240, SPI_MODE2);
  display.setTextColor(0xFFFF, 0x0000);
  display.setRotation(3);
  
  display.fillScreen(0x0000);

}

void loop(){
  display.setCursor(0, 50);
  display.setTextSize(2);
  display.println(gpsdata.messg);

  if(gpsdata.sats > 5){
    display.drawBitmap(200, 10, gps_bitmap, 30, 30, GREEN);
  }
  else {
    display.drawBitmap(200, 10, gps_bitmap, 30, 30, RED);
  }

  smartDelay(500);
}

void smartDelay(unsigned long delayTime){
  unsigned long loopTime = millis() + delayTime;
  while (millis() < loopTime){
    if (Serial2.available() > 0) {

      char incomingChar = Serial2.read();

      if (incomingChar == '?') {
        String message = "";
        message += '?';
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
        message += '!';
        gpsdata.messg = message;
        decrypt_message();
      }
    }
  }
}

void decrypt_message(){
  int position = 1;
  String dataBuffer;
  if(gpsdata.messg[0] == '?' && gpsdata.messg[gpsdata.messg.length() - 1] == '!'){

    while (gpsdata.messg[position] != ',') {    //save drive number as long
      dataBuffer += gpsdata.messg[position];
      position++;
    }
    gpsdata.driveNumber = dataBuffer.toInt();
    dataBuffer = "";
    position++;

    while (gpsdata.messg[position] != ',') {    //save speed in kph as float
      dataBuffer += gpsdata.messg[position];
      position++;
    }
    gpsdata.speed = dataBuffer.toFloat();
    dataBuffer = "";
    position++;

    while (gpsdata.messg[position] != ',') {    //save sats as int
      dataBuffer += gpsdata.messg[position];
      position++;
    }
    gpsdata.sats = dataBuffer.toInt();
    dataBuffer = "";
    position++;


    while (gpsdata.messg[position] != '$') {    //save lat as String
      dataBuffer += gpsdata.messg[position];
      position++;
    }
    gpsdata.lat = dataBuffer;
    dataBuffer = "";
    position++;

    while (gpsdata.messg[position] != '!') {    //save lon as String
      dataBuffer += gpsdata.messg[position];
      position++;
    }
    gpsdata.lon = dataBuffer;
    dataBuffer = "";
    position++;
    debug_print();
  }
}

void KERNEL_PANIC(){
  ESP.restart();
}

void debug_print(){
Serial.print("Dri N: ");
Serial.println(gpsdata.driveNumber);
Serial.print("Speed: ");
Serial.println(gpsdata.speed);
Serial.print("Sats:");
Serial.println(gpsdata.sats);
}
