#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "DHT.h"

#define DHTPIN D4      // Connect DHT data pin to D4 (GPIO2)
#define DHTTYPE DHT11  // or DHT22

const char* ssid = "Your_SSID";         // replace with your WiFi name
const char* password = "Your_PASSWORD"; // replace with your WiFi password

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // Port to listen on
char incomingPacket[255];          // buffer for incoming packets
char replyPacket[255];             // buffer for reply

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Udp.begin(localUdpPort);
  Serial.printf("UDP server started at port %d\n", localUdpPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // Receive incoming UDP packet
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = '\0';  // null-terminate the string
    }

    Serial.printf("Received UDP packet: %s\n", incomingPacket);

    // Respond only if command is "humidity"
    if (strcmp(incomingPacket, "humidity") == 0) {
      float hum = dht.readHumidity();
      if (isnan(hum)) {
        sprintf(replyPacket, "Error reading humidity");
      } else {
        sprintf(replyPacket, "Humidity: %.2f %%", hum);
      }

      // Send back the humidity data to the client
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(replyPacket);
      Udp.endPacket();
      Serial.printf("Sent response: %s\n", replyPacket);
    }
  }

  delay(100);
}
