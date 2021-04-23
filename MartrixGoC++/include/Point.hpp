//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC___POINT_H
#define MARTRIXGOC___POINT_H

#include "vector"
#include "ostream"

#define vector_2d(type) std::vector<std::vector<type>>

class Point
{
public:
    int x, y, boardSize;

    Point(int x, int y, int boardSize);

    static void getAround(Point *nowPoint, const vector_2d(Point*) &allBoardPoints, std::vector<Point *> &aroundPoints);

    static void getDiagonal(Point *nowPoint, const vector_2d(Point*) &allBoardPoints, std::vector<Point *> &diagonalPoints);

    static void pointsInit(vector_2d(Point*) &allBoardPoints);

    friend std::ostream &operator<<(std::ostream &out, const Point &o)
    {
        out << "x: " << o.x << " y: " << o.y;
        return out;
    }

    static void test();
};

#endif //MARTRIXGOC___POINT_H