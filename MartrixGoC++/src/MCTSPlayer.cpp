//
// Created by 11409 on 2021/5/5.
//

#include <iostream>
#include <cstring>
#include "MCTSPlayer.hpp"

MCTSPlayer::MCTSPlayer(int color)
{
    this->playerColor = color;
    this->randNum = std::default_random_engine(GetCurrentThreadId() + std::chrono::system_clock::now().time_since_epoch().count());
    this->dist = std::uniform_int_distribution<int>(0, 0x7fffffff);
}

void MCTSPlayer::getFirstStep(Step* nextStep)
{
    nextStep->player = playerColor;
    int star = BOARD_SIZE * (BOARD_SIZE / 4 - 1) + BOARD_SIZE - (BOARD_SIZE / 4);
    int dPos[5] = {0, 1, -1, BOARD_SIZE, -BOARD_SIZE};
    int i = this->dist(this->randNum) % 5;
    nextStep->pos = star + dPos[i];
}

void MCTSPlayer::getNextStep(Step* nextStep)
{
    nextStep->player = playerColor;
    if(this->roots[0]->legalMoveSize == 0)
    {
        nextStep->pos = -1;
    }
    int mostVis = -1;
    for(size_t i = 0; i < this->roots[0]->legalMoveSize; i++)
    {
        int vis = this->roots[0]->children[i]->numRollouts;
        if(vis > mostVis)
        {
            mostVis = vis;
            nextStep->pos = this->roots[0]->legalMove[i];
        }
    }
    delete this->roots[0];
}

void MCTSPlayer::updatePlayer(Game* game)
{
    MCTS searchTree(game);
    searchTree.work();
    this->roots[0] = searchTree.root;
    for(size_t i = 0; i < searchTree.root->legalMoveSize; i++)
    {
        std::cout << "pos: " << i << "  vis: " << searchTree.root->children[i]->numRollouts << '\n';
    }
}
