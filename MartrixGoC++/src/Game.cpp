//
// Created by 11409 on 2021/4/17.
//

#include <set>
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
}

void Game::initHandCap(std::vector<Step*> &handCapSteps, int numOfHandCap)
{
    for (int i = 0; i < numOfHandCap; i++)
    {
        this->board[handCapSteps[i]->x][handCapSteps[i]->y] = BLACK_PLAYER;
    }
}

void Game::redo()
{
    this->steps.pop_back();
    this->board.assign(this->historyBoard.back().begin(), this->historyBoard.back().end());
    this->newBoard.assign(this->board.begin(), this->board.end());
    this->historyBoard.pop_back();
    this->player = this->player == WHITE_PLAYER ? BLACK_PLAYER : WHITE_PLAYER;
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
    this->newBoard.assign(this->board.begin(), this->board.end());
    this->newBoardZobristHash = this->boardZobristHash;
    this->getPickUpBlock(this->allBoardPoints[x][y]);
    this->targetBlock->update(this->allBoardPoints[x][y], this->newBoard, this->allBoardPoints);
    if (this->targetBlock->getQi(this->newBoard, this->allBoardPoints) == 0)
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

void Game::move(bool handCapFlag)
{
    if(!handCapFlag)
    {
        this->steps.push_back(this->nextStep);
    }
    this->player = this->player == WHITE_PLAYER ? BLACK_PLAYER : WHITE_PLAYER;
    this->historyBoard.push_back(this->board);
    this->historyZobristHash.insert(this->boardZobristHash);
        // 其他棋块的气的处理
    // 合并本颜色的棋块
    this->board.assign(this->newBoard.begin(), this->newBoard.end());
    this->boardZobristHash = this->newBoardZobristHash;
}

void Game::getPickUpBlock(Point* targetPoint)
{
    std::set<GoBlock*> nearGoBlocks;
    std::vector<Point*> around;
    Point::getAround(targetPoint, this->allBoardPoints, around);
    this->newBoard[targetPoint->x][targetPoint->y] = this->player;
    this->newBoardZobristHash ^= targetPoint->zobristHash;
    for (auto &point :around)
    {
        if (this->newBoard[point->x][point->y] != this->player && this->newBoard[point->x][point->y] != 0)
        {
            bool containCheck = true;
            for (auto &block : nearGoBlocks)
            {
                if (block->contain(point))
                {
                    containCheck = false;
                }
            }
            if (containCheck)
            {
                nearGoBlocks.insert(new GoBlock(point, this->newBoard, this->allBoardPoints));
            }
        }
    }
    for (auto &block : nearGoBlocks)
    {
        if (block->getQi(this->newBoard, this->allBoardPoints) == 0)
        {
            for (auto &piece : block->pieces)
            {
                this->newBoard[piece->x][piece->y] = 0;
                this->newBoardZobristHash ^= piece->zobristHash;
                    // 可以把围棋块的哈希值也存起来
            }
        }
    }
    //delete
    for (auto &block : nearGoBlocks)
    {
        delete block;
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
                this->newBoardZobristHash ^= this->allBoardPoints[i][j]->zobristHash;
            }
        }
    }
    inFile.close();
    this->move(true);
}

void Game::loadFromBoard(const std::string &boardCode, int gapPlayer)
{
    this->player = gapPlayer;
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            this->newBoard[i][j] = boardCode[i * BOARD_SIZE + j + 1] - '0';
            if(this->newBoard[i][j] != 0)
            {
                this->newBoardZobristHash ^= this->allBoardPoints[i][j]->zobristHash;
            }
        }
    }
    this->move(true);
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