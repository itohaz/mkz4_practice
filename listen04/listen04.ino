#include <ESP8266WiFi.h>

const char *ssid = "MKZ4_XXXXXXXXX";
const char *password = "";

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup start...");

  Serial.println("wifi configuration");
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(myIP);

  pinMode(12, OUTPUT);
  Serial.println("setup end.");
}

void loop() {
  // put your main code here, to run repeatedly:
}
