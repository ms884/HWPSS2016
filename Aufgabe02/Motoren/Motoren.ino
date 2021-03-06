//global
#include <LiquidCrystal.h>

int analogPinValue;
int motorA1 = 10;
int motorA2 = 11;
int motorB1 = 6;
int motorB2 = 9;
int us1 = 12;

LiquidCrystal lcd(2, 3, 4, 5, 7, 8);
// Pins 10, 11 as output and connect them
// with motorsignals A1 and A2
// Pin10 with A1
// Pin11 with A2
// check if spd got right int value.
// Pin6 with B1
// Pin9 with B2

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  //pinMode(us1, INPUT);
  lcd.begin(20,4);
}

void loop() {
  driveForward(2000, 100);
  delay(5000);
  int cm = meassureDistance(us1);
  lcd.print(cm);
}
//Values for selectmotor: 0 for motor A
// 1 for motor B

void setMotorSpeed (boolean forward, uint8_t spd, int selectMotor) {
  // Run the motor with the strength of the variable spd(values from
  // 0-255). 
  // If variable forward == True the robotor moves forward.
  // If variable forward == False the robotor moves backwards.
  // Variable selectMotor decides which motor should work.
  // If selectMotor == 0 motor A works,
  // if selectMotor == 1 motor B works.
  // if selectMotor == 2 both motor works.
  if (selectMotor == 0) {
    if (forward == true) {
      // TODO activate A2, A1 is backwards.
      analogWrite(motorA2, spd);
      digitalWrite(motorA1, LOW);
    } else {
      analogWrite(motorA1, spd);
      digitalWrite(motorA2,  LOW);
      }
  } 
  if (selectMotor == 1) {
    if (forward == true) {
      analogWrite(motorB2, spd);
      digitalWrite(motorB1, LOW);
    } else {
      analogWrite(motorB1, spd);
      digitalWrite(motorB2,  LOW);
    }
  }
  if (selectMotor == 2) {
    if (forward == true) {
      analogWrite(motorB2, spd);
      digitalWrite(motorB1, LOW);
      analogWrite(motorA2, spd);
      digitalWrite(motorA1, LOW);
    } else {
      analogWrite(motorB1, spd);
      digitalWrite(motorB2,  LOW);
      analogWrite(motorA1, spd);
      digitalWrite(motorA2,  LOW);
    }  
}
}

void driveForward(int runtime, uint8_t spd) {
  // let the roboter drive forward for variable runtime with the value
  // of variable speed.
  setMotorSpeed(true, spd, 2); // let both motor run.
  delay(runtime);
  setMotorSpeed(true, 0, 2); // stop both motors.
}

void driveCurve(int runtime, uint8_t spd, uint8_t strength){
  // strength has to be between -90 and 90
  // at 90 and -90 do a circle
  // at 0 drive straight forward
  float sp = - spd * (strength - 45)/45;
  int spB = (int)sp;
  sp = spd * (45 - strength)/45;
  int spF = (int)sp;
  if(strength > -1){
      analogWrite(motorA1, spd);
      digitalWrite(motorA2, LOW);
      
      if (strength == 0){
        driveForward(runtime, spd);
        }    
      // motor B backwards
      else if(strength == 45){
        digitalWrite(motorB1, LOW);
        digitalWrite(motorB2, LOW);
        }
      else if(strength == 45){
        analogWrite(motorB1, spB);
        digitalWrite(motorB2, LOW);
        }
      // motor B forward
      else{
        analogWrite(motorB2, spF);
        digitalWrite(motorB1, LOW);
      }
  }
  else{
     sp =  spd * (45 - strength)/45;
    int spB1 = (int)sp;
     sp = spd * (strength + 45)/45;
    int spF1 = (int)sp;

    analogWrite(motorB2, spd);
    digitalWrite(motorB1, LOW);
    // motor A backwards
    if(strength >= -45){
      if (strength == -45){
        digitalWrite(motorA2, LOW);
        digitalWrite(motorA1, LOW);
        }
      else{
        analogWrite(motorA2, spB1);
        digitalWrite(motorA1, LOW);
      }
    }
     // motor A forward
    else{
      analogWrite(motorA1, spF1);
      digitalWrite(motorA2, LOW);
    }
  }
    delay(runtime);
  }
  
// detect the distance of a barrier and return the distance in cm.
// -1 is returned if no barrier is detected.
// use PIN 12 as connector for US1
int meassureDistance (int Pin) {
  activateSensor();
  while (digitalRead(Pin) == LOW) {
    if (digitalRead(Pin) == HIGH) {
      break;
    }
  }
 uint32_t stTime = micros();
  while (digitalRead(Pin) == HIGH)
  {
    if (digitalRead(Pin) == LOW) {
      break;
    }
  }
  uint32_t enTime = micros();
  uint32_t impulseTime = enTime - stTime;
  if (impulseTime == 0) // no barrier detected
  {
    return -1;
  }
  else 
  {
    uint32_t distance = (impulseTime/58);
    return distance;
  }
 /* uint32_t tim;
  activateSensor();
  //int stat = digitalRead(12);
  //lcd.print(stat);
  while(digitalRead(12) == HIGH) {
    tim = micros();
 
  }
 // lcd.print(tim);
  delay(5000);
  int dist = tim/58;
  return (dist);
  /*uint32_t impulseTime = pulseIn(12, HIGH, 30000);
  if (impulseTime == 0) // no barrier detected
  {
    return -1;
  }
  else 
  {
    uint32_t distance = (impulseTime/58);
    return distance;
  }*/
}

void activateSensor() {
  // method to activate US1 according to the exercisesheet.
  pinMode(us1, OUTPUT);
  digitalWrite(us1, LOW);
  digitalWrite(us1, HIGH);
  delayMicroseconds(15);  // it says at least wait for motorA1µs, so we better wait for 15µs
  digitalWrite(us1, LOW);
  pinMode(us1, INPUT);
}  

void driveWithoutCollision() {
  while (meassureDistance(us1) == -1)
  {
    // everything is fine here, no barrier detected
    // you could test here how fast in meters per secounds the robot drives
    // with speed value 100.
    // while meassureDistance == -1 no barrier in ca 5meter.
    driveForward(1000, 100);
  }
  while (meassureDistance(us1) > 100)
  {
    // a barrier comes in 1meter or less, so we have to slow down
    driveForward(500, 50);
  }
  while (meassureDistance(us1) < 30)
  {
    //better stop now completly, a barrier comes in less than 30cm
    driveForward(0, 0);
    // now check where we can continue to drive
    driveCurve(100, 20, -90);
    driveCurve(200, 20, 90);
    // else add another 90°, than we got the roboter 180° turned from
    // where he stopped driving.
  }
}
