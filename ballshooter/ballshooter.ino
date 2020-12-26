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

char input = 0;
int addr = 0;
int v = 0;
String serialStr;
int id = 0;
const int ballShootPin = 4;
const int ballSenPin = 5;
const int rotateSenPin = 6;
const int ackLED = 13;

void setup() {
  Serial.begin(9600);
  // 1 second delay to help host OS detect the connection
  delay(1000);
  pinMode(ballShootPin, OUTPUT);
  pinMode(rotateSenPin, INPUT);
  pinMode(ackLED, OUTPUT);
  Serial.println("READY");
  EEPROM.get(addr, v);
  id = v;
  Serial.println(v, HEX);
  Serial.println("Andromeda V1.7 for ATMEGA328 (Arduino)");
  Serial.println("Developed by Blackbeard Softworks 2020");
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
    else if (serialStr == "CHK" && id == 0x64){
      blink();
        Serial.println("BALL STATUS");
        // Insert code to check whether or not the ball is there
        Serial.println(digitalRead(ballSenPin));
        Serial.println(digitalRead(rotateSenPin));
        Serial.println("OK");
    }
    else if (serialStr == "SHT" && id == 0x64){
      blink();
        Serial.println("SHOOT BALL");
        // Turn on the motor controller
        digitalWrite(ballShootPin, 1);
		// Status message
        Serial.println("WAITING FOR ROTATION");
		// Give the motor a slight headstart so it will not false trigger
		delay(100);
		// I know we have overabundance of variables
        int n = 0;
		// Reset the timelimit variable
        int timelimit = 0;
		// While the rotate sensor is not triggered...
        while (digitalRead(rotateSenPin) != HIGH){
          // Increment the timelimit variable
          timelimit++;
		  // Wait 100ms, short delay for debouncing
          delay(100);
          // Print out the time 
          Serial.print(n);
		  // Print out a blank space to make the number easier to see
          Serial.print(" ");
          // Add 1 to the counter
          n++;
          // Reset counter
          if (n > 9){
			// Simply reset the character to 0
            n = 0;
			// A hack to print a linefeed character
            Serial.println("");
          }
		  // Waits approximately 2s for the motor to return to the original point
		  // Then raises error message if it does not
          else if (timelimit > 10) {
            Serial.println("ERROR");
			// Exit loop
             break;
          }
         
          }
        Serial.println("WAIT LOOP EXIT");
		// Turn off the motor controller
        digitalWrite(ballShootPin, 0);
        Serial.println("OK");
        }
    else if (serialStr == "RETCON"){
      blink();
		// Return commands
        Serial.println("COMMANDS");
        Serial.println("AT: Heartbeat check");
        Serial.println("CHK: Check if the ball is in the chute (0x64)");
        Serial.println("SHT: Activate the ball shooter (0x64)");
        Serial.println("[Polaris - Developed by Blackbeard Softworks]");
        Serial.println("Build 1587 - Helios");
        Serial.println("OK");
    }
    
    else{
        blink();
        blink();
        blink();
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
