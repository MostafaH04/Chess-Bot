from board import Board
import cv2 as cv
import numpy as np
import time
import os
import imutils
import requests
from comms import Comms
import chess
import chess.engine

# call back opencv function for mouse clicks
corners = []
def getCorners(event, x,y,flags,param):
    global corners
    if event == cv.EVENT_LBUTTONDOWN:
        corners.append([x,y])
        print(corners)

# state constants
__STATE_STANDBY__ = -1
__STATE_MOVE__ = 0
__STATE_TAKE__ = 1
__STATE_CASTLE__ = 2
__STATE_CHECK__ = 3
__STATE_ERROR__ = 4
__STATE_PROMOTE__ = 5

# letters string used for referncing
# that way each letter maps to an index
# i.e a = 0, b = 1, etc.
letters = "abcdefgh"
# Using a "raw string" since backslackes can be seen as
# actual commands rather than text
engine = chess.engine.SimpleEngine.popen_uci(PATH) 

# ASCI TEXT (used an online ascii text art generator)
# https://patorjk.com/software/taag/#p=display&f=USA%20Flag&t=Chess%20bot
titleStr = r"""
 _____  _   _  _____ _____ _____  ______  _____ _____                            
/  __ \| | | ||  ___/  ___/  ___| | ___ \|  _  |_   _|                           
| /  \/| |_| || |__ \ `--.\ `--.  | |_/ /| | | | | |                             
| |    |  _  ||  __| `--. \`--. \ | ___ \| | | | | |                             
| \__/\| | | || |___/\__/ /\__/ / | |_/ /\ \_/ / | |                             
 \____/\_| |_/\____/\____/\____/  \____/  \___/  \_/
"""

print(titleStr)

cameraInput = input("Input any button to begin.\n")
os.system("cls")

running = True
initialize = False
first = False
#frame = cv.imread("overhead2.png")
#frame2 = cv.imread("overhead2.png")
#currBoard = Board(frame)
ev3 = Comms()
chessSetup = False
prevBotMove = ""

while (running):
    img_resp = requests.get('http://192.168.137.8:8080/shot.jpg')
    img_arr = np.array(bytearray(img_resp.content), dtype=np.uint8)
    frame = cv.imdecode(img_arr, -1)
    frame = imutils.resize(frame, 1000, 1080)
    cv.imshow("cam",frame)
    cv.setMouseCallback("cam", getCorners)
    if len(corners)<4:
        print(corners)
        cv.waitKey(1)
        continue
    if not initialize:
        if not first:
            currBoard = Board(frame,corners)
            first = True
        else:
            currBoard.updateBoard(frame)
        initialize = currBoard.createBoard()
        if initialize:
            ev3.setState(__STATE_STANDBY__)
            continue
        ev3.setState(__STATE_ERROR__)
        ev3.sendComms()
        print("\nBoard not setup correctly!")
        print("--------------------------\n")
        cv.waitKey()
        for row in currBoard.boardMap:
            print(row)
        #cv.imwrite("testing.jpg", frame)
        continue

    if not chessSetup:
        chessSet = chess.Board()
        chessSetup = True
    
    #wait for move
    currBoard.updateBoard(frame)
    for row in currBoard.boardMap:
        print(row)
    changes = currBoard.compareBoard()
    if chessSet.is_game_over():
        print("GAME OVER!")
        break
    
    if (not (len(changes[0]) == 1 and len(changes[1]) == 1) and 
        not (len(changes[0]) == 1 and len(changes[1]) == 2) and 
        not (len(changes[1]) == 0 and len(changes[0]) == 0) and 
        not (len(changes[0])==2 and len(changes[1]) == 2 and
             changes[0][0].pos[1] == changes[0][1].pos[1])):
        print(changes)
        ev3.setState(__STATE_ERROR__)
        ev3.sendComms()
        print("\nERROR: Please reset the board!")
        print("______________________________\n")
        currBoard.bitMapBlack=currBoard.prevBitMapB
        currBoard.bitMapWhite=currBoard.prevBitMapW
        currBoard.boardMap = currBoard.prevBoardMap
        cv.waitKey()
        continue

    print(f"Peices added: {len(changes[0])} \nPeices removed: {len(changes[1])}")
    cv.imwrite("testing.jpg", frame)
    if len(changes[0]) == 0 and len(changes[1]) == 0:
        print("noChanges")
    elif (len(changes[0]) > 0):
        if prevBotMove != "":
            prevBotMove = ""
            time.sleep(0.3)
            continue
        currMove = ""
        if changes[0][0] == changes[1][0]:
            changes[1][0] = changes[1][1]
        elif len(changes[1]) == 2 and len(changes[0]) == 2:
            num1 = 7-changes[1][0].pos[0]
            num2 = 7-changes[1][0].pos[1]+1
            num3 = 7-changes[1][1].pos[0]
            num4 = 7-changes[1][1].pos[1]+1
            
            if num1 == 4:
                temp1 = num1
                temp2 = num2
                num1 = num3
                num2 = num4
                num3 = temp1
                num4 = temp2
        else:
            print(changes[0][0].pos, changes[1][0].pos)
            num1 = 7-changes[0][0].pos[0]
            num2 = 7-changes[0][0].pos[1]+1

            num3 = 7-changes[1][0].pos[0]
            num4 = 7-changes[1][0].pos[1]+1

        outName = letters[num3]+str(num4)+letters[num1]+str(num2)
        print(outName)
        if chess.Move.from_uci(outName) in chessSet.legal_moves:
            chessSet.push_uci(outName)

            if not chessSet.is_game_over():
                result = engine.play(chessSet, chess.engine.Limit(time=0.1))
                chessSet.push(result.move)
                ev3.setState(__STATE_MOVE__)
                prevBotMove = str(result.move)
                if prevBotMove == "e8g8" or prevBotMove == "e8c8":
                    ev3.setState(__STATE_CASTLE__)
                    if prevBotMove == "e8g8":
                        prevBotMove = "e8h8"
                    else:
                        prevBotMove = "e8a8"
                print(prevBotMove)
                letA = letters.find(prevBotMove[0])
                num1 = int(prevBotMove[1])-1
                letB = letters.find(prevBotMove[2])
                num2 = int(prevBotMove[3])-1
                print(letB,num2)
                if currBoard.boardMap[num2][letB] != 0:
                    ev3.setState(__STATE_TAKE__)
                    

                if len(str(result.move))>4:
                    print("JUST PROMOTED")
                    print(prevBotMove[-1])

                ev3.endPosition= [7-letB,num2]
                ev3.startPosition = [7-letA, num1]
                ev3.sendComms()
            else:
                ev3.setState(__STATE_CHECK__)
                print("CHECKMATE")
                ev3.sendComms()
                break


        else:
            ev3.setState(__STATE_ERROR__)
            ev3.sendComms()
            print("ILLEGAL MOVE!")
            currBoard.bitMapBlack=currBoard.prevBitMapB
            currBoard.bitMapWhite=currBoard.prevBitMapW
            currBoard.boardMap = currBoard.prevBoardMap
            cv.waitKey()
            continue
        
        
    time.sleep(0.2)

    #cv.imwrite("testing.jpg", frame)
    #break
    inLetter = cv.waitKey()
    if inLetter == ord('s'):
        running = False

cv.destroyAllWindows()
