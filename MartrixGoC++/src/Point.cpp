//
// Created by 11409 on 2021/4/17.
//
#include <vector>
#include "Point.hpp"
#include "Game.hpp"

Point::Point(int x, int y, int boardSize)
{
    this->x = x;
    this->y = y;
    this->boardSize = boardSize;
}

std::vector<Point *> Point::getAround(Point &nowPoint, vector_2d(Point) &points)
{
    std::vector<Point *> reAround;
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    for(int i = 0; i < 4; i++)
    {
        int newX = nowPoint.x + dx[i];
        int newY = nowPoint.y + dy[i];
        if(newX >= 0 && newX < nowPoint.boardSize && newY >= 0 && newY < nowPoint.boardSize)
        {
            reAround.push_back(&points[newX][newY]);
        }
    }
    return reAround;
}

void Point::pointsInit(vector_2d(Point) &points)
{
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        std::vector<Point> tmp;
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            points[i].push_back(Point(i, j, BOARD_SIZE));
        }
    }
}