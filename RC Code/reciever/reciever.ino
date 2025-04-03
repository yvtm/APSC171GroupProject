#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(9,8);

const int pinPotThrottle = A0;
const int pinForwardMotor = 5;
const int pinReverseMotor = 6;
const int pinChargingSensor = A1;
const int pinWhiteLED = 3;
const int pinRedLED = 4;
const int pinGreenLED = 2;
const int pinServo = 7;

const int deadZone = 10;
const int deadZoneLow = 512 - deadZone;
const int deadZoneHigh = 512 + deadZone;
const byte address[6] = "00001";

const int sensorThreshold = 512;

struct DataPackage{
  int inputX;
  int inputY;
  int inputZ;
};

DataPackage data = {512, 512, 1};

Servo myServo;

int calculateMotorOutput(int input) {
  if (input < deadZoneLow) {
    return map(input, 0, deadZoneLow, -1, -63); // will reverse at a quarter of the speed
  }
  if (input > deadZoneHigh) {
    return map(input, deadZoneHigh, 1023, 1, 255);
  }
  return 0; // within the dead zone
}

int motorOutput = 0; // default motor output off
int servoOutput = 90; // default servo output straight
int whiteLEDOutput = 1; // default white led is on
int greenLEDOutput = 0; // default not charging

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinWhiteLED, OUTPUT);
  pinMode(pinRedLED, OUTPUT);
  pinMode(pinGreenLED, OUTPUT);
  pinMode(pinForwardMotor, OUTPUT);
  pinMode(pinReverseMotor, OUTPUT);

  myServo.attach(pinServo);

  digitalWrite(pinWhiteLED, whiteLEDOutput);

  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MAX);
  Serial.print("Began2");
  radio.startListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  //radio.startListening();
  if(radio.available()){
    Serial.println("Success");
  }else{
    Serial.println("Womp womp");
  }
}
