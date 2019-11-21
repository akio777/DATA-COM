import serial
import time
import keyboard
import numpy as np
from PIL import Image
from PIL import ImageShow
import datetime



def millis():
    return int(round(time.time() * 1000))
UNO = 'COM1'
BAUD = 1000000
serialPort = serial.Serial()
serialPort.port = UNO
serialPort.baudrate = BAUD
serialPort.open()
serialPort.flush()
print('*****************\n',UNO,' Opened\n*****************')
inFrame = ''
startWord = ''
word = ['','R','D','Y','']
index = 0
img_a = []
img_b = []
img_c = []
img_d = None
start = 0
while(not keyboard.is_pressed('q')):
    index = 0
    img_a = []
    img_b = []
    img_c = []
    img_d = None
    startWord = ''
    while(startWord != 'RDY' and not keyboard.is_pressed('q')):
        if(serialPort.inWaiting()):
            din = serialPort.read()
            # print(din)
            try:
                data = din.decode('utf-8')
            except:
                data = ''
            if(data == word[index]):
                startWord += data
                index+=1
    #print(startWord)
    print('Reading Image')
    while(not keyboard.is_pressed('q')):
        if(serialPort.inWaiting()):
            din = ord(serialPort.read())
            img_a.append(din)
            img_a.append(din)
            img_a.append(din)
            img_b.append(img_a)
            img_a = []
            if(len(img_b) == 320):
                img_c.append(img_b)
                ##
                # if(len(img_c)%60 == 0):
                    # print(int(len(img_c)*100/240),'%') 
                #print(len(img_c))
                ##
                if(len(img_c) == 240):
                    img_d = np.asarray(img_c,dtype = np.uint8())
                    break
                img_b = []
    print('Image '+str(start)+' Processed\n')
    # startX = millis()
    img_e = Image.fromarray(img_d)
    # img_e = img_e.crop((40,0,280,240)) #LEFT,TOP,RIGHT,BOTTOM
    img_e = img_e.rotate(90)
    # img_e.show()
    img_e.save('ImageProcess/raw_pic/'+str(start)+'.bmp')
    start+=1
    # print(millis()-startX)
    # img_a = img_b = img_c = img_d = img_e = None
    # time.sleep(0.05)
    # while(1):pass
#####
serialPort.close()
print('\******************\n',UNO,' Closed\n******************')