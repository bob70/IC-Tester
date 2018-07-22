
/********************************
Component tester
James Solonika
V 1.0

First version tests logic chips only
with pin layout 7 = ground, 14 = power
ABY repeating pin pattern

Last Updated 12/17/15
*********************************/
#include <EEPROM.h>

//EEPROM offset values
const int OR_OFFSET = 256;
const int NAND_OFFSET = 256 + 256;
const int NOR_OFFSET = 256 + 256 + 256;
const int XOR_OFFSET = 256 + 256 + 256 + 256;

//Logic chip pin map - chip pin 0 does not exist. 0 is a temp value in array 0
const int LOGIC_MAP[15] = {0,31,32,33,34,35,36,37,38,39,40,41,42,43,44};

//Inputs
const int LOGIC_Y[4]={LOGIC_MAP[3],LOGIC_MAP[6],LOGIC_MAP[8],LOGIC_MAP[11]};

//Outputs
const int LOGIC_AB[8]={LOGIC_MAP[1],LOGIC_MAP[2],LOGIC_MAP[4],LOGIC_MAP[5],LOGIC_MAP[9],LOGIC_MAP[10],LOGIC_MAP[12],LOGIC_MAP[13]};
const int VCC = LOGIC_MAP[14];
const int GND = LOGIC_MAP[7];

//Constants
const int visual_delay = 200;

void setup() {
  // Begin processes
  Serial.begin(9600);

  //Set input pins
  for (int k = 0; k < 4; k++)
    pinMode(LOGIC_Y[k], INPUT);

  //Set output pins
  for (int k = 0; k < 8; k++)
    pinMode(LOGIC_AB[k], OUTPUT);

  pinMode(VCC, OUTPUT);
  pinMode(GND, OUTPUT);

  //Initialize outputs
  digitalWrite(GND, LOW);
  digitalWrite(VCC, HIGH);
}
void loop() {
   // temp variables
  int i, j; //loop counters
  int n; //temp variable
  byte BINARY_VALUE;
  byte BINARY_MASK = B00000001;
  // logic storage
  boolean is_and = true;
  boolean is_or = true;
  boolean is_nand = true;
  boolean is_nor = true;
  boolean is_xor = true;
  int output_array[5] = {0,0,0,0,0};

  for (i= 0; i < 8; i++)
  {
     digitalWrite(LOGIC_AB[i], LOW);
  }

  for (i = 0; i < 256; i++)
  {
      for (j = 0; j < 4; j++)
     {
       output_array[j] = digitalRead(LOGIC_Y[j]);
     }
         
     if (output_array[0] == 1)
       output_array[4] += 8;
     if (output_array[1] == 1)
       output_array[4] += 4;
     if (output_array[2] == 1)
       output_array[4] += 2;
     if (output_array[3] == 1)
       output_array[4] += 1;
  
      
     Serial.print("i - ");
     Serial.println(i);
     Serial.print("Value - ");  
     Serial.println(output_array[4]);
     
     
     if (output_array[4] != EEPROM.read(i))
       is_and = false;
     
     /* 
     Serial.print("AND - ");  
     Serial.print(EEPROM.read(i));  
     Serial.print(" - ");
     Serial.println(is_and);
     */
       
       n = i;
       n += OR_OFFSET;
     if (output_array[4] != EEPROM.read(n))
       is_or = false;

     
     Serial.print("OR - ");  
     Serial.print(EEPROM.read(n));  
     Serial.print(" - ");
     Serial.println(is_or);  
     
     
       n = i;
       n += NAND_OFFSET;
     if (output_array[4] != EEPROM.read(n))
       is_nand = false;
     
     /*
     Serial.print("NAND - ");  
     Serial.print(EEPROM.read(n));    
     Serial.print(" - ");
     Serial.println(is_nand);  
     */
       
       n = i;
       n += NOR_OFFSET;
     if (output_array[4] != EEPROM.read(n))
       is_nor = false;
     
      /*
     Serial.print("NOR - ");  
     Serial.print(EEPROM.read(n));
     Serial.print(" - ");
     Serial.println(is_nor);  
     */
     
     n = i;
     n += XOR_OFFSET;
     if (output_array[4] != EEPROM.read(n))
       is_xor = false;
     
     /*
     Serial.print("XOR - ");  
     Serial.print(EEPROM.read(n)); 
     Serial.print(" - ");
     Serial.println(is_xor);
     Serial.print('\n');  
    */
    
     output_array[4] = 0;

     BINARY_VALUE = byte(i);
     for (j = 0; j < 8; j++)
     {
      
        /*
        Serial.print("BINARY_VALUE = "),Serial.print(BINARY_VALUE);
        Serial.print('\n');
        Serial.print("BINARY_MASK = "),Serial.print(BINARY_MASK);
        Serial.print('\n');
        Serial.print('\t'),Serial.print("& = "),Serial.print(BINARY_VALUE & BINARY_MASK);
        Serial.print('\n');
        */    
     
        if (BINARY_VALUE & BINARY_MASK)
        {
          digitalWrite(LOGIC_AB[j], HIGH);
         // Serial.print(j),Serial.print(" - 1"); 
        }
        else
        {
          digitalWrite(LOGIC_AB[j], LOW);
          //Serial.print(j),Serial.print(" - 0");
        }

      //Serial.print('\n');
      //Serial.print('\n');
      BINARY_MASK = BINARY_MASK << 1;
     }

      Serial.print('\n');
      BINARY_MASK = B00000001;
      delay(visual_delay);
      }

  }
