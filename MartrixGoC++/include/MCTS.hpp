//
// Created by 11409 on 2021/5/6.
//

#ifndef MARTRIXGOC_MCTS_HPP
#define MARTRIXGOC_MCTS_HPP

#include <vector>
#include <cmath>
#include "Step.hpp"
#include "Game.hpp"
#include "Player.hpp"

const double C = std::sqrt(2.0);

class TreeNode
{
public:
    TreeNode* parent = nullptr;
    Step* nextStep = nullptr;
    int numRollouts = 0;
    int selfWinCount = 0, opponentWinCount = 0;
    Game* game = nullptr;
    int legalMove[BOARD_SIZE * BOARD_SIZE] = {0};
    int qiAfterMove[BOARD_SIZE * BOARD_SIZE] = {0};
    size_t legalMoveSize = 0;
    TreeNode* children[BOARD_SIZE * BOARD_SIZE] = {nullptr};

    TreeNode(TreeNode* fa, Game* faGame);

    void chooseBest(TreeNode* bestNode);

    double score(int totRollouts) const;

    ~TreeNode();
};

class MCTS
{
public:
    TreeNode* root;

    RandomPlayer* blackRandomPlayer = nullptr;
    RandomPlayer* whiteRandomPlayer = nullptr;

    explicit MCTS(Game* game);

    void updateAllChildren(TreeNode* node);

    void treePolicy(TreeNode* node);

    void defaultPolicy(TreeNode* node);

    static void backup(TreeNode* node);
};

#endif //MARTRIXGOC_MCTS_HPP