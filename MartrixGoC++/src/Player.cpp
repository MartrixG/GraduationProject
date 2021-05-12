//
// Created by 11409 on 2021/5/5.
//

#include <iostream>
#include <cstring>
#include "Player.hpp"
#include "MCTS.hpp"

PlayerBase::PlayerBase(playerEnum type, int color)
{
    this->playerType = type;
    this->playerColor = color;
    this->randNum = std::default_random_engine(GetCurrentThreadId() + std::chrono::system_clock::now().time_since_epoch().count());
    this->dist = std::uniform_int_distribution<int>(0, 0x7fffffff);
}

void CommandLinePlayer::getNextStep(Step* nextStep)
{
    int x, y;
    std::cin >> x >> y;
    nextStep->player = playerColor;
    nextStep->pos = (x - 1) * BOARD_SIZE + y - 1;
}

void SocketPlayer::getNextStep(Step* nextStep)
{
    size_t len = strlen(this->srcMessage);
    int pos = 0;
    for(size_t i = 1; i < len; i++)
    {
        pos *= 10;
        pos += this->srcMessage[i] - '0';
    }
    nextStep->player = this->playerColor;
    nextStep->pos = pos;
}

void SocketPlayer::updatePlayer(char* message)
{
    this->srcMessage = message;
}

void MCTSPlayer::getFirstStep(Step* nextStep)
{
    nextStep->player = playerColor;
    int star = BOARD_SIZE * (BOARD_SIZE / 4 - 1) + BOARD_SIZE - (BOARD_SIZE / 4);
    int dPos[5] = {0, 1, -1, BOARD_SIZE, -BOARD_SIZE};
    int i = this->dist(this->randNum) % 5;
    nextStep->pos = star + dPos[i];
}

void MCTSPlayer::getNextStep(Step* nextStep)
{
    nextStep->player = playerColor;
    nextStep->pos = this->dist(this->randNum) % (BOARD_SIZE * BOARD_SIZE);
}

void MCTSPlayer::updatePlayer(Game* game)
{
    MCTS searchTree(game);
}

void RandomPlayer::getNextStep(Step* nextStep)
{
    if(this->legalMoveSize == 0)
    {
        nextStep->pos = -BOARD_SIZE - 1;
        return;
    }
    nextStep->player = playerColor;
    int chosen = this->dist(this->randNum) % (int)this->legalMoveSize;
    nextStep->pos = this->legalMove[chosen];
}

void RandomPlayer::updatePlayer(Game* globalGame)
{
    this->legalMoveSize = 0;
    globalGame->legalMove(legalMove, qiAfterMove, this->legalMoveSize);
}

RandomPlayer::~RandomPlayer()
= default;
