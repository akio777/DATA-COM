#include <Servo.h>
Servo upper,under; //ประกาศตัวแปรแทน Servo กำหนดข
char tmp;
//left
//mid 84 87
//right 82 42
int up = 81,down = 83
;
int change = 0;
void setup()
{
  Serial.begin(9600);
  under.attach(9);// กำหนดขา 9 ควบคุม Servo
  upper.attach(10);
}
void loop(){
  if(Serial.available()){
    tmp = Serial.read();
    if (tmp == 's'){
      up = up +1;
    }
    else if (tmp == 'w'){
      up =up- 1;
    }
    else if (tmp == 'a'){
      down = down+ 1;
    }
    else if (tmp == 'd'){
      down -= 1;
    }
    if (tmp == 'k'){
      up = up +10;
    }
    else if (tmp == 'i'){
      up =up- 10;
    }
    else if (tmp == 'j'){
      down = down+ 10;
    }
    else if (tmp == 'l'){
      down -= 10;
    }
  }
  upper.write(up);
  Serial.print("Up : ");
  Serial.print(up);
  under.write(down);
  Serial.print("Down : ");
  Serial.println(down);
  delay(250);
}
