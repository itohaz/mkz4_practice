/* listen40: タッチした座標を表示 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "MKZ4_XXXXXXXXX";
const char *password = "";

ESP8266WebServer server(80);

// html
String form = "<html>"
"<head><meta name=\"viewport\" content=\"width=device-width\"></head>"
"<body>"
"<div style=\"position:fixed; text-align:center; width:100%; font-size:100%;\">"
"Welcome MKZ4"
"<div id=\"message1\"></div>"
"<div id=\"message2\"></div>"
"</div>"
"<script type=\"text/javascript\">"
"document.body.style.height = document.body.clientHeight + 'px';"
"document.body.style.width = document.body.clientWidth + 'px';"
"document.getElementsByTagName(\"html\")[0].style.height = document.body.clientHeight + 'px';"
"document.getElementsByTagName(\"html\")[0].style.width = document.body.clientWidth + 'px';"
"var startX = 0; var startY = 0;"
"document.body.ontouchstart = function(event) {"
  "startX = event.touches[0].clientX;"
  "startY = event.touches[0].clientY;"
  "document.getElementById(\"message1\").innerHTML = \"start x=\" + startX + \"y=\" + startY;"
"};"
"document.body.ontouchmove = function(event) {"
  "var x = parseInt(event.touches[0].clientX - startX);"
  "var y = parseInt(event.touches[0].clientY - startY);"
  "document.getElementById(\"message2\").innerHTML = \"move x=\" + x + \"y=\" + y;"
"};"
"document.body.ontouchend = function(event) {"
"};"
"</script>"
"</body>"
"</html>";

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

  pinMode(12, OUTPUT); // LEDのピンを出力に設定
  digitalWrite(12, HIGH); // HIGH:点灯

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
