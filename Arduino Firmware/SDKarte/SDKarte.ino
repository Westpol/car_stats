//#include <HardwareSerial.h>

void setup(){
  Serial.begin(115200);
  /*Serial1.begin(9600);*/
  Serial2.begin(9600);
}

void loop(){
  if(Serial2.available() > 0){
    Serial.print((char) Serial2.read());
  }
}
