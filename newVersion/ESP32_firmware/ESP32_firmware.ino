
void setup(){
  Serial2.begin(115200);
  Serial.begin(115200);
}

void loop(){
  if (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
}
