#include <AccelStepper.h>
#include <MultiStepper.h>
AccelStepper pan(AccelStepper::FULL4WIRE, 2, 4, 3, 5);
AccelStepper tilt(AccelStepper::FULL4WIRE, 6, 8, 7, 9);

char input = 0;
String serialStr;
int id = 0;
const int ackLED = 13;

MultiStepper steppers;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pan.setMaxSpeed(500);
  tilt.setMaxSpeed(500);
  pan.setAcceleration(5000);
  tilt.setAcceleration(5000);
  pinMode(ackLED, OUTPUT);
  Serial.println("Stargazer 1.14 by Blackbeard Softworks");
}

void loop() {
  if (Serial.available() > 0) {
    serialStr = Serial.readString();
    // Ugly, ugly hack to somehow FORCE it to understand that I don't want any of that special characters...
    for(int i = 0; i < sizeof(serialStr); i++){
          if (serialStr[i] == 0x0A){
            serialStr.remove(i,1);
          }
          if (serialStr[i] == 0x0D){
            serialStr.remove(i,1);
          }
          if (serialStr[i] == 0x03){
            serialStr.remove(i,1);
          }
          if (serialStr[i] == 0x01){
            serialStr.remove(i,1);
          }
        }
    serialStr.trim();
    if (serialStr == "ok"){
        blink();
        Serial.println("OK");
    }
    else if (serialStr == "info"){ 
      blink();
      Serial.println("Stargazer 1.14 by Blackbeard Softworks");
      Serial.println("OK");
    }
    else if (serialStr == "lQQ"){
      blink();
      Serial.println("WAIT");
      pan.enableOutputs();
      pan.runToNewPosition(pan.currentPosition()-50);
      pan.disableOutputs();
      Serial.println("OK");
    }
    else if (serialStr == "llQ"){
      blink();
      blink();
      Serial.println("WAIT");
      pan.enableOutputs();
      pan.runToNewPosition(pan.currentPosition()-100);
      pan.disableOutputs();
      Serial.println("OK");
    }
    else if (serialStr == "lll"){
      blink();
      blink();
      blink();
      Serial.println("WAIT");
      pan.enableOutputs();
      pan.runToNewPosition(pan.currentPosition()-300);
      pan.disableOutputs();
      Serial.println("OK");
    }
    else if (serialStr == "rQQ"){
      blink();
      Serial.println("WAIT");
      pan.enableOutputs();
      pan.runToNewPosition(pan.currentPosition()+50);
      pan.disableOutputs();
      Serial.println("OK");
    }
    else if (serialStr == "rrQ"){
      blink();
      blink();
      Serial.println("WAIT");
      pan.enableOutputs();
      pan.runToNewPosition(pan.currentPosition()+100);
      pan.disableOutputs();
      Serial.println("OK");
    }
    else if (serialStr == "rrr"){
      blink();
      blink();
      blink();
      Serial.println("WAIT");
      pan.enableOutputs();
      pan.runToNewPosition(pan.currentPosition()+300);
      pan.disableOutputs();
      Serial.println("OK");
    }
    else if (serialStr == "uQQ"){
      blink();
      Serial.println("WAIT");
      tilt.enableOutputs();
      tilt.runToNewPosition(tilt.currentPosition()+50);
      tilt.disableOutputs();
      Serial.println("OK");
    }
    else if (serialStr == "uuQ"){
      blink();
      blink();
      Serial.println("WAIT");
      tilt.enableOutputs();
      tilt.runToNewPosition(tilt.currentPosition()+100);
      tilt.disableOutputs();
      Serial.println("OK");
    }
    else if (serialStr == "uuu"){
      blink();
      blink();
      blink();
      Serial.println("WAIT");
      tilt.enableOutputs();
      tilt.runToNewPosition(tilt.currentPosition()+300);
      tilt.disableOutputs();
      Serial.println("OK");
        }
    else if (serialStr == "dQQ"){
      blink();
      Serial.println("WAIT");
      tilt.enableOutputs();
      tilt.runToNewPosition(tilt.currentPosition()-50);
      tilt.disableOutputs();
      Serial.println("OK");
    }
    else if (serialStr == "ddQ"){
      blink();
      blink();
      Serial.println("WAIT");
      tilt.enableOutputs();
      tilt.runToNewPosition(tilt.currentPosition()-100);
      tilt.disableOutputs();
      Serial.println("OK");
    }
    else if (serialStr == "ddd"){
      blink();
      blink();
      blink();
      Serial.println("WAIT");
      tilt.runToNewPosition(tilt.currentPosition()-300);
      tilt.disableOutputs();
      Serial.println("OK");
    }
    else{
        Serial.print("I don't understand ");
        Serial.println(serialStr);
        Serial.print("Debug: ");
        for(int i = 0; i < sizeof(serialStr); i++){
          Serial.print(serialStr[i],HEX);
          Serial.print(" ");
        }
        Serial.println("-- EOF --");
    }
    }
}

void blink(){
  digitalWrite(ackLED, 1);
  delay(100);
  digitalWrite(ackLED, 0);
}
