//
// Created by 11409 on 2021/4/17.
#include "Step.hpp"

Step::Step(std::string srcString) {
    this->player = srcString[0] == 'B' ? 1 : 2;
    char strX = srcString[3];
    char strY = srcString[2];
    if (strX >= 'a' && strX <= 'z' && strY >= 'a' && strY <= 'z') {
        this->x = x - 'a';
        this->y = y - 'a';
    }
}

Step::Step(int player, Point *point) {
    this->player = player;
    this->x = point->x;
    this->y = point->y;
}

Step::Step(int x, int y, int player) {
    this->player = player;
    this->x = x;
    this->y = y;
}
//

