import cv2
import numpy as np
import imutils

from spot import Spot

__emptySpace__ = 0
__blackPeice__ = 1
__whitePeice__ = 2

__rook__ = 0
__knight__ = 1
__bishop__ = 2
__queen__ = 3
__king__ = 4
__pawn__ = 5

__boardSize__ = 8
__imageSize__ = 400
__peiceWindow__ = int(__imageSize__/__boardSize__)
__tightWinLow__ = 23
__tightWinUp__ = 28

__blackLowerBound__ = [0, 0, 0]
__blackUpperBound__ = [179, 156, 95]

__whiteLowerBound__ = [0, 0, 127]
__whiteUpperBound__ = [179, 49, 225]

__blackColour__ = [0,0,0]
__whiteColour__ = [255,255,255]

__lineColour__ = (0,255,0)
__lineThickness__ = 1


class Board:
    def __init__(self, boardImg, corners, warped = True) -> None:
        self.topLeftCorner = corners[0]
        self.bottomLeftCorner = corners[1]
        self.topRightCorner = corners[2]
        self.bottomRightCorner = corners[3]

        self.boardCorners = np.float32([self.topLeftCorner,
                                        self.bottomLeftCorner,
                                        self.topRightCorner,
                                        self.bottomRightCorner])

        self._boardCornersOut = np.float32([[0,0],
                                            [0,__imageSize__],
                                            [__imageSize__,0],
                                            [__imageSize__,__imageSize__]])

        self._transformMat = cv2.getPerspectiveTransform(self.boardCorners,
        
                                                   self._boardCornersOut)
    

        self.boardImg = boardImg
        self.boardMap = []

        self.spotMap = []

        self.bitMapBlack = 0
        self.bitMapWhite = 0

        fullRowB = [1 for entry in range(__boardSize__)]
        fullRowW = [2 for entry in range(__boardSize__)]
        emptyRow = [0 for entry in range(__boardSize__)]
        self._startingBoard = [fullRowW,
                               fullRowW,
                               emptyRow,
                               emptyRow,
                               emptyRow,
                               emptyRow,
                               fullRowB,
                               fullRowB]
        self.prevBoardMap = []
        self.warped = warped
        pass
    
    def updateBoard(self, newImg):

        pass

    def readImage(self, image, warped = True):
        newBoardMap = []
        warped = self.warped
        # Generated warped image (to align board corners)
        if warped:
            warpedImg = cv2.warpPerspective(self.boardImg,
                                            self._transformMat,
                                            (self.boardImg.shape[1],
                                             self.boardImg.shape[0]),
                                            flags = cv2.INTER_LINEAR)

            # Reducing image size to 400 by 400
            warpedImg = warpedImg[0:__imageSize__, 0:__imageSize__]
            warpedImg = cv2.resize(warpedImg, (200,200))
            warpedImg = cv2.resize(warpedImg, (400, 400))
            #warpedImg = imutils.rotate(warpedImg, 90)
        
        else:
            warpedImg = cv2.resize(image,
                                   (__imageSize__,__imageSize__))

        
        for rows in range(0,
                          __imageSize__,
                          __peiceWindow__):

            cv2.line(warpedImg, (0,rows),
                    (__imageSize__,rows), __lineColour__,
                    __lineThickness__)

            cv2.line(warpedImg, (rows, 0),
                    (rows, __imageSize__), __lineColour__,
                    __lineThickness__)

            newBoardCol = []
            for cols in range(0,
                              __imageSize__,
                              __peiceWindow__):

                currWindow = warpedImg[rows:rows+__peiceWindow__,
                                       cols:cols+__peiceWindow__]
                
                if warped:   
                    currWindow = currWindow[__tightWinLow__:__tightWinUp__,
                                            __tightWinLow__:__tightWinUp__]
                    #blackImg = (255 - currWindow) 
                    #(inverts the image not being used right now)
                    blackImg = currWindow
                    currWindowB = cv2.cvtColor(blackImg,
                                              cv2.COLOR_BGR2HSV)    


                    blackColrLow = np.array(__blackLowerBound__)
                    blackColrHigh = np.array(__blackUpperBound__)

                    mask = cv2.inRange(currWindowB,
                                       blackColrLow,
                                       blackColrHigh)

                    contoursB = cv2.findContours(mask.copy(),
                                                 cv2.RETR_EXTERNAL,
                                                 cv2.CHAIN_APPROX_SIMPLE) [0]

                    currWindowW = cv2.cvtColor(currWindow,
                                              cv2.COLOR_BGR2HSV) 

                    whiteColrLow = np.array(__whiteLowerBound__)
                    whiteColrHigh = np.array(__whiteUpperBound__)  

                    mask = cv2.inRange(currWindowW,
                                       whiteColrLow,
                                       whiteColrHigh)

                    contoursW = cv2.findContours(mask.copy(),
                                                 cv2.RETR_EXTERNAL,
                                                 cv2.CHAIN_APPROX_SIMPLE) [0]
                    
                    if len(contoursB) > len(contoursW):
                        newBoardCol.append(__blackPeice__)
                    elif len(contoursW) > 0:
                        newBoardCol.append(__whitePeice__)
                    else:
                        newBoardCol.append(__emptySpace__)
                
                else:
                    currWindow = currWindow[__tightWinLow__:__tightWinUp__,
                                            __tightWinLow__:__tightWinUp__]

                    reshaped = currWindow.reshape(-1,currWindow.shape[-1])
                    colours, count = np.unique(reshaped,
                                               axis = 0,
                                               return_counts = True)
                                               
                    dominentColour = colours[count.argmax()]
                    
                    blackCol = np.array(__blackColour__)
                    whiteCol = np.array(__whiteColour__)

                    diffBlack = abs(dominentColour - blackCol)
                    diffWhite = abs(dominentColour - whiteCol)

                    # Calculating average difference
                    difference = 0
                    checkVal = int(dominentColour[-1])
                    for colour in range(len(dominentColour)-1):
                        currVal = int(dominentColour[colour])
                        difference += abs(currVal-checkVal)
                    difference /= 3

                    if difference < 5:
                        if diffBlack[1] < diffWhite[1]:
                            newBoardCol.append(__blackPeice__)
                        else:
                            newBoardCol.append(__whitePeice__)
                    else:
                        newBoardCol.append(__emptySpace__)
                
            newBoardMap.append(newBoardCol)
        
        # This shows the board (for debugging)
        cv2.imshow("board", warpedImg)
        
        return newBoardMap

    def compareBoard(self):
        self._genSpotMap()

        diffBlack = self.prevBitMapB ^ self.bitMapBlack
        diffWhite = self.prevBitMapW ^ self.bitMapWhite
        
        binStringB = bin(diffBlack).replace("0b", "0000000000000000000"
                                                  +"000000000000000000"
                                                  +"000000000000000000"
                                                  +"000000000000000000"
                                                  +"000000000")

        binStringW = bin(diffWhite).replace("0b","000000000000000000"
                                                 +"00000000000000000"
                                                 +"00000000000000000"
                                                 +"00000000000000000"
                                                 +"000000000000")
        
        binStringOldB = bin(self.prevBitMapB).replace("0b","00000000000"
                                                           +"0000000000"
                                                           +"0000000000"
                                                           +"0000000000"
                                                           +"0000000000"
                                                           +"0000000000"
                                                           +"00000000000")

        binStringOldW = bin(self.prevBitMapW).replace("0b","00000000000"
                                                           +"0000000000"
                                                           +"0000000000"
                                                           +"0000000000"
                                                           +"0000000000"
                                                           +"0000000000"
                                                           +"00000000000")
        if (len(binStringOldB) < 65):
            binStringOldB = ("00000000000"
                            +"0000000000"
                            +"0000000000"
                            +"0000000000"
                            +"0000000000"
                            +"0000000000"
                            +"00000000000")

        if (len(binStringOldW) < 65):
            binStringOldW = ("00000000000"
                            +"0000000000"
                            +"0000000000"
                            +"0000000000"
                            +"0000000000"
                            +"0000000000"
                            +"00000000000")
        spotsBlack = []
        spotsWhite = []

        toRemove = []
        toAdd = []
        for boardPos in range(-1,-65,-1):
            if binStringB[boardPos] != "0":
                spotsBlack.append(self.spotMap[boardPos])
                if binStringOldB[boardPos] != "0":
                    toRemove.append(self.spotMap[boardPos])
                else:
                    toAdd.append(self.spotMap[boardPos])
            
            if binStringW[boardPos] != "0":
                spotsWhite.append(self.spotMap[boardPos])
                if binStringOldW[boardPos] != "0":
                    toRemove.append(self.spotMap[boardPos])
                else:
                    toAdd.append(self.spotMap[boardPos])
    
        return[toAdd, toRemove]

    def updateBoard(self, newImg):
        self.boardImg = newImg
        self.createBoard()

        pass


    def createBoard(self):
        self.prevBoardMap = self.boardMap
        self.boardMap = self.readImage(self.boardImg)
        if not self._startingBoardCheck():
            return False    
        
        self._genSpotMap()

        return True

    def _genSpotMap(self):
        self.spotMap = []
        self.prevBitMapB = self.bitMapBlack
        self.prevBitMapW = self.bitMapWhite
        self.bitMapBlack = 0
        self.bitMapWhite = 0

        posCount = 0
        for y in range(8):
            for x in range(8):
                currSpot = Spot(x,y,self.boardMap[y][x]) 
                self.spotMap.append(currSpot)

                valBlack = 0
                valWhite = 0

                if (self.boardMap[y][x] == 2):
                    valBlack = 0
                    valWhite = 1
                elif (self.boardMap[y][x] == 1):
                    valBlack = 1
                    valWhite = 0
                
                self.bitMapBlack += valBlack * (2 ** posCount)
                self.bitMapWhite += valWhite * (2 ** posCount)

                posCount += 1
        
        self.bitMapBoard = self.bitMapBlack ^ self.bitMapWhite

        pass

    def _startingBoardCheck(self):
        if self.boardMap != self._startingBoard:
            return False
        return True

    
