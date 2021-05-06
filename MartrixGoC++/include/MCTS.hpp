//
// Created by 11409 on 2021/5/6.
//

#ifndef MARTRIXGOC_MCTS_HPP
#define MARTRIXGOC_MCTS_HPP

#include <vector>
#include "Step.hpp"
#include "Game.hpp"

class TreeNode
{
public:
    TreeNode* parent = nullptr;
    Step* nextStep = nullptr;
    int blackWinCount = 0, whiteWinCount = 0, numRollouts = 0;
    Game* game = nullptr;
    std::vector<int> unvisitedSteps = std::vector<int>();
    std::vector<TreeNode*> children = std::vector<TreeNode*>();

    TreeNode(TreeNode* fa, Game* faGame);
};

class MCTS
{

};

#endif //MARTRIXGOC_MCTS_HPP