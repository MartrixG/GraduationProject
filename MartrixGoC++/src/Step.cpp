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
        this->pos = (x - 'a') * BOARD_SIZE + y - 'a';
    }
}

Step::Step(int player, PointPtr point)
{
    this->player = player;
    this->pos = point->pos;
}

Step::Step(int x, int y, int player)
{
    this->player = player;
    this->pos = x * BOARD_SIZE + y;
}

bool Step::operator==(const Step &o) const
{
    return this->pos == o.pos;
}

std::string Step::toString() const
{
    std::string res = this->player == BLACK_PLAYER ? "B " : "W ";
    res += char('a' + this->pos / BOARD_SIZE);
    res += char('a' + this->pos % BOARD_SIZE);
    return res;
}
