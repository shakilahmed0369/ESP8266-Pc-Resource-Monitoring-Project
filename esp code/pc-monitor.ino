#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the I2C address if necessary

// Wi-Fi credentials
const char* ssid = "";       // Replace with your Wi-Fi SSID
const char* password = ""; // Replace with your Wi-Fi Password

// Open Hardware Monitor Web API URL
const char* serverName = "http://192.168.0.104:8085/data.json"; // Your open hardware monitor server address

// Timing variables
unsigned long lastFetchTime = 0;
const unsigned long fetchInterval = 2000; // Fetch data every 2 seconds

void setup() {
  Serial.begin(115200);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.print("WiFi");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  lcd.setCursor(4, 0);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    lcd.print(".");
  }

  // Display IP address
  lcd.clear();
  lcd.print("Connected!");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(2000);

  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

void loop() {
  if (millis() - lastFetchTime >= fetchInterval) {
    lastFetchTime = millis();
    fetchData();
  }
}

void fetchData() {
  if (WiFi.status() != WL_CONNECTED) {
    reconnectWiFi();
    return;
  }

  HTTPClient http;
  WiFiClient client;

  http.begin(client, serverName);
  int httpResponseCode = http.GET();

  if (httpResponseCode == HTTP_CODE_OK) {
    String payload = http.getString();
    parseAndDisplayData(payload);
  } else {
    Serial.printf("HTTP Error: %d\n", httpResponseCode);
  }

  http.end();
}

void reconnectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Reconnecting to WiFi...");
  }
}

void parseAndDisplayData(const String& jsonData) {
  StaticJsonDocument<1024> doc;

  DeserializationError error = deserializeJson(doc, jsonData);
  if (error) {
    Serial.printf("JSON Parsing Error: %s\n", error.f_str());
    return;
  }

  // Extract data
  JsonObject cpuTempObj = doc["Children"][0]["Children"][0]["Children"][1]["Children"][0];
  JsonObject cpuLoadObj = doc["Children"][0]["Children"][0]["Children"][2]["Children"][0];
  JsonObject gpuTempObj = doc["Children"][0]["Children"][1]["Children"][1]["Children"][0];
  JsonObject gpuLoadObj = doc["Children"][0]["Children"][1]["Children"][2]["Children"][0];

  if (cpuTempObj.isNull() || cpuLoadObj.isNull() || gpuTempObj.isNull() || gpuLoadObj.isNull()) {
    Serial.println("Invalid JSON path");
    return;
  }

  String cpuTemp = getSubstring(cpuTempObj["Value"], '.');
  String cpuLoad = getSubstring(cpuLoadObj["Value"], ' ');
  String gpuTemp = getSubstring(gpuTempObj["Value"], '.');
  String gpuLoad = getSubstring(gpuLoadObj["Value"], ' ');

  // Display data
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("CPU: %sC%s %s%%", cpuTemp.c_str(), "\xDF", cpuLoad.c_str());
  lcd.setCursor(0, 1);
  lcd.printf("GPU: %sC%s %s%%", gpuTemp.c_str(), "\xDF", gpuLoad.c_str());
}

String getSubstring(const char* input, char delimiter) {
  String strInput = String(input);
  int index = strInput.indexOf(delimiter);
  return (index != -1) ? strInput.substring(0, index) : strInput;
}
