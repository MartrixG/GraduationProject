//
// Created by 11409 on 2021/4/17.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include "Game.hpp"

Game::Game(PointPtr* points, std::unordered_map<PointPtr, PArVecPtr>* around, std::unordered_map<PointPtr, PDivecPtr>* diagonal)
{
    this->allBoardPoints = points;
    this->allAround = around;
    this->allDiagonal = diagonal;
    this->targetBlock = new GoBlock();
    this->historyZobristHash.insert(0);
    memset(this->board, 0, sizeof(this->board));
}

void Game::initHandCap(std::vector<Step*> &handCapSteps, int numOfHandCap)
{
    /*
    vector_2d(int) tmpBoard(BOARD_SIZE, std::vector<int>(BOARD_SIZE));
    this->historyBoard.push_back(tmpBoard);
    for(size_t i = 0; i < BOARD_SIZE; i++)
    {
        for(size_t j = 0; j < BOARD_SIZE; j++)
        {
            this->historyBoard.back()[i][j] = this->board[i * BOARD_SIZE + j];
        }
    }
    */
    for (int i = 0; i < numOfHandCap; i++)
    {
        int pos = handCapSteps[i]->pos;
        this->getPickUpBlock(this->allBoardPoints[pos]);
        this->board[pos] = player;
        if(this->mergedBlock.empty())
        {
            this->pointBlockMap[this->allBoardPoints[pos]] = this->targetBlock;
            this->targetBlock = new GoBlock();
        }
        else
        {
            BlockPtr startBlock = *this->mergedBlock.begin();
            startBlock->clear();
            startBlock->update(this->targetBlock);
            this->pointBlockMap[this->allBoardPoints[pos]] = startBlock;
            for(auto &block : this->mergedBlock)
            {
                if(block != startBlock)
                {
                    for(int j = 0; j < BOARD_SIZE * BOARD_SIZE; j++)
                    {
                        if(block->points.test(j))
                        {
                            this->pointBlockMap[this->allBoardPoints[j]] = startBlock;
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
    int pos = step->pos;
    if (step->player != this->player)
    {
        return false;
    }
    if (this->board[pos] != 0)
    {
        return false;
    }
    this->newBoardZobristHash = this->boardZobristHash;
    this->getPickUpBlock(this->allBoardPoints[pos]);
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
    PointPtr targetPoint = this->allBoardPoints[this->nextStep->pos];
    /*
    this->historyBoard.push_back(tmpBoard);
    for(size_t i = 0; i < BOARD_SIZE; i++)
    {
        for(size_t j = 0; j < BOARD_SIZE; j++)
        {
            this->historyBoard.back()[i][j] = this->board[i * BOARD_SIZE + j];
        }
    }
    */
    this->historyZobristHash.insert(this->newBoardZobristHash);
    this->boardZobristHash = this->newBoardZobristHash;

    this->board[targetPoint->pos] = player;
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
        BlockPtr startBlock = *this->mergedBlock.begin();
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
                        this->pointBlockMap[this->allBoardPoints[i]] = startBlock;
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
                    PointPtr nowPoint = this->allBoardPoints[i];
                    PArVecPtr around = (*this->allAround)[nowPoint];
                    for(auto &point : *around)
                    {
                        if (this->board[point->pos] + player == BLACK_PLAYER + WHITE_PLAYER)
                        {
                            this->pointBlockMap[point]->addQi(nowPoint);
                        }
                    }
                    this->board[nowPoint->pos] = 0;
                    this->pointBlockMap.erase(nowPoint);
                }
            }
            delete block;
        }
    }
}

void Game::getPickUpBlock(PointPtr targetPoint)
{
    this->newBoardZobristHash ^= targetPoint->zobristHash[this->player - 1];
    this->pickUpFlag = false;

    this->mergedBlock.clear();
    this->opponentBlock.clear();

    int isolatedFlag = true;
    std::vector<PointPtr>* around = (*this->allAround)[targetPoint];
    for (auto &point : *around)
    {
        if (this->board[point->pos] != 0)
        {
            auto nearBlock = this->pointBlockMap[point];
            if(this->mergedBlock.count(nearBlock) != 0 || this->opponentBlock.count(nearBlock) != 0)
            {
                continue;
            }
            if(this->board[point->pos] == player)
            {
                this->mergedBlock.insert(nearBlock);
                if(isolatedFlag)
                {
                    this->targetBlock->clear();
                    this->targetBlock->update(nearBlock);
                    this->targetBlock->addPoint(targetPoint, around, this->board);
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

void Game::boardStrEncode(char* boardStr) const
{
    boardStr[0] = char(this->player + '0');
    for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        boardStr[i + 1] = char(this->board[i] + '0');
    }
    boardStr[BOARD_SIZE * BOARD_SIZE + 1] = '\0';
}

int Game::getWinner()
{
    int blackCount = 0, whiteCount = 0;
    for(size_t i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        if(this->board[i] != 0)
        {
            if(this->pointBlockMap[this->allBoardPoints[i]]->getQi() == 1)
            {
                BlockPtr deadBlock = this->pointBlockMap[this->allBoardPoints[i]];
                for(size_t k = 0; k < BOARD_SIZE * BOARD_SIZE; k++)
                {
                    if(deadBlock->points.test(k))
                    {
                        this->board[k] = BLACK_PLAYER + WHITE_PLAYER - deadBlock->color;
                    }
                }
            }
        }
    }
    for(size_t i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        switch (this->board[i])
        {
            case 0:
                if(this->isEye(this->allBoardPoints[i], WHITE_PLAYER))
                {
                    whiteCount++;
                }
                if(this->isEye(this->allBoardPoints[i], BLACK_PLAYER))
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
    return blackCount - whiteCount;
}

void Game::legalMove(int* legalMoves, int* qiAfterMove, size_t &len)
{
    Step probStep(-1, -1, this->player);
    for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        probStep.pos = i;
        if(this->moveAnalyze(&probStep))
        {
            if(!this->isEye(this->allBoardPoints[i], this->player))
            {
                legalMoves[len] = i;
                qiAfterMove[len] = this->targetBlock->getQi();
                len++;
            }
        }
    }
}

bool Game::isEye(PointPtr pos, int posPlayer) const
{
    PArVecPtr around = (*this->allAround)[pos];
    for(auto &point : *around)
    {
        if(this->board[point->pos] != posPlayer)
        {
            return false;
        }
    }
    size_t count = 0;
    PDivecPtr diagonal = (*this->allDiagonal)[pos];
    for(auto &point : *diagonal)
    {
        if(this->board[point->pos] == posPlayer)
        {
            count++;
        }
    }
    if((count == 3 && (*this->allDiagonal)[pos]->size() == 4) || (count !=3 && count == (*this->allDiagonal)[pos]->size()))
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
    std::unordered_map<BlockPtr, BlockPtr> blockMap;
    o->player = this->player;
    memcpy(o->board, this->board, sizeof(*this->board));
    o->boardZobristHash = this->boardZobristHash;
//    std::copy(this->historyBoard.begin(), this->historyBoard.end(), o->historyBoard.begin());
    o->historyZobristHash = this->historyZobristHash;
    o->newBoardZobristHash = this->newBoardZobristHash;
    for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        if(this->board[i] != 0 && this->pointBlockMap.count(this->allBoardPoints[i]))
        {
            PointPtr nowPoint = this->allBoardPoints[i];
            BlockPtr nowBlock = this->pointBlockMap[nowPoint];
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

Game::~Game()
{
    std::unordered_set<BlockPtr> blockSet;
    for(size_t i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        blockSet.insert(this->pointBlockMap[this->allBoardPoints[i]]);
    }
    for(auto &block : blockSet)
    {
        delete block;
    }
    delete targetBlock;
}