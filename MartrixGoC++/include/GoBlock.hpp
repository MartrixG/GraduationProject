//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC_GOBLOCK_HPP
#define MARTRIXGOC_GOBLOCK_HPP

#include <set>
#include <vector>
#include "Point.hpp"

class GoBlock
{
public:
    std::set<Point *> pieces = std::set<Point *>();
    std::set<Point *> qiPoint = std::set<Point *>();
    Point *beginPoint = nullptr;
    int color = -1;
    int qi = -1;

    bool check(int x, int y) const;

    GoBlock(Point *beginPoint,const vector_2d(int) &board, const vector_2d(Point*) &allBoardPoints);

    void update(Point * beginPoint, const vector_2d(int) &board, const vector_2d(Point*) &allBoardPoints);

    void findLinkedBlock(const vector_2d(int) &board, const vector_2d(Point*) &allBoardPoints);

    int getQi(const vector_2d(int) &board, const vector_2d(Point*) &allBoardPoints);

    bool contain(Point *point) const;

    static void test();

    ~GoBlock()
    {
        this->pieces.clear();
        this->qiPoint.clear();
    }
};

#endif //MARTRIXGOC_GOBLOCK_HPP
