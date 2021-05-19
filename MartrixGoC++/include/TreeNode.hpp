//
// Created by 11409 on 2021/5/13.
//

#ifndef MARTRIXGOC_TREENODE_HPP
#define MARTRIXGOC_TREENODE_HPP

#include <unordered_set>
#include <mutex>
#include <atomic>
#include "Game.hpp"
#include "RandomPlayer.hpp"

const double C = 2.0;

class TreeNode
{
public:
    TreeNode* parent = nullptr;
    std::atomic<int> winCount[3] = {0};
    std::atomic<int> numRollouts = 0;
    std::atomic<int> threadVis = 0;

    std::unordered_set<int> visitedMove;
    int legalMove[BOARD_SIZE * BOARD_SIZE] = {0};
    int qiAfterMove[BOARD_SIZE * BOARD_SIZE] = {0};
    size_t legalMoveSize = 0;

    RandomPlayer* nodeRandomPlayer = nullptr;
    Game* game = nullptr;
    TreeNode* children[BOARD_SIZE * BOARD_SIZE] = {nullptr};

    TreeNode(TreeNode* fa, Game* faGame);

    double score(double totRollouts);

    ~TreeNode();
};

#endif //MARTRIXGOC_TREENODE_HPP
