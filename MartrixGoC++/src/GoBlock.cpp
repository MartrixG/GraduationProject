//
// Created by 11409 on 2021/4/17.
//

#include "GoBlock.hpp"

GoBlock::GoBlock() = default;

void GoBlock::update(PointPtr beginPoint, int pointColor, PArVecPtr around, const int* board)
{
    this->color = pointColor;
    this->points.set(beginPoint->pos);
    this->zobristHash = beginPoint->zobristHash[pointColor - 1];
    for(auto point : *around)
    {
        if(board[point->pos] == 0)
        {
            this->qiPoints.set(point->pos);
        }
    }
}

void GoBlock::update(BlockPtr otherBlock)
{
    this->points |= otherBlock->points;
    this->zobristHash |= otherBlock->zobristHash;
    this->qiPoints |= otherBlock->qiPoints;
    this->color = otherBlock->color;
}

void GoBlock::merge(PointPtr linkPointSelf, BlockPtr otherBlock)
{
    this->points |= otherBlock->points;
    this->zobristHash |= otherBlock->zobristHash;
    this->qiPoints |= otherBlock->qiPoints;
    this->qiPoints.reset(linkPointSelf->pos);
}

void GoBlock::addPoint(PointPtr linkPoint, PArVecPtr around, const int* board)
{
    this->points.set(linkPoint->pos);
    this->zobristHash |= linkPoint->zobristHash[this->color - 1];
    this->qiPoints.reset(linkPoint->pos);
    for(auto &point : *around)
    {
        if(board[point->pos] == 0)
        {
            this->addQi(point->pos);
        }
    }
}

void GoBlock::removeQi(int targetPoint)
{
    this->qiPoints.reset(targetPoint);
}

void GoBlock::addQi(int targetPoint)
{
    this->qiPoints.set(targetPoint);
}

int GoBlock::getQi() const
{
    return (int)this->qiPoints.count();
}

int GoBlock::getSize() const
{
    return (int)this->points.count();
}

void GoBlock::clear()
{
    this->points.reset();
    this->qiPoints.reset();
    this->zobristHash = 0LL;
    this->color = -1;
}

void GoBlock::test()
{
    /*
     * dx[4] = {0, 0, -1, 1}
     * dy[4] = {-1, 1, 0, 0}
     */
    PointPtr allBoardPoints[BOARD_SIZE * BOARD_SIZE];
    PArVecPtr allAround[BOARD_SIZE * BOARD_SIZE];
    PDiVecPtr allDiagonal[BOARD_SIZE * BOARD_SIZE];
    Point::pointsInit(allBoardPoints, allAround, allDiagonal);

    int testBoard[BOARD_SIZE * BOARD_SIZE];
    memset(testBoard, 0, sizeof(testBoard));

    PArVecPtr around;

    testBoard[3 * BOARD_SIZE + 2] = 1;
    around = allAround[3 * BOARD_SIZE + 2];
    GoBlock block1;
    block1.update(allBoardPoints[3 * BOARD_SIZE + 2], 1, around, testBoard);
    std::cout << block1;
    std::cout << '\n';

    testBoard[3 * BOARD_SIZE + 3] = 1;
    around = allAround[3 * BOARD_SIZE + 3];
    block1.addPoint(allBoardPoints[3 * BOARD_SIZE + 3], around, testBoard);
    std::cout << block1;
    std::cout << '\n';

    testBoard[3 * BOARD_SIZE + 5] = 1;
    around = allAround[3 * BOARD_SIZE + 5];
    GoBlock block2;
    block2.update(allBoardPoints[3 * BOARD_SIZE + 5], 1, around, testBoard);
    std::cout << block2;
    std::cout << '\n';

    testBoard[3 * BOARD_SIZE + 6] = 1;
    around = allAround[3 * BOARD_SIZE + 6];
    block2.addPoint(allBoardPoints[3 * BOARD_SIZE + 6], around, testBoard);
    std::cout << block2;
    std::cout << '\n';

    testBoard[2 * BOARD_SIZE + 4] = 1;
    around = allAround[2 * BOARD_SIZE + 4];
    GoBlock block3;
    block3.update(allBoardPoints[2 * BOARD_SIZE + 4], 1, around, testBoard);
    std::cout << block3;
    std::cout << '\n';
    testBoard[1 * BOARD_SIZE + 4] = 1;
    around = allAround[1 * BOARD_SIZE + 4];
    block3.addPoint(allBoardPoints[1 * BOARD_SIZE + 4], around, testBoard);
    std::cout << block3;
    std::cout << '\n';

    around = allAround[3 * BOARD_SIZE + 4];
    block3.addPoint(allBoardPoints[3 * BOARD_SIZE + 4], around, testBoard);
    std::cout << block3;
    std::cout << '\n';
    block3.merge(allBoardPoints[3 * BOARD_SIZE + 4], &block2);
    std::cout << block3;
    std::cout << '\n';
    block3.merge(allBoardPoints[3 * BOARD_SIZE + 4], &block1);
    std::cout << block3;
    std::cout << '\n';
}