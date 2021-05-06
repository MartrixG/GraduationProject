//
// Created by 11409 on 2021/4/17.
//

#include <iostream>
#include <fstream>
#include "Game.hpp"

Game::Game(vector_2d(Point*) &points)
{
    this->allBoardPoints = points;
    this->targetBlock = new GoBlock();
    this->historyZobristHash.insert(0);
}

void Game::initHandCap(std::vector<Step*> &handCapSteps, int numOfHandCap)
{
    this->historyBoard.push_back(this->board);
    for (int i = 0; i < numOfHandCap; i++)
    {
        int x = handCapSteps[i]->x, y = handCapSteps[i]->y;
        this->getPickUpBlock(this->allBoardPoints[x][y]);
        this->board[x][y] = player;
        if(this->mergedBlock.empty())
        {
            this->pointBlockMap[this->allBoardPoints[x][y]] = this->targetBlock;
            this->targetBlock = new GoBlock();
        }
        else
        {
            GoBlock* startBlock = *this->mergedBlock.begin();
            startBlock->clear();
            startBlock->update(this->targetBlock);
            this->pointBlockMap[this->allBoardPoints[x][y]] = startBlock;
            for(auto &block : this->mergedBlock)
            {
                if(block != startBlock)
                {
                    for(int j = 0; j < BOARD_SIZE * BOARD_SIZE; j++)
                    {
                        if(block->points.test(j))
                        {
                            this->pointBlockMap[this->allBoardPoints[j / BOARD_SIZE][j % BOARD_SIZE]] = startBlock;
                        }
                    }
                    delete block;
                }
            }
        }
    }
    this->boardZobristHash = this->newBoardZobristHash;
}

bool Game::moveAnalyze(Step* step)
{
    int x = step->x, y = step->y;
    if (step->player != this->player)
    {
        return false;
    }
    if (this->board[x][y] != 0)
    {
        return false;
    }
    this->newBoardZobristHash = this->boardZobristHash;
    this->getPickUpBlock(this->allBoardPoints[x][y]);
    if (this->targetBlock->getQi() == 0 && !pickUpFlag)
    {
        return false;
    }
    if (this->historyZobristHash.find(this->newBoardZobristHash) != this->historyZobristHash.end())
    {
        return false;
    }
    this->nextStep = step;
    return true;
}

void Game::move()
{
    this->steps.push_back(this->nextStep);
    Point* targetPoint = this->allBoardPoints[this->nextStep->x][this->nextStep->y];

    this->historyBoard.push_back(this->board);
    this->historyZobristHash.insert(this->newBoardZobristHash);
    this->boardZobristHash = this->newBoardZobristHash;

    this->board[targetPoint->x][targetPoint->y] = player;
    this->player = this->player == WHITE_PLAYER ? BLACK_PLAYER : WHITE_PLAYER;
    // board update

    // self point update
    if(this->mergedBlock.empty())
    {
        this->pointBlockMap[targetPoint] = this->targetBlock;
        this->targetBlock = new GoBlock();
    }
    else
    {
        GoBlock* startBlock = *this->mergedBlock.begin();
        startBlock->clear();
        startBlock->update(this->targetBlock);
        this->pointBlockMap[targetPoint] = startBlock;
        for(auto &block : this->mergedBlock)
        {
            if(block != startBlock)
            {
                for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
                {
                    if(block->points.test(i))
                    {
                        this->pointBlockMap[this->allBoardPoints[i / BOARD_SIZE][i % BOARD_SIZE]] = startBlock;
                    }
                }
                delete block;
            }
        }
    }

    // opponent update
    std::vector<Point*> around;
    for(auto &block : this->opponentBlock)
    {
        block->removeQi(targetPoint);
        if (block->getQi() == 0)
        {
            for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
            {
                if(block->points.test(i))
                {
                    Point* point = this->allBoardPoints[i / BOARD_SIZE][i % BOARD_SIZE];
                    around.clear();
                    Point::getAround(point, this->allBoardPoints, around);
                    for (auto &aroundPoint : around)
                    {
                        if (this->board[aroundPoint->x][aroundPoint->y] + player == BLACK_PLAYER + WHITE_PLAYER)
                        {
                            this->pointBlockMap[aroundPoint]->addQi(point);
                        }
                    }
                    this->board[point->x][point->y] = 0;
                    this->pointBlockMap.erase(point);
                }
            }
            delete block;
        }
    }
}

void Game::getPickUpBlock(Point* targetPoint)
{
    std::vector<Point*> around;
    Point::getAround(targetPoint, this->allBoardPoints, around);
    this->newBoardZobristHash ^= targetPoint->zobristHash[this->player - 1];
    this->pickUpFlag = false;

    this->mergedBlock.clear();
    this->opponentBlock.clear();

    int isolatedFlag = true;
    for (auto &point :around)
    {
        if (this->board[point->x][point->y] != 0)
        {
            auto nearBlock = this->pointBlockMap[point];
            if(!(this->mergedBlock.count(nearBlock) == 0 && this->opponentBlock.count(nearBlock) == 0))
            {
                continue;
            }
            if(this->board[point->x][point->y] == player)
            {
                this->mergedBlock.insert(nearBlock);
                if(isolatedFlag)
                {
                    this->targetBlock->clear();
                    this->targetBlock->update(nearBlock);
                    this->targetBlock->addPoint(targetPoint, this->board, this->allBoardPoints);
                    isolatedFlag = false;
                }
                else
                {
                    this->targetBlock->merge(targetPoint, nearBlock);
                }
            }
            else
            {
                this->opponentBlock.insert(nearBlock);
                if(nearBlock->getQi() - 1 == 0)
                {
                    this->pickUpFlag = true;
                    this->newBoardZobristHash ^= nearBlock->zobristHash;
                }
            }
        }
    }
    if(isolatedFlag)
    {
        this->targetBlock->clear();
        this->targetBlock->update(targetPoint, this->player, around, this->board);
    }
}

void Game::boardStrEncode(char* boardStr)
{
    boardStr[0] = char(this->player + '0');
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            boardStr[i * BOARD_SIZE + j + 1] = char(this->board[i][j] + '0');
        }
    }
    boardStr[BOARD_SIZE * BOARD_SIZE + 1] = '\0';
}

Game::~Game()
{
    std::unordered_set<GoBlock*> blockSet;
    for(auto &line : this->allBoardPoints)
    {
        for(auto &point : line)
        {
            blockSet.insert(this->pointBlockMap[point]);
        }
    }
    for(auto &block : blockSet)
    {
        delete block;
    }
    delete targetBlock;
}

void Game::copy(Game &o)
{
    std::unordered_map<GoBlock*, GoBlock*> blockMap;
    o.player = this->player;
    o.allBoardPoints = this->allBoardPoints;
    std::copy(this->board.begin(), this->board.end(), o.board.begin());
    o.boardZobristHash = this->boardZobristHash;
    std::copy(this->historyBoard.begin(), this->historyBoard.end(), o.historyBoard.begin());
    o.historyZobristHash = this->historyZobristHash;
    o.newBoardZobristHash = this->newBoardZobristHash;
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            if(this->pointBlockMap.count(this->allBoardPoints[i][j]))
            {
                Point* nowPoint = this->allBoardPoints[i][j];
                GoBlock* nowBlock = this->pointBlockMap[nowPoint];
                if(blockMap.count(nowBlock))
                {
                    o.pointBlockMap[nowPoint] = blockMap[nowBlock];
                }
                else
                {
                    auto* tmpBlock = new GoBlock();
                    tmpBlock->update(this->pointBlockMap[nowPoint]);
                    o.pointBlockMap[nowPoint] = tmpBlock;
                    blockMap[nowBlock] = tmpBlock;
                }
            }
        }
    }
}

void Game::legalMove(std::vector<int> &legalMoves, std::vector<int> &qiAfterMove, bool eyeFlag)
{
    Step probStep(-1, -1, this->player);
    for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        int x = i / BOARD_SIZE, y = i % BOARD_SIZE;
        probStep.x = x;
        probStep.y = y;
        if(this->moveAnalyze(&probStep))
        {
            if(eyeFlag)
            {
                if(!this->isEye(this->allBoardPoints[x][y], this->player))
                {
                    legalMoves.push_back(i);
                    qiAfterMove.push_back(this->targetBlock->getQi());
                }
            }
            else
            {
                legalMoves.push_back(i);
                qiAfterMove.push_back(this->targetBlock->getQi());
            }
        }
    }
}

bool Game::isEye(Point* pos, int posPlayer)
{
    std::vector<Point*> around;
    Point::getAround(pos, this->allBoardPoints, around);
    for(auto &point : around)
    {
        if(this->board[point->x][point->y] != posPlayer)
        {
            return false;
        }
    }
    size_t count = 0;
    around.clear();
    Point::getDiagonal(pos, this->allBoardPoints, around);
    for(auto &point : around)
    {
        if(this->board[point->x][point->y] == posPlayer)
        {
            count++;
        }
    }
    if((count == 3 && around.size() == 4) || count == around.size())
    {
        return true;
    }
    else
    {
        return false;
    }
}
