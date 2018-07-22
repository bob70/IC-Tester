/*******************************
James Solonika

Portland Community College
EET 256
Spring 2015
Capstone Project

Beta v3.3
Logic  Tester:


Check the function of the inserted chip
and determine if it is a functioning AND, OR, NAND,
or non-functioning. Full counter spread
requides an array with 256 values to compare.
Logic array is stored in EEPROM bytes 0 to 255.
*******************************/

//Includes
#include <EEPROM.h>

//Set Pin Assingments
//LEDS
const int DMUX_A = 9;
const int DMUX_B = 8;
const int DMUX_C = 7;
const int ready_LED = 13;
//Buttons
const int logic_interrupt = 2;
const int opamp_interrupt = 3;
//Logic
const int clock = 11;
const int clock2 = 5;
const int reset = 12;

//Inputs
const int AND_Y[4] = {A3,A2,A1,A0};
//Outputs
const int AB_Select = 4;
const int HIGHZ_Select = 10;

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
  for (int k = 0; k < 4; k++)
    pinMode(AND_Y[k], INPUT);
  
  //Set Output Pins
  pinMode(clock, OUTPUT);
  pinMode(clock2, OUTPUT);
  pinMode(reset, OUTPUT);
  pinMode(AB_Select, OUTPUT);
  pinMode(HIGHZ_Select, OUTPUT);
  pinMode(ready_LED, OUTPUT);
  
  pinMode(DMUX_A, OUTPUT);
  pinMode(DMUX_B, OUTPUT);
  pinMode(DMUX_C, OUTPUT);
  
  //Set input pins
  pinMode(logic_interrupt, INPUT);
  pinMode(opamp_interrupt, INPUT);
  
  //Initialize outputs
  digitalWrite(clock, LOW);
  digitalWrite(clock2, LOW);
  digitalWrite(reset, HIGH);
  digitalWrite(AB_Select, LOW);
  digitalWrite(HIGHZ_Select, LOW);
  
  //Initial Serial message
  Serial.println("*********************");
  Serial.print('\n');
  Serial.print("Welcome to LOGIC Checker.");
  Serial.print('\n');
  Serial.print("Program has begun.");
  Serial.print('\n');
  Serial.print("******************************");
  Serial.print('\n');
  Serial.print('\n');
  Serial.print('\n');
  
  set_DMUX(7);
}

void loop() {
  digitalWrite(ready_LED, HIGH);
}

void test_logic_func()
{
  set_DMUX(7);
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
  boolean is_and2 = true;
  boolean is_or2 = true;
  boolean is_nand2 = true;
  boolean is_nor2 = true;
  boolean is_xor2 = true;
  int output_array[5] = {0,0,0,0,0};
  int output_array2[5] = {0,0,0,0,0};
  
  //clear counters
  digitalWrite(reset, HIGH);
  delay(10);
  digitalWrite(reset, LOW);
  
  //Prepare for counter one
  digitalWrite(HIGHZ_Select, LOW);
  digitalWrite(AB_Select, LOW);
  
  for (i = 0; i < 256; i++)
  {
     
     
     for (j = 0; j < 4; j++)
     {
       output_array[j] = digitalRead(AND_Y[j]);
     }
     
     if (output_array[0] == 1)
       output_array[4] += 8;
     if (output_array[1] == 1)
       output_array[4] += 4;
     if (output_array[2] == 1)
       output_array[4] += 2;
     if (output_array[3] == 1)
       output_array[4] += 1;
     
     /*  
     Serial.print(i);
     Serial.print(" - ");  
     Serial.print(output_array[4]);
     */
     
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
     
     /*
     Serial.print(" - ");  
     Serial.print(EEPROM.read(n));    
     Serial.print(" - ");
     Serial.println(is_nand);  
     */
       
       n = i;
       n += NOR_Offset;
     if (output_array[4] != EEPROM.read(n))
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
  
  
  //clear counters
  digitalWrite(reset, HIGH);
  delay(10);
  digitalWrite(reset, LOW);
  
  //Prepare for counter two
  digitalWrite(HIGHZ_Select, HIGH);
  digitalWrite(AB_Select, HIGH);
  
  for (i = 0; i < 256; i++)
  {
    for (j = 0; j < 4; j++)
     {
       output_array2[j] = digitalRead(AND_Y[j]);
     }
     
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
     Serial.print(output_array2[4]);
     
     if (output_array[4] != EEPROM.read(i))
       is_and2 = false;
     
     /*  
     Serial.print(" - ");  
     Serial.print(EEPROM.read(i));  
     Serial.print(" - ");
     Serial.println(is_and2);
     */
       
       n = i;
       n += OR_Offset;
     if (output_array2[4] != EEPROM.read(n))
       is_or2 = false;
     
     /*
     Serial.print(" - ");  
     Serial.print(EEPROM.read(n));  
     Serial.print(" - ");
     Serial.println(is_or2);  
     */
     
       n = i;
       n += NAND_Offset;
     if (output_array2[4] != EEPROM.read(n))
       is_nand2 = false;
     
     /*
     Serial.print(" - ");  
     Serial.print(EEPROM.read(n));    
     Serial.print(" - ");
     Serial.println(is_nand2);  
     */
       
       n = i;
       n += NOR_Offset;
     if (output_array2[4] != EEPROM.read(n))
       is_nor2 = false;
     
     
     Serial.print(" - ");  
     Serial.print(EEPROM.read(n));
     Serial.print(" - ");
     Serial.println(is_nor2);  
     
     
     if (output_array2[4] != XOR_Table[i])
       is_xor2 = false;
     
     /*
     Serial.print(" - ");  
     Serial.print(XOR_Table[i]);  
     Serial.print(" - ");
     Serial.println(is_xor2);  
     */
     
     output_array2[4] = 0;
    
    //clocking at end so program begins with all outputs at 0
     digitalWrite(clock2, HIGH);
     delay(chip_delay); //let chip sense clock
     digitalWrite(clock2, LOW);
     delay(visual_delay); //insert pause for visual verification
  }
  
  
  if (is_and == true || is_and2 == true)
    set_DMUX(1);
  else if (is_or == true || is_or2 == true)
    set_DMUX(2);
  else if (is_nand == true || is_nand2 == true)
    set_DMUX(3);
  else if (is_nor == true || is_nor2 == true)
    set_DMUX(4);
  else if (is_xor == true || is_xor2 == true)
    set_DMUX(5);
  else
    set_DMUX(0);
    
  //clear counters
  digitalWrite(reset, HIGH);
  delay(10);
  digitalWrite(reset, LOW);  
    
}

void test_opamp_func() {
  
}

void set_DMUX(int num) {
 if (num == 0)
 {
   digitalWrite(DMUX_A, LOW);
   digitalWrite(DMUX_B, LOW);
   digitalWrite(DMUX_C, LOW);
 }
 else if (num == 1)
 {
   digitalWrite(DMUX_A, HIGH);
   digitalWrite(DMUX_B, LOW);
   digitalWrite(DMUX_C, LOW);
 }
 else if (num == 2)
 {
   digitalWrite(DMUX_A, LOW);
   digitalWrite(DMUX_B, HIGH);
   digitalWrite(DMUX_C, LOW);
 }
 else if (num == 3)
 {
   digitalWrite(DMUX_A, HIGH);
   digitalWrite(DMUX_B, HIGH);
   digitalWrite(DMUX_C, LOW);
 }
 else if (num == 4)
 {
   digitalWrite(DMUX_A, LOW);
   digitalWrite(DMUX_B, LOW);
   digitalWrite(DMUX_C, HIGH);
 }
 else if (num == 5)
 {
   digitalWrite(DMUX_A, HIGH);
   digitalWrite(DMUX_B, LOW);
   digitalWrite(DMUX_C, HIGH);
 }
 else if (num == 6)
 {
   digitalWrite(DMUX_A, LOW);
   digitalWrite(DMUX_B, HIGH);
   digitalWrite(DMUX_C, HIGH);
 }
 else if (num == 7)
 {
   digitalWrite(DMUX_A, HIGH);
   digitalWrite(DMUX_B, HIGH);
   digitalWrite(DMUX_C, HIGH);
 }

}
