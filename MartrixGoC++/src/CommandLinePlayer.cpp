//
// Created by 11409 on 2021/5/13.
//

#include "CommandLinePlayer.hpp"

CommandLinePlayer::CommandLinePlayer(int color)
{
    this->playerColor = color;
}

void CommandLinePlayer::getNextStep(Step* nextStep) const
{
    int x, y;
    std::cin >> x >> y;
    nextStep->player = playerColor;
    nextStep->pos = (x - 1) * BOARD_SIZE + y - 1;
}