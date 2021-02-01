class GoEntity(object):
    def __init__(self):
        self.board = GoBoard()

    def show(self):
        print(self)

    def __str__(self):
        pass

    def __repr__(self):
        return self.__str__()

    def countPoint(self):
        pass

    def move(self):
        pass


class GoBlock(object):
    def __init__(self, x, y, board):
        self.beginX = x
        self.beginY = y
        self.board = board
        self.findLinkedBlock()

    def findLinkedBlock(self):
        self.block = set()


class GoBoard(object):
    def __init__(self):
        self.board = [[0 for i in range(19)] for j in range(19)]
        self.player = 1 # player 1 -- black  2 -- white

    def move(self, x, y):
        if self.board[x][y] != 0:
            return -1
        self.board[x][y] = self.player
        return self.legalCheck(x, y)

    def legalCheck(self, x, y):
        self.pickUpPiece(x, y)
        pass

    def pickUpPiece(self, x, y):
        pass


    def countPoint(self):
        pass

    def __str__(self):
        pass

    def __repr__(self):
        pass
