//
// Created by 11409 on 2021/5/6.
//

#ifndef MARTRIXGOC_MCTS_HPP
#define MARTRIXGOC_MCTS_HPP

#include <exception>
#include "Game.hpp"
#include "RandomPlayer.hpp"
#include "TreeNode.hpp"

class MCTS
{
public:
    TreeNode* root;
    std::default_random_engine randNum;
    std::uniform_int_distribution<int> dist;

    explicit MCTS(Game* game);

    int search(TreeNode* &chosenNode);

    static void expand(TreeNode* node, int location);

    static void defaultPolicy(TreeNode* node);

    static void backup(TreeNode* node, int win);

    static void updateAllChildren(TreeNode* node);

    void work();
};

#endif //MARTRIXGOC_MCTS_HPP