import matplotlib.pyplot as plt


class Robot:
    def __init__(self,position):  # Create agent robot & update param of agent
        self.robot = plt.Circle((position[0], position[1]), 10, color='red')  # Create agent robot


    def getPosition(self): # Update position of robot
        self.position = [self.robot.center[0], self.robot.center[1]]

    def createPath(self,taget,speed=10):
        self.getPosition()
        return [range(self.position[0],taget[0],speed),range(self.position[1],taget[1],speed)]
    def move(self,target):
        path = self.createPath(target)
        print(path[0])
    def checkchange(self, frame):
        self.robot.set_color("red")
        if (frame % 2 == 0):
            self.robot.set_color("blue")
        return self.robot,
# if __name__ == "__main__":
#     env = Robot(position=[0,0])