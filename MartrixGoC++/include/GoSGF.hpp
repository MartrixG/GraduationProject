//
// Created by 11409 on 2021/4/21.
//

#ifndef MARTRIXGOC_GOSGF_H
#define MARTRIXGOC_GOSGF_H

#include <string>
#include "Step.hpp"

const int STEP_STR_LENGTH   = 6;
const int PLAYER_BIAS       = 1;
const int X_BIAS            = 4;
const int Y_BIAS            = 3;
const int INFORMATION_BIAS  = 3;
const int INFORMATION_START = 2;
const int INFORMATION_END   = 3;
const int HAND_CAP_BIAS     = 4;

class GoSGF
{
public:
    std::string RU, fileContext, gameInformation;
    int RE, HA;
    int nowStep;
    std::vector<Step*> steps;

    explicit GoSGF(std::string &file);
    void show();
    bool haveNextStep() const;
    void getNextStep(Step* nextStep);
};
#endif //MARTRIXGOC_GOSGF_H
