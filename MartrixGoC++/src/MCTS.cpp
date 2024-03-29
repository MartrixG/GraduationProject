//
// Created by 11409 on 2021/5/6.
//

#include "MCTS.hpp"
#include <ctime>

MCTS::MCTS(Game* game, ThreadPool* pool, SOCKET &serverSocket, bool mctsFlag, bool dlFlag)
{
    this->threadPool = pool;
    this->poolSize = pool->poolSize;
    this->backend = new PythonBackend(serverSocket);
    this->mctsFlag = mctsFlag;
    this->dlFlag = dlFlag;

    Game* tmpGame = new Game(game->allBoardPoints, game->allAround, game->allDiagonal);
    game->copy(tmpGame);
    this->root = new TreeNode(nullptr, 1.0, tmpGame);
    if(this->dlFlag)
    {
        this->backend->commit(this->root);
        this->backend->pushRequest();
    }
    if(this->mctsFlag)
    {
        MCTS::addThreadVis(root);
        MCTS::defaultPolicy(root);
    }
}

inline int MCTS::search(TreeNode* &chosenNode) const
{
    chosenNode = this->root;
    TreeNode* bestNode = nullptr;
    auto totRollouts = (double)this->root->numRollouts;
    while(chosenNode->legalMoveSize != 0 && chosenNode->visitedMove.size() == chosenNode->legalMoveSize)
    {
        double score = -1, tmpScore;
        TreeNode* child;
        for(auto &pos : chosenNode->visitedMove)
        {
            child = chosenNode->children[pos];
            if(child->numRollouts == 0)
            {
                continue;
            }
            if(this->dlFlag)
            {
                if(std::abs(child->qValue - 1.0) <= 1e-6)
                {
                    logger.warning("Meeting child's q not update.");
                    this->backend->commit(chosenNode);
                    this->backend->pushRequest();
                }
            }
            tmpScore = child->score(totRollouts);
            if(tmpScore > score)
            {
                score = tmpScore;
                bestNode = child;
            }
        }
        chosenNode = bestNode;
    }
    size_t i;
    {
        if(chosenNode->legalMoveSize == 0)
        {
            return -1;
        }
        for(i = 0; i < chosenNode->legalMoveSize; i++)
        {
            if(chosenNode->children[i] == nullptr)
            {
                break;
            }
        }
    }
    return (int)i;
}

inline void MCTS::expand(TreeNode* node, int location) const
{
    Game* tmpGame = new Game(node->game->allBoardPoints, node->game->allAround, node->game->allDiagonal);
    node->game->copy(tmpGame);
    tmpGame->nextStep->player = node->game->player;
    tmpGame->nextStep->pos = node->legalMove[location];
    tmpGame->moveAnalyze(tmpGame->nextStep);
    tmpGame->move();
    node->children[location] = new TreeNode(node, node->childQ[location], tmpGame);
    node->visitedMove.insert(location);
    if(this->dlFlag)
    {
        this->backend->commit(node->children[location]);
    }
}

void MCTS::defaultPolicy(TreeNode* node)
{
    Game experimentGame(node->game->allBoardPoints, node->game->allAround, node->game->allDiagonal);
    node->game->copy(&experimentGame);
    RandomPlayer* player = node->nodeRandomPlayer;
    player->playerColor = node->game->player;
    size_t maxEpoch = BOARD_SIZE * BOARD_SIZE * 3 / 2;
    player->updatePlayer(&experimentGame);
    for(size_t i = BOARD_SIZE * BOARD_SIZE - player->legalMoveSize; i < maxEpoch; i++)
    {
        player->updatePlayer(&experimentGame);
        player->getNextStep(experimentGame.nextStep);
        if(experimentGame.nextStep->pos == -1)
        {
            break;
        }
        experimentGame.moveAnalyze(experimentGame.nextStep);
        experimentGame.move();
        player->playerColor = BLACK_PLAYER + WHITE_PLAYER - player->playerColor;
    }
    int winColor = 2 - (experimentGame.getWinner() >= 2);
    while(node != nullptr)
    {
        node->threadVis--;
        node->winCount[winColor]++;
        node->numRollouts++;
        node = node->parent;
    }
}

inline void MCTS::addThreadVis(TreeNode* node)
{
    while(node != nullptr)
    {
        node->threadVis++;
        node = node->parent;
    }
}

void MCTS::updateAllChildren(TreeNode* node) const
{
    for(size_t i = 0; i < node->legalMoveSize; i++)
    {
        if(node->children[i] == nullptr)
        {
            MCTS::expand(node, (int)i);
            MCTS::addThreadVis(node->children[i]);
            this->threadPool->addTask(defaultPolicy, node->children[i]);
        }
    }
    while(this->threadPool->availableThreads != this->poolSize)
    {
        Sleep(0);
    }
}

void MCTS::work(int rolloutTime) const
{
    if(!this->mctsFlag)
    {
        return;
    }
    clock_t start = std::clock(), end;
    if(this->root->legalMoveSize == 0)
    {
        return;
    }
    if(!this->dlFlag)
    { updateAllChildren(this->root); }

    TreeNode* expandNode = nullptr;
    while (true)
    {
        end = std::clock();
        if(end - start > rolloutTime)
        {
            break;
        }
        int location = this->search(expandNode);
        if(location == -1)
        {
            continue;
        }
        MCTS::expand(expandNode, location);
        MCTS::addThreadVis(expandNode->children[location]);
        this->threadPool->addTask(defaultPolicy, expandNode->children[location]);

        if(this->threadPool->queueSize >= 2 * (int)this->poolSize)
        {
            while (this->threadPool->availableThreads != this->poolSize)
            {
                Sleep(0);
            }
        }
    }
    logger.info("Last pending works:" + std::to_string(this->threadPool->queueSize));
    while(this->threadPool->availableThreads != this->poolSize)
    {
        Sleep(0);
    }
    if(this->dlFlag)
    {
        logger.info("Last pending board state for model:" + std::to_string(this->backend->size()));
        this->backend->clear();
    }
}

MCTS::~MCTS()
{
    delete root;
    delete backend;
}
