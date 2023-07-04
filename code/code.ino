// define the pins for the ultrasonic sensors
#define rightTrigPin 3
#define rightEchoPin 2
#define middleTrigPin 10
#define middleEchoPin 9
#define leftTrigPin 12
#define leftEchoPin 11

// define the pins for the motor driver
#define leftMotorPin1 5
#define leftMotorPin2 4
#define rightMotorPin1 7
#define rightMotorPin2 8
#define motorSpeedPin 6

// define the distance thresholds for obstacle avoidance
int threshold = 10; // in centimeters
int motorSpeed = 90;
bool manualControl = true;

void setup() {
  Serial.begin(9600);

  // initialize the ultrasonic sensors
  pinMode(rightTrigPin, OUTPUT);
  pinMode(rightEchoPin, INPUT);
  pinMode(middleTrigPin, OUTPUT);
  pinMode(middleEchoPin, INPUT);
  pinMode(leftTrigPin, OUTPUT);
  pinMode(leftEchoPin, INPUT);

  // initialize the motor driver
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(motorSpeedPin, OUTPUT);
}

void loop() {
  // Check for incoming Bluetooth commands
  if (Serial.available()) {
    char command = Serial.read();

      switch (command) {
      case 'F':
        moveForward();
        break;
      case 'B':
        moveBackward();
        break;
      case 'L':
        turnLeft();
        break;
      case 'R':
        turnRight();
        break;
      case 'S':
        stop();
        break;
      case '+':
        increaseSpeed();
        break;
      case '-':
        decreaseSpeed();
        break;
      case 'U':
        increaseThreshold();
        break;
      case 'D':
        decreaseThreshold();
        break;
      case 'A':
        manualControl = false; // Switch to automatic mode
        Serial.print("Set Auto!\n");
        break;
      case 'M':
        manualControl = true; // Switch to manual mode
        Serial.print("Set Manual!\n");
        stop();
        break;
    }
  }

  // Read the distances from the ultrasonic sensors
  float rightDistance = getDistance(rightTrigPin, rightEchoPin);
  float middleDistance = getDistance(middleTrigPin, middleEchoPin);
  float leftDistance = getDistance(leftTrigPin, leftEchoPin);

  if (!manualControl) {
    // Automatic mode: Obstacle avoidance
    if (middleDistance > threshold && leftDistance > threshold && rightDistance > threshold) {
      // No obstacles, move forward
      moveForward();
    } else {
      // Obstacle detected, turn towards the side with more distance
      if (rightDistance > leftDistance && rightDistance > threshold) {
        // Turn right
        turnRight();
      } else if (leftDistance > rightDistance && leftDistance > threshold) {
        // Turn left
        turnLeft();
      } else {
        // Both sides have obstacles, move backward
        moveBackward();
        delay(1000);
      }
    }
  }
}

int getDistance(int trigPin, int echoPin) {
  // Send a pulse to the trig pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the echo pulse
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in centimeters
  float distance = duration * 0.034 / 2;

  return distance;
}

void moveForward() {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(motorSpeedPin, motorSpeed);
}

void moveBackward() {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
  analogWrite(motorSpeedPin, motorSpeed);
}

void turnLeft() {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(motorSpeedPin, motorSpeed);
}

void turnRight() {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
  analogWrite(motorSpeedPin, motorSpeed);
}

void stop() {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, LOW);
}

void increaseSpeed() {
  if (motorSpeed < 255) {
    motorSpeed += 10;
    Serial.print("Motor Speed: ");
    Serial.println(motorSpeed);
  }
}

void decreaseSpeed() {
  if (motorSpeed > 70) {
    motorSpeed -= 10;
    Serial.print("Motor Speed: ");
    Serial.println(motorSpeed);
  }
}

void increaseThreshold() {
  threshold += 2;
  Serial.print("Threshold Distance [cm]: ");
  Serial.println(threshold);
}

void decreaseThreshold() {
  if (threshold > 5) {
    threshold -= 2;
    Serial.print("Threshold Distance [cm]: ");
    Serial.println(threshold);
  }
}
