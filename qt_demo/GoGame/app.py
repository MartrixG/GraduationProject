import GoBoard
if __name__ == '__main__':
    game = GoBoard.GoBoard()
    with open('test_board') as f:
        game.load(f)
    print(game)