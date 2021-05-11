//
// Created by 11409 on 2021/4/17.
//

#include "Step.hpp"

Step::Step(char player, char x, char y)
{
    this->player = player == 'B' ? BLACK_PLAYER : WHITE_PLAYER;
    char strX = x;
    char strY = y;
    if (strX >= 'a' && strX <= 't' && strY >= 'a' && strY <= 't')
    {
        this->x = x - 'a';
        this->y = y - 'a';
    }
    this->pos = this->x * BOARD_SIZE + this->y;
}

Step::Step(int player, PointPtr point)
{
    this->player = player;
    this->pos = point->pos;
    this->x = point->pos / BOARD_SIZE;
    this->y = point->pos % BOARD_SIZE;
}

Step::Step(int x, int y, int player)
{
    this->player = player;
    this->x = x;
    this->y = y;
    this->pos = x * BOARD_SIZE + y;
}

bool Step::operator==(const Step &o) const
{
    return this->pos == o.pos;
}