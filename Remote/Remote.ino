#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 
 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

char msg[4];
const int btnUpPin = 2;
const int btnDownPin = 3;
const int btnLeftPin = 4;
const int btnRightPin = 5;
const int btnShootPin = 6;
const int btnCWPin = 7;
const int btnWagPin = 8;
const int btnCCWPin = 9;
const int ackLED = 13; // Blink LED

void setup()
{
    // Initialize ASK Object
    rf_driver.init();
    Serial.begin(9600);
  Serial.println("Maurice V1.0");
  Serial.println("By Blackbeard Softworks");
  Serial.println("Initial Release: January 4, 2021");
  // Pin definition
  pinMode(btnUpPin, INPUT);
  pinMode(btnDownPin, INPUT);
  pinMode(btnLeftPin, INPUT);
  pinMode(btnRightPin, INPUT);
  pinMode(btnShootPin, INPUT);
  pinMode(btnCWPin, INPUT);
  pinMode(btnWagPin, INPUT);
  pinMode(btnCCWPin, INPUT);
  pinMode(ackLED, OUTPUT);
}

void loop(){ 
    switch(getButtonState()){
      case 1:
        strcpy(msg, "uQQ");
        sendmsg();
        break;
      case 2:
        strcpy(msg, "dQQ");
        sendmsg();
        break;
      case 3:
        strcpy(msg, "lQQ");
        sendmsg();
        break;
      case 4:
        strcpy(msg, "rQQ");
        sendmsg();
        break;
      case 5:
        strcpy(msg, "SHT");
        sendmsg();
        break;
      case 6:
        strcpy(msg, "LQQ");
        sendmsg();
        break;
      case 7:
       strcpy(msg, "WAG");
        sendmsg();
        break;
      case 8:
        strcpy(msg, "RQQ");
        sendmsg();
        break;
      case 0:
        break;
      default:
        break;
    }
    
    delay(100);
}

int getButtonState(){
    // Get the state of the button
  byte state = 0b00000000;
  int upbtn = digitalRead(btnUpPin); // UP button
  int downbtn = digitalRead(btnDownPin); // DOWN button
  int leftbtn = digitalRead(btnLeftPin); // LEFT button
  int rightbtn = digitalRead(btnRightPin); // RIGHT button
  int shootbtn = digitalRead(btnShootPin); // OK button
  int cwbtn = digitalRead(btnCWPin); // DISPENSE button
  int wagbtn = digitalRead(btnWagPin); // DISPENSE button
  int ccwbtn = digitalRead(btnCCWPin); // DISPENSE button
  bitWrite(state, 0, upbtn);
  bitWrite(state, 1, downbtn);
  bitWrite(state, 2, leftbtn);
  bitWrite(state, 3, rightbtn);
  bitWrite(state, 4, shootbtn);
  bitWrite(state, 5, cwbtn);
  bitWrite(state, 6, wagbtn);
  bitWrite(state, 7, ccwbtn);
  switch(state){
    case 1: //
      return 1;
      break;
    case 2:
      return 2;
      break;
    case 4:
      return 3;
      break;
    case 8:
      return 4;
      break;
    case 16:
      return 5;
      break;
    case 32:
      return 6;
      break;
    case 64:
      return 7;
      break;
    case 128:
      return 8;
      break;
    case 0:
      return 0;
      break;
    default:
      return 255;
      break;
  }
}

void sendmsg(){
    digitalWrite(ackLED, HIGH);
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    Serial.println("Debug:");
    for(int i = 0; i < sizeof(msg); i++){
          Serial.print(msg[i],HEX);
          Serial.print(" ");
        }
        Serial.println("-- EOF --");
    digitalWrite(ackLED, LOW);
}
