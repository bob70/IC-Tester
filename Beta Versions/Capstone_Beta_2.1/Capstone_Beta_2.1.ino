/******************************
James Solonika

Portland Community College
EET 256
Spring 2015
Capstone Project

Beta v2.1
AND & OR Tester:
Check the function of the inserted chip
and determine if it is a functioning AND,]
OR, or non-functioning.
*******************************/

//Includes

//Set Pin Assingments
const int yellow_LED = 7;
const int AND_LED = 4;
const int OR_LED = 5;

//global temp variables
int i; //loop counter
int temp; //misc
boolean is_and;
boolean is_or;

//timing constants
const int delay_time = 400;

//global IO configurations
const int Output_1 = 13;
const int Output_2 = 12;
const int AND_Y[4] = {11,10,9,8};

//global logic storage
int Logic_Result[4][4];

//global logic compare tables
const int AND_Good[4][4] = {
  {LOW,LOW,LOW,LOW},
  {LOW,LOW,LOW,LOW},
  {LOW,LOW,LOW,LOW},
  {HIGH,HIGH,HIGH,HIGH}
};

const int OR_Good[4][4] = {
  {LOW,LOW,LOW,LOW},
  {HIGH,HIGH,HIGH,HIGH},
  {HIGH,HIGH,HIGH,HIGH},
  {HIGH,HIGH,HIGH,HIGH}
};

void setup() {  
  
  //Misc Processes
  Serial.begin(9600);
  
  // Set Input Pins
  for (i  =0; i < 4; i++)
    pinMode(AND_Y[i], INPUT);
  
  //Set Output Pins
  pinMode(Output_1, OUTPUT);
  pinMode(Output_2, OUTPUT);
  
  pinMode(AND_LED, OUTPUT);
  pinMode(OR_LED, OUTPUT);
  pinMode(yellow_LED, OUTPUT);
  
  //Initialize Outputs
  digitalWrite(AND_LED, LOW);
  digitalWrite(OR_LED, LOW);
  digitalWrite(yellow_LED, LOW);
  for (i  =0; i < 4; i++)
    digitalWrite(AND_Y[i], LOW);
  
  //Initial Serial message
  Serial.print("*********************");
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
  
  //check all zeroes
  digitalWrite(Output_1, LOW);
  digitalWrite(Output_2, LOW);
  delay(delay_time);
  for (i=0; i < 4; i++)
    Logic_Result[0][i] = digitalRead(AND_Y[i]);
  
  
  //check all 0,1
  digitalWrite(Output_1, HIGH);
  delay(delay_time);
  for (i=0; i < 4; i++)
    Logic_Result[1][i] = digitalRead(AND_Y[i]);
     
     
  //check all 1,0
  digitalWrite(Output_1, LOW);
  digitalWrite(Output_2, HIGH);   
  delay(delay_time);
  for (i=0; i < 4; i++)
    Logic_Result[2][i] = digitalRead(AND_Y[i]);   
  
  
  //check all ones
  digitalWrite(Output_1, HIGH);   
  delay(delay_time);
  for (i=0; i < 4; i++)
    Logic_Result[3][i] = digitalRead(AND_Y[i]);
  
  //check results
  is_and = true;
  is_or = true;
  for (i = 0; i < 4; i++)
  {
    for (temp = 0; temp < 4; temp++)
    {
     if (AND_Good[i][temp] != Logic_Result[i][temp])
       is_and = false;
       
     if (OR_Good[i][temp] != Logic_Result[i][temp])
       is_or = false;
    }
  };
    
  if (is_and == true)
     {
       digitalWrite(AND_LED, HIGH);
       digitalWrite(yellow_LED, LOW); 
     }
  else if (is_or == true && is_and == false)
     {
       digitalWrite(OR_LED, HIGH);
       digitalWrite(yellow_LED, LOW);  
     }
  else
     {
       digitalWrite(yellow_LED, HIGH);
       digitalWrite(OR_LED, LOW);
       digitalWrite(AND_LED, LOW);
     }
}
