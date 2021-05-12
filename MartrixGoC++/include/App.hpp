//
// Created by 11409 on 2021/4/22.
//

#ifndef MARTRIXGOC_APP_HPP
#define MARTRIXGOC_APP_HPP

#include <iostream>
#include <fstream>
#include "Point.hpp"
#include "Player.hpp"
#include "Game.hpp"


class Application
{
public:
    using PointPtr = Point::PointPtr;
    using PArVecPtr = Point::PArVecPtr;
    using PDiVecPtr = Point::PDiVecPtr;

    static void loadSGF(int argc, char* argv[]);

    static void makeData(int argc, char* argv[]);

    static void gameInformationAnalyze(PointPtr* allBoardPoints, PArVecPtr* around, PDiVecPtr* diagonal,
                                       std::string &srcSgf, std::ofstream &featureFileStream, std::ofstream&labelFileStream);

    static int gameCore(Game* game, PlayerBase* player, Step* nextStep);

    static void commandLine(int argc, char* argv[]);

    static void uiSocket(int argc, char* argv[]);

    static void randomPlayerTest(int argc, char** argv);
};

#endif //MARTRIXGOC_APP_HPP
