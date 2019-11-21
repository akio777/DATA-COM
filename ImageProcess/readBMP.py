
#==============================================================================================================================================================

from PIL import Image
from PIL import ImageFilter
from PIL import ImageShow
from PIL import *
import numpy as np
import time

# ----- read crop and save ----- #
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

#### white : 100
WHITE = [255, 255, 255]
BLACK = [0, 0, 0]
LEFT = 0
TOP = 40
RIGHT = 240
BOTTOM = 280
NxN = 4
# NxN --- > X * Y

# ! ---- READ  BLUR  B&W backtoPIC CROP DONE ---- ! #
# readIM = Image.open('blur.jpg', 'r')
# imFT = readIM.filter(ImageFilter.GaussianBlur(radius = 3))
# imBW = goBW(imFT)
# imNEW = BWgo2Array(imBW)
# newSIZE = (NxN, NxN)
# imCROP = imNEW.crop((LEFT, TOP, RIGHT, BOTTOM))
# imDONE = imCROP.resize(newSIZE)
# # imDONE.save('DONE.jpg')
# imDONE.show()
# !!! must be list in lsit before CHANGE data

# ! --- test LOOP image SAVE  --- ! #
count = 0
name = ''
while count <= 58: # !!! <<<<<  number of picture (after readBMP)
    name = str(count) + '.bmp'
    readIM = Image.open(name, 'r')
    imFT = readIM.filter(ImageFilter.GaussianBlur(radius = 3))
    imBW = goBW(imFT)
    imNEW = BWgo2Array(imBW)
    newSIZE = (NxN, NxN)
    imCROP = imNEW.crop((LEFT, TOP, RIGHT, BOTTOM))
    imDONE = imCROP.resize(newSIZE)
    savefile = 'xxx/' + name
    imDONE.save(savefile)
    # imDONE.show()
    count += 1
    time.sleep(0.05)



