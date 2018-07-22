/***********************
James Solonika

Portland Community College
EET 242
Fall 2014
Final Project

Pill reminder:
This program reminds a user to take pills once
a day using a flashing LED. Inpiration was taken
from http://www.instructables.com/id/The-Pill-Reminder/
This version only reminds once a day for seven days. It 
allows the user to set the time per day that the pills
should be taken, by selecting set at the time of day 
the user wants (cannot be pre set). This program also 
displays log information on the Serial Monitor, including
how long it took a user to take a pill and how often a day
was missed.
Missed dose is indicated with a red LED, and is considered
missed if a pill is not taken within 1/2 of a cycle. The missed 
led will blink until the missed reset button is pressed.

For demonstration purposes, a "day" lasts a few seconds

**********************/


//////////////////////////////////////////////////////////////////////////

//Includes
#include <Wire.h>


//timing assingments

/***************************************
        --------------------  CHANGE LENGTH OF CYCLE HERE   -------------------- 
                                       |
                                       |
                                       v 
***************************************/
                          int cycle_hours = 0;
                          int cycle_minutes = 0;
                          int cycle_seconds = 8;
/***************************************
                                       ^
                                       |
                                       |
        --------------------  CHANGE LENGTH OF CYCLE HERE   --------------------                                       
***************************************/
int day_delay; //will be calculated in setup function

// cycle is set at 1 second. This effectivley sets the blink speed
// as well as how often data values are checked
const int delay_decrement = 1000;


//set pin assingments
const int day_pin[7] = {4,5,6,7,8,9,10};
const int missed_pill_LED = 11;

const int missed_reset = 2;
const int take_pill = 3;
const int set_day = 12;
const int end_set_day = 13;

//global loop temp variables
int i;


//global data variables
int day;
int remaining_delay;
const int wire_address = 0x40; //Transmission address set to Raspbery pi

int took_pill[7];
int missed_pill[7];
int missed_pill_notice[7];


//////////////////////////////////////////////////////////////////////////


void setup() {
  
  //calclate delay length of cycle
  cycle_minutes += cycle_hours * 60;
  cycle_seconds += cycle_minutes * 60;
  day_delay = cycle_seconds * 1000;  //delay function operates in miliseconds
  
  //set output pins
  for (i = 0; i < 7; i++)
    pinMode(day_pin[i], OUTPUT);
  
  pinMode(missed_pill_LED, OUTPUT);

  //clear all outputs
  for (i = 0; i < 7; i++)
    digitalWrite(day_pin[i], LOW);
  digitalWrite(day_pin[0], HIGH);
  
  digitalWrite(missed_pill_LED, LOW);
  
  //set input pins
  pinMode(missed_reset, INPUT);
  pinMode(take_pill, INPUT);
  pinMode(set_day, INPUT);
  pinMode(end_set_day, INPUT);

  //initialize global data variables
  day = 0;
  remaining_delay = day_delay;
  
  // for day truth vales
  // 0 = false. 1 = true. 2 = future day / unknown
  for (i = 0; i < 7; i++)
  {
    took_pill[i] = 2; 
    missed_pill[i] = 2;
    missed_pill_notice[i] = 2;
  }
    
  //begin misc processes
  Serial.begin(9600);
  Wire.begin();

  //set interrupts
  interrupts();
  attachInterrupt(0, missed_reset_func, FALLING);
  attachInterrupt(1, take_pill_func, FALLING);
  
  //Print welcome message
  Serial.print("******************************");
  Serial.print('\n');
  Serial.print("Welcome to pill reminder.");
  Serial.print('\n');
  Serial.print("Program has begun.");
  Serial.print('\n');
  Serial.print("******************************");
  for (i = 0; i < 3; i++)
    Serial.print('\n');
  
  //initialize day
  set_day_func();

}


//////////////////////////////////////////////////////////////////////////


void loop() {
  
  //Initialize day. Current day LED = ON, Previous day = OFF, Missed = OFF, Board indicator = OFF
  digitalWrite(day_pin[day], HIGH);
  digitalWrite(missed_pill_LED, LOW);
  print_day();
  Serial.print('\n');
  
  if (day == 0)
    digitalWrite(day_pin[6], LOW); 
  else
    digitalWrite(day_pin[day-1], LOW);
  
  //Have not taken pill
  took_pill[day] = 0;
  missed_pill[day] = 0;
  missed_pill_notice[day] = 0;
  
  //new day
  remaining_delay = day_delay;
  
  //loop for one day
  while (remaining_delay != 0)
  {
   //run for one incriment
   delay(delay_decrement);
   remaining_delay -= delay_decrement;
  
    //if user has not taken todays pill, blink that LED. If they have, turn it off
   if (took_pill[day] == 0 && missed_pill_notice[day] != 1)  //user has not taken pill
      digitalWrite(day_pin[day], !digitalRead(day_pin[day]));  //blink day LED
   else if (took_pill[day] == 1)  //user took pill
       digitalWrite(day_pin[day], LOW);
    
   //if it is halfway through the day, user has not taken pill and user has not noticed that they have not taken a pill 
   if (remaining_delay <= (day_delay/2) && took_pill[day] == 0 && missed_pill_notice[day] == 0) 
     digitalWrite(missed_pill_LED, HIGH);
    
  }
  
  //if a user missed a pill, record it
  if (took_pill[day] == 0)
    missed_pill[day] = 1;
  
    
  /************************
  display weeks values
  *************************/
  Serial.print("Displaying data as of: ");
  print_day();
  
  Serial.print('\n');
  
    //Table header
   Serial.print('\t'); Serial.print('\t'); Serial.print('\t'); Serial.print("Monday:"); Serial.print('\t'); Serial.print("Tuesday:"); Serial.print('\t');
   Serial.print("Wednesday:"); Serial.print('\t'); Serial.print("Thursday:"); Serial.print('\t'); Serial.print("Friday:"); Serial.print('\t');;
   Serial.print("Saturday:"); Serial.print('\t'); Serial.print("Sunday:");
   
   Serial.print('\n');
   
   //took pill data
  Serial.print("Took Pill:");
  Serial.print('\t');
  for (i = 0; i < 7; i++)
  {
    Serial.print('\t');
    
    //tab formatting
    if (i == 2 || i == 3 || i == 4 || i ==6)
        Serial.print('\t');
      
    if (took_pill[i] == 0)
      Serial.print("No");
    else if (took_pill[i] == 1)
      Serial.print("Yes");
    else
      Serial.print('\t');
  }
      
   Serial.print('\n');
      
  //missed pill data
  Serial.print("Missed Pill:");
  Serial.print('\t');
  for (i = 0; i <7; i++)
  {
    Serial.print('\t');
   
     //tab formatting
    if (i == 2 || i == 3 || i == 4 || i ==6)
      Serial.print('\t');
   
    if (missed_pill[i] == 0)
      Serial.print("No");
    else if (missed_pill[i] == 1)
      Serial.print("Yes");
    else
      Serial.print('\t');
  }
   
   Serial.print('\n');
      
   //missed pill notice data
   Serial.print("Noticed Missed Pill:");
   for (i = 0; i <7; i++)
   {
    Serial.print('\t');
    
    //tab formatting
    if (i == 2 || i == 3 || i == 4 || i ==6)
      Serial.print('\t');
    
    if (missed_pill_notice[i] == 0)
      Serial.print("No");
    else if (missed_pill_notice[i] == 1)
      Serial.print("Yes");
    else
      Serial.print('\t');
   }
   
   for (i = 0; i < 5; i++)  
     Serial.print('\n');
  

  //Transmit data to recorder
  Wire.beginTransmission(wire_address);
  Wire.write(day);
  for (i = 0; i < 7; i++)
  {
    Wire.write(took_pill[i]);
    Wire.write(missed_pill[i]);
    Wire.write(missed_pill_notice[i]);
  }
  Wire.endTransmission();
    
      
     //incriment the day
  if (day == 6)
    day = 0;
  else
    day++;  
    
    //loop up to next day
}



//////////////////////////////////////////////////////////////////////////


void missed_reset_func()
/*****************
Resets the missed pill LED when button is pressed
****************/
{
  
  Serial.println("Missed Pill Noticed!");
  Serial.print('\n');
  
  digitalWrite(missed_pill_LED, LOW);
  digitalWrite(day_pin[day], LOW);
  missed_pill_notice[day] = 1;

}


//////////////////////////////////////////////////////////////////////////


void take_pill_func() 
/*****************
Turns off the LED for the current day
****************/
{
  
  Serial.println("take_pill");
  Serial.print('\n');
  
  digitalWrite(day_pin[day], LOW); 
  took_pill[day] = 1;

}


//////////////////////////////////////////////////////////////////////////


void set_day_func() 
/*****************
Sets the day. This resets the day countdown, effectivley
creating a one day delay from the time this button is
pressed. System will indicate to take a pill every 24 hours
from time this button is pressed. Pressing the set day button
allows the user to cycle through the day LEDs to set what
day of the week it is.
****************/
{
  
 Serial.println("Set Day:"); 
 Serial.print('\n');
 
 //record button states to detect falling edge
 int button_state;
 int last_button_state = digitalRead(set_day); //record initial state 
  
 //run until end_set_day is pressed
 while (digitalRead(end_set_day) == HIGH)
  {
    //record current state
    button_state = digitalRead(set_day);
    
    //if state has changed to low
    if (button_state != last_button_state && button_state == LOW)
    {
        
        if (day == 6)
           day = 0;
        else
           day++;
         
        Serial.print("Day is now: ");
        print_day();
        Serial.print('\n');  
     }
     
     //save previous state and delay to prevent bouncing
     last_button_state = button_state;
      
    
    //turn off all pins
    for (i =0; i < 7; i++)
      digitalWrite(day_pin[i], LOW);
    
    //set new day
    digitalWrite(day_pin[day], HIGH);
  } 


  //reset all stored information
  // 0 = false. 1 = true. 2 = future day / unknown
  for (i = 0; i < 7; i++)
  {
    took_pill[i] = 2; 
    missed_pill[i] = 2;
    missed_pill_notice[i] = 2;
  } 

  Serial.println("Program begun");
  Serial.print("Program begun on: ");
  print_day();
  for (i = 0; i < 3; i++)
    Serial.print('\n');
}

void print_day() {
  switch (day)
  {
   case 0:
    Serial.print("Monday");
    break;
   case 1:
    Serial.print("Tuesday"); 
    break;
   case 2:
    Serial.print("Wednesday");
    break; 
   case 3:
    Serial.print("Thursday"); 
    break;
   case 4:
    Serial.print("Friday");
    break;
   case 5:
    Serial.print("Saturday"); 
    break;
   case 6:
    Serial.print("Sunday"); 
    break;
  }
}
