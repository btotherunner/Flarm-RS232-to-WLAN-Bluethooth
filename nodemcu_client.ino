/*
Flarm-RS232       MAX3232       WEMOSD1Mini
    RX             <- rx            d7 (gpio13)
    TX             -> tx            d6 (gpio12)
    GND             (-)             GND  
    3V              (+)             5V
    
Die IP wird nach dem Verbinden mit dem WLAN in der Software Console angezeigt
Port für Flarm Daten sind 4353
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <SoftwareSerial.h>

const char *ssid = "ssid des wlans";
const char *password = "passwort des wlans";

SoftwareSerial softSerial(12, 13);
String inputString = "";
char incomingByte;

WiFiClient client;
WiFiServer server(4353);

void setup() {
  Serial.begin(115200);

  softSerial.begin(19200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  server.begin();
  Serial.println("NodeMCU connected to WiFi");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  while (client.connected()) {

    if (softSerial.available() > 0) {
      incomingByte = softSerial.read();
      inputString += incomingByte;
      if (incomingByte == '\n') {
        // Der komplette String wurde empfangen
        // Verarbeiten Sie inputString hier
        Serial.print(inputString);
        client.print(inputString);
        inputString = "";
      }
    } 
  //  String currentTime = String(millis());
  //  client.println(currentTime);
  }

  client.stop();
}
