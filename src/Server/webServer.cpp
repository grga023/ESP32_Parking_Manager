#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SetupAll.h"
#include "WebServer.h"
#include "Servo_main.h"
#include "Display_main.h"
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "RFID_main.h"
#include "esp_system.h"
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

extern TaskHandle_t xTaskDisplayState;
extern components componentInit;
extern bool EParkingSlots[4];
extern int empty;
extern int cpu1;
extern TaskHandle_t xTaskExitRamp;
extern TaskHandle_t xTaskEntryRamp;

String logTableRows = "";

String getTimeString() {
  timeClient.setTimeOffset(7200);
  timeClient.update();
  return timeClient.getFormattedTime();
}

void addLogEntry(String name) {
  String currentTime = getTimeString(); 
  logTableRows += "<tr><td>" + currentTime + "</td><td>" + name + "</td></tr>";
}

String getComponentStatusHTML() {
  String html = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>Component Status</title>";
  html += "<style>";
  html += ".green { background-color: green; width: 100px; height: 100px; display: inline-block; margin: 10px; }";
  html += ".red { background-color: red; width: 100px; height: 100px; display: inline-block; margin: 10px; }";
  html += "h2 { margin-bottom: 0; }";  
  html += "</style></head><body>";
  html += "<h1>CPU load is: "+String(cpu1)+"%</h1>";
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
  html += "<h2>Restart ESP32</h2>";
  html += "<div>";
  html += "<button onclick=\"RestartESP()\">Restart ESP32</button>";
  html += "</div>";
  html += "<div id='dateTime'></div>";
  html += "<script>";
  html += "function RestartESP() {";
  html += "  fetch('/RestartESP').then(response => {";
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

String getParkingSlotsHTML() {
  String html = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>Parking Slots Status</title>";
  html += "<style>";
  html += ".green { background-color: green; width: 100px; height: 100px; display: inline-block; margin: 10px; }";
  html += ".red { background-color: red; width: 100px; height: 100px; display: inline-block; margin: 10px; }";
  html += ".number { display: block; text-align: center; margin-bottom: 5px; font-size: 20px; }";
  html += ".slot-container { display: flex; justify-content: space-around; }";
  html += "table { width: 100%; border-collapse: collapse; margin-top: 20px; }";
  html += "th, td { border: 1px solid black; padding: 8px; text-align: left; }";
  html += "th { background-color: #f2f2f2; }";
  html += "</style></head><body>";
  html += "<h1>Parking Slots Status</h1>";
  html += "<h1>Empty parking slots: " + String(empty) + "</h1>";
  html += "<div class='slot-container'>";

  for (int i = 0; i < 4; i++) {
    html += "<div>";
    html += "<span class='number'>" + String(i + 1) + "</span>";
    html += "<div class='" + String(EParkingSlots[i] ? "red" : "green") + "'></div>";
    html += "</div>";
  }

  html += "</div>";
  html += "<button onclick=\"EnterRamp()\">Open entry ramp</button>";
  html += "<button onclick=\"ExitRamp()\">Open exit ramp</button>";
  html += "<table id='logTable'>";
  html += "<thead><tr><th>Time</th><th>Name</th></tr></thead>";
  html += "<tbody>" + logTableRows + "</tbody>";
  html += "</table>";
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

void handleDateTime(AsyncWebServerRequest *request) {
  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();
  request->send(200, "text/plain", formattedTime);
}

void setupWebServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", getComponentStatusHTML());
  });

  server.on("/slots", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", getParkingSlotsHTML());
  });

  server.on("/openEntryRamp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Function triggered");
    vTaskResume(xTaskEntryRamp);
  });  
  server.on("/openExitRamp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Function triggered");
    vTaskResume(xTaskExitRamp);
  });  
  server.on("/RestartESP", HTTP_GET, [](AsyncWebServerRequest *request){
    vTaskSuspend(xTaskDisplayState);
    WriteOnDisplay(2, 0, 0, "RESTARTING!", true);
    displayContent();
    request->send(200, "text/plain", "Function triggered");
    esp_restart();
  });

  server.on("/dateTime", HTTP_GET, handleDateTime);

  server.begin();
}
