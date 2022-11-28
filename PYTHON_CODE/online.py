import pyautogui
import cv2
import time
from PIL import Image
import numpy as np
from board import Board

def getContours(imgContour): # finds the outlines of the shapes (corners)
    contours,hierarchy = cv2.findContours(imgContour,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    contours = sorted(contours, key=lambda x: cv2.contourArea(x), reverse=True)
    cnt = contours[0]
    cv2.drawContours(imgContour,cnt,-1,(255,0,0),3)
    if True:
        cv2.drawContours(imgContour, cnt, -1, (255,0,0),3)
        peri=cv2.arcLength(cnt,True)
        approx = cv2.approxPolyDP(cnt,0.02*peri,True)
        objCor= len(approx)
        x,y,w,h =cv2.boundingRect(approx) # gives width,height of objects

        if objCor ==4:objectType ="Square"
        else: objectType="None"

        cv2.rectangle(imgContour,(x,y),(x+w,y+h),(0,255,0),2)
        print((x,y))
        print((w,h))
        return x,y,w,h

def drawLines(img):
    totalheight = img.shape[0]
    totalwidth = img.shape[1]
    increment=int(totalwidth/8)
    for i in range (0,totalwidth,increment):
        cv2.line (img,(i,0),(i,totalheight),(255,0,0),4)
        cv2.line (img,(0,i),(totalwidth,i),(255,0,0),4)

def movepiece(xpos1,ypos1,xpos2,ypos2,xValue, yValue,wValue):
    increment=((xValue+wValue)-xValue)/8
    square1x=(xValue+ (xpos1*increment))+increment/2
    square1y=(yValue+ (ypos1*increment))+increment/2
    square2x=(xValue+ (xpos2*increment))+increment/2
    square2y=(yValue+ (ypos2*increment))+increment/2
    pyautogui.dragTo(square1x, square1y, button='left')     # drag mouse to X of 100, Y of 200 while holding down left mouse button
    pyautogui.dragTo(square2x, square2y, 0.1, button='left')
    time.sleep(1)

def unique_count_app(a):
    colors, count = np.unique(a.reshape(-1,a.shape[-1]), axis=0, return_counts=True)
    return colors[count.argmax()]

def checkmateChecker(white255,colourVal):
    if (white255[0] == colourVal[0] and white255[1]==colourVal[1]):
        return print("checkmate")
    else:
        return print("not checkmate")


def GetImage():
    time.sleep(2)
    im2=pyautogui.screenshot('my_screenshot.png')
    img= cv2.imread('my_screenshot.png')
    imgContour= img.copy()


    imgCanny= cv2.Canny(img,50,50)
    xVal,yVal,wVal,hVal= getContours(imgCanny)#calls the function
    crop_image = img[yVal:yVal+hVal, xVal:xVal+wVal]
    return crop_image, xVal,yVal,wVal,hVal

    
