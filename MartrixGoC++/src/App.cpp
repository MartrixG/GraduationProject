//
// Created by 11409 on 2021/4/22.
//

#include <string>
#include <iostream>
#include <fstream>
#include "Game.hpp"
#include "GoSGF.hpp"
#include "App.hpp"
#include "BoardEncode.hpp"

void Application::loadSGF(int argc, char* argv[])
{
    // read sgf file
    if(argc < 3)
    {
        std::cout << "need file name";
        return;
    }
    std::fstream fileStick(argv[2]);
    std::string fileContext;
    std::getline(fileStick, fileContext);
    // init go game
    vector_2d(Point*) allBoardPoints(BOARD_SIZE);
    Point::pointsInit(allBoardPoints);
    Game game = Game(allBoardPoints);

    GoSGF sgf(fileContext);

    if (sgf.HA != 0)
    {
        sgf.nowStep += sgf.HA;
        game.initHandCap(sgf.steps, sgf.HA);
    }
    while (sgf.haveNextStep())
    {
        Step *step = nullptr;
        sgf.getNextStep(step);
        std::cout << "No." << sgf.nowStep - sgf.HA << "(" << *step << ")\n";
        if (game.moveAnalyze(step))
        {
            game.move();
            std::cout << game;
        }
    }
}

void Application::makeData(int argc, char* argv[])
{
    // arg analyze
    if(argc < 4)
    {
        std::cout << "need file name";
        return;
    }
    // init points
    vector_2d(Point*) allBoardPoints(BOARD_SIZE);
    Point::pointsInit(allBoardPoints);
    // read sgf file
    std::fstream fileStick(argv[2]);
    std::string fileContext;
    int chosenNumberOfLine;
    chosenNumberOfLine = strtol(argv[3], new char*, 10);
    if(chosenNumberOfLine == 0)
    {
        while(std::getline(fileStick, fileContext))
        {
            gameInformationAnalyze(allBoardPoints, fileContext);
        }
    }
    while(chosenNumberOfLine != 0)
    {
        chosenNumberOfLine--;
        std::getline(fileStick, fileContext);
    }
    gameInformationAnalyze(allBoardPoints, fileContext);
}

void Application::gameInformationAnalyze(vector_2d(Point*) allBoardPoints, std::string fileContext)
{
    // init go game
    Game game = Game(allBoardPoints);
    GoSGF sgf(fileContext);
    // handCap
    if (sgf.HA != 0)
    {
        sgf.nowStep += sgf.HA;
        game.initHandCap(sgf.steps, sgf.HA);
    }
    // step by step
    while (sgf.haveNextStep())
    {
        Step *step = nullptr;
        sgf.getNextStep(step);
        if (game.moveAnalyze(step))
        {
            game.move();
            boardEncode(game, "aaa");
            //std::cout << sgf.nowStep << std::endl;
        }
    }
}

void Application::commandLine(int argc, char* argv[])
{
    // init go game
    vector_2d(Point*) allBoardPoints(BOARD_SIZE);
    Point::pointsInit(allBoardPoints);
    Game game = Game(allBoardPoints);

    int player = game.player == WHITE_PLAYER ? BLACK_PLAYER : WHITE_PLAYER;
    std::cout << "game start.\n";
    while (true)
    {
        player = player == WHITE_PLAYER ? BLACK_PLAYER : WHITE_PLAYER;
        int x, y;
        std::cin >> x >> y;
        if (x == -1 && y == -1)
        {
            break;
        }
        x--;
        y--;
        Step *nextStep = new Step(player, allBoardPoints[x][y]);
        if (game.moveAnalyze(nextStep))
        {
            game.move();
            std::cout << *game.nextStep << std::endl << game;
        } else
        {
            std::cout << "illegal location\n";
            std::cout << game;
            player = player == WHITE_PLAYER ? BLACK_PLAYER : WHITE_PLAYER;
        }
    }
}