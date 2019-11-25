#include <Servo.h>
Servo upper,under; //ประกาศตัวแปรแทน Servo กำหนดข
//left
//mid 81 83
//right 82 42
int up = 74,down = 83;
char python;
int first_count = 0;
int MODE = 0;
int DATA[3];
int c_data_got = 0;
int data_req = 0;

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
    wait_PC1(   'r'  );   // <-------------------------------  pc1 what u want (l m r)
  }
  else if(MODE == 4){
    wait_python_take_image();
  }else if(MODE == 5){
    Pause();
  }
}


void Pause(){
  while(1){
    
  }
}

void goMID(){
  upper.write(78);
  under.write(83);
}
void goLEFT(){
  upper.write(77);
  under.write(133);
}

void goRIGHT(){
  upper.write(79);
  under.write(44);
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


void PC1_first_Order(){ // MODE == 0
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

void PC2_taken_img(){ // MODE == 1
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

void python_send_all_img(){ // MODE == 2

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
      MODE = 3;
      for(int i=0;i<3;i++){
        Serial.print(DATA[i]);
      }
      Serial.print('g');
      MODE = 3;
      
    } 
  }
}


// ------------------------------------------------------------------------------------------- //

void wait_PC1(char in){    //  MODE == 3
//   if(Serial.available() > 0){
//      in = Serial.read();
      if(in == 'm'){
        goMID();
        MODE = 4;
        Serial.print('g');
        delay(1000);
        MODE = 4;
      }
      else if(in == 'l'){
        goLEFT();
        MODE = 4;
        Serial.print('g');
        delay(1000);
        MODE = 4;
      }
      else if(in == 'r'){
        goRIGHT();
        MODE = 4;
        Serial.print('g');
        delay(1000);
        MODE = 4;
      }
      else if(in == 'X'){ // DATA lost , send nack   NINE NINE NINE NINE NINE NINE
        /////
      }
//   }
}

void wait_python_take_image(){  // MODE == 4
  if(Serial.available() > 0){
    python = Serial.read();
    if(python == 's' and c_data_got == 4){
      c_data_got = 5;
      Serial.print('g');
    }
    if(python != 's' and c_data_got ==5){
      data_req = python;
      Serial.print('g');
      c_data_got == 6;
      MODE = 5;
    }
  }
}
