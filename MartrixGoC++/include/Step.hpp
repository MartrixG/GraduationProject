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
    using PointPtr = Point::PointPtr;

    int player, pos;

    explicit Step(char player, char x, char y);

    Step(int player, PointPtr point);

    Step(int x, int y, int player);

    std::string toString() const;

    friend std::ostream &operator<<(std::ostream &out, const Step &o)
    {
        out << (o.player == BLACK_PLAYER ? "B " : "W ");
        out << char('a' + o.pos / BOARD_SIZE) << char('a' + o.pos % BOARD_SIZE);
        return out;
    }

    bool operator==(const Step& o) const;
};

#endif //MARTRIXGOC_STEP_HPP
