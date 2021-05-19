//
// Created by 11409 on 2021/5/5.
//

#include <iostream>
#include <cstring>
#include "MCTSPlayer.hpp"

MCTSPlayer::MCTSPlayer(int color, ThreadPool* pool)
{
    this->playerColor = color;
    this->randNum = std::default_random_engine(GetCurrentThreadId() + std::chrono::system_clock::now().time_since_epoch().count());
    this->dist = std::uniform_int_distribution<int>(0, 0x7fffffff);
    this->threadPool = pool;
}

void MCTSPlayer::getFirstStep(Step* nextStep)
{
    nextStep->player = playerColor;
    int star = BOARD_SIZE * (BOARD_SIZE / 4 - 1) + BOARD_SIZE - (BOARD_SIZE / 4);
    int dPos[3] = {0, -1, BOARD_SIZE};
    int i = this->dist(this->randNum) % 3;
    nextStep->pos = star + dPos[i];
}

void MCTSPlayer::getNextStep(Step* nextStep) const
{
    TreeNode* root = this->selfMct->root;
    nextStep->player = playerColor;
    if(root->legalMoveSize == 0)
    {
        nextStep->pos = -1;
    }
    int mostVis = -1, winCount = 0;
    for(size_t i = 0; i < this->selfMct->root->legalMoveSize; i++)
    {
        int vis = root->children[i]->numRollouts;
        if(vis > mostVis)
        {
            mostVis = vis;
            winCount = this->selfMct->root->children[i]->winCount[root->game->player];
            nextStep->pos = root->legalMove[i];
        }
    }
    // Confess
    if((double)winCount / (double)mostVis < 0.05)
    {
        nextStep->pos = -2;
    }
}

void MCTSPlayer::updatePlayer(Game* game)
{
    if(this->selfMct == nullptr)
    {
        this->selfMct = new MCTS(game, this->threadPool);
    }
    else
    {
        size_t chosenStep;
        for(size_t i = 0; i < this->selfMct->root->legalMoveSize; i++)
        {
            if(this->selfMct->root->legalMove[i] == game->nextStep->pos)
            {
                chosenStep = i;
            }
            else
            {
                delete this->selfMct->root->children[i];
            }
        }
        this->selfMct->root = this->selfMct->root->children[chosenStep];
        this->selfMct->root->parent = nullptr;
        int historySearchChild = 0;
        for(size_t i = 0; i < this->selfMct->root->legalMoveSize; i++)
        {
            if(this->selfMct->root->children[i] != nullptr)
            {
                historySearchChild++;
            }
        }
        std::cout <<"history search child number:" << historySearchChild;
        std::cout << "\nhistory search count:" << this->selfMct->root->numRollouts << std::endl;
    }
    this->selfMct->work();
    std::cout << "\ntot search:" << this->selfMct->root->numRollouts
              << " thread vis:" << this->selfMct->root->threadVis << "\n";
}
