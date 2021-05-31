//
// Created by 11409 on 2021/5/6.
//

#ifndef MARTRIXGOC_MCTS_HPP
#define MARTRIXGOC_MCTS_HPP

#include "RandomPlayer.hpp"
#include "ThreadPool.hpp"
#include "PythonBackend.hpp"
#include "Log.hpp"

class MCTS
{
public:
    PythonBackend* backend;
    TreeNode* root;
    size_t poolSize;
    bool mctsFlag;
    bool dlFlag;

    ThreadPool* threadPool = nullptr;

    MCTS(Game* game, ThreadPool* pool, SOCKET &serverSocket, bool mctsFlag, bool dlFlag);

    int search(TreeNode* &chosenNode) const;

    void expand(TreeNode* node, int location) const;

    static void defaultPolicy(TreeNode* node);

    static void addThreadVis(TreeNode* node);

    void updateAllChildren(TreeNode* node) const;

    void work(int rolloutTime) const;

    ~MCTS();
};

#endif //MARTRIXGOC_MCTS_HPP