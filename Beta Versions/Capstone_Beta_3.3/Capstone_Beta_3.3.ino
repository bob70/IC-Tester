/******************************
James Solonika

Portland Community College
EET 256
Spring 2015
Capstone Project

Beta v3.3
Logic  Tester:

--Prep Circuit for NOR--

Check the function of the inserted chip
and determine if it is a functioning AND, OR, NAND, NOR
or non-functioning. Full counter spread
requides an array with 256 values to compare.
Logic array is stored in EEPROM bytes 0 to 255.
*******************************/

//Includes
#include <EEPROM.h>

//Set Pin Assingments
//LEDS
const int ready_LED = 13;
const int red_LED = 9;
const int AND_LED = 8;
const int OR_LED = 7;
const int NAND_LED = 6;
const int NOR_LED = 5;
const int XOR_LED = 4;
//Buttons
const int logic_interrupt = 1;
const int opamp_interrupt = 2;
//Logic
const int clock = 11;
const int reset = 12;
//Inputs
const int AND_Y[4] = {A3,A2,A1,A0};
//Outputs
const int AB_Select = 10;

//timing constants
const int chip_delay = 0.00001; //clock speed for counter chip
const int visual_delay = 0; //optional slow down for visual checking

//EEPROM Address Offsets
const int OR_Offset = 256;
const int NAND_Offset = 512;
const int NOR_Offset = 768;

//XOR Table
const int XOR_Table[256] = {
  0,1,1,0,2,3,3,2,2,3,3,2,0,3,1,0,
  4,5,5,4,6,7,7,6,6,7,7,6,4,7,5,4,
  4,5,5,4,6,7,7,6,6,7,7,6,4,7,5,4,
  0,1,1,0,2,3,3,2,2,3,3,2,0,3,1,0,
  8,9,9,8,10,11,11,10,10,11,11,10,8,11,9,8,
  12,13,13,12,14,15,15,14,14,15,15,14,12,15,13,12,
  12,13,13,12,14,15,15,14,14,15,15,14,12,15,13,12,
  8,9,9,8,10,11,11,10,10,11,11,10,8,11,9,8,
  8,9,9,8,10,11,11,10,10,11,11,10,8,11,9,8,
  12,13,13,12,14,15,15,14,14,15,15,14,12,15,13,12,
  12,13,13,12,14,15,15,14,14,15,15,14,12,15,13,12,
  8,9,9,8,10,11,11,10,10,11,11,10,8,11,9,8,
  0,1,1,0,2,3,3,2,2,3,3,2,0,3,1,0,
  4,5,5,4,6,7,7,6,6,7,7,6,4,7,5,4,
  4,5,5,4,6,7,7,6,6,7,7,6,4,7,5,4,
  0,1,1,0,2,3,3,2,2,3,3,2,0,3,1,0
};

void setup() {  
  
  //Begin Processes
  Serial.begin(9600);
  interrupts();
  
  //set interrupts
  attachInterrupt(0, test_logic_func, FALLING);
  attachInterrupt(1, test_opamp_func, FALLING);
  
  // Set Input Pins
  /*
  for (int k = 0; k < 4; k++)
    pinMode(AND_Y[k], INPUT);
  */
  
  //Set Output Pins
  pinMode(clock, OUTPUT);
  pinMode(reset, OUTPUT);
  
  pinMode(AND_LED, OUTPUT);
  pinMode(NAND_LED, OUTPUT);
  pinMode(OR_LED, OUTPUT);
  pinMode(NOR_LED, OUTPUT);
  pinMode(XOR_LED, OUTPUT);
  pinMode(ready_LED, OUTPUT);
  pinMode(red_LED, OUTPUT);
  
  //Set input pins
  pinMode(logic_interrupt, INPUT);
  pinMode(opamp_interrupt, INPUT);
  
  //Initialize Outputs
  digitalWrite(AND_LED, LOW);
  digitalWrite(NAND_LED, LOW);
  digitalWrite(OR_LED, LOW);
  digitalWrite(NOR_LED, LOW);
  digitalWrite(XOR_LED, LOW);
  digitalWrite(red_LED, LOW);
  digitalWrite(ready_LED, LOW);
  
  digitalWrite(clock, LOW);
  digitalWrite(reset, HIGH);
  
  //Initial Serial message
  Serial.println("*********************");
  Serial.print('\n');
  Serial.print("Welcome to AND Checker.");
  Serial.print('\n');
  Serial.print("Program has begun.");
  Serial.print('\n');
  Serial.print("******************************");
  Serial.print('\n');
  Serial.print('\n');
  Serial.print('\n');
}

void loop() {
  digitalWrite(ready_LED, HIGH);
  
}

void test_logic_func()
{
  digitalWrite(ready_LED, LOW);
  
  // temp variables
  int i, j; //loop counters
  int n; //temp variable
  // logic storage
  boolean is_and = true;
  boolean is_or = true;
  boolean is_nand = true;
  boolean is_nor = true;
  boolean is_xor = true;
  int output_array[5] = {0,0,0,0,0};
  int output_array2[5] = {0,0,0,0,0};
  
  //initialize LEDs
  digitalWrite(red_LED, LOW);
  digitalWrite(AND_LED, LOW);
  digitalWrite(OR_LED, LOW);
  digitalWrite(NAND_LED, LOW);
  digitalWrite(NOR_LED, LOW);
  digitalWrite(XOR_LED, LOW);
  
  digitalWrite(reset, HIGH);
  delay(10);
  digitalWrite(reset, LOW);
  
  for (i = 0; i < 256; i++)
  {
     
     
     for (j = 0; j < 4; j++)
     {
       output_array[j] = digitalRead(AND_Y[j]);
     }
     
     //Read for AND, OR, NAND
     digitalWrite(AB_Select, LOW);
     if (output_array[0] == 1)
       output_array[4] += 8;
     if (output_array[1] == 1)
       output_array[4] += 4;
     if (output_array[2] == 1)
       output_array[4] += 2;
     if (output_array[3] == 1)
       output_array[4] += 1;
       
     //Read for NOR
     digitalWrite(AB_Select, HIGH);
     if (output_array2[0] == 1)
       output_array2[4] += 8;
     if (output_array2[1] == 1)
       output_array2[4] += 4;
     if (output_array2[2] == 1)
       output_array2[4] += 2;
     if (output_array2[3] == 1)
       output_array2[4] += 1;
       
     Serial.print(i);
     Serial.print(" - ");  
     Serial.print(output_array[4]);
     
     for (int k = 0; k < 4; k++)
      pinMode(AND_Y[k], INPUT);
     
     if (output_array[4] != EEPROM.read(i))
       is_and = false;
     
     /*  
     Serial.print(" - ");  
     Serial.print(EEPROM.read(i));  
     Serial.print(" - ");
     Serial.println(is_and);
     */
       
       n = i;
       n += OR_Offset;
     if (output_array[4] != EEPROM.read(n))
       is_or = false;
     
     /*
     Serial.print(" - ");  
     Serial.print(EEPROM.read(n));  
     Serial.print(" - ");
     Serial.println(is_or);  
     */
     
       n = i;
       n += NAND_Offset;
     if (output_array[4] != EEPROM.read(n))
       is_nand = false;
     
     
     Serial.print(" - ");  
     Serial.print(EEPROM.read(n));    
     Serial.print(" - ");
     Serial.println(is_nand);  
    
       n = i;
       n += NOR_Offset;
     if (output_array2[4] != EEPROM.read(n))
       is_nor = false;
     
     /*  
     Serial.print(" - ");  
     Serial.print(EEPROM.read(n));
     Serial.print(" - ");
     Serial.println(is_nor);  
     */
     
     if (output_array[4] != XOR_Table[i])
       is_xor = false;
     
     /*
     Serial.print(" - ");  
     Serial.print(XOR_Table[i]);  
     Serial.print(" - ");
     Serial.println(is_xor);  
     */
     
     output_array[4] = 0;
     
     //clocking at end so program begins with all outputs at 0
     digitalWrite(clock, HIGH);
     delay(chip_delay); //let chip sense clock
     digitalWrite(clock, LOW);
     delay(visual_delay); //insert pause for visual verification
  }
  
  if (is_and == true)
    digitalWrite(AND_LED, HIGH);
  else if (is_or == true)
    digitalWrite(OR_LED, HIGH);
  else if (is_nand == true)
    digitalWrite(NAND_LED, HIGH);
  else if (is_nor == true)
    digitalWrite(NOR_LED, HIGH);
  else if (is_xor == true)
    digitalWrite(XOR_LED, HIGH);
  else
    digitalWrite(red_LED, HIGH);
    
}

void test_opamp_func() {
  
}
