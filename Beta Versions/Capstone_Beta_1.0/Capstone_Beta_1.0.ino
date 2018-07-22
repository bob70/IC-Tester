/******************************
James Solonika

Portland Community College
EET 256
Spring 2015
Capstone Project

Beta v1.0
AND Tester:
*******************************/

//Includes

//Set Pin Assingments
const int green_LED = 0;
const int red_LED = 7;

//global temp variables
int i; //loop counter
int temp; //misc

//global IO configurations
 int AND_In[8] = {1,2,4,5,9,10,12,13};
 int AND_Out[4] = {3,6,8,11};

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
  for (i=0; i < 8; i++)
  {
    pinMode(AND_In[i], OUTPUT);
  }
  
  //Set Output Pins
  for (i=0; i < 4; i++)
  {
    pinMode(AND_Out[i], INPUT);
  }
  
  pinMode(green_LED, OUTPUT);
  pinMode(red_LED, OUTPUT);
  
  //Initialize LEDs
  digitalWrite(green_LED, LOW);
  digitalWrite(red_LED, LOW);
  
  Serial.print("*********************");
  Serial.print('\n');
  Serial.print("Welcome to AND Checker.");
  Serial.print('\n');
  Serial.print("Program has begun.");
  Serial.print('\n');
  Serial.print("******************************");
  for (i = 0; i < 3; i++)
    Serial.print('\n');
}

void loop() {
  
  //check all zeroes
  for (i=0; i < 8; i++)
     digitalWrite(AND_In[i], LOW);
  
  delay(10);
  
  for (i=0; i < 4; i++)
    AND_Result[0][i] = digitalRead(AND_Out[i]);
  
  
  //check all 0,1
  digitalWrite(AND_In[0], LOW);
  for (i=1; i < 8; i++)
     digitalWrite(AND_In[i], !digitalRead(AND_In[i-1]));
  
  delay(10);
  
  for (i=0; i < 4; i++)
    AND_Result[1][i] = digitalRead(AND_Out[i]);
     
     
     
  //check all 1,0
  digitalWrite(AND_In[0], HIGH);
  for (i=1; i < 8; i++)
     digitalWrite(AND_In[i], !digitalRead(AND_In[i-1]));   
  
  delay(10);
  
  for (i=0; i < 4; i++)
    AND_Result[2][i] = digitalRead(AND_Out[i]);   
  
  
  
  //check all ones
  for (i=0; i < 8; i++)
     digitalWrite(AND_In[i], HIGH);     
  
  delay(10);
  
  for (i=0; i < 4; i++)
    AND_Result[3][i] = digitalRead(AND_Out[i]);
  
  for (i = 0; i < 4; i++)
  {
    Serial.println();
    for (temp = 0; temp < 4; temp++)
    {
     Serial.print(i);
     Serial.print(": ");
     Serial.print(AND_Result[i][temp]);
     if (AND_Good[i][temp] != AND_Result[i][temp])
       digitalWrite(red_LED, HIGH);
    }
  }
  if (digitalRead(red_LED) == LOW);
    digitalWrite(green_LED, HIGH);
    
  
}
