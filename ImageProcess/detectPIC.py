from PIL import Image
from PIL import ImageFilter
from PIL import ImageShow
from PIL import *
import numpy as np
import time

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


def whichPIC(arrayBW):
    output = ''
    BOTTOM =       [1,1,1,1,0,0,0,0]
    RIGHT =      [1,0,1,0,1,0,1,0]
    LOWER =     [1,1,0,1,0,1,0,0]
    TOP =    [0,0,0,0,1,1,1,1]
    LEFT =     [0,1,0,1,0,1,0,1]
    UPPER =     [0,0,1,0,1,0,1,1]
    fixer = [1,2,4,7,8,11,13,14]
    checker = []
    for i in fixer:
        checker.append(arrayBW[i])
    
    if checker == TOP:
        output = 'TOP'
    elif checker == LEFT:
        output = 'LEFT'
    elif checker == RIGHT:
        output = 'RIGHT'
    elif checker == BOTTOM:
        output = 'BOTTOM'
    elif checker == UPPER:
        output = 'UPPER'
    elif checker == LOWER:
        output = 'LOWER'
    else:
        output = 'Unknown'
    return output
        


# A = w1b0(array2Dto1D(RIGHT_REF)) # !!! <<<<< 16pixels goto back and white 1Darray 16 index
# print(whichPIC(A))
count = 0
while(count<=58): # !!! <<<<<  number of picture (after readBMP)
    name = 'xxx/' + str(count) + '.bmp'
    read = img2array(Image.open(name, 'r'))
    temp = whichPIC(w1b0(array2Dto1D(read)))
    print('PIC :', count, temp)
    count += 1



