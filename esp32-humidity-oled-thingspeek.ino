#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
//#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define DHTPIN 4           // Pin where the DHT11 data pin is connected
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);
const char* ssid = "OPPO A3 Pro 5G";
const char* password = "pravallika123";
const char* apiKey = "SRAYJO7S5BG96X4U";
const char* server = "http://api.thingspeak.com/update";
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.begin();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay(); // Clear the OLED screen
// Initialize the DHT sensor
  display.setTextSize(1);      // Set text size to normal
  display.setTextColor(SSD1306_WHITE); // White text
  display.setCursor(0, 0);     // Start at top-left corner
  display.println(F("DHT11 Data:"));
  display.display();           // Show the text on the OLED
}
void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
 if (!isnan(temperature) && !isnan(humidity)) {
    // Send data to ThingSpeak
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = String(server) + "?api_key=" + apiKey + "&field1=" + temperature + "&field2=" + humidity;
      http.begin(url);
      int httpResponseCode = http.GET();
      http.end();
    }
// Display temperature and humidity on OLED
    display.clearDisplay();  // Clear previous display data
    display.setCursor(0, 0); // Start at top-left corner
    display.print(F("Temp: "));
    display.print(temperature);
    display.println(F(" C"));
    display.print(F("Humidity: "));
    display.print(humidity);
    display.println(F(" %"));
    display.display();  // Update the OLED display
  } else {
    // Display error message if reading fails
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("Error reading data"));
    display.display();
  }
delay(1500);  // Wait for 1.5 seconds before the next loop
}
