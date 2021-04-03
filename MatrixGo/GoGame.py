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
        # player 1 -- black  2 -- white
        self.player = player
        self.boardSize = 19
        self.steps = []
        self.numOfStep = 0
        self.board = [[0 for _ in range(self.boardSize)] for _ in range(self.boardSize)]
        self.historyBoard = [deepcopy(self.board)]
        # next step
        self.nextStep = None
        self.targetBlock = None
        self.newBoard = None

    def initHandCap(self, handCapSteps, numOfStep):
        for i in range(numOfStep - 1):
            self.board[handCapSteps[i].x][handCapSteps[i].y] = 1

    def redo(self):
        self.numOfStep -= 1
        self.steps.pop()
        self.board = self.historyBoard[-1]
        self.historyBoard.pop()
        self.player = 1 if self.player == 2 else 2

    def moveAnalyze(self, step):
        x, y = step.x, step.y
        # player check
        if not((step.player == 'B' and self.player == 1) or (step.player == 'W' and self.player == 2)):
            print('Now step player is illegal.')
            return False
        # location check
        if self.board[x][y] != 0:
            print('Illegal location.')
            return False
        # ko check
        self.newBoard = self.getPickUpBlock(points[x][y])
        if self.newBoard == self.historyBoard[-1]:
            print('Ko illegal.')
            return False
        # qi check
        self.targetBlock = GoBlock(points[x][y], self.newBoard)
        if self.targetBlock.getQi() == 0:
            print('Illegal location.')
            return False
        self.nextStep = step
        return True

    def move(self):
        self.player = 1 if self.player == 2 else 2
        self.historyBoard.append(self.board)
        self.board = self.newBoard
        self.steps.append(self.nextStep)
        self.numOfStep += 1

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
        for block in goBlocks:
            if block.getQi() == 0:
                for piece in block.pieces:
                    tmpBoard[piece.x][piece.y] = 0

        return tmpBoard

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
        self.pieces = []
        self.beginPoint = beginPoint
        self.color = board[beginPoint.x][beginPoint.y]
        self.board = board
        self.qiPoint = []
        self.qi = -1
        self.findLinkedBlock()

    def contain(self, point):
        return self.pieces.__contains__(point)

    def check(self, x, y):
        if 0 <= x <= self.beginPoint.boardSize - 1 and 0 <= y <= self.beginPoint.boardSize - 1:
            return True
        else:
            return False

    def findLinkedBlock(self):
        Q = queue.Queue()
        Q.put(self.beginPoint)
        self.pieces.append(self.beginPoint)
        dx = [0, 0, 1, -1]
        dy = [1, -1, 0, 0]
        while not Q.empty():
            nowPoint = Q.get()
            for i in range(4):
                newX = nowPoint.x + dx[i]
                newY = nowPoint.y + dy[i]
                if self.check(newX, newY) and self.board[newX][newY] == self.color:
                    newPoint = points[newX][newY]
                    if newPoint not in self.pieces:
                        Q.put(newPoint)
                        self.pieces.append(newPoint)

    def getQi(self):
        dx = [0, 0, 1, -1]
        dy = [1, -1, 0, 0]
        for point in self.pieces:
            for i in range(4):
                newX = point.x + dx[i]
                newY = point.y + dy[i]
                if self.check(newX, newY) and self.board[newX][newY] == 0:
                    nowQi = points[newX][newY]
                    if nowQi not in self.qiPoint:
                        self.qiPoint.append(nowQi)
        self.qi = len(self.qiPoint)
        return self.qi

    def __str__(self):
        reStr = ''
        for point in self.pieces:
            reStr += point.__str__() + '\n'
        return reStr

    def __repr__(self):
        return self.__str__()