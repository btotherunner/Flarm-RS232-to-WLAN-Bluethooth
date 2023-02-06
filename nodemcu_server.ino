#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <SoftwareSerial.h>

const char *ssid = "flarm_ap";
const char *password = "password";

/*
Flarm-RS232       MAX3232       WEMOSD1Mini
    RX             <- rx            d7 (gpio13)
    TX             -> tx            d6 (gpio12)
    GND             (-)             GND  
    3V              (+)             5V
    
Baut einen SoftAP auf - und hört auf die IP 192.168.4.1
Port für Flarm Daten sind 4353
    
*/
SoftwareSerial softSerial(12, 13);
String inputString = "";
char incomingByte;

WiFiClient client;
WiFiServer server(4353);

void setup() {
  Serial.begin(115200);

  softSerial.begin(19200);

  WiFi.softAP(ssid, password);
  IPAddress ip = WiFi.softAPIP();

  Serial.print("AP IP address: ");
  Serial.println(ip);
  server.begin();
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
