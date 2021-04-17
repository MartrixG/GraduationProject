//
// Created by 11409 on 2021/4/17.
#include <set>
#include <queue>
#include "GoBlock.hpp"

bool GoBlock::check(int x, int y)
{
    return x >= 0 && x <= this->beginPoint->boardSize && y >= 0 && y <= this->beginPoint->boardSize;
}

GoBlock::GoBlock(Point &beginPoint, vector_2d(int) &board, vector_2d(Point) &points)
{
    this->pieces = std::set<Point*>();
    this->qiPoint = std::set<Point*>();
    this->board = board;
    this->beginPoint = &beginPoint;
    this->color = board[beginPoint.x][beginPoint.y];
    this->qi = -1;
    this->findLinkedBlock(points);
}

GoBlock::GoBlock()= default;

void GoBlock::findLinkedBlock(vector_2d(Point) &points)
{
    std::queue<Point*> Q;
    Q.push(this->beginPoint);
    this->pieces.insert(this->beginPoint);
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    while(!Q.empty())
    {
        Point* nowPoint = Q.front();
        for(int i = 0; i < 4; i++)
        {
            int newX = nowPoint->x + dx[i];
            int newY = nowPoint->y + dy[i];
            if(this->check(newX, newY) && this->board[newX][newY] == this->color)
            {
                Point* newPoint = &points[newX][newY];
                if(this->pieces.find(newPoint) == this->pieces.end())
                {
                    Q.push(newPoint);
                    this->pieces.insert(newPoint);
                }
            }
        }
    }
}

int GoBlock::getQi(vector_2d(Point) &points)
{
    for(auto point : this->pieces)
    {
        auto around = Point::getAround(*point, points);
        for(auto aroundPoint : around)
        {
            int newX = aroundPoint->x;
            int newY = aroundPoint->y;
            if(this->board[newX][newY] == 0)
            {
                Point* nowQi = &points[newX][newY];
                if(this->qiPoint.find(nowQi) == this->qiPoint.end())
                {
                    this->qiPoint.insert(nowQi);
                }
            }
        }
    }
    this->qi = int(this->qiPoint.size());
    return this->qi;
}

//

