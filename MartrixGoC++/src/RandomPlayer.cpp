//
// Created by 11409 on 2021/5/13.
//

#include "RandomPlayer.hpp"

RandomPlayer::RandomPlayer(int color)
{
    this->playerColor = color;
    this->legalMove = new int[BOARD_SIZE * BOARD_SIZE];
    this->qiAfterMove = new int[BOARD_SIZE * BOARD_SIZE];
    this->randNum = std::default_random_engine(GetCurrentThreadId() + std::chrono::system_clock::now().time_since_epoch().count());
    this->dist = std::uniform_int_distribution<int>(0, 0x7fffffff);
}

void RandomPlayer::getNextStep(Step* nextStep)
{
    if(this->legalMoveSize == 0)
    {
        nextStep->pos = -1;
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
{
    delete []legalMove;
    delete []qiAfterMove;
}
