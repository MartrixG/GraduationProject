//
// Created by 11409 on 2021/5/6.
//

#ifndef MARTRIXGOC_MCTS_HPP
#define MARTRIXGOC_MCTS_HPP

#include <exception>
#include "Game.hpp"
#include "RandomPlayer.hpp"
#include "TreeNode.hpp"
#include "ThreadPool.hpp"
#include "Log.hpp"

extern MiniLog logger;

class MCTS
{
public:
    TreeNode* root;
    size_t poolSize;

    ThreadPool* threadPool = nullptr;

    MCTS(Game* game, ThreadPool* pool);

    int search(TreeNode* &chosenNode) const;

    static void expand(TreeNode* node, int location);

    static void defaultPolicy(TreeNode* node);

    static void addThreadVis(TreeNode* node);

    void updateAllChildren(TreeNode* node) const;

    void work(int rolloutTime) const;
};

#endif //MARTRIXGOC_MCTS_HPP