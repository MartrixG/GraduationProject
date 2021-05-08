//
// Created by 11409 on 2021/4/23.
//

#ifndef MARTRIXGOC_BOARDENCODE_HPP
#define MARTRIXGOC_BOARDENCODE_HPP

#include <cstring>
#include <iostream>
#include <fstream>
#include "Game.hpp"

static void boardEncode(Game &game, std::ofstream &featureFileStream)
{
    int state[7][BOARD_SIZE][BOARD_SIZE];
    memset(state, 0, sizeof(state));
    //color
    //layer:0 0:empty 1:black 2:white
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            state[0][i][j] = game.board[i][j];
        }
    }
    //turn
    //layer:1 state[x][y] = k, (x, y) is last k step
//    std::vector<Step*>* steps = &game.steps;
//    int length = int(steps->size()), epoch = std::min(7, length);
//    for (int i = 0; i < epoch; i++)
//    {
//        state[1][(*steps)[length - i - 1]->x][(*steps)[length - i - 1]->y] = i + 1;
//    }
//    for (int i = 0; i < BOARD_SIZE; i++)
//    {
//        for (int j = 0; j < BOARD_SIZE; j++)
//        {
//            if (game.historyBoard[game.historyBoard.size() - epoch][i][j] != 0)
//            {
//                state[1][i][j] = epoch + 1;
//            }
//        }
//    }
    //qi
    //layer:2 state[x][y] = k, (x, y) has k liberties
    //capture
    //layer:3 state[x][y] = k, (x, y)'block has k points of opponent would be captured
    //selfAtari
    //layer:4 state[x][y] = k, (x, y)'block has k points of own would be captured
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (game.board[i][j] != 0)
            {
                int qi = game.pointBlockMap[game.allBoardPoints[i][j]]->getQi();
                int blockSize = game.pointBlockMap[game.allBoardPoints[i][j]]->getSize();
                qi = qi > 8 ? 8 : qi;
                blockSize = blockSize > 8 ? 8 : blockSize;
                state[2][i][j] = qi;
                if (qi == 1)
                {
                    if (game.board[i][j] != game.player)
                    {
                        state[3][i][j] = blockSize;
                    } else
                    {
                        state[4][i][j] = blockSize;
                    }
                }
            }
        }
    }
    // legal
    // layer:5 1:legal 0:illegal (fill own eyes is illegal)
    // qi after move
    // layer:6 state[x][y] = k move to (x, y) would have k liberties
    int legalMove[BOARD_SIZE * BOARD_SIZE];
    int qiAfterMove[BOARD_SIZE * BOARD_SIZE];
    size_t len = 0;
    game.legalMove(legalMove, qiAfterMove, len);
    for(size_t i = 0; i < len; i++)
    {
        int x = legalMove[i] / BOARD_SIZE, y = legalMove[i] % BOARD_SIZE;
        state[5][x][y] = 1;
        state[6][x][y] = qiAfterMove[i] > 8 ? 8 : qiAfterMove[i];
    }
    for(auto & i : state)
    {
        for(auto & j : i)
        {
            for(auto & k : j)
            {
                featureFileStream << k;
            }
        }
    }
    featureFileStream << '\n';
}

#endif //MARTRIXGOC_BOARDENCODE_HPP
