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
    int boardSize;
    std::vector<Step> steps = std::vector<Step>(0);
    vector_2d(int) board = std::vector<std::vector<int>>(BOARD_SIZE, std::vector<int>(BOARD_SIZE));
    std::vector<vector_2d(int)> historyBoard;
    vector_2d(int) newBoard = std::vector<std::vector<int>>(BOARD_SIZE, std::vector<int>(BOARD_SIZE));;
    Step* nextStep = nullptr;
    GoBlock* targetBlock = nullptr;
    Game(int player, int boardSize);
};

#endif //MARTRIXGOC_GAME_HPP
