//
// Created by 11409 on 2021/5/13.
//

#ifndef MARTRIXGOC_COMMANDLINEPLAYER_HPP
#define MARTRIXGOC_COMMANDLINEPLAYER_HPP

#include <iostream>
#include "Step.hpp"

class CommandLinePlayer
{
public:
    int playerColor;

    explicit CommandLinePlayer(int color);

    void getNextStep(Step* nextStep) const;

};

#endif //MARTRIXGOC_COMMANDLINEPLAYER_HPP
