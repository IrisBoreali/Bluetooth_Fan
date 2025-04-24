/*
 * Bluetooth-controlled Rotating Desk Fan
 * Made by Jonathon Donoho for CSC 303 at Carson-Newman University
 * Email: jrdonoho@cn.edu
 */
#include <Servo.h>
#include <SoftwareSerial.h>

int motorPin = 3;
int servoPin = 5;

SoftwareSerial bluetooth(A4,A5);
char btInput;
Servo rotServo;
int currRot = 0, newRot = 0;
int fanSpeed = 127;

bool right = true;
bool fanIsOn = false;
bool sweeping = false;

// Functions
// Uses switch case to enact functions based on bluetooth inputs.
void processInput(char input) {
  switch(input) {
    case 'F': // Fan ON
      if (!fanIsOn){
        fanIsOn = true;
        Serial.println("Fan turned on.");
        analogWrite(motorPin,fanSpeed);
        }
      break;
    case 'f':// Fan OFF
      if (fanIsOn){
        fanIsOn = false;
        right = true;
        Serial.println("Fan turned off.");
        analogWrite(motorPin, 0);
        }
      break;
    case '^': //Increase fan speed
      fanSpeed++;
      if (fanIsOn) analogWrite(motorPin, fanSpeed);
      break;
    case 'V': //Decrease fan speed
      fanSpeed--;
      if (fanIsOn) analogWrite(motorPin, fanSpeed);
      break;
    case '<': //Rotate fan LEFT
      sweeping = false;
      right = false;
      if (newRot > 0) newRot--;
      Serial.println("Attempted left rotation");
      break;
    case '>': //Rotate fan RIGHT
      sweeping = false;
      if (newRot < 180) newRot++;
      Serial.println("Attempted right rotation");
      break;
    case 's': //Turn on or off fan sweeping.
         sweeping = !sweeping;
         Serial.println("Sweeping status changed.");
         break;
    case 10: //Ignore EoL
      break;
    default:
      Serial.print("Unknown input of ");
      Serial.println((int)input);
      break;
  }
}

void rotate(){   
    while (currRot != newRot) {
      Serial.print("Rotating from: ");
      Serial.print(currRot);
      Serial.print(" to: ");
      Serial.println(newRot);
      if (currRot < newRot) currRot += 1;
      else currRot -= 1;
      rotServo.write(currRot);
      delay(15);
    }
   
}

void sweep() {
  if (right && newRot < 180) newRot++;
  else newRot--;
  if (newRot == 180) right = false;
  if (newRot == 0) right = true;
}

void setup() {
  bluetooth.begin(9600);
  Serial.begin(9600);
  rotServo.attach(servoPin);
  newRot = 0;
  rotServo.write(0);
}

void loop() {
  if (bluetooth.available()){
    char btInput = bluetooth.read();
    processInput(btInput);
  }
  if (sweeping) sweep();
  if (currRot != newRot){
    rotate();
  }
}
