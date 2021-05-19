#include <map>
#include <iostream>
#include <ctime>
#include "App.hpp"
#include "GoBlock.hpp"
#include "ThreadPool.hpp"
#include <random>
#include "windows.h"

enum argsEnum
{
    loadSgf, makeData, test, commandLine, ui, randomPlayerTest, mctsPlayerTest
};
std::map<std::string, int> argsTransform;

void initArgs()
{
    argsTransform["loadSgf"]       =  loadSgf;
    argsTransform["makeData"]      =  makeData;
    argsTransform["test"]          =  test;
    argsTransform["commandLine"]   =  commandLine;
    argsTransform["ui"]            =  ui;
    argsTransform["random"]        =  randomPlayerTest;
    argsTransform["mcts"]          =  mctsPlayerTest;
}

int func()
{
    std::default_random_engine randNum(GetCurrentThreadId() + std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(0, 0x7fffffff);
    int epoch = (dist(randNum) % 10) + 969000;
    int a = 1, b = 1, c = 0;
    for(int i = 0; i < epoch; i++)
    {
        c = a + b;
        a = b;
        b = c;
        c %= 1000000000;
    }
    return c;
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
        case ui:
            Application::uiSocket(argc, argv);
            break;
        case randomPlayerTest:
            Application::randomPlayerTest(argc, argv);
            break;
        case mctsPlayerTest:
            Application::mctsPlayerTest(argc, argv);
            break;
        case test:
            testCode(argc, argv);
            break;
    }
    clock_t end = std::clock();
    std::cout << '\n' << end - start << '\n';
}