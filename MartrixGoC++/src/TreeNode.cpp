//
// Created by 11409 on 2021/5/13.
//

#include "TreeNode.hpp"

TreeNode::TreeNode(TreeNode* fa, double q, Game* faGame)
{
    this->qValue = q;
    this->parent = fa;
    this->game = faGame;
    this->game->legalMove(this->legalMove, nullptr, this->legalMoveSize);
    for(float & i : childQ)
    {
        i = 1.0;
    }
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
    return parentWinCount / thisNumRollouts + this->qValue * std::sqrt(C * std::log(totRollouts + threadVisD) / (thisNumRollouts + threadVisD));
}

void TreeNode::updateChildQ(const char* recvMessage)
{
    float recvNum;
    int i = 0;
    for(size_t legalPosNum = 0; legalPosNum < this->legalMoveSize; legalPosNum++)
    {
        while(i != this->legalMove[legalPosNum])
        {
            i++;
        }
        recvNum = 0.0;
        for(size_t j = 0; j < 6; j++)
        {
            recvNum /= 10;
            recvNum += float(recvMessage[i * 6 + j] - '0') / 10.0f;
        }
        this->childQ[legalPosNum] = recvNum * 50;
    }
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