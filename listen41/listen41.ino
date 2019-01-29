/* listen41: スワイプした方向を表示 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

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
  "var Move = \"\";"
  "if (x < -1) {"
    "if (y < -1) {"
      "Move = \"left + forward\";"
    "}"
    "else if (y > 1) {"
      "Move = \"left + back\";"
    "}"
    "else {"
      "Move = \"left\";"
    "}"
  "}"
  "else if (x > 1) {"
    "if (y < -1) {"
      "Move = \"right + forward\";"
    "}"
    "else if (y > 1) {"
      "Move = \"right + back\";"
    "}"
    "else {"
      "Move = \"right\";"
    "}"
  "}"
  "else {"
    "if (y < -1) {"
      "Move = \"forward\";"
    "}"
    "else if (y > 1) {"
      "Move = \"back\";"
    "}"
  "}"
  "document.getElementById(\"message3\").innerHTML = Move;"
"};"
"document.body.ontouchend = function(event) {"
  "document.getElementById(\"message3\").innerHTML = \"\";"
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
