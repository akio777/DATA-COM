#include <TEA5767Radio.h>
#include <Wire.h>
#include<Adafruit_MCP4725.h>
#include<Adafruit_ADS1015.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
#define ACK 7
#define r_slope 250

#define ODD 1
#define EVEN 0

#define SENDPIC 2
#define SENDDATA 4
#define ACK 7

#define SENDER 0
#define RECEIVER 1

#define defaultFreq 1700 // dac speed (Hz)
#define f0 500 // FSK f0
#define f1 750 // FSK f1
#define f2 1000 // FSK f2
#define f3 1250 // ASK f3

TEA5767Radio radio = TEA5767Radio();

int PEEK = 500; // >= 500
int MAX = 0;
int prev = 600;
int count = 0;
int t1, t2, t3, t4, t = 0;
int times = 0;
int index = 0;
int state = 0;
bool check = false;


int delay0, delay1, delay2, delay3;
const uint16_t S_DAC[4] = {2047, 4095, 2047, 0};
Adafruit_MCP4725 dac;

int createParityBit(int);
void createFrame(int, int, int);
bool checkParity(int);
void sendFrame(int*);
void sendFSK(int);
void setPayload(int, int);
void sendPic(int, int, int);

long timeOut = 3000, timer = 0;
bool getAck = false;
int parityType = EVEN, myState = SENDER;
int newFrame[12] = {1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1};
int mode[8][3] = {
  {0, 0, 0},
  {0, 0, 1},
  {0, 1, 0},
  {0, 1, 1},
  {1, 0, 0},
  {1, 0, 1},
  {1, 1, 0},
  {1, 1, 1}
};
int payload[5] = {0, 0, 0, 0, 0};
char in;
String inMode = "";


// PYTHON -------------------------------------- //
#include <Servo.h>
Servo upper, under; //ประกาศตัวแปรแทน Servo กำหนดข
//left
//mid 81 83
//right 82 42
int up = 81, down = 83;
char python;
int first_count = 0;
int MODE = 0;
int DATA[3] = {0,0,0};
int c_data_got = 0;


void setup() {
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  Serial.begin(115200);
  dac.begin(0x64);
  delay0 = (1000000 / f0 - 1000000 / defaultFreq) / 4;
  delay1 = (1000000 / f1 - 1000000 / defaultFreq) / 4;
  delay2 = (1000000 / f2 - 1000000 / defaultFreq) / 4;
  delay3 = (1000000 / f3 - 1000000 / defaultFreq) / 4;
  Wire.begin();
  radio.setFrequency(104.1);

  under.attach(8);// กำหนดขา 9 ควบคุม Servo
  upper.attach(9);
  upper.write(up);
  under.write(down);

  Serial.flush();

}
void reset_millis()
{
  extern volatile unsigned long timer0_millis, timer0_overflow_count;
  noInterrupts();
  timer0_millis = timer0_overflow_count = 0;
  interrupts();
}
void loop() {
  int tmp = analogRead(A1);
  //      Serial.println(tmp);
  char str[12];
  if (tmp - prev > r_slope && check == false) {
    MAX = 0;
    check = true;
  }
  if (tmp > MAX) {
    MAX = tmp;
  }
  if (MAX - tmp > r_slope) {
    if (check == true) {
      if (micros() - t >= 3550) {
        reset_millis();
        t = micros() - 200;
        if (count >= 5) {
          str[index++] = '1';
          str[index++] = '1';
        }
        else if (count >= 4) {
          str[index++] = '1';
          str[index++] = '0';
        }
        else if (count >= 3) {
          str[index++] = '0';
          str[index++] = '1';
        }
        else if (count >= 1) {
          str[index++] = '0';
          str[index++] = '0';
        }

        count = 0;
        state++;
        //        Serial.print("state : ");
        //        Serial.println(state);
      }
      count++;
      //            Serial.println(count);

    }
    check = false;
    if (micros() - t >= 3550 && state == 6) {
      if (count >= 5) {
        str[index++] = '1';
        str[index++] = '1';
      }
      else if (count >= 4) {
        str[index++] = '1';
        str[index++] = '0';
      }
      else if (count >= 3) {
        str[index++] = '0';
        str[index++] = '1';
      }
      else if (count >= 1) {
        str[index++] = '0';
        str[index++] = '0';
      }
      state = 7;
    }
    if (state == 7) {
      //      react1();
      //      long value = strtol(str, NULL, 2);
      //      for (int i = 0; i < 12; i++)
      //        Serial.print(str[i]);
      //      Serial.println();
      //    Serial.print((char)value);
      delay(2000);
      if (str[1] == '1' && str[2] == '1' && str[3] == '1'); //ACK

      else if (str[1] == '0' && str[2] == '0' && str[3] == '1') { //Start PC1 -> PC2
        if (str[10] == '1' && str[11] == '1') { //Stop bit
          createFrame(ACK, 0, 0);
          sendFrame(newFrame);
          delay(1000);
          //          sendPic(0, 1, 2);
          //          Serial.print('R');
          //          first_order(); //Start Shooting Pic
//          while(MODE!=4){
//            if (MODE == 0) {
//              PC1_first_Order();
//            }
//            else if (MODE == 1) {
//              PC2_taken_img();
//            }
//            else if (MODE == 2) {
//              python_send_all_img();
//            }
//            else if(MODE == 3){
//              goMID();
//              sendPic(DATA[0] - 65, DATA[1] - 65, DATA[2] - 65);
//              MODE += 1;
//            }
//          }
            react1();
            delay(5000);
            goRIGHT();
            delay(1000);
            goMID();
            delay(1000);
            goLEFT();
            delay(1000);
            goMID();
            sendPic(3,1,4);

          //Start Send Pic to PC1 Step2
//          if(DATA[2] != 0)
          

        }
      }

      else if (str[1] == '0' && str[2] == '1' && str[3] == '1') { //Start Send data back to PC1
        if (str[10] == '1' && str[11] == '1') { //Stop bit
          createFrame(ACK, 0, 0);
          sendFrame(newFrame);
          delay(1000);
          if (str[4] == '0' && str[5] == '1') { // -45
          }
          else if (str[4] == '0' && str[5] == '0') { // 0
          }
          else if (str[4] == '1' && str[5] == '0') { // 45
          }
          //Start Send
        }
      }
      delay(2000);
      index = 0;
      for (int i = 0; i < 12; i++)
        str[i] = NULL;
      count = 0;
      state = 0;
      t = 0;
    }
  }
}

void createFrame(int changeMode, int angle, int pic)
{
  ///change mode loop
  for (int i = 1; i < 4; i++)
  {
    newFrame[i] = mode[changeMode][i - 1];
  }

  ///change payload loop
  if (changeMode == ACK)
  {
    for (int i = 4; i < 9; i++)
    {
      newFrame[i] = 0;
    }
  }

  else if (changeMode == 2)
  {
    //Set Payload
    setPayload(angle, pic);
    for (int i = 4; i < 9; i++)
    {
      newFrame[i] = payload[i - 4];
    }
  }

  ///change parity bit
  newFrame[9] = createParityBit(EVEN);

  //return startBit + data + stopBit;
}

void setPayload(int angle, int pic)
{
  int set[3][2] = {
    {0, 1},
    {0, 0},
    {1, 0}
  };
  int setPic[7][3] = {
    {0, 0, 1},
    {0, 1, 0},
    {0, 1, 1},
    {1, 0, 0},
    {1, 0, 1},
    {1, 1, 0},
    {1, 1, 1}
  };
  payload[0] = set[angle][0];
  payload[1] = set[angle][1];
  payload[2] = setPic[pic][0];
  payload[3] = setPic[pic][1];
  payload[4] = setPic[pic][2];
}
void setPayloadData(int count, int pic) {
  int set[4] = {1, 1, 1, 0};
  int setPic[6][4][4] = {
    {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {1, 1, 1, 1},
      {1, 1, 1, 1},
    },
    {
      {1, 1, 1, 1},
      {1, 1, 1, 1},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
    },
    {
      {0, 0, 1, 1},
      {0, 0, 1, 1},
      {0, 0, 1, 1},
      {0, 0, 1, 1},
    },
    {
      {1, 1, 0, 0},
      {1, 1, 0, 0},
      {1, 1, 0, 0},
      {1, 1, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {1, 0, 0, 0},
      {1, 1, 0, 0},
      {1, 1, 1, 0},
    },
    {
      {0, 1, 1, 1},
      {0, 0, 1, 1},
      {0, 0, 0, 1},
      {0, 0, 0, 0},
    },
  };
  payload[0] = set[count];
  payload[1] = setPic[pic][count][0];
  payload[2] = setPic[pic][count][1];
  payload[3] = setPic[pic][count][2];
  payload[4] = setPic[pic][count][3];
}
void sendPic(int pic1, int pic2, int pic3) {
  //  Serial.println();
  createFrame(SENDPIC, 0, pic1);
  sendFrame(newFrame);
  //  Serial.println();
  delay(1000);
  createFrame(2, 1, pic2);
  sendFrame(newFrame);
  //  Serial.println();
  delay(1000);
  createFrame(2, 2, pic3);
  sendFrame(newFrame);
  delay(1000);
}
void sendData(int pic) { //Send Data
  createFrame(SENDPIC, 0, pic);
  sendFrame(newFrame);
  delay(1000);
  createFrame(SENDDATA, 1, pic);
  sendFrame(newFrame);
  delay(1000);
  createFrame(SENDDATA, 2, pic);
  sendFrame(newFrame);
  delay(1000);
  createFrame(SENDDATA, 2, pic);
  sendFrame(newFrame);
  delay(1000);
}
void sendFrame(int *frameToSend)
{
  int copyFrame[12];
  int temp = 0;

  for (int i = 0; i < 12; i++)
    copyFrame[i] = newFrame[i];

  for (int i = 0 ; i < 12; i += 2)
  {
    if (copyFrame[i] == 0 && copyFrame[i + 1] == 0)
    {
      temp = 0;
      //      Serial.print("00");
    }
    if (copyFrame[i] == 0 && copyFrame[i + 1] == 1)
    {
      temp = 1;
      //      Serial.print("01");
    }
    if (copyFrame[i] == 1 && copyFrame[i + 1] == 0)
    {
      temp = 2;
      //      Serial.print("10");
    }
    if (copyFrame[i] == 1 && copyFrame[i + 1] == 1)
    {
      temp = 3;
      //      Serial.print("11");
    }
    sendFSK(temp);
    //Serial.print(temp);
  }
  //  Serial.println();
}

int createParityBit(int parityType)
{
  if (checkParity(parityType)) return 0;
  else return 1;
}

bool checkParity(int check)
{
  int count = 0;
  int temp;
  int copyFrame[8];

  for (int i = 0; i < 8; i++)
    copyFrame[i] = newFrame[i + 1];

  /*Serial.println(copyFrame);
    Serial.println(frame);
    Serial.println();
    Serial.print("temp = ");
  */

  for (int j = 1; j < 8; j++)
  {
    temp = int(copyFrame[j]) & 1;
    //Serial.print(temp);
    if (temp == 1) count++;
    copyFrame[j] >>= 1;
  }

  ///check parity
  if (count % 2 == check) return true;
  else return false;
}

void sendFSK(int tmp)
{
  if (tmp == 0) { // for input 00 -> 500 Hz 2cycle/baud
    //Serial.println("00");
    for (int i = 0 ; i < 2 ; i++) {
      for (int j = 0 ; j < 4 ; j++) {
        dac.setVoltage(S_DAC[j], false);
        delayMicroseconds(delay0);
      }
    }
  }
  else if (tmp == 1) { // for input 01 - 750 Hz 3cycle/baud
    //Serial.println("01");
    for (int i = 0 ; i < 3 ; i++) {
      for (int j = 0 ; j < 4 ; j++) {
        dac.setVoltage(S_DAC[j], false);
        delayMicroseconds(delay1);
      }
    }
  }
  else if (tmp == 2) { // for input 10 - 1000 Hz 4cycle/baud
    //Serial.println("10");
    for (int i = 0 ; i < 4 ; i++) {
      for (int j = 0 ; j < 4 ; j++) {
        dac.setVoltage(S_DAC[j], false);
        delayMicroseconds(delay2);
      }
    }
  }
  else if (tmp == 3) { // for input 11 - 1250 Hz 5cycle/baud
    //Serial.println("11");
    for (int i = 0 ; i < 5 ; i++) {
      for (int j = 0 ; j < 4 ; j++) {
        dac.setVoltage(S_DAC[j], false);
        delayMicroseconds(delay3);
      }
    }
  }
  in >>= 2;
  //Serial.println("----------------------");
  dac.setVoltage(2047, false); // for don't send
}





void Pause(){
  while(1){
    
  }
}

void goMID(){
  upper.write(up);
  under.write(86);
}
void goLEFT(){
  upper.write(77);
  under.write(134);
}

void goRIGHT(){
  upper.write(74);
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

char decode_order_PC1(char x){
  if(x == 'l'){
    delay(100);
    goLEFT();
    return x;
  }
  else if(x == 'm'){
    delay(100);
    goMID();
    return x;
  }
  else if(x == 'r'){
    delay(100);
    goRIGHT();
    return x;
  }
  else{
    x = 'X';
    return x;
  }
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


// ------------------------------------------------------------------------------------------- //

void wait_PC1(char in){
  if(decode_order_PC1(in) != 'X'){
     // got order 
     Serial.print('g');
      react1();
      react2();
      goMID();
      MODE = 4;
  }
  else{
    /// ------ send NAK back to PC1 ------ ///
  }
  
}
