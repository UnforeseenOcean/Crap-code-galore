#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 
 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;
String serialStr;
char msg[4];

void setup()
{
    // Initialize ASK Object
    rf_driver.init();
    Serial.begin(9600);
    pinMode(13, OUTPUT);
}
 
void loop()
{ 
    if (Serial.available() > 0) {
    serialStr = Serial.readString();

    serialStr.toCharArray(msg, 4);
    
    //char msg[4] = "u";
    digitalWrite(13, HIGH);
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    digitalWrite(13, LOW);
    
    Serial.println("Debug:");
    for(int i = 0; i < sizeof(msg); i++){
          Serial.print(msg[i],HEX);
          Serial.print(" ");
        }
        Serial.println("-- EOF --");
    delay(1500);
}
}
