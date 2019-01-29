/* listen43: スワイプした方向に走る */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h> //I2C library

#define SERVO_LEFT  1200
#define SERVO_MIDLE 1450
#define SERVO_RIGHT 1700

const char *ssid = "MKZ4_XXXXXXXXXX";
const char *password = "";

ESP8266WebServer server(80);

// html
String form = "<html>"
"<head><meta name=\"viewport\" content=\"width=device-width\"></head>"
"<body>"
"<div style=\"position:fixed; text-align:center; width:100%; font-size:200%;\">"
"Welcome MKZ4"
"<div id=\"message1\"></div>"
"<div id=\"message2\"></div>"
"<div id=\"message3\"></div>"
"</div>"
"<form action=\"\" target=\"tif\" id=\"form\">"
"<iframe src=\"javascript: false;\" style=\"display: none;\" name=\"tif\" id=\"tif\"></iframe>"
"</form>"
"<script type=\"text/javascript\">"
"document.body.style.height = document.body.clientHeight + 'px';"
"document.body.style.width = document.body.clientWidth + 'px';"
"document.getElementsByTagName(\"html\")[0].style.height = document.body.clientHeight + 'px';"
"document.getElementsByTagName(\"html\")[0].style.width = document.body.clientWidth + 'px';"
"var startX = 0; var startY = 0;"
"var threshold = 40;"
"var esp_port = \'http://192.168.4.1:80\';"
"var el_form = document.getElementById(\'form\');"
"document.body.ontouchstart = function(event) {"
  "startX = event.touches[0].clientX;"
  "startY = event.touches[0].clientY;"
  "document.getElementById(\"message1\").innerHTML = \"start x=\" + startX + \" y=\" + startY;"
"};"
"document.body.ontouchmove = function(event) {"
  "var x = parseInt(event.touches[0].clientX - startX);"
  "var y = parseInt(event.touches[0].clientY - startY);"
  "document.getElementById(\"message2\").innerHTML = \"move x=\" + x + \" y=\" + y;"
  "var Move = \"\";"
  "var command = \'/stop\';"
  "if (x < (threshold * -1)) {"
    "if (y < (threshold * -1)) {"
      "Move = \"left + forward\";"
      "url = \'/left<br>forward\';"
    "}"
    "else if (y > threshold) {"
      "Move = \"left + back\";"
      "url = \'/left<br>back\';"
    "}"
    "else {"
      "Move = \"left\";"
      "url = \'/left\';"
    "}"
  "}"
  "else if (x > threshold) {"
    "if (y < (threshold * -1)) {"
      "Move = \"right + forward\";"
      "url = \'/right<br>forward\';"
    "}"
    "else if (y > threshold) {"
      "Move = \"right + back\";"
      "url = \'/right<br>back\';"
    "}"
    "else {"
      "Move = \"right\";"
      "url = \'/right\';"
    "}"
  "}"
  "else {"
    "if (y < (threshold * -1)) {"
      "Move = \"forward\";"
      "url = \'/forward\';"
    "}"
    "else if (y > threshold) {"
      "Move = \"back\";"
      "url = \'/back\';"
    "}"
  "}"
  "document.getElementById(\"message3\").innerHTML = Move;"
  "if (command != url) {"
    "if (url) {"
      "el_form.action = esp_port + url;"
      "el_form.submit();"
    "}"
  "}"
  "command = url;"
"};"
"document.body.ontouchend = function(event) {"
  "document.getElementById(\"message3\").innerHTML = \"\";"
    "el_form.action = esp_port + \'/stop\';"
    "el_form.submit();"
"};"
"</script>"
"</body>"
"</html>";

#define ADDR1 0x64 // A1, A0 open

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup start...");

  Serial.println("I2C configuration");
  Wire.begin(4, 14); // SDA=IO4, SCL=IO14
  delay(40);

  Serial.println("wifi configuration");
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(myIP);

  Serial.println("web server configuration");
  server.on("/", handleRoot);
  server.on("/stop", handle_stop);
  server.on("/forward", handle_forward);
  server.on("/back", handle_back);
  server.on("/left", handle_left);
  server.on("/midle", handle_midle);
  server.on("/right", handle_right);
  server.begin();

  pinMode(16, OUTPUT); // PWM

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

// モーター制御

void handle_stop() {
  stop_motor();
  server.send(200, "text/html", form);
}

void handle_forward() {
  start_motor();
  server.send(200, "text/html", form);
}

void handle_back() {
  reverse_motor();
  server.send(200, "text/html", form);
}

// DRV8830 レジスタ 0
// VSET[7..2]: 0x06..0x3F
// IN1[1], IN0[0]

void start_motor() {
  char i, volt;
  volt = 0x0C;
  for(i = 0; i < 4; i++) {
    volt = volt + ((0x10) << i);
    volt = volt | 0x02;
    motor_func(ADDR1, volt);
    Serial.printf("volt:0x%x vset:0x%x\n", volt, volt>>2);
    delay(10);
  }
}

void reverse_motor() {
  char i, volt;
  volt = 0x0C;
  for(i = 0; i < 4; i++) {
    volt = volt + ((0x10) << i);
    volt = volt | 0x01;
    motor_func(ADDR1, volt);
    delay(10);
  }
}

void stop_motor() {
  motor_func(ADDR1, 0x18);
  delay(10);
  motor_func(ADDR1, 0x1B);
  delay(10);
}

void motor_func(char add, char duty) {
  Wire.beginTransmission(add);
  Wire.write(0x00);
  Wire.write(duty);
  Wire.endTransmission(true);
}

// サーボモーター制御

void handle_left() {
  servo_control(SERVO_LEFT);
  server.send(200, "text/html", form);
}

void handle_midle() {
  servo_control(SERVO_MIDLE);
  server.send(200, "text/html", form);
}

void handle_right() {
  servo_control(SERVO_RIGHT);
  server.send(200, "text/html", form);
}

void servo_control(int angle) {
  int i;
  for(i = 0; i < 15; i++) {
    digitalWrite(16, HIGH);
    delayMicroseconds(angle);
    digitalWrite(16, LOW);
    delayMicroseconds(20000 - angle);
  }
}
