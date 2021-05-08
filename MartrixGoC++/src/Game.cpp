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
//    this->historyBoard.push_back(this->board);
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
//    this->steps.push_back(this->nextStep);
    Point* targetPoint = this->allBoardPoints[this->nextStep->x][this->nextStep->y];

//    this->historyBoard.push_back(this->board);
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
                    this->aroundSize = 0;
                    Point::getAround(point, this->allBoardPoints, this->around, this->aroundSize);
                    for(size_t j = 0; j < this->aroundSize; j++)
                    {
                        if (this->board[this->around[j]->x][this->around[j]->y] + player == BLACK_PLAYER + WHITE_PLAYER)
                        {
                            this->pointBlockMap[this->around[j]]->addQi(point);
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
    this->aroundSize = 0;
    Point::getAround(targetPoint, this->allBoardPoints, this->around, this->aroundSize);
    this->newBoardZobristHash ^= targetPoint->zobristHash[this->player - 1];
    this->pickUpFlag = false;

    this->mergedBlock.clear();
    this->opponentBlock.clear();

    int isolatedFlag = true;
    for (size_t i = 0; i < this->aroundSize; i++)
    {
        if (this->board[this->around[i]->x][this->around[i]->y] != 0)
        {
            auto nearBlock = this->pointBlockMap[this->around[i]];
            if(!(this->mergedBlock.count(nearBlock) == 0 && this->opponentBlock.count(nearBlock) == 0))
            {
                continue;
            }
            if(this->board[this->around[i]->x][this->around[i]->y] == player)
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
        this->targetBlock->update(targetPoint, this->player, around, this->aroundSize, this->board);
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

int Game::getWinner()
{
    int blackCount = 0, whiteCount = 0;
    for(size_t i = 0; i < BOARD_SIZE; i++)
    {
        for(size_t j = 0; j < BOARD_SIZE; j++)
        {
            if(this->board[i][j] != 0)
            {
                if(this->pointBlockMap[this->allBoardPoints[i][j]]->getQi() == 1)
                {
                    GoBlock* deadBlock = this->pointBlockMap[this->allBoardPoints[i][j]];
                    for(size_t k = 0; k < BOARD_SIZE * BOARD_SIZE; k++)
                    {
                        if(deadBlock->points.test(k))
                        {
                            this->board[k / BOARD_SIZE][k % BOARD_SIZE] = BLACK_PLAYER + WHITE_PLAYER - deadBlock->color;
                        }
                    }
                }
            }
        }
    }
    for(size_t i = 0; i < BOARD_SIZE; i++)
    {
        for(size_t j = 0; j < BOARD_SIZE; j++)
        {
            switch (this->board[i][j])
            {
                case 0:
                    if(this->isEye(this->allBoardPoints[i][j], WHITE_PLAYER))
                    {
                        whiteCount++;
                    }
                    if(this->isEye(this->allBoardPoints[i][j], BLACK_PLAYER))
                    {
                        blackCount++;
                    }
                    break;
                case 1:
                    blackCount++;
                    break;
                case 2:
                    whiteCount++;
            }
        }
    }
    return blackCount - whiteCount;
}

void Game::legalMove(int* legalMoves, int* qiAfterMove, size_t &len)
{
    Step probStep(-1, -1, this->player);
    for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        int x = i / BOARD_SIZE, y = i % BOARD_SIZE;
        probStep.x = x;
        probStep.y = y;
        if(this->moveAnalyze(&probStep))
        {
            if(!this->isEye(this->allBoardPoints[x][y], this->player))
            {
                legalMoves[len] = i;
                qiAfterMove[len] = this->targetBlock->getQi();
                len++;
            }
        }
    }
}

bool Game::isEye(Point* pos, int posPlayer)
{
    this->aroundSize = 0;
    Point::getAround(pos, this->allBoardPoints, this->around, this->aroundSize);
    for(size_t i = 0; i < this->aroundSize; i++)
    {
        if(this->board[this->around[i]->x][this->around[i]->y] != posPlayer)
        {
            return false;
        }
    }
    size_t count = 0;
    this->aroundSize = 0;
    Point::getDiagonal(pos, this->allBoardPoints, this->around, this->aroundSize);
    for(size_t i = 0; i < this->aroundSize; i++)
    {
        if(this->board[this->around[i]->x][this->around[i]->y] == posPlayer)
        {
            count++;
        }
    }
    if((count == 3 && this->aroundSize == 4) || (count !=3 && count == this->aroundSize))
    {
        return true;
    }
    else
    {
        return false;
    }
}


void Game::copy(Game* o)
{
    std::unordered_map<GoBlock*, GoBlock*> blockMap;
    o->player = this->player;
    std::copy(this->board.begin(), this->board.end(), o->board.begin());
    o->boardZobristHash = this->boardZobristHash;
//    std::copy(this->historyBoard.begin(), this->historyBoard.end(), o->historyBoard.begin());
    o->historyZobristHash = this->historyZobristHash;
    o->newBoardZobristHash = this->newBoardZobristHash;
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            if(this->board[i][j] != 0 && this->pointBlockMap.count(this->allBoardPoints[i][j]))
            {
                Point* nowPoint = this->allBoardPoints[i][j];
                GoBlock* nowBlock = this->pointBlockMap[nowPoint];
                if(blockMap.count(nowBlock))
                {
                    o->pointBlockMap[nowPoint] = blockMap[nowBlock];
                }
                else
                {
                    auto* tmpBlock = new GoBlock();
                    tmpBlock->update(this->pointBlockMap[nowPoint]);
                    o->pointBlockMap[nowPoint] = tmpBlock;
                    blockMap[nowBlock] = tmpBlock;
                }
            }
        }
    }
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