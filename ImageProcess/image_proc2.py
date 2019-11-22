from IMPbw import *
from PIL import Image
from PIL import ImageFilter
from PIL import ImageShow
from PIL import *
import numpy as np
import serial
import keyboard
import time
import os


RESIZE_val = (240, 320)


def go3to1index(data):
    row_length = len(data)
    column_length = len(data[0])
    for row in range(0, row_length):
        for column in range(0, column_length):
            change = data[row][column][0]
            data[row][column] = change
    # print('row_length  : ', len(data[row]))
    # print('data_length  : ', len(data))
    return data

def ref_image(path_img):
    path = "C:\\out\\ref_"+str(path_img)+".bmp"
    image = Image.open(path,'r')
    image = image.resize(RESIZE_val)
    im = goNParray(image) #]
    im = goLineArray(im)  #] ---- #! processing for Image.PIL array goto ---> 320x240 lineArray
    im = go3to1index(im)  #]
    return im

def checkWhite(data):
    if data == 0:
        return True
    else:
        return False

def countBW(data):
    row_length = len(data)
    column_length = len(data[0])
    cB = 0
    cW = 0
    for row in range(0,row_length):
        for column in range(0,column_length):
            if checkWhite(data[row][column]):
                cW +=1
            else:
                cB +=1
    # print('black:',cB,' white:',cW)
    return cB,cW

def READ_realimage():
    image = javaCapture()                          #!! << read image
    image = image.rotate(180)                      #!! << rotate for think as our see
    imBW = goBW(image)                             #!! << real_image go to ---> black white
    imBW = BWgo2Array(imBW)                        #!! << real_black_white go to ---> image.fromarray
    imBW = imBW.crop((LEFT, TOP, RIGHT, BOTTOM))   #!! <<  Crop for ez check
    imBW = imBW.resize(RESIZE_val)                 #!! << resize as real_image
    # imBW = imBW.resize((4, 4))                     #!! << down_size for go to 4x4 pixels
    # imBW = imBW.resize((240, 320))                 #!! resize back to real_image for check B&W 
    # imBW.show()
    imBW = goLineArray(imBW)                       #!! pixels[row[column]] goto --- > list[list[]]
    DONEimage = go3to1index(imBW)                  #!! defalut 1 pixels has [r,g,b] goto ---> only one value
    return DONEimage

def compareImage(image, ref):
    row_length = len(image)
    column_length = len(image[0])
    percent = 0
    for row in range(0,row_length):
        for column in range(0,column_length):
            if image[row][column] == ref[row][column]:
                percent += 1
    return (percent*100)//(row_length*column_length)

def What_image(a, b, c, d, e, f):
    temp = [a, b, c, d, e, f]
    index = temp.index(max(temp))
    answer = ['left', 'right', 'upper', 'lower', 'bottom', 'top']
    if temp[index] > 80:
        return answer[index]
    else:
        return False


ref_LEFT = ref_image('left')
ref_RIGHT = ref_image('right')
ref_TOP = ref_image('top')
ref_BOTTOM = ref_image('bottom')
ref_UPPER = ref_image('upper')
ref_LOWER = ref_image('lower')

bw_left = countBW(ref_LEFT)
bw_right = countBW(ref_RIGHT)
bw_top = countBW(ref_TOP)
bw_bottom = countBW(ref_BOTTOM)
bw_upper = countBW(ref_UPPER)
bw_lower = countBW(ref_LOWER)

def ImageProcessing_V2(image):
    real_image = image
    c_left = compareImage(real_image, ref_LEFT) #! <--- compare with ref_LEFT
    c_right = compareImage(real_image, ref_RIGHT) #! <--- compare with ref_RIGHT
    c_upper = compareImage(real_image, ref_UPPER) #! <--- compare with ref_UPPER
    c_lower = compareImage(real_image, ref_LOWER) #! <--- compare with ref_LOWER
    c_bottom = compareImage(real_image, ref_BOTTOM) #! <--- compare with ref_BOTTOM
    c_top = compareImage(real_image, ref_TOP) #! <--- compare with ref_TOP
    return What_image(c_left, c_right, c_upper, c_lower, c_bottom, c_top)


# os.remove("C:\\out\\Pic.bmp")
while(1):
    image = READ_realimage()
    while(1):
        process = ImageProcessing_V2(image)
        if process != False:
            print(process)
            break
        else:
            print('Unknown')
    time.sleep(0.1)





# while(1):
#     image = javaCapture() # !! << read image
#     image = image.rotate(180) # !! << rotate for think as our see
#     imBW = goBW(image) # !! << real_image go to ---> black white
#     imBW = BWgo2Array(imBW) # !! << real_black_white go to ---> image.fromarray
#     imBW = imBW.crop((LEFT, TOP, RIGHT, BOTTOM)) #!! <<  Crop for ez check
#     imBW = imBW.resize((240, 320)) # !! << resize as real_image
#     imBW = imBW.resize((4, 4)) # !! << down_size for go to 4x4 pixels
#     imBW = imBW.resize((240, 320)) # !! resize back to real_image for check B&W
#     imBW.save("C:\\out\\P_real1.bmp") 
#     imBW = goLineArray(imBW) #!! pixels[row[column]] goto --- > list[list[]]
#     imBW = go3to1index(imBW) #!! defalut 1 pixels has [r,g,b] goto ---> only one value
#     time.sleep(0.1)











































