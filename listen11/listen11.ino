#include <Wire.h>

#define ADDR1 0x64

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup start...");

  Serial.println("I2C configuration");
  Wire.begin(4, 14);
  delay(40);

  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);

  Serial.println("setup end.");
}

void loop() {
  // put your main code here, to run repeatedly:
  start_motor();
  delay(3000);
  stop_motor();
  reverse_motor();
  delay(3000);
  stop_motor();
}

void start_motor(){
  char i, volt;
  volt = 0x0C;
  for(i=0; i<4; i++){
    volt = volt + ((0x10)<<i);
    volt = volt | 0x01;
    motor_func(ADDR1, volt);
    Serial.printf("volt: 0x%x vset: 0x%x\n", volt, volt>>2);
    delay(10);
  }
}

void reverse_motor(){
  char i, volt;
  volt = 0x0C;
  for(i=0; i<4; i++){
    volt = volt + ((0x10)<<i);
    volt = volt | 0x02;
    motor_func(ADDR1, volt);
    delay(10);
  }
}

void stop_motor(){
    motor_func(ADDR1, 0x18);
    delay(10);
    motor_func(ADDR1, 0x1B);
    delay(10);
}

void motor_func(char add, char duty){
  Wire.beginTransmission(add);
  Wire.write(0x00);
  Wire.write(duty);
  Wire.endTransmission(true);
}
