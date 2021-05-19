#include <map>
#include <iostream>
#include "App.hpp"

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

void testCode(int argc, char* argv[])
{
    GoBlock::test();
    Point::test();
}

MiniLog logger;

int main(int argc, char* argv[])
{
//    clock_t start = std::clock();
    logger.init("E:/LEARNING/GraduationProject/data/out/log.txt", false);
    initArgs();
    if (argsTransform.find(argv[1]) == argsTransform.end())
    {
        logger.fatal("Can not find arg: " + std::string(argv[1]));
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
//    clock_t end = std::clock();
//    std::cout << '\n' << end - start << '\n';
}