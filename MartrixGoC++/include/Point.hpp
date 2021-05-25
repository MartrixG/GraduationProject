//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC___POINT_H
#define MARTRIXGOC___POINT_H

#include "vector"
#include "ostream"

const int BOARD_SIZE = 9;
const double PRI_VALUE[81] =
        {0,    0,    0,    0,    0,    0,    0,    0, 0,
         0, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0,
         0, 0.01, 0.04, 0.01, 0.02, 0.01, 0.04, 0.01, 0,
         0, 0.01, 0.01,    0,    0,    0, 0.01, 0.01, 0,
         0, 0.01, 0.02,    0,    0,    0, 0.02, 0.01, 0,
         0, 0.01, 0.01,    0,    0,    0, 0.01, 0.01, 0,
         0, 0.01, 0.04, 0.01, 0.02, 0.01, 0.04, 0.01, 0,
         0, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0,
         0,    0,    0,    0,    0,    0,    0,    0, 0};

class Point
{
public:
    using PointPtr = Point*;
    using PArVecPtr = std::vector<int>*;
    using PDiVecPtr = std::vector<int>*;

    int pos;
    int64_t zobristHash[2]{};

    Point(int pos, int64_t blackHash, int64_t whiteHash);

    static void pointsInit(PointPtr* allBoardPoints, PArVecPtr* allAround, PDiVecPtr* allDiagonal);

    static void test();

    friend std::ostream &operator<<(std::ostream &out, const Point &o)
    {
        out << "x: " << o.pos / BOARD_SIZE << " y: " << o.pos % BOARD_SIZE;
        return out;
    }
private:
    static void getAround(int nowPoint, PArVecPtr aroundPoints);

    static void getDiagonal(int nowPoint, PDiVecPtr diagonalPoints);
};

#endif //MARTRIXGOC___POINT_H