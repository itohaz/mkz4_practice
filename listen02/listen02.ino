void setup() {
  pinMode(12, OUTPUT); // ピンを出力に設定
}

void loop() {
  digitalWrite(12, HIGH); // 点灯
  delay(1000); // 1秒
  digitalWrite(12, LOW); // 消灯
  delay(1000);
}

