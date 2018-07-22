const int IO_OUT[8] = {13,12,11,10,9,8,7,6};

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 8; i++)
    pinMode(IO_OUT[i], OUTPUT);
    
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 0; i < 8; i++)
  {
    digitalWrite(IO_OUT[i], LOW);
    Serial.print(i);
    Serial.print(": ");
    Serial.print(digitalRead(IO_OUT[i]));
    Serial.println();
  }
  Serial.println();
  Serial.println();
  delay(2000);
  
  for (int i = 0; i < 8; i=i+2)
  {
    digitalWrite(IO_OUT[i], HIGH);
    Serial.print(i);
    Serial.print(": ");
    Serial.print(digitalRead(IO_OUT[i]));
    Serial.println();
  }
  Serial.println();
  Serial.println();
  delay(2000);
  
   for (int i = 1; i < 8; i=i+2)
   {
     digitalWrite(IO_OUT[i-1], LOW);
    digitalWrite(IO_OUT[i], HIGH);
    Serial.print(i);
    Serial.print(": ");
    Serial.print(digitalRead(IO_OUT[i]));
    Serial.println();
  }
  Serial.println();
  Serial.println();
  delay(2000);
  
   for (int i = 0; i < 8; i++)
   {
     digitalWrite(IO_OUT[i+1], LOW);
    digitalWrite(IO_OUT[i], HIGH);
    Serial.print(i);
    Serial.print(": ");
    Serial.print(digitalRead(IO_OUT[i]));
    Serial.println();
  }
  Serial.println();
  Serial.println();
  delay(2000);
  
}
