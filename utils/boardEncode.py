from MatrixGo import GoSGF, GoGame
from time import time


class BoardState(object):
    def __init__(self, game):
        self.game = game
        self.state = []
        for i in range(45):
            if i == 3:
                self.state.append([[1] * 19 for _ in range(19)])
            else:
                self.state.append([[0] * 19 for _ in range(19)])
        if game.player == 1:
            self.state.append([[1] * 19 for _ in range(19)])
        else:
            self.state.append([[0] * 19 for _ in range(19)])

        self.initColor()
        self.initTurnSince()
        self.initQiCaptureSelfAtari()
        self.initLegalPosQiAfterMove()

    def initColor(self):
        """
        set the color in tensor(3 layer)
        layer(3) : 0 - 2(player opponent empty)
        """
        for i in range(19):
            for j in range(19):
                if self.game.board[i][j] == self.game.player:
                    self.state[0][i][j] = 1
                elif self.game.board[i][j] == 0:
                    self.state[2][i][j] = 1
                else:
                    self.state[1][i][j] = 1

    # A constant layer filled with 1
    # layer(1) : 3 - 3

    def initTurnSince(self):
        """
        set turns since: how many turns since a move was played(one-hot encode, 8 layer)
        layer(8) : 4 - 11
        """
        steps = self.game.steps
        epoch = min(7, len(steps))
        for i in range(epoch):
            self.state[4 + i][steps[-(i + 1)].x][steps[-(i + 1)].y] = 1
        for i in range(19):
            for j in range(19):
                if self.game.historyBoard[-epoch][i][j] != 0:
                    self.state[4 + epoch][i][j] = 1

    def initQiCaptureSelfAtari(self):
        """
        set the Qi: number of liberties(one-hot encode, 8 layer)
        layer(8) : 12 - 19(layer[i] is 1 means the block has i - 11 qi)
        set capture size(how many opponent pieces would be captured)
        layer(8) : 20 - 27(layer[i] is 1 means there would be i - 19 pieces of opponent would be captured)
        set self-atari size(how many of own pieces would be captured)
        layer(8) : 28 - 35(layer[i] is 1 means there would be i - 127 pieces of own would be captured)
        """
        tmpBoard = [[0 for _ in range(19)] for _ in range(19)]
        for i in range(19):
            for j in range(19):
                if tmpBoard[i][j] == 0 and self.game.board[i][j] != 0:
                    block = GoGame.GoBlock(GoGame.points[i][j], self.game.board)
                    qi = block.getQi()
                    blockSize = len(block.pieces)
                    for piece in block.pieces:
                        tmpBoard[piece.x][piece.y] = 1
                        qi = qi if qi <= 8 else 8
                        blockSize = blockSize if blockSize <= 8 else 8
                        self.state[11 + qi][piece.x][piece.y] = 1
                        if qi == 1:
                            if self.game.board[piece.x][piece.y] != self.game.player:
                                self.state[19 + blockSize][piece.x][piece.y] = 1
                            else:
                                self.state[27 + blockSize][piece.x][piece.y] = 1

    def initLegalPosQiAfterMove(self):
        """
        whether a move is legal
        layer(1) : 44(layer[44][i][j] is 1 means the point(i, j) is legal to move)
        qi after this move is played(one-hot encode, 8 layer)!!!!!!!!!!!!!!!!!!!!!!!!!(特判是否填眼)
        layer(8) : 36 - 43(layer[k][i][j] is 1 means move to point(i, j) would have k - 35 liberties)
        """
        if not isinstance(self.game, GoGame.Game):
            return False
        for i in range(19):
            for j in range(19):
                step = GoSGF.Step()
                step.initFromArgs('B' if self.game.player == 1 else 'W', GoGame.points[i][j])
                if self.game.moveAnalyze(step):
                    self.state[44][i][j] = 1
                    qi = self.game.targetBlock.qi if self.game.targetBlock.qi < 8 else 8
                    self.state[35 + qi][i][j] = 1


def loadSgf(fileName):
    with open(fileName, encoding='utf-8', errors='ignore') as f:
        text = f.readlines()
        sgf = GoSGF.GoSGF(text[0])
        game = GoGame.Game()
        if sgf.HA != 0:
            sgf.nowStep += sgf.HA - 1
            game.initHandCap(sgf.steps, sgf.HA)
        while sgf.haveNextStep():
            step = sgf.getNextStep()
            if game.moveAnalyze(step):
                game.move()
                tmpState = BoardState(game)

                # print(step)
                # print(game)
            else:
                break


if __name__ == '__main__':
    begin = time()
    loadSgf('../data/test.sgf')
    end = time()
    print(end - begin)
