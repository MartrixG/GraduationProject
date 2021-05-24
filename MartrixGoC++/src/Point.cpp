//
// Created by 11409 on 2021/4/17.
//

#include <random>
#include <chrono>
#include "Point.hpp"
#include <cassert>

Point::Point(int pos, int64_t blackHash, int64_t whiteHash)
{
    this->pos = pos;
    this->zobristHash[0] = blackHash;
    this->zobristHash[1] = whiteHash;
}

void Point::getAround(int nowPoint, PArVecPtr aroundPoints)
{
    int dx[4] = {0, 0, -1, 1};
    int dy[4] = {-1, 1, 0, 0};
    for (size_t i = 0; i < 4; i++)
    {
        int newX = nowPoint / BOARD_SIZE + dx[i];
        int newY = nowPoint % BOARD_SIZE + dy[i];
        if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE)
        {
            aroundPoints->push_back(newX * BOARD_SIZE + newY);
        }
    }
}

void Point::getDiagonal(int nowPoint, PDiVecPtr diagonalPoints)
{
    int dx[4] = {-1, -1, 1, 1};
    int dy[4] = {-1, 1, -1, 1};
    for (size_t i = 0; i < 4; i++)
    {
        int newX = nowPoint / BOARD_SIZE + dx[i];
        int newY = nowPoint % BOARD_SIZE + dy[i];
        if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE)
        {
            diagonalPoints->push_back(newX * BOARD_SIZE + newY);
        }
    }
}

void Point::pointsInit(PointPtr* allBoardPoints, PArVecPtr* allAround, PDiVecPtr* allDiagonal)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 rand_num(seed);
    std::uniform_int_distribution<int64_t> dist(0, 0x7fffffffffffffff);
    for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        allBoardPoints[i] = new Point(i, dist(rand_num), dist(rand_num));
    }
    for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        auto around = new std::vector<int>();
        auto diagonal = new std::vector<int>();
        getAround(i, around);
        getDiagonal(i ,diagonal);
        allAround[i] = around;
        allDiagonal[i] = diagonal;
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
    PointPtr allBoardPoints[BOARD_SIZE * BOARD_SIZE];
    PArVecPtr allAround[BOARD_SIZE * BOARD_SIZE];
    PDiVecPtr allDiagonal[BOARD_SIZE * BOARD_SIZE];
    Point::pointsInit(allBoardPoints, allAround, allDiagonal);
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        assert(allBoardPoints[i]->pos == i);
    }

    PArVecPtr around1, around2, around3, around4;
    around1 = allAround[0 * BOARD_SIZE + 0];
    around2 = allAround[0 * BOARD_SIZE + 1];
    around3 = allAround[1 * BOARD_SIZE + 0];
    around4 = allAround[1 * BOARD_SIZE + 1];

    PDiVecPtr diagonal1, diagonal2, diagonal3, diagonal4;
    diagonal1 = allDiagonal[0 * BOARD_SIZE + 0];
    diagonal2 = allDiagonal[0 * BOARD_SIZE + 1];
    diagonal3 = allDiagonal[1 * BOARD_SIZE + 0];
    diagonal4 = allDiagonal[1 * BOARD_SIZE + 1];
    //test 00
    assert(0 * BOARD_SIZE + 1 == around1->at(0));
    assert(1 * BOARD_SIZE + 0 == around1->at(1));
    assert(1 * BOARD_SIZE + 1 == diagonal1->at(0));
    //test 01
    assert(0 * BOARD_SIZE + 0 == around2->at(0));
    assert(0 * BOARD_SIZE + 2 == around2->at(1));
    assert(1 * BOARD_SIZE + 1 == around2->at(2));
    assert(1 * BOARD_SIZE + 0 == diagonal2->at(0));
    assert(1 * BOARD_SIZE + 2 == diagonal2->at(1));
    //test 10
    assert(1 * BOARD_SIZE + 1 == around3->at(0));
    assert(0 * BOARD_SIZE + 0 == around3->at(1));
    assert(2 * BOARD_SIZE + 0 == around3->at(2));
    assert(0 * BOARD_SIZE + 1 == diagonal3->at(0));
    assert(2 * BOARD_SIZE + 1 == diagonal3->at(1));
    //test 11
    assert(1 * BOARD_SIZE + 0 == around4->at(0));
    assert(1 * BOARD_SIZE + 2 == around4->at(1));
    assert(0 * BOARD_SIZE + 1 == around4->at(2));
    assert(2 * BOARD_SIZE + 1 == around4->at(3));
    assert(0 * BOARD_SIZE + 0 == diagonal4->at(0));
    assert(0 * BOARD_SIZE + 2 == diagonal4->at(1));
    assert(2 * BOARD_SIZE + 0 == diagonal4->at(2));
    assert(2 * BOARD_SIZE + 2 == diagonal4->at(3));
}