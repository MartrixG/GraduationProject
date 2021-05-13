//
// Created by 11409 on 2021/5/13.
//

#ifndef MARTRIXGOC_TREENODE_HPP
#define MARTRIXGOC_TREENODE_HPP

#include <unordered_set>
#include "Game.hpp"
#include "RandomPlayer.hpp"

const double C = 2.0;

class TreeNode
{
public:
    TreeNode* parent = nullptr;
    int numRollouts = 0;
    int selfWinCount = 0, opponentWinCount = 0;
    std::unordered_set<int> unvisitedMove;
    int legalMove[BOARD_SIZE * BOARD_SIZE] = {0};
    int qiAfterMove[BOARD_SIZE * BOARD_SIZE] = {0};

    RandomPlayer* nodeRandomPlayer = nullptr;
    Game* game = nullptr;
    size_t legalMoveSize = 0;
    TreeNode* children[BOARD_SIZE * BOARD_SIZE] = {nullptr};

    TreeNode(TreeNode* fa, Game* faGame);

    void chooseBest(TreeNode* &bestNode, int totRollouts);

    double score(int totRollouts) const;

    ~TreeNode();
};

#endif //MARTRIXGOC_TREENODE_HPP
