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
  pinMode(LED_BUILTIN, OUTPUT);
  softSerial.begin(19200);

  WiFi.softAP(ssid, password);
  IPAddress ip = WiFi.softAPIP();

  Serial.println("AP IP address: ");
  Serial.println(ip);
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
    Serial.print("No Client connected...");
    delay(200);
  }

  while (client.connected()) {
    
    if (client.available()) {
      // Read XCSoar Data and send it to SoftwareSerial (TX)
      String nmea = client.readStringUntil('\n');
      Serial.println(nmea);
      digitalWrite(LED_BUILTIN, HIGH);
      softSerial.println(nmea);
      digitalWrite(LED_BUILTIN, LOW);
      }

    // Read Data from Software Serial and send Data to WifiClient (RX)
    if (softSerial.available() > 0) {
      incomingByte = softSerial.read();
      inputString += incomingByte;
      digitalWrite(LED_BUILTIN, LOW);
      if (incomingByte == '\n') {
        digitalWrite(LED_BUILTIN, HIGH);
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
