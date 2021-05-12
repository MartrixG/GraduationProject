//
// Created by 11409 on 2021/4/17.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include "Game.hpp"

Game::Game(PointPtr* points, PArVecPtr* around, PDiVecPtr* diagonal)
{
    this->allBoardPoints = points;
    this->allAround = around;
    this->allDiagonal = diagonal;
    this->targetBlock = new GoBlock();
    this->historyZobristHash.insert(0);
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
        this->getPickUpBlock(pos);
        this->board[pos] = player;
        if(this->mergedBlock.empty())
        {
            this->pointBlockMap[pos] = this->targetBlock;
            this->targetBlock = new GoBlock();
        }
        else
        {
            BlockPtr startBlock = *this->mergedBlock.begin();
            startBlock->clear();
            startBlock->update(this->targetBlock);
            this->pointBlockMap[pos] = startBlock;
            for(auto &block : this->mergedBlock)
            {
                if(block != startBlock)
                {
                    for(int j = 0; j < BOARD_SIZE * BOARD_SIZE; j++)
                    {
                        if(block->points.test(j))
                        {
                            this->pointBlockMap[j] = startBlock;
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
    this->getPickUpBlock(pos);
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
        this->pointBlockMap[targetPoint->pos] = this->targetBlock;
        this->targetBlock = new GoBlock();
    }
    else
    {
        BlockPtr startBlock = *this->mergedBlock.begin();
        startBlock->clear();
        startBlock->update(this->targetBlock);
        this->pointBlockMap[targetPoint->pos] = startBlock;
        for(auto &block : this->mergedBlock)
        {
            if(block != startBlock)
            {
                for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
                {
                    if(block->points.test(i))
                    {
                        this->pointBlockMap[i] = startBlock;
                    }
                }
                delete block;
            }
        }
    }

    // opponent update
    for(auto &block : this->opponentBlock)
    {
        block->removeQi(targetPoint->pos);
        if (block->getQi() == 0)
        {
            for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
            {
                if(block->points.test(i))
                {
                    PArVecPtr around = this->allAround[i];
                    for(auto &point : *around)
                    {
                        if (this->board[point] + player == BLACK_PLAYER + WHITE_PLAYER)
                        {
                            this->pointBlockMap[point]->addQi(i);
                        }
                    }
                    this->board[i] = 0;
                    this->pointBlockMap[i] = nullptr;
                }
            }
            delete block;
        }
    }
}

void Game::getPickUpBlock(int targetPoint)
{
    this->newBoardZobristHash ^= this->allBoardPoints[targetPoint]->zobristHash[this->player - 1];
    this->pickUpFlag = false;

    this->mergedBlock.clear();
    this->opponentBlock.clear();

    int isolatedFlag = true;
    PArVecPtr around = this->allAround[targetPoint];
    for (auto &point : *around)
    {
        if (this->board[point] != 0)
        {
            auto nearBlock = this->pointBlockMap[point];
            if(this->mergedBlock.count(nearBlock) != 0 || this->opponentBlock.count(nearBlock) != 0)
            {
                continue;
            }
            if(this->board[point] == player)
            {
                this->mergedBlock.insert(nearBlock);
                if(isolatedFlag)
                {
                    this->targetBlock->clear();
                    this->targetBlock->update(nearBlock);
                    this->targetBlock->addPoint(this->allBoardPoints[targetPoint], around, this->board);
                    isolatedFlag = false;
                }
                else
                {
                    this->targetBlock->merge(this->allBoardPoints[targetPoint], nearBlock);
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
        this->targetBlock->update(this->allBoardPoints[targetPoint], this->player, around, this->board);
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
            if(this->pointBlockMap[i]->getQi() == 1)
            {
                BlockPtr deadBlock = this->pointBlockMap[i];
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
    PArVecPtr around;
    for(size_t i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        switch (this->board[i])
        {
            case 0:
                around = this->allAround[i];
                if(this->board[around->at(0)] == WHITE_PLAYER)
                {
                    whiteCount++;
                }
                if(this->board[around->at(0)] == WHITE_PLAYER)
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
            if(!this->isEye(i, this->player))
            {
                legalMoves[len] = i;
                qiAfterMove[len] = this->targetBlock->getQi();
                len++;
            }
        }
    }
}

bool Game::isEye(int pos, int posPlayer) const
{
    PArVecPtr around = this->allAround[pos];
    for(auto &point : *around)
    {
        if(this->board[point] != posPlayer)
        {
            return false;
        }
    }
    size_t count = 0;
    PArVecPtr diagonal = this->allDiagonal[pos];
    for(auto &point : *diagonal)
    {
        if(this->board[point] == posPlayer)
        {
            count++;
        }
    }
    if((count == 3 && diagonal->size() == 4) || (count !=3 && count == diagonal->size()))
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
    memcpy(o->board, this->board, sizeof(this->board));
    o->boardZobristHash = this->boardZobristHash;
//    std::copy(this->historyBoard.begin(), this->historyBoard.end(), o->historyBoard.begin());
    o->historyZobristHash = this->historyZobristHash;
    o->newBoardZobristHash = this->newBoardZobristHash;
    for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        BlockPtr nowBlock = this->pointBlockMap[i];
        if(nowBlock != nullptr)
        {
            if(blockMap.count(nowBlock))
            {
                o->pointBlockMap[i] = blockMap[nowBlock];
            }
            else
            {
                auto* tmpBlock = new GoBlock();
                tmpBlock->update(nowBlock);
                o->pointBlockMap[i] = tmpBlock;
                blockMap[nowBlock] = tmpBlock;
            }
        }
    }
}

Game::~Game()
{
    std::unordered_set<BlockPtr> blockSet;
    for(auto & i : this->pointBlockMap)
    {
        blockSet.insert(i);
    }
    for(auto &block : blockSet)
    {
        delete block;
    }
    delete targetBlock;
}