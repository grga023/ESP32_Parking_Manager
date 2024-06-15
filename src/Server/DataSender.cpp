#include "DataSender.h" // Header file for this implementation
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "SetupAll.h"

extern bool EParkingSlots[4]; // Example EParkingSlots array

// // Function to send data to Vercel server
// void sendDataToVercel(const components &data) {
//   // Initialize the HTTP client
//   HTTPClient http;
  
//   // Vercel server endpoint URL
//   const char* serverUrl = "https://parking-wine.vercel.app/api/data"; // Replace with your Vercel server URL
  
//   // Construct JSON payload
//   StaticJsonDocument<256> doc; // Adjust the size as per your data size
//   doc["rfid"] = data.rfid;
//   doc["display"] = data.display;
//   JsonArray distances = doc.createNestedArray("distance");
//   for (int i = 0; i < 5; ++i) {
//     distances.add(data.distance[i]);
//   }
//   // Convert JSON document to string
//   String jsonStr;
//   serializeJson(doc, jsonStr);

//   // Send POST request to server
//   http.begin(serverUrl); // Specify the URL
//   http.addHeader("Content-Type", "application/json"); // Specify content-type header
  
//   int httpResponseCode = http.POST(jsonStr); // Send the actual POST request
//   if (httpResponseCode > 0) {
//     Serial.printf("[HTTP] POST request sent, response code: %d\n", httpResponseCode);
//     String payload = http.getString(); // Get the response payload
//     Serial.println(payload); // Print the response payload
//   } else {
//     Serial.printf("[HTTP] POST request failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
//   }
  
//   http.end(); // Free resources
// }

// Example function to send EParkingSlots data
void sendEParkingSlotsToVercel(const bool slots[4]) {
  HTTPClient http;
  
  const char* serverUrl = "https://parking-wine.vercel.app/api/recieveData"; // Replace with your Vercel server URL
  
  // Construct JSON payload
  StaticJsonDocument<128> doc; // Adjust the size as per your data size
  JsonArray slotsArray = doc.to<JsonArray>();
  for (int i = 0; i < 4; ++i) {
    slotsArray.add(slots[i]);
  }
  // Convert JSON document to string
  String jsonStr;
  serializeJson(doc, jsonStr);

  // Send POST request to server
  http.begin(serverUrl); // Specify the URL
  http.addHeader("Content-Type", "application/json"); // Specify content-type header
  
  int httpResponseCode = http.POST(jsonStr); // Send the actual POST request
  if (httpResponseCode > 0) {
    Serial.printf("[HTTP] POST request sent, response code: %d\n", httpResponseCode);
    String payload = http.getString(); // Get the response payload
    Serial.println(payload); // Print the response payload
  } else {
    Serial.printf("[HTTP] POST request failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
  }
  
  http.end(); // Free resources
}
