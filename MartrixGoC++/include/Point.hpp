//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC___POINT_H
#define MARTRIXGOC___POINT_H

#include "vector"
#include "ostream"

const int BOARD_SIZE = 9;

class Point
{
public:
    using PointPtr = Point*;
    using PArVecPtr = std::vector<int>*;
    using PDiVecPtr = std::vector<int>*;

    int pos;
    long long zobristHash[2]{};

    Point(int pos, long long blackHash, long long whiteHash);

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