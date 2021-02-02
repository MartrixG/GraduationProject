import queue


class GoBoard(object):
    def __init__(self):
        self.boardSize = 19
        self.board = [[0 for _ in range(self.boardSize)] for _ in range(self.boardSize)]
        # player 1 -- black  2 -- white
        self.player = 1

    def move(self, targetPoint):
        x = targetPoint.x
        y = targetPoint.y
        if self.board[x][y] != 0:
            return -1
        self.board[x][y] = self.player
        return self.legalCheck(targetPoint)

    def legalCheck(self, targetPoint):
        self.pickUpPiece(targetPoint)
        pass

    def pickUpPiece(self, targetPoint):
        goBlocks = set()
        around = targetPoint.getAround()
        for point in around:
            if self.board[point.x][point.y] != self.player:
                goBlocks.add(GoBlock(point, self.board))

    def countPoint(self):
        pass

    def __str__(self):
        pass

    def __repr__(self):
        pass


class Point(object):
    def __init__(self, x, y, boardSize):
        self.x = x
        self.y = y
        self.boardSize = boardSize

    def getAround(self):
        around = set()
        dx = [0, 0, 1, -1]
        dy = [1, -1, 0, 0]
        for i in range(4):
            nexX = self.x + dx[i]
            nexY = self.y + dy[i]
            if 0 <= nexX <= self.boardSize and 0 <= nexY <= self.boardSize:
                around.add(Point(nexX, nexY, self.boardSize))
        return around

    def __eq__(self, other):
        if not isinstance(other, Point):
            return False
        else:
            if other.x == self.x and other.y == self.y:
                return True
            else:
                return False


class GoBlock(object):
    def __init__(self, beginPoint, board):
        self.block = set()
        self.beginPoint = beginPoint
        self.color = board[beginPoint.x][beginPoint.y]
        self.board = board
        self.qi = 0
        self.findLinkedBlock()

    def findLinkedBlock(self):
        def check(x, y):
            if 0 <= x <= self.beginPoint.boardSize and 0 <= y <= self.beginPoint.boardSize:
                return True
            else:
                return False
        Q = queue.Queue()
        Q.put(self.beginPoint)
        self.block.add(self.beginPoint)
        dx = [0, 0, 1, -1]
        dy = [1, -1, 0, 0]
        while not Q.empty():
            for i in range(4):
                nowPoint = Q.get()
                newX = nowPoint.x + dx[i]
                newY = nowPoint.y + dy[i]
                if check(newX, newY) and self.board[newX][newY] == self.color:
                    newPoint = Point(newX, newY, self.beginPoint.boardsize)
                    Q.put(newPoint)
                    self.block.add(newPoint)
