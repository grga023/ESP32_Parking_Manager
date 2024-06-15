#include <WiFi.h> 
#include "SetupAll.h"
#include "WiFi_conector.h"

void WIFIsetup()
{
  WiFi.begin(WIFI_SSID, WIFI_PASWORD);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
