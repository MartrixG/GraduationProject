//
// Created by 11409 on 2021/5/5.
//

#ifndef MARTRIXGOC_MCTSPLAYER_HPP
#define MARTRIXGOC_MCTSPLAYER_HPP

#include "Step.hpp"
#include "Game.hpp"
#include "MCTS.hpp"
#include "ThreadPool.hpp"
#include "Log.hpp"

extern MiniLog logger;

class MCTSPlayer
{
public:
    int playerColor;
    int timeLimit;
    MCTS* selfMct = nullptr;
    SOCKET server;

    int playerType;
    bool mctsFlag = false;
    bool dlFlag = false;

    ThreadPool* threadPool = nullptr;

    MCTSPlayer(int color, ThreadPool* pool, int type, SOCKET &serverSocket);

    void getNextStep(Step *nextStep);

    void getFirstStep(Step* netStep) const;

    void updatePlayer(Game* game);
};

#endif //MARTRIXGOC_MCTSPLAYER_HPP
