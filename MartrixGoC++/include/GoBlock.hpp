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
private:
    bool check(int x, int y);
public:
    std::set<Point*> pieces;
    std::set<Point*> qiPoint;
    std::vector<std::vector<int>> board;
    Point* beginPoint;
    int color;
    int qi;
    GoBlock(Point& beginPoint, vector_2d(int) &board, vector_2d(Point) &points);
    GoBlock();
    void findLinkedBlock(vector_2d(Point) &points);
    int getQi(vector_2d(Point) &points);
};

#endif //MARTRIXGOC_GOBLOCK_HPP
