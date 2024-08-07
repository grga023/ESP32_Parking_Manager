#include <WiFi.h> 
#include "SetupAll.h"
#include "WiFi_conector.h"

void WIFIsetup()
{
  WiFi.begin(WIFI_SSID, WIFI_PASWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}
