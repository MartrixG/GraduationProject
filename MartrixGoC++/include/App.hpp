//
// Created by 11409 on 2021/4/22.
//

#ifndef MARTRIXGOC_APP_HPP
#define MARTRIXGOC_APP_HPP

#include "Point.hpp"

class Application
{
public:
    static void loadSGF(int argc, char* argv[]);

    static void makeData(int argc, char* argv[]);

    static void gameInformationAnalyze(vector_2d(Point*) allBoardPoints, std::string fileContext);

    static void commandLine(int argc, char* argv[]);
};

#endif //MARTRIXGOC_APP_HPP
