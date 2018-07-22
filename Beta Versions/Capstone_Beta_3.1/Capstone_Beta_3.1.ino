/******************************
James Solonika

Portland Community College
EET 256
Spring 2015
Capstone Project

Beta v3.0
AND  Tester:
Check the function of the inserted chip
and determine if it is a functioning AND,
or non-functioning. Full counter spread
requides an array with 256 values to compare.
AND array is stored in EEPROM bytes 0 to 255.
*******************************/

//Includes
#include <EEPROM.h>

//Set Pin Assingments
const int red_LED = 9;
const int AND_LED = 8;
const int OR_LED = 7;
const int NAND_LED = 6;
const int NOR_LED = 5;
const int XOR_LED = 4;
const int logic_interrupt = 1;
const int opamp_interrupt = 2;
const int clock = 11;
const int reset = 12;
const int AND_Y[4] = {A3,A2,A1,A0};

//timing constants
const int chip_delay = 0.00001; //clock speed for counter chip
const int visual_delay = 0; //optional slow down for visual checking

//EEPROM Address Offsets
const int OR_Offset = 256;
const int NAND_Offset = 256 + 256;
const int NOR_Offset = 256 + 256 + 256;

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
  pinMode(reset, OUTPUT);
  
  pinMode(AND_LED, OUTPUT);
  pinMode(red_LED, OUTPUT);
  
  //Set input pins
  pinMode(logic_interrupt, INPUT);
  pinMode(opamp_interrupt, INPUT);
  
  //Initialize Outputs
  digitalWrite(AND_LED, LOW);
  digitalWrite(red_LED, LOW);
  
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
  
  
}

void test_logic_func()
{
  // temp variables
  int i, j; //loop counters
  
  // logic storage
  boolean is_and = true;
  int output_array[5] = {0,0,0,0,0};
  
  digitalWrite(reset, HIGH);
  delay(10);
  digitalWrite(reset, LOW);
  
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
       
     Serial.print(i);
     Serial.print(" - ");  
     Serial.println(output_array[4]);
     
     if (output_array[4] != EEPROM.read(i))
       is_and = false;
     
     output_array[4] = 0;
     
     //clocking at end so program begins with all outputs at 0
     digitalWrite(clock, HIGH);
     delay(chip_delay); //let chip sense clock
     digitalWrite(clock, LOW);
     delay(visual_delay); //insert pause for visual verification
  }
  
  if (is_and == true)
  {
    digitalWrite(AND_LED, HIGH);
    digitalWrite(red_LED, LOW);
  }
  else
  {
    digitalWrite(AND_LED, LOW);
    digitalWrite(red_LED, HIGH);
  }
}

void test_opamp_func() {
  
}
