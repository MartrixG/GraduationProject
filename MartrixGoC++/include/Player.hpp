//
// Created by 11409 on 2021/5/5.
//

#ifndef MARTRIXGOC_PLAYER_HPP
#define MARTRIXGOC_PLAYER_HPP

#include <vector>
#include <random>
#include <chrono>
#include "windows.h"
#include "Step.hpp"
#include "Game.hpp"

class CommandLinePlayer
{
public:
    int playerColor;

    explicit CommandLinePlayer(int color);

    void getNextStep(Step* nextStep) const;

};

class SocketPlayer
{
public:
    char* srcMessage = nullptr;
    int playerColor;

    explicit SocketPlayer(int color);

    void updatePlayer(char* message);

    void getNextStep(Step* nextStep) const;
};

class RandomPlayer
{
public:
    int playerColor;
    int legalMove[BOARD_SIZE * BOARD_SIZE]{};
    int qiAfterMove[BOARD_SIZE * BOARD_SIZE]{};
    size_t legalMoveSize = 0;
    std::default_random_engine randNum;
    std::uniform_int_distribution<int> dist;

    explicit RandomPlayer(int color);

    void getNextStep(Step* nextStep);

    void updatePlayer(Game* game);
};

class MCTSPlayer
{
public:
    int playerColor;
    std::default_random_engine randNum;
    std::uniform_int_distribution<int> dist;

    explicit MCTSPlayer(int color);

    void getNextStep(Step *nextStep);

    void getFirstStep(Step* netStep);

    void updatePlayer(Game* game);
};

#endif //MARTRIXGOC_PLAYER_HPP
