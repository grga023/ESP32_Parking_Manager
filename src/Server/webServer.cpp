
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SetupAll.h"
#include "WebServer.h"
#include "Servo_main.h"
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

extern components componentInit;
extern bool EParkingSlots[4];
extern TaskHandle_t xexit;
extern TaskHandle_t xenter;

String getComponentStatusHTML() {
  String html = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>Component Status</title>";
  html += "<style>";
  html += ".green { background-color: green; width: 100px; height: 100px; display: inline-block; margin: 10px; }";
  html += ".red { background-color: red; width: 100px; height: 100px; display: inline-block; margin: 10px; }";
  html += "h2 { margin-bottom: 0; }";  // Remove bottom margin from h2 to place divs right below
  html += "</style></head><body>";
  html += "<h1>Component Status</h1>";

  // RFID
  html += "<h2>RFID:</h2><div class='" + String(componentInit.rfid ? "green" : "red") + "'></div>";

  // Display
  html += "<h2>Display:</h2><div class='" + String(componentInit.display ? "green" : "red") + "'></div>";

  // Distance Sensors
  html += "<h2>Distance Sensors:</h2>";
  for (int i = 0; i < 5; i++) {
    html += "<div class='" + String(componentInit.distance[i] ? "green" : "red") + "'></div>";
  }

  html += "</body></html>";
  return html;
}

String getParkingSlotsHTML() {
  String html = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>Parking Slots Status</title>";
  html += "<style>";
  html += ".green { background-color: green; width: 100px; height: 100px; display: inline-block; margin: 10px; }";
  html += ".red { background-color: red; width: 100px; height: 100px; display: inline-block; margin: 10px; }";
  html += ".number { display: block; text-align: center; margin-bottom: 5px; font-size: 20px; }";
  html += ".slot-container { display: flex; justify-content: space-around; }";
  html += "</style></head><body>";
  html += "<h1>Parking Slots Status</h1>";
  html += "<div class='slot-container'>";

  for (int i = 0; i < 4; i++) {
    html += "<div>";
    html += "<span class='number'>" + String(i + 1) + "</span>";
    html += "<div class='" + String(EParkingSlots[i] ? "green" : "red") + "'></div>";
    html += "</div>";
  }

  html += "</div>";
  html += "<button onclick=\"EnterRamp()\">Open entery ramp</button>";
  html += "<button onclick=\"ExitRamp()\">Open exit ramp</button>";
  html += "<script>";
  html += "function EnterRamp() {";
  html += "  fetch('/openEntryRamp').then(response => {";
  html += "    if (response.ok) {";
  html += "      alert('Function triggered successfully!');";
  html += "    } else {";
  html += "      alert('Function trigger failed.');";
  html += "    }";
  html += "  });";
  html += "}";
  html += "function ExitRamp() {";
  html += "  fetch('/openExitRamp').then(response => {";
  html += "    if (response.ok) {";
  html += "      alert('Function triggered successfully!');";
  html += "    } else {";
  html += "      alert('Function trigger failed.');";
  html += "    }";
  html += "  });";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  return html;
}

void setupWebServer() {
  // Serve HTML page on root
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", getComponentStatusHTML());
  });

  server.on("/slots", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", getParkingSlotsHTML());
  });
  server.on("/openEntryRamp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Function triggered");
    vTaskResume(xenter);
  });  
  server.on("/openExitRamp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Function triggered");
    vTaskResume(xexit);
  });
  // Start server
  server.begin();
}

