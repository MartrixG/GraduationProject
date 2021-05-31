//
// Created by 11409 on 2021/4/17.
//

#include <iostream>
#include <cstring>
#include "Game.hpp"

Game::Game(PointPtr* points, PArVecPtr* around, PDiVecPtr* diagonal)
{
    this->allBoardPoints = points;
    this->allAround = around;
    this->allDiagonal = diagonal;
    this->targetBlock = new GoBlock();
    this->nextStep = new Step(-1, -1, -1);
    this->historyZobristHash.insert(0);
    this->historyBoard.emplace_back(BOARD_SIZE * BOARD_SIZE);

    this->board = new int[BOARD_SIZE * BOARD_SIZE];
    this->pointBlockMap = new BlockPtr[BOARD_SIZE * BOARD_SIZE];
    for(size_t i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        this->board[i] = 0;
        this->pointBlockMap[i] = nullptr;
        this->historyBoard[0][i] = this->board[i];
    }
}

void Game::initHandCap(std::vector<Step*> &handCapSteps, int numOfHandCap)
{
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
    this->historyBoard.emplace_back(BOARD_SIZE * BOARD_SIZE);
    for(size_t i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        this->historyBoard.back()[i] = this->board[i];
    }
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
    PointPtr targetPoint = this->allBoardPoints[this->nextStep->pos];
    this->historyZobristHash.insert(this->newBoardZobristHash);
    this->steps.push_back(new Step(this->nextStep->pos / BOARD_SIZE, this->nextStep->pos % BOARD_SIZE, this->player));
    this->historyBoard.emplace_back(BOARD_SIZE * BOARD_SIZE);
    for(size_t i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        this->historyBoard.back()[i] = this->board[i];
    }
    this->boardZobristHash = this->newBoardZobristHash;

    // board update
    this->board[targetPoint->pos] = player;
    this->player = this->player == WHITE_PLAYER ? BLACK_PLAYER : WHITE_PLAYER;

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
    boardStr[1] = char(this->player + '0');
    for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        boardStr[i + 2] = char(this->board[i] + '0');
    }
    boardStr[BOARD_SIZE * BOARD_SIZE + 2] = '\0';
}

int Game::getWinner() const
{
    int blackCount = 0, whiteCount = 0;
    for(size_t i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        switch (this->board[i])
        {
            case BLACK_PLAYER:
                blackCount++;
                break;
            case WHITE_PLAYER:
                whiteCount++;
                break;
        }
    }
    return blackCount - whiteCount;
}

void Game::legalMove(int* legalMoves, int* qiAfterMove, size_t &len)
{
    this->nextStep->player = this->player;
    for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        this->nextStep->pos = i;
        if(this->moveAnalyze(this->nextStep))
        {
            if(!this->isEye(i, this->player))
            {
                legalMoves[len] = i;
                if(qiAfterMove != nullptr)
                { qiAfterMove[len] = this->targetBlock->getQi(); }
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


void Game::copy(Game* o) const
{
    std::unordered_map<BlockPtr, BlockPtr> blockMap;
    o->player = this->player;
    for(size_t i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        o->board[i] = this->board[i];
    }
    o->historyBoard.assign(this->historyBoard.begin(), this->historyBoard.end());
    for(auto& step : this->steps)
    {
        o->steps.push_back(new Step(step->pos / BOARD_SIZE, step->pos % BOARD_SIZE, step->player));
    }
    o->boardZobristHash = this->boardZobristHash;
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
    for(size_t i = 0 ; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        blockSet.insert(this->pointBlockMap[i]);
    }
    for(auto &block : blockSet)
    {
        delete block;
    }
    delete targetBlock;
    delete nextStep;
    delete []board;
    delete []pointBlockMap;
    for(auto &step : this->steps)
    {
        delete step;
    }
}