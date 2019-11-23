from PIL import Image
from PIL import ImageFilter
from PIL import ImageShow
from PIL import *
import numpy as np
import serial
import keyboard
import time
import os



#! ------------------------- STATIC value     --------------------------------- !#
WHITE = [255, 255, 255]
BLACK = [0, 0, 0]
LEFT = 0
TOP = 40
RIGHT = 240
BOTTOM = 280
RESIZE_val = (240, 320)
NxN = 4
#! ------------------------------------------------------------------------------- !#



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

def goBW(img_data): # !! ----- read crop and save ----- #
    goLIST = np.array(img_data).tolist()
    rowlength = len(goLIST)
    columnlength = len(goLIST[0])
    for row in range(0, rowlength):
        for column in range (0, columnlength):
            if goLIST[row][column][0] > 50 :
                goLIST[row][column] = WHITE
            else:
                goLIST[row][column] =  BLACK
    return np.asarray(goLIST)

def goLineArray(data):
    return np.array(data).tolist()

def goNParray(data):
    return np.asarray(data)

def BWgo2Array(bw_data):
    tempARRAY = np.array(bw_data, dtype=np.uint8)
    return Image.fromarray(tempARRAY)

def GaussBlur(img):
    return img.filter(ImageFilter.GaussianBlur(radius = 2))

def readBMP(inputIMG): # ! ---- READ  BLUR  B&W backtoPIC CROP DONE ---- ! #
    # time.sleep(0.1)
    readIM = inputIMG
    # img_array.save('raw_pic\\'+'BEFORE'+'.bmp')
    imFT = readIM.filter(ImageFilter.GaussianBlur(radius = 2))
    imBW = goBW(imFT)
    imNEW = BWgo2Array(imBW)
    # newSIZE = imNEW.size
    newSIZE = (NxN, NxN)
    imCROP = imNEW.crop((LEFT, TOP, RIGHT, BOTTOM))
    # imCROP.save('raw_pic\\'+'CROP'+'.bmp')
    imDONE = imCROP.resize(newSIZE)
    # imDONE.save('raw_pic\\'+'DATACOM'+'.bmp')
    # imDONE.show()
    # imDONE.save('ImageProcess/raw_pic/'+'DATACOM'+'.bmp')
    # imDONE.show()
    return imDONE
    # !!! must be list in lsit before CHANGE data

def whichPIC(arrayBW):
    output=''
    A_TOP =       [1,1,1,1,0,0,0,0]
    A_LEFT =     [1,0,1,0,1,0,1,0]
    A_RIGHT =      [0,1,0,1,0,1,0,1]
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

def javaCapture():
    img = None
    while(True):
        try:
            img = Image.open("C:\\out\\Pic.bmp",'r')
            break
        except:
            pass
    return img

# ! ------------------------------------------------------------------  Image Processing Function    -------------------------------------------------------------------------!!!
def ImageProcessing():
    Success = 0
    readImage = None
    c_unknown = 0
    while(Success < 20):
        temp = 'Unknown'
        # serialPort = serial_connecting()
        while temp is 'Unknown':
            # print(' Still processing... ')
            readImage = readBMP(javaCapture())
            go2BW = w1b0(array2Dto1D(img2array(readImage)))
            # print(go2BW)
            temp = whichPIC(go2BW)
            if temp is not 'Unknown':
                break
            else:
                c_unknown +=1
                Success = 0
            if c_unknown > 100:
                break
            time.sleep(0.1)
        # time.sleep(0.1)
        Success += 1
    
    # time.sleep(1)
    # print('Success Processed , This picture is >>>',temp)
    return temp
# ! --------------------------------------------------------------------------  XXX  -------------------------------------------------------------------------!!!

def ProcessIMG_out(): # ! <<<< ------ RUN and return what is image :)
    Success_rate = 0
    while(Success_rate < 10):
        output  = ImageProcessing()
        if output != 'Unknown':
            Success_rate += 1
        else:
            break
    #     print('-'*Success_rate,end='')
    # print('processed')

    return output


def serial_connecting(): #! <<< ------ Serial Port Setting
    serialPort = serial.Serial('COM3', 115200, timeout=1)
    return serialPort

def gotoBYTE(cha):
    return cha.encode()

def imageToint(image):
    if image == 'TOP':
        return 'A'
    elif image == 'BOTTOM':
        return 'B'
    elif image == 'LEFT':
        return 'C'
    elif image == 'RIGHT':
        return 'D'
    elif image == 'UPPER':
        return 'E'
    elif image == 'LOWER':
        return 'F'
    elif image == 'Unknown':
        return 'G'

    
# def waitACK(ack):
#     temp = ''
#     while temp != ack:
#         if Ser.inWaiting():
#             temp = Ser.read()
#     print('Got ack :',ack)


#! VOID setup -------------------------------
# os.remove("C:\\out\\Pic.bmp")
Ser = serial_connecting()
Ser.flush()
arduino = ''
time.sleep(3)
current_img = None

mode = 0
c_mode = 0
python_input = None

storage = []
allpic = None
temp_keep = []
#! VOID LOOP ------------------------------
while(1):
    # while not keyboard.i_pressed('q'):
    if Ser.inWaiting():
        arduino = Ser.read()
        if arduino == b'R':
            print('- - - PC1 Order PC2 take all pictures - - -')

            Ser.write(b'r') #! <--- -45 angle
            os.remove("C:\\out\\Pic.bmp")
            time.sleep(5)
            current_img = ProcessIMG_out()
            temp_keep.append(current_img)
            current_img = imageToint(current_img)
            storage.append(current_img)
            

            Ser.write(b'm') #! <--- 0 angle
            os.remove("C:\\out\\Pic.bmp")
            time.sleep(5)
            current_img = ProcessIMG_out()
            temp_keep.append(current_img)
            current_img = imageToint(current_img)
            storage.append(current_img)
            

            Ser.write(b'l') #! <--- 45 angle
            os.remove("C:\\out\\Pic.bmp")
            time.sleep(5)
            current_img = ProcessIMG_out()
            temp_keep.append(current_img)
            current_img = imageToint(current_img)
            storage.append(current_img)
            # time.sleep(3)
            
            for i in range(0,3):
                storage[i] = bytes(storage[i], 'utf')
            print(temp_keep)
            print(storage)

            arduino = '' ###### !  <-----------------  RESET arduino when END step ------------------------

            print('tell arduino for send all img')
            while arduino != b'n': #! @ <--- n = next step
                if Ser.inWaiting():
                    arduino = Ser.read()
                Ser.write(b'4') #! <---- tell arduino , python will send data back
                time.sleep(0.1)
            arduino= ''
            
            print('send all img')
            while arduino != b'g': #! @ <--- send 's' until ack 'g' back
                if Ser.inWaiting():
                    arduino = Ser.read()
                Ser.write(b's')
            print('Got data!!')
            arduino = ''
            
            while arduino != b'g': #!    DATA 1
                if Ser.inWaiting():
                    arduino = Ser.read()
                Ser.write(storage[0])
            print('send data1 already')
            arduino = ''

            while arduino != b'g': #!    DATA 2
                if Ser.inWaiting():
                    arduino = Ser.read()
                Ser.write(storage[1])
            print('send data2 already')
            arduino = ''

            while arduino != b'g': #!    DATA 3
                if Ser.inWaiting():
                    arduino = Ser.read()
                Ser.write(storage[2])
            print('send data3 already')
            arduino = ''

            while arduino != b'g':  #!    got all DATA , send ack back to python
                if Ser.inWaiting():
                    arduino = Ser.read()
            print('Arduino got all Image')
            arduino = ''

            while arduino != b'g':  #! if got order , OUT this loop
                if Ser.inWaiting():
                    arduino = Ser.read()
            print('python tell Arduino waiting for PC1 order')

            print('DONE')
            pause()

                # Ser.write(data)
                # while arduino != b'g':
                #     Ser.write(b'1')
                #     time.sleep(0.1)
                #     Ser.write(data)
                #     time.sleep(0.1)
                #     arduino = Ser.read()
                #     time.sleep(0.1)
                #     print(arduino)
                
                # print('arduino got 1')
                # arduino = ''
                # print('can send 2')
                # while(1):
                #     arduino = Ser.read()
                #     print(arduino)
                    
                

                # Ser.write(data)
                # # while arduino != b'2':
                # if Ser.inWaiting():
                #     arduino = Ser.read()
                # print(arduino)

                # data = int(storage[1], 2)
                # print('send 2')
                # Ser.write(b'2')
                # Ser.write(data)
                
                # if Ser.inWaiting():
                #     arduino = Ser.read()

                # data1 = int(storage[2], 2)
                # print('send 3')
                # Ser.write(b'3')
                # Ser.write(data)
                

            arduino = ''
            print('-----------------------------------')
            while Ser.in_waiting != 3:
                pass
            print(Ser.read_all())
            print('---------------------------------')
            pause()
            
                

    # current_img = ProcessIMG_out()
    # print(current_img)
    
    


















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





