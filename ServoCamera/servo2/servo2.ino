#include <Servo.h>
Servo upper,under; //ประกาศตัวแปรแทน Servo กำหนดข
char tmp;
//left
//mid 81 83
//right 82 42
int up = 81,down = 83;

char python;

int first_count = 0;
int MODE = 0;

int DATA[3];
int data_count = 0;


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
  if(MODE == 0){
    PC1_first_Order();
  }
  else if(MODE == 1){
    PC2_taken_img();
  }
  else if(MODE == 2){
    python_send_all_img();
  }
  else if(MODE == 3){
    Pause();
  }
}


void Pause(){
  while(1){
    
  }
}

void goMID(){
  upper.write(81);
  under.write(83);
}
void goLEFT(){
  upper.write(81);
  under.write(134);
}

void goRIGHT(){
  upper.write(81);
  under.write(41);
}

void PC1_first_Order(){
  Serial.print('R');
  if(Serial.available() > 0){
    python = Serial.read();
    if(python == 'm'){
      goMID();
      first_count += 1;
    }
    else if(python == 'l'){
      goLEFT();
      first_count += 1;
    }
    else if(python == 'r'){
      goRIGHT();
      first_count += 1;
    }
    
  }
  if(first_count == 3){
    MODE = 1;
  }
}

void PC2_taken_img(){
//  Serial.print('n');
  if(Serial.available() > 0){
    python = Serial.read();
    if(python == '4'){
      upper.write(81);
      under.write(83);
      MODE = 2;
      Serial.print('n');
    }
  }
}

void python_send_all_img(){
  if(Serial.available() > 0){
    python = Serial.read();
    if(python == '1'){
      python = Serial.read();
      DATA[0] = python;
      data_count = 1;
      goRIGHT();
      while(1)
      Serial.print('2');
    }
    else if(python == '2'){
      python = Serial.read();
      DATA[1] = python;
      data_count = 2;
      goMID();
      Serial.print('3');
    }
    else if(python == '3'){
      python = Serial.read();
      DATA[2] = python;
      data_count = 3;
      goLEFT();
      Serial.print('q');
    }
  }
  if(data_count == 3){
    Serial.print('g');
    for(int i=0;i<3;i++) Serial.print(DATA[i]);
    MODE = 3;
  }

  
}
