//
// Created by 11409 on 2021/5/6.
//

#include "MCTS.hpp"

TreeNode::TreeNode(TreeNode* fa, Game* faGame)
{
    this->parent = fa;
    this->game = new Game(faGame->allBoardPoints);
    faGame->copy(*this->game);
    //->legalMove(this->unvisitedSteps, true);
}