//
// Created by 11409 on 2021/5/5.
//

#include <iostream>
#include <cstring>
#include "Player.hpp"

PlayerBase::PlayerBase(playerEnum type, playColorEnum color)
{
    this->playerType = type;
    this->playerColor = color;
    this->randNum = std::default_random_engine(GetCurrentThreadId());
    this->dist = std::uniform_int_distribution<int>(0, 0x7fffffff);
}

void CommandLinePlayer::getNextStep(Step* nextStep)
{
    int x, y;
    std::cin >> x >> y;
    nextStep->x = x - 1;
    nextStep->y = y - 1;
    nextStep->player = (int)playerColor + 1;
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
    nextStep->player = (int)this->playerColor + 1;
    nextStep->x = pos / BOARD_SIZE;
    nextStep->y = pos % BOARD_SIZE;
}

void SocketPlayer::updatePlayer(char* message)
{
    this->srcMessage = message;
}

void MCTSPlayer::getFirstStep(Step* nextStep)
{
    nextStep->player = (int)playerColor + 1;
    int dx[5] = {0, 1, -1, 0, 0};
    int dy[5] = {0, 0, 0, 1, -1};
    int i = this->dist(this->randNum) % 5;
    nextStep->x = dx[i];
    nextStep->y = dy[i];
}

void MCTSPlayer::getNextStep(Step* nextStep)
{
    nextStep->player = (int)playerColor + 1;
    nextStep->x = this->dist(this->randNum) % BOARD_SIZE;
    nextStep->y = this->dist(this->randNum) % BOARD_SIZE;
}

void MCTSPlayer::updatePlayer()
{

}

void RandomPlayer::getNextStep(Step* nextStep)
{
    if(this->legalMoveSize == 0)
    {
        nextStep->x = -1;
        nextStep->y = -1;
        return;
    }
    nextStep->player = (int)playerColor + 1;
    int chosen = this->dist(this->randNum) % (int)this->legalMoveSize;
    nextStep->x = this->legalMove[chosen] / BOARD_SIZE;
    nextStep->y = this->legalMove[chosen] % BOARD_SIZE;
}

void RandomPlayer::updatePlayer(Game* globalGame)
{
    this->legalMoveSize = 0;
    this->game = globalGame;
    this->game->legalMove(legalMove, qiAfterMove, this->legalMoveSize);
}
