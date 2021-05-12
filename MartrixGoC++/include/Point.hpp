//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC___POINT_H
#define MARTRIXGOC___POINT_H

#include "vector"
#include "ostream"

#define vector_2d(T) std::vector<std::vector<T>>

const int BOARD_SIZE = 19;

class Point
{
public:
    using PointPtr = Point*;
    using PArVecPtr = std::vector<PointPtr>*;
    using PDiVecPtr = std::vector<PointPtr>*;

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
    static void getAround(PointPtr nowPoint, PointPtr* allBoardPoints, PArVecPtr aroundPoints);

    static void getDiagonal(Point* nowPoint, PointPtr* allBoardPoints, PDiVecPtr diagonalPoints);
};

#endif //MARTRIXGOC___POINT_H