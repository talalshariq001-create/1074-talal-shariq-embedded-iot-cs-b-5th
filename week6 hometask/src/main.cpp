/*
  Project: LDR + DHT22 Sensor with OLED Display
  Name: Talal Shariq
  Reg No: 23-NTU-CS-1074
  Date: 21-Oct-2025
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// --- Pin Configuration ---
#define LDR_PIN 34        // LDR connected to analog pin
#define DHTPIN 14         // DHT22 data pin
#define DHTTYPE DHT22     // DHT sensor type

#define SDA_PIN 21        // I2C SDA pin for OLED
#define SCL_PIN 22        // I2C SCL pin for OLED

// --- OLED Setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- DHT Sensor Setup ---
DHT dht(DHTPIN, DHTTYPE);

// --- Setup Function ---
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 LDR + DHT22 + OLED Demo");

  // Initialize I2C on custom pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed!");
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Initializing Sensors...");
  display.display();

  // Initialize DHT Sensor
  dht.begin();
  delay(1500);
}

// --- Main Loop ---
void loop() {
  // --- Read LDR Sensor ---
  int adcValue = analogRead(LDR_PIN);
  float voltage = (adcValue / 4095.0) * 3.3;

  // --- Read DHT22 Sensor ---
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // --- Check for DHT read failure ---
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading from DHT22!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("DHT22 Error!");
    display.display();
    delay(2000);
    return;
  }

  // --- Serial Monitor Output ---
  Serial.println("===== Sensor Readings =====");
  Serial.printf("LDR ADC: %d | Voltage: %.2f V\n", adcValue, voltage);
  Serial.printf("Temperature: %.2f °C | Humidity: %.2f %%\n", temperature, humidity);
  Serial.println("===========================\n");

  // --- OLED Display Output ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("IoT Sensor Readings");

  display.setCursor(0, 14);
  display.print("LDR ADC: ");
  display.println(adcValue);

  display.setCursor(0, 26);
  display.print("Voltage: ");
  display.print(voltage, 2);
  display.println(" V");

  display.setCursor(0, 40);
  display.print("Temp: ");
  display.print(temperature, 1);
  display.println(" C");

  display.setCursor(0, 52);
  display.print("Humidity: ");
  display.print(humidity, 1);
  display.println(" %");

  display.display();

  delay(2000); // Update every 2 seconds
}