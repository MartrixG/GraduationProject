//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC___POINT_H
#define MARTRIXGOC___POINT_H

#include "vector"
#include "ostream"

#define vector_2d(type) std::vector<std::vector<type>>
const int BOARD_SIZE = 19;

class Point
{
public:
    int x, y, boardSize;
    long long zobristHash[2]{};

    Point(int x, int y, int boardSize, long long blackHash, long long whiteHash);

    static void getAround(Point* nowPoint, const vector_2d(Point*) &allBoardPoints, Point** aroundPoints, size_t &aroundSize);

    static void
    getDiagonal(Point* nowPoint, const vector_2d(Point*) &allBoardPoints, Point** diagonalPoints, size_t& diagonalSize);

    static void pointsInit(vector_2d(Point*) &allBoardPoints);

    int getPos() const;

    friend std::ostream &operator<<(std::ostream &out, const Point &o)
    {
        out << "x: " << o.x << " y: " << o.y;
        return out;
    }

    static void test();
};

#endif //MARTRIXGOC___POINT_H