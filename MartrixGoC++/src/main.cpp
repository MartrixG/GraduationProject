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

void testCode()
{
    GoBlock::test();
    Point::test();
}

MiniLog logger;

int main(int argc, char* argv[])
{
    ShellExecute(nullptr, "open", "cmd", "/k python python_backend.py", nullptr, SW_HIDE);
    Sleep(100);
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream fileName;
    fileName << std::put_time(std::localtime(&t), "/log/%Y-%m-%d--%H-%M-%S_log.log");
    logger.init(std::filesystem::current_path().generic_string() + fileName.str(), false);
    logger.info("Board size:" + std::to_string(BOARD_SIZE) + "x" + std::to_string(BOARD_SIZE));
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
            logger.info("Load Sgf start.");
            Application::loadSGF(argc, argv);
            break;
        case makeData:
            logger.info("Make data start.");
            Application::makeData(argc, argv);
            break;
        case commandLine:
            logger.info("Command line mode is on.");
            Application::commandLine();
            break;
        case ui:
            logger.info("UI mode in on.");
            Application::uiSocket(argc, argv);
            break;
        case randomPlayerTest:
            logger.info("Random player test.");
            Application::randomPlayerTest(argc, argv);
            break;
        case mctsPlayerTest:
            logger.info("MCTS player test.");
            Application::mctsPlayerTest(argc, argv);
            break;
        case test:
            testCode();
            break;
    }
}