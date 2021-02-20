import queue
from copy import deepcopy


class GoBoard(object):
    def __init__(self, player=1):
        self.boardSize = 19
        self.board = [[0 for _ in range(self.boardSize)] for _ in range(self.boardSize)]
        self.preBoard = self.board
        # player 1 -- black  2 -- white
        self.player = player

    def move(self, step):
        if not((step.player == 'B' and self.player == 1) or (step.player == 'W' and self.player == 2)):
            return False
        x, y = step.x, step.y
        targetPoint = Point(x, y, self.boardSize)
        if self.board[x][y] == 0:
            self.board[x][y] = self.player
        if self.legalCheck(targetPoint):
            self.player = 1 if self.player == 2 else 2
            return True
        else:
            return False

    def legalCheck(self, targetPoint):
        flag = self.pickUpPiece(targetPoint)
        if flag == 2:
            return False
        elif flag == 1:
            return True
        else:
            targetBlock = GoBlock(targetPoint, self.board)
            if targetBlock.getQi() != 0:
                return True
            else:
                return False

    def pickUpPiece(self, targetPoint):
        # flag = 0 不能提 flag = 1 能提 flag = 2 打劫
        flag = 0
        goBlocks = set()
        around = targetPoint.getAround()
        for point in around:
            if self.board[point.x][point.y] != self.player and self.board[point.x][point.y] != 0:
                if len(goBlocks) == 0:
                    goBlocks.add(GoBlock(point, self.board))
                flag = 1
                for block in goBlocks:
                    if block.contain(point):
                        flag = 0
                if flag:
                    goBlocks.add(GoBlock(point, self.board))
        tmpBoard = self.board
        for block in goBlocks:
            if block.getQi() == 0:
                flag = 1
                for point in block.block:
                    tmpBoard[point.x][point.y] = 0
        if tmpBoard == self.preBoard and flag == 1:
            flag = 2
        else:
            self.preBoard = deepcopy(self.board)
            for i in range(self.boardSize):
                for j in range(self.boardSize):
                    self.board[i][j] = tmpBoard[i][j]
            del tmpBoard
        return flag

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
                    newPoint = Point(newX, newY, self.beginPoint.boardSize)
                    if newPoint in self.block:
                        del newPoint
                    else:
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
                    nowQi = Point(newX, newY, self.board)
                    if nowQi not in qi:
                        qi.append(nowQi)
        return len(qi)
