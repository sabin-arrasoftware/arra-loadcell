
void setup() {
  Serial.begin(9600);
}

void loop() {
  for(int i = 0; i < 5; ++i){
    int rawValue = analogRead(A0 + i);
    const float voltage = rawValue * (5.0 / 1023.0);
    String si = String(i);
    Serial.print("A" + si + " "); 
    Serial.print(voltage);
    Serial.println(" Volts");
  }
  Serial.println(" \n\n\n");
  delay(5000);
}