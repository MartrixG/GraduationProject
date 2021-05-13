//
// Created by 11409 on 2021/5/13.
//

#ifndef MARTRIXGOC_SOCKETPLAYER_HPP
#define MARTRIXGOC_SOCKETPLAYER_HPP

#include <cstring>
#include "Step.hpp"

class SocketPlayer
{
public:
    char* srcMessage = nullptr;
    int playerColor;

    explicit SocketPlayer(int color);

    void updatePlayer(char* message);

    void getNextStep(Step* nextStep) const;
};

#endif //MARTRIXGOC_SOCKETPLAYER_HPP
