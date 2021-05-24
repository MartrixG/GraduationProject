//
// Created by 11409 on 2021/5/13.
//

#ifndef MARTRIXGOC_RANDOMPLAYER_HPP
#define MARTRIXGOC_RANDOMPLAYER_HPP

#include <random>
#include <chrono>
#include "Game.hpp"

class RandomPlayer
{
public:
    int playerColor;
    int* legalMove;
    int* qiAfterMove;
    size_t legalMoveSize = 0;
    std::default_random_engine randNum;
    std::uniform_int_distribution<int> dist;

    explicit RandomPlayer(int color);

    void getNextStep(Step* nextStep);

    void updatePlayer(Game* game);

    ~RandomPlayer();
};

#endif //MARTRIXGOC_RANDOMPLAYER_HPP
