//
// Created by 11409 on 2021/4/21.
//

#include "GoSGF.hpp"
#include <iostream>
#include <fstream>

GoSGF::GoSGF(std::string &srcFileContext)
{
    this->nowStep = 0;
    this->fileContext = srcFileContext;
    this->gameInformation = this->fileContext.substr(INFORMATION_START, this->fileContext.size() - INFORMATION_END);
    this->RU = this->gameInformation[this->gameInformation.find("RU") + INFORMATION_BIAS];
    char tmpPlayer = this->gameInformation[this->gameInformation.find("RE") + INFORMATION_BIAS];
    this->RE = tmpPlayer == 'B' ? BLACK_PLAYER : WHITE_PLAYER;
    this->HA = this->gameInformation[this->gameInformation.find("HA") + INFORMATION_BIAS] - '0';
    int numOfSteps = 0, startOfStep = int(this->gameInformation.find(';')), bias = 0;
    for (auto &c : this->gameInformation)
    {
        if (c == ';')
        { numOfSteps++; }
    }
    if (this->HA > 1)
    {
        int afterABPos = int(this->gameInformation.find("AB")) + 2;
        int numOfHandCap = 0;
        for (int i = afterABPos; i < int(this->gameInformation.size()); i++)
        {
            if (this->gameInformation[i] == ';')
            {
                startOfStep = i;
                numOfSteps--;
                break;
            }
            if (this->gameInformation[i] == '[')
            { numOfHandCap++; }
        }
        for (int i = 0; i < numOfHandCap; i++)
        {
            this->steps.push_back(new Step('B',
                                           this->gameInformation[afterABPos + i * HAND_CAP_BIAS + 2],
                                           this->gameInformation[afterABPos + i * HAND_CAP_BIAS + 1]));
        }
    }
    for (int i = 0; i < numOfSteps; i++)
    {
        this->steps.push_back(new Step(this->gameInformation[startOfStep + bias + PLAYER_BIAS],
                                       this->gameInformation[startOfStep + bias + X_BIAS],
                                       this->gameInformation[startOfStep + bias + Y_BIAS]));
        bias += STEP_STR_LENGTH;
    }
}

void GoSGF::show()
{
    std::cout << "Rule :\t" << this->RU << std::endl;
    std::cout << "result :\t" << (this->RE == BLACK_PLAYER ? "B" : "W") << std::endl;
    std::cout << "hand cap :\t" << this->HA << std::endl;
    for (auto &step : this->steps)
    {
        std::cout << (BLACK_PLAYER == step->player ? "B " : "W ") << *step;
    }
}

bool GoSGF::haveNextStep() const
{
    if (this->nowStep == int(this->steps.size()))
    {
        return false;
    } else
    {
        return true;
    }
}

void GoSGF::getNextStep(Step*&nextStep)
{
    nextStep = this->steps[this->nowStep];
    this->nowStep++;
}