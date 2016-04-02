/*
 Example sketch for the Xbox Wireless Reciver library - developed by Kristian Lauszus
 It supports up to four controllers wirelessly
 For more information see the blog post: http://blog.tkjelectronics.dk/2012/12/xbox-360-receiver-added-to-the-usb-host-library/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <XBOXRECV.h>
#include <Servo.h>


// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
XBOXRECV Xbox(&Usb);
Servo left;
Servo right;
int ledPin = 13;


void arm(){
 // arm the speed controller, modify as necessary for your ESC  
 setSpeed(left, 55); 
 setSpeed(right, 55);
 delay(1000); //delay 1 second,  some speed controllers may need longer
}

void setSpeed(Servo servo, int speed){
 // speed is from 0 to 100 where 0 is off and 100 is maximum speed
 //the following maps speed values of 0-100 to angles from 0-180,
 // some speed controllers may need different values, see the ESC instructions
 int angle = map(speed, 0, 100, 0, 180);
 servo.write(angle);    
}

void updateController(int leftSpeed, int rightSpeed){
  double leftRatio = leftSpeed / 33000.0;
  double rightRatio = rightSpeed / 33000.0;

  //leftMotor = 55.0 + ((abs(leftRatio) > tolerance ? leftRatio : 0.0) * 25.0);
  int leftMotor = 55.0 + (leftRatio * 25.0);
  int rightMotor = 55.0 + (rightRatio * 25.0);

  setSpeed(left, leftMotor);
  setSpeed(right, rightMotor);
  
}

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));

  pinMode(13, OUTPUT);
  left.attach(8);
  right.attach(7);
  arm();
  
}
void loop() {
  Usb.Task();
  if (Xbox.XboxReceiverConnected) {
    for (uint8_t i = 0; i < 4; i++) {
      if (Xbox.Xbox360Connected[i]) {
        if (Xbox.getButtonPress(L2, i) || Xbox.getButtonPress(R2, i)) {
          Serial.print("L2: ");
          Serial.print(Xbox.getButtonPress(L2, i));
          Serial.print("\tR2: ");
          Serial.println(Xbox.getButtonPress(R2, i));
          Xbox.setRumbleOn(Xbox.getButtonPress(L2, i), Xbox.getButtonPress(R2, i), i);
        }

        if (Xbox.getAnalogHat(LeftHatX, i) > 7500 || Xbox.getAnalogHat(LeftHatX, i) < -7500 || Xbox.getAnalogHat(LeftHatY, i) > 7500 || Xbox.getAnalogHat(LeftHatY, i) < -7500 || Xbox.getAnalogHat(RightHatX, i) > 7500 || Xbox.getAnalogHat(RightHatX, i) < -7500 || Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500) {
          if (Xbox.getAnalogHat(LeftHatX, i) > 7500 || Xbox.getAnalogHat(LeftHatX, i) < -7500) {
            Serial.print(F("LeftHatX: "));
            Serial.print(Xbox.getAnalogHat(LeftHatX, i));
            Serial.print("\t");
          }
          if (Xbox.getAnalogHat(LeftHatY, i) > 7500 || Xbox.getAnalogHat(LeftHatY, i) < -7500) {
            Serial.print(F("LeftHatY: "));
            Serial.print(Xbox.getAnalogHat(LeftHatY, i));
            Serial.print("\t");
          }
          if (Xbox.getAnalogHat(RightHatX, i) > 7500 || Xbox.getAnalogHat(RightHatX, i) < -7500) {
            Serial.print(F("RightHatX: "));
            Serial.print(Xbox.getAnalogHat(RightHatX, i));
            Serial.print("\t");
          }
          if (Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500) {
            Serial.print(F("RightHatY: "));
            Serial.print(Xbox.getAnalogHat(RightHatY, i));
          }
          Serial.println();
        }

        if (Xbox.getButtonClick(UP, i)) {
          Xbox.setLedOn(LED1, i);
          Serial.println(F("Up"));
        }
        if (Xbox.getButtonClick(DOWN, i)) {
          Xbox.setLedOn(LED4, i);
          Serial.println(F("Down"));
        }
        if (Xbox.getButtonClick(LEFT, i)) {
          Xbox.setLedOn(LED3, i);
          Serial.println(F("Left"));
        }
        if (Xbox.getButtonClick(RIGHT, i)) {
          Xbox.setLedOn(LED2, i);
          Serial.println(F("Right"));
        }

        if (Xbox.getButtonClick(START, i)) {
          Xbox.setLedMode(ALTERNATING, i);
          Serial.println(F("Start"));
        }
        if (Xbox.getButtonClick(BACK, i)) {
          Xbox.setLedBlink(ALL, i);
          Serial.println(F("Back"));
        }
        if (Xbox.getButtonClick(L3, i))
          Serial.println(F("L3"));
        if (Xbox.getButtonClick(R3, i))
          Serial.println(F("R3"));

        if (Xbox.getButtonClick(L1, i))
          Serial.println(F("L1"));
        if (Xbox.getButtonClick(R1, i))
          Serial.println(F("R1"));
        if (Xbox.getButtonClick(XBOX, i)) {
          Xbox.setLedMode(ROTATING, i);
          Serial.print(F("Xbox (Battery: "));
          Serial.print(Xbox.getBatteryLevel(i)); // The battery level in the range 0-3
          Serial.println(F(")"));
        }
        if (Xbox.getButtonClick(SYNC, i)) {
          Serial.println(F("Sync"));
          Xbox.disconnect(i);
        }

        if (Xbox.getButtonClick(A, i)){
          Serial.println(F("A"));
          digitalWrite(ledPin, HIGH);
        }
        if (Xbox.getButtonClick(B, i)){
          Serial.println(F("B"));
          digitalWrite(ledPin, LOW);
        }
        if (Xbox.getButtonClick(X, i)){
          Serial.println(F("X"));
        }
        if (Xbox.getButtonClick(Y, i)){
          Serial.println(F("Y"));
        }

        updateController(Xbox.getAnalogHat(LeftHatY, i), Xbox.getAnalogHat(RightHatY, i));
        
      }
    }
  }
}
