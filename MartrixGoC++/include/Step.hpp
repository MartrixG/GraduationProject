//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC_STEP_HPP
#define MARTRIXGOC_STEP_HPP

#include <string>
#include <ostream>
#include "Point.hpp"

const int BLACK_PLAYER = 1;
const int WHITE_PLAYER = 2;

class Step
{
public:
    int x, y, player;

    explicit Step(char player, char x, char y);

    Step(int player, Point *point);

    Step(int x, int y, int player);

    friend std::ostream &operator<<(std::ostream &out, const Step &o)
    {
        out << o.x << " " << o.y << std::endl;
        return out;
    }
};

#endif //MARTRIXGOC_STEP_HPP
