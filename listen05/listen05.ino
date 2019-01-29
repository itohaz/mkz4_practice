#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "MKZ4_XXXXXXXXX";
const char *password = "";

ESP8266WebServer server(80);

String form = "<h1>Welcome MKZ4</h1>";

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

  Serial.println("web server configuration");
  server.on("/", handleRoot);
  server.begin();

  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
 
  Serial.println("setup end.");
}

void handleRoot() {
  Serial.println("recieve req: /");
  server.send(200, "text/html", form);
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();  
}
