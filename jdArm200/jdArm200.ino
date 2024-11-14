/*MIT License

Copyright (c) 2024 JD edu. http://jdedu.kr author: conner.jeong@gmail.com
     
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
     
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
     
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN TH
SOFTWARE.*/

#include <Wire.h>
#include <Arduino.h>
#include <U8x8lib.h>
#include "ServoEasing.hpp"

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

#define ACTION_TIME_PERIOD 1000

const int base_pin = 0;
const int shoulder_pin = 1;
const int upperarm_pin = 2;
const int gripper_pin = 3;

float baseAngle = 90.0;
float shoulderAngle = 100.0;
float upperarmAngle = 135.0;
float gripperAngle = 180.0;

float last_baseAngle = baseAngle;
float last_shoulderAngle = shoulderAngle;
float last_upperarmAngle = upperarmAngle;
float last_gripperAngle = gripperAngle;

// 서보의 이동 시간 설정
int base_st = 1000;
int shoulder_st = 1000;
int upperarm_st = 1000;
int gripper_st = 1000;

String inString;

ServoEasing base(PCA9685_DEFAULT_ADDRESS, &Wire);
ServoEasing shoulder(PCA9685_DEFAULT_ADDRESS, &Wire);
ServoEasing upperarm(PCA9685_DEFAULT_ADDRESS, &Wire);
ServoEasing gripper(PCA9685_DEFAULT_ADDRESS, &Wire);

void setup() {
  Serial.begin(115200);
  
  Serial.println("baud:115200");

  base.attach(base_pin);
  shoulder.attach(shoulder_pin);
  upperarm.attach(upperarm_pin);
  gripper.attach(gripper_pin);

  base.setEasingType(EASE_CUBIC_IN_OUT);
  shoulder.setEasingType(EASE_CUBIC_IN_OUT);
  upperarm.setEasingType(EASE_CUBIC_IN_OUT);
  gripper.setEasingType(EASE_CUBIC_IN_OUT);

  base.write(last_baseAngle);
  shoulder.write(last_shoulderAngle);
  upperarm.write(last_upperarmAngle);
  gripper.write(last_gripperAngle);

  delay(500);
}

void loop() {
  if (Serial.available() > 0) {
    inString = Serial.readStringUntil('\n');
    baseAngle = constrain(inString.substring(inString.indexOf('a') + 1, inString.indexOf('b')).toInt(), 0, 360);
    shoulderAngle = constrain(inString.substring(inString.indexOf('b') + 1, inString.indexOf('c')).toInt(), 0, 180);
    upperarmAngle = constrain(inString.substring(inString.indexOf('c') + 1, inString.indexOf('d')).toInt(), 0, 180);
    gripperAngle = constrain(inString.substring(inString.indexOf('d') + 1, inString.indexOf('e')).toInt(), 0, 180);

    set_servo();
    Serial.print("a ");
    Serial.print(baseAngle);
    Serial.print(" b ");
    Serial.print(shoulderAngle);
    Serial.print(" c ");
    Serial.print(upperarmAngle);
    Serial.print(" d ");
    Serial.print(gripperAngle);
    Serial.println(" e");
  }

}

void set_servo() {
  if (baseAngle != last_baseAngle) {
    base.startEaseToD(baseAngle, base_st);
  }
  if (shoulderAngle != last_shoulderAngle) {
    shoulder.startEaseToD(shoulderAngle, shoulder_st);
  }
  if (upperarmAngle != last_upperarmAngle) {
    upperarm.startEaseToD(upperarmAngle, upperarm_st);
  }
  if (gripperAngle != last_gripperAngle) {
    gripper.startEaseToD(gripperAngle, gripper_st);
  }
  last_baseAngle = baseAngle;  // 이전 각도 갱신
  last_shoulderAngle = shoulderAngle;
  last_upperarmAngle = upperarmAngle;
  last_gripperAngle = gripperAngle;
}
