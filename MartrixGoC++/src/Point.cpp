//
// Created by 11409 on 2021/4/17.
//
#include <vector>
#include "Point.hpp"
#include "Game.hpp"

#include <cassert>

Point::Point(int x, int y, int boardSize)
{
    this->x = x;
    this->y = y;
    this->boardSize = boardSize;
}

void Point::getAround(Point *nowPoint, const vector_2d(Point*) &allBoardPoints, std::vector<Point *> &aroundPoints)
{
    int dx[4] = {0, 0, -1, 1};
    int dy[4] = {-1, 1, 0, 0};
    for (int i = 0; i < 4; i++)
    {
        int newX = nowPoint->x + dx[i];
        int newY = nowPoint->y + dy[i];
        if (newX >= 0 && newX < nowPoint->boardSize && newY >= 0 && newY < nowPoint->boardSize)
        {
            aroundPoints.push_back(allBoardPoints[newX][newY]);
        }
    }
}

void Point::pointsInit(vector_2d(Point*) &allBoardPoints)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        std::vector<Point> tmp;
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            allBoardPoints[i].push_back(new Point(i, j, BOARD_SIZE));
        }
    }
}

void Point::test()
{
    /*   0 1 2
     * 0 x x x
     * 1 x x x
     * 2 x x x
     * dx[4] = {0, 0, -1, 1}
     * dy[4] = {-1, 1, 0, 0}
     */
    vector_2d(Point*) allBoardPoints(BOARD_SIZE);
    Point::pointsInit(allBoardPoints);
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 19; j++)
        {
            assert(allBoardPoints[i][j]->x == i);
            assert(allBoardPoints[i][j]->y == j);
        }
    }
    Point *testMatrix[3][3];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            testMatrix[i][j] = allBoardPoints[i][j];
        }
    }
    std::vector<Point *> around1, around2, around3, around4;
    Point::getAround(testMatrix[0][0], allBoardPoints, around1);
    Point::getAround(testMatrix[0][1], allBoardPoints, around2);
    Point::getAround(testMatrix[1][0], allBoardPoints, around3);
    Point::getAround(testMatrix[1][1], allBoardPoints, around4);
    //test 00
    assert(testMatrix[0][1] == around1[0]);
    assert(testMatrix[1][0] == around1[1]);
    //test 01
    assert(testMatrix[0][0] == around2[0]);
    assert(testMatrix[0][2] == around2[1]);
    assert(testMatrix[1][1] == around2[2]);
    //test 10
    assert(testMatrix[1][1] == around3[0]);
    assert(testMatrix[0][0] == around3[1]);
    assert(testMatrix[2][0] == around3[2]);
    //test 11
    assert(testMatrix[1][0] == around4[0]);
    assert(testMatrix[1][2] == around4[1]);
    assert(testMatrix[0][1] == around4[2]);
    assert(testMatrix[2][1] == around4[3]);
}