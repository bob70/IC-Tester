const int op_amp_in_1 = 5;
const int op_amp_in_2 = 4;

const int op_amp_out_1 = A4;
const int op_amp_out_2 = A5;

const int op_amp_hi_LED = 10;
const int op_amp_lo_LED = 6;

const int op_amp_delay = 3000; //time to allow op amp to propagate

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  digitalWrite(op_amp_hi_LED, LOW);
  digitalWrite(op_amp_lo_LED, LOW);
  
  pinMode(op_amp_hi_LED, OUTPUT);
  pinMode(op_amp_lo_LED, OUTPUT);
  
  digitalWrite(op_amp_hi_LED, LOW);
  digitalWrite(op_amp_lo_LED, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

//test first high, second low
  digitalWrite(op_amp_in_1, HIGH);
  digitalWrite(op_amp_in_2, LOW);
  
  digitalWrite(op_amp_hi_LED, HIGH);
  digitalWrite(op_amp_lo_LED, LOW);
  
  delay(op_amp_delay);
  
  
  
  Serial.print("1 = HI, 2 = LO");
  Serial.print('\n');
  Serial.print("1 = ");
  Serial.print(analogRead(op_amp_out_1));
  Serial.print('\n');
  Serial.print("2 = ");
  Serial.print(analogRead(op_amp_out_2));
  Serial.print('\n');
  

    
    
    
    //test both high
  digitalWrite(op_amp_in_1, HIGH);
  digitalWrite(op_amp_in_2, HIGH);
  
  digitalWrite(op_amp_hi_LED, HIGH);
  digitalWrite(op_amp_lo_LED, LOW);
  
  delay(op_amp_delay);
  
 
  
  Serial.print("1 = HI, 2 = HI");
  Serial.print('\n');
  Serial.print("1 = ");
  Serial.print(analogRead(op_amp_out_1));
  Serial.print('\n');
  Serial.print("2 = ");
  Serial.print(analogRead(op_amp_out_2));
  Serial.print('\n');
 
    
    
    
     //test first low, second high
  digitalWrite(op_amp_in_1, LOW);
  digitalWrite(op_amp_in_2, HIGH);
  
  digitalWrite(op_amp_hi_LED, LOW);
  digitalWrite(op_amp_lo_LED, HIGH);
  
  delay(op_amp_delay);
  
  Serial.print("1 = LO, 2 = HI");
  Serial.print('\n');
  Serial.print("1 = ");
  Serial.print(analogRead(op_amp_out_1));
  Serial.print('\n');
  Serial.print("2 = ");
  Serial.print(analogRead(op_amp_out_2));
  Serial.print('\n');
    
    
    
  //test both low
  digitalWrite(op_amp_in_1, LOW);
  digitalWrite(op_amp_in_2, LOW);
  
  digitalWrite(op_amp_hi_LED, LOW);
  digitalWrite(op_amp_lo_LED, HIGH);
  
  delay(op_amp_delay);
  
  Serial.print("1 = LO, 2 = LO");
  Serial.print('\n');
  Serial.print("1 = ");
  Serial.print(analogRead(op_amp_out_1));
  Serial.print('\n');
  Serial.print("2 = ");
  Serial.print(analogRead(op_amp_out_2));
  Serial.print('\n');

}
