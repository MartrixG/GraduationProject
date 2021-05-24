//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC_GOBLOCK_HPP
#define MARTRIXGOC_GOBLOCK_HPP

#include <unordered_set >
#include <iostream>
#include <bitset>
#include <cstring>
#include "Point.hpp"

class GoBlock
{
public:
    using BlockPtr = GoBlock*;
    using PointPtr = Point::PointPtr;
    using PArVecPtr = Point::PArVecPtr;
    using PDiVecPtr = Point::PDiVecPtr;

    std::bitset<BOARD_SIZE * BOARD_SIZE> points = std::bitset<BOARD_SIZE * BOARD_SIZE>();
    std::bitset<BOARD_SIZE * BOARD_SIZE> qiPoints = std::bitset<BOARD_SIZE * BOARD_SIZE>();
    int color = -1;
    int64_t zobristHash = 0LL;

    GoBlock();

    void update(BlockPtr otherBlock);

    void update(PointPtr beginPoint, int pointColor, PArVecPtr around, const int* board);

    void merge(PointPtr linkPointSelf, BlockPtr otherBlock);

    void addPoint(PointPtr linkPoint, PArVecPtr around, const int* board);

    void removeQi(int targetPoint);

    void addQi(int targetPoint);

    int getQi() const;

    int getSize() const;

    void clear();

    static void test();

    friend void operator<<(std::ostream &out, const GoBlock &o)
    {
        std::cout << "point and qi\n" << " ";
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            std::cout << " " << char('a' + i);
        }
        std::cout << std::endl;
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            std::cout << char('a' + i) << " ";
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if(o.points.test(i * BOARD_SIZE + j))
                {
                    std::cout << "x ";
                }
                else if(o.qiPoints.test(i * BOARD_SIZE + j))
                {
                    std::cout << "o ";
                }
                else
                {
                    std::cout << ". ";
                }
            }
            std::cout << '\n';
        }
    }
};

#endif //MARTRIXGOC_GOBLOCK_HPP
