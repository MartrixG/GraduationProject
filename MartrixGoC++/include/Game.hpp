//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC_GAME_HPP
#define MARTRIXGOC_GAME_HPP

#include <vector>
#include <string>
#include <iostream>
#include "Step.hpp"
#include "GoBlock.hpp"
#include "Point.hpp"


const int BOARD_SIZE = 19;

class Game
{
public:
    int player = BLACK_PLAYER;
    int boardSize = BOARD_SIZE;
    vector_2d(Point*) allBoardPoints;
    std::vector<Step *> steps = std::vector<Step *>();
    vector_2d(int) board = vector_2d(int)(BOARD_SIZE, std::vector<int>(BOARD_SIZE));
    std::vector<vector_2d(int)> historyBoard = std::vector<vector_2d(int)>();
    vector_2d(int) newBoard = vector_2d(int)(BOARD_SIZE, std::vector<int>(BOARD_SIZE));
    vector_2d(int) tmpBoard = vector_2d(int)(BOARD_SIZE, std::vector<int>(BOARD_SIZE));
    Step *nextStep = nullptr;
    GoBlock *targetBlock = nullptr;


    explicit Game(vector_2d(Point*) &points);

    void initHandCap(std::vector<Step *> &handCapSteps);

    void redo();

    bool moveAnalyze(Step *Step);

    void move();

    void getPickUpBlock(Point *targetPoint, vector_2d(int) &processBoard);

    void loadFromBoard(const std::string& fileName, int player);

    friend void operator<<(std::ostream &out, const Game &o)
    {
        std::cout << " ";
        for(int i = 0; i < BOARD_SIZE; i++)
        {
            std::cout << " " << char('a' + i);
        }
        std::cout << std::endl;
        for(int i = 0; i < BOARD_SIZE; i++)
        {
            std::cout << char('a' + i) << " ";
            for(int j = 0; j < BOARD_SIZE; j++)
            {
                switch (o.board[i][j])
                {
                    case 0:
                        std::cout << "  ";break;
                    case BLACK_PLAYER:
                        std::cout << "x ";break;
                    case WHITE_PLAYER:
                        std::cout << "o ";break;
                }
            }
            std::cout << '\n';
        }
    }
};

#endif //MARTRIXGOC_GAME_HPP
