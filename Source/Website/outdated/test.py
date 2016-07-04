from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import math
from socketIO_client import SocketIO
import os

#socketIO = SocketIO("localhost", 3000)

displayRadius = 14
imageWidth = 480
mittelpunktX = imageWidth / 2
mittelpunktY = imageWidth / 2
fixedDistance = 5
distanceBetweenLEDs = 0.5
anzahlLEDs = 16
gradProWinkelschritt = 10

#pixels = np.array(Image.open("images/image.png").convert("L"))
#plt.imshow(pixels, cmap= cm.gray)
#plt.show()

#functions
def transformImageToMatrix():
    #pixels = np.array(Image.open("641x641.png").convert("L"))
    pixels = np.array(Image.open("images/image.png").convert("L"))
    plt.imshow(pixels, cmap=cm.gray)
    plt.show()
    
    pixelsScanned = np.ones((480,480))
    pixelMatrix = np.ones((36,anzahlLEDs))
    
    for phi in range(360, 0, -gradProWinkelschritt):
        for n in range(0,anzahlLEDs):
            radiant = phi * (math.pi / 180)
            
            xWert = int(round(mittelpunktX + transformDistanceToPixel((fixedDistance + n * distanceBetweenLEDs) * math.cos(radiant))))
            #print("x: " + str(xWert))
            yWert = int(round(mittelpunktY + (-1 *transformDistanceToPixel((fixedDistance + n * distanceBetweenLEDs) * math.sin(radiant)))))
            #print("y: " + str(yWert))
            
            if(pixels[yWert, xWert] == 0):
                #print("x: " + str(xWert) + " y: " + str(yWert))            
                pixelsScanned[yWert, xWert] = 0
                
                """ 
                weil ab jetzt mit dem Uhrzeigersinn abgetastet wird, muss von 360° runtergezählt werden auf 0°
                
                (0°|LED16) entspricht dem rechts-äußersten Punkt auf dem Kreis
                
                in einer Matrix sollen die Winkelschritte gemerkt werden; sie hat die Form Winkelschritt x LED#
                
                """
                pixelMatrix[(phi-360)/(-10), n] = 0
                
    plt.imshow(pixelsScanned, cmap=cm.gray)
    plt.show()

    plt.imshow(pixelMatrix, cmap=cm.gray)
    plt.show()
    
    #print(pixelMatrix)
    #print("")
    
    for i in range(0,36):
        #print(i)
        binaryToInt(pixelMatrix[i])
            
        #codesend = binaryToInt(pixelMatrix[i])        
       # os.system("sudo /home/pi/433Utils/RPi_utils/codesend ")
            
def transformDistanceToPixel(cm):
    pixelDistance = cm * ((imageWidth / 2) / displayRadius)
    return pixelDistance
    
def binaryToInt(array):
    print(array)
    twoPotency = 1
    sum = 0
    for j in range(anzahlLEDs -1,-1,-1):
        #print("j")
        sum = sum + array[j] * twoPotency
        twoPotency = twoPotency * 2
    print(int(sum))
    #os.system("sudo /home/pi/433Utils/RPi_utils/codesend " + str(int(sum)))

transformImageToMatrix()   

def onImageUpdate(*args):
    #data = args[0]
    print("received update")
    transformImageToMatrix()
    
#socketIO.on('imageUpdate', onImageUpdate)
#socketIO.wait()