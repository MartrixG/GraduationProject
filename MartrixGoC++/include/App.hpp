//
// Created by 11409 on 2021/4/22.
//

#ifndef MARTRIXGOC_APP_HPP
#define MARTRIXGOC_APP_HPP

#include <iostream>
#include <fstream>
#include "Point.hpp"

class Application
{
public:
    static void loadSGF(int argc, char* argv[]);

    static void makeData(int argc, char* argv[]);

    static void gameInformationAnalyze(vector_2d(Point*) &allBoardPoints, std::string &srcSgf,
                                       std::ofstream &featureFileStream, std::ofstream&labelFileStream);

    static void commandLine(int argc, char* argv[]);
};

#endif //MARTRIXGOC_APP_HPP
