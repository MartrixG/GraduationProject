//
// Created by 11409 on 2021/4/17.
#include <set>
#include "Game.hpp"
#include "Step.hpp"
#include "GoBlock.hpp"
#include "Point.hpp"

Game::Game(int player, vector_2d(Point*) &points) {
    this->player = player;
    this->historyBoard = std::vector<vector_2d(int)>();
    historyBoard.emplace_back(this->board);
    this->points = points;
}

void Game::initHandCap(std::vector<Step *> &handCapSteps) {
    for (auto step : handCapSteps) {
        this->board[step->x][step->y] = 1;
    }
}

void Game::redo() {
    this->steps.pop_back();
    this->board.assign(this->historyBoard.back().begin(), this->historyBoard.back().end());
    this->historyBoard.pop_back();
    this->player = this->player == 2 ? 1 : 2;
}

bool Game::moveAnalyze(Step *step) {}

void Game::move() {
    this->player = this->player == 2 ? 1 : 2;
    this->historyBoard.push_back(this->board);
    this->board.assign(this->newBoard.begin(), this->newBoard.end());
}

void Game::getPickUpBlock(Point *targetPoint, vector_2d(int) &tmpBoard) {
    std::set<GoBlock *> nearGoBlocks;
    std::vector<Point *> around;
    Point::getAround(targetPoint, this->allBoardPoints, around);
    tmpBoard[targetPoint->x][targetPoint->y] = this->player;
    for (auto &point :around) {
        if (tmpBoard[point->x][point->y] != this->player && tmpBoard[point->x][point->y] != 0) {
            if (nearGoBlocks.empty()) {
                nearGoBlocks.insert(new GoBlock(point, tmpBoard, this->allBoardPoints));
            }
            int containCheck = 1;
            for(auto &block : nearGoBlocks){
                if(block->contain(point)){
                    containCheck = 0;
                }
            }
            if(containCheck){
                nearGoBlocks.insert(new GoBlock(point, tmpBoard, this->allBoardPoints));
            }
        }
    }
    for(auto &block : nearGoBlocks){
        if(block->getQi(this->allBoardPoints) == 0){
            for(auto &piece : block->pieces){
                tmpBoard[piece->x][piece->y] = 0;
            }
        }
    }
    //delete
    for(auto &block : nearGoBlocks){
        delete block;
    }
}
//

