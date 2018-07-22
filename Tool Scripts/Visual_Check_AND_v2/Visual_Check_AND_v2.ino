const int Output_1 = 13;
const int Output_2 = 12;

void setup() {
  pinMode(Output_1, OUTPUT);
  pinMode(Output_2, OUTPUT);
    
  Serial.begin(9600);
}

void loop() {
  digitalWrite(Output_1, LOW);
  digitalWrite(Output_2, LOW);
  delay(2000);
  
  digitalWrite(Output_1, HIGH);
  delay(2000);
  
  digitalWrite(Output_1, LOW);
  digitalWrite(Output_2, HIGH);
  delay(2000);
  
  digitalWrite(Output_1, HIGH);
  delay(2000);
  
}
