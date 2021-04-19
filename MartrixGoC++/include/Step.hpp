//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC_STEP_HPP
#define MARTRIXGOC_STEP_HPP

#include <string>
#include <ostream>
#include "Point.hpp"

class Step {
public:
    int x, y, player;

    explicit Step(std::string srcString);

    Step(int player, Point *point);

    Step(int x, int y, int player);

    friend std::ostream &operator<<(std::ostream &out, const Step &o) {
        out << o.x << " " << o.y;
        return out;
    }
};

#endif //MARTRIXGOC_STEP_HPP
