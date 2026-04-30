
#include <EEPROM.h>
#include <SoftwareSerial.h>

SoftwareSerial BT(11, 12); // RX, TX

// ================== PIN DEFINITIONS ==================
const int a1   = 6;
const int a2   = 5;
const int b1A  = 10;
const int b2A  = 9;
const int line_track_left  = 4;   
const int line_track_right = 13;
const int ir_left  = 8;
const int ir_right = 7;
const int trigpin  = A0;           
const int echopin  = A1;

// ================== SETTINGS ==================
int speed          = 150;
int turn_speed     = 150;
float leftOffset   = 1.0;
float rightOffset  = 1.0;
const int stopDistance = 15;

const float tilt_threshold  = 3.0; // minimum tilt to trigger a turn
const float drive_threshold = 3.0; // minimum forward/back tilt to drive


// ================== STATE ==================
int mode          = 0;
int last_position = 0;

// ================== MOVEMENT FUNCTIONS ==================
void moveforward() {
  analogWrite(a1,  int(speed * rightOffset));
  analogWrite(b2A, int(speed * leftOffset));
  digitalWrite(a2,  LOW);
  digitalWrite(b1A, LOW);
}

void turn_left() {
  analogWrite(a1,  turn_speed);
  analogWrite(a2,  LOW);
  analogWrite(b1A, LOW);
  analogWrite(b2A, LOW);
}

void turn_right() {
  analogWrite(b2A, turn_speed);
  analogWrite(a1,  LOW);
  analogWrite(a2,  LOW);
  analogWrite(b1A, LOW);
}

void stop_move() {
  analogWrite(a1,  0);
  analogWrite(a2,  0);
  analogWrite(b1A, 0);
  analogWrite(b2A, 0);
}

void moveBackward() {
  analogWrite(a2,  speed);
  digitalWrite(a1,  LOW);
  analogWrite(b1A, speed);
  digitalWrite(b2A, LOW);
}

// ================== SENSOR READING ==================
long readDist() {
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  long duration = pulseIn(echopin, HIGH, 5000);
  return (duration > 0) ? (duration * 0.034 / 2) : 0;
}

// ================== LINE TRACKING MODE ==================
void lineTracking() {
  int left  = digitalRead(line_track_left);
  int right = digitalRead(line_track_right);

  Serial.print("Left:"); Serial.println(left);
  Serial.print("Right:"); Serial.println(right);

  if (left == 1) turn_right();
  else           turn_left();
}

// ================== OBSTACLE AVOIDANCE MODE ==================
void obstacleAvoidance() {
  int left      = digitalRead(ir_left);
  int right     = digitalRead(ir_right);
  long distance = readDist();

  Serial.println(distance);

  if (distance > 0 && distance < stopDistance) {
    moveBackward();
    delay(200);
    if (right && !left) turn_right();
    else                turn_left();
    return;
  }

  if      (right && !left)  turn_right();
  else if (left  && !right) turn_left();
  else                      moveforward();
}

// ================== SETUP & LOOP ==================
void setup() {
  Serial.begin(9600);
  BT.begin(9600);

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

  EEPROM.write(0, 80);
  EEPROM.write(1, 93);
  leftOffset  = EEPROM.read(0) * 0.01;
  rightOffset = EEPROM.read(1) * 0.01;
}

// ================== GYRO CONTROL MODE ==================
void handleGyro() {
  // Look for the 'M' prefix in the Bluetooth stream
  if (BT.peek() == 'M') { 
    BT.read(); // Consume the 'M'
    BT.read(); // Consume the ':'
    
    // parseInt automatically looks for the next valid integer
    int xValue = BT.parseInt(); 
    int yValue = BT.parseInt(); 

    // Debugging: See the parsed numbers in your Serial Monitor
    Serial.print("X: "); Serial.print(xValue);
    Serial.print(" | Y: "); Serial.println(yValue);

    // Movement Logic based on your Terminal values
    if (yValue < -50) {
      moveforward();    // Negative Y in your app seems to be "Up"
    } else if (yValue > 50) {
      moveBackward();   // Positive Y is "Down"
    } else if (xValue < -50) {
      turn_left();      // Negative X is "Left"
    } else if (xValue > 50) {
      turn_right();     // Positive X is "Right"
    } else {
      stop_move();      // Phone is relatively level
    }
  } else {
    // If the data isn't an 'M' string, just clear it
    BT.read(); 
  }
}

void loop() {
  if (BT.available()) {
    char check = BT.peek();
    
    if (check >= '0' && check <= '3') {
      mode = BT.read() - '0';
      stop_move();
      
      // Send message back to the Phone App Terminal
      BT.print(">>> ACTIVE MODE: ");
      if (mode == 0) BT.println("STANDBY");
      if (mode == 1) BT.println("LINE TRACKING");
      if (mode == 2) BT.println("OBSTACLE AVOIDANCE");
      if (mode == 3) BT.println("GYRO DRIVE");
      
      Serial.print("New Mode: "); Serial.println(mode);
    } 
    
    if (mode == 3) {
      handleGyro();
    } else {
      while(BT.available() && BT.peek() != '0' && BT.peek() != '1' && BT.peek() != '2' && BT.peek() != '3') {
        BT.read();
      }
    }
  }
  switch (mode) {
    case 1:  lineTracking();      break;
    case 2:  obstacleAvoidance(); break;
    case 3:  break; 
    default: stop_move();         break;
  }
}