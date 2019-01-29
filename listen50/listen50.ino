/* listen50: Milkcocoaを通してLEDを操作する */

#include <Milkcocoa.h>

/* servo control */
#include <ESP8266WiFi.h>
#include <Wire.h> //I2C library

/* Wi-Fi AP settings */
#define WLAN_SSID "XXXXXXXX" // 接続先SSID
#define WLAN_PASS "XXXXXXXX" // そのpass

/* Milkcocoa setup */
#define MILKCOCOA_APP_ID "XXXXXXXX" // my token
#define MILKCOCOA_DATASTORE "XXXXXXXX"
#define MILKCOCOA_SERVERPORT 1883

/* Global State */
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;

Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

void onsend(DataElement *elem) {
  int command; // 0: LED OFF 1: LED ON

  Serial.println("onsend");
  command = elem -> getInt("com");
  if(command == 0) {
    digitalWrite(12, LOW);
  } else {
    digitalWrite(12, HIGH);
  }
}

void setupWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup start...");

  setupWiFi();

  pinMode(12, OUTPUT); // LEDのピンを出力に設定

  Serial.println(milkcocoa.on(MILKCOCOA_DATASTORE, "send", onsend));

  Serial.println("setup end.");
}

void loop() {
  // put your main code here, to run repeatedly:
  milkcocoa.loop();
}

