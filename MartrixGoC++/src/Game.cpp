//
// Created by 11409 on 2021/4/17.
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
}

void Game::initHandCap(std::vector<Step *> &handCapSteps)
{
    for (auto step : handCapSteps)
    {
        this->newBoard[step->x][step->y] = 1;
    }
    this->player = this->player == 2 ? 1 : 2;
    this->move();
}

void Game::redo()
{
    this->steps.pop_back();
    this->board.assign(this->historyBoard.back().begin(), this->historyBoard.back().end());
    this->newBoard.assign(this->board.begin(), this->board.end());
    this->historyBoard.pop_back();
    this->player = this->player == 2 ? 1 : 2;
}

bool Game::moveAnalyze(Step *step)
{
    int x = step->x, y = step->y;
    if(step->player != this->player)
    {
        return false;
    }
    if(this->board[x][y] != 0)
    {
        return false;
    }
    this->tmpBoard.assign(this->newBoard.begin(), this->newBoard.end());
    this->getPickUpBlock(this->allBoardPoints[x][y], this->newBoard);
    if(this->newBoard == this->historyBoard.back())
    {
        this->newBoard.swap(this->tmpBoard);
        return false;
    }
    if(this->targetBlock == nullptr)
    {
        this->targetBlock = new GoBlock(allBoardPoints[x][y], this->newBoard, this->allBoardPoints);
    }
    else
    {
        this->targetBlock->update(this->allBoardPoints[x][y], this->newBoard, this->allBoardPoints);
    }
    if(this->targetBlock->getQi(this->allBoardPoints) == 0)
    {
        this->newBoard.swap(this->tmpBoard);
        return false;
    }
    this->steps.push_back(step);
    this->nextStep = step;
    return true;
}

void Game::move()
{
    this->player = this->player == 2 ? 1 : 2;
    this->historyBoard.push_back(this->board);
    this->board.assign(this->newBoard.begin(), this->newBoard.end());
}

void Game::getPickUpBlock(Point *targetPoint, vector_2d(int) &processBoard)
{
    std::set<GoBlock *> nearGoBlocks;
    std::vector<Point *> around;
    Point::getAround(targetPoint, this->allBoardPoints, around);
    processBoard[targetPoint->x][targetPoint->y] = this->player;
    for (auto &point :around)
    {
        if (processBoard[point->x][point->y] != this->player && processBoard[point->x][point->y] != 0)
        {
            int containCheck = 1;
            for (auto &block : nearGoBlocks)
            {
                if (block->contain(point))
                {
                    containCheck = 0;
                }
            }
            if (containCheck)
            {
                nearGoBlocks.insert(new GoBlock(point, processBoard, this->allBoardPoints));
            }
        }
    }
    for (auto &block : nearGoBlocks)
    {
        if (block->getQi(this->allBoardPoints) == 0)
        {
            for (auto &piece : block->pieces)
            {
                processBoard[piece->x][piece->y] = 0;
            }
        }
    }
    //delete
    for (auto &block : nearGoBlocks)
    {
        delete block;
    }
}

void Game::loadFromBoard(const std::string& fileName, int nowPlayer)
{
    this->player = nowPlayer == 2 ? 1 : 2;
    std::ifstream inFile;
    inFile.open(fileName);
    for(int i=0;i<19;i++)
    {
        for(int j=0;j<19;j++)
        {
            inFile >> this->newBoard[i][j];
        }
    }
    inFile.close();
    this->move();
}
//

