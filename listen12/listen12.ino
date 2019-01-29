#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define SERVO_LEFT  1200
#define SERVO_MIDLE 1450
#define SERVO_RIGHT 1700

const char *ssid = "MKZ4_XXXXXXXXX";
const char *password = "";

ESP8266WebServer server(80); // 80番ポートでWebサーバを立てる

// html
String form = "<html>"
"<head>"
"<meta name=\"viewport\" content=\"width=device-width\">"
"</head>"
"<body>"
"<center>"
"<h1>Welcome to MKZ4</h1>"
"<form action=\"forward\"><input type=\"submit\" value=\"forward 1s\"></form>"
"<form action=\"left\"><input type=\"submit\" value=\"turn left\"></form>"
"<form action=\"midle\"><input type=\"submit\" value=\"midle\"></form>"
"<form action=\"right\"><input type=\"submit\" value=\"turn right\"></form>"
"<form action=\"stop\"><input type=\"submit\" value=\"stop\"></form>"
"<form action=\"back\"><input type=\"submit\" value=\"back 1s\"></form>"
"</center>"
"</body>"
"</html>";

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

  Serial.println("wifi configuration");
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(myIP);

  Serial.println("web server configuration");
  server.on("/", handleRoot);
  server.on("/forward", forward_1s);
  server.on("/back", back_1s);
  server.on("/left", left);
  server.on("/midle", midle);
  server.on("/right", right);
  server.on("/stop", stop_motor);
  server.begin();

  pinMode(16, OUTPUT); // PWM制御のためにピンを出力に設定

  pinMode(12, OUTPUT); // LEDのピンを出力に設定
  digitalWrite(12, HIGH); // HIGH:点灯

  Serial.println("setup end.");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}

void handleRoot() {
  Serial.println("recieve req: /");
  server.send(200, "text/html", form);
}

void forward_1s(){
  start_motor();
  delay(1000);
  stop_motor();
}

void back_1s(){
  reverse_motor();
  delay(1000);
  stop_motor();
}

void left(){
  turn(SERVO_LEFT);
}

void midle(){
  turn(SERVO_MIDLE);
}

void right(){
  turn(SERVO_RIGHT);
}

void turn(int angle){
  int j;
  for(j=0; j<15; j++){
    digitalWrite(16, HIGH);
    delayMicroseconds(angle);
    digitalWrite(16, LOW);
    delayMicroseconds(20000 - angle);
  }
  server.send(200, "text/html", form);
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
    server.send(200, "text/html", form);
}

void motor_func(char add, char duty){
  Wire.beginTransmission(add);
  Wire.write(0x00);
  Wire.write(duty);
  Wire.endTransmission(true);
}
