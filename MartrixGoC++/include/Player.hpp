//
// Created by 11409 on 2021/5/5.
//

#ifndef MARTRIXGOC_PLAYER_HPP
#define MARTRIXGOC_PLAYER_HPP

#include <vector>
#include <random>
#include "windows.h"
#include "Step.hpp"
#include "Game.hpp"

enum playerEnum
{
    commandLinePlayer, socketPlayer, MCTPlayer, randomPlayer
};

enum playColorEnum
{
    black, white
};

class PlayerBase
{
public:
    std::default_random_engine randNum;
    std::uniform_int_distribution<int> dist;
    playerEnum playerType;
    playColorEnum playerColor;
    PlayerBase(playerEnum type, playColorEnum color);
    virtual void getNextStep(Step* nextStep) = 0;
};

class CommandLinePlayer : public PlayerBase
{
public:
    using PlayerBase::PlayerBase;
    void getNextStep(Step* nextStep) override;
};

class SocketPlayer : public PlayerBase
{
public:
    char* srcMessage = nullptr;

    using PlayerBase::PlayerBase;
    void getNextStep(Step* nextStep) override;
    void updatePlayer(char* message);
};

class MCTSPlayer : public PlayerBase
{
public:
    using PlayerBase::PlayerBase;
    void getNextStep(Step *nextStep) override;
    void getFirstStep(Step* netStep);
    void updatePlayer();
};

class RandomPlayer : public PlayerBase
{
public:
    int legalMove[BOARD_SIZE * BOARD_SIZE];
    int qiAfterMove[BOARD_SIZE * BOARD_SIZE];
    size_t legalMoveSize = 0;
    Game* game;

    using PlayerBase::PlayerBase;
    void getNextStep(Step* nextStep) override;
    void updatePlayer(Game* game);

};

#endif //MARTRIXGOC_PLAYER_HPP
