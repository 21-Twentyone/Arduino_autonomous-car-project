// ================== DABBLE SETUP ==================
#define CUSTOM_SETTINGS
#define INCLUDE_TERMINAL_MODULE   // for mode switching via Terminal
#define INCLUDE_SENSOR_MODULE     // for accelerometer gyro control
#include <Dabble.h>

<<<<<<< HEAD
int turn_speed=100;
int back_speed=130;
int baseSpeed = 120;
int leftOffset = 10;

int startBoost = 150;     
int boostTime = 100;      
=======
//(rx,tx)(3,2)
// ================== PIN DEFINITIONS ==================
const int a1   = 6;
const int a2   = 5;
const int b1A  = 10;
const int b2A  = 9;

const int line_track_left  = 4;   // moved from 2 — Dabble uses pin 2 for BT
const int line_track_right = 13;
>>>>>>> bd22df1 (Final code of the project)

const int ir_left  = 8;
const int ir_right = 7;

const int trigpin  = A0;          // moved from 3 — Dabble uses pin 3 for BT
const int echopin  = A1;

// ================== SETTINGS ==================
int speed          = 120;
int turn_speed     = 90;
int boost          = 170;
int boost_duration = 20;

const int stopDistance = 15;

// gyro tilt thresholds (m/s^2)
const float tilt_threshold  = 3.0; // minimum tilt to trigger a turn
const float drive_threshold = 3.0; // minimum forward/back tilt to drive

// ================== STATE ==================
int mode          = 0;
int last_position = 0;

// ================== MOVEMENT FUNCTIONS ==================
void moveforward() {
  analogWrite(a1,  boost - 10);
  analogWrite(b2A, boost);
  delay(boost_duration);

  analogWrite(a1,  speed - 10);
  analogWrite(b2A, speed);
  digitalWrite(a2,  LOW);
  digitalWrite(b1A, LOW);
}

void moveBackward() {
  analogWrite(a2,  boost);
  analogWrite(b1A, boost);
  delay(boost_duration);

  analogWrite(a2,  speed);
  digitalWrite(a1,  LOW);
  analogWrite(b1A, speed);
  digitalWrite(b2A, LOW);
}

void turn_left() {
  analogWrite(a1, boost);
  delay(boost_duration);

  analogWrite(a1,  turn_speed);
  analogWrite(a2,  LOW);
  analogWrite(b1A, LOW);
  analogWrite(b2A, LOW);
}

void turn_right() {
  analogWrite(b2A, boost);
  delay(boost_duration);

  analogWrite(b2A, turn_speed);
  analogWrite(b1A, LOW);
  analogWrite(a1,  LOW);
  analogWrite(a2,  LOW);
}

void stop_move() {
  analogWrite(a1,  0);
  analogWrite(a2,  0);
  analogWrite(b1A, 0);
  analogWrite(b2A, 0);
}

// ================== SENSOR READING ==================
long readDist() {
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  long duration = pulseIn(echopin, HIGH, 20000);
  return (duration > 0) ? (duration * 0.034 / 2) : 400;
}

// ================== MODE LOGIC ==================
void lineTracking() {
  Dabble.processInput();
   int left = digitalRead(line_track_left);
  int right = digitalRead(line_track_right);

  /*Serial.print("L: ");
  Serial.print(left);
  Serial.print(" | R: ");
  Serial.println(right);
*/
  if (left == 1 && right == 1) {
    moveforward();
  }
  else if (left == 0 && right == 1) {
    turn_left();
    last_position = -1;
  }
  else if (left == 1 && right == 0) {
    turn_right();
    last_position = 1;
  }
  else if (left == 0 && right == 0) {
    if (last_position == -1) {
      turn_left();
    } else if (last_position == 1) {
      turn_right();
    } else {
      stop_move();
    }
  }
  delay(10);
}

void obstacleAvoidance() {
  Dabble.processInput();
  int left      = digitalRead(ir_left);
  int right     = digitalRead(ir_right);
  long distance = readDist();

  //Serial.println(distance);
if (distance > 0 && distance < stopDistance) {
Dabble.processInput();
moveBackward();
delay(200);
<<<<<<< HEAD

=======
Dabble.processInput();
>>>>>>> bd22df1 (Final code of the project)
if (right && !left){
  
  turn_right();
} 
else{
  turn_left();
}
return;
}

if (right && !left){
  turn_right();
} 
else if (left && !right){
  turn_left();
} 
else {
  moveforward();
}
}

void handleGyro() {
  Dabble.processInput();
  // read accelerometer axes from phone
  float x = Sensor.getAccelerometerXaxis(); // left/right tilt
  float y = Sensor.getAccelerometerYaxis(); // forward/back tilt

  //Serial.print("X: "); Serial.print(x);
  //Serial.print(" Y: "); Serial.println(y);

  if (y > drive_threshold) {
    moveforward();                    // tilt phone forward
  } else if (y < -drive_threshold) {
    moveBackward();                   // tilt phone backward
  } else if (x < -tilt_threshold) {
    turn_left();                      // tilt phone left
  } else if (x > tilt_threshold) {
    turn_right();                     // tilt phone right
  } else {
    stop_move();                      // phone flat → stop
  }
}

// ================== SETUP & LOOP ==================
void setup() {
  Dabble.begin(9600); // connect HC-05 TX→pin2, RX→pin3

  pinMode(a1,               OUTPUT);
  pinMode(a2,               OUTPUT);
  pinMode(b1A,              OUTPUT);
  pinMode(b2A,              OUTPUT);
  pinMode(line_track_left,  INPUT);
  pinMode(line_track_right, INPUT);
  pinMode(ir_left,          INPUT);
  pinMode(ir_right,         INPUT);
  pinMode(trigpin,          OUTPUT);
  pinMode(echopin,          INPUT);
}

void loop() {
  Dabble.processInput(); // must be called every loop for Dabble to work

  // mode switching via Dabble Terminal module
  if (Terminal.available()) {
    char cmd = Terminal.read();

    if (cmd == '1') {
      mode = 1; last_position = 0;
      stop_move();
      Terminal.println(F(">>> MODE: LINE TRACKING"));
    } else if (cmd == '2') {
      mode = 2; last_position = 0;
      stop_move();
      Terminal.println(F(">>> MODE: OBSTACLE AVOIDANCE"));
    } else if (cmd == '3') {
      mode = 3; last_position = 0;
      stop_move();
      Terminal.println(F(">>> MODE: GYRO CONTROL"));
    } else if (cmd == '0') {
      mode = 0; last_position = 0;
      stop_move();
      Terminal.println(F(">>> MODE: STANDBY"));
    }
  }

  switch (mode) {
    case 1: lineTracking();      break;
    case 2: obstacleAvoidance(); break;
    case 3: handleGyro();        break;
    default: stop_move();        break;
  }
}