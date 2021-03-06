from qt_demo.MatrixGo import GoSGF, GoGame


def loadSGF():
    with open('D:/GraduationProject/data/jie.sgf', encoding='utf-8', errors='ignore') as f:
        sgf = GoSGF.GoSGF(f)
        board = GoGame.Game()
        while sgf.haveNextStep():
            step = sgf.getNextStep()
            if board.move(step):
                print(sgf.nowStep)
                print(board)
            else:
                break


def commandLine():
    player1 = 'W'
    player2 = 'B'
    game = GoGame.Game()
    while True:
        player1, player2 = player2, player1
        player = player1
        line = input()
        x, y = line.split(' ')
        x = int(x) - 1
        y = int(y) - 1
        step = GoSGF.Step()
        step.initFromCommandLine(x, y, player)
        if game.move(step):
            print(game.numOfStep)
            print(game)
        else:
            player1, player2 = player2, player1


def boardText():
    pass


if __name__ == '__main__':
    commandLine()
