#include <TinyGPS++.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>

#define TX_PIN 2   // TX pin of NEO-6 GPS module
#define RX_PIN 15  // RX pin of NEO-6 GPS module
#define PIR_SENSOR_PIN 12  // Pin for the PIR sensor

const char* ssid = "wifi_ssid";
const char* password = "wifi_password";
const char* serverUrl = "http://server_under_construction";

TinyGPSPlus gps;
int passengerCount = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN); // Initialize the GPS module
  pinMode(PIR_SENSOR_PIN, INPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Check PIR sensor for passenger detection
  if (digitalRead(PIR_SENSOR_PIN) == HIGH) {
    // Passenger boarded
    passengerCount++;
    Serial.println("Passenger boarded. Count: " + String(passengerCount));
    delay(1000); // Debounce
  }

  while (Serial1.available() > 0) {
    if (gps.encode(Serial1.read())) {
      if (gps.location.isUpdated()) {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();

        // Send GPS data and passenger count to a server
        sendDataToServer(latitude, longitude, passengerCount);
      }
    }
  }
  delay(10000);  // Update GPS data every 10 seconds 
}

void sendDataToServer(float latitude, float longitude, int passengers) {
  HTTPClient http;

  String data = "lat=" + String(latitude, 6) + "&lng=" + String(longitude, 6) + "&passengers=" + String(passengers);
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST(data);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("HTTP Response Code: " + String(httpResponseCode));
    Serial.println(response);
  } else {
    Serial.println("HTTP Error");
  }

  http.end();
}

