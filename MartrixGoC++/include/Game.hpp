//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC_GAME_HPP
#define MARTRIXGOC_GAME_HPP

#include <vector>
#include "Step.hpp"
#include "GoBlock.hpp"
#include "Point.hpp"


const int BOARD_SIZE = 19;

class Game
{
public:
    int player;
    int boardSize = BOARD_SIZE;
    vector_2d(Point*) allBoardPoints;
    std::vector<Step *> steps = std::vector<Step *>();
    vector_2d(int) board = vector_2d(int)(BOARD_SIZE, std::vector<int>(BOARD_SIZE));
    std::vector<vector_2d(int)> historyBoard = std::vector<vector_2d(int)>();
    vector_2d(int) newBoard = vector_2d(int)(BOARD_SIZE, std::vector<int>(BOARD_SIZE));
    Step *nextStep = nullptr;
    GoBlock *targetBlock = nullptr;

    Game(int player, vector_2d(Point*) &points);

    void initHandCap(std::vector<Step *> &handCapSteps);

    void redo();

    bool moveAnalyze(Step *Step);

    void move();

    void getPickUpBlock(Point *targetPoint, vector_2d(int) &tmpBoard);
};

#endif //MARTRIXGOC_GAME_HPP
