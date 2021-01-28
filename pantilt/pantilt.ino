#include <AccelStepper.h>
#include <MultiStepper.h>
#include <RH_ASK.h>
#include <SPI.h>
AccelStepper pan(AccelStepper::FULL4WIRE, 2, 4, 3, 5);
AccelStepper tilt(AccelStepper::FULL4WIRE, 6, 8, 7, 9);
RH_ASK rf_driver;

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

  rf_driver.init();
  
  pinMode(ackLED, OUTPUT);
  Serial.println("Stargazer 1.14 by Blackbeard Softworks");
}

void loop() {
    
  uint8_t buf[4];
  uint8_t buflen = sizeof(buf);
  if (rf_driver.recv(buf, &buflen))
    {
      // Message received with valid checksum
      Serial.write((char*)buf);
      serialStr = (char*)buf;
    
  //serialStr = Serial.readString();
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
          serialStr.remove(3,1);
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
    else if (serialStr.equals("lQQ")){
      blink();
      Serial.println("WAIT");
      pan.enableOutputs();
      pan.runToNewPosition(pan.currentPosition()-150);
      pan.disableOutputs();
      Serial.println("OK");
    }
    else if (serialStr == "rQQ"){
      blink();
      Serial.println("WAIT");
      pan.enableOutputs();
      pan.runToNewPosition(pan.currentPosition()+150);
      pan.disableOutputs();
      Serial.println("OK");
    }
    else if (serialStr == "uQQ"){
      blink();
      Serial.println("WAIT");
      tilt.enableOutputs();
      tilt.runToNewPosition(tilt.currentPosition()+100);
      tilt.disableOutputs();
      Serial.println("OK");
    }
    else if (serialStr == "dQQ"){
      blink();
      Serial.println("WAIT");
      tilt.enableOutputs();
      tilt.runToNewPosition(tilt.currentPosition()-100);
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
