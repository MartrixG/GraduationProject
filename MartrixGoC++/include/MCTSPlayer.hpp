//
// Created by 11409 on 2021/5/5.
//

#ifndef MARTRIXGOC_MCTSPLAYER_HPP
#define MARTRIXGOC_MCTSPLAYER_HPP

#include <random>
#include <chrono>
#include "windows.h"
#include "Step.hpp"
#include "Game.hpp"
#include "MCTS.hpp"
#include "ThreadPool.hpp"
#include "Log.hpp"

extern MiniLog logger;
class TreeNode;

class MCTSPlayer
{
public:
    int playerColor;
    std::default_random_engine randNum;
    std::uniform_int_distribution<int> dist;
    int timeLimit;
    MCTS* selfMct = nullptr;

    ThreadPool* threadPool = nullptr;

    MCTSPlayer(int color, ThreadPool* pool, int timeLimit);

    void getNextStep(Step *nextStep) const;

    void getFirstStep(Step* netStep);

    void updatePlayer(Game* game);
};

#endif //MARTRIXGOC_MCTSPLAYER_HPP
