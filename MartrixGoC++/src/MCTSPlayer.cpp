//
// Created by 11409 on 2021/5/5.
//

#include <iostream>
#include <cstring>
#include "MCTSPlayer.hpp"

MCTSPlayer::MCTSPlayer(int color, ThreadPool* pool, int type, SOCKET &serverSocket)
{
    this->playerColor = color;
    this->timeLimit = type;
    this->threadPool = pool;
    this->server = serverSocket;
    this->playerType = type % 10;
    // 1: mcts 2: dl 3: mcts + dl
    switch (this->playerType)
    {
        case 1:
            this->mctsFlag = true;
            break;
        case 2:
            this->dlFlag = true;
            break;
        case 3:
            this->mctsFlag = true;
            this->dlFlag = true;
            break;
    }
}

void MCTSPlayer::getFirstStep(Step* nextStep) const
{
    nextStep->player = playerColor;
    nextStep->pos = 72;
}

void MCTSPlayer::getNextStep(Step* nextStep)
{
    TreeNode* root = this->selfMct->root;
    if(this->playerColor != root->game->player)
    {
        logger.fatal("Player error.");
        exit(EXIT_FAILURE);
    }
    nextStep->player = this->playerColor;
    int chosenStep = 0;
    if(root->legalMoveSize == 0)
    {
        nextStep->pos = -1;
    }
    int mostVis = -1, winCount = 0, totVis = 0;
    float maxQ = -1;
    for(size_t i = 0; i < this->selfMct->root->legalMoveSize; i++)
    {
        if(this->mctsFlag)
        {
            int vis = root->children[i]->numRollouts;
            if(vis > mostVis)
            {
                if(mostVis != -1)
                {
                    logger.debug("Find new max vis child: " + std::to_string(i) + " vis num:" + std::to_string(root->children[i]->numRollouts));
                    logger.debug("Delete pre max children:" + std::to_string(chosenStep) + " vis num:" + std::to_string(root->children[chosenStep]->numRollouts));
                    totVis += root->children[chosenStep]->numRollouts;
                    delete root->children[chosenStep];
                    root->children[chosenStep] = nullptr;
                }
                mostVis = vis;
                winCount = root->children[i]->winCount[this->playerColor];
                nextStep->pos = root->legalMove[i];
                chosenStep = (int)i;
            }
            else
            {
                logger.debug("Delete children:" + std::to_string(i) + " num:" + std::to_string(root->children[i]->numRollouts));
                totVis += root->children[i]->numRollouts;
                delete root->children[i];
                root->children[i] = nullptr;
            }
        }
        else
        {
            if(root->childQ[root->legalMove[i]] > maxQ)
            {
                maxQ = root->childQ[root->legalMove[i]];
                nextStep->pos = root->legalMove[i];
            }
        }
    }
    // Confess
    if(this->mctsFlag)
    {
        if((double)winCount / (double)mostVis < 0.15)
        {
            nextStep->pos = -2;
        }
        logger.info(std::string() + (this->playerColor == BLACK_PLAYER ? "B" : "W") + " predict win rate:" + std::to_string((double)winCount / (double)mostVis));
        totVis += root->children[chosenStep]->numRollouts;
        logger.debug("tot vis : " + std::to_string(totVis));
        if(totVis + 1 != root->numRollouts)
        {
            logger.warning("Children rollout does not match with root rollout.");
        }
        this->selfMct->root = root->children[chosenStep];
        this->selfMct->root->parent = nullptr;

        logger.debug("Delete pre root:" + std::to_string(chosenStep));
        root->children[chosenStep] = nullptr;
        delete root;
    }
    else
    {
        delete this->selfMct;
        this->selfMct = nullptr;
    }
}

void MCTSPlayer::updatePlayer(Game* game)
{
    logger.info(std::string(this->playerColor == BLACK_PLAYER ? "Black player" : "White player") + std::string(" search start."));

    if(this->selfMct == nullptr)
    {
        this->selfMct = new MCTS(game, this->threadPool, this->server, this->mctsFlag, this->dlFlag);
        logger.info("MCT is null, creat new MCT.");
    }
    else
    {
        int chosenStep = -1;
        if(this->selfMct->root->game->player != game->nextStep->player || this->playerColor != game->player)
        {
            logger.fatal("Player error.");
            exit(EXIT_FAILURE);
        }
        for(size_t i = 0; i < this->selfMct->root->legalMoveSize; i++)
        {
            if(this->selfMct->root->children[i] == nullptr)
            {
                continue;
            }
            if(this->selfMct->root->legalMove[i] == game->nextStep->pos)
            {
                chosenStep = (int)i;
                logger.info("Expand predict hit.");
            }
            else
            {
                delete this->selfMct->root->children[i];
                this->selfMct->root->children[i] = nullptr;
            }
        }
        if(chosenStep == -1)
        {
            delete this->selfMct;
            this->selfMct = new MCTS(game, this->threadPool, this->server, this->mctsFlag, this->dlFlag);
            logger.info("Expand predict miss. Creat new MCT.");
        }
        else
        {
            TreeNode* preRoot = this->selfMct->root;
            this->selfMct->root = this->selfMct->root->children[chosenStep];
            this->selfMct->root->parent = nullptr;
            preRoot->children[chosenStep] = nullptr;
            delete preRoot;
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
    }
    logger.info("Search start.");
    this->selfMct->work(this->timeLimit);
    logger.info("Search finish. Total search:" + std::to_string(this->selfMct->root->numRollouts));
    if(this->selfMct->root->threadVis != 0)
    {
        logger.warning("root thread vis:" + std::to_string(this->selfMct->root->threadVis));
    }
    else
    {
        logger.info("root thread vis:" + std::to_string(this->selfMct->root->threadVis));
    }
}
