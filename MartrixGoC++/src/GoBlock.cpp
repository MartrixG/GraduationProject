//
// Created by 11409 on 2021/4/17.
//

#include "GoBlock.hpp"

GoBlock::GoBlock() = default;

void GoBlock::update(Point* beginPoint, int pointColor, const std::vector<Point*> &aroundPoint,
                     const std::vector<std::vector<int>> &board)
{
    this->color = pointColor;
    this->points.insert(beginPoint);
    for(auto &point : aroundPoint)
    {
        if(board[point->x][point->y] == 0)
        {
            this->qiPoints.insert(point);
        }
    }
}

void GoBlock::update(GoBlock* otherBlock)
{
    this->points.insert(otherBlock->points.begin(), otherBlock->points.end());
    this->qiPoints.insert(otherBlock->qiPoints.begin(), otherBlock->qiPoints.end());
    this->color = otherBlock->color;
}

void GoBlock::merge(Point* linkPointSelf, GoBlock* otherBlock)
{
    this->points.insert(otherBlock->points.begin(), otherBlock->points.end());
    this->qiPoints.insert(otherBlock->qiPoints.begin(), otherBlock->qiPoints.end());
    this->qiPoints.erase(linkPointSelf);
}

void GoBlock::addPoint(Point* linkPoint, const vector_2d(int) &board, const vector_2d(Point*) &allBoardPoints)
{
    this->points.insert(linkPoint);
    this->qiPoints.erase(linkPoint);
    int dx[4] = {0, 0, -1, 1};
    int dy[4] = {-1, 1, 0, 0};
    for(int i = 0; i < 4; i++)
    {
        int newX = linkPoint->x + dx[i];
        int newY = linkPoint->y + dy[i];
        if(newX >= 0 && newX < linkPoint->boardSize && newY >= 0 && newY < linkPoint->boardSize)
        {
            if(board[newX][newY] == 0)
            {
                this->addQi(allBoardPoints[newX][newY]);
            }
        }
    }
}

void GoBlock::removeQi(Point* targetPoint)
{
    this->qiPoints.erase(targetPoint);;
}

void GoBlock::addQi(Point* targetPoint)
{
    this->qiPoints.insert(targetPoint);
}

int GoBlock::getQi() const
{
    return int(this->qiPoints.size());
}

int GoBlock::getSize() const
{
    return int(this->points.size());
}

void GoBlock::clear()
{
    this->points.clear();
    this->qiPoints.clear();
    this->color = -1;
}

void GoBlock::test()
{
    /*
     * dx[4] = {0, 0, -1, 1}
     * dy[4] = {-1, 1, 0, 0}
     */
    vector_2d(Point*) allBoardPoints(19);
    Point::pointsInit(allBoardPoints);
    vector_2d(int) testBoard(19);
    for (auto &line :testBoard)
    {
        for (int i = 0; i < 19; i++)
        {
            line.push_back(0);
        }
    }

    std::vector<Point*> around;

    testBoard[3][2] = 1;
    around.clear();
    Point::getAround(allBoardPoints[3][2], allBoardPoints, around);
    GoBlock block1;
    block1.update(allBoardPoints[3][2], 1, around, testBoard);
    std::cout << block1;
    std::cout << '\n';

    testBoard[3][3] = 1;
    block1.addPoint(allBoardPoints[3][3], testBoard, allBoardPoints);
    std::cout << block1;
    std::cout << '\n';

    testBoard[3][5] = 1;
    around.clear();
    Point::getAround(allBoardPoints[3][5], allBoardPoints, around);
    GoBlock block2;
    block2.update(allBoardPoints[3][5], 1, around, testBoard);
    std::cout << block2;
    std::cout << '\n';

    testBoard[3][6] = 1;
    block2.addPoint(allBoardPoints[3][6], testBoard, allBoardPoints);
    std::cout << block2;
    std::cout << '\n';

    testBoard[2][4] = 1;
    around.clear();
    Point::getAround(allBoardPoints[2][4], allBoardPoints, around);
    GoBlock block3;
    block3.update(allBoardPoints[2][4], 1, around, testBoard);
    std::cout << block3;
    std::cout << '\n';
    testBoard[1][4] = 1;
    block3.addPoint(allBoardPoints[1][4], testBoard, allBoardPoints);
    std::cout << block3;
    std::cout << '\n';

    block3.addPoint(allBoardPoints[3][4], testBoard, allBoardPoints);
    std::cout << block3;
    std::cout << '\n';
    block3.merge(allBoardPoints[3][4], &block2);
    std::cout << block3;
    std::cout << '\n';
    block3.merge(allBoardPoints[3][4], &block1);
    std::cout << block3;
    std::cout << '\n';
}