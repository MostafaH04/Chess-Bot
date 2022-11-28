
class Spot:
    def __init__(self,x,y, side) -> None:
        self.pos = (x,y)
        self.neighbors = []
        self.side = side
        
        if side == 0:
           self.empty = True
        else:
            self.empty = False

        pass
    
    # everything past init (constructor) was not needed
    # these were prototyped but not tested as an initial mock up
    # for this class
    def addNeighbors(self, spotArr):
        xPos = self.pos[0]
        yPos = self.pos[1]
        
        left = spotArr[yPos, xPos-1]
        right = spotArr[yPos, xPos+1]
        top = spotArr[yPos-1, xPos]
        bottom = spotArr[yPos+1, xPos]

        self.neighbors.append(left, right, top, bottom)

        pass

    def removePeice(self):
        self.side = 0
        self.empty = True

        pass

    def placePeice(self,startSpot):
        if self.empty:
            self.side = startSpot.side
            self.empty = startSpot.empty

            startSpot.removePeice()

            return True
        
        return False
    
    def takeSpot(self, startSpot):
        self.side = startSpot.side
        self.empty = startSpot.empty

        startSpot.removePeice()

        pass
