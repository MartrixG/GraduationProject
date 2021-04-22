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
}

Step::Step(int player, Point *point)
{
    this->player = player;
    this->x = point->x;
    this->y = point->y;
}

Step::Step(int x, int y, int player)
{
    this->player = player;
    this->x = x;
    this->y = y;
}