//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC_GOBLOCK_HPP
#define MARTRIXGOC_GOBLOCK_HPP

#include <unordered_set >
#include <iostream>
#include <bitset>
#include "Point.hpp"

class GoBlock
{
public:
    std::bitset<361> points = std::bitset<361>();
    std::bitset<361> qiPoints = std::bitset<361>();
    int color = -1;
    long long zobristHash = 0LL;

    GoBlock();

    void update(GoBlock* otherBlock);

    void update(Point* beginPoint, int pointColor, const std::vector<Point*> &aroundPoint, const vector_2d(int)& board);

    void merge(Point* linkPointSelf, GoBlock* otherBlock);

    void addPoint(Point* linkPoint, const vector_2d(int)& board, const vector_2d(Point*) &allBoardPoints);

    void removeQi(Point* targetPoint);

    void addQi(Point* targetPoint);

    int getQi() const;

    int getSize() const;

    void clear();

    static void test();

    friend void operator<<(std::ostream &out, const GoBlock &o)
    {
        std::cout << "point and qi\n" << " ";
        for (int i = 0; i < 19; i++)
        {
            std::cout << " " << char('a' + i);
        }
        std::cout << std::endl;
        for (int i = 0; i < 19; i++)
        {
            std::cout << char('a' + i) << " ";
            for (int j = 0; j < 19; j++)
            {
                if(o.points.test(i * 19 + j))
                {
                    std::cout << "x ";
                }
                else if(o.qiPoints.test(i * 19 + j))
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
