//
// Created by 11409 on 2021/5/5.
//

#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include "Player.hpp"

PlayerBase::PlayerBase(playerEnum type, playColorEnum color)
{
    this->playerType = type;
    this->playerColor = color;
}

void CommandLinePlayer::getNextStep(Step* nextStep)
{
    int x, y;
    std::cin >> x >> y;
    nextStep->x = x - 1;
    nextStep->y = y - 1;
    nextStep->player = (int)playerColor + 1;
}

void SocketPlayer::getNextStep(Step* nextStep)
{
    size_t len = strlen(this->srcMessage);
    int pos = 0;
    for(size_t i = 1; i < len; i++)
    {
        pos *= 10;
        pos += this->srcMessage[i] - '0';
    }
    nextStep->player = (int)this->playerColor + 1;
    nextStep->x = pos / 19;
    nextStep->y = pos % 19;
}

void SocketPlayer::updatePlayer(char* message)
{
    this->srcMessage = message;
}

void MCTSPlayer::getNextStep(Step* nextStep)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 randNum(seed);
    std::uniform_int_distribution<int> dist(0, 361);
    nextStep->player = (int)playerColor + 1;
    nextStep->x = dist(randNum) % 19;
    nextStep->y = dist(randNum) % 19;
}

void MCTSPlayer::getFirstStep(Step* nextStep) const
{
    nextStep->player = (int)playerColor + 1;
    int dx[5] = {0, 1, -1, 0, 0};
    int dy[5] = {0, 0, 0, 1, -1};
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 randNum(seed);
    std::uniform_int_distribution<int> dist(0, 4);
    int i = dist(randNum);
    nextStep->x = dx[i];
    nextStep->y = dy[i];
}

void MCTSPlayer::updatePlayer()
{

}
