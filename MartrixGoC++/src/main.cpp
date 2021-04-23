#include <map>
#include <iostream>
#include "App.hpp"
#include "Point.hpp"
#include "Game.hpp"
#include <sys/time.h>
#include <unistd.h>

enum argsEnum
{
    loadSgf, makeData, test, commandLine
};
std::map<std::string, int> argsTransform;

void initArgs()
{
    argsTransform["loadSgf"] = loadSgf;
    argsTransform["makeData"] = makeData;
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
    struct timeval start, end;
    gettimeofday(&start, nullptr);
    initArgs();
    if (argsTransform.find(argv[1]) == argsTransform.end())
    {
        std::cout << "Can not find arg: " << argv[1] << '\n';
        return 0;
    }
    switch (argsTransform[argv[1]])
    {
        case loadSgf:
            Application::loadSGF(argc, argv);
            break;
        case makeData:
            Application::makeData(argc, argv);
            break;
        case commandLine:
            Application::commandLine(argc, argv);
            break;
        case test:
            testCode();
            break;
    }
    gettimeofday(&end, nullptr);
    std::cout << end.tv_usec - start.tv_usec;
}