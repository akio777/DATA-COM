#include <Servo.h>
Servo upper,under; //ประกาศตัวแปรแทน Servo กำหนดข
//left
//mid 81 83
//right 82 42
int up = 81,down = 83;
char python;
int first_count = 0;
int MODE = 0;
int DATA[3];
int c_data_got = 0;


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
    wait_PC1();
  }
}


void Pause(){
  while(1){
    
  }
}

void goMID(){
  upper.write(up);
  under.write(83);
}
void goLEFT(){
  upper.write(up);
  under.write(134);
}

void goRIGHT(){
  upper.write(up);
  under.write(41);
}

void react1(){
      goMID();
      delay(200);
      goLEFT();
      delay(200);
      goMID();
      delay(200);
      goLEFT();
      delay(200);
      goMID();
      delay(200);
      goLEFT();
      delay(200);
}

void react2(){
    goRIGHT();
    delay(200);
    goMID();
    delay(200);
    goRIGHT();
    delay(200);
    goMID();
    delay(200);
    goRIGHT();
    delay(200);
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
//      react1();
      MODE = 2;
      Serial.print('n');
    }
  }
}


void python_send_all_img(){

  if( Serial.available() > 0){
    python = Serial.read();
    if(python == 's' and c_data_got == 0){
//      react1();
//      react2();
//      goMID();
      Serial.print('g'); // <---- ready for receive data
//      goRIGHT();
      c_data_got = 1;
    }
    else if(python != 's' and c_data_got == 1){ // DATA 1
//      react1();
//      react2();
      DATA[0] = python;
      Serial.print('g'); // <---- got data , send ack back
      c_data_got = 2;
    }
    else if(python != 's' and c_data_got == 2){ // DATA 2
//      react2();
//      react1();
      DATA[1] = python;
      Serial.print('g'); // <---- got data , send ack back
      c_data_got = 3;
      
    }
    else if(python != 's' and c_data_got == 3){ // DATA 2
//      react2();
//      react1();
      DATA[2] = python;
      Serial.print('g'); // <---- got data , send ack back
      c_data_got = 4;
    }
    else if(python != 's' and c_data_got == 4){
      Serial.print('g');
      MODE = 3;
      for(int i=0;i<3;i++){
        Serial.print(DATA[i]);
      }
      delay(1000);
      Serial.print('g');
    } 
  }
}

void wait_PC1(){
  // NINE
  while(1){
    delay(1000);
  }
  react1();
  RIGHT();
  MODE = 4;
}
