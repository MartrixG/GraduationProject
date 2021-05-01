//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC_GOBLOCK_HPP
#define MARTRIXGOC_GOBLOCK_HPP

#include <unordered_set >
#include <iostream>
#include "Point.hpp"

class GoBlock
{
public:
    std::unordered_set<Point*> points = std::unordered_set<Point*>();
    std::unordered_set<Point*> qiPoints = std::unordered_set<Point*>();
    int color = -1;

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
        int board[19][19];
        for(auto & i : board)
        {
            for(int & j : i)
            {
                j = 0;
            }
        }
        for(auto &point : o.points)
        {
            board[point->x][point->y] = 1;
        }
        std::cout << "point\n" << " ";
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
                switch (board[i][j])
                {
                    case 0:
                        std::cout << ". ";
                        break;
                    case 1:
                        std::cout << "x ";
                        board[i][j] = 0;
                        break;
                }
            }
            std::cout << '\n';
        }
        for(auto &point : o.qiPoints)
        {
            board[point->x][point->y] = 2;
        }
        std::cout << "qi\n" << " ";
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
                switch (board[i][j])
                {
                    case 0:
                        std::cout << ". ";
                        break;
                    case 2:
                        std::cout << "x ";
                        board[i][j] = 0;
                        break;
                }
            }
            std::cout << '\n';
        }
    }

    ~GoBlock()
    {
        this->points.clear();
        this->qiPoints.clear();
    }
};

#endif //MARTRIXGOC_GOBLOCK_HPP
