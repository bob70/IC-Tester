const int clock = 11;
const int reset = 12;
const int visual_delay = 200;
const int chip_delay = 10;

void setup() {
  pinMode(reset, OUTPUT);
  pinMode(clock, OUTPUT);
  digitalWrite(clock, LOW);
  digitalWrite(reset, HIGH);
}

void loop() {
  digitalWrite(reset, HIGH);
  delay(chip_delay);
  digitalWrite(reset, LOW);
  
  for (int i = 0; i < 256; i++)
  {
     digitalWrite(clock, HIGH);
     delay(chip_delay);
     digitalWrite(clock, LOW);
     delay(visual_delay);
  }
    
  
}
