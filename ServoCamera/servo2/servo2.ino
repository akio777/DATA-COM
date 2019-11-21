#include <Servo.h>
Servo upper,under; // <<< define servo value
char inputdata;
//left
//mid 84 87
//right 82 42
int angle = 81,base = 83;

int right = 40;
int mid = 85;
int left = 130;

int current = right;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  under.attach(9);// กำหนดขา 9 ควบคุม Servo
  upper.attach(10);
  under.write(40);
  upper.write(84);
}

void loop() {
  // put your main code here, to run repeatedly:
//  goLEFT(current);
//  Serial.println(current);
//  goLEFT(current);
//  Serial.println(current);
}

void goLEFT(int in){
  int temp = 0;
  for(int i=in;i<=in+45;i++){
    under.write(i);
    delay(75);
    temp = i;
  }
  current = temp;
}
