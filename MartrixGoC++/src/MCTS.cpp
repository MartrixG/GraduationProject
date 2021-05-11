//
// Created by 11409 on 2021/5/6.
//

#include "MCTS.hpp"
#include "App.hpp"

TreeNode::TreeNode(TreeNode* fa, Game* faGame)
{
    this->parent = fa;
    this->nextStep = new Step(-1, -1, -1);
    this->game = new Game(faGame->allBoardPoints, faGame->allAround, faGame->allDiagonal);
//    faGame->copy(*this->game);
    this->game = faGame;
    //->legalMove(this->unvisitedSteps, true);
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

void TreeNode::chooseBest(TreeNode* bestNode)
{

}

double TreeNode::score(int totRollouts) const
{
    return (double)this->selfWinCount / (double)this->numRollouts + C * std::sqrt(std::log((double)totRollouts) / (double)this->numRollouts);
}

MCTS::MCTS(Game* game)
{
    Game* tmpGame = new Game(game->allBoardPoints, game->allAround, game->allDiagonal);
    game->copy(tmpGame);
    this->root = new TreeNode(nullptr, tmpGame);
    this->blackRandomPlayer = new RandomPlayer(randomPlayer, black);
    this->whiteRandomPlayer = new RandomPlayer(randomPlayer, white);
    this->defaultPolicy(root);
    updateAllChildren(root);
}

void MCTS::updateAllChildren(TreeNode* node)
{
    RandomPlayer* player;
    if(node->game->player == BLACK_PLAYER)
    {
        player = this->blackRandomPlayer;
    }
    else
    {
        player = this->whiteRandomPlayer;
    }
    player->updatePlayer(node->game);
    Step nextStep(-1, -1, -1);
    for(size_t i = 0; i < player->legalMoveSize; i++)
    {
        nextStep.player = node->game->player;
        nextStep.x = player->legalMove[i] / 19;
        nextStep.y = player->legalMove[i] % 19;
        Game* tmpGame = new Game(node->game->allBoardPoints, node->game->allAround, node->game->allDiagonal);
        node->game->copy(tmpGame);
        tmpGame->moveAnalyze(node->nextStep);
        tmpGame->move();
        node->children.push_back(new TreeNode(node, tmpGame));
        this->defaultPolicy(node->children.back());
        MCTS::backup(node->children.back());
    }
}

void MCTS::treePolicy(TreeNode* node)
{

}

void MCTS::defaultPolicy(TreeNode* node)
{
    Game* experimentGame = new Game(node->game->allBoardPoints, node->game->allAround, node->game->allDiagonal);
    node->game->copy(experimentGame);
    RandomPlayer* player;
    if(node->game->player == BLACK_PLAYER)
    {
        player = this->blackRandomPlayer;
    }
    else
    {
        player = this->whiteRandomPlayer;
    }
    Step nextStep(-1, -1, -1);
    while(true)
    {
        int res;
        player->updatePlayer(experimentGame);
        res = Application::gameCore(experimentGame, player, &nextStep);
        if(res == 2)
        {
            break;
        }
        player = player == this->blackRandomPlayer ? this->whiteRandomPlayer : this->blackRandomPlayer;
    }
    node->numRollouts++;
    if(2 - ((double)experimentGame->getWinner() >= 2.5) == node->game->player)
    {
        node->selfWinCount++;
    }
}

void MCTS::backup(TreeNode* node)
{
    while(node != nullptr)
    {
        node->parent->selfWinCount += node->numRollouts - node->selfWinCount;
        node = node->parent;
    }
}
