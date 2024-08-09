#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define SLAVE_ADDRESS 0x20

#define CMD_BEGIN     0
#define CMD_STOP      1 
#define CMD_RESET     2
#define CMD_SET_LED   3
#define CMD_SET_RGB_LED 4
#define CMD_GET_BATTERY_VOLTAGE 5
#define CMD_GET_BUTTON 6
#define CMD_SET_SERVO_POSITIONS 7
#define CMD_GET_LINE_SENSORS 8
#define CMD_GET_OBSTACLE_SENSORS 9

#define NUM_LEDS 2
#define PIN_RGB 4
#define PIN_BUTTON 35



Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN_RGB, NEO_GRB + NEO_KHZ800);

void setLedColor(int ledIndex, int redValue, int greenValue, int blueValue) {
  pixels.setPixelColor(ledIndex, pixels.Color(redValue, greenValue, blueValue));
  pixels.show();
}

bool ledState = false;

void setup() {
  Wire.begin(21, 22);  
  Serial.begin(9600);
  pixels.begin();
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), buttonISR, FALLING);
}

void buttonISR() {
  ledState = !ledState;
}

void loop() {
  ledState = !ledState;
  
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(CMD_SET_LED);
  Wire.write(ledState);
  Wire.endTransmission();

  Serial.print("Sent LED state: ");
  Serial.println(ledState);

  delay(1000); // Wait for 1 second

  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(CMD_SET_SERVO_POSITIONS);
  Wire.write(0);
  Wire.write(0);
  Wire.endTransmission();

  delay(1000);

  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(CMD_SET_SERVO_POSITIONS);
  Wire.write(128);
  Wire.write(128);
  Wire.endTransmission();
  delay(1000);

}