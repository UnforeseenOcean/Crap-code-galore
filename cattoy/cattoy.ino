/*
 * Polaris for Pet Servant
 * Blackbeard Softworks 2020
 * Bugfixes: 
 * Fixed wrong pin association for sensor pins
 * Fixed outputs not working
 * Fixed stepper driver malfunctioning
 * Fixed AccelStepper commands
 * Now includes sound pin (For use with PX088A)
 * 
 * D0-D1: Not used (Connect serial port) (INPUT/OUTPUT)
 * StepStick:
 * D2: DIR pin (Unused if it's set to ID 64) (OUTPUT)
 * D3: STEP pin (Unused if it's set to ID 64) (OUTPUT)
 * 
 * D4: Motor controller pin (Unused if it's set to ID 63) (OUTPUT)
 * D5: Ball sensor pin (Unused if it's set to ID 63) (INPUT)
 * D6: Rotation sensor pin (Unused if it's set to ID 63) (INPUT)
 * D7: TRIGGER pin for sound chip (Note: Use 1K resistor in series) (OUTPUT)
 * D13: Heartbeat LED (Blinks when receiving the signal) (OUTPUT)
 */

#include <EEPROM.h>
#include <AccelStepper.h>
#include <SPI.h> 

char input = 0;
int addr = 0;
int v = 0;
String serialStr;
int id = 0;
const int dirPin = 2;
const int stepPin = 3;
const int soundPin = 7;
const int ackLED = 13;

// Type: 2wire, STEP: D3, DIR: D2
AccelStepper stepper(1, stepPin, dirPin);


void setup() {
  
  Serial.begin(9600);
  // 1 second delay to help host OS detect the connection
  delay(1000);
  pinMode(soundPin, OUTPUT);
  pinMode(ackLED, OUTPUT);
  Serial.println("READY");
  EEPROM.get(addr, v);
  id = v;
  Serial.println(v, HEX);
  Serial.println("Reinhardt V1.5 for ATMEGA328 (Arduino)");
  Serial.println("Developed by Blackbeard Softworks 2020");
}

void loop() {
  stepper.setMaxSpeed(5000);
  stepper.setAcceleration(5000);
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
    
    if (serialStr == "AT"){
        blink();
        Serial.println("OK");
    }
    else if (serialStr == "ID"){
      blink();
        EEPROM.get(addr, v);
        Serial.println(v, HEX);
        Serial.println("OK");
    }
    else if (serialStr == "TYPE 0"){
      blink();
        EEPROM.put(addr, 0x63);
        Serial.println("ID: 0x63 (CAT)");
        id = EEPROM.get(addr, v);
        Serial.println("OK");
    }
    else if (serialStr == "TYPE 1"){
      blink();
        EEPROM.put(addr, 0x64);
        Serial.println("ID: 0x64 (DOG)");
        id = EEPROM.get(addr, v);
        Serial.println("OK");
    }
    else if (serialStr == "LQQ" && id == 0x63){
      blink();
        Serial.println("MOTOR CCW");
        // Insert code to pan the motor COUNTER-clockwise
        stepper.runToNewPosition(stepper.currentPosition()-25);
        Serial.println("OK");
    }
    else if (serialStr == "RQQ" && id == 0x63){
      blink();
        Serial.println("MOTOR CW");
        // Insert code to pan the motor clockwise
        stepper.runToNewPosition(stepper.currentPosition()+25);
        Serial.println("OK");
    }
    else if (serialStr == "LLL" && id == 0x63){
      blink();
        Serial.println("MOTOR CCW FAR");
        // Insert code to pan the motor COUNTER-clockwise
        stepper.runToNewPosition(stepper.currentPosition()-100);
        Serial.println("OK");
    }
    else if (serialStr == "RRR" && id == 0x63){
      blink();
        Serial.println("MOTOR CW FAR");
        // Insert code to pan the motor clockwise
        stepper.runToNewPosition(stepper.currentPosition()+100);
        Serial.println("OK");
    }
    else if (serialStr == "WAG" && id == 0x63){
      blink();
        Serial.println("SHAKE");
        // Insert code to shake the fishing rod
        stepper.runToNewPosition(stepper.currentPosition()+50);
        stepper.runToNewPosition(stepper.currentPosition()-50);
        digitalWrite(soundPin, 1);
        stepper.runToNewPosition(stepper.currentPosition()+50);
        stepper.runToNewPosition(stepper.currentPosition()-50);
        digitalWrite(soundPin, 0);
        delay(500);
       
        Serial.println("OK");
    }
    else if (serialStr == "RETCON"){
      blink();
		// Return commands
        Serial.println("COMMANDS");
        Serial.println("ID 0x63: Wagging Fish - ID 0x64: Ball Shooter");
        Serial.println("Use TYPE n to configure ID");
        Serial.println("AT: Heartbeat check");
        Serial.println("ID: Check ID");
        Serial.println("MV L/R: Move fish to direction (L:CCW, R:CW) (0x63)");
        Serial.println("WAG: Shake fish left and right (0x63)");
        Serial.println("[Reinhardt - Developed by Blackbeard Softworks]");
        Serial.println("Build 4932 - Wyvern");
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
