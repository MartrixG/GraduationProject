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
    if(this->playerColor != root->game->player)
    {
        logger.fatal("Player error.");
        exit(EXIT_FAILURE);
    }
    nextStep->player = this->playerColor;
    int chosenStep;
    if(root->legalMoveSize == 0)
    {
        nextStep->pos = -1;
    }
    int mostVis = -1, winCount = 0;
    int totVis = 0;
    for(size_t i = 0; i < this->selfMct->root->legalMoveSize; i++)
    {
        int vis = root->children[i]->numRollouts;
        if(vis > mostVis)
        {
            if(mostVis != -1)
            {
                logger.debug("Find new max vis. Delete pre max children:" + std::to_string(chosenStep) + " num:" + std::to_string(root->children[chosenStep]->numRollouts));
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
    // Confess
    if((double)winCount / (double)mostVis < 0.05)
    {
        nextStep->pos = -2;
    }
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

void MCTSPlayer::updatePlayer(Game* game)
{
    logger.info(std::string(this->playerColor == BLACK_PLAYER ? "Black player" : "White player") + std::string(" search start."));
    if(this->selfMct == nullptr)
    {
        this->selfMct = new MCTS(game, this->threadPool);
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
            this->selfMct = new MCTS(game, this->threadPool);
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
