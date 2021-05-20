//
// Created by 11409 on 2021/5/13.
//

#include "TreeNode.hpp"

TreeNode::TreeNode(TreeNode* fa, Game* faGame)
{
    this->parent = fa;
    this->game = faGame;
    this->game->legalMove(this->legalMove, this->qiAfterMove, this->legalMoveSize);
    this->nodeRandomPlayer = new RandomPlayer(BLACK_PLAYER);
}

double TreeNode::score(double totRollouts)
{
    auto parentWinCount = (double)this->winCount[BLACK_PLAYER + WHITE_PLAYER - this->game->player];
    auto threadVisD = (double)this->threadVis;
    auto thisNumRollouts = (double)this->numRollouts;
    if(thisNumRollouts == 0)
    {
        logger.warning("0 rollout is in search.");
    }
    return parentWinCount / thisNumRollouts + std::sqrt(C * std::log(totRollouts + threadVisD) / (thisNumRollouts + threadVisD));
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