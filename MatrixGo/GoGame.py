import queue
from copy import deepcopy


class Point(object):
    def __init__(self, x, y, boardSize):
        self.x = x
        self.y = y
        self.boardSize = boardSize

    def getAround(self):
        around = []
        dx = [0, 0, 1, -1]
        dy = [1, -1, 0, 0]
        for i in range(4):
            nexX = self.x + dx[i]
            nexY = self.y + dy[i]
            if 0 <= nexX <= self.boardSize - 1 and 0 <= nexY <= self.boardSize - 1:
                around.append(Point(nexX, nexY, self.boardSize))
        return around

    def __hash__(self):
        return self.x * 100 + self.y

    def __eq__(self, other):
        if not isinstance(other, Point):
            return False
        else:
            return self.__hash__() == other.__hash__()

    def __str__(self):
        return 'x:{:}  y:{:}'.format(self.x, self.y)

    def __repr__(self):
        return self.__str__()


points = [[Point(i, j, 19) for j in range(19)] for i in range(19)]


class Game(object):
    def __init__(self, player=1):
        self.boardSize = 19
        self.steps = []
        self.numOfStep = 0
        self.board = [[0 for _ in range(self.boardSize)] for _ in range(self.boardSize)]
        self.preBoard = deepcopy(self.board)
        # player 1 -- black  2 -- white
        self.player = player

    def initHandCap(self, handCapSteps, numOfStep):
        for i in range(numOfStep - 1):
            self.board[handCapSteps[i].x][handCapSteps[i].y] = 1

    def move(self, step):
        if not((step.player == 'B' and self.player == 1) or (step.player == 'W' and self.player == 2)):
            return False
        x, y = step.x, step.y
        targetPoint = points[x][y]
        if self.board[x][y] == 0 and self.legalCheck(targetPoint):
            self.player = 1 if self.player == 2 else 2
            self.steps.append(step)
            self.numOfStep += 1
            return True
        else:
            print('Illegal location.')
            return False

    def legalCheck(self, targetPoint):
        # tmpBoard = self.getPickUpBlock(targetPoint)
        goBlocks, tmpBoard = self.getPickUpBlock(targetPoint)
        if len(goBlocks) > 0:
            for block in goBlocks:
                for point in block.block:
                    tmpBoard[point.x][point.y] = 0
        if tmpBoard == self.preBoard:
            return False
        targetBlock = GoBlock(targetPoint, tmpBoard)
        if targetBlock.getQi() != 0:
            self.preBoard = self.board
            self.board = tmpBoard
            return True
        else:
            return False

    def getPickUpBlock(self, targetPoint):
        goBlocks = set()
        around = targetPoint.getAround()
        tmpBoard = deepcopy(self.board)
        tmpBoard[targetPoint.x][targetPoint.y] = self.player
        for point in around:
            if tmpBoard[point.x][point.y] != self.player and tmpBoard[point.x][point.y] != 0:
                if len(goBlocks) == 0:
                    goBlocks.add(GoBlock(point, tmpBoard))
                containCheck = 1
                for block in goBlocks:
                    if block.contain(point):
                        containCheck = 0
                if containCheck:
                    goBlocks.add(GoBlock(point, tmpBoard))

        deadBlocks = []
        for block in goBlocks:
            if block.getQi() == 0:
                deadBlocks.append(block)
        return deadBlocks, tmpBoard

    def loadFromBoard(self, f):
        line = f.readline()
        i = 0
        while line:
            chess = line.strip().split(' ')
            for j in range(len(chess)):
                self.board[i][j] = int(chess[j])
            i += 1
            line = f.readline()

    def countPoint(self):
        pass

    def __str__(self):
        reStr = ''
        for i in range(self.boardSize):
            for j in range(self.boardSize):
                if self.board[i][j] == 0:
                    reStr += '+  '
                elif self.board[i][j] == 1:
                    reStr += '●  '
                else:
                    reStr += '○  '
            reStr += '\n'
        return reStr

    def __repr__(self):
        return self.__str__()


class GoBlock(object):
    def __init__(self, beginPoint, board):
        self.block = []
        self.beginPoint = beginPoint
        self.color = board[beginPoint.x][beginPoint.y]
        self.board = board
        self.qi = 0
        self.findLinkedBlock()

    def contain(self, point):
        return self.block.__contains__(point)

    def check(self, x, y):
        if 0 <= x <= self.beginPoint.boardSize - 1 and 0 <= y <= self.beginPoint.boardSize - 1:
            return True
        else:
            return False

    def findLinkedBlock(self):
        Q = queue.Queue()
        Q.put(self.beginPoint)
        self.block.append(self.beginPoint)
        dx = [0, 0, 1, -1]
        dy = [1, -1, 0, 0]
        while not Q.empty():
            nowPoint = Q.get()
            for i in range(4):
                newX = nowPoint.x + dx[i]
                newY = nowPoint.y + dy[i]
                if self.check(newX, newY) and self.board[newX][newY] == self.color:
                    newPoint = points[newX][newY]
                    if newPoint not in self.block:
                        Q.put(newPoint)
                        self.block.append(newPoint)

    def getQi(self):
        qi = []
        dx = [0, 0, 1, -1]
        dy = [1, -1, 0, 0]
        for point in self.block:
            for i in range(4):
                newX = point.x + dx[i]
                newY = point.y + dy[i]
                if self.check(newX, newY) and self.board[newX][newY] == 0:
                    nowQi = points[newX][newY]
                    if nowQi not in qi:
                        qi.append(nowQi)
        return len(qi)

    def __str__(self):
        reStr = ''
        for point in self.block:
            reStr += point.__str__() + '\n'
        return reStr

    def __repr__(self):
        return self.__str__()