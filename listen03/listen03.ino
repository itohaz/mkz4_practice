void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup start...");

  pinMode(12, OUTPUT);
  Serial.println("setup end.");
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(12, HIGH);
  Serial.println("LED ON");
  delay(1000);
  digitalWrite(12, LOW);
  Serial.println("LED OFF");
  delay(1000);
}
