#include <Servo.h>
Servo upper,under; //ประกาศตัวแปรแทน Servo กำหนดข
char tmp;
//left
//mid 81 83
//right 82 42
int up = 81,down = 83;

char python;

void setup()
{
  Serial.begin(115200);
  under.attach(D7);// กำหนดขา 9 ควบคุม Servo
  upper.attach(D8);
  upper.write(up);
  under.write(down);
  Serial.flush();
//  Serial.print('R');
}

void loop(){
  delay(1000);
  Serial.print('R');
  if(Serial.available() > 0){
    python = Serial.read();
    if(python == 'm'){
      upper.write(81);
      under.write(83);
      
    }
    else if(python == 'l'){
      upper.write(81);
      under.write(134);
      
    }
    else if(python == 'r'){
      upper.write(81);
      under.write(41);
      
    }
    else{
      delay(100);
    }
  }

}

void Pause(){
  while(1){
    
  }
}
