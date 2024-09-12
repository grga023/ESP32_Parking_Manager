#include <WiFi.h> 
#include "SetupAll.h"
#include "WiFi_conector.h"

void WIFIsetup()
{
    WiFi.begin(WIFI_SSID, WIFI_PASWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
    #ifdef DEBUG
      Serial.println("\nWiFi connected!");
      Serial.print("SSID: ");
      Serial.println(WiFi.SSID()); 
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      Serial.print("MAC Address: ");
      Serial.println(WiFi.macAddress());
      Serial.print("Signal strength (RSSI): ");
      Serial.println(WiFi.RSSI());
      Serial.print("Subnet Mask: ");
      Serial.println(WiFi.subnetMask());
      Serial.print("Gateway IP: ");
      Serial.println(WiFi.gatewayIP());
    #endif
}
