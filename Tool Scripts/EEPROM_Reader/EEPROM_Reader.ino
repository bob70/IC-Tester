#include <EEPROM.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  for (int i = 0; i < 1025; i++)
  {
    Serial.print(i);
    Serial.print(" - ");
    Serial.println(EEPROM.read(i));
  }
    
}

void loop() {
  // put your main code here, to run repeatedly:

}
