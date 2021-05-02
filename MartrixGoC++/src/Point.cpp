//
// Created by 11409 on 2021/4/17.
//

#include <random>
#include <chrono>
#include "Point.hpp"
#include "Game.hpp"
#include <cassert>

Point::Point(int x, int y, int boardSize, long long blackHash, long long whiteHash)
{
    this->x = x;
    this->y = y;
    this->boardSize = boardSize;
    this->zobristHash[0] = blackHash;
    this->zobristHash[1] = whiteHash;
}

void Point::getAround(Point* nowPoint, const vector_2d(Point*) &allBoardPoints, std::vector<Point*> &aroundPoints)
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

void Point::getDiagonal(Point* nowPoint, const vector_2d(Point*) &allBoardPoints, std::vector<Point*> &diagonalPoints)
{
    int dx[4] = {-1, -1, 1, 1};
    int dy[4] = {-1, 1, -1, 1};
    for (int i = 0; i < 4; i++)
    {
        int newX = nowPoint->x + dx[i];
        int newY = nowPoint->y + dy[i];
        if (newX >= 0 && newX < nowPoint->boardSize && newY >= 0 && newY < nowPoint->boardSize)
        {
            diagonalPoints.push_back(allBoardPoints[newX][newY]);
        }
    }
}

void Point::pointsInit(vector_2d(Point*) &allBoardPoints)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 rand_num(seed);
    std::uniform_int_distribution<long long> dist(0, 0x7fffffffffffffff);
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        std::vector<Point> tmp;
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            allBoardPoints[i].push_back(new Point(i, j, BOARD_SIZE, dist(rand_num), dist(rand_num)));
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
    Point* testMatrix[3][3];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            testMatrix[i][j] = allBoardPoints[i][j];
        }
    }
    std::vector<Point*> around1, around2, around3, around4;
    Point::getAround(testMatrix[0][0], allBoardPoints, around1);
    Point::getAround(testMatrix[0][1], allBoardPoints, around2);
    Point::getAround(testMatrix[1][0], allBoardPoints, around3);
    Point::getAround(testMatrix[1][1], allBoardPoints, around4);

    std::vector<Point*> diagonal1, diagonal2, diagonal3, diagonal4;
    Point::getDiagonal(testMatrix[0][0], allBoardPoints, diagonal1);
    Point::getDiagonal(testMatrix[0][1], allBoardPoints, diagonal2);
    Point::getDiagonal(testMatrix[1][0], allBoardPoints, diagonal3);
    Point::getDiagonal(testMatrix[1][1], allBoardPoints, diagonal4);
    //test 00
    assert(testMatrix[0][1] == around1[0]);
    assert(testMatrix[1][0] == around1[1]);
    assert(testMatrix[1][1] == diagonal1[0]);
    //test 01
    assert(testMatrix[0][0] == around2[0]);
    assert(testMatrix[0][2] == around2[1]);
    assert(testMatrix[1][1] == around2[2]);
    assert(testMatrix[1][0] == diagonal2[0]);
    assert(testMatrix[1][2] == diagonal2[1]);
    //test 10
    assert(testMatrix[1][1] == around3[0]);
    assert(testMatrix[0][0] == around3[1]);
    assert(testMatrix[2][0] == around3[2]);
    assert(testMatrix[0][1] == diagonal3[0]);
    assert(testMatrix[2][1] == diagonal3[1]);
    //test 11
    assert(testMatrix[1][0] == around4[0]);
    assert(testMatrix[1][2] == around4[1]);
    assert(testMatrix[0][1] == around4[2]);
    assert(testMatrix[2][1] == around4[3]);
    assert(testMatrix[0][0] == diagonal4[0]);
    assert(testMatrix[0][2] == diagonal4[1]);
    assert(testMatrix[2][0] == diagonal4[2]);
    assert(testMatrix[2][2] == diagonal4[3]);
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            std::cout << allBoardPoints[i][j]->zobristHash << '\n';
        }
    }
}