//
// Created by 11409 on 2021/5/13.
//

#include "TreeNode.hpp"

TreeNode::TreeNode(TreeNode* fa, Game* faGame)
{
    this->parent = fa;
    this->game = faGame;
    this->game->legalMove(this->legalMove, this->qiAfterMove, this->legalMoveSize);
    for(size_t i = 0; i < this->legalMoveSize; i++)
    {
        this->unvisitedMove.insert((int)i);
    }
    this->nodeRandomPlayer = new RandomPlayer(BLACK_PLAYER);
}

void TreeNode::chooseBest(TreeNode* &bestNode, int totRollouts)
{
    double score = -1;
    for(auto &child : this->children)
    {
        if(child != nullptr)
        {
            double tmpScore = child->score((double)totRollouts);
            if(tmpScore > score)
            {
                score = tmpScore;
                bestNode = child;
            }
        }
    }
}

double TreeNode::score(double totRollouts) const
{
    auto parentWinCount = (double)this->winCount[BLACK_PLAYER + WHITE_PLAYER - this->game->player];
    return parentWinCount / (double)this->numRollouts + std::sqrt(C * std::log(totRollouts) / (double)this->numRollouts);
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