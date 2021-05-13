//
// Created by 11409 on 2021/5/13.
//

#include "SocketPlayer.hpp"

SocketPlayer::SocketPlayer(int color)
{
    this->playerColor = color;
}

void SocketPlayer::updatePlayer(char* message)
{
    this->srcMessage = message;
}

void SocketPlayer::getNextStep(Step* nextStep) const
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
