#include <map>
#include <iostream>
#include <ctime>
#include "App.hpp"
#include "Point.hpp"
#include "Game.hpp"

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

void testCode(int argc, char* argv[])
{
    GoBlock::test();
    Point::test();
}

int main(int argc, char* argv[])
{
    clock_t start = std::clock();
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
            testCode(argc, argv);
            break;
    }
    clock_t end = std::clock();
    std::cout << end - start;
}