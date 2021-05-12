//
// Created by 11409 on 2021/5/6.
//

#include "MCTS.hpp"
#include "App.hpp"

TreeNode::TreeNode(TreeNode* fa, Game* faGame)
{
    this->parent = fa;
    this->nextStep = new Step(-1, -1, -1);
    this->game = faGame;
    this->game->legalMove(this->legalMove, this->qiAfterMove, this->legalMoveSize);
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
    delete this->nextStep;
    delete this->game;
    for(auto &child : this->children)
    {
        delete child;
    }
}

MCTS::MCTS(Game* game)
{
    Game* tmpGame = new Game(game->allBoardPoints, game->allAround, game->allDiagonal);
    game->copy(tmpGame);
    this->blackRandomPlayer = new RandomPlayer(randomPlayer, BLACK_PLAYER);
    this->whiteRandomPlayer = new RandomPlayer(randomPlayer, WHITE_PLAYER);
    this->root = new TreeNode(nullptr, tmpGame);
    this->defaultPolicy(root);
    updateAllChildren(root);
}

void MCTS::updateAllChildren(TreeNode* node)
{
    for(size_t i = 0; i < node->legalMoveSize; i++)
    {
        node->nextStep->player = node->game->player;
        node->nextStep->pos = node->legalMove[i];
        Game* tmpGame = new Game(node->game->allBoardPoints, node->game->allAround, node->game->allDiagonal);
        node->game->copy(tmpGame);
        tmpGame->moveAnalyze(node->nextStep);
        tmpGame->move();
        node->children[i] = new TreeNode(node, tmpGame);
        this->defaultPolicy(node->children[i]);
        MCTS::backup(node->children[i]);
    }
}

void MCTS::defaultPolicy(TreeNode* node)
{
    Game experimentGame(node->game->allBoardPoints, node->game->allAround, node->game->allDiagonal);
    node->game->copy(&experimentGame);
    RandomPlayer* player;
    if(node->game->player == BLACK_PLAYER)
    {
        player = this->blackRandomPlayer;
    }
    else
    {
        player = this->whiteRandomPlayer;
    }
    while(true)
    {
        int res;
        player->updatePlayer(&experimentGame);
        res = Application::gameCore(&experimentGame, player, node->nextStep);
        if(res == 2)
        {
            break;
        }
        player = player == this->blackRandomPlayer ? this->whiteRandomPlayer : this->blackRandomPlayer;
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
