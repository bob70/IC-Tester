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
Logic array is stored in EEPROM
*******************************/

//Includes
#include <EEPROM.h>

//Set Pin Assingments
//LEDS
const int DMUX_A = 9;
const int DMUX_B = 8;
const int DMUX_C = 7;
const int ready_LED = 13;
const int op_amp_good_LED = 10;
const int op_amp_bad_LED = 6;

//Buttons
const int logic_interrupt = 2;

//Logic
const int clock = 11;
const int reset = 12;
const int op_amp_in_1 = 5;
const int op_amp_in_2 = 4;

//Inputs
const int AND_Y[4] = {A3,A2,A1,A0};
const int op_amp_out_1 = A4;
const int op_amp_out_2 = A5;

//timing constants
const int chip_delay = 1; //clock speed for counter chip
const int visual_delay = 0; //optional slow down for visual checking
const int op_amp_delay = 100; //time to allow op amp to propagate

//EEPROM Address Offsets
const int OR_Offset = 256;
const int NAND_Offset = 512;
const int NOR_Offset = 768;

//Op Amp Constants
const int high_value = 590;
const int low_value = 585;

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
  Serial.begin(115200);
  interrupts();
  
  //set interrupt
  attachInterrupt(0, test_logic_func, FALLING);
  
  // Set Input Pins
  for (int k = 0; k < 4; k++)
    pinMode(AND_Y[k], INPUT);
  
  //Set Output Pins
  pinMode(clock, OUTPUT);
  pinMode(reset, OUTPUT);
  
  pinMode(ready_LED, OUTPUT);
  pinMode(op_amp_good_LED, OUTPUT);
  pinMode(op_amp_bad_LED, OUTPUT);
  
  pinMode(DMUX_A, OUTPUT);
  pinMode(DMUX_B, OUTPUT);
  pinMode(DMUX_C, OUTPUT);
  
  pinMode(op_amp_in_1, OUTPUT);
  pinMode(op_amp_in_2, OUTPUT);
  
  //Set input pins
  pinMode(logic_interrupt, INPUT);
  pinMode(op_amp_out_1, INPUT);
  pinMode(op_amp_out_2, INPUT);
  
  //Initialize outputs
  digitalWrite(ready_LED, LOW);
  digitalWrite(clock, LOW);
  digitalWrite(reset, HIGH);
  digitalWrite(op_amp_good_LED, LOW);
  digitalWrite(op_amp_bad_LED, LOW);
  digitalWrite(op_amp_in_1, LOW);
  digitalWrite(op_amp_in_2, LOW);
  set_DMUX(7);
  
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
    detachInterrupt(0);
    noInterrupts();
    
  digitalWrite(op_amp_good_LED, LOW);
  digitalWrite(op_amp_bad_LED, LOW);
  digitalWrite(ready_LED, LOW);
  set_DMUX(7);
  
  
  /*******
  Test logic
  *******/  
  
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
  
  
  digitalWrite(reset, HIGH);
  delay(10);
  digitalWrite(reset, LOW);
  
  for (i = 0; i < 256; i++)
  {
    //blink ready LED to indicate program is running
     n = i;
     if (n % 30 == 0)
       digitalWrite(ready_LED, !digitalRead(ready_LED));
     
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
     
     Serial.print("i - ");
     Serial.println(i);
     Serial.print("Value - ");  
     Serial.println(output_array[4]);
     
     
     if (output_array[4] != EEPROM.read(i))
       is_and = false;
     
      
     Serial.print("AND - ");  
     Serial.print(EEPROM.read(i));  
     Serial.print(" - ");
     Serial.println(is_and);
     
       
       n = i;
       n += OR_Offset;
     if (output_array[4] != EEPROM.read(n))
       is_or = false;
     
     
     Serial.print("OR - ");  
     Serial.print(EEPROM.read(n));  
     Serial.print(" - ");
     Serial.println(is_or);  
     
     
       n = i;
       n += NAND_Offset;
     if (output_array[4] != EEPROM.read(n))
       is_nand = false;
     
     
     Serial.print("NAND - ");  
     Serial.print(EEPROM.read(n));    
     Serial.print(" - ");
     Serial.println(is_nand);  
     
       
       n = i;
       n += NOR_Offset;
     if (output_array[4] != EEPROM.read(n))
       is_nor = false;
     
      
     Serial.print("NOR - ");  
     Serial.print(EEPROM.read(n));
     Serial.print(" - ");
     Serial.println(is_nor);  
     
     
     if (output_array[4] != XOR_Table[i])
       is_xor = false;
     
     
     Serial.print("XOR - ");  
     Serial.print(XOR_Table[i]);  
     Serial.print(" - ");
     Serial.println(is_xor);
     Serial.print('\n');  
     
     
     output_array[4] = 0;
     
     //clocking at end so program begins with all outputs at 0
     digitalWrite(clock, HIGH);
     delay(chip_delay); //let chip sense clock
     digitalWrite(clock, LOW);
     delay(visual_delay); //insert pause for visual verification
  }
  
  if (is_and == true)
    set_DMUX(1);
  else if (is_or == true)
    set_DMUX(2);
  else if (is_nand == true)
    set_DMUX(3);
  else if (is_nor == true)
    set_DMUX(4);
  else if (is_xor == true)
    set_DMUX(5);
  else
    set_DMUX(0);
    
       
       
  /*******
  Test op-amp
  *******/  
       
  
  
  boolean op_amp_good = true;
  
  //test first high, second low
  digitalWrite(op_amp_in_1, HIGH);
  digitalWrite(op_amp_in_2, LOW);
  delay(op_amp_delay);
  
  Serial.print("1 = HI, 2 = LO");
  Serial.print('\n');
  Serial.print("1 = ");
  Serial.print(analogRead(op_amp_out_1));
  Serial.print('\n');
  Serial.print("2 = ");
  Serial.print(analogRead(op_amp_out_2));
  Serial.print('\n');
  
  if (analogRead(op_amp_out_1) < high_value && analogRead(op_amp_out_2) > low_value)
    op_amp_good = false;
    
    
    
    //test both high
  digitalWrite(op_amp_in_1, HIGH);
  digitalWrite(op_amp_in_2, HIGH);
  delay(op_amp_delay);
  
  Serial.print("1 = HI, 2 = HI");
  Serial.print('\n');
  Serial.print("1 = ");
  Serial.print(analogRead(op_amp_out_1));
  Serial.print('\n');
  Serial.print("2 = ");
  Serial.print(analogRead(op_amp_out_2));
  Serial.print('\n');
  
  if (analogRead(op_amp_out_1) < high_value && analogRead(op_amp_out_2) < high_value)
    op_amp_good = false;
    
    
    
     //test first low, second high
  digitalWrite(op_amp_in_1, LOW);
  digitalWrite(op_amp_in_2, HIGH);
  delay(op_amp_delay);
  
  Serial.print("1 = LO, 2 = HI");
  Serial.print('\n');
  Serial.print("1 = ");
  Serial.print(analogRead(op_amp_out_1));
  Serial.print('\n');
  Serial.print("2 = ");
  Serial.print(analogRead(op_amp_out_2));
  Serial.print('\n');
  
  if (analogRead(op_amp_out_1) > low_value && analogRead(op_amp_out_2) < high_value)
    op_amp_good = false;
    
    
    
  //test both low
  digitalWrite(op_amp_in_1, LOW);
  digitalWrite(op_amp_in_2, LOW);
  delay(op_amp_delay);
  
  Serial.print("1 = LO, 2 = LO");
  Serial.print('\n');
  Serial.print("1 = ");
  Serial.print(analogRead(op_amp_out_1));
  Serial.print('\n');
  Serial.print("2 = ");
  Serial.print(analogRead(op_amp_out_2));
  Serial.print('\n');
  
  if (analogRead(op_amp_out_1) > low_value && analogRead(op_amp_out_2) > low_value)
    op_amp_good = false;
  
  Serial.print('\n');
  Serial.print(op_amp_good);
  Serial.print('\n');
  
  if (op_amp_good == true)
    digitalWrite(op_amp_good_LED, HIGH);
  else
    digitalWrite(op_amp_bad_LED, HIGH);
  
  interrupts();
  attachInterrupt(0, test_logic_func, FALLING);
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
