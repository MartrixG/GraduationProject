//
// Created by 11409 on 2021/4/17.
#include <set>
#include <queue>
#include "GoBlock.hpp"

#include <cassert>

bool GoBlock::check(int x, int y) const
{
    return x >= 0 && x < this->beginPoint->boardSize && y >= 0 && y < this->beginPoint->boardSize;
}

GoBlock::GoBlock(Point *beginPoint,const vector_2d(int) &board, const vector_2d(Point*) &allBoardPoints)
{
    this->beginPoint = beginPoint;
    this->color = board[beginPoint->x][beginPoint->y];
    this->findLinkedBlock(board, allBoardPoints);
}

void GoBlock::update(Point *newBeginPoint, const vector_2d(int) &board, const vector_2d(Point*) &allBoardPoints)
{
    this->beginPoint = newBeginPoint;
    this->color = board[newBeginPoint->x][newBeginPoint->y];
    this->qi = -1;
    this->findLinkedBlock(board, allBoardPoints);
}

void GoBlock::findLinkedBlock(const vector_2d(int) &board, const vector_2d(Point*) &allBoardPoints)
{
    this->pieces.clear();
    this->qiPoint.clear();
    std::queue<Point *> Q;
    Q.push(this->beginPoint);
    this->pieces.insert(this->beginPoint);
    int dx[4] = {0, 0, -1, 1};
    int dy[4] = {-1, 1, 0, 0};
    while (!Q.empty())
    {
        Point *nowPoint = Q.front();
        Q.pop();
        for (int i = 0; i < 4; i++)
        {
            int newX = nowPoint->x + dx[i];
            int newY = nowPoint->y + dy[i];
            if (this->check(newX, newY) && board[newX][newY] == this->color)
            {
                Point *newPoint = allBoardPoints[newX][newY];
                if (this->pieces.find(newPoint) == this->pieces.end())
                {
                    Q.push(newPoint);
                    this->pieces.insert(newPoint);
                }
            }
        }
    }
}

int GoBlock::getQi(const vector_2d(int) &board, const vector_2d(Point*) &allBoardPoints)
{
    for (auto &point : this->pieces)
    {
        std::vector<Point *> around;
        Point::getAround(point, allBoardPoints, around);
        for (auto &aroundPoint : around)
        {
            int newX = aroundPoint->x;
            int newY = aroundPoint->y;
            if (board[newX][newY] == 0)
            {
                Point *nowQi = allBoardPoints[newX][newY];
                if (this->qiPoint.find(nowQi) == this->qiPoint.end())
                {
                    this->qiPoint.insert(nowQi);
                }
            }
        }
    }
    this->qi = int(this->qiPoint.size());
    return this->qi;
}

bool GoBlock::contain(Point *point) const
{
    return this->pieces.count(point) == 1;
}

void GoBlock::test()
{
    /*
     *   0 1 2 3 4 5
     * 0 o o y o o o
     * 1 o x y y y o
     * 2 x x y x o o
     * 3 o o x x o o
     * 4 o x o x o o
     * 5 o o o o o o
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
    testBoard[0][2] = 2;
    testBoard[1][2] = 2;
    testBoard[1][3] = 2;
    testBoard[1][4] = 2;
    testBoard[2][2] = 2;
    testBoard[1][1] = 1;
    testBoard[2][0] = 1;
    testBoard[2][1] = 1;
    testBoard[2][3] = 1;
    testBoard[3][2] = 1;
    testBoard[3][3] = 1;
    testBoard[4][3] = 1;
    testBoard[4][1] = 1;
    //white 1
    GoBlock w1(allBoardPoints[0][2], testBoard, allBoardPoints);
    auto blockPieces = w1.pieces;
    std::set<Point *> w1Block(blockPieces.begin(), blockPieces.end());
    assert(w1Block.count(allBoardPoints[0][2]) == 1);
    assert(w1Block.count(allBoardPoints[1][2]) == 1);
    assert(w1Block.count(allBoardPoints[1][3]) == 1);
    assert(w1Block.count(allBoardPoints[1][4]) == 1);
    assert(w1Block.count(allBoardPoints[2][2]) == 1);
    assert(w1.getQi(testBoard, allBoardPoints) == 5);
    //black 1
    GoBlock b1(allBoardPoints[1][1], testBoard, allBoardPoints);
    blockPieces = b1.pieces;
    std::set<Point *> b1Block(blockPieces.begin(), blockPieces.end());
    assert(b1Block.count(allBoardPoints[1][1]) == 1);
    assert(b1Block.count(allBoardPoints[2][0]) == 1);
    assert(b1Block.count(allBoardPoints[2][1]) == 1);
    assert(b1.getQi(testBoard, allBoardPoints) == 4);
    //black 2
    GoBlock b2(allBoardPoints[3][3], testBoard, allBoardPoints);
    blockPieces = b2.pieces;
    std::set<Point *> b2Block(blockPieces.begin(), blockPieces.end());
    assert(b2Block.count(allBoardPoints[2][3]) == 1);
    assert(b2Block.count(allBoardPoints[3][2]) == 1);
    assert(b2Block.count(allBoardPoints[3][3]) == 1);
    assert(b2Block.count(allBoardPoints[4][3]) == 1);
    assert(b2.getQi(testBoard, allBoardPoints) == 6);
    //black 3
    GoBlock b3(allBoardPoints[4][1], testBoard, allBoardPoints);
    blockPieces = b3.pieces;
    std::set<Point *> b3Block(blockPieces.begin(), blockPieces.end());
    assert(b3Block.count(allBoardPoints[4][1]) == 1);
    assert(b3.getQi(testBoard, allBoardPoints) == 4);
}
//

