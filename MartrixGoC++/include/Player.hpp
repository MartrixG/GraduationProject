//
// Created by 11409 on 2021/5/5.
//

#ifndef MARTRIXGOC_PLAYER_HPP
#define MARTRIXGOC_PLAYER_HPP

#include <winsock.h>
#include "Step.hpp"

enum playerEnum
{
    commandLinePlayer, socketPlayer, MCTPlayer
};

enum playColorEnum
{
    black, white
};

class PlayerBase
{
public:
    playerEnum playerType;
    playColorEnum playerColor;
    PlayerBase(playerEnum type, playColorEnum color);
    virtual void getNextStep(Step* nextStep) = 0;
};

class CommandLinePlayer : public PlayerBase
{
public:
    using PlayerBase::PlayerBase;
    void getNextStep(Step* nextStep) override;
};

class SocketPlayer : public PlayerBase
{
public:
    char* srcMessage = nullptr;

    using PlayerBase::PlayerBase;
    void getNextStep(Step* nextStep) override;
    void updatePlayer(char* message);
};

class MCTSPlayer : public PlayerBase
{
public:
    using PlayerBase::PlayerBase;
    void getNextStep(Step *nextStep) override;
    void getFirstStep(Step* netStep) const;
    void updatePlayer();
};

#endif //MARTRIXGOC_PLAYER_HPP
