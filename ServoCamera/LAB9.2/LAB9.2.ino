#include <TEA5767Radio.h>

TEA5767Radio radio = TEA5767Radio();

#include <Servo.h>
Servo upper,under;

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

int PEEK = 500; // >= 500
int MAX = 0;
int prev = 600;
int count = 0;
int t1, t2, t3, t4, t = 0;
int times = 0;
int index = 0;
int state = 0;
bool check = false;
#define ODD 1
#define EVEN 0

#define START 1
#define CONTROL 2
#define ACK 7

#define SENDER 0
#define RECEIVER 1

#define defaultFreq 1700 // dac speed (Hz)
#define f0 500 // FSK f0
#define f1 750 // FSK f1
#define f2 1000 // FSK f2
#define f3 1250 // ASK f3


int up = 81, down = 83;
char python;


int delay0, delay1, delay2, delay3;
/*S_DAC : Amplitude (12bit) of sine wave at 0,90,180,270*/
const uint16_t S_DAC[4] = {2000, 4095, 2000, 0};
Adafruit_MCP4725 dac;

int createParityBit(int);
bool checkParity(int);
void sendFrame(int*);
void sendFSK(int);
void setPayload(int);

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
void setup() {
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  Serial.begin(115200);
  dac.begin(0x64);
  delay0 = (1000000/f0-1000000/defaultFreq)/4;
  delay1 = (1000000/f1-1000000/defaultFreq)/4;
  delay2 = (1000000/f2-1000000/defaultFreq)/4;
  delay3 = (1000000/f3-1000000/defaultFreq)/4;
  Wire.begin();
  radio.setFrequency(106.1);

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
  int tmp = analogRead(A0);
//    Serial.println(tmp);
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
          //          Serial.println("11");
          str[index++] = '1';
          str[index++] = '1';
        }
        else if (count >= 4) {
          //          Serial.println("10");
          str[index++] = '1';
          str[index++] = '0';
        }
        else if (count >= 3) {
          //          Serial.println("01");
          str[index++] = '0';
          str[index++] = '1';
        }
        else if (count >= 1) {
          //          Serial.println("00");
          str[index++] = '0';
          str[index++] = '0';
        }

        count = 0;
        state++;
        Serial.print("state : ");
        Serial.println(state);
      }
      count++;
      Serial.println(count);

    }
    check = false;
    if (micros() - t >= 3550 && state == 6) {
      if (count >= 5) {
        //      Serial.println("11");
        str[index++] = '1';
        str[index++] = '1';
      }
      else if (count >= 4) {
        //      Serial.println("10");
        str[index++] = '1';
        str[index++] = '0';
      }
      else if (count >= 3) {
        //      Serial.println("01");
        str[index++] = '0';
        str[index++] = '1';
      }

      else if (count >= 1) {
        //      Serial.println("00");
        str[index++] = '0';
        str[index++] = '0';
      }
      count = 0;
      state = 7;
    }
    if (state == 7) {
      long value = strtol(str, NULL, 2);
      for(int i=0;i<12;i++)
      {
        Serial.print(str[i]);  
      }
      Serial.println();
      //    Serial.print((char)value);
      delay(1200);
      if (str[2] == '1' && str[3] == '1' && str[4] == '1') {
          
      }
      else {
        if (str[10] == '1' && str[11] == '1') {
          createFrame(ACK);
          sendFrame(newFrame);
          Serial.print('R');
          first_order();
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

void createFrame(int changeMode)
{
  ///change mode loop
  for (int i = 1; i < 4; i++)
  {
    newFrame[i] = mode[changeMode][i - 1];
  }

  ///change payload loop
  if (changeMode != CONTROL)
  {
    for (int i = 4; i < 9; i++)
    {
      newFrame[i] = 0;
    }
  }

  else
  {
    for (int i = 4; i < 9; i++)
    {
      newFrame[i] = payload[i - 4];
    }
  }

  ///change parity bit
  newFrame[9] = createParityBit(EVEN);

  //return startBit + data + stopBit;
}

void setPayload(int angle)
{
  int set[3][2] = {
    {0, 0},
    {0, 1},
    {1, 0}
  };
  //Serial.print("angleee ");
  //Serial.println(angle);
  payload[0] = set[angle][0];
  payload[1] = set[angle][1];
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
      Serial.print("00");
    }
    if (copyFrame[i] == 0 && copyFrame[i + 1] == 1)
    {
      temp = 1;
      Serial.print("01");
    }
    if (copyFrame[i] == 1 && copyFrame[i + 1] == 0)
    {
      temp = 2;
      Serial.print("10");
    }
    if (copyFrame[i] == 1 && copyFrame[i + 1] == 1)
    {
      temp = 3;
      Serial.print("11");
    }
    sendFSK(temp);
    //Serial.print(temp);
  }
  Serial.println();
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

  /*Serial.println();
    Serial.print("count = ");
    Serial.println(count);
  */

  ///check parity
  if (count % 2 == check) return true;
  else return false;
}
//
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
  dac.setVoltage(2000, false); // for don't send
}

void first_order(){
  int count = 0;
  while(count<3){
    if(Serial.available() > 0){
      python = Serial.read();
//      if(count==0){
        if(python == 'm'){
          upper.write(81);
          under.write(83);
//          count+=1;
        }
//      }
//      if(count==1){
        if(python == 'l'){
         upper.write(81);
         under.write(134);
//         count+=1;
      }
//    }
//    if(count==2){
     if(python == 'r'){
        upper.write(81);
        under.write(41);
//        count+=1;
      }
//    }
   }
  }
  
}
