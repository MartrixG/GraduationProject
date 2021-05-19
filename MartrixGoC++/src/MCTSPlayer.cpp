//
// Created by 11409 on 2021/5/5.
//

#include <iostream>
#include <cstring>
#include "MCTSPlayer.hpp"

MCTSPlayer::MCTSPlayer(int color, ThreadPool* pool, int timeLimit)
{
    this->playerColor = color;
    this->randNum = std::default_random_engine(GetCurrentThreadId() + std::chrono::system_clock::now().time_since_epoch().count());
    this->dist = std::uniform_int_distribution<int>(0, 0x7fffffff);
    this->timeLimit = timeLimit;
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
        size_t chosenStep = -1;
        for(size_t i = 0; i < this->selfMct->root->legalMoveSize; i++)
        {
            if(this->selfMct->root->legalMove[i] == game->nextStep->pos)
            {
                chosenStep = i;
                logger.info("Find step searched.");
            }
            else
            {
                delete this->selfMct->root->children[i];
            }
        }
        if(chosenStep == -1)
        {
            logger.fatal("Can not find step searched.");
            exit(EXIT_FAILURE);
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
        logger.info("history search child number:" + std::to_string(historySearchChild));
        logger.info("history search count:" + std::to_string(this->selfMct->root->numRollouts));
    }
    this->selfMct->work(this->timeLimit);
    logger.info("tot search:" + std::to_string(this->selfMct->root->numRollouts));
    if(this->selfMct->root->threadVis != 0)
    {
        logger.warning("root thread vis:" + std::to_string(this->selfMct->root->threadVis));
    }
    else
    {
        logger.info("root thread vis:" + std::to_string(this->selfMct->root->threadVis));
    }
}
