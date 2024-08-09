#include <Wire.h>
#include <Servo.h>

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


#define PIN_LED PB3

#define PIN_SCL PB8
#define PIN_SDA PB9

#define PIN_SERVOLEFT PB10
#define PIN_SERVORIGHT PB11

Servo left_servo, right_servo;

bool led_state = false;
uint32_t rgb_led_left = 0, rgb_led_right = 0;
uint16_t battery_voltage = 0;
uint8_t button_state = 0;
uint16_t line_sensor_left = 0, line_sensor_center = 0, line_sensor_right = 0;
uint16_t obstacle_sensor_left = 0, obstacle_sensor_right = 0;

void begin() {
  // Initialize the slave functionality 

  // Initialize I2C
  Wire.setSCL(PIN_SCL);
  Wire.setSDA(PIN_SDA);

  pinMode(PIN_LED, OUTPUT);

  //Attach Servo
  left_servo.attach(PIN_SERVOLEFT);
  right_servo.attach(PIN_SERVORIGHT);

}

void stop() {
  // Stop the slave operation
}

void reset() {
  // Reset the slave to initial state
}

void set_led(bool state) {
  digitalWrite(PIN_LED, state); 
}

void set_rgb_led(uint32_t left, uint32_t right) {
  rgb_led_left = left;
  rgb_led_right = right;
}

void get_battery_voltage() {
  Wire.write(battery_voltage & 0xFF);
  Wire.write((battery_voltage >> 8) & 0xFF);
}

void get_button() {
  Wire.write(button_state);
}

void set_servo_positions(uint8_t left, uint8_t right) {
  left_servo.write(left);
  right_servo.write(right);
}

void get_line_sensors() {
  Wire.write(line_sensor_left & 0xFF);
  Wire.write((line_sensor_left >> 8) & 0xFF);
  Wire.write(line_sensor_center & 0xFF);
  Wire.write((line_sensor_center >> 8) & 0xFF);
  Wire.write(line_sensor_right & 0xFF);
  Wire.write((line_sensor_right >> 8) & 0xFF);
}

void get_obstacle_sensors() {
  Wire.write(obstacle_sensor_left & 0xFF);
  Wire.write((obstacle_sensor_left >> 8) & 0xFF);
  Wire.write(obstacle_sensor_right & 0xFF);
  Wire.write((obstacle_sensor_right >> 8) & 0xFF);
}

void receiveEvent(int bytes) {
  uint8_t command = Wire.read();
  switch (command) {
    case CMD_BEGIN:
      begin();
      break;
    case CMD_STOP:
      stop();
      break;
    case CMD_RESET:
      reset();
      break;
    case CMD_SET_LED:
      set_led(Wire.read());
      break;
    case CMD_SET_RGB_LED:
      set_rgb_led(Wire.read() | (Wire.read() << 8) | (Wire.read() << 16) | (Wire.read() << 24),
                  Wire.read() | (Wire.read() << 8) | (Wire.read() << 16) | (Wire.read() << 24));
      break;
    case CMD_GET_BATTERY_VOLTAGE:
      get_battery_voltage();
      break;
    case CMD_GET_BUTTON:
      get_button();
      break;
    case CMD_SET_SERVO_POSITIONS:
      set_servo_positions(Wire.read(), Wire.read());
      break;
    case CMD_GET_LINE_SENSORS:
      get_line_sensors();
      break;
    case CMD_GET_OBSTACLE_SENSORS:
      get_obstacle_sensors();
      break;
  }
}

void setup() {

  begin();
  
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent);

  
}

void loop() {
  // No additional logic needed in the loop

  // set_servo_positions(0, 0);
  // delay(1000); // Wait for 1 second
  //   set_servo_positions(128, 128);
  // delay(1000); // Wait for 1 second

}