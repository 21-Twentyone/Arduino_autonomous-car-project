const int a1 = 6;
const int a2 = 5;
const int b1A = 10;
const int b2A = 9;
const int linetrack=2;
const int ir_right=12;
const int ir_left=11;
const int trigpin=3;
const int echopin=4;
const int stopDistance=15;

int turn_speed=100;
int back_speed=130;
int baseSpeed = 120;
int leftOffset = 10;

int startBoost = 150;     
int boostTime = 100;      

long readDist(){
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);

  long duration = pulseIn(echopin, HIGH, 20000); // timeout added
  long distance = duration * 0.034 / 2;

  return distance;
}

void moveForward() {
  int leftSpeed = baseSpeed - leftOffset;

  analogWrite(a1, startBoost);
  digitalWrite(a2, LOW);
  analogWrite(b2A, startBoost);
  digitalWrite(b1A, LOW);
  delay(boostTime);

 
  analogWrite(a1, leftSpeed);
  digitalWrite(a2, LOW);
  analogWrite(b2A, baseSpeed);
  digitalWrite(b1A, LOW);
}

void moveBackward(){
  analogWrite(a2,back_speed);
  digitalWrite(a1,0);
  analogWrite(b1A,back_speed);
  digitalWrite(b2A,0);
}

void turn_left(){
  // Right wheel moves forward faster
  analogWrite(a1,turn_speed);
  analogWrite(a2,0);
  // Left wheel spins slower
  analogWrite(b1A,0);
  analogWrite(b2A,0);
}

void turn_right(){
  // Left wheel moves forward faster
  analogWrite(b1A,0);
  analogWrite(b2A,turn_speed); 
  // Right wheel spins slower
  analogWrite(a1,0);
  analogWrite(a2,0);
}

void stop_move(){
  analogWrite(a1,0);
  analogWrite(a2,0);
  analogWrite(b1A,0);
  analogWrite(b2A,0);
}

void setup() {
  Serial.begin(9600);
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT); 
  pinMode(b1A, OUTPUT);
  pinMode(b2A, OUTPUT);
  pinMode(linetrack,INPUT);
  pinMode(ir_right,INPUT);
  pinMode(ir_left,INPUT);
  pinMode(echopin,INPUT);
  pinMode(trigpin,OUTPUT);
}

void loop() {
int left = digitalRead(ir_left);
int right = digitalRead(ir_right);
long distance = readDist();
Serial.println(distance);

if (distance > 0 && distance < stopDistance) {

moveBackward();
delay(200);

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
  moveForward();
}
}
/*
if (lineColour==1){ //line detected, so value is equal to 1
  moveforward();

}else{ //no line detected.
  turn_left();
}
*/

