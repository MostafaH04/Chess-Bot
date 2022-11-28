import vgamepad as vg
import time

__STATE_STANDBY__ = -1
__STATE_MOVE__ = 0
__STATE_TAKE__ = 1
__STATE_CASTLE__ = 2
__STATE_CHECK__ = 3
__STATE_ERROR__ = 4
__STATE_PROMOTE__ = 5

class Comms:
    def __init__(self) -> None:
        self.gamepad = vg.VX360Gamepad()
        self.resetComms()

        pass

    def resetComms(self):
        self.state = __STATE_STANDBY__
        self.startPosition = [0,0]
        self.endPosition = [0,0]

        self.gamepad.reset()
        #this is standby state (sends 0,0 to robot)
        self.gamepad.left_joystick_float(x_value_float=-1, y_value_float=1)
        self.gamepad.update()
    
    def sendComms(self):
        #state
        print(self.state)
        if self.state == __STATE_ERROR__:
            self.gamepad.press_button(
                button = vg.XUSB_BUTTON.XUSB_GAMEPAD_LEFT_SHOULDER)
        elif self.state == __STATE_TAKE__:
            self.gamepad.press_button(
                button = vg.XUSB_BUTTON.XUSB_GAMEPAD_START)
        elif self.state == __STATE_CASTLE__:
            self.gamepad.press_button(
                button = vg.XUSB_BUTTON.XUSB_GAMEPAD_LEFT_THUMB)
        elif self.state == __STATE_CHECK__:
            self.gamepad.press_button(
                button = vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_THUMB)
        elif self.state == __STATE_MOVE__:
            self.gamepad.press_button(
                button = vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_SHOULDER)
        elif self.state == __STATE_PROMOTE__:
            self.gamepad.press_button(
                button = vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_SHOULDER)
            self.gamepad.press_button(
                button = vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_THUMB)

        if (self.state != __STATE_ERROR__ or 
            self.state != __STATE_CHECK__ or 
            self.state != __STATE_STANDBY__):
            #start position

            yCord = bin(self.startPosition[1]).replace("0b","000000")
            if yCord[-4] == "1":
                self.gamepad.press_button(
                    button=vg.XUSB_BUTTON.XUSB_GAMEPAD_Y)
            if yCord[-3] == "1":
                self.gamepad.press_button(
                    button=vg.XUSB_BUTTON.XUSB_GAMEPAD_X)
            if yCord[-2] == "1":
                self.gamepad.press_button(
                    button=vg.XUSB_BUTTON.XUSB_GAMEPAD_B)
            if yCord[-1] == "1":
                self.gamepad.press_button(
                    button=vg.XUSB_BUTTON.XUSB_GAMEPAD_A)

            yCord = self.startPosition[0]
            if yCord == 0:
                self.gamepad.press_button(
                    button = vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_UP)
            if yCord == 1:
                self.gamepad.press_button(
                    button = vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_UP)
                self.gamepad.press_button(
                    button = vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_RIGHT)
            if yCord == 2:
                self.gamepad.press_button(
                    button = vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_RIGHT)
            if yCord == 3:
                self.gamepad.press_button(
                    button = vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_RIGHT)
                self.gamepad.press_button(
                    button = vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_DOWN)
            if yCord == 4:
                self.gamepad.press_button(
                    button = vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_DOWN)
            if yCord == 5:
                self.gamepad.press_button(
                    button = vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_DOWN)
                self.gamepad.press_button(
                    button = vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_LEFT)
            if yCord == 6:
                self.gamepad.press_button(
                    button = vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_LEFT)
            if yCord == 7:
                self.gamepad.press_button(
                    button = vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_LEFT)
                self.gamepad.press_button(
                    button = vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_UP)

            # end point
            xCord2 = (2/8)*self.endPosition[0]-1
            yCord2 = (2/8)*self.endPosition[1]-1
            self.gamepad.left_joystick_float(x_value_float=xCord2,
                y_value_float=-yCord2)

        self.gamepad.update()
        # comms stay on for 3 seconds
        time.sleep(3)

        self.resetComms()
        pass

    def setState(self, state):
        self.state = state
        pass

    def setPosStart(self, arr):
        for elem in arr:
            if elem > 7:
                elem = 7
            elif elem < 0:
                elem = 0
        
        self.startPosition = arr
        pass
    
    def setPosEnd(self, arr):
        for elem in arr:
            if elem > 7:
                elem = 7
            elif elem < 0:
                elem = 0
        
        self.endPosition = arr
        pass