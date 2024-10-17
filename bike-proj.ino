#include <MPU6050_tockn.h>
#include <Servo.h>

MPU6050 mpu(Wire);

// Configuration
const int potPin = A0;
const int escPin = 9;
const int servoPin = 10;

const int escRpm = 2000;

Servo esc;
Servo servo;

// State variables
int pot;
float voltage;
int bikeAngle;
int diskAngle;

// Control Variables
int diskAngleRef;

// State Machine variables
typedef enum {
  IDLE,
  RUNNING,
} State;

State state = RUNNING;

void setup() {
  Serial.begin(115200);
  
  servo.attach(servoPin);
  esc.attach(escPin, 0, escRpm);
  esc.writeMicroseconds(5000);

  Wire.begin();
  mpu.begin();
  mpu.calcGyroOffsets(true);

  Serial.println("\nProgram started...");
}

void loop() {
  char cmd = 0;
  if (Serial.available() > 0)
    cmd = Serial.read();

  switch (state) {
    case IDLE:
      if (cmd == 'r') {
        state = RUNNING;
        esc.writeMicroseconds(escRpm);
        Serial.println("Changing to state RUNNING in 5 seconds");
        delay(5000);
      }

      break;
    case RUNNING:
      if (cmd == 'i') {
        state = IDLE;
        esc.writeMicroseconds(0);
        Serial.println("Changing to state IDLE");
      }
      input();

      control();

      output();
      break;
  }
}

void input() {
  // Disk Angle
  pot = analogRead(potPin);
  voltage = pot * (5.0 / 1023.0) - 2.29;
  diskAngle = voltage * 1;

  // Bike Angle
  mpu.update();
  bikeAngle = mpu.getAngleX();
}

void control() {
  // Write control logic
  diskAngleRef = 90;
}

void output() {
  servo.write(diskAngleRef);

  Serial.print(bikeAngle);
  Serial.print(" ");
  Serial.println(diskAngle);
}
