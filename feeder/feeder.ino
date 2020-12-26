/* The screen should look something like this
 *  __________________
 * | 20-01-23 12:34 P |
 * |_Feed_1/6_____000_|
 * 
 * Top line: Date and time
 * Bottom line: Current and remaining feed count
 * 
 * Six buttons, one LED (connected to the motor driver INPUT pin, not OUTPUT) are connected to D2~D8
 * UP: D2
 * DOWN: D3
 * LEFT: D4
 * RIGHT: D5
 * OK: D6
 * DISPENSE: D7
 * MOTOR: D8
 * 
 * Optional speaker can be added to D9 (with an amp)
 * 
 * Rotation sensor is connected to A0, and D13 is defined as output.
 * An LDR is connected to A6 with this circuit, where OUT is the output:
 * VCC - LDR - OUT - 10K - GND
 * I2C display is connected as such:
 * VCC: 5V
 * GND: GND
 * SCL: A5
 * SDA: A4
 * 
 * TODO:
 * - Add backlight control
 * - Read buttons
 * - Add menu system
 * - Add RTC support
 * - Add motor condition check
 * - Add motor 
 * 
 * Display: 0x3F
 * RTC: 0x68
 * 
 * 
 * 
 * Looks like there should be a modification to allow the timers to return back to default value
 * 
 */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <EEPROM.h>

// Settings and data

const int btnUpPin = 2; // UP button
const int btnDownPin = 3; // DOWN button
const int btnLeftPin = 4; // LEFT button
const int btnRightPin = 5; // RIGHT button
const int btnOKPin = 6; // OK button
const int btnDispensePin = 7; // DISPENSE button
const int motorPin = 9; // Motor
const int rotateSensePin = 8; // Rotation sensor
const int backlightPin = 10; // Backlight pin
const int ackLED = 13; // Blink LED

// EEPROM address and value
int address = 0;
int value;

char lcdtime [16];
int counter = 86400; // 24 hours
// Menu element counter
int menucount = 0;
int interval = 10;
int timeRemaining;

int dimThres = 300; // Automatic dimming threshold
int brightness = 255; // Maximum brightness
int dimBrightness = 38; // Minimum brightness


void(* resetFunc) (void) = 0;

// Change if you use DS1307
RTC_DS3231 rtc;
DateTime now;
//Wire.begin();
// Start I2C LCD with address 0x3f, 16 characters and 2 lines
LiquidCrystal_I2C lcd(0x3f,16,2);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Fido V1.0 (FW 1001 - Spot)");
  Serial.println("By Blackbeard Softworks");
  Serial.println("Initial Release: November 2020");
  lcd.init();
  lcd.backlight();
  Wire.begin();
  rtc.begin();
  // Pin definition
  pinMode(btnUpPin, INPUT); // UP button
  pinMode(btnDownPin, INPUT); // DOWN button
  pinMode(btnLeftPin, INPUT); // LEFT button
  pinMode(btnRightPin, INPUT); // RIGHT button
  pinMode(btnOKPin, INPUT); // OK button
  pinMode(btnDispensePin, INPUT); // DISPENSE button
  pinMode(motorPin, OUTPUT); // Motor
  pinMode(rotateSensePin, INPUT); // Rotation sensor
  pinMode(backlightPin, OUTPUT); // Backlight pin
  
  EEPROM.get(address,interval);
  int timeRemaining = interval;
  
  updateLCD();
}

void loop() {
  // put your main code here, to run repeatedly:
  updateLCD();
  timer();
  backlightSet();
  delay(500);
}

int getButtonState(){
    // Get the state of the button
  byte state = 0b00000000;
  int upbtn = digitalRead(btnUpPin); // UP button
  int downbtn = digitalRead(btnDownPin); // DOWN button
  int leftbtn = digitalRead(btnLeftPin); // LEFT button
  int rightbtn = digitalRead(btnRightPin); // RIGHT button
  int okbtn = digitalRead(btnOKPin); // OK button
  int dispbtn = digitalRead(btnDispensePin); // DISPENSE button
  bitWrite(state, 0, upbtn);
  bitWrite(state, 1, downbtn);
  bitWrite(state, 2, leftbtn);
  bitWrite(state, 3, rightbtn);
  bitWrite(state, 4, okbtn);
  bitWrite(state, 5, dispbtn);
  switch(state){
    case 1:
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
    case 0:
      return 0;
    default:
      return 255;
      break;
  }
}

void dispense(){
  //startPlayback(sample, sizeof(sample));
  // Stolen straight from the other module
  blinkIt();
  Serial.println("Dispense cycle start");
  analogWrite(motorPin, 100);
  delay(2000);
  // Turn off the motor controller
  analogWrite(motorPin, 0);
  Serial.println("Dispense cycle end");

}

void getTime(){
  
  // Get time from RTC
  // TODO: Find the I2C address for the RTC and implement read and write functions
  now = rtc.now();
  DateTime timezone = now; //+ TimeSpan(0,timezoneSetting,0,0);
  
  // This char array must be redefined every time the function is called, else it won't work
  char tsc [] = "hh:mm AP      ss";
  strncpy(lcdtime, timezone.toString(tsc), sizeof(lcdtime));
  return 0;
}

void writeTime(){
  // Write time to RTC
}

void menu(int button){
  // Settings menu
  lcd.setCursor(0,0);
  lcd.print("Press OK to save");
  if (button == 0){
    if (interval < 1440){
      interval += 10;
      lcd.setCursor(0,1);
      lcd.print("Delay:           ");
      lcd.setCursor(9,1);
      lcd.print(interval);
      lcd.setCursor(13,1);
      lcd.print("min");
      delay(100);
    }
    else if (button == 0 && interval > 1430) {
      interval = 1430;
      lcd.setCursor(0,1);
      lcd.print("Delay:           ");
      lcd.setCursor(9,1);
      lcd.print(interval);
      lcd.setCursor(13,1);
      lcd.print("min");
      delay(100);
    }
  }
  else if (button == 255){
    if (interval >= 20){
      interval -= 10;
      lcd.setCursor(0,1);
      lcd.print("Delay:           ");
      lcd.setCursor(9,1);
      lcd.print(interval);
      lcd.setCursor(13,1);
      lcd.print("min");
      delay(100);
    }
    else if (button == 255 && interval < 20) {
      interval = 10;
      lcd.print("Delay:           ");
      lcd.setCursor(9,1);
      lcd.print(interval);
      lcd.setCursor(13,1);
      lcd.print("min");
      delay(100);
    }
    else {
      interval = 10;
    }
  }
  else if (button == 128){
    EEPROM.put(address,interval);
    lcd.setCursor(0,1);
    lcd.print("Settings saved! ");
    delay(1500);
    // Blank the bottom row
    lcd.setCursor(0,1);
    lcd.print("                ");
    // Reset the Arduino for maximum jank
    resetFunc(); 
    lcd.setCursor(0,1);
    lcd.print("DOBBY IS FREE!!!");
  }
}

void timer(){
  // Every time the sub timer reaches 0, subtract 1 from the main timer
  // When the timer reaches 0, dispense the feed and reset the main timer
  char cmpa[3];
  now = rtc.now();
  DateTime tzq = now;
  char tmb [] = "ss";
  strncpy(cmpa, tzq.toString(tmb), sizeof(cmpa));
  int m = (int)cmpa[0];
  int n = (int)cmpa[1]; 
  if (m == 48 && n == 48){
    blinkIt();
    timeRemaining = timeRemaining - 1;
    // Very lazy way to prevent double-activation
    // Don't do this
    delay(500);
  }
  else if (timeRemaining <= 0){
      timeRemaining = interval;
      dispense();
  }
  Serial.print(m);
  Serial.print(n);
  Serial.print("; ");
  Serial.print(interval);
  Serial.print("; ");
  Serial.println(timeRemaining);
}

void updateLCD(){ 
  getTime();
  lcd.setCursor(0,0);
  lcd.print(lcdtime); // Date
  lcd.setCursor(0,1);
  lcd.print("Delay: ");
  lcd.setCursor(13,1);
  lcd.print("min");
  lcd.setCursor(9,1);
  lcd.print("    ");
  lcd.setCursor(9,1);
  lcd.print(timeRemaining);
  
  switch(getButtonState()){
    case 1:
      menu(0);
    break;
    case 2:
      menu(255);
    break;
    case 5:
      menu(128);
    break;
    case 6:
      dispense();
    break;
    default:
    break;
  }

}

void backlightSet(){
  //Serial.println(analogRead(A6));
  if (analogRead(A6) < dimThres){
    analogWrite(backlightPin, dimBrightness);
  }
  else{
    analogWrite(backlightPin, brightness);
  }
}

void blinkIt(){
  digitalWrite(ackLED, 1);
  delay(100);
  digitalWrite(ackLED, 0);
}
