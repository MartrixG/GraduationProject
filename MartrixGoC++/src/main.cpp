#include <map>
#include <iostream>
#include "App.hpp"
#include "Point.hpp"
#include "Game.hpp"
#include <sys/time.h>
#include <unistd.h>

enum argsEnum
{
    loadSgf, analyze, test, commandLine
};
std::map<std::string, int> argsTransform;

void initArgs()
{
    argsTransform["loadSgf"] = loadSgf;
    argsTransform["analyze"] = analyze;
    argsTransform["test"] = test;
    argsTransform["commandLine"] = commandLine;
}

void testCode()
{
    GoBlock::test();
    Point::test();
}

int main(int argc, char *argv[])
{
    //struct timeval start, end;
    //gettimeofday(&start, nullptr);
    initArgs();
    if (argsTransform.find(argv[1]) == argsTransform.end())
    {
        std::cout << "Can not find arg: " << argv[1] << '\n';
        return 0;
    }
    switch (argsTransform[argv[1]])
    {
        case loadSgf:
            if (argc < 3)
            {
                std::cout << "load sgf must have a file.";
                break;
            }
            Application::loadSGF(argv[2]);
            break;
        case analyze:
            Application::gameInformationAnalyze();
            break;
        case commandLine:
            Application::commandLine();
            break;
        case test:
            testCode();
            break;
    }
    //gettimeofday(&end, nullptr);
    //std::cout << end.tv_usec - start.tv_usec;
}