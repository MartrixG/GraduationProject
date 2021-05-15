//
// Created by 11409 on 2021/5/6.
//

#include "MCTS.hpp"
#include <ctime>

MCTS::MCTS(Game* game)
{
    this->randNum = std::default_random_engine(GetCurrentThreadId() + std::chrono::system_clock::now().time_since_epoch().count());
    this->dist = std::uniform_int_distribution<int>(0, 0x7fffffff);
    Game* tmpGame = new Game(game->allBoardPoints, game->allAround, game->allDiagonal);
    game->copy(tmpGame);
    this->root = new TreeNode(nullptr, tmpGame);
    MCTS::defaultPolicy(root);
}

int MCTS::search(TreeNode* &chosenNode)
{
    chosenNode = this->root;
    TreeNode* bestNode = nullptr;
    int totRollouts = this->root->numRollouts;
    while(chosenNode->unvisitedMove.empty() && chosenNode->legalMoveSize != 0)
    {
        chosenNode->chooseBest(bestNode, totRollouts);
        chosenNode = bestNode;
    }
    if(chosenNode->legalMoveSize == 0)
    {
        return -1;
    }
    int location = this->dist(this->randNum) % (int)chosenNode->unvisitedMove.size();
    for(auto &i : chosenNode->unvisitedMove)
    {
        location--;
        if(location == -1)
        {
            return i;
        }
    }
}

void MCTS::expand(TreeNode* node, int location)
{
    Game* tmpGame = new Game(node->game->allBoardPoints, node->game->allAround, node->game->allDiagonal);
    node->game->copy(tmpGame);
    tmpGame->nextStep->player = node->game->player;
    tmpGame->nextStep->pos = node->legalMove[location];
    tmpGame->moveAnalyze(tmpGame->nextStep);
    tmpGame->move();
    node->children[location] = new TreeNode(node, tmpGame);
    node->unvisitedMove.erase(location);
}

int MCTS::defaultPolicy(TreeNode* node)
{
    Game experimentGame(node->game->allBoardPoints, node->game->allAround, node->game->allDiagonal);
    node->game->copy(&experimentGame);
    RandomPlayer* player = node->nodeRandomPlayer;
    player->playerColor = node->game->player;
    while(true)
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
    int winColor = 2 - ((double)experimentGame.getWinner() >= 2.5);
    node->numRollouts++;
    node->winCount[winColor]++;
    return winColor;
}

void MCTS::backup(TreeNode* node, int winColor)
{
    while(node != nullptr)
    {
        node->winCount[winColor]++;
        node->numRollouts++;
        node = node->parent;
    }
}

void MCTS::updateAllChildren(TreeNode* node)
{
    for(size_t i = 0; i < node->legalMoveSize; i++)
    {
        MCTS::expand(node, (int)i);
        int winColor = MCTS::defaultPolicy(node->children[i]);
        MCTS::backup(node, winColor);
    }
}

void MCTS::work()
{
    if(this->root->legalMoveSize == 0)
    {
        return;
    }
    updateAllChildren(this->root);
    for (size_t i = 0; i < 10000; i++)
    {
        TreeNode* expandNode = nullptr;
        int location = this->search(expandNode);
        if(location == -1)
        {
            return;
        }
        expand(expandNode, location);
        int winColor = defaultPolicy(expandNode->children[location]);
        MCTS::backup(expandNode, winColor);
    }
}