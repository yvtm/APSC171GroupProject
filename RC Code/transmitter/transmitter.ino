#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 8

const int pinJoyStickX = A0;
const int pinJoyStickY = A1;
const int pinJoyStickZ = 2;

const int pinControlSwitch = A4;
const int pinModeSwitch = A5;

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

struct DataPackage {
  int inputX;
  int inputY;
  int inputZ;
};

DataPackage data = {512, 512, 1};

bool isControlRC = true;
bool isModePark = true;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinControlSwitch, INPUT_PULLUP);
  pinMode(pinModeSwitch, INPUT_PULLUP);
  pinMode(pinJoyStickZ, INPUT_PULLUP);
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  data.inputX = analogRead(pinJoyStickX);
  data.inputY = map(analogRead(pinJoyStickY), 0, 1023, 1023, 0);
  data.inputZ = digitalRead(pinJoyStickZ);

  int speedPercent = 0;                    // default speed percentage
  String state = "";                       // reset to empty string after every iteration
  state += isControlRC ? "RC " : "POT ";   // determine control
  state += isModePark ? "PARK" : "DRIVE";  // determine mode

  if (isControlRC) {  // RC control logic
    radio.stopListening();
  //  radio.write(&dataSend, sizeof(DataPackage));
    //myRadio.startListening();
  } else {  // POT control logic
    radio.startListening();
    if (radio.available()) {
  //    radio.read(&dataReceive, sizeof(DataPackage));
    }
    radio.stopListening();
  }

  // display on LCD
  // myLCD.clear();
  // myLCD.print("SPEED: " + String(speedPercent) + "%");
  // myLCD.setCursor(0, 1);
  // myLCD.print("STATE: " + state);

  // updating control and mode states
  isControlRC = (digitalRead(pinControlSwitch) == LOW);  // low (closed switch) equals true
  isModePark = (digitalRead(pinModeSwitch) == LOW);      // high (open switch) equals false

  // for testing sending communications

  Serial.print(data.inputX);
  Serial.print(" ");
  Serial.print(data.inputY);
  Serial.print(" ");
  Serial.print(data.inputZ);
  Serial.print(" ");
  Serial.print(isControlRC);
  Serial.print(" ");
  Serial.println(isModePark);
}
