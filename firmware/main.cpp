#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <string.h>
#include <esp_wifi.h>

WiFiUDP Udp;

const char* ssid = "***";
const char* password = "***";

const int UDP_SERVER_PORT = 5005;
const int UDP_NODE_PORT = 6000;

const char* DISCOVERY_MESSAGE = "DISCOVER_REQUEST";
const char* NODE_NAME = "ExternalLightNode";

bool already_discovered = false;
bool luzEncendida = false;
unsigned long last_discovery_time = 0;

// Function prototypes
void handleUDP();

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  esp_wifi_set_promiscuous(true);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Connected to WiFi");
  Serial.print("📡 IP Address: ");
  Serial.println(WiFi.localIP());

  Udp.begin(UDP_NODE_PORT);
  Serial.printf("📨 Listening for UDP on %s:%i\n", WiFi.localIP().toString().c_str(), UDP_NODE_PORT);
}

void loop() {
  handleUDP();
// Reset every 90 seconds
  if (already_discovered && millis() - last_discovery_time > 90000) {
    already_discovered = false;
  }

}

void handleUDP() {
  int packetSize = Udp.parsePacket();
  if (packetSize > 0) {
    char packetBuffer[64];
    int len = Udp.read(packetBuffer, sizeof(packetBuffer) - 1);
    if (len > 0) {
      packetBuffer[len] = '\0';
    }

    Serial.printf("📥 Received packet from %s:%d: %s\n",
                  Udp.remoteIP().toString().c_str(), Udp.remotePort(), packetBuffer);

    if (strstr(packetBuffer, DISCOVERY_MESSAGE) && !already_discovered) {
      // Responder al descubrimiento
      IPAddress serverIP = Udp.remoteIP();
      char response[128];
      snprintf(response, sizeof(response), "NODE_RESPONSE|%s|%s", NODE_NAME, WiFi.localIP().toString().c_str());
      Udp.beginPacket(serverIP, UDP_SERVER_PORT);
      Udp.write((uint8_t*)response, strlen(response));
      Udp.endPacket();

      already_discovered = true;
      last_discovery_time = millis();
      Serial.printf("📤 Responded to server at %s: %s\n", serverIP.toString().c_str(), response);
    }

    else if (strcmp(packetBuffer, "ENCENDER") == 0) {
      luzEncendida = true;
      Serial.println("💡 Luz encendida");

      // Enviar ACK
      delay(500);
      const char* ack = "{\"ack\":\"ON\",\"uuid\":\"sin-uuid\",\"key\":\"C3sarToken2025\"}";
      Udp.beginPacket(Udp.remoteIP(), UDP_SERVER_PORT);
      Udp.write((uint8_t*)ack, strlen(ack));
      Udp.endPacket();
      Serial.printf("📤 ACK enviado: %s\n", ack);
    }

    else if (strcmp(packetBuffer, "APAGAR") == 0) {
      luzEncendida = false;
      Serial.println("💡 Luz apagada");

      // Enviar ACK
      delay(500);
      const char* ack = "{\"ack\":\"OFF\",\"uuid\":\"sin-uuid\",\"key\":\"C3sarToken2025\"}";
      Udp.beginPacket(Udp.remoteIP(), UDP_SERVER_PORT);
      Udp.write((uint8_t*)ack, strlen(ack));
      Udp.endPacket();
      Serial.printf("📤 ACK enviado: %s\n", ack);
    }
  }
}
