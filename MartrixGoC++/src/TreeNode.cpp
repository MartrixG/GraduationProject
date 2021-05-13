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
            double tmpScore = child->score(totRollouts);
            if(tmpScore > score)
            {
                score = tmpScore;
                bestNode = child;
            }
        }
    }
}

double TreeNode::score(int totRollouts) const
{
    return (double)this->selfWinCount / (double)this->numRollouts + std::sqrt(C * std::log((double)totRollouts) / (double)this->numRollouts);
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