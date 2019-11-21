from PIL import Image
from PIL import ImageFilter
from PIL import ImageShow
from PIL import *
import numpy as np
import serial
import keyboard
import time

WHITE = [255, 255, 255]
BLACK = [0, 0, 0]
LEFT = 0
TOP = 40
RIGHT = 240
BOTTOM = 280
NxN = 4

def img2array(img_data):
    goLIST = np.array(img_data).tolist()
    return goLIST

def array2Dto1D(array2D):
    output = []
    for row in range (0, len(array2D)):
        for column in range(0, len(array2D[0])):
            output.append(array2D[row][column][0])
    return output

def w1b0(array1D):
    output = []
    for i in range(0,len(array1D)):
        if array1D[i] == 0:
            output.append(1)
        else:
            output.append(0)
    return output

# !! ----- read crop and save ----- #
def goBW(img_data):
    goLIST = np.array(img_data).tolist()
    for row in range(0, len(goLIST)):
        for column in range (0,len(goLIST[0])):
            if goLIST[row][column][0] > 50 :
                goLIST[row][column] = WHITE
            else:
                goLIST[row][column] =  BLACK
    return np.asarray(goLIST)

def BWgo2Array(bw_data):
    tempARRAY = np.array(bw_data, dtype=np.uint8)
    return Image.fromarray(tempARRAY)

def readBMP():
    # ! ---- READ  BLUR  B&W backtoPIC CROP DONE ---- ! #
    readIM = Image.open('C:/out/Pic.bmp', 'r')
    # img_array.save('raw_pic\\'+'BEFORE'+'.bmp')
    imFT = readIM.filter(ImageFilter.GaussianBlur(radius = 2))
    imBW = goBW(imFT)
    imNEW = BWgo2Array(imBW)
    # newSIZE = imNEW.size
    newSIZE = (NxN, NxN)
    imCROP = imNEW.crop((LEFT, TOP, RIGHT, BOTTOM))
    imCROP.save('raw_pic\\'+'CROP'+'.bmp')
    imDONE = imCROP.resize(newSIZE)
    imDONE.save('raw_pic\\'+'DATACOM'+'.bmp')
    # imDONE.show()
    # imDONE.save('ImageProcess/raw_pic/'+'DATACOM'+'.bmp')
    # imDONE.show()
    return imDONE
    # !!! must be list in lsit before CHANGE data

def whichPIC(arrayBW):
    output=''
    A_TOP =       [1,1,1,1,0,0,0,0]
    A_RIGHT =     [1,0,1,0,1,0,1,0]
    A_LEFT =      [0,1,0,1,0,1,0,1]
    A_UPPER =     [1,1,0,1,0,1,0,0]
    A_BOTTOM =    [0,0,0,0,1,1,1,1]
    A_LOWER =     [0,0,1,0,1,0,1,1]
    fixer = [1,2,4,7,8,11,13,14]
    checker = []
    for i in fixer:
        checker.append(arrayBW[i])
    
    if checker == A_TOP:
        output = 'TOP'
    elif checker == A_LEFT:
        output = 'LEFT'
    elif checker == A_RIGHT:
        output = 'RIGHT'
    elif checker == A_BOTTOM:
        output = 'BOTTOM'
    elif checker == A_UPPER:
        output = 'UPPER'
    elif checker == A_LOWER:
        output = 'LOWER'
    else:
        output = 'Unknown'
    return output

def pause():
    while(1):pass

def serial_connecting():
    UNO = 'COM1'
    serialPort = serial.Serial()
    serialPort.port = UNO
    serialPort.baudrate = 1000000
    serialPort.open()
    return serialPort

def readCAMERA(serial_connected):
    # print('- - - - SerialPort Connecting - - - -')
    serial_connected.flush()
    inFrame = ''
    startWord = ''
    word = ['','R','D','Y','']
    index = 0    
    index = 0
    pixel = []
    column = []
    row = []
    startWord = ''
    percent = 0
    while(startWord != 'RDY' and not keyboard.is_pressed('q')):
        if(serial_connected.inWaiting()):
            din = serial_connected.read()
            try:
                data = din.decode('utf-8')
            except:
                data = ''
            if(data == word[index]):
                startWord += data
                index+=1
    # print('Reading Image')
    while(not keyboard.is_pressed('q')):
        if(serial_connected.inWaiting()):
            din = ord(serial_connected.read())
            pixel.append(din)
            pixel.append(din)
            pixel.append(din)
            column.append(pixel)
            pixel = []
            if(column.__len__() == 320):
                row.append(column)
                column = []
            
            if(row.__len__()== 240):
                array2IM = np.asarray(row, dtype = np.uint8())
                array2IM = np.rot90(array2IM)
                row = []
                break
            # percent += 1
            # print('='*((1//percent)*(100//percent)),end='')
    print('Processed --->> ',end='')
        # print('')
    imDONE = Image.fromarray(array2IM)
    # imDONE.save('ImageProcess/raw_pic/'+'DATACOM'+'.bmp')
    serial_connected.flush()
    return imDONE
    # print('\******************\n',UNO,' Closed\n******************')


# !!! --------------------------------------------------------------------------  MAIN  -------------------------------------------------------------------------!!!

round = 0
while(True):
    temp = 'Unknown'
    count = 0
    # serialPort = serial_connecting()
    while temp is 'Unknown':
        readImage = readBMP()
        go2BW = w1b0(array2Dto1D(img2array(readImage)))
        temp = whichPIC(go2BW)
        if temp is not 'Unknown':
            break
        if count == 7:
            print('- - -TIME OUT CANT PROCESS - - -')
            count = 0
        else:
            print(temp)
        count+=1
    print('# ---- ROUND : ',round,' ---- #')
    round += 1
    # time.sleep(1)

#!!! --------------------------------------------------------------------------  XXX  -------------------------------------------------------------------------!!!













# image = image.
# A = w1b0(array2Dto1D(RIGHT_REF)) # !!! <<<<< 16pixels goto back and white 1Darray 16 index
# print(whichPIC(A))
# count = 0
# while(count<=58): # !!! <<<<<  number of picture (after readBMP)
#     name = 'xxx/' + str(count) + '.bmp'
#     read = img2array(Image.open(name, 'r'))
#     temp = whichPIC(w1b0(array2Dto1D(read)))
#     print('PIC :', count, temp)
#     count += 1


# !!! Only ONCE RUN !!! #
# temp = 'Unknown'
# count = 0
# serialPort = serial_connecting()
# while temp is 'Unknown':
#     image = readCAMERA(serialPort)
#     detect = readBMP(image)
#     temp = whichPIC(w1b0(array2Dto1D(img2array(detect))))
#     if temp is not 'Unknown':
#         # detect.save('ImageProcess\\raw_pic\\'+'DATACOM'+'.bmp')
#         serialPort.close()
#     if count == 7:
#         serialPort.close()
#         print('- - -TIME OUT CANT PROCESS - - -')
#         time.sleep(0.100)
#         serialPort.open()
#         count = 0
#     count+=1
#     print(temp)





