#include <Wire.h>
#include "RichShieldTM1637.h"
 
// Define LED pins
#define LED_RED 4
#define LED_GREEN 5
#define LED_YELLOW 7
#define LED_BLUE 6
 
// Define Button pin
#define BUTTON_K2 9
 
// Define Potentiometer pin
#define POT_PIN A0
 
// Define 7-segment display pins
#define CLK 10
#define DIO 11
TM1637 tm1637(CLK, DIO);
 
// Define number of LEDs
#define LED_COUNT 4
int ledPins[LED_COUNT] = {LED_RED, LED_GREEN, LED_YELLOW, LED_BLUE};
bool ledStates[LED_COUNT] = {false, false, false, false};
 
void setup() {
  Serial.begin(9600); // Initialize serial communication
 
  // Set pin modes for LEDs
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Initialize LEDs to off
  }
 
  // Set pin mode for the button
  pinMode(BUTTON_K2, INPUT_PULLUP); // Use INPUT_PULLUP for button
 
  // Initialize 7-segment display
  tm1637.init();
}
 
void loop() {
  // Read potentiometer value (0-1023)
  int potValue = analogRead(POT_PIN);
 
  // Map potentiometer value to a range for display (e.g., 0-99 for a percentage)
  int humidityLevel = map(potValue, 0, 1023, 0, 99);
 
  // Display the mapped humidity level on the 7-segment display
  displayHumidity(humidityLevel);
 
  // Update LEDs based on humidity level
  updateLEDs(humidityLevel);
 
  // Check the state of BUTTON_K2
  if (digitalRead(BUTTON_K2) == LOW) { // Button pressed (active low)
    // Turn on the Green LED
    digitalWrite(LED_GREEN, HIGH);
    ledStates[1] = true;
  } else {
    // Turn off the Green LED
    digitalWrite(LED_GREEN, LOW);
    ledStates[1] = false;
  }
 
  // Display potentiometer value for debugging
  Serial.print("Potentiometer Value: "); Serial.println(potValue);
  Serial.print("Simulated Humidity Level: "); Serial.println(humidityLevel);
 
  delay(100); // Short delay for responsiveness
}
 
// Function to update LED states based on humidity level
void updateLEDs(int humidityLevel) {
  // Turn off all LEDs except Green LED controlled by button
  for (int i = 0; i < LED_COUNT; i++) {
    if (i != 1) { // Skip Green LED as it's controlled by the button
      digitalWrite(ledPins[i], LOW);
      ledStates[i] = false;
    }
  }
 
  // Control LEDs based on humidity level
  if (humidityLevel < 25) {
    digitalWrite(LED_BLUE, HIGH); // Example: Blue LED for low humidity
    ledStates[3] = true;
  } else if (humidityLevel < 50) {
    digitalWrite(LED_GREEN, HIGH); // Example: Green LED for moderate humidity
    ledStates[1] = true;
  } else {
    digitalWrite(LED_RED, HIGH); // Example: Red LED for high humidity
    ledStates[0] = true;
  }
}
 
// Function to display humidity level on the 7-segment display
void displayHumidity(int humidity) {
  int8_t digits[4] = {0xFF, 0xFF, 0xFF, 0xFF}; // Default: blank display
 
  if (humidity >= 0 && humidity <= 99) {
    digits[0] = humidity / 10;  // Tens place
    digits[1] = humidity % 10;  // Ones place
  }
 
  tm1637.display(digits);
}