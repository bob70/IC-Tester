/******************************
James Solonika

Portland Community College
EET 256
Spring 2015
Capstone Project

Beta v2.0
AND Tester:
*******************************/

//Includes

//Set Pin Assingments
const int yellow_LED = 7;
const int green_LED = 4;

//global temp variables
int i; //loop counter
int temp; //misc
bool good_or_bad;

//timing constants
const int delay_time = 500;

//global IO configurations
const int Output_1 = 13;
const int Output_2 = 12;
const int AND_Y[4] = {11,10,9,8};

//global logic storage
int AND_Result[4][4];

//global logic compare tables
const int AND_Good[4][4] = {
  {LOW,LOW,LOW,LOW},
  {LOW,LOW,LOW,LOW},
  {LOW,LOW,LOW,LOW},
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
  
  pinMode(green_LED, OUTPUT);
  pinMode(yellow_LED, OUTPUT);
  
  //Initialize Outputs
  digitalWrite(green_LED, LOW);
  digitalWrite(yellow_LED, LOW);
  for (i  =0; i < 4; i++)
    digitalWrite(AND_Y[i], LOW);
  
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
    AND_Result[0][i] = digitalRead(AND_Y[i]);
  
  
  //check all 0,1
  digitalWrite(Output_1, HIGH);
  delay(delay_time);
  for (i=0; i < 4; i++)
    AND_Result[1][i] = digitalRead(AND_Y[i]);
     
     
  //check all 1,0
  digitalWrite(Output_1, LOW);
  digitalWrite(Output_2, HIGH);   
  delay(delay_time);
  for (i=0; i < 4; i++)
    AND_Result[2][i] = digitalRead(AND_Y[i]);   
  
  
  //check all ones
  digitalWrite(Output_1, HIGH);   
  delay(delay_time);
  for (i=0; i < 4; i++)
    AND_Result[3][i] = digitalRead(AND_Y[i]);
  
  //chec results
  for (i = 0; i < 4; i++)
  {
    Serial.println();
    for (temp = 0; temp < 4; temp++)
    {
     Serial.print(i);
     Serial.print(": ");
     Serial.print(AND_Result[i][temp]);
     if (AND_Good[i][temp] == AND_Result[i][temp])
       good_or_bad = true;
     else
       good_or_bad = false;
    }
  };
    
  if (good_or_bad == true)
     {
       digitalWrite(green_LED, HIGH);
       digitalWrite(yellow_LED, LOW); 
     }
     else
     {
       digitalWrite(green_LED, LOW);
       digitalWrite(yellow_LED, HIGH);  
     }
  
}
