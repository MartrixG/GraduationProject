#include <map>
#include <iostream>
#include <ctime>
#include "App.hpp"
#include "GoBlock.hpp"

enum argsEnum
{
    loadSgf, makeData, test, commandLine, ui
};
std::map<std::string, int> argsTransform;

void initArgs()
{
    argsTransform["loadSgf"] = loadSgf;
    argsTransform["makeData"] = makeData;
    argsTransform["test"] = test;
    argsTransform["commandLine"] = commandLine;
    argsTransform["ui"] = ui;
}

void testCode(int argc, char* argv[])
{
//    GoBlock::test();
//    Point::test();
    std::fstream preLabel("E:/LEARNING/GraduationProject/data/train_data/label.txt");
    std::fstream nowLabel("E:/LEARNING/GraduationProject/data/out/label.txt");
    std::string pre, now;
    bool flag = true;
    int line = 0;
    while(flag)
    {
        if(pre != now)
        {
            std::cout << line << '\n';
            flag = false;
        }
        else
        {
            line++;
            std::getline(preLabel, pre);
            std::getline(nowLabel, now);
        }
    }
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
        case test:
            testCode(argc, argv);
            break;
    }
    clock_t end = std::clock();
    std::cout << '\n' << end - start << '\n';
}