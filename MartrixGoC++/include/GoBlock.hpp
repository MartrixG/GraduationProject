//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC_GOBLOCK_HPP
#define MARTRIXGOC_GOBLOCK_HPP

#include <set>
#include <vector>
#include "Point.hpp"

class GoBlock {
public:
    std::set<Point *> pieces = std::set<Point *>();
    std::set<Point *> qiPoint = std::set<Point *>();
    std::vector<std::vector<int>> board;
    Point *beginPoint = nullptr;
    int color = -1;
    int qi = -1;

    bool check(int x, int y);

    GoBlock(Point *beginPoint, vector_2d(int) &board, vector_2d(Point*) &allBoardPoints);

    void findLinkedBlock(vector_2d(Point*) &allBoardPoints);

    int getQi(vector_2d(Point*) &allBoardPoints);

    bool contain(Point *point);

    static void test();
};

#endif //MARTRIXGOC_GOBLOCK_HPP
