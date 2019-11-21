import serial
import keyboard
import time
import numpy as np
from PIL import Image
from PIL import ImageShow


def serial_connecting():
    UNO = 'COM1'
    B2M = 1000000
    PRE = 2
    BAUD = B2M / PRE
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






# UNO = 'COM1'
# B2M = 1000000
# PRE = 2
# BAUD = B2M / PRE
# serialPort = serial.Serial()
# serialPort.port = UNO
# serialPort.baudrate = 1000000
# serialPort.open()
# serialPort.flush()
# print('*****************\n',UNO,' Opened\n*****************')
# inFrame = ''
# startWord = ''
# word = ['','R','D','Y','']
# index = 0
# img_a = []
# img_b = []
# img_c = []
# img_d = None
# start = 0
# while(not keyboard.is_pressed('q')):
#     index = 0
#     pixel = []
#     column = []
#     row = []
#     startWord = ''
#     while(startWord != 'RDY' and not keyboard.is_pressed('q')):
#         if(serialPort.inWaiting()):
#             din = serialPort.read()
#             try:
#                 data = din.decode('utf-8')
#             except:
#                 data = ''
#             if(data == word[index]):
#                 startWord += data
#                 index+=1
#     print('Reading Image')
#     while(not keyboard.is_pressed('q')):
#         if(serialPort.inWaiting()):
#             din = ord(serialPort.read())
#             pixel.append(din)
#             pixel.append(din)
#             pixel.append(din)
#             column.append(pixel)
#             pixel = []
#             if(column.__len__() == 320):
#                 row.append(column)
#                 column = []
            
#             if(row.__len__()== 240):
#                 array2IM = np.asarray(row, dtype = np.uint8())
#                 array2IM = np.rot90(array2IM)
#                 row = []
#                 break
#     imDONE = Image.fromarray(array2IM)
#     imDONE.save('ImageProcess/raw_pic/'+str(start)+'.bmp')
#     serialPort.flush()
#     row = column = pixel = array2IM = imDONE =  [].clear()
#     start+=1
# #####
# serialPort.close()
# print('\******************\n',UNO,' Closed\n******************')