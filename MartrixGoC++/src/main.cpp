#include <map>
#include <iostream>
#include <filesystem>
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
//    GoBlock::test();
//    Point::test();
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << std::endl;
}

MiniLog logger;

int main(int argc, char* argv[])
{
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream fileName;
    fileName << std::put_time(std::localtime(&t), "/log/%Y-%m-%d--%H-%M-%S_log.txt");
    logger.init(std::filesystem::current_path().generic_string() + fileName.str(), false);
    logger.debug("Debug mode is on.");
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
}