//
// Created by 11409 on 2021/4/17.
#include "Game.hpp"
#include "Step.hpp"
#include "GoBlock.hpp"
#include "Point.hpp"

Game::Game(int player, int boardSize)
{
    this->player = player;
    this->boardSize = boardSize;
    this->historyBoard = std::vector<vector_2d(int)>();
    historyBoard.emplace_back(this->board);
}
//

