//
// Created by 11409 on 2021/4/17.
//

#include <iostream>
#include <fstream>
#include "Game.hpp"
#include "Step.hpp"
#include "GoBlock.hpp"
#include "Point.hpp"

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
                            this->pointBlockMap[this->allBoardPoints[j / 19][j % 19]] = startBlock;
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

    this->board[targetPoint->x][targetPoint->y] = player;
    this->player = this->player == WHITE_PLAYER ? BLACK_PLAYER : WHITE_PLAYER;
    this->historyBoard.push_back(this->board);
    this->historyZobristHash.insert(this->newBoardZobristHash);
    this->boardZobristHash = this->newBoardZobristHash;
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
                        this->pointBlockMap[this->allBoardPoints[i / 19][i % 19]] = startBlock;
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
                    Point* point = this->allBoardPoints[i / 19][i % 19];
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

void Game::loadFromBoardFile(const std::string &fileName, int gapPlayer)
{
    this->player = gapPlayer;
    std::ifstream inFile;
    inFile.open(fileName);
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            inFile >> this->newBoard[i][j];
            if(this->newBoard[i][j] != 0)
            {
                //this->newBoardZobristHash ^= this->allBoardPoints[i][j]->zobristHash;
            }
        }
    }
    inFile.close();
    this->move();
}

void Game::loadFromBoardStr(const std::string &boardCode, int gapPlayer)
{
    //BFS
    this->player = gapPlayer;
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            if(boardCode[i * BOARD_SIZE + j + 1] - '0' != 0)
            {
                //Step* step = new S
            }
            this->newBoard[i][j] = boardCode[i * BOARD_SIZE + j + 1] - '0';
            if(this->newBoard[i][j] != 0)
            {
                //this->newBoardZobristHash ^= this->allBoardPoints[i][j]->zobristHash;
            }
        }
    }
    this->move();
}

void Game::boardStrEncode(char* boardStr)
{
    boardStr[0] = char(this->player + '0');
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            boardStr[i * 19 + j + 1] = char(this->board[i][j] + '0');
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