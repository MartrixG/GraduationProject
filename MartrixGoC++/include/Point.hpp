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
    static std::vector<Point *> getAround(Point &nowPoint, vector_2d(Point) &points);
    static void pointsInit(vector_2d(Point) &points);
    friend std::ostream& operator<<(std::ostream& out, const Point& o) {
        out << "x: " << o.x << " y: " << o.y;
        return out;
    }
};

#endif //MARTRIXGOC___POINT_H