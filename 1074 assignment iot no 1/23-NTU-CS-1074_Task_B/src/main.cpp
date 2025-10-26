#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define LED_PIN 19
#define BTN_PIN 35
#define BUZZER_PIN 17

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

unsigned long buttonPressTime = 0;
bool buttonPressed = false;
bool ledState = LOW;

const unsigned long longPressTime = 1500; // 1.5 seconds

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);

  digitalWrite(LED_PIN, ledState);
  digitalWrite(BUZZER_PIN, LOW);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Ready...");
  display.display();
}

void loop() {
  bool btnState = digitalRead(BTN_PIN) == LOW; 

  if (btnState && !buttonPressed) {
    // Button just pressed
    buttonPressed = true;
    buttonPressTime = millis();
  }
  
  if (!btnState && buttonPressed) {
    // Button just released
    unsigned long pressDuration = millis() - buttonPressTime;

    if (pressDuration >= longPressTime) {
      // Long press action
      digitalWrite(LED_PIN, LOW);

      tone(BUZZER_PIN, 1000, 200); // play 1 kHz tone for 200 ms
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Long press detected!");
      display.display();
    } else {
      // Short press action
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);

      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Short press detected!");
      display.display();
    }

    buttonPressed = false; // reset for next press
  }
}