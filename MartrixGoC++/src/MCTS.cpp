//
// Created by 11409 on 2021/5/6.
//

#include "MCTS.hpp"
#include "App.hpp"

TreeNode::TreeNode(TreeNode* fa, Game* faGame)
{
    this->parent = fa;
    this->game = faGame;
    this->game->legalMove(this->legalMove, this->qiAfterMove, this->legalMoveSize);
    this->nodeRandomPlayer = new RandomPlayer(BLACK_PLAYER);
}

void TreeNode::chooseBest(TreeNode* bestNode)
{

}

double TreeNode::score(int totRollouts) const
{
    return (double)this->selfWinCount / (double)this->numRollouts + C * std::sqrt(std::log((double)totRollouts) / (double)this->numRollouts);
}

TreeNode::~TreeNode()
{
    delete this->game;
    delete this->nodeRandomPlayer;

    for(auto &child : this->children)
    {
        delete child;
    }
}

MCTS::MCTS(Game* game)
{
    Game* tmpGame = new Game(game->allBoardPoints, game->allAround, game->allDiagonal);
    game->copy(tmpGame);
    this->root = new TreeNode(nullptr, tmpGame);
    MCTS::defaultPolicy(root);
    MCTS::updateAllChildren(root);
}

void MCTS::updateAllChildren(TreeNode* node)
{
    for(size_t i = 0; i < node->legalMoveSize; i++)
    {
        Game* tmpGame = new Game(node->game->allBoardPoints, node->game->allAround, node->game->allDiagonal);
        node->game->copy(tmpGame);
        tmpGame->nextStep->player = node->game->player;
        tmpGame->nextStep->pos = node->legalMove[i];
        tmpGame->moveAnalyze(tmpGame->nextStep);
        tmpGame->move();
        node->children[i] = new TreeNode(node, tmpGame);
        MCTS::defaultPolicy(node->children[i]);
        MCTS::backup(node->children[i]);
    }
}

void MCTS::defaultPolicy(TreeNode* node)
{
    Game experimentGame(node->game->allBoardPoints, node->game->allAround, node->game->allDiagonal);
    node->game->copy(&experimentGame);
    RandomPlayer* player = node->nodeRandomPlayer;
    player->playerColor = node->game->player;
    while(true)
    {
        player->updatePlayer(&experimentGame);
        if(player->legalMoveSize == 0)
        {
            break;
        }
        player->getNextStep(experimentGame.nextStep);
        experimentGame.moveAnalyze(experimentGame.nextStep);
        experimentGame.move();
        player->playerColor = BLACK_PLAYER + WHITE_PLAYER - player->playerColor;
    }
    node->numRollouts++;
    if(2 - ((double)experimentGame.getWinner() >= 2.5) == node->game->player)
    {
        node->selfWinCount++;
    }
}

void MCTS::treePolicy(TreeNode* node)
{

}

void MCTS::backup(TreeNode* node)
{
    while(node->parent != nullptr)
    {
        node->parent->selfWinCount += node->numRollouts - node->selfWinCount;
        node->parent->numRollouts += node->numRollouts;
        node = node->parent;
    }
}
